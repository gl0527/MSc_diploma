@echo off

set out_dir=..\Build

if not exist %out_dir% ( mkdir %out_dir% )
cd %out_dir%
cmake -G "Visual Studio 14 2015" ..\Projects\

:NONE