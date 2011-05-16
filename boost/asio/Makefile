CFLAGS   = -g0 -O3
CXXFLAGS = $(CFLAGS) 
LDFLAGS = -lboost_system

TARGETS = \
	timer3 \
	server \
	$(NULL)

all: $(TARGETS)

timer3: timer3.cpp

server: server.cpp

strip:
	strip $(TARGETS)

clean:
	$(RM) $(TARGETS) *~ *.o

