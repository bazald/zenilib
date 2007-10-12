# MODIFY BELOW

#DISABLEGL = -DDISABLE_GL
DISABLEWGL = -DDISABLEWGL
DISABLEDX9 = -DDISABLE_DX9
#DISABLEAL = -DDISABLE_AL

#BUILD = DEBUG
BUILD = RELEASE

# MODIFY ABOVE
# LEAVE THE REST ALONE (unnless you know what you are doing

CC = g++

ALLBUILDS = -pedantic-errors -Wall -W -Werror $(DISABLEGL) $(DISABLEWGL) $(DISABLEDX9) $(DISABLEAL) -D_CRT_SECURE_NO_DEPRECATE -Iinclude
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
LFLAGS = `sdl-config --cflags --static-libs` $(3DSFLAGS) $(LGLFLAGS) $(LSDLFLAGS) --static-libgcc
LD = ld

SRCS =  src/zenilib.o \
	src/zeniapp.o
OBJS = $(SRCS:.cpp=.o)

all: zeniapp
	

zeniapp: $(OBJS)
ifeq ($(DISABLEAL),-DDISABLE_AL)
	$(CC) $(CFLAGS) $(OBJS) -o zeniapp $(LFLAGS)
else
	$(CC) $(CFLAGS) $(OBJS) -o zeniapp $(LFLAGS) $(LOALFLAGS)
endif

%.o:
	$(CC) $(CFLAGS) -c $*.cpp -o $@

partclean:
	rm -f stdout.txt stderr.txt *~ */*~ */*/*~ */*/*/*~

moreclean: partclean
	rm -f *.o */*.o */*/*.o */*/*/*.o

clean: moreclean
	rm -f zeniapp zeniapp.exe

package: clean
	cd ../ && rm -f zeniapp.7z && 7z a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=off zeniapp.7z zeniapp/
