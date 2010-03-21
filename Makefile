CC=g++
LD=g++
CXXFLAGS = `allegro-config --cppflags` -m32 -g
LDLIBS = `allegro-config --libs` -m32
OBJS=main.o polygons.o tank.o world.o tests.o
PROG=tank

$(PROG): $(OBJS)

tests/cube: tests/cube.o
tests/cube2: tests/cube2.o
tests/world: tests/world.o

clean:
	$(RM) $(OBJS) $(PROG)


