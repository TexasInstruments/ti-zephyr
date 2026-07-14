# TI Zephyr SDK environment setup — Windows PowerShell
# Usage: . env_setup.ps1
python3 "$PSScriptRoot\env_setup.py" --shell powershell | Invoke-Expression
Write-Host "TI Zephyr SDK environment ready."
Write-Host "  ZEPHYR_BASE=$env:ZEPHYR_BASE"
Write-Host "  ZEPHYR_SDK_INSTALL_DIR=$env:ZEPHYR_SDK_INSTALL_DIR"
