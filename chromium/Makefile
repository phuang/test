WEBGL_DEMO = http://www.yuichiroharai.com/wgl/
GTEST_FILTER := *
OUT := out/brand
OUT := out/mus_Debug
OUT := out/link
OUT := out_cyan/Release
OUT := out/release
OUT := out_link/mus
OUT := out/mus
OUT := out/Release
# OUT := out/linux_ozone_rel
# OUT := out_android/Release
OUT := out_chell/Release
# OUT = out/mus_Debug
# OUT =out/linux_chromium
# OUT=out_veyron_minnie/Release
NINJIA_ARGS = -j 2000 -k50 -C $(OUT)
NINJIA = ninja $(NINJIA_ARGS)

DEVICE_IP = penghuang-crbook
STRIP_FLAGS = --strip-flags="--strip-debug -w -N '*blink*' -w -N '*WebCore*' -w -N '*content*'"
# STRIP_FLAGS = --strip-flags="--strip-debug -w -N '*blink*' -w -N '*WebCore*' -w -N '*Sk*'"
# STRIP_FLAGS = --nostrip
TARGET_FLAGS = --target-dir=/tmp/chrome --mount  --mash
TARGET_FLAGS = --target-dir=/tmp/chrome --mount
# TARGET_FLAGS =
DEPLOY_FLAGS = \
	--build-dir=$(OUT) \
	$(TARGET_FLAGS) \
	--to=$(DEVICE_IP) \
	$(STRIP_FLAGS) \
	--debug \
	$(NULL)

	# --debug \
	# --nostartui \

CHROME_FLAGS =  \
								--user-data-dir=/tmp/penghuang-mustash \
 								--screen-config=1000x800

CHROME_FLAGS +=  \
								--show-composited-layer-borders \
								$(NULL)
								# --ui-show-composited-layer-borders=renderpass,surface,layer \
								#--show-fps-counter
# CHROME_FLAGS += --wait-for-debugger=ui
# EXO_DEMO = exo_demo

# PAGE = file:///usr/local/google/home/penghuang/sources/phuang.github.io/backdrop_with_mask.html
PAGE = phuang.github.io/backdrop_with_mask.html
# PAGE = file:///usr/local/google/home/penghuang/sources/chromium/src/third_party/WebKit/LayoutTests/css3/filters/backdrop-filter-rendering.html
PAGE = https://webkit.org/blog-files/3d-transforms/poster-circle.html
CHROME_FLAGS = \
							 --user-data-dir=/tmp/penghuang-mustash \
							 --enable-experimental-web-platform-features \
							 $(PAGE)
# CHROME_FLAGS += --show-overdraw-feedback

ADB = third_party/android_sdk/public/platform-tools/adb


.PHONY: chrome content mash mus_demo $(EXO_DEMO) exo_unittests clean

all:
	$(NINJIA)

chrome_sandbox:
	$(NINJIA) chrome_sandbox

chrome: chrome_sandbox
	$(NINJIA) chrome

test_chrome: chrome_sandbox
	$(NINJIA) test_chrome

content:
	$(NINJIA) content

mash:
	# $(NINJIA) mash:all
	echo mash

mus_demo:
	$(NINJIA) mus_demo

exo_demo:
	$(NINJIA) exo_demo

# deploy_chrome: chrome mash mus_demo
deploy_chrome: chrome
	deploy_chrome $(DEPLOY_FLAGS)


deploy_chrome_so: chrome
	GN_ARGS='is_component_build=true' deploy_chrome $(DEPLOY_FLAGS)
	# GN_ARGS='is_component_build=true is_chrome_branded=true' deploy_chrome $(DEPLOY_FLAGS)

deploy_mash: mash mus_demo
	GN_ARGS='is_component_build=true is_chrome_branded=true' deploy_chrome $(DEPLOY_FLAGS)

run_mus_demo: mus_demo mash chrome
	$(OUT)/mash --service=mus_demo

run_exo_demo: exo_demo mash chrome
	$(OUT)/mash --service=exo_demo


run_mus_demo_debug_ui: mus_demo
	$(OUT)/mash --service=mus_demo --wait-for-debugger=ui

run_mus_mash: mash content $(EXO_DEMO)
	$(OUT)/mash --service=mash_session --no-sandbox

run_mus_ui: mash ui
	$(OUT)/mash --service=ui --no-sandbox

run_chrome_pixel_diff: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) && $(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=UseSkiaRenderer

run_chrome: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --no-sandbox

run_chrome_wayland_oopd: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --enable-wayland-server --enable-features=VizDisplayCompositor

run_chrome_wayland: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --enable-wayland-server --trace-to-console=penghuang

run_chrome_oopd: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --enable-features=VizDisplayCompositor

run_chrome_oopd_skia: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --enable-features=VizDisplayCompositor,UseSkiaRenderer \
	$(NULL)

run_chrome_oopd_skia_ddl: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UseSkiaDeferredDisplayList \
		--use-gl=any --disable-features=Mus \
		--no-sandbox

run_chrome_oopd_skia_ddl_vk: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UseSkiaDeferredDisplayList \
		--use-gl=any --disable-features=Mus \
		--enable-vulkan

run_chrome_oopd_skia_ddl_oopr: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UseSkiaDeferredDisplayList \
		--use-gl=any --disable-features=Mus --enable-oop-rasterization --enable-gpu-rasterization

run_chrome_oopd_skia_ddl_oopr_vk: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UseSkiaDeferredDisplayList,UiGpuRasterization \
		--use-gl=any --disable-features=Mus --enable-oop-rasterization --enable-vulkan --enable-gpu-rasterization \
		--trace-to-console=vulkan \
		$(PAGE)

run_chrome_oopd_skia_ddl_swiftshader: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UseSkiaDeferredDisplayList \
		--use-gl=swiftshader-webgl --disable-features=Mus

run_chrome_oopd_oopr: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--no-sandbox \
		$(NULL)

run_chrome_oopr: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--enable-oop-rasterization-ddl \
		--no-sandbox \
		$(NULL)

run_chrome_oopr_with_tracing: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--no-sandbox \
		--trace-to-console=gpu \
		$(NULL)

		# --disable-gpu-rasterization 
run_chrome_skia_renderer: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --use-skia-renderer --trace-to-console=penghuang3

run_chrome_skia_renderer_viz: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --use-skia-renderer --enable-viz

run_chrome_vulkan: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --enable-vulkan  --disable-gpu

run_chrome_skia_renderer_vk_disable_gpu_compositing: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --use-skia-renderer --enable-vulkan --enable-viz --disable-gpu --disable-gpu-compositing

run_chrome_skia_renderer_vk_gpu: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --use-skia-renderer --enable-vulkan --enable-viz

run_chrome_ozone_x11: chrome
	$(OUT)/chrome  $(CHROME_FLAGS)  --ozone-platform=x11

run_chrome_egl: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --use-gl=egl

run_chrome_overdraw: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --show-overdraw-feedback --enable-draw-occlusion

run_chrome_presentation_callback: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --enable-presentation-callback \
		--enable-draw-occlusion


FAST_INK_FLAGS = \
	--has-internal-stylus \
	--force-enable-stylus-tools \
	--pen-devices=10 \
	--ash-debug-shortcuts \
	--use-first-display-as-internal

run_chrome_with_fast_ink: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) $(FAST_INK_FLAGS) --enable-features=VizDisplayCompositorrr

run_chrome_with_fast_ink_hdpi: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) $(FAST_INK_FLAGS) --force-device-scale-factor=2.0 --mus

run_chrome_tracing: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --trace-to-console=GLX

run_chrome_scale_2: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --force-device-scale-factor=2.0

run_chrome_mojo_sink: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --mojo-compositor-frame-sink

run_chrome_two_displays: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --ash-host-window-bounds=900x900*2,900+0-900x900 --ash-dev-shortcuts --ash-debug-shortcuts --use-first-display-as-internal

run_chrome_touch: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --touch-devices=12 --force-enable-stylus-tools --force-tablet-mode --ash-enable-palette-on-all-displays

run_mus_chrome: mash chrome
	$(OUT)/chrome --mus http://www.google.com $(CHROME_FLAGS)

run_mus_chrome_ozone_wayland: mash chrome
	$(OUT)/chrome --mus http://www.google.com $(CHROME_FLAGS) --ozone-platform=wayland

run_mus_chrome_egl: mash chrome
	$(OUT)/chrome --mus http://www.google.com $(CHROME_FLAGS) --use-gl=egl

run_mus_chrome_with_fast_ink: mash chrome
	$(OUT)/chrome --mus http://www.google.com $(CHROME_FLAGS) $(FAST_INK_FLAGS)

run_mus_chrome_scale_2: mash chrome
	$(OUT)/chrome --mus http://www.google.com $(CHROME_FLAGS) --force-device-scale-factor=2.0

run_mus_chrome_two_displays: mash chrome
	$(OUT)/chrome --mus http://www.google.com $(CHROME_FLAGS) --mus --ash-debug-shortcuts --ash-dev-shortcuts --screen-config=900x900^300/i,900x900

run_mus_chrome_sync: mash chrome
	$(OUT)/chrome --mus http://www.google.com $(CHROME_FLAGS) --enable-surface-synchronization

run_mus_chrome_youtube: mash chrome
	$(OUT)/chrome $(CHROME_FLAGS) \
		--mus \
		https://www.youtube.com/watch?v=NhKiJOX6zfo

run_mash_chrome: mash chrome
	$(OUT)/chrome --mash http://www.google.com $(CHROME_FLAGS)

run_mash_chrome_sync: mash chrome
	$(OUT)/chrome --mash http://www.google.com $(CHROME_FLAGS) --enable-surface-synchronization

run_mash_chrome_youtube: mash chrome
	$(OUT)/chrome $(CHROME_FLAGS) \
		--mash \
		https://www.youtube.com/watch?v=NhKiJOX6zfo

run_mus_webgl_demo: chrome
	$(OUT)/chrome $(CHROME_FLAGS) \
		--mash $(WEBGL_DEMO) --show-composited-layer-borders

run_mus_webgl_demo_without_mash: chrome
	$(OUT)/chrome $(CHROME_FLAGS) \
		$(WEBGL_DEMO) --show-composited-layer-borders

run_mus_webgl_demo_and_use_mus_in_renderer: chrome
	$(OUT)/chrome --mash --use-mus-in-renderer $(WEBGL_DEMO)

run_mus_chrome_and_use_mus_in_renderer: chrome
	$(OUT)/chrome --mash --use-mus-in-renderer https://www.google.ca

vulkan_demo:
	$(NINJIA) vulkan_demo

run_vulkan_demo: vulkan_demo
	$(OUT)/vulkan_demo --trace-to-console=vulkan

exo_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

components_unittests:
	$(NINJIA) $@ && $(OUT)/$@

display_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

components_perftests:
	$(NINJIA) $@ && $(OUT)/$@ --mus

content_browsertests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

browser_tests:
	$(NINJIA) $@ && xvfb-run $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

browser_tests_video:
	$(NINJIA) browser_tests && xvfb-run $(OUT)/browser_tests --gtest_filter=*TestVideoDisplayPerfVP9/0 --run-manual


mus_browser_tests:
	$(NINJIA) browser_tests && xvfb-run $(OUT)/browser_tests --mus --gtest_filter=$(GTEST_FILTER)

interactive_ui_tests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER) --no-sandbox

unit_tests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

ash_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

ash_unittests_mus:
	$(NINJIA) ash_unittests && $(OUT)/ash_unittests --mus --gtest_filter=$(GTEST_FILTER)

ash_perftests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

views_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

views_mus_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

views_mus_interactive_ui_tests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

telemetry_perf_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

mus_ws_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

mash_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

aura_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

aura_unittests_mus:
	$(NINJIA) aura_unittests && $(OUT)/aura_unittests --gtest_filter=$(GTEST_FILTER) --mus

cc_unittests:
	$(NINJIA) cc_unittests && $(OUT)/cc_unittests --gtest_filter=$(GTEST_FILTER) 

ui_service_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

content_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

cc_perftests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

gpu_perftests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

viz_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

wayland_rects_client:
	$(NINJIA) $@ && $(OUT)/$@ --num-rects=10  --show-fps-counter  --size=256x256

wayland_simple_client:
	$(NINJIA) $@ && $(OUT)/$@

wayland_subsurface_client:
	$(NINJIA) $@ && $(OUT)/$@

wayland_client_perftests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

wayland_client_perftests_external_wayland:
	$(NINJIA) wayland_client_perftests && $(OUT)/wayland_client_perftests  \
		--run-with-external-wayland-server \
		--gtest_filter=$(GTEST_FILTER)

skia_demo:
	$(NINJIA) $@ && $(OUT)/$@ --use-skia --use-ddl

ash_standalone:
	$(NINJIA) mash mash/session ash_standalone && $(OUT)/mash --service=mash_session --window-manager=ash_standalone

ppapi_example_compositor:
	$(NINJIA) ppapi/examples/compositor

run_ppapi_compositor: chrome ppapi_example_compositor
	$(OUT)/chrome \
		--register-pepper-plugins="$(OUT)/libppapi_example_compositor.so;application/x-ppapi-example-compositor" \
		file://`pwd`/ppapi/examples/compositor/compositor.html

run_mus_ppapi_compositor: chrome ppapi_example_compositor mash
	$(OUT)/chrome --mash $(CHROME_FLAGS) \
		--register-pepper-plugins="$(OUT)/libppapi_example_compositor.so;application/x-ppapi-example-compositor" \
		file://`pwd`/ppapi/examples/compositor/compositor.html

smoothness.top_25_smooth_mash: test_chrome
	./tools/perf/run_benchmark --browser=exact \
		--browser-executable=$(OUT)/test_chrome \
		--extra-browser-args="--use-gl=egl --mash --gpu-no-complete-info-collection" \
		smoothness.top_25_smooth

args:
	gn args $(OUT)

check:
	gn check $(OUT)

check_chromium:
	gn check out/linux_chromium

clean:
	$(NINJIA) -t clean

cros_flush:
	cros flash ssh://$(DEVICE_IP)  chromiumos_test_image.bin

cros_flush_usb:
	cros flash usb://  chromiumos_test_image.bin

ssh:
	ssh root@$(DEVICE_IP)

rsync:
	rsync -e ssh -r -v \
		--exclude 'gen' \
		--exclude 'obj' \
		--exclude 'clang_x64' \
		--exclude '*.lib' \
		--exclude '*.pdb' \
		--delete \
		out_win/Release \
		user@100.107.103.71:/mnt/c/Users/User/chromium/src/out_cross

deploy_windows:
	ninja -C out_win/Release -j 128 chrome
	make rsync

deploy_gl_tests:
	ninja -C out_win/Release -j 128 gl_tests
	make rsync

chrome_apk:
	$(NINJIA) chrome_apk

chrome_apk_incremental:
	$(NINJIA) chrome_apk_incremental

run_chrome_apk: chrome_apk
	# $(OUT)/bin/chrome_apk install --non-incremental
	$(ADB) install -r $(OUT)/apks/Chrome.apk

run_chrome_apk_incremental: chrome_apk_incremental
	$(OUT)/bin/chrome_apk install --incremental

android_symbol:
	third_party/android_platform/development/scripts/stack --output-directory=$(OUT)
