APP_BUNDLE_NAME = "com.samples.ndkopengl"

all:

entry-default-signed.hap: entry/build/default/outputs/default/entry-default-unsigned.hap
	sign_hap.sh

install: entry-default-signed.hap
	hdc install entry-default-signed.hap

stop:
	hdc shell aa force-stop ${APP_BUNDLE_NAME}

run:
	hdc shell aa start -b ${APP_BUNDLE_NAME} -a EntryAbility