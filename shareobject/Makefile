CXX = clang++-15
CXXFLAGS = --no-undefined -fvisibility=hidden

all: libA.so libB.so test

libA.so: A.cpp A.h
	${CXX} ${CXXFLAGS} -shared -o libA.so -fPIC A.cpp -DA_IMPLEMENTATION

libB.so: libA.so B.cpp B.h
	${CXX} ${CXXFLAGS} -shared -o libB.so -fPIC B.cpp -DA_IMPLEMENTATION -DB_IMPLEMENTATION libA.so

test: libA.so libB.so main.cpp
	${CXX} ${CXXFLAGS} -o test main.cpp -DA_IMPLEMENTATION -DB_IMPLEMENTATION libA.so libB.so

clean:
	rm -f libA.so test libB.so