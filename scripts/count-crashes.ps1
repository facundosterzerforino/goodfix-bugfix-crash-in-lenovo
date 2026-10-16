# Count Goodix UMDF crashes (DriverFrameworks-UserMode 10111 in the System log) per day.
# Use it to compare the crash rate before and after a change.
param([int]$Days = 14)

$since = (Get-Date).AddDays(-$Days)
Get-WinEvent -FilterHashtable @{ LogName = 'System'; Id = 10111; StartTime = $since } -ErrorAction SilentlyContinue |
    Where-Object { $_.Message -match 'Goodix' } |
    Group-Object { $_.TimeCreated.ToString('yyyy-MM-dd') } |
    Sort-Object Name |
    Select-Object @{ n = 'Day'; e = { $_.Name } }, Count
