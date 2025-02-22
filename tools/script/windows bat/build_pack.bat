@echo off
::  变量延迟执行
setlocal enabledelayedexpansion
:: 获取当前路径
set batDir=%~dp0
:: 获取git仓库路径
for /f "delims=" %%i in ('git rev-parse --show-toplevel') do set gitDir=%%i
:: 当前路径
set currentDir=%cd%

set "binName=common.bin"
set "binPath=.\out"    
set "outPath=.\out2"                   
set configPath=.\versioninfo.h

: 编译时间
set hour=%time:~0,2%
if "%hour:~0,1%"==" " set hour=0%hour:~1,1%
set compileTime=%date:~2,2%%date:~5,2%%date:~8,2%%hour%

: 获取git信息
for /f "delims=" %%i in ('git rev-parse --abbrev-ref HEAD') do set GIT_BRANCH=%%i
for /f "delims=" %%i in ('git describe --tags --abbrev^=0') do set GIT_TAG=%%i
for /f "delims=" %%i in ('git log --pretty^=format:"%%h" -1') do set GIT_HASH=%%i
for /f "delims=" %%i in ('git show -s --format^=%%cd --date^=short') do set GIT_DATETIME=%%i

set fileName=%GIT_BRANCH%_%GIT_HASH%_%compileTime%

:: 删除之前生成的bin
:: 如果没有文件夹创建文件夹
if not exist "%outPath%" mkdir "%outPath%"
if exist "!outPath!\*.bin" (
    del /f /s /q "!outPath!\*.bin"
) 
:: 移动新bin到输出目录
copy "%binPath%\%binName%" "%outPath%" >nul
:: 重命名新bin
cd %outPath%
ren "%binName%" "%fileName%.bin"