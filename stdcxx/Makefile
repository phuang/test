# vim:set noet:
CFLAGS   = -g0 -O0
CXXFLAGS = $(CFLAGS) 
LDFLAGS = 

TARGETS = \
	map \
	list \
	$(NULL)

all: $(TARGETS)

map: map.cpp

list: list.cpp

clean:
	$(RM) *.o *~ $(TARGETS)
