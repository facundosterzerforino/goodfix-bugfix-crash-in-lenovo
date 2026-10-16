# Goodix 27C6:550A (wbdi.dll 3.0.536.x) - turn off the driver's own S0 idle (selective suspend).
# Run elevated. Use -Rollback to undo.
#
# Why: UsbDevicePrepareHardware only calls WdfDeviceAssignS0IdleSettings when
# HKLM\SOFTWARE\Goodix\FP\EnableS0Idle != 0 (default 1). That call uses
# IdleDoNotAllowUserControl, so Device Manager / IdleInWorkingState / DeviceIdleEnabled
# have no effect. The value is read when the driver loads, so the device is restarted here.
param([switch]$Rollback)

$ErrorActionPreference = 'Stop'
$key = 'HKLM:\SOFTWARE\Goodix\FP'

if ($Rollback) {
    Remove-ItemProperty -Path $key -Name EnableS0Idle -ErrorAction SilentlyContinue
} else {
    New-Item -Path $key -Force | Out-Null
    New-ItemProperty -Path $key -Name EnableS0Idle -PropertyType DWord -Value 0 -Force | Out-Null
}

$dev = Get-PnpDevice -PresentOnly | Where-Object InstanceId -like 'USB\VID_27C6&PID_550A\*'
foreach ($d in $dev) { pnputil /restart-device "$($d.InstanceId)" | Out-Null }

# Wait past the 5 s idle timeout. With S0 idle on, an idle sensor sits in D2; with it off it stays in D0.
Start-Sleep -Seconds 20
foreach ($d in $dev) {
    $pd = (Get-PnpDeviceProperty -InstanceId $d.InstanceId -KeyName DEVPKEY_Device_PowerData).Data
    [pscustomobject]@{
        Device       = $d.InstanceId
        Status       = (Get-PnpDevice -InstanceId $d.InstanceId).Status
        EnableS0Idle = (Get-ItemProperty $key -ErrorAction SilentlyContinue).EnableS0Idle
        PowerState   = "D$($pd[4] - 1)"
    }
}
