### Decide single compilation unit

program_scu = 'src/zeniapp.cxx'
library_scu = 'src/zenilib.cxx'

program = [Glob('src/*.cpp')]
library = [Glob('src/Zeni/*.cpp')]

scu = ARGUMENTS.get('scu', 0)
if scu == 'zeni' or scu == 'Zeni' or scu == 'ZENI':
  library = library_scu
else:
  if scu == 'scu' or scu == 'Scu' or scu == 'SCU':
    program = program_scu
    library = library_scu

### Decide libraries

disable_al = ' -DDISABLE_AL '
disable_dx9 = ' -DDISABLE_DX9 '
disable_gl = ' -DDISABLE_GL '
disable_wgl = ' -DDISABLEWGL '

defines = ' -D_CRT_SECURE_NO_DEPRECATE ' + disable_dx9 + disable_wgl

libs = ['SDL', 'SDLmain', 'SDL_image', 'SDL_gfx', 'SDL_ttf', 'SDL_net',
  'Cg',
  'lib3ds']
gl_libs = ['GL', 'GLEW', 'GLU', 'CgGL']
al_libs = ['openal', 'alut', 'vorbisfile']

nogl = ARGUMENTS.get('nogl', 0)
if int(nogl):
  defines += disable_gl
else:
  libs += gl_libs

noal = ARGUMENTS.get('noal', 0)
if int(noal):
  defines += disable_al
else:
  libs += al_libs

### Decide pedantism

warnings = ' -Wall -Wno-variadic-macros '
pedantic_warnings = ' -pedantic '

pedantic = ARGUMENTS.get('pedantic', 0)
if int(pedantic):
  warnings += pedantic_warnings

### Decide optimization

program_name = 'zeniapp'
library_name = 'zenilib'
debug_optimization = ' -ggdb '
release_optimization = ' -O2 -fexpensive-optimizations -ffast-math -funroll-loops '

debug = ARGUMENTS.get('debug', 0)
if int(debug):
  program_name += '_d'
  library_name += '_d'
  optimization = debug_optimization;
else:
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

libs += [library_name]

env = Environment(
  CC = 'g++',
  CCFLAGS = defines + warnings + optimization + arch + profiling,
  CPPPATH = 'include',
  LINKFLAGS = arch + profiling,
  LIBS = libs + gl_libs + al_libs,
  LIBPATH = '.')

env.Program(
  program_name,
  program)

env.StaticLibrary(
  library_name,
  library)

### Provide help

opts = Options('custom.py')
opts.Add('debug', 'Set to 1 to build with debug information', 0)
opts.Add('noal', 'Set to 1 to disable the use of OpenAL', 0)
opts.Add('nogl', 'Set to 1 to disable the use of OpenGL', 0)
opts.Add('pedantic', 'Set to 1 to add warnings for violations of ANSI standards', 0)
opts.Add('profile', 'Set to 1 to enable profiling using gprof/kprof', 0)
opts.Add('scu', 'Set to \'scu\' to use SCU for everything or set to \'zeni\' to use SCU on zenilib only', 0)
opts.Add('tune', 'Set to 1 to tune the executable for this computer\'s architecture', 0)
opts.Update(env)
Help(opts.GenerateHelpText(env))
