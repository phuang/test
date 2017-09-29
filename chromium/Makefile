WEBGL_DEMO = http://www.yuichiroharai.com/wgl/
GTEST_FILTER := *
OUT := out/brand
OUT := out/mus_Debug
OUT := out/link
OUT := out_cyan/Release
OUT := out/release
OUT := out_link/mus
OUT := out/mus
# OUT = out/mus_Debug
# OUT =out/linux_chromium
# OUT=out_veyron_minnie/Release
NINJIA_ARGS = -j1000 -k50 -C $(OUT)
NINJIA = ninja $(NINJIA_ARGS)

DEVICE_IP = my_crbook
STRIP_FLAGS = --strip-flags="--strip-debug -w -N '*blink*' -w -N '*WebCore*' -w -N '*Sk*' -w -N '*content*'"
STRIP_FLAGS = --strip-flags="--strip-debug -w -N '*blink*' -w -N '*WebCore*' -w -N '*Sk*'"
# STRIP_FLAGS = --nostrip
TARGET_FLAGS = --target-dir=/tmp/chrome --mount  --mash
TARGET_FLAGS = --target-dir=/tmp/chrome --mount
TARGET_FLAGS =
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
								--enable-wayland-server \
								--no-sandbox \
								--user-data-dir=/tmp/penghuang-mustash \
 								--screen-config=1000x800
CHROME_FLAGS +=  \
								--show-composited-layer-borders \
								--ui-show-composited-layer-borders=renderpass,surface,layer \
								--show-fps-counter

# CHROME_FLAGS += --wait-for-debugger=ui
# EXO_DEMO = exo_demo

.PHONY: chrome content mash mus_demo $(EXO_DEMO) exo_unittests clean

chrome_sandbox:
	$(NINJIA) chrome_sandbox

chrome: chrome_sandbox
	$(NINJIA) chrome

test_chrome: chrome_sandbox
	$(NINJIA) test_chrome

content:
	$(NINJIA) content

mash:
	$(NINJIA) mash:all

mus_demo:
	$(NINJIA) mus_demo

exo_demo:
	$(NINJIA) exo_demo

# deploy_chrome: chrome mash mus_demo
deploy_chrome: chrome
	deploy_chrome $(DEPLOY_FLAGS)

deploy_chrome_so: chrome mash mus_demo
	GN_ARGS='is_component_build=true is_chrome_branded=true' deploy_chrome $(DEPLOY_FLAGS)

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

run_chrome: chrome
	$(OUT)/chrome  $(CHROME_FLAGS)

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
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

interactive_ui_tests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER) --no-sandbox

unit_tests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

ash_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

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

ui_service_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

cc_perftests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

gpu_perftests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

wayland_rects_client:
	$(NINJIA) $@ && $(OUT)/$@ --num-rects=10  --show-fps-counter  --benchmark-interval=60

wayland_simple_client:
	$(NINJIA) $@ && $(OUT)/$@

wayland_subsurface_client:
	$(NINJIA) $@ && $(OUT)/$@

wayland_client_perftests:
	$(NINJIA) $@ && $(OUT)/$@

wayland_client_perftests_mus:
	$(NINJIA) $@ && $(OUT)/$@  --mus

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
		--extra-browser-args="--mash" \
		smoothness.top_25_smooth

args:
	gn args $(OUT)

check:
	gn check $(OUT)

clean:
	$(NINJIA) -t clean

cros_flush:
	cros flash ssh://$(DEVICE_IP)  chromiumos_test_image.bin

cros_flush_usb:
	cros flash usb://  chromiumos_test_image.bin

ssh:
	ssh root@$(DEVICE_IP)
