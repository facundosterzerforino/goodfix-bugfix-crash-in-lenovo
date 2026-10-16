# Goodix fingerprint reader crash (HostTimeout) on Lenovo ThinkBook: analysis and workaround

The Goodix fingerprint reader `USB\VID_27C6&PID_550A` (in the power button) on the Lenovo ThinkBook 16 G6 IRL (21KH) goes offline a few times a day on Windows 11. This repo documents what the driver does with power management, found by reverse engineering, and a registry workaround that does not modify the signed driver.

> **Status:** the root cause analysis is done. The workaround is **not yet validated**: the crash-rate test is still running. Results will be added here.

## Symptom

- System log: `DriverFrameworks-UserMode` events **10111** ("device is offline due to a user-mode driver crash") and 10120.
- WER signature, the same every time: `WUDFHostProblem2`, P2 `HostTimeout`, P8 `0xC00000B5` (`STATUS_IO_TIMEOUT`), P9 `wbdi.dll`.
  This is not an access violation. A request the driver was handling did not complete in time, so the UMDF host was killed.
- About 2–3 crashes per day, at any hour, sometimes in bursts. Most of them are not near Modern Standby entry or exit.
- Driver: Goodix 3.0.536.320 (2024-04-10), UMDF 2, `wbdiusb.inf`. Reinstalling it does not help.

## What does *not* work, and why

The usual advice is to turn off USB selective suspend for the device:

- Device Manager → Power Management → "Allow the computer to turn off this device" (this is `Device Parameters\WDF\IdleInWorkingState`).
- `Device Parameters\DeviceIdleEnabled = 0`, `DefaultIdleTimeout`, and similar values.

**None of these have any effect on this driver:**

1. The driver assigns its idle policy with `UserControlOfIdleSettings = IdleDoNotAllowUserControl` (see [`usb_enable_s0idle.c`](analysis/pseudocode/usb_enable_s0idle.c)). WDF therefore ignores the user/Device Manager setting.
2. `DeviceIdleEnabled` and `DefaultIdleTimeout` are **WinUSB** power-policy settings. The INF sets `WinUsbPowerPolicyOwnershipDisabled = 1`, so the UMDF driver owns power policy, not WinUSB.

You can check this on an affected machine: with those values set, the idle sensor still reports power state **D2** (selectively suspended):

```powershell
$id = (Get-PnpDevice -PresentOnly | Where-Object InstanceId -like 'USB\VID_27C6&PID_550A\*').InstanceId
$b  = (Get-PnpDeviceProperty -InstanceId $id -KeyName DEVPKEY_Device_PowerData).Data
"D$($b[4]-1)"   # D0 = powered, D2 = suspended
```

## What the driver actually does

Reverse engineering of `wbdi.dll` 3.0.536.320 (REA + Ghidra; excerpts in [`analysis/pseudocode`](analysis/pseudocode)):

- At start-up the driver reads its own configuration from the registry ([`config_reader_excerpt.c`](analysis/pseudocode/config_reader_excerpt.c)):

  | Value | Key | Default in binary | Meaning |
  |---|---|---|---|
  | `EnableS0Idle` | `HKLM\SOFTWARE\Goodix\FP` | **1** | Assign an S0 idle (selective suspend) policy at all |
  | `S0IdleTimeout` | `HKLM\SYSTEM\CurrentControlSet\Control\Goodix\FP` | 5 (INF also sets 5) | Idle timeout in seconds |
  | `EnableRemoteWakeup` | same as above | 0 (INF sets 1) | Wake support |
  | `LogLevel`, `LogTarget`, `LogPath` | `HKLM\SOFTWARE\Goodix\FP\LogOutput` | – | Driver's own debug log |

- `UsbDevicePrepareHardware` only calls `EnableS0Idle()` → `WdfDeviceAssignS0IdleSettings` when `EnableS0Idle != 0`. Otherwise it logs `"s0 idle is disabled"` ([`usb_prepare_hardware_gate.c`](analysis/pseudocode/usb_prepare_hardware_gate.c)). If no S0 idle policy is assigned, WDF keeps the device in D0.
- With the default 5 s timeout the sensor cycles D0 → D2 → D0 many times an hour.

**Hypothesis (not proven yet):** one of these frequent power transitions occasionally hangs, for example while waiting on the sensor MCU after resume. The binary has messages such as `Wait pipe restart timeout`, `D0Exit time`, and MCU power-lost/reset handling. The reflector then times out and kills the host (`HostTimeout`, `STATUS_IO_TIMEOUT`). Turning off the driver's own S0 idle should remove most of those transitions.

## Workaround

Set one DWORD and restart the device. The value is read when the driver loads.

```powershell
# elevated
.\scripts\apply-disable-s0idle.ps1            # sets HKLM\SOFTWARE\Goodix\FP\EnableS0Idle = 0, restarts the sensor
.\scripts\apply-disable-s0idle.ps1 -Rollback  # removes the value again
```

After about 20 s the script should report `PowerState = D0`.

Things to check before rolling it out widely:

- **Crash rate:** run `scripts\count-crashes.ps1` before and after (several days).
- **Standby battery drain:** compare `powercfg /sleepstudy` before and after. A sensor that stays in D0 might keep the USB controller out of its lowest power state during Modern Standby.
- **Middle option** if drain gets worse: keep `EnableS0Idle = 1` and raise `S0IdleTimeout` (for example to `300`) under `HKLM\SYSTEM\CurrentControlSet\Control\Goodix\FP`. This reduces the number of transitions a lot.
- **Driver updates:** the INF writes only the `SYSTEM\...\Control\Goodix\FP` key, so `HKLM\SOFTWARE\Goodix\FP\EnableS0Idle` should survive a reinstall. Check again after any Goodix driver update.

Because it is a single HKLM value, it can be deployed with GPO Preferences or Intune. A reboot or a device restart is needed afterwards.

## Other notes

- The older driver 3.0.536.300 (2023-06-05) contains the same device logic; the differences are WDF stub and runtime changes. Rolling back is unlikely to help.
- Debug strings in the binary point to a ThinkPad code base (`thinkpad_gm168\winfpcode2\driver_tatin`) reused for this sensor.
- Analysis notes, the INF diff and how to reproduce: [`analysis/README.md`](analysis/README.md).

## Disclaimer

Not affiliated with Goodix or Lenovo. No driver files are included or modified. Use the scripts at your own risk and test on one machine first.
