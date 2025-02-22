:: 该脚本主要作用生成git脚本信息
@REM #ifndef __BUILD_INFO_H__ 
@REM #define __BUILD_INFO_H__ 
@REM     #define GIT_BRANCH  "main" 
@REM     #define GIT_TAG "" 
@REM     #define GIT_HASH "8aff77c" 
@REM     #define GIT_DATETIME "2025-01-07" 
@REM     #define COMPILE_DATETIME 25010709 
@REM #endif 
@echo off
::  变量延迟执行
setlocal enabledelayedexpansion

:: 获取当前路径
set batDir=%~dp0
:: 获取git仓库路径
for /f "delims=" %%i in ('git rev-parse --show-toplevel') do set gitDir=%%i

set buildFilePath=.\app_build_info.h
set outPath=!rootPath!\out\
set binPath=!rootPath!\code\firmware\application\usb_sata\csky\

: 编译时间
set hour=%time:~0,2%
if "%hour:~0,1%"==" " set hour=0%hour:~1,1%
set compileTime=%date:~2,2%%date:~5,2%%date:~8,2%%hour%

: 获取git信息
for /f "delims=" %%i in ('git rev-parse --abbrev-ref HEAD') do set GIT_BRANCH=%%i
for /f "delims=" %%i in ('git describe --tags --abbrev^=0') do set GIT_TAG=%%i
for /f "delims=" %%i in ('git log --pretty^=format:"%%h" -1') do set GIT_HASH=%%i
for /f "delims=" %%i in ('git show -s --format^=%%cd --date^=short') do set GIT_DATETIME=%%i

rem 将数据输出到文件 versioninfo.ini中
if exist !buildFilePath! del !buildFilePath!
echo #ifndef __BUILD_INFO_H__ > !buildFilePath!
echo #define __BUILD_INFO_H__ >> !buildFilePath!
echo     #define GIT_BRANCH  ^"%GIT_BRANCH%^" >> !buildFilePath!
echo     #define GIT_TAG ^"%GIT_TAG%^" >> !buildFilePath!
echo     #define GIT_HASH ^"%GIT_HASH%^" >> !buildFilePath!
echo     #define GIT_DATETIME ^"%GIT_DATETIME%^" >> !buildFilePath!
echo     #define COMPILE_DATETIME !compileTime! >> !buildFilePath!
echo #endif >> !buildFilePath!