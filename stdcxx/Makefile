# vim:set noet:
CFLAGS   = -g0 -O0 -std=c++0x
CXXFLAGS = $(CFLAGS) 
LDFLAGS = 

TARGETS = \
	map \
	list \
	shared_ptr \
	$(NULL)

all: $(TARGETS)

map: map.cpp

list: list.cpp

shared_ptr: shared_ptr.cpp

clean:
	$(RM) *.o *~ $(TARGETS)
