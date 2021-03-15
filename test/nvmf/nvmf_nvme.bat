@echo off
@setlocal enableextensions enabledelayedexpansion

findstr /L LOCAL-IP "%~dp0\nvmf_nvme.json" > nul:
if not errorlevel 1 (
	echo Replace LOCAL-IP in nvmf_nvme.json with local IP address
	goto :eof
)

set "PATH=%~dp0\..\..\..\dpdk\build\bin;%~dp0\..\..\..\wpdk\build\bin;%PATH%"
"%~dp0\..\..\..\build\bin\nvmf_tgt.exe" -c "%~dp0\nvmf_nvme.json"
