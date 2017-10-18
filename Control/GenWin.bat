@echo off

if "%1"=="" (
	echo Build type parameter not found!
	goto NONE
)

set build_type=%1

if "%build_type%" == "Debug" (
	set out_dir=..\Build
	
	if not exist %out_dir% (mkdir %out_dir%)
	cd %out_dir%
	cmake -DCMAKE_BUILD_TYPE="%build_type%" -DCMAKE_MODULE_PATH="e:/dev/tools/OGRE-SDK-1.9.0-vc140-x86-12.03.2016/CMake/" -G "Visual Studio 14 2015" ..\Projects\
)

:NONE