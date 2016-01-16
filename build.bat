@echo off

rem Metashell - Interactive C++ template metaprogramming shell
rem Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
rem
rem This program is free software: you can redistribute it and/or modify
rem it under the terms of the GNU General Public License as published by
rem the Free Software Foundation, either version 3 of the License, or
rem (at your option) any later version.
rem
rem This program is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem GNU General Public License for more details.
rem
rem You should have received a copy of the GNU General Public License
rem along with this program.  If not, see <http://www.gnu.org/licenses/>.

rem Build Templight
cd 3rd\templight
  md build
  cd build
    cmake ..\llvm
    msbuild LLVM.sln /p:Configuration=Release "/t:Clang executables\templight"
    if errorlevel 1 goto no_dev
  cd ..
cd ..\..

rem Build Metashell
md bin
cd bin
  cmake ..
  msbuild metashell.sln /p:Configuration=Release
  if errorlevel 1 goto no_dev

  rem Run tests
  test\unit\Release\metashell_unit_test.exe
  if errorlevel 1 goto test_failed



  test\system\app\core\Release\metashell_core_system_test.exe ^
    app\Release\metashell.exe ^
    -- ^
    -I%cd%\..\3rd\boost\include ^
    --
  if errorlevel 1 goto test_failed

  test\system\app\core\Release\metashell_core_system_test.exe ^
    app\Release\metashell.exe ^
    --engine clang ^
    -- ^
    %cd%\app\Release\templight\templight.exe ^
    -std=c++0x ^
    -ftemplate-depth=256 ^
    -Wfatal-errors ^
    -fno-ms-compatibility ^
    -U_MSC_VER ^
    -I%cd%\app\Release\windows_headers ^
    -I%cd%\app\Release\windows_headers\mingw32 ^
    -I%cd%\app\Release\templight\include ^
    -I%cd%\..\3rd\boost\include ^
    --
  if errorlevel 1 goto test_failed



  test\system\app\pp\Release\metashell_pp_system_test.exe ^
    app\Release\metashell.exe ^
    -- ^
    -I%cd%\..\3rd\boost\include ^
    --
  if errorlevel 1 goto test_failed

  test\system\app\pp\Release\metashell_pp_system_test.exe ^
    app\Release\metashell.exe ^
    --engine clang ^
    -- ^
    %cd%\app\Release\templight\templight.exe ^
    -std=c++0x ^
    -ftemplate-depth=256 ^
    -Wfatal-errors ^
    -fno-ms-compatibility ^
    -U_MSC_VER ^
    -I%cd%\app\Release\windows_headers ^
    -I%cd%\app\Release\windows_headers\mingw32 ^
    -I%cd%\app\Release\templight\include ^
    -I%cd%\..\3rd\boost\include ^
    --
  if errorlevel 1 goto test_failed



  test\system\app\mdb\Release\metashell_mdb_system_test.exe ^
    app\Release\metashell.exe ^
    -- ^
    -I%cd%\..\3rd\boost\include ^
    --
  if errorlevel 1 goto test_failed

  test\system\app\mdb\Release\metashell_mdb_system_test.exe ^
    app\Release\metashell.exe ^
    --engine clang ^
    -- ^
    %cd%\app\Release\templight\templight.exe ^
    -std=c++0x ^
    -ftemplate-depth=256 ^
    -Wfatal-errors ^
    -fno-ms-compatibility ^
    -U_MSC_VER ^
    -I%cd%\app\Release\windows_headers ^
    -I%cd%\app\Release\windows_headers\mingw32 ^
    -I%cd%\app\Release\templight\include ^
    -I%cd%\..\3rd\boost\include ^
    --
  if errorlevel 1 goto test_failed

  rem Create installer
  cpack
cd ..

exit /B 0

:no_dev
echo "Please run this script in a Visual Studio Developer Command Prompt"
exit /B 1

:test_failed
echo "Tests failed"
exit /B 1

