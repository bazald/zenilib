import math, sys, os.path, glob, string, re, os

is_windows = sys.platform == 'win32'
is_win64   = is_windows and (os.environ['PROCESSOR_ARCHITECTURE'] == 'AMD64' or (os.environ.has_key('PROCESSOR_ARCHITEW6432') and os.environ['PROCESSOR_ARCHITEW6432'] == 'AMD64'))
is_linux   = sys.platform == 'linux2'
is_mac     = sys.platform == 'darwin'

### Decide architecture

if is_win64:
  if int(ARGUMENTS.get('x64', 0)):
    cc = ' "' + os.environ['VSINSTALLDIR'] + '\\VC\\BIN\\amd64\\cl.exe" '
    lib = ' "' + os.environ['VSINSTALLDIR'] + '\\VC\\BIN\\amd64\\lib.exe" '
    link = '"' + os.environ['VSINSTALLDIR'] + '\\VC\\BIN\\vcvars64.bat"' + ' && ' + ' "' + os.environ['VSINSTALLDIR'] + '\\VC\\BIN\\amd64\\link.exe" '
  else:
    cc = ' "' + os.environ['VSINSTALLDIR'] + '\\VC\\BIN\\cl.exe" '
    lib = ' "' + os.environ['VSINSTALLDIR'] + '\\VC\\BIN\\lib.exe" '
    link = ' "' + os.environ['VSINSTALLDIR'] + '\\VC\\BIN\\link.exe" '
    is_win64 = 0
elif is_linux or is_max:
  cc = ' g++ '
  link = ' ld '

### Decide single compilation unit

program_scu = 'src/application.cxx'
library_scu = 'src/zenilib.cxx'

launcher = ['Visual Studio 2008/Launcher.cpp']
program = [Glob('src/*.cpp')]
library = [Glob('src/Zeni/*.cpp')]
tinyxml = [Glob('src/TinyXML/*.cpp')]

scu = ARGUMENTS.get('scu', 0)
if scu == 'zeni' or scu == 'Zeni' or scu == 'ZENI':
  scu = 1
  library = library_scu
elif scu == 'scu' or scu == 'Scu' or scu == 'SCU':
  scu = 2
  program = program_scu
  library = library_scu
else:
  scu = 0

### Decide libraries

disable_al = ' -DDISABLE_AL '
disable_dx9 = ' -DDISABLE_DX9 '
disable_gl = ' -DDISABLE_GL '
disable_wgl = ' -DDISABLE_WGL '

if is_windows:
  if is_win64:
    defines = ' /D "WIN64" /D "_WINDOWS" /D "X64" '
  else:
    defines = ' /D "WIN32" /D "_WINDOWS" '
else:
  defines = ' -D_CRT_SECURE_NO_DEPRECATE ' + disable_dx9 + disable_wgl

libs = ['SDL', 'SDLmain', 'SDL_image', 'SDL_gfx', 'SDL_ttf', 'SDL_net', 'Cg']
if is_windows:
  libs += ['lib3ds-2_0', 'Advapi32', 'SHFolder', 'Shell32', 'User32']
else:
  libs += ['lib3ds']
if is_windows:
  al_libs = ['OpenAL32', 'alut', 'libvorbis', 'libvorbisfile']
else:
  al_libs = ['openal', 'alut', 'libvorbis', 'vorbisfile']
dx9_libs = ['d3d9', 'd3dx9', 'gdi32', 'CgD3D9']
if is_windows:
  gl_libs = ['opengl32', 'glew32', 'glu32', 'CgGL']
else:
  gl_libs = ['GL', 'GLEW', 'GLU', 'CgGL']

noal = ARGUMENTS.get('noal', 0)
if int(noal):
  defines += disable_al
else:
  libs += al_libs

if is_windows:
  nodx9 = ARGUMENTS.get('nodx9', 0)
  if int(nodx9):
    defines += disable_dx9
  else:
    libs += dx9_libs

nogl = ARGUMENTS.get('nogl', 0)
if is_windows and int(nogl):
  defines += disable_gl
else:
  libs += gl_libs

if is_windows:
  nowgl = ARGUMENTS.get('nowgl', 0)
  if int(nowgl):
    defines += disable_wgl

use_soar = int(ARGUMENTS.get('soar', 0))

### Decide pedantism

warnings = ' -Wall -Wno-variadic-macros '
pedantic_warnings = ' -pedantic '

pedantic = ARGUMENTS.get('pedantic', 0)
if int(pedantic):
  warnings += pedantic_warnings

### Decide optimization

launcher_name = 'Launcher'
if is_windows:
  program_name = 'Application'
else:
  program_name = 'application'
library_name = 'zenilib'
tinyxml_name = 'tinyxml'

if is_windows:
  debug_optimization =   ' /Od              /Gy     /D "_DEBUG"     /RTC1 /MDd /ZI '
  release_optimization = ' /Ob2 /Oi /Ot /Oy /GT /GL /D "NDEBUG"           /MD  /Zi '
else:
  debug_optimization = ' -ggdb '
  release_optimization = ' -O2 -fexpensive-optimizations -ffast-math -funroll-loops '

debug = ARGUMENTS.get('debug', 0)
if int(debug):
  if is_win64:
    launcher_name += '_x64d'
    program_name += '_x64d'
    library_name += '_x64d'
  else:
    launcher_name += '_d'
    program_name += '_d'
    library_name += '_d'
  if is_windows:
    link_optimization = ' /DEBUG /ASSEMBLYDEBUG /MAP /MAPINFO:EXPORTS /NODEFAULTLIB:msvcrt '
  optimization = debug_optimization;
else:
  if is_windows:
    link_optimization = ' /INCREMENTAL:NO /OPT:REF /OPT:ICF /LTCG '
    if is_win64:
      launcher_name += '_x64'
      program_name += '_x64'
      library_name += '_x64'
  optimization = release_optimization;

### Decide profiling

profiling = ' -pg '

enable_gprof = ARGUMENTS.get('profile', 0)
if not int(enable_gprof):
  profiling = ''

### Tune

arch = ' -march=native -mfpmath=sse -fomit-frame-pointer '

tune = ARGUMENTS.get('tune', 0)
if not int(tune):
  arch = ''

### Finally define the environment + library + program

cpppath = ['include']
libpath = ['.']

if is_windows:
  ccflags = defines + ' /FD /EHsc /fp:fast /W4 /nologo /c /TP /wd4005 /wd4312 /wd4505 /wd4311 /errorReport:prompt ' + optimization
  linkflags = '/NOLOGO /SUBSYSTEM:WINDOWS /DYNAMICBASE:NO /ERRORREPORT:PROMPT' + link_optimization
  cpppath += ['include_win']
  cpppath += [os.environ['PROGRAMFILES(X86)'] + '\\OpenAL 1.1 SDK\\include']
  cpppath += [os.environ['DXSDK_DIR'] + 'Include']
  if is_win64:
    program_name = "bin\\x64\\" + program_name
    linkflags += ' /MACHINE:X64 '
    libpath += [os.environ['DXSDK_DIR'] + 'Lib\\x64']
    libpath += [os.environ['PROGRAMFILES(X86)'] + '\\OpenAL 1.1 SDK\\libs\\Win64']
    libpath += ['lib_win\\x64']
  else:
    program_name = "bin\\" + program_name
    libpath += [os.environ['DXSDK_DIR'] + 'Lib\\x86']
    libpath += [os.environ['PROGRAMFILES(X86)'] + '\\OpenAL 1.1 SDK\\libs\\Win32']
    libpath += ['lib_win']
else:
  ccflags = defines + warnings + optimization + arch + profiling
  linkflags = arch + profiling
  cpppath += ['/usr/local/include']
  libpath += ['/usr/local/lib']
  if use_soar:
    cpppath += ['../SoarSuite/Core/ClientSML/include/']
    cpppath += ['../SoarSuite/Core/ConnectionSML/include/']
    cpppath += ['../SoarSuite/Core/shared/']
    cpppath += ['../SoarSuite/Tools/TOHSML/']
    libpath += ['../SoarSuite/SoarLibrary/lib/']
    libs += ['ClientSML', 'ConnectionSML', 'ElementXML']

libs += [library_name]
if not is_windows:
  libs += [tinyxml_name]

env = Environment(
  CCFLAGS = ccflags,
  CPPPATH = cpppath,
  LINKFLAGS = linkflags,
  LIBS = libs,
  LIBPATH = libpath)

if is_windows:
  env['CC'] = cc
  env['LINK'] = link
  env['AR'] = lib

env.Program(
  program_name,
  program)

env.StaticLibrary(
  library_name,
  library)

if is_windows:
  env.Program(
    launcher_name,
    launcher)
else:
  env.StaticLibrary(
    tinyxml_name,
    tinyxml)

### Provide help

opts = Options('custom.py')
opts.Add('debug', 'Set to 1 to build with debug information', 0)
opts.Add('noal', 'Set to 1 to disable the use of OpenAL', 0)
if is_windows:
  opts.Add('nodx9', 'Set to 1 to disable the use of DirectX 9', 0)
opts.Add('nogl', 'Set to 1 to disable the use of OpenGL', 0)
if is_windows:
  opts.Add('nowgl', 'Set to 1 to disable the use of Windows GL Extensions', 0)
if not is_windows:
  opts.Add('pedantic', 'Set to 1 to add warnings for violations of ANSI standards', 0)
  opts.Add('profile', 'Set to 1 to enable profiling using gprof/kprof', 0)
opts.Add('scu', 'Set to \'scu\' to use SCU for everything or set to \'zeni\' to use SCU on zenilib only', 0)
if not is_windows:
  opts.Add('soar', 'Set to 1 to use Soar', 0)
  opts.Add('tune', 'Set to 1 to tune the executable for this computer\'s architecture', 0)
if is_windows:
  opts.Add('x64', 'Set to 1 to compile for the AMD64/EMT64 target', 0)
opts.Update(env)
Help(opts.GenerateHelpText(env))
