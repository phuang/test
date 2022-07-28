GTEST_FILTER := *

# build output dir
OUT := out/Release
# OUT := out_msan/Release
# OUT := out_cros/Release
# OUT := out_official/Release
# OUT := out_wayland/Release
OUT := out_android/Release
# OUT := out_android_x86/Release
# OUT := out_win/Release
# GOMA_DISABLED=true

J := 3000
NINJIA_ARGS = -C $(OUT) -j $(J)
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

# Flags for unittests
GTEST_FLAGS = --test-launcher-jobs=50 --single-process-tests --use-gpu-in-tests-no

# CHROME_FLAGS += --wait-for-debugger=ui
# EXO_DEMO = exo_demo

WEBGL_DEMO = http://www.yuichiroharai.com/wgl/

# Pages
PAGE = https://www.google.com
# PAGE = https://solidot.org 
# PAGE = https://www.facebook.com
# PAGE = file:///usr/local/google/home/penghuang/sources/phuang.github.io/backdrop_with_mask.html
# PAGE = phuang.github.io/backdrop_with_mask.html
# PAGE = file:///usr/local/google/home/penghuang/sources/chromium/src/third_party/WebKit/LayoutTests/css3/filters/backdrop-filter-rendering.html
# PAGE = https://webkit.org/blog-files/3d-transforms/poster-circle.html
# PAGE = https://youtu.be/1roy4o4tqQM?t=60
# PAGE = https://www.youtube.com/watch?v=4pkhEyRoidk
# PAGE = https://www.youtube.com/watch?v=M2ciY79mlQw
# PAGE = https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/Tutorial/Animating_textures_in_WebGL
# PAGE = file:///usr/local/google/home/penghuang/sources/chromium/src/third_party/blink/web_tests/media/video-colorspace-yuv420.html
# PAGE = file:///`pwd`/third_party/blink/web_tests/media/color-profile-video-seek-filter.html
# PAGE = http://storage.googleapis.com/watk/v
# PAGE = phuang.github.io/fuzz/fuzz-70.html
# PAGE = https://codepen.io/sol0mka/pen/mICGv
# PAGE = third_party/blink/web_tests/media/alpha-video-playback.html
# PAGE = http://webglsamples.org/aquarium/aquarium.html
# PAGE = https://mrdoob.com/projects/chromeexperiments/ball-pool/
# PAGE = http://webglsamples.org/field/field.html
# PAGE = file:///home/penghuang/sources/chromium/src/content/test/data/gpu/pixel_canvas_display_linear-rgb.html
# PAGE = file:///home/penghuang/Downloads/a.pdf
# PAGE = https://pixijs.io/examples/\#/text/text.js
# PAGE = https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/Tutorial/Animating_textures_in_WebGL
# PAGE = https://developer.mozilla.org/en-US/docs/Web/API/Screen_Capture_API/Using_Screen_Capture
# PAGE = content/test/data/gpu/pixel_background_image.html
# PAGE = https://www.w3schools.com/html/html5_canvas.asp
# PAGE = https://delphic.me.uk/tutorials/webgl-text
# PAGE = https://mdn.github.io/webgl-examples/tutorial/sample8/
# PAGE = https://phuang.github.io/canvas.html
# PAGE = https://phuang.github.io/canvas2.html
# PAGE = https://www.therecord.com/?redirect=true
# PAGE = https://www.openstreetmap.org/#map=5/51.454/10.459
# PAGE = text_shadow.html
# PAGE = https://www.solidot.org
# PAGE = text_shadow_zoom.html
# PAGE = svg.html
# PAGE = crash.html
PAGE = third_party/blink/web_tests/fast/canvas/canvas-drawImage-video.html
PAGE = third_party/webgl/src/conformance-suites/2.0.0/conformance/glsl/constructors/glsl-construct-vec-mat-index.html
PAGE = third_party/webgl/src/conformance-suites/1.0.3/conformance/glsl/constructors/glsl-construct-vec-mat-index.html
PAGE = third_party/webgl/src/sdk/tests/conformance/glsl/constructors/glsl-construct-vec-mat-index.html
PAGE = third_party/webgl/src/sdk/tests/conformance/glsl/functions/glsl-function-abs.html


FEATURES := CanvasOopRasterization

CHROME_FLAGS = \
	--user-data-dir=/tmp/$(USER)-chromium \
	--enable-logging \
	$(MULL)

# CHROME_FLAGS += --enable-gpu-service-tracing --enable-gpu-service-logging --enable-gpu-debugging
# CHROME_FLAGS += --show-overdraw-feedback
CHROME_FLAGS += $(PAGE)
# CHROME_FLAGS += --use-gl=swiftshader

# CHROME_FLAGS += --use_virtualized_gl_contexts=0
CHROME_FLAGS += --disable-partial-raster

ADB = third_party/android_sdk/public/platform-tools/adb


.PHONY: chrome content mash mus_demo $(EXO_DEMO) exo_unittests clean media cc

all:
	$(NINJIA)

chrome_sandbox:
	$(NINJIA) chrome_sandbox

media:
	$(NINJIA) media

cc:
	$(NINJIA) cc

chrome:
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
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UiGpuRasterization \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--show-fps-counter-no \
		--no-sandbox

run_chrome_swangle: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=$(FEATURES) \
		--use-angle=swiftshader \
		--no-sandbox

run_chrome_swangle_vk: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=Vulkan,VulkanFromANGLE,$(FEATURES) \
		--use-angle=swiftshader \
		--no-sandbox

run_chrome_passthrough: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UiGpuRasterization \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--show-fps-counter-no \
		--use-gl=angle \
		--use-angle=gl \
	  --use-cmd-decoder=passthrough \
	  --no-sandbox

run_chrome_no_raw_draw: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=Vulkan,VulkanFromANGLE,NoRawDraw:IsUsingMSAA/true/TileSizeFactor/0.5 \
		--use-angle=swiftshader \
		--force-device-scale-factor=1 \
		--disable-vulkan-fallback-to-gl-for-testing \
		--enable-unsafe-webgpu \
		--use-webgpu-adapter=swiftshader \
		--ignore-gpu-blocklist  \
		--force-color-profile=scrgb-linear \
		--no-sandbox

run_chrome_raw_draw: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=Vulkan,VulkanFromANGLE,RawDraw:IsUsingMSAA/true/TileSizeFactor/0.5/SVGImageInLayer/true \
		--force-device-scale-factor=1 \
		--disable-vulkan-fallback-to-gl-for-testing \
		--enable-unsafe-webgpu \
		--ignore-gpu-blocklist  \
		--force-color-profile=scrgb-linear \
		--no-sandbox \
		--use-angle=swiftshader \
		--show-composited-layer-borders \

run_chrome_raw_draw_gl: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=RawDraw:UsingMSAA/true \
		--disable-features=Vulkan,VulkanFromANGLE \
		--use-angle=swiftshader \
		--force-device-scale-factor=1 \
		--disable-vulkan-fallback-to-gl-for-testing \
		--no-sandbox

run_chrome_oopd: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --enable-features=VizDisplayCompositor

run_chrome_oopd_skia: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --enable-features=VizDisplayCompositor,UseSkiaRendererNonDDL \
		--enable-threaded-texture-mailboxes \
		--use-gl=egl \
		--no-sandbox \
	$(NULL)

run_chrome_oopd_skia_vulkan: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --enable-features=VizDisplayCompositor,UseSkiaRendererNonDDL \
		--no-sandbox \
	$(NULL)

run_chrome_oopd_skia_ddl: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer \
		--disable-features=Mus \
		--no-sandbox

run_chrome_oopd_skia_ddl_vk: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer \
		--disable-features=Mus \
		--use-vulkan=native

run_chrome_oopd_skia_ddl_oopr: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer \
		--disable-features=Mus \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--no-sandbox

run_chrome_oopd_skia_ddl_oopr_hud: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer \
		--disable-features=Mus \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--show-fps-counter \
		--ui-show-fps-counter \
		--no-sandbox

#		--enable-gpu-service-tracing \
#		--enable-gpu-service-logging \
#		--enable-gpu-debugging \

run_chrome_skia_no_gpu_rasterization: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization \
		--no-sandbox

run_chrome_skia: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization,UiOopRasterization \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--no-sandbox

run_chrome_skia_wayland: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization,UiOopRasterization,Vulkan \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--ozone-platform=wayland \
		--no-sandbox


run_chrome_vk_no_gpu_rasterization: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization \
		--use-vulkan=native \
		--disable-perfetto --trace-to-console=foo \
		--disable-gpu-rasterization \
		--enable-zero-copy \
		--no-sandbox

run_chrome_vk: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization,Vulkan \
		--force-color-profile=srgb \
		--ensure-forced-color-profile \
		--use-vulkan=native \
		--disable-software-compositing-fallback \
		--enable-unsafe-webgpu \
		--force-gpu-rasterization \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--test-type=gpu \
		--enable-experimental-web-platform-features \
		--disable-gpu-sandbox \
		# --no-sandbox \
	  # --use-cmd-decoder=validating \
		# --enable-gpu-debugging \
		# --enable-gpu-benchmarking \
		$(NULL)

run_chrome_vk_ooor: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization,UiOopRasterization \
		--enable-oop-rasterization \
		--enable-oop-rasterization-ooor \
		--use-vulkan=native \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--enable-unsafe-webgpu \
		--no-sandbox

run_chrome_vk_with_angle: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization,Vulkan \
		--enable-oop-rasterization \
		--use-vulkan=native \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--use-gl=angle --use-angle=vulkan \
		--no-sandbox

run_chrome_swiftshader_vk: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization,UiOopRasterization,Vulkan \
		--enable-oop-rasterization \
		--use-vulkan=swiftshader \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--no-sandbox


run_chrome_swiftshader_vk_no_surface: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization \
		--enable-oop-rasterization \
		--use-vulkan=swiftshader \
		--disable-vulkan-surface \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--disable-perfetto --trace-to-console=foo \
		--no-sandbox


run_chrome_vk_no_vulkan_surface: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization \
		--enable-oop-rasterization \
		--use-vulkan=native \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--disable-vulkan-surface \
		--show-fps-counter-no \
		--use-gl=angle \
		--use-angle=vulkan \
		--disable-gpu-sandbox \
	  --no-sandbox
		#--use-gl=swiftshader \

run_chrome_vk_angle: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization,Vulkan \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--show-fps-counter-no \
		--use-angle=vulkan \
		--disable-gpu-sandbox \
		--vulkan=native \
	  	$(NULL)
	# --no-sandbox

run_chrome_oopd_skia_ddl_oopr_vk_hud: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UseSkiaDeferredDisplayList,UiGpuRasterization \
		--disable-features=Mus \
		--enable-oop-rasterization \
		--use-vulkan=native \
		--enable-gpu-rasterization \
		--trace-to-console=vulkan \
		--use-raster-context-provider \
		--show-fps-counter \
		--ui-show-fps-counter \
		--force-gpu-rasterization

run_chrome_oopd_skia_ddl_swiftshader: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UseSkiaDeferredDisplayList \
		--disable-features=Mus

run_chrome_oopd_oopr: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--use-raster-context-provider \
		--no-sandbox \
		$(NULL)

run_chrome_oopd_oopr_gpu_debugging: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--use-raster-context-provider \
		--no-sandbox \
		--enable-gpu-debugging \
		$(NULL)

run_chrome_hud: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor \
		--show-fps-counter \
		--ui-show-fps-counter \
		--no-sandbox \
		$(NULL)

run_chrome_oopd_oopr_hud: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=VizDisplayCompositor \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--use-raster-context-provider \
		--show-fps-counter \
		--ui-show-fps-counter \
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
	$(OUT)/chrome  $(CHROME_FLAGS) \
		--enable-features=UseSkiaRenderer \
		--trace-to-console=penghuang3

run_chrome_vulkan: chrome
	$(OUT)/chrome  $(CHROME_FLAGS) --use-vulkan=native  --disable-gpu


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
	$(OUT)/vulkan_demo --ozone-platform=wayland

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

content_browsertests_skia_ddl:
	$(NINJIA) content_browsertests && $(OUT)/content_browsertests \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UseSkiaDeferredDisplayList \
		--gtest_filter=$(GTEST_FILTER)


content_browsertests_vk:
	$(NINJIA) content_browsertests && $(OUT)/content_browsertests \
		--enable-gpu \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization,Vulkan \
		--use-gl=any \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--disable-gpu-process-crash-limit \
		--gtest_filter=$(GTEST_FILTER) \
		--gtest_repeat=1 \
		--gtest_break_on_failure \
		# --no-sandbox

content_browsertests:
	$(NINJIA) $@ && $(OUT)/$@ \
		--gtest_filter=$(GTEST_FILTER) \
		$(CHROME_FLAGS)

components_browsertests:
	$(NINJIA) $@ && $(OUT)/$@ \
		--gtest_filter=$(GTEST_FILTER) \
		$(CHROME_FLAGS)


content_shell:
	$(NINJIA) $@ && $(OUT)/$@

content_shell_raw_draw:
	$(NINJIA) content_shell && $(OUT)/content_shell --enable-features=Vulkan,VulkanFromANGLE,RawDraw --no-sandbox $(PAGE)


browser_tests:
	$(NINJIA) $@ && xvfb-run $(OUT)/$@ --gtest_filter=$(GTEST_FILTER) --no-sandbox

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
	$(NINJIA) cc_unittests && $(OUT)/cc_unittests --gtest_filter=$(GTEST_FILTER) $(GTEST_FLAGS)

ui_service_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

content_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

cc_perftests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

gpu_perftests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

gpu_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER) $(GTEST_FLAGS)

gfx_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER) $(GTEST_FLAGS)


viz_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER) $(GTEST_FLAGS)

gfx_unittests:
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
		file://`pwd`/ppapi/examples/compositor/compositor.html \

run_ppapi_compositor_skia_ddl_oopr: chrome ppapi_example_compositor
	$(OUT)/chrome \
		--register-pepper-plugins="$(OUT)/libppapi_example_compositor.so;application/x-ppapi-example-compositor" \
		file://`pwd`/ppapi/examples/compositor/compositor.html \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UseSkiaDeferredDisplayList \
		--disable-features=Mus \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \


run_mus_ppapi_compositor: chrome ppapi_example_compositor mash
	$(OUT)/chrome --mash $(CHROME_FLAGS) \
		--register-pepper-plugins="$(OUT)/libppapi_example_compositor.so;application/x-ppapi-example-compositor" \
		file://`pwd`/ppapi/examples/compositor/compositor.html

smoothness.top_25_smooth: chrome
	./tools/perf/run_benchmark run --browser=desktop \
		--browser-executable=$(OUT)/chrome \
		--extra-browser-args="--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization --enable-oop-rasterization --enable-gpu-rasterization --force-gpu-rasterization --enable-oop-rasterization-ooor" \
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
		penghuang@penghuang-win.c.googlers.com:/cygdrive/c/src

deploy_windows:
	ninja -C out_win/Release -j 128 chrome
	make rsync

deploy_gl_tests:
	ninja -C out_win/Release -j 128 gl_tests
	make rsync

chrome_apk:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental

chrome_public_apk:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental

run_system_webview_shell_apk_with_monochrome:
	$(NINJIA) monochrome_apk system_webview_shell_apk
	$(OUT)/bin/monochrome_apk install --non-incremental
	$(OUT)/bin/system_webview_shell_apk install --non-incremental
	$(OUT)/bin/system_webview_shell_apk launch

monochrome_64:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental

run_monochrome_apk:
	$(OUT)/bin/monochrome_apk launch --args=" \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization \
		--enable-oop-rasterization \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--disable-software-compositing-fallback \
		$(PAGE)

run_monochrome_apk_vk:
	$(OUT)/bin/monochrome_apk launch --args=" \
		--enable-features=VizDisplayCompositor,UseSkiaRenderer,UiGpuRasterization \
		--enable-oop-rasterization \
		--use-vulkan=native \
		--enable-gpu-rasterization \
		--force-gpu-rasterization \
		--disable-software-compositing-fallback \
		$(PAGE)"

trichrome_webview_apk:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental

trichrome_chrome_google_apk:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental

trichrome_webview_google_apk:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental

trichrome_library_google_apk:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental

chrome_apk:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental

run_chrome_apk: chrome_apk
	$(OUT)/bin/chrome_apk launch \
		--args=""
 
run_chrome_apk_angle: chrome_apk
	$(OUT)/bin/chrome_apk launch \
		--args="--enable-features=DefaultPassthroughCommandDecoder --disable-features=VulkanNo,UseSkiaRendererNo"

run_chrome_apk_vulkan: chrome_apk
	$(OUT)/bin/chrome_apk launch \
		--args="--enable-features=Vulkan"

run_chrome_apk_vulkan_with_black-list: chrome_apk
	$(OUT)/bin/chrome_apk launch \
		--args="--enable-features=Vulkan<FakeStudy --force-fieldtrials=FakeStudy/FakeGroup --force-fieldtrial-params=FakeStudy.FakeGroup:BlockListByDevice/generic_x86_64"

run_chrome_apk_gl: chrome_apk
	$(OUT)/bin/chrome_apk launch \
		--args="--enable-features=DefaultPassthroughCommandDecoder --enable-low-end-device-mode-no"

run_chrome_apk_noskia: chrome_apk
	$(OUT)/bin/chrome_apk launch --args="--disable-features=UseSkiaRenderer --disable-oop-rasterization-no"
  
run_chrome_apk_skia_vk: chrome_apk
	$(OUT)/bin/chrome_apk launch --args="--enable-features=useskiarenderer,Vulkan"

run_chrome_apk_vk: chrome_apk
	$(OUT)/bin/chrome_apk launch --args="--enable-features=UseSkiaRenderer,VizDisplayCompositor,DefaultEnableOopRasterization,Vulkan,DisableSurfaceControlForWebview-no --enable-oop-rasterization --enable-gpu-rasterization --force-gpu-rasterization --enable-low-end-device-mode-no --disable-gpu-sandbox"

system_webview_apk:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental

system_webview_shell_apk:
	$(NINJIA) system_webview_shell_apk
	$(OUT)/bin/$@ install --non-incremental

run_system_webview_shell_apk:
	$(OUT)/bin/system_webview_shell_apk launch

webview_instrumentation_apk:
	$(NINJIA) $@
	$(OUT)/bin/$@ install --non-incremental
	$(OUT)/bin/$@ launch --args="--enable-features=UseSkiaRendererNonDDL --webview-enable-shared-image --show-fps-counter"

adb_shell:
	$(ADB) shell

adb_logcat:
	$(ADB) logcat -c 
	$(ADB) logcat \
		chromium:V ANGLE:V DEBUG:V *:S
adb_logcat_all:
	$(ADB) logcat -c 
	$(ADB) logcat

android_symbol:
	third_party/android_platform/development/scripts/stack --output-directory=$(OUT)


print_out:
	echo $(OUT)

set_webview_flags:
	build/android/adb_system_webview_command_line \
		--enable-features=UseSkiaRenderer

vulkan_tests:
	$(NINJIA) $@
	$(OUT)/$@ --gtest_filter=$(GTEST_FILTER)


vulkan_tests_wayland:
	$(NINJIA) vulkan_tests
	$(OUT)/vulkan_tests --gtest_filter=$(GTEST_FILTER) --ozone-platform=wayland

vulkan_tests_apk:
	$(NINJIA) $@
	$(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

gl_tests:
	$(NINJIA) $@
	$(OUT)/$@  --gtest_filter=$(GTEST_FILTER)

adb_switch_vk:
	adb root && adb shell su 0 setenforce 0 && adb shell setprop debug.hwui.renderer skiavk && adb shell stop && adb shell start

goma:
	goma_ctl ensure_stop
	goma_ctl ensure_start
