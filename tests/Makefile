CFLAGS = `allegro-config --cflags` -m32 -g
LDLIBS = `allegro-config --libs` -m32
PROGS=cube cube2 world

all: $(PROGS)

cube: cube.o
cube2: cube2.o
world: world.o

clean:
	$(RM) $(PROGS) *.o
