@ECHO OFF
TITLE DkfltUninstall
RUNDLL32.EXE SETUPAPI.DLL,InstallHinfSection DefaultUninstall.NT 130 %~dp0/Dkflt.inf
PAUSE