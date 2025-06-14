OHOS_SDK_PATH = /home/penghuang/sources/command-line-tools/sdk/default/openharmony/native/
LLVM_PATH = ${OHOS_SDK_PATH}/llvm
SYSROOT = ${OHOS_SDK_PATH}/SYSROOT

CC = ${LLVM_PATH}/bin/clang
CXX = ${LLVM_PATH}/bin/clang++

CFLAGS = \
	--target=aarch64-linux-ohos \
	$(NULL)

CXXFLAGS = $(CFLAGS)

LDFLAGS = -lEGL -lGLESv3

test: test.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) test.cc -o test

run: test
	hdc.exe file send test /data
	hdc.exe shell chmod +x /data/test
	hdc.exe shell /data/test

clean:
	rm test