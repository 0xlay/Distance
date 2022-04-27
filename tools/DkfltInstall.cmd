@ECHO OFF
TITLE DkfltInstall
RUNDLL32.EXE SETUPAPI.DLL,InstallHinfSection DefaultInstall.NT 130 %~dp0/Dkflt.inf
PAUSE