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
  echo "  --dir=DIR         arbitrary build directory"
  echo "        build       (default)"
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
DIR=build
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
        --dir) STATE=dir ;;
          --dir=*) DIR=$(echo "$arg" | sed 's/--dir=//') ;;
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
    dir)
      DIR="$arg"
      STATE=config
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

pushd $(echo $0 | sed 's/\/[^\/]*$//')
if [ $? -ne 0 ]; then exit -4; fi

case $OSTYPE in
  darwin*)
    if [ "$CONFIG" == "release" ]; then
      CONFIG=releaseuniv
    fi

    echo Building: Mac OS X $BUILD $MACOSX $CONFIG

    #
    # Generate Makefiles for Mac OS X
    #

    for dir in $(ls -d "$DIR/gmake" 2> /dev/null); do rm -r $dir; done
    chmod +x dev/premake/premake4-macosx
    dev/premake/premake4-macosx --os=macosx --build=$BUILD --dir="$DIR" --macosx=$MACOSX gmake
    if [ $? -ne 0 ]; then
      popd
      exit -1
    fi

    mkdir -p "$DIR/macosx"
    for file in $(ls -d "$DIR/macosx/*" 2> /dev/null); do
      if [ -f "$file" ]; then rm "$file"; fi
    done

    for mf in $(ls "$DIR/gmake/" 2> /dev/null); do
      cat "$DIR/gmake/$mf" | sed 's/-MF [^ ]* //' \
                           | sed 's/\-arch ppc \{0,1\}//' \
                           | sed 's/\-arch ppc64 \{0,1\}//' \
                           > "$DIR/macosx/$mf"
    done
    rm -r "$DIR/gmake"

    #
    # Setup the build
    #

    CCACHE=$(which ccache)
    if [ ! -x $CCACHE ]; then
      CCACHE=""
    fi

    export CC="$CCACHE clang"
    export CXX="$CCACHE clang++"

    make -j 4 -C "$DIR/macosx" config=$CONFIG
    if [ $? -ne 0 ]; then
      echo
      if [ "$BUILD" == "mine" ]; then
        echo "Build failed.  Retry with --build=all"
      else
        echo "Build failed."
      fi
      popd
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

    for dir in $(ls -d "$DIR/gmake" 2> /dev/null); do rm -r $dir; done
    chmod +x dev/premake/premake4-$(arch)
    dev/premake/premake4-$(arch) --os=linux --build=$BUILD --dir="$DIR" --macosx=$MACOSX gmake
    if [ $? -ne 0 ]; then
      popd
      exit -1
    fi

    # Migrate Makefiles to build/linux

    mkdir -p "$DIR/linux"
    for file in $(ls -d "$DIR/linux/*" 2> /dev/null); do
      if [ -f "$file" ]; then rm "$file"; fi
    done

    for mf in $(ls "$DIR/gmake/" 2> /dev/null); do
      cat "$DIR/gmake/$mf" | sed 's/ -rcs / -rso /' \
                           | sed 's/CC = gcc//' \
                           | sed 's/CXX = g++//' \
                           | sed -n 'h; p; {g; s/LDDEPS    +=/LIBS      +=/; t keep_going; d; :keep_going; s/\.\.\/\.\.\/lib\/...\/lib/-l/g; s/\.so//g; s/\.a//g; p}' \
                           | sed 's/+= \$(LDDEPS)/+=/' \
                          > "$DIR/linux/$mf"
    done
    rm -r "$DIR/gmake"

    #
    # Setup the build
    #

    if [ "$CONFIG" == "" ]; then
      CONFIG=debug
    fi

    if [ "$CONFIG" == "debug" ]; then
      CONFIG_CHAR=d
    elif [ "$CONFIG" == "release" ]; then
      CONFIG_CHAR=x
    fi

    if [ "$LSB_HOME" == "" ]; then
      LSB_HOME=/opt/lsb
    fi

    CCACHE=$(which ccache)
    if [ ! -x $CCACHE ]; then
      CCACHE=""
    fi

    LSBCC_LIB_PREFIX=$LSB_HOME/lib-
    BIT=32
    if [ $(uname -m) == "x86_64" ]; then
      LSBCC_LIB_PREFIX=$LSB_HOME/lib64-
      BIT=64
    fi

    if [ "$COMPILE_FOR_LSB" == "" ]; then
      COMPILE_FOR_LSB=1
    fi

    if [ "$CC" == "" ]; then CC=gcc; fi
    if [ "$CXX" == "" ]; then CXX=g++; fi
    VERCC=( $($CC --version \
            | grep -o '[0-9]\{1,\}\.[0-9]\{1,\}\.[0-9]\{1,\}' \
            | head -n 1 \
            | sed 's/\([0-9]\{1,\}\)\.\([0-9]\{1,\}\)\.\([0-9]\{1,\}\)/\1 \2 \3/') )
    VERCXX=( $($CXX --version \
             | grep -o '[0-9]\{1,\}\.[0-9]\{1,\}\.[0-9]\{1,\}' \
             | head -n 1 \
             | sed 's/\([0-9]\{1,\}\)\.\([0-9]\{1,\}\)\.\([0-9]\{1,\}\)/\1 \2 \3/') )
    echo "Detected gcc version ${VERCC[0]}.${VERCC[1]}.${VERCC[2]}"
    echo "Detected g++ version ${VERCXX[0]}.${VERCXX[1]}.${VERCXX[2]}"

    if [ ! -x "$LSB_HOME/bin/lsbcc" ] || [ ! -x "$LSB_HOME/bin/lsbc++" ]; then
      COMPILE_FOR_LSB=0
      echo "LSB compilation toolchain not found."
    fi

    if [ $COMPILE_FOR_LSB -ne 0 ] && [ ${VERCC[0]} -gt 4 -o ${VERCXX[0]} -gt 4 -o ${VERCC[1]} -gt 4 -o ${VERCXX[1]} -gt 4 ]
    then
      echo "gcc/g++ 4.5-4.8 require ld.gold for LSB compilation."

      GOLD_LD=$(echo $(whereis -b gold-ld) | sed 's/.* //')
      if [ -x "$GOLD_LD/ld" ]; then
        MAX_MINOR=8
        echo "ld.gold found: $GOLD_LD"
      else
        MAX_MINOR=4
        echo "gold-ld could not be found, but is required for LSB build with GCC 4.5-4.8."
        echo "Notes: gold-ld is merely a directory on the path (e.g. /usr/lib/gold-ld)"
        echo "       gold-ld must contain 'ld', a symlink to ld.gold or gold"
      fi
    fi

    if [ $COMPILE_FOR_LSB -ne 0 ] && [ ${VERCC[0]} -gt 4 -o ${VERCXX[0]} -gt 4 -o ${VERCC[1]} -gt $MAX_MINOR -o ${VERCXX[1]} -gt $MAX_MINOR ]
    then
      COMPILE_FOR_LSB=0
      for minor in $(seq $MAX_MINOR -1 0); do
        TESTCC=$(which "gcc-4.$minor")
        TESTCXX=$(which "g++-4.$minor")
        if [ -x $TESTCC -a -x $TESTCXX ]; then
          COMPILE_FOR_LSB=1
          CC=$TESTCC
          CXX=$TESTCXX
          VERCC[0]=4
          VERCXX[0]=4
          VERCC[1]=$MAX_MINOR
          VERCXX[1]=$MAX_MINOR
          break
        fi
      done

      if [ $COMPILE_FOR_LSB -eq 0 ]; then
        echo "No version of gcc/g++ usable for LSB compilation found."
      fi
    fi

    export LSBCC_LIB_PREFIX
    export LSBCC=$CC
    export LSBCXX=$CXX
    export LSBCC_LSBVERSION=4.0
    export LSBCC_LIBS=$LSBCC_LIB_PREFIX$LSBCC_LSBVERSION
    export LSB_SHAREDLIBPATH="$(pwd)/lib/$CONFIG_CHAR$BIT"
    export CCACHE
    export CCACHE_BASEDIR=$HOME
    export CCACHE_CC=$LSB_HOME/bin/lsbcc
    export CCACHE_CXX=$LSB_HOME/bin/lsbc++
    export CCACHE_SLOPPINESS=time_macros
    export CCACHE_COMPILERCHECK="$CCACHE_CC -v"

    if [ $COMPILE_FOR_LSB -ne 0 ]; then
      echo "Compiling for the LSB with $CC and $CXX"

      export CC="$CCACHE $CCACHE_CC"
      export CXX="$CCACHE $CCACHE_CXX"

      if [ ${VERCC[0]} -gt 4 -o ${VERCXX[0]} -gt 4 -o ${VERCC[1]} -gt 4 -o ${VERCXX[1]} -gt 4 ]; then
        echo "Using ld.gold: $GOLD_LD"
        export LDFLAGS="-B$GOLD_LD -Wl,--no-gnu-unique"
      fi

      make -j 4 -C "$DIR/linux" config=$CONFIG$BIT
      EXIT_CODE=$?
      if [ $EXIT_CODE -ne 0 ]; then
        echo
        if [ "$BUILD" == "mine" ]; then
          echo "Build failed.  Retry with --build=all"
        else
          echo "Build failed."
        fi
        popd
        exit -2
      fi

      if [ $EXIT_CODE -eq 0 ] && [ -x game_$CONFIG_CHAR$BIT ]; then
        $LSB_HOME/bin/lsbappchk --no-journal --missing-symbols --lsb-version=$LSBCC_LSBVERSION game_$CONFIG_CHAR$BIT &> lsbappchk_full.txt
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
      echo "Not compiling for the LSB."

      make -j 4 -C "$DIR/linux" config=$CONFIG$BIT
      if [ $? -ne 0 ]; then
        echo
        if [ "$BUILD" == "mine" ]; then
          echo "Build failed.  Retry with --build=all"
        else
          echo "Build failed."
        fi
        popd
        exit -2
      fi

      echo
      echo "Build not LSB-compliant.  Do not distribute binaries."
    fi
    ;;
  ?*)
    echo "Unknown"
    popd
    exit -3
    ;;
esac

popd
exit



:WINDOWS 



SET DP0=%~dp0
SET BUILD=mine
SET CONFIG=release32
SET DIR=build
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
  ) ELSE ( IF "%1"=="--dir" (
    SET STATE=dir
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
  )))))))))
) ELSE ( IF "%STATE%"=="dir" (
  SET DIR=%1
  SET STATE=config
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
))))

SHIFT
GOTO NEXTARG 

:ARGERROR 

ECHO(
ECHO Usage: multi-build_sh.bat [options] [debug[32/64] or release[32/64]]
ECHO(
ECHO   --build=all       game and all dependencies
ECHO           mine      game only (default)
ECHO(
ECHO   --dir=DIR         arbitrary build directory
echo         build       (default)
ECHO(
ECHO   --macosx=10.6     Mac OS 10.6
ECHO            10.7     Mac OS 10.7
ECHO            10.8     Mac OS 10.8
ECHO            native   Whatever version you happen to be running (default)
ECHO(
ECHO   release32 is the default

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


:: Backup environment variables
IF NOT EXIST "%DIR%" MKDIR "%DIR%"
IF EXIST "%DIR%\backupenv.bat" DEL "%DIR%\backupenv.bat"
FOR /f "tokens=1* delims==" %%a in ('SET') DO ECHO SET %%a=%%b>> "%DIR%\backupenv.bat"

:: If logged in over Cygwin SSH, must use a password or commands fail.
SET WHOAMI=
FOR /F %%v in ('C:\Windows\System32\whoami.exe /USER') DO SET WHOAMI=%%v
FOR /F %%v in ('echo.exe %WHOAMI% ^| sed.exe "s/.*\\\\//"') DO SET WHOAMI=%%v
IF "%WHOAMI%"=="cyg_server" (
  echo Logged in as cyg_server.  Please relog in using your password.
  exit -1
)

:: If logged in over Cygwin SSH, necessary variables are unset.
CALL "%DP0%\dev\VCVarsQueryRegistry.bat"
IF "%AppData%"=="" SET AppData=C:\Users\%WHOAMI%\AppData

:: Setup devenv
IF EXIST "%VS110COMNTOOLS%" (
  CALL "%VS110COMNTOOLS%vsvars32.bat"
  SET VSVER=vs2012
) ELSE ( IF EXIST "%VS100COMNTOOLS%" (
  CALL "%VS100COMNTOOLS%vsvars32.bat"
  SET VSVER=vs2010
) ELSE (
  ECHO "Neither Visual Studio 2012 nor 2010 detected."
  EXIT /B
))

ECHO %VSVER% building: Windows %BUILD% x86:%CONFIG32% amd64:%CONFIG64%

:: Generate Visual Studio solution and projects
::IF NOT EXIST "%DIR%\%VSVER%" (
  IF EXIST "%DP0%\dev\premake\premake4-windows.exe" (
    DEL /Q "%DIR%\%VSVER%\*.filters" "%DIR%\%VSVER%\*.user" "%DIR%\%VSVER%\*.vcxproj"

    "%DP0%\dev\premake\premake4-windows.exe" --file="%DP0%\premake4.lua" --os=windows --build=%BUILD% --dir=%DIR% --macosx=%MACOSX% %VSVER%
  )
::)

SET EL=0
IF %EL%==0 IF "%CONFIG32%"=="debug" (
  MSBuild "%DIR%\%VSVER%\zenilib.sln" /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Debug /p:Platform=Win32 /fileLogger /fileLoggerParameters:LogFile="%DIR%\d32.log";Encoding=UTF-8
  IF ERRORLEVEL 1 SET EL=1
)
IF %EL%==0 IF "%CONFIG64%"=="debug" (
  MSBuild "%DIR%\%VSVER%\zenilib.sln" /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Debug /p:Platform=x64 /fileLogger /fileLoggerParameters:LogFile="%DIR%\d64.log";Encoding=UTF-8
  IF ERRORLEVEL 1 SET EL=1
)
IF %EL%==0 IF "%CONFIG32%"=="release" (
  MSBuild "%DIR%\%VSVER%\zenilib.sln" /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Release /p:Platform=Win32 /fileLogger /fileLoggerParameters:LogFile="%DIR%\x32.log";Encoding=UTF-8
  IF ERRORLEVEL 1 SET EL=1
)
IF %EL%==0 IF "%CONFIG64%"=="release" (
  MSBuild "%DIR%\%VSVER%\zenilib.sln" /m /p:MultiProcessorCompilation=true /t:Build /p:Configuration=Release /p:Platform=x64 /fileLogger /fileLoggerParameters:LogFile="%DIR%\x64.log";Encoding=UTF-8
  IF ERRORLEVEL 1 SET EL=1
)

IF NOT %EL%==0 (
  ECHO(
  IF "%BUILD%"=="mine" (
    ECHO Build failed.  Retry with --build=all
  ) ELSE (
    ECHO Build failed.
  )
) ELSE (
  IF "%CONFIG32%"=="debug" (
    ECHO(
    ECHO Do not distribute the 32-bit debug build.
  )
  IF "%CONFIG64%"=="debug" (
    ECHO(
    ECHO Do not distribute the 64-bit debug build.
  )
)

:: Restore environment variables
FOR /f "tokens=1* delims==" %%a in ('SET') DO (
  IF NOT "%%a"=="DIR" (
    SET %%a=
  )
)
CALL "%DIR%\backupenv.bat"

EXIT /B
