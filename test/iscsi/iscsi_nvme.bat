@echo off
@setlocal enableextensions enabledelayedexpansion

set "PATH=%~dp0\..\..\..\dpdk\build\bin;%PATH%"
"%~dp0\..\..\..\build\bin\iscsi_tgt.exe" -c "%~dp0\iscsi_nvme.json"