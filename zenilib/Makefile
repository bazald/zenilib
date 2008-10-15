# MODIFY BELOW

#DISABLEGL = -DDISABLE_GL
DISABLEWGL = -DDISABLEWGL
DISABLEDX9 = -DDISABLE_DX9
#DISABLEAL = -DDISABLE_AL

BUILD = DEBUG
#BUILD = RELEASE

#SRCS_TYPE = FULL_SRCS
#EXTENSION = cpp
SRCS_TYPE = SCU_SRCS
EXTENSION = cxx

# MODIFY ABOVE
# LEAVE THE REST ALONE (unless you know what you are doing

CC = g++

#PEDANTIC = -Werror
#PEDANTIC = -pedantic
#PEDANTIC = -pedantic-errors -Werror

ALLBUILDS = -Wall -W $(PEDANTIC) $(DISABLEGL) $(DISABLEWGL) $(DISABLEDX9) $(DISABLEAL) -D_CRT_SECURE_NO_DEPRECATE -Iinclude
DEBUG = -ggdb $(ALLBUILDS)
RELEASE = -O2 $(ALLBUILDS)

LSDLFLAGS = -lSDL_image -lSDL_gfx -lSDL_ttf -lSDL_net
LGLFLAGS = -lGL -lGLU
WSDLFLAGS = -lSDLmain -lSDL $(LSDLFLAGS)
WGLFLAGS = -lopengl32 -lglu32
WFLAGS = -mwindows -lmingw32 -lwinmm -lcomctl32 -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
DX9FLAGS = -ld3dx9 -ld3d9 -ld3dxof -ldxguid
3DSFLAGS = -l3ds
LOALFLAGS = -lalut -lopenal -lvorbisfile

CFLAGS = $($(BUILD))
XFLAGS = $(3DSFLAGS) $(WGLFLAGS) $(WFLAGS) $(WSDLFLAGS) -static-libgcc
LFLAGS = `sdl-config --cflags --static-libs` $(3DSFLAGS) $(LGLFLAGS) $(LSDLFLAGS)
LD = ld

LIB_SRCS = src/Zeni/Camera.cpp \
	src/Zeni/Collision.cpp \
	src/Zeni/Color.cpp \
	src/Zeni/Colors.cpp \
	src/Zeni/Coordinate.cpp \
	src/Zeni/Core.cpp \
	src/Zeni/EZ2D.cpp \
	src/Zeni/Fog.cpp \
	src/Zeni/Font.cpp \
	src/Zeni/Fonts.cpp \
	src/Zeni/Game.cpp \
	src/Zeni/Gamestate.cpp \
	src/Zeni/Image.cpp \
	src/Zeni/Light.cpp \
	src/Zeni/main.cpp \
	src/Zeni/Material.cpp \
	src/Zeni/Matrix4f.cpp \
	src/Zeni/Model.cpp \
	src/Zeni/Mutex.cpp \
	src/Zeni/Net.cpp \
	src/Zeni/Net_Primitives.cpp \
	src/Zeni/Quaternion.cpp \
	src/Zeni/Render_Wrapper.cpp \
	src/Zeni/Resource.cpp \
	src/Zeni/Sound.cpp \
	src/Zeni/Sounds.cpp \
	src/Zeni/Texture.cpp \
	src/Zeni/Textures.cpp \
	src/Zeni/Thread.cpp \
	src/Zeni/Timer.cpp \
	src/Zeni/Vector3f.cpp \
	src/Zeni/Vertex2f.cpp \
	src/Zeni/Vertex3f.cpp \
	src/Zeni/Vertex_Buffer.cpp \
	src/Zeni/Video.cpp \
	src/Zeni/Video_DX9.cpp \
	src/Zeni/Video_GL.cpp \
	src/Zeni/Widget.cpp

LIB_SCU = src/zenilib.cxx

GAME_SRCS = src/Gamestate_One.cpp

GAME_SCU = src/zeniapp.cxx

FULL_SRCS = $(GAME_SRCS) $(LIB_SRCS)

SCU_SRCS = $(GAME_SCU) $(LIB_SCU)

SRCS =  $($(SRCS_TYPE))

OBJS = $(SRCS:.$(EXTENSION)=.o)

all: zeniapp
	

zeniapp: $(OBJS)
ifeq ($(DISABLEAL),-DDISABLE_AL)
	$(CC) $(CFLAGS) $(OBJS) -o zeniapp $(LFLAGS)
else
	$(CC) $(CFLAGS) $(OBJS) -o zeniapp $(LFLAGS) $(LOALFLAGS)
endif

%.o:
	$(CC) $(CFLAGS) -c $*.c* -o $@

partclean:
	rm -f stdout.txt stderr.txt *~ */*~ */*/*~ */*/*/*~

moreclean: partclean
	rm -f *.o */*.o */*/*.o */*/*/*.o

clean: moreclean
	rm -f zeniapp zeniapp.exe

package: clean
	cd ../ && rm -f zeniapp.7z && 7z a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=off zeniapp.7z zeniapp/
