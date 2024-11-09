OUT=/home/penghuang/sources/chromium/src/out/Release
SIGN_TOOL_PATH = /home/penghuang/sources/command-line-tools/sdk/default/openharmony/toolchains/lib
APP_BUNDLE_NAME = "org.chromium.content_shell"

all:

content_shell:
	ninja -C $(OUT) content_shell

sign-profile: openharmony_sx.p7b

unsigned-hap: entry/build/default/outputs/default/entry-default-unsigned.hap

entry/build/default/outputs/default/entry-default-unsigned.hap: .libs.stamp .resfile.stamp
	hvigorw assembleHap

openharmony_sx.p7b: UnsgnedReleasedProfileTemplate.json
	java -jar ${SIGN_TOOL_PATH}/hap-sign-tool.jar sign-profile \
		-keyAlias "openharmony application profile release" \
		-signAlg "SHA256withECDSA" \
		-mode "localSign" \
		-profileCertFile "${SIGN_TOOL_PATH}/OpenHarmonyProfileRelease.pem" \
		-inFile "UnsgnedReleasedProfileTemplate.json" \
		-keystoreFile "${SIGN_TOOL_PATH}/OpenHarmony.p12" \
		-outFile "openharmony_sx.p7b" \
		-keyPwd "123456" \
		-keystorePwd "123456"

sign-hap: entry-default-signed.hap

entry-default-signed.hap: entry/build/default/outputs/default/entry-default-unsigned.hap openharmony_sx.p7b
	java -jar ${SIGN_TOOL_PATH}/hap-sign-tool.jar sign-app \
		-keyAlias "openharmony application release" \
		-signAlg "SHA256withECDSA" \
		-mode "localSign" \
		-appCertFile "${SIGN_TOOL_PATH}/OpenHarmonyApplication.pem" \
		-inFile "entry/build/default/outputs/default/entry-default-unsigned.hap" \
		-keystoreFile "${SIGN_TOOL_PATH}/OpenHarmony.p12" \
		-profileFile "openharmony_sx.p7b" \
		-keyPwd "123456" \
		-keystorePwd "123456" \
		-outFile "entry-default-signed.hap"

install: entry-default-signed.hap
	hdc install entry-default-signed.hap

stop:
	hdc shell aa force-stop ${APP_BUNDLE_NAME}

run:
	hdc shell hilog -b D -T chromium
	hdc shell hilog -r
	hdc shell aa start -b ${APP_BUNDLE_NAME} -a EntryAbility
	hdc shell hilog -T chromium,Faultlogger | tee log
	# hdc hilog | tee log
	# hdc shell "hilog -r && hilog -p off && hilog -b F && hilog -b E -T chromium && hilog -b I -T Faultlogger && hilog"

libs .libs.stamp: ${OUT}/libcontent_shell_content_view.so ${OUT}/libtest_trace_processor.so
	cp ${OUT}/libcontent_shell_content_view.so entry/libs/arm64-v8a
	cp ${OUT}/libtest_trace_processor.so entry/libs/arm64-v8a
	touch .libs.stamp

resfile:
	mkdir -p AppScope/resources/resfile/
	cp ${OUT}/icudtl.dat AppScope/resources/resfile/
	cp ${OUT}/v8_context_snapshot.bin AppScope/resources/resfile/
	cp ${OUT}/content_shell.pak AppScope/resources/resfile/
	touch .resfile.stamp

clean:
	hvigorw clean
	rm \
		entry-default-signed.hap \
		openharmony_sx.p7b \
		.libs.stamp
