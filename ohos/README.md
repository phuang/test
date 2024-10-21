# Build Update Stream for OHOS

1. Follow steps at [get source code](https://chromium.googlesource.com/chromium/src/+/main/docs/linux/build_instructions.md#Get-the-code) to get upstream chromium source code.
2. Download HarmonyOS command line tootls from [huawei](https://developer.huawei.com/consumer/cn/download/command-line-tools-for-hmos). The link may not work outside of China.
3. Unextract the commandline-tools-windows-x64-5.0.3.900.zip to ${HOME} (You can put it to other place as well).
4. Add several symbol links in ${HOME}/command-line-tools. It fixes severa compile issues.
```shell
cd ${HOME}/command-line-tools/sdk/default/openharmony/native/sysroot/usr/lib/
ln -s aarch64-linux-ohos aarch64-unknown-linux-ohos

cd ${HOME}/command-line-tools/sdk/default/openharmony/native/sysroot/usr/include/
ln -s aarch64-linux-ohos aarch64-unknown-linux-ohos

 ```
