def accept(arg):
  if arg == 'yes' or arg == 'Yes' or arg == 'YES' or arg == 'y' or arg == 'Y' or arg == 'true' or arg == 'True' or arg == 'TRUE' or arg == 't' or arg == 'T' or int(arg):
    return 1
  else:
    return 0

### Decide single compilation unit

program_scu = 'src/zeniapp.cxx'
library_scu = 'src/zenilib.cxx'

program = [Glob('src/*.cpp')]
library = [Glob('src/Zeni/*.cpp')]

scu = ARGUMENTS.get('scu', 0)
if scu == 'zeni' or scu == 'Zeni' or scu == 'ZENI':
  library = library_scu
else:
  if scu == 'scu' or scu == 'Scu' or scu == 'SCU' or accept(arg=scu):
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
if accept(arg=nogl):
  defines += disable_gl
else:
  libs += gl_libs

noal = ARGUMENTS.get('noal', 0)
if accept(arg=noal):
  defines += disable_al
else:
  libs += al_libs

### Decide pedantism

warnings = ' -Wall -Wno-variadic-macros '
pedantic_warnings = ' -pedantic '

pedantic = ARGUMENTS.get('pedantic', 0)
if accept(arg=pedantic):
  pedantic_warnings += pedantic

### Decide optimization

program_name = 'zeniapp'
library_name = 'zenilib'
debug_optimization = ' -ggdb '
release_optimization = ' -O2 '

debug = ARGUMENTS.get('debug', 0)
if accept(arg=debug):
  program_name += '_d'
  library_name += '_d'
  optimization = debug_optimization;
else:
  optimization = release_optimization;

### Decide profiling

profiling = ' -pg '

enable_gprof = ARGUMENTS.get('profile', 0)
if not accept(arg=enable_gprof):
  profiling = ''

### Finally define the environment + library + program

libs += [library_name]

env = Environment(
  CC = 'g++',
  CCFLAGS = defines + warnings + optimization + profiling,
  CPPPATH = 'include',
  LINKFLAGS = profiling,
  LIBS = libs + gl_libs + al_libs,
  LIBPATH = '.')

env.Program(
  program_name,
  program)

env.StaticLibrary(
  library_name,
  library)
