@echo off

pushd %~dp0
setlocal

set ThirdPartyDir=%~dp0\third_party
set PythonExe=%ThirdPartyDir%\pico-dist\python\python.exe
call %PythonExe% -m build_tool %*

endlocal
popd
