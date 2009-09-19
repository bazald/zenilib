import math, sys, os.path, glob, string, re, os, subprocess

#for entry in os.environ:
#  print entry + ' = ' + os.environ[entry]

is_windows = sys.platform == 'win32'
is_win64   = is_windows and (os.environ['PROCESSOR_ARCHITECTURE'] == 'AMD64' or (os.environ.has_key('PROCESSOR_ARCHITEW6432') and os.environ['PROCESSOR_ARCHITEW6432'] == 'AMD64'))
is_linux   = sys.platform == 'linux2'
is_mac     = sys.platform == 'darwin'

if is_windows:
  VSINSTALLDIR = os.environ['VS90COMNTOOLS']
  VSINSTALLDIR = VSINSTALLDIR[:VSINSTALLDIR.find('Common')]
  VCBINDIR = VSINSTALLDIR + '\\VC\\BIN\\'

### Defaults

if not is_windows:
  cc = 'g++'
cpp0x = not is_windows
debug = True
lib = 'ar'
noal = False
nodx9 = not is_windows
nogl = False
nowgl = not is_windows
pedantic = False
profile = False
scu = 'zeni'
soar = False
stress = True
tune = False
x64 = False

### Get architecture

if not is_windows:
  proc = subprocess.Popen(' uname -m ', shell=True, stdout=subprocess.PIPE)
  proc.wait()
  arch = proc.stdout.readline().split('\n', 1)[0]
  x64 = arch == 'x86_64'

### Generate help

opts = Options('custom.py')
if not is_windows:
  opts.Add('cc', 'Replace \'g++\' as the C++ compiler', cc)
  opts.Add('cpp0x', 'Set to 1 to enable the c++0x standard (if supported)', cpp0x)
opts.Add('debug', 'Set to 1 to build with debug information', debug)
opts.Add('noal', 'Set to 1 to disable the use of OpenAL', noal)
if is_windows:
  opts.Add('nodx9', 'Set to 1 to disable the use of DirectX 9', nodx9)
opts.Add('nogl', 'Set to 1 to disable the use of OpenGL', nogl)
if is_windows:
  opts.Add('nowgl', 'Set to 1 to disable the use of Windows GL Extensions', nowgl)
if not is_windows:
  opts.Add('pedantic', 'Set to 1 to add warnings for violations of ANSI standards', pedantic)
  opts.Add('profile', 'Set to 1 to enable profiling using gprof/kprof', profile)
opts.Add('scu', 'Set to \'scu\' to use SCU for everything, \'zeni\' to use SCU on zenilib only, \'no\' to disable', scu)
if not is_windows:
  opts.Add('soar', 'Set to 1 to use Soar', soar)
opts.Add('stress', 'Set to 1 to stress test your code', stress)
if not is_windows:
  opts.Add('tune', 'Set to 1 to tune the executable for this computer\'s architecture', tune)
if is_windows:
  opts.Add('x64', 'Set to 1 to compile for the AMD64/EMT64 target', x64)

### Read arguments

def arg_eval(arg):
  if arg is True:
    return True
  if arg is False:
    return False
  sarg = str(arg).lower()
  if sarg == 'true' or sarg == 't':
    return True
  if sarg == 'false' or sarg == 'f':
    return False
  return int(arg)

if not is_windows:
  cc = ARGUMENTS.get('cc', cc)
  cpp0x = arg_eval(ARGUMENTS.get('cpp0x', cpp0x))
debug = arg_eval(ARGUMENTS.get('debug', debug))
noal = arg_eval(ARGUMENTS.get('noal', noal))
if is_windows:
  nodx9 = arg_eval(ARGUMENTS.get('nodx9', nodx9))
nogl = arg_eval(ARGUMENTS.get('nogl', nogl))
if is_windows:
  nowgl = arg_eval(ARGUMENTS.get('nowgl', nowgl))
if not is_windows:
  pedantic = arg_eval(ARGUMENTS.get('pedantic', pedantic))
  profile = arg_eval(ARGUMENTS.get('profile', profile))
scu = ARGUMENTS.get('scu', scu).lower()
if not is_windows:
  soar = arg_eval(ARGUMENTS.get('soar', soar))
stress = arg_eval(ARGUMENTS.get('stress', stress))
if not is_windows:
  tune = arg_eval(ARGUMENTS.get('tune', tune))
if is_windows:
  x64 = arg_eval(ARGUMENTS.get('x64', x64))

### Print arguments

#if not is_windows:
  #print 'cc = ' + str(cc)
  #print 'cpp0x = ' + str(cpp0x)
#print 'debug = ' + str(debug)
#if not is_windows:
  #print 'link = ' + str(link)
#print 'noal = ' + str(noal)
#if is_windows:
  #print 'nodx9 = ' + str(nodx9)
#print 'nogl = ' + str(nogl)
#if is_windows:
  #print 'nowgl = ' + str(nowgl)
#if not is_windows:
  #print 'pedantic = ' + str(pedantic)
  #print 'profile = ' + str(profile)
#print 'scu = ' + str(scu)
#if not is_windows:
  #print 'soar = ' + str(soar)
#print 'stress = ' + str(stress)
#if not is_windows:
  #print 'tune = ' + str(tune)
#if is_windows:
  #print 'x64 = ' + str(x64)

### Get g++ Version (if applicable)

def gcc_version():
	proc = subprocess.Popen(cc + ' --version ', shell=True, stdout=subprocess.PIPE)
	proc.wait()
	version_line = proc.stdout.readline().rsplit('\n', 1)[0]
	for possible_vs in version_line.split(' '):
		possible_svs = possible_vs.split('.')
		try:
			if len(possible_svs) is 3 and str(int(possible_svs[0])) and str(int(possible_svs[1])) and str(int(possible_svs[2])):
				split_version_string = possible_svs
				break
		except ValueError:
			continue
	return [int(split_version_string[0]), int(split_version_string[1]), int(split_version_string[2])]

if is_linux:
  gcc = gcc_version()
else:
  gcc = [0, 0, 0]

cpp0x = cpp0x and gcc[0] > 3 and gcc[1] > 2

### Decide architecture

if is_windows:
  if x64:
    cc = ' "' + VCBINDIR + 'amd64\\cl.exe" '
    lib = ' "' + VCBINDIR + 'amd64\\lib.exe" '
    link = '"' + VCBINDIR + 'vcvars64.bat"' + ' && ' + ' "' + VCBINDIR + 'amd64\\link.exe" '
  else:
    cc = ' "' + VCBINDIR + 'cl.exe" '
    lib = ' "' + VCBINDIR + 'lib.exe" '
    link = ' "' + VCBINDIR + 'link.exe" '
else:
  link = cc

### Decide single compilation unit

launcher = ['Visual Studio 2008/Launcher.cpp']

if scu == 'scu':
  program = 'src/application.cxx'
else:
  program = [Glob('src/*.cpp')]

if scu == 'scu' or scu == 'zeni':
  library = 'src/zenilib.cxx'
else:
  library = [Glob('src/TinyXML/*.cpp'), Glob('src/Zeni/*.cpp')]

### Decide defines / options

if is_windows:
  define = ' /D '
  ccflags = ' /D "WIN32" /D "_WINDOWS" '
else:
  define = ' -D'
  ccflags = ' -D_CRT_SECURE_NO_DEPRECATE '
  if is_mac:
    ccflags += ' -D_MACOSX '
  elif is_linux:
    ccflags += ' -D_LINUX '

if cpp0x:
  ccflags += ' -std=c++0x '
if debug:
  ccflags += define + '_DEBUG'
  if is_windows:
    ccflags += ' /Od              /Gy     /RTC1 /MDd '
    if x64:
      ccflags += ' /Zi '
    else:
      ccflags += ' /ZI '
  else:
    ccflags += ' -ggdb '
else:
  ccflags += define + 'NDEBUG'
  if is_windows:
    ccflags += ' /Ob2 /Oi /Ot /Oy /GT /GL       /MD  /Zi '
  else:
    ccflags += ' -O2 -fexpensive-optimizations -ffast-math -funroll-loops '
if pedantic:
  ccflags += ' -pedantic '
if debug and stress:
  ccflags += define + 'TEST_NASTY_CONDITIONS'
if noal:
  ccflags += define + 'DISABLE_AL'
if nodx9:
  ccflags += define + 'DISABLE_DX9'
if nogl:
  ccflags += define + 'DISABLE_GL'
if nowgl:
  ccflags += define + 'DISABLE_WGL'
if profile:
  ccflags += ' -pg '
if tune:
  ccflags += ' -march=native -mfpmath=sse -fomit-frame-pointer '
if x64:
  ccflags += define + 'X64'

### Decide libraries

libs = ['SDL', 'SDLmain', 'SDL_image', 'SDL_gfx', 'SDL_ttf', 'SDL_net', 'Cg']

if is_windows:
  libs += ['lib3ds-2_0', 'Advapi32', 'SHFolder', 'Shell32', 'User32']
else:
  libs += ['lib3ds']

if not noal:
  if is_windows:
    libs += ['OpenAL32', 'alut', 'libvorbis', 'libvorbisfile']
  else:
    libs += ['openal', 'alut', 'libvorbis', 'vorbisfile']

if not nodx9:
  libs += ['d3d9', 'd3dx9', 'gdi32', 'CgD3D9']

if not nogl:
  if is_windows:
    libs += ['opengl32', 'glew32', 'glu32', 'CgGL']
  else:
    libs += ['GL', 'GLEW', 'GLU', 'CgGL']

### Decide build options

launcher_name = 'Launcher'
if is_windows:
  program_name = 'Application'
else:
  program_name = 'application'
library_name = 'zenilib'

if debug:
  if x64:
    suffix = '_x64d'
  else:
    suffix = '_d'
  if is_windows:
    link_optimization = ' /DEBUG /ASSEMBLYDEBUG /MAP /MAPINFO:EXPORTS /NODEFAULTLIB:msvcrt '
else:
  if x64:
    suffix = '_x64'
  else:
    suffix = ''
  if is_windows:
    link_optimization = ' /INCREMENTAL:NO /OPT:REF /OPT:ICF /LTCG '

def write_appname_header():
  fout = open("Visual Studio 2008\\Application_Name.h", "w")
  fout.write('#define APPLICATION_NAME "' + program_name + '"')

if is_windows:
  write_appname_header()

launcher_name += suffix
program_name += suffix
library_name += suffix

libs += [library_name]

### Finally define the environment + library + program

cpppath = ['include']
libpath = ['.']

if is_windows:
  #ccflags += ' /Wall /wd4548 /wd4820 /wd4668 /wd4619 /wd4571 /wd4710 /wd4555 /wd4061 /wd4640 /wd4264 /wd4266 /wd4917 '
  #ccflags += ' /wd4191 ' # eliminate OpenGL extension pointer cast warnings
  ccflags += ' /D "NOMINMAX" /D "WIN32_LEAN_AND_MEAN" /FD /EHsc /fp:fast /W4 /nologo /c /TP /wd4005 /wd4312 /wd4505 /wd4311 /errorReport:prompt '
  linkflags = '/NOLOGO /SUBSYSTEM:WINDOWS /DYNAMICBASE:NO /ERRORREPORT:PROMPT' + link_optimization
  cpppath += ['include_win']
  cpppath += [os.environ['PROGRAMFILES(X86)'] + '\\OpenAL 1.1 SDK\\include']
  cpppath += [os.environ['DXSDK_DIR'] + 'Include']
  if x64:
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
  ccflags += ' -Wall '
  if gcc[0] > 3:
    ccflags += ' -Wno-variadic-macros '
  linkflags = ''
  cpppath += ['/usr/local/include']
  libpath += ['/usr/local/lib']

if soar:
  cpppath += ['../SoarSuite/Core/ClientSML/include/']
  cpppath += ['../SoarSuite/Core/ConnectionSML/include/']
  cpppath += ['../SoarSuite/Core/shared/']
  cpppath += ['../SoarSuite/Tools/TOHSML/']
  libpath += ['../SoarSuite/SoarLibrary/lib/']
  libs += ['ClientSML', 'ConnectionSML', 'ElementXML']

env = Environment(
  AR = lib,
  CC = cc,
  CXX = cc,
  CCFLAGS = ccflags,
  CPPPATH = cpppath,
  LINK = link,
  LINKFLAGS = linkflags,
  LIBS = libs,
  LIBPATH = libpath)

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

### Provide help

opts.Update(env)
Help(opts.GenerateHelpText(env))
