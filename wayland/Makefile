CC = clang++
CXX = clang++
CXXFLAGS = -std=c++11 -MMD
LDFLAGS = -lwayland-client -lwayland-egl -lEGL -lGLESv2
SOURCES = \
	callback.cc \
	compositor.cc \
	display.cc \
	keyboard.cc \
	main.cc \
	pointer.cc \
	registry.cc \
	seat.cc \
	shell.cc \
	shell_surface.cc \
	subcompositor.cc \
	subsurface.cc \
	surface.cc \
	$(NULL)

OBJECTS = $(SOURCES:.cc=.o)
DEPS = $(SOURCES:.cc=.d)

test: waytest
	./waytest

waytest: $(OBJECTS)
	$(CC) $(LDFLAGS) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(OBJECTS) $(DEPS) *~ waytest

-include $(DEPS)
