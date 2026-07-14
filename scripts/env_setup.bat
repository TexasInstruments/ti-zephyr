@echo off
REM TI Zephyr SDK environment setup -- Windows CMD
REM Usage: env_setup.bat (or CALL env_setup.bat from another script)
FOR /F "usebackq tokens=*" %%i IN (`python3 "%~dp0env_setup.py" --shell cmd`) DO %%i
echo TI Zephyr SDK environment ready.
echo   ZEPHYR_BASE=%ZEPHYR_BASE%
echo   ZEPHYR_SDK_INSTALL_DIR=%ZEPHYR_SDK_INSTALL_DIR%
