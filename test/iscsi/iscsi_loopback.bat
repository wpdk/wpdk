@echo off
@setlocal enableextensions enabledelayedexpansion

"%~dp0\..\..\..\build\bin\iscsi_tgt.exe" -c "%~dp0\iscsi_loopback.json"