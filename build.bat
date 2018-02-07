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

if not defined INCLUDE goto no_dev

rem Build Templight
if defined no_templight goto skip_templight
  cd 3rd\templight
    md build
    cd build
      cmake ..\llvm
      msbuild LLVM.sln /p:Configuration=Release /p:Platform="Win32" "/t:Clang executables\templight"
      if errorlevel 1 goto no_dev
    cd ..
  cd ..\..
  goto build_metashell
:skip_templight
  echo Skipping Templight build, because %%NO_TEMPLIGHT%% = "%NO_TEMPLIGHT%"
:build_metashell

rem Build Metashell
md bin
cd bin
  if defined METASHELL_NO_DOC_GENERATION cmake .. -DMETASHELL_NO_DOC_GENERATION=1
  if not defined METASHELL_NO_DOC_GENERATION cmake ..
  msbuild metashell.sln /p:Configuration=Release /p:Platform="Win32"
  if errorlevel 1 goto no_dev

  rem Run tests
  ctest -C Release
  if errorlevel 1 goto test_failed

  rem Create installer
  if defined no_installer goto skip_cpack
    cpack
    goto after_cpack
  :skip_cpack
    echo Skipping installer generation, because %%NO_INSTALLER%% = "%NO_INSTALLER%"
  :after_cpack
cd ..

exit /B 0

:no_dev
echo "Please run this script in a Visual Studio Developer Command Prompt"
exit /B 1

:test_failed
echo "Tests failed"
type Testing\Temporary\LastTest.log
exit /B 1
