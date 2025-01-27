@echo off
set drive=C:
set threshold=88

rem Call the PowerShell script to check free disk space
PowerShell -NoProfile -ExecutionPolicy Bypass -Command ^
    "$drive = '%drive%'; " ^
    "$threshold = '%threshold%'; " ^
    "$disk = Get-WmiObject -Class Win32_LogicalDisk -Filter \"DeviceID='$drive'\"; " ^
    "if ($disk -ne $null) { " ^
    "    $freeSpaceGB = [math]::Round($disk.FreeSpace / 1GB, 2); " ^
    "    if ($freeSpaceGB -lt $threshold) { " ^
    "        Add-Type -AssemblyName PresentationFramework; " ^
    "        [System.Windows.MessageBox]::Show(\"Warning: Free disk space on $drive is below $threshold GB.`nCurrent free space: $freeSpaceGB GB\", \"Disk Space Alert\", [System.Windows.MessageBoxButton]::OK, [System.Windows.MessageBoxImage]::Warning); " ^
    "    } else { " ^
    "        Write-Host \"Your free disk space on $drive is $freeSpaceGB GB.\"; " ^
    "    } " ^
    "} else { " ^
    "    Write-Host \"Error: Drive $drive not found.\"; " ^
    "}"