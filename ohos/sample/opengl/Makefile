SIGN_TOOL_PATH = /home/penghuang/sources/command-line-tools/sdk/default/openharmony/toolchains/lib
APP_BUNDLE_NAME = "com.samples.ndkopengl"


all:

build-hap:
	hvigorw assembleHap

sign-profile: openharmony_sx.p7b

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
	hdc shell aa start -b ${APP_BUNDLE_NAME} -a EntryAbility
	hdc hilog