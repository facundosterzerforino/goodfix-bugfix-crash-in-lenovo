# Analysis notes

## Target

| File | Version | SHA-256 |
|---|---|---|
| `wbdi.dll` (UMDF driver) | 3.0.536.320, `DriverVer = 04/11/2024` | `792271a28c3087d8acd1626bbdc35b2190c85058bfc49af1fa00f1ea457285ec` |

On Windows 11 22H2 and later, the INF section `Biometric_Install2` loads `wbdi.dll` directly from the DriverStore (`%13%`) with `UmdfHostProcessSharing = ProcessSharingDisabled`.

## Relevant INF settings (`wbdiusb.inf`, both 3.0.536.300 and .320)

```
HKR,,"WinUsbPowerPolicyOwnershipDisabled",0x00010001,1   ; UMDF driver owns power policy
HKR,,"SystemWakeEnabled",0x00010001,1
HKR,,"DeviceIdleEnabled",0x00010001,1                    ; WinUSB setting - not used
HKR,,"DefaultIdleTimeout",0x00010001,5000                ; WinUSB setting - not used
HKR,"WDF","WdfDirectedPowerTransitionEnable",0x00010001,1
HKLM,"System\CurrentControlSet\Control\Goodix\FP",EnableRemoteWakeup,0x00010001,1
HKLM,"System\CurrentControlSet\Control\Goodix\FP",S0IdleTimeout,0x00010001,5
HKLM,"System\CurrentControlSet\Control\Goodix\FP",S3NeedTouchWakeup,0x00010001,1
```

INF changes from .300 to .320: a new `NTamd64.10.0...22621` install section (run from the DriverStore, `PnpLockdown=1`) plus renamed sections. The power settings are the same.

## Key functions (Ghidra addresses, image base 0x180000000)

| Address | Role |
|---|---|
| `0x180031e54` | Reads configuration from `HKLM\SOFTWARE\Goodix\FP` and `HKLM\SYSTEM\CurrentControlSet\Control\Goodix\FP` into a global config block |
| `0x180031c44` / `0x180031b38` | Registry helpers (`RegOpenKeyExW` + `RegQueryValueExW`; raw value data is copied, so `REG_DWORD` works) |
| `0x1800ba0d0` | `WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS_INIT` |
| `0x1800ba500` | `EnableS0Idle`, USB path: `IdleDoNotAllowUserControl`, timeout = `S0IdleTimeout * 1000` |
| `0x1800b9408` | `EnableS0Idle`, SPI path (allows user control; not used by this USB sensor) |
| call at `0x1800bb1d5` | `UsbDevicePrepareHardware`, gated on `cfg+0x430` (`EnableS0Idle`) |

Config block defaults (`.data` at `0x180244428`): `+0x430 EnableS0Idle = 1`, `+0x434 S0IdleTimeout = 5`, `+0x438 EnableRemoteWakeup = 0`.

## Reproduce

Requirements: Node.js 22.19+/24.11+, [REA](https://github.com/morluto/rea) (`npm i -g rea-agents`), Ghidra 12.1.x and JDK 21.

```powershell
$env:GHIDRA_INSTALL_DIR = "C:\tools\ghidra_12.1.4_PUBLIC"
$env:JAVA_HOME          = "C:\path\to\jdk-21"
copy C:\Windows\System32\DriverStore\FileRepository\wbdiusb.inf_amd64_*\wbdi.dll C:\work\wbdi.dll

rea trace     C:\work\wbdi.dll EnableS0Idle --provider ghidra --format json
rea decompile C:\work\wbdi.dll 0x1800ba500   --provider ghidra --format json
rea xrefs     C:\work\wbdi.dll 0x1800ba500   --provider ghidra --format json
rea read-bytes C:\work\wbdi.dll 0x180244428 --length 24 --provider ghidra --format json
```

Keep the working directory **outside Dropbox/OneDrive**. Sync clients lock REA's temporary files (`EBUSY`).
