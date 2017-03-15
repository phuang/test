WEBGL_DEMO = http://www.yuichiroharai.com/wgl/
OUT = out/brand
OUT = out/mus_Debug
OUT = out/link
OUT = out_cyan/Release
OUT = out/release
OUT = out_link/mus
OUT = out/mus
NINJIA_ARGS = -j1000 -k50 -C $(OUT)
NINJIA = ninja $(NINJIA_ARGS)

DEVICE_IP = 100.107.103.78
STRIP_FLAGS = --strip-flags=-d
TARGET_FLAGS = --target-dir=/tmp/chrome --mount  --mash
TARGET_FLAGS = --target-dir=/tmp/chrome --mount
DEPLOY_FLAGS = \
	--build-dir=$(OUT) \
	$(TARGET_FLAGS) \
	--to=$(DEVICE_IP) \
	--debug \
	$(STRIP_FLAGS) \
	--nostartui \
	$(NULL)


CHROME_FLAGS =  \
								--show-composited-layer-borders \
								--show-fps-counter \
								--enable-wayland-server \
								--no-sandbox \
								--user-data-dir=/tmp/penghuang-mustash

CHROME_FLAGS =  \
								--enable-wayland-server \
								--no-sandbox \
								--user-data-dir=/tmp/penghuang-mustash

# CHROME_FLAGS += --wait-for-debugger=ui
# EXO_DEMO = exo_demo

.PHONY: chrome content mash mus_demo $(EXO_DEMO) exo_unittests clean

chrome:
	$(NINJIA) chrome chrome_sandbox nacl_helper

content:
	$(NINJIA) content

mash:
	$(NINJIA) mash:all

mus_demo:
	$(NINJIA) mus_demo

exo_demo:
	$(NINJIA) exo_demo

deploy_chrome: chrome mash mus_demo
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

run_mus_chrome: mash chrome
	$(OUT)/chrome --mash http://www.google.com $(CHROME_FLAGS)

run_mus_chrome_youtube: mash chrome
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


TEST_FILTER = --gtest_filter=TouchTest.OnTouchMotion
# TEST_FILTER =
exo_unittests:
	$(NINJIA) $@ && $(OUT)/$@ \
		$(TEST_FILTER)

components_unittests:
	$(NINJIA) $@ && $(OUT)/$@

display_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

components_perftests:
	$(NINJIA) $@ && $(OUT)/$@

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


views_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

telemetry_perf_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

mus_ws_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

mash_unittests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

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
