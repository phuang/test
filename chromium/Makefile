OUT= out/mus_Debug
# OUT = out/link
NINJIA_ARGS = -j1000 -k50 -C $(OUT)
NINJIA = ninja $(NINJIA_ARGS)

DEVICE_IP = 100.101.167.119
STRIP_FLAGS = --strip-flags=-d
DEPLOY_FLAGS = \
	--build-dir=$(OUT) \
	--target-dir=/tmp/chrome \
	--mount \
	--nostartui \
	--mash \
	--to=$(DEVICE_IP) \
	$(STRIP_FLAGS) \
	$(NULL)

.PHONY: chrome mash mus_demo

chrome:
	$(NINJIA) $(NINJIA_ARS) chrome

mash:
	$(NINJIA) $(NINJIA_ARS) mash:all

mus_demo:
	$(NINJIA) $(NINJIA_ARS) mus_demo

deploy_chrome: chrome mash mus_demo
	deploy_chrome $(DEPLOY_FLAGS)

deploy_mash: mash mus_demo
	deploy_chrome $(DEPLOY_FLAGS)

run_mus_demo: mus_demo
	$(OUT)/mojo_runner mojo:mus_demo --use-chrome-gpu-command-buffer-in-mus

run_mus_mash: mash
	$(OUT)/mojo_runner mojo:mash_session --use-chrome-gpu-command-buffer-in-mus

run_mus_chrome: mash chrome
	$(OUT)/chrome --mash --use-chrome-gpu-command-buffer-in-mus
