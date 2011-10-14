#!/usr/bin/python
# -*- coding: utf-8 -*-

import math, sys, os.path, glob, string, re, os, subprocess

is_windows = sys.platform == 'win32'
is_win64   = is_windows and (os.environ['PROCESSOR_ARCHITECTURE'] == 'AMD64' or (os.environ.has_key('PROCESSOR_ARCHITEW6432') and os.environ['PROCESSOR_ARCHITEW6432'] == 'AMD64'))
is_linux   = sys.platform == 'linux2'
is_mac     = sys.platform == 'darwin'

if is_windows:
  if(os.environ.has_key('VS100COMNTOOLS')):
    VSINSTALLDIR = os.environ['VS100COMNTOOLS']
    VSINSTALLDIR = VSINSTALLDIR[:VSINSTALLDIR.find('Common')]
    VCBINDIR = VSINSTALLDIR + '\\VC\\BIN\\'
    VCVARSBAT86 = ' "' + VCBINDIR + 'vcvars32.bat" '
    VCVARSBAT64 = ' "' + VCBINDIR + 'amd64\\vcvars64.bat" '
  else:
    print "No compatible version of VisualC found.\nAttempt to use GNU toolchain."
    is_windows = False
    is_linux = True

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
nocg = True
pedantic = False
profile = False
scu = 'zeni'
soar = False
stress = True
tune = False
x64 = False
manual_vsync_delay = False

### Get architecture

if not is_windows:
  proc = subprocess.Popen(' uname -m ', shell=True, stdout=subprocess.PIPE)
  proc.wait()
  arch = proc.stdout.readline().split('\n', 1)[0]
  x64 = arch == 'x86_64'

### Generate help

vars = Variables('custom.py')

if not is_windows:
  vars.Add('cc', 'Replace \'g++\' as the C++ compiler', cc)
  vars.Add('cpp0x', 'Set to 1 to enable the c++0x standard (if supported)', cpp0x)
vars.Add('debug', 'Set to 1 to build with debug information', debug)
vars.Add('noal', 'Set to 1 to disable the use of OpenAL', noal)
if is_windows:
  vars.Add('nodx9', 'Set to 1 to disable the use of DirectX 9', nodx9)
vars.Add('nogl', 'Set to 1 to disable the use of OpenGL', nogl)
if is_windows:
  vars.Add('nowgl', 'Set to 1 to disable the use of Windows GL Extensions', nowgl)
#vars.Add('nocg', 'Set to 1 to disable the use of Cg shaders', nocg)
if not is_windows:
  vars.Add('pedantic', 'Set to 1 to add warnings for violations of ANSI standards', pedantic)
  vars.Add('profile', 'Set to 1 to enable profiling using gprof/kprof', profile)
vars.Add('scu', 'Set to \'scu\' to use SCU for everything, \'zeni\' to use SCU on zenilib only, \'no\' to disable', scu)
if not is_windows:
  vars.Add('soar', 'Set to 1 to use Soar', soar)
vars.Add('stress', 'Set to 1 to stress test your code', stress)
if not is_windows:
  vars.Add('tune', 'Set to 1 to tune the executable for this computer\'s architecture', tune)
if is_windows:
  vars.Add('x64', 'Set to 1 to compile for the AMD64/EMT64 target', x64)
vars.Add('manual_vsync_delay', 'Set to 1 to save CPU time', manual_vsync_delay)
vars.Add('application_name', 'Force output file name')

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
#nocg = arg_eval(ARGUMENTS.get('nocg', nocg))
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
manual_vsync_delay = arg_eval(ARGUMENTS.get('manual_vsync_delay', manual_vsync_delay))
if(ARGUMENTS.get('application_name')):
  application_target = ARGUMENTS.get('application_name')
else:
  application_target = False

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
    link = VCVARSBAT64 + ' && "' + VCBINDIR + 'amd64\\link.exe" '
  else:
    cc = ' "' + VCBINDIR + 'cl.exe" '
    lib = ' "' + VCBINDIR + 'lib.exe" '
    link = VCVARSBAT86 + ' && "' + VCBINDIR + 'link.exe" '
else:
  link = cc

### Collect src files

if scu == 'scu' or scu == 'zeni':
  zeni_src = ['src/zeni.cxx']
  zeni_audio_src = ['src/zeni_audio.cxx']
  zeni_core_src = ['src/zeni_core.cxx']
  zeni_graphics_src = ['src/zeni_graphics.cxx']
  zeni_net_src = ['src/zeni_net.cxx']
  zeni_rest_src = ['src/zeni_rest.cxx']
else:
  zeni_src = [
    'src/Zeni/Camera.cpp',
    'src/Zeni/Collision.cpp',
    'src/Zeni/Color.cpp',
    'src/Zeni/Colors.cpp',
    'src/Zeni/Coordinate.cpp',
    'src/Zeni/File_Ops.cpp',
    'src/Zeni/Matrix4f.cpp',
    'src/Zeni/Quaternion.cpp',
    'src/Zeni/Quit_Event.cpp',
    'src/Zeni/Random.cpp',
    'src/Zeni/Resource.cpp',
    'src/Zeni/Serialization.cpp',
    'src/Zeni/String.cpp',
    'src/Zeni/Timer_HQ.cpp',
    'src/Zeni/Vector2f.cpp',
    'src/Zeni/Vector3f.cpp',
    'src/Zeni/XML.cpp']
  zeni_audio_src = [
    'src/Zeni/Sound.cpp',
    'src/Zeni/Sound_Buffer.cpp',
    'src/Zeni/Sound_Renderer_AL.cpp',
    'src/Zeni/Sound_Source.cpp',
    'src/Zeni/Sound_Source_Pool.cpp',
    'src/Zeni/Sounds.cpp']
  zeni_core_src = [
    'src/Zeni/Core.cpp',
    'src/Zeni/Joysticks.cpp',
    'src/Zeni/Mutex.cpp',
    'src/Zeni/Thread.cpp',
    'src/Zeni/Timer.cpp']
  zeni_graphics_src = [
    'src/Zeni/EZ2D.cpp',
    'src/Zeni/Fog.cpp',
    'src/Zeni/Font.cpp',
    'src/Zeni/Fonts.cpp',
    'src/Zeni/Image.cpp',
    'src/Zeni/Light.cpp',
    'src/Zeni/Material.cpp',
    'src/Zeni/Model.cpp',
    'src/Zeni/Projector.cpp',
    'src/Zeni/Renderable.cpp',
    'src/Zeni/Shader.cpp',
    'src/Zeni/Texture.cpp',
    'src/Zeni/Textures.cpp',
    'src/Zeni/Vertex2f.cpp',
    'src/Zeni/Vertex3f.cpp',
    'src/Zeni/Vertex_Buffer.cpp',
    'src/Zeni/Video.cpp',
    'src/Zeni/Video_DX9.cpp',
    'src/Zeni/Video_GL.cpp',
    'src/Zeni/Window.cpp']
  zeni_net_src = [
    'src/Zeni/Net.cpp',
    'src/Zeni/VLUID.cpp']
  zeni_rest_src = [
    'src/Zeni/Configurator_Video.cpp',
    'src/Zeni/Console_State.cpp',
    'src/Zeni/Game.cpp',
    'src/Zeni/Gamestate.cpp',
    'src/Zeni/Gamestate_II.cpp',
    'src/Zeni/Logo.cpp',
    'src/Zeni/main.cpp',
    'src/Zeni/Widget.cpp',
    'src/Zeni/Widget_Gamestate.cpp']

if scu == 'scu':
  application_src = 'src/application.cxx'
else:
  application_src = [Glob('src/*.cpp')]

launcher_src = ['Visual Studio 2010/Launcher.cpp']

### Decide defines / options

if is_windows:
  define = ' /D '
  ccflags = ' /D "WIN32" /D "_WINDOWS" '
else:
  define = ' -D'
  ccflags = ' -D_CRT_SECURE_NO_DEPRECATE -DZENI_DLL= -DZENI_EXT= -DZENI_AUDIO_DLL= -DZENI_AUDIO_EXT= -DZENI_CORE_DLL= -DZENI_CORE_EXT= -DZENI_GRAPHICS_DLL= -DZENI_GRAPHICS_EXT= -DZENI_NET_DLL= -DZENI_NET_EXT= -DZENI_REST_DLL= -DZENI_REST_EXT= -fPIC '
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
if nocg:
  ccflags += define + 'DISABLE_CG'
if profile:
  ccflags += ' -pg '
if tune:
  ccflags += ' -march=native -mfpmath=sse -fomit-frame-pointer '
if x64:
  ccflags += define + 'X64'
if manual_vsync_delay:
  ccflags += define + 'MANUAL_GL_VSYNC_DELAY'

### Decide libraries

application_libs = ['tinyxml', 'SDL', 'SDLmain', 'SDL_image', 'SDL_ttf', 'SDL_net']

dx9_libs = []
if not nodx9:
  dx9_libs += ['gdi32']
  if not nocg:
    dx9_libs += ['CgD3D9']

gl_libs = []
if not nogl:
  if is_windows:
    gl_libs += ['opengl32', 'glew32', 'glu32']
  else:
    gl_libs += ['GL', 'GLEW', 'GLU']
  if not nocg:
    gl_libs += ['CgGL']

### Decide build options

if debug:
  suffix = '_d'
  if is_windows:
    link_optimization = ' /DEBUG /ASSEMBLYDEBUG /MAP /MAPINFO:EXPORTS /NODEFAULTLIB:msvcrt '
else:
  suffix = ''
  if is_windows:
    link_optimization = ' /INCREMENTAL:NO /OPT:REF /OPT:ICF /LTCG '

zeni_target = 'zeni' + suffix
zeni_audio_target = 'zeni_audio' + suffix
zeni_core_target = 'zeni_core' + suffix
zeni_graphics_target = 'zeni_graphics' + suffix
zeni_net_target = 'zeni_net' + suffix
zeni_rest_target = 'zeni_rest' + suffix
if not application_target:
  if is_windows:
    application_target = 'Application' + suffix
  else:
    application_target = 'application' + suffix
launcher_target = 'Launcher' + suffix

def write_appname_header():
  fout = open("Visual Studio 2010\\Application_Name.h", "w")
  fout.write('#define APPLICATION_NAME "' + program_name + '"')

if is_windows:
  write_appname_header()

### Finally define the environment + library + program

cpppath = ['include']
libpath = ['./bin']

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
  linkflags = ''
  cpppath += ['/usr/local/include']
  libpath += ['/usr/local/lib']
  if os.getenv('CPATH'):
    cpppath += os.getenv('CPATH').split(':')
  if os.getenv('C_INCLUDE_PATH'):
    cpppath += os.getenv('C_INCLUDE_PATH').split(':')
  if os.getenv('CPLUS_INCLUDE_PATH'):
    cpppath += os.getenv('CPLUS_INCLUDE_PATH').split(':')
  if os.getenv('LDPATH'):
    libpath += os.getenv('LDPATH').split(':')

if soar:
  soarpath = os.getenv('SOAR_HOME')
  cpppath += [soarpath + '/include']
  libpath += [soarpath + '/lib']
  application_libs += ['ClientSML', 'ConnectionSML', 'ElementXML']

for path in libpath:
  linkflags += " -Wl,-rpath=" + path
linkflags += " "

# Prepare to Build Application

env = Environment(
  variables = vars,
  AR = lib,
  CC = cc,
  CXX = cc,
  CCFLAGS = ccflags,
  CPPPATH = cpppath,
  LINK = link,
  LINKFLAGS = linkflags,
  LIBS = ['SDLmain', zeni_target, zeni_audio_target, zeni_core_target, zeni_graphics_target, zeni_net_target, zeni_rest_target],
  LIBPATH = libpath)

# Build Shared Library: zeni

zeni_env = Environment(
  variables = vars,
  AR = lib,
  CC = cc,
  CXX = cc,
  CCFLAGS = ccflags,
  CPPPATH = cpppath,
  LINKFLAGS = linkflags,
  LIBS = ['tinyxml'],
  LIBPATH = libpath)
zeni = zeni_env.SharedLibrary(
  target = './bin/' + zeni_target,
  source = zeni_src)
env.Alias('zeni', zeni)

# Build Shared Library: zeni_audio

zeni_audio_env = Environment(
  variables = vars,
  AR = lib,
  CC = cc,
  CXX = cc,
  CCFLAGS = ccflags,
  CPPPATH = cpppath,
  LINK = link,
  LINKFLAGS = linkflags,
  LIBS = [zeni_target],
  LIBPATH = libpath)
zeni_audio = zeni_audio_env.SharedLibrary(
  target = './bin/' + zeni_audio_target,
  source = zeni_audio_src)
env.Alias('zeni_audio', zeni_audio)
  
# Build Shared Library: zeni_core

zeni_core_env = Environment(
  variables = vars,
  AR = lib,
  CC = cc,
  CXX = cc,
  CCFLAGS = ccflags,
  CPPPATH = cpppath,
  LINK = link,
  LINKFLAGS = linkflags,
  LIBS = ['SDL', zeni_target],
  LIBPATH = libpath)
zeni_core = zeni_core_env.SharedLibrary(
  target = './bin/' + zeni_core_target,
  source = zeni_core_src)
env.Alias('zeni_core', zeni_core)

# Build Shared Library: zeni_graphics

zeni_graphics_env = Environment(
  variables = vars,
  AR = lib,
  CC = cc,
  CXX = cc,
  CCFLAGS = ccflags,
  CPPPATH = cpppath,
  LINK = link,
  LINKFLAGS = linkflags,
  LIBS = dx9_libs + gl_libs + ['SDL_image', 'SDL_ttf', '3ds', zeni_target],
  LIBPATH = libpath)
zeni_graphics = zeni_graphics_env.SharedLibrary(
  target = './bin/' + zeni_graphics_target,
  source = zeni_graphics_src)
env.Alias('zeni_graphics', zeni_graphics)

# Build Shared Library: zeni_net

zeni_net_env = Environment(
  variables = vars,
  AR = lib,
  CC = cc,
  CXX = cc,
  CCFLAGS = ccflags,
  CPPPATH = cpppath,
  LINK = link,
  LINKFLAGS = linkflags,
  LIBS = ['SDL_net', zeni_target],
  LIBPATH = libpath)
zeni_net = zeni_net_env.SharedLibrary(
  target = './bin/' + zeni_net_target,
  source = zeni_net_src)
env.Alias('zeni_net', zeni_net)

# Build Shared Library: zeni_rest

zeni_rest_env = Environment(
  variables = vars,
  AR = lib,
  CC = cc,
  CXX = cc,
  CCFLAGS = ccflags,
  CPPPATH = cpppath,
  LINK = link,
  LINKFLAGS = linkflags,
  LIBS = [zeni_target],
  LIBPATH = libpath)
zeni_rest = zeni_rest_env.SharedLibrary(
  target = './bin/' + zeni_rest_target,
  source = zeni_rest_src)
env.Alias('zeni_rest', zeni_rest)

# Build Application

application = env.Program(
  target = application_target,
  source = application_src)
env.Alias('application', application)

if is_windows:
  launcher = env.Program(
    target = launcher_target,
    source = launcher_src)
  env.Alias('launcher', launcher)

# Build zenilib

env.Alias('zenilib', ['zeni', 'zeni_audio', 'zeni_core', 'zeni_graphics', 'zeni_net', 'zeni_rest'])

### Provide help

vars.Update(env)
Help(vars.GenerateHelpText(env))
