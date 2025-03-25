CXX = clang++-15

all: libA.so libB.so test

libA.so:
	${CXX} -shared -o libA.so -fPIC A.cpp -DA_IMPLEMENTATION

libB.so: libA.so
	${CXX} -shared -o libB.so -fPIC B.cpp -DA_IMPLEMENTATION -DB_IMPLEMENTATION -L. -lA

test: libA.so
	${CXX} -o test main.cpp -L. -lA -DA_IMPLEMENTATION

clean:
	rm -f libA.so test libB.so