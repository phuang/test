WEBGL_DEMO = http://www.yuichiroharai.com/wgl/
OUT = out/mus_Release
# OUT = out/mus_Debug
# OUT = out/link
# OUT = out_cyan/Release
NINJIA_ARGS = -j1000 -k50 -C $(OUT)
NINJIA = ninja $(NINJIA_ARGS)

DEVICE_IP = 100.101.79.180
STRIP_FLAGS = --strip-flags=-d
TARGET_FLAGS = --target-dir=/tmp/chrome --mount  --mash
DEPLOY_FLAGS = \
	--build-dir=$(OUT) \
	$(TARGET_FLAGS) \
	--nostartui \
	--to=$(DEVICE_IP) \
	$(STRIP_FLAGS) \
	$(NULL)

CHROME_FLAGS =  --show-composited-layer-borders \
								--show-fps-counter --no-sandbox \
								--enable-wayland-server \
								--no-sandbox \
								--user-data-dir=/tmp/penghuang-mustash

# CHROME_FLAGS += --wait-for-debugger=ui
# EXO_DEMO = exo_demo

.PHONY: chrome content mash mus_demo $(EXO_DEMO) exo_unittests

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
	deploy_chrome $(DEPLOY_FLAGS)

deploy_mash: mash mus_demo
	deploy_chrome $(DEPLOY_FLAGS)

run_mus_demo: mus_demo mash chrome
	$(OUT)/mojo_runner mojo:mus_demo

run_exo_demo: exo_demo mash chrome
	$(OUT)/mojo_runner service:exo_demo


run_mus_demo_debug_ui: mus_demo
	$(OUT)/mojo_runner mojo:mus_demo --wait-for-debugger=ui

run_mus_mash: mash content $(EXO_DEMO)
	$(OUT)/mojo_runner service:mash_session --no-sandbox

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


TEST_FILTER = --gtest_filter=PointerTest.IgnorePointerEventDuringModal
# TEST_FILTER =
exo_unittests:
	$(NINJIA) $@ && $(OUT)/$@ \
		$(TEST_FILTER)

components_unittests:
	$(NINJIA) $@ && $(OUT)/$@


components_perftests:
	$(NINJIA) $@ && $(OUT)/$@

content_browsertests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

browser_tests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)

interactive_ui_tests:
	$(NINJIA) $@ && $(OUT)/$@ --gtest_filter=$(GTEST_FILTER)
