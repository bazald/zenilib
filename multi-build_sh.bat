#!/bin/bash

@echo off
GOTO WINDOWS 



function usage {
  echo
  echo "Usage: $0 [options] [debug or release]"
  echo
  echo "  --build=all       game and all dependencies"
  echo "          mine      game only (default)"
  echo
  echo "  --macosx=10.6     Mac OS 10.6"
  echo "           10.7     Mac OS 10.7"
  echo "           10.8     Mac OS 10.8"
  echo "           native   Whatever version you happen to be running (default)"
  echo
  echo "  release is the default"
  echo
}

function usage_error {
  echo
  echo "Error: $1"
  usage
  exit $2
}

BUILD=mine
CONFIG=release
MACOSX=native

STATE=config
for arg in "$@"; do
  case "$STATE" in
    build)
      case "$arg" in
        all) BUILD=all ;;
        mine) BUILD=mine ;;
        *) usage_error "Invalid Argument '$arg'" 2
      esac
      STATE=config
      ;;
    config)
      case "$arg" in
        --build) STATE=build ;;
          --build=all) BUILD=all ;;
          --build=mine) BUILD=mine ;;
        --macosx) STATE=macosx ;;
          --macosx=10.6) MACOSX=10.6 ;;
          --macosx=10.7) MACOSX=10.7 ;;
          --macosx=10.8) MACOSX=10.8 ;;
          --macosx=native) MACOSX=native ;;
        debug) CONFIG=debug ;;
        release) CONFIG=release ;;
        releaseuniv) CONFIG=release ;;
        *) usage_error "Invalid Argument '$arg'" 3
      esac
      ;;
    macosx)
      case "$arg" in
        10.6) MACOSX=10.6 ;;
        10.7) MACOSX=10.7 ;;
        10.8) MACOSX=10.8 ;;
        native) MACOSX=native ;;
        *) usage_error "Invalid Argument '$arg'" 4
      esac
      STATE=config
      ;;
    *)
      usage_error "Invalid Argument '$arg'" 1
  esac
done
if [ "$STATE" != "config" ]; then
  usage_error "Trailing Argument" 5
fi


case $OSTYPE in
  darwin*)
    if [ "$CONFIG"=="release" ]; then
      CONFIG=releaseuniv
    fi

    echo Building: Mac OS X $BUILD $MACOSX $CONFIG

    #
    # Generate Makefiles for Mac OS X
    #

    rm -r build/gmake
    chmod +x dev/premake/premake4-macosx
    dev/premake/premake4-macosx --os=macosx --build=$BUILD --macosx=$MACOSX gmake
    if [ $? -ne 0 ]; then exit -1; fi

    mkdir -p build/macosx
    for file in build/macosx/*; do
      if [ -f "$file" ]; then rm "$file"; fi
    done

    pushd build/gmake
    for mf in *; do
      cat "$mf" | sed 's/-MF [^ ]* //' \
                | sed 's/\-arch ppc \{0,1\}//' \
                | sed 's/\-arch ppc64 \{0,1\}//' \
                > "../../build/macosx/$mf"
    done
    popd
    rm -r build/gmake

    #
    # Setup the build
    #

    CCACHE=""
    if [ "$(which ccache)" != "" ]; then
      CCACHE=ccache
    fi

    export CC="$CCACHE clang"
    export CXX="$CCACHE clang++"

    make -j 4 -C build/macosx config=$CONFIG
    if [ $? -ne 0 ]; then
      echo
      if [ "$BUILD" == "mine" ]; then
        echo "Build failed.  Retry with --build=all"
      else
        echo "Build failed."
      fi
      exit -2
    fi

    if [ "$CONFIG" != "releaseuniv" ]; then
      echo
      echo "Recompile with config=releaseuniv before distribution."
    fi
    ;;
  linux*)
    echo Building: Linux $BUILD $CONFIG

    #
    # Generate Makefiles for Linux
    #

    rm -r build/gmake
    chmod +x dev/premake/premake4-linux
    dev/premake/premake4-linux --os=linux --build=$BUILD --macosx=$MACOSX gmake
    if [ $? -ne 0 ]; then exit -1; fi

    # Migrate Makefiles to build/linux

    mkdir -p build/linux
    for file in build/linux/*; do
      if [ -f "$file" ]; then rm "$file"; fi
    done
    cp build/gmake/* build/linux/
    rm -r build/gmake

    #
    # Setup the build
    #

    if [ "$LSB_HOME" == "" ]; then
      LSB_HOME=/opt/lsb
    fi

    CCACHE=""
    if [ -x $(which ccache) ]; then
      CCACHE=ccache
    fi

    LSBCC_LIB_PREFIX=$LSB_HOME/lib-
    BIT=32
    if [ $(uname -m) == "x86_64" ]; then
      LSBCC_LIB_PREFIX=$LSB_HOME/lib64-
      BIT=64
    fi

    if [ "$CONFIG" == "" ]; then
      CONFIG=debug
    fi

    if [ "$CONFIG" == "debug" ]; then
      CONFIG_CHAR=d
    elif [ "$CONFIG" == "release" ]; then
      CONFIG_CHAR=x
    fi

    export LSBCC_LIB_PREFIX
    export LSBCC=gcc-4.4
    export LSBCXX=g++-4.4
    export LSBCC_LSBVERSION=4.0
    export LSBCC_LIBS=$LSBCC_LIB_PREFIX$LSBCC_LSBVERSION
    export LSB_SHAREDLIBPATH="$(pwd)/lib/$CONFIG_CHAR$BIT"
    export CCACHE
    export CCACHE_BASEDIR=$HOME
    export CCACHE_CC=$LSB_HOME/bin/lsbcc
    export CCACHE_CXX=$LSB_HOME/bin/lsbc++
    export CCACHE_SLOPPINESS=time_macros
    export CCACHE_COMPILERCHECK="$CCACHE_CC -v"

    if [ -x $(which $LSBCC) ] && [ -x $(which $LSBCXX) ] && [ -x "$LSB_HOME/bin/lsbcc" ] && [ -x "$LSB_HOME/bin/lsbc++" ]
    then
      COMPILING_WITH_LSB=1
      echo "Compiling for LSB"

      export CC="$CCACHE $CCACHE_CC"
      export CXX="$CCACHE $CCACHE_CXX"

      make -j 4 -C build/linux config=$CONFIG$BIT
      EXIT_CODE=$?
      if [ $EXIT_CODE -ne 0 ]; then
        echo
        if [ "$BUILD" == "mine" ]; then
          echo "Build failed.  Retry with --build=all"
        else
          echo "Build failed."
        fi
        exit -2
      fi

      if [ $EXIT_CODE -eq 0 ] && [ -x game_$CONFIG_CHAR$BIT ]
      then
        $LSB_HOME/bin/lsbappchk --no-journal --missing-symbols --lsb-version=4.0 game_$CONFIG_CHAR$BIT &> lsbappchk_full.txt
        cat lsbappchk_full.txt \
          | grep -v "WARNING" \
          | grep -v "ignoring" \
          | grep -v "not checked" \
          | grep -v "Not recognized by name" \
          | grep -v "not a compliance issue" \
          | grep -v "please rerun" \
          | grep -v "email the output" \
          | grep -v "lsb-discuss" \
          | grep -v "deprecated" \
          | grep -v "SO-Pass" \
          > lsbappchk_filtered.txt
      #   rm lsbappchk_full.txt
        cat lsbappchk_filtered.txt
      fi

      echo
      if [ "$CONFIG" == "release" ]; then echo "Distribute binaries only if no unspecified symbols were found."
      else echo "Do not distribute debug binaries."
      fi
    else
      make -j 4 -C build/linux config=$CONFIG$BIT
      if [ $? -ne 0 ]; then
        echo
        if [ "$BUILD" == "mine" ]; then
          echo "Build failed.  Retry with --build=all"
        else
          echo "Build failed."
        fi
        exit -2
      fi

      echo
      echo "Build not LSB-compliant.  Do not distribute binaries."
    fi
    ;;
  ?*)
    echo "Unknown"
    exit -2
    ;;
esac
exit



:WINDOWS 



:: Backup environment variables
IF NOT EXIST %~dp0\build MKDIR %~dp0\build
IF EXIST "%~dp0\dev\backupenv.bat" DEL "%~dp0\dev\backupenv.bat"
FOR /f "tokens=1* delims==" %%a in ('SET') DO ECHO SET %%a=%%b>> %~dp0\build\backupenv.bat

:: If logged in over Cygwin SSH, must use a password or commands fail.
SET WHOAMI=
FOR /F %%v in ('C:\Windows\System32\whoami.exe /USER') DO SET WHOAMI=%%v
FOR /F %%v in ('echo.exe %WHOAMI% ^| sed.exe "s/.*\\\\//"') DO SET WHOAMI=%%v
IF "%WHOAMI%"=="cyg_server" (
  echo Logged in as cyg_server.  Please relog in using your password.
  exit -1
)

:: If logged in over Cygwin SSH, necessary variables are unset.
CALL %~dp0\dev\VCVarsQueryRegistry.bat
IF "%AppData%"=="" SET AppData=C:\Users\%WHOAMI%\AppData


SET BUILD=mine
SET CONFIG=release32
SET MACOSX=native

SET STATE=config
:NEXTARG 
IF "%1" == "" GOTO DONEARGS 

IF "%STATE%"=="build" (
  IF "%1"=="all" (
    SET BUILD=all
  ) ELSE ( IF "%1"=="mine" (
    SET BUILD=mine
  ) ELSE (
    ECHO(
    ECHO Error: Invalid Argument '%1'
    GOTO ARGERROR 
  ))
  SET STATE=config
) ELSE ( IF "%STATE%"=="config" (
  IF "%1"=="--build" (
    SET STATE=build
  ) ELSE ( IF "%1"=="--macosx" (
    SET STATE=macosx
  ) ELSE ( IF "%1"=="debug" (
    SET CONFIG=debug
  ) ELSE ( IF "%1"=="debug32" (
    SET CONFIG=debug32
  ) ELSE ( IF "%1"=="debug64" (
    SET CONFIG=debug64
  ) ELSE ( IF "%1"=="release" (
    SET CONFIG=release
  ) ELSE ( IF "%1"=="release32" (
    SET CONFIG=release32
  ) ELSE ( IF "%1"=="release64" (
    SET CONFIG=release64
  ) ELSE (
    ECHO(
    ECHO Error: Invalid Argument '%1'
    GOTO ARGERROR 
  ))))))))
) ELSE ( IF "%STATE%"=="macosx" (
  IF "%1"=="10.6" (
    SET MACOSX=10.6
  ) ELSE ( IF "%1"=="10.7" (
    SET MACOSX=10.7
  ) ELSE ( IF "%1"=="10.8" (
    SET MACOSX=10.8
  ) ELSE ( IF "%1"=="native" (
    SET MACOSX=native
  ) ELSE (
    ECHO(
    ECHO Error: Invalid Argument '%1'
    GOTO ARGERROR 
  ))))
  SET STATE=config
) ELSE (
  ECHO(
  ECHO Error: Invalid Argument '%1'
  GOTO ARGERROR 
)))

SHIFT
GOTO NEXTARG 

:ARGERROR 

:: Restore environment variables
FOR /f "tokens=1* delims==" %%a in ('SET') DO SET %%a=
CALL %~dp0\build\backupenv.bat

ECHO(
ECHO Usage: multi-build_sh.bat [options] [debug[32/64] or release[32/64]]
ECHO(
ECHO   --build=all       game and all dependencies
ECHO           mine      game only (default)
ECHO(
ECHO   --macosx=10.6     Mac OS 10.6
ECHO            10.7     Mac OS 10.7
ECHO            10.8     Mac OS 10.8
ECHO            native   Whatever version you happen to be running (default)
ECHO(
ECHO   release32 is the default

:: Restore environment variables
FOR /f "tokens=1* delims==" %%a in ('SET') DO SET %%a=
CALL %~dp0\build\backupenv.bat

EXIT /B 1

:DONEARGS 

IF NOT "%STATE%"=="config" (
  ECHO(
  ECHO Error: Trailing Argument
  GOTO ARGERROR 
)


:: Decide build targets
IF "%CONFIG%"=="" (
  :: The default is to build release32
  SET CONFIG32=release
  SET CONFIG64=none
) ELSE ( IF "%CONFIG%"=="debug" (
  SET CONFIG32=debug
  SET CONFIG64=debug
) ELSE ( IF "%CONFIG%"=="release" (
  SET CONFIG32=release
  SET CONFIG64=release
) ELSE ( IF "%CONFIG%"=="debug32" (
  SET CONFIG32=debug
  SET CONFIG64=none
) ELSE ( IF "%CONFIG%"=="debug64" (
  SET CONFIG32=none
  SET CONFIG64=debug
) ELSE ( IF "%CONFIG%"=="release32" (
  SET CONFIG32=release
  SET CONFIG64=none
) ELSE ( IF "%CONFIG%"=="release64" (
  SET CONFIG32=none
  SET CONFIG64=release
) ELSE (
  ECHO Invalid Configuration: %CONFIG%
  EXIT /B 1
)))))))

ECHO Building: Windows %BUILD% x86:%CONFIG32% amd64:%CONFIG64%

:: Generate Visual Studio 2010 solution and projects
::IF NOT EXIST %~dp0\build\vs2010 (
  IF EXIST %~dp0\dev\premake\premake4-windows.exe (
    %~dp0\dev\premake\premake4-windows.exe --file=%~dp0\premake4.lua --os=windows --build=%BUILD% --macosx=%MACOSX% vs2010
  )
::)

:: Setup devenv
CALL "%VS100COMNTOOLS%vsvars32.bat"

IF "%CONFIG32%"=="debug" (
  MSBuild %~dp0\build\vs2010\zenilib.sln /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Debug /p:Platform=Win32 /fileLogger /fileLoggerParameters:LogFile=%~dp0\build\d32.log;Encoding=UTF-8
  IF ERRORLEVEL 1 GOTO BUILDERROR 
)
IF "%CONFIG64%"=="debug" (
  MSBuild %~dp0\build\vs2010\zenilib.sln /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Debug /p:Platform=x64 /fileLogger /fileLoggerParameters:LogFile=%~dp0\build\d64.log;Encoding=UTF-8
  IF ERRORLEVEL 1 GOTO BUILDERROR 
)
IF "%CONFIG32%"=="release" (
  MSBuild %~dp0\build\vs2010\zenilib.sln /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Release /p:Platform=Win32 /fileLogger /fileLoggerParameters:LogFile=%~dp0\build\x32.log;Encoding=UTF-8
  IF ERRORLEVEL 1 GOTO BUILDERROR 
  COPY /Y %~dp0\jni\external\bin\x32\* %~dp0\bin\x32\
)
IF "%CONFIG64%"=="release" (
  MSBuild %~dp0\build\vs2010\zenilib.sln /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Release /p:Platform=x64 /fileLogger /fileLoggerParameters:LogFile=%~dp0\build\x64.log;Encoding=UTF-8
  IF ERRORLEVEL 1 GOTO BUILDERROR 
  COPY /Y %~dp0\jni\external\bin\x64\* %~dp0\bin\x64\
)

IF "%CONFIG32%"=="debug" (
  ECHO(
  ECHO Do not distribute the 32-bit debug build.
)
IF "%CONFIG64%"=="debug" (
  ECHO(
  ECHO Do not distribute the 64-bit debug build.
)


goto RESTORE 
:BUILDERROR 

ECHO(
IF "%BUILD%"=="mine" (
  ECHO Build failed.  Retry with --build=all
) ELSE (
  ECHO Build failed.
)


:RESTORE 
:: Restore environment variables
FOR /f "tokens=1* delims==" %%a in ('SET') DO SET %%a=
CALL %~dp0\build\backupenv.bat



EXIT /B
