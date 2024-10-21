# Build Update Stream for OHOS

1. Follow steps [get source code](https://chromium.googlesource.com/chromium/src/+/main/docs/linux/build_instructions.md#Get-the-code) to get upstream chromium source code.
2. Download HarmonyOS command line tootls from [huawei](https://developer.huawei.com/consumer/cn/download/command-line-tools-for-hmos). (The link may not work outside of China.)
3. Unextract the commandline-tools-windows-x64-5.0.3.900.zip to ${HOME} (You can put it to other place as well).
4. Add several symbol links in ${HOME}/command-line-tools. It fixes severa compile issues.
```shell
cd ${HOME}/command-line-tools/sdk/default/openharmony/native/sysroot/usr/include/
ln -s aarch64-linux-ohos aarch64-unknown-linux-ohos
ln -s x86_64-linux-ohos x86_64-unknown-linux-ohos

cd ${HOME}/command-line-tools/sdk/default/openharmony/native/sysroot/usr/lib/
ln -s aarch64-linux-ohos aarch64-unknown-linux-ohos
ln -s x86_64-linux-ohos x86_64-unknown-linux-ohos

cd ${HOME}/command-line-tools/sdk/default/openharmony/native/llvm/lib
ln -s aarch64-linux-ohos aarch64-unknown-linux-ohos
ln -s x86_64-linux-ohos x86_64-unknown-linux-ohos
 ```
5. Download prebuilt latest [llvm](https://drive.google.com/file/d/1ylDw3YgAtE9MEg-5bIp2lonNcnMUtnEs/view?usp=drive_link) for OHOS, and extra it to ${HOME}/chrome-prebuilts-ohos
6. Download prebuilt latest [rust-toolchain](https://drive.google.com/file/d/1Iecbs6ZkMjpDDJgyea0t3sH9I6BC-Kv3/view?usp=drive_link) for OHOS, and extra it to ${HOME}/chrome-prebuilts-ohos
```shell
cd /path/to/chromium/src/third_party
rm -rf rust-toolchain
ln -s ${HOME}/chrome-prebuilts-ohos/rust-toolchain .
```
7. Checkout my branch of chromium at...
8. Run `gn args out/Release` with below build arguments
```
target_os = "ohos"
target_cpu = "arm64"

is_debug = false
dcheck_always_on = true
is_component_build = true

ohos_ndk_level = ""
# replace it to sdk path on your file system
ohos_sdk_root = "/home/penghuang/sources/command-line-tools/sdk/default/openharmony"

# replace it to sdk path on your file system
clang_base_path = "/home/penghuang/sources/llvm-install"

# rust_sysroot_absolute = "/home/penghuang/chrome-prebuilts-ohos/rust-toolchain/"
# rust_bindgen_root = "/home/penghuang/sources/chrome-prebuilts-ohos/rust-toolchain"
# rustc_version = "rustc 1.83.0-dev (f5cd2c5888011d4d80311e5b771c6da507d860dd-2-llvmorg-20-init-6794-g3dbd929e chromium)"

# workaround some build errors
treat_warnings_as_errors = false

dawn_use_swiftshader = false
use_custom_libcxx = true

chrome_root_store_only = true
use_kerberos = false

# build errors with ohos and musl
use_allocator_shim = false
use_partition_alloc = false
use_partition_alloc_as_malloc = false
toolchain_allows_use_partition_alloc_as_malloc = false

# disable symbol
blink_symbol_level = 0
v8_symbol_level = 0
enable_nacl = false

use_ozone = true
use_aura = true

# try build graphite and dawn
skia_use_dawn = true

```
9. Build chromium conthen_shell
```shell
ninja -C out/Release content_shell
```
