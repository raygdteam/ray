@echo off

pushd src
..\bin\tools\Sharpmake.Application.exe /sources(@"ray.sharpmake.cs") /waitfordebugger()
pause