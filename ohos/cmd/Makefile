OHOS_SDK_PATH=/home/penghuang/sources/command-line-tools/sdk/default/openharmony/native
PREBUILT_CLANG_PATH = /home/penghuang/sources/roms/ohos/prebuilts/clang_linux-x86_64-c5b0b3-20240320
OHOS_ROOT_PATH = /home/penghuang/sources/roms/ohos
FOUNDATION_ROOT_PATH = ${OHOS_ROOT_PATH}/foundation
SYSROOT = ${OHOS_SDK_PATH}/sysroot

CC = ${PREBUILT_CLANG_PATH}/bin/clang
CXX = ${PREBUILT_CLANG_PATH}/bin/clang++

INCLUDES = \
			-I${OHOS_ROOT_PATH} \
			-I${OHOS_ROOT_PATH}/commonlibrary/c_utils/base/include \
			-I${OHOS_ROOT_PATH}/third_party/skia \
			-I${OHOS_ROOT_PATH}/third_party/mindspore/mindspore-src/source/third_party/securec/include \
			-I${FOUNDATION_ROOT_PATH} \
			-I${FOUNDATION_ROOT_PATH}/arkui \
			-I${FOUNDATION_ROOT_PATH}/arkui/ace_engine \
			-I${FOUNDATION_ROOT_PATH}/arkui/ace_engine/frameworks \
			-I${FOUNDATION_ROOT_PATH}/arkui/ace_engine/interfaces/inner_api/ace_kit/include \
			-I${FOUNDATION_ROOT_PATH}/arkui/ace_engine/interfaces/native \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/2d_graphics/include \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/2d_graphics/src \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/2d_graphics/src/drawing/engine_adapter/ \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/platform/eventhandler \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/platform/image_native \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/platform/ipc_core \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/platform/utils \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/render_service_base/include \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/render_service_client/core \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/2d_engine/rosen_text/export/rosen_text/ \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_surface/interfaces/inner_api/common \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_surface/interfaces/inner_api/surface \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_surface/interfaces/inner_api/utils \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/interfaces/inner_api/composer \
			-I${FOUNDATION_ROOT_PATH}/graphic/graphic_2d/rosen/modules/composer/vsync/include \
			$(NULL)

LINK_DIRS = -L${OHOS_ROOT_PATH}/libs

CFLAGS = \
	--target=aarch64-linux-ohos \
	--sysroot=${SYSROOT} \
	$(NULL)

CXXFLAGS = \
      $(CFLAGS) \
		  ${INCLUDES} \
			-std=c++17 \
			-fno-rtti \
			-Wno-c++11-narrowing \
			-D__OHOS__=1 \
			-DDOHOS_PLATFORM \
			$(NULL)

LDFLAGS = \
			${LINK_DIRS} \
			-lace_compatible.z \
			-lrender_service_base.z \
			-lrender_service_client.z \
			$(NULL)

test: main.cc
	${CXX} $(CXXFLAGS) $(LDFLAGS) -o test main.cc

run: test
	hdc.exe file send test /data
	hdc.exe shell chmod +x /data/test
	hdc.exe shell /data/test

clean:
	rm test