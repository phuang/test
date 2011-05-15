obj-m += phuang.o

phuang-y := super.o

KMAKE := make -C /lib/modules/`uname -r`/build M=`pwd`

all:
	$(KMAKE)

clean:
	$(KMAKE) clean
	$(RM) *~
