# Build Upstream Chromium for OHOS

1. Follow steps to [install depot_tools](https://chromium.googlesource.com/chromium/src/+/main/docs/linux/build_instructions.md#install).
2. Checkout chromium source code branch ohos_support at https://github.com/phuang/chromium/tree/ohos_support
```shell
mkdir -p ${HOME}/sources/chromium
cd ${HOME}/sources/chromium
git clone --branch ohos_support git@github.com:phuang/chromium.git src
```
2. Download commandline-tools-linux-x64-5.0.3.900.tar.xz, llvm-install.tar.xz and rust-toolchain.tar.xz from [Google Drive](https://drive.google.com/drive/folders/19mz5nA8PWqA3e0wEk1cQNbgEnaTkZ4GB?usp=drive_link)
3. Extract commandline-tools-linux-x64-5.0.3.900.tar.xz & llvm-install.tar.xz to ${HOME}/sources (You can put it to other place as well).
4. Extract rust-toolchain.tar.xz to ${HOME}/chromium/src/third_party/ folder
5. Create ${HOME}/chromium/.gclient with below content
```json
solutions = [
  {
    "name": "src",
    "url": "https://chromium.googlesource.com/chromium/src.git",
    "managed": False,
    "custom_deps": {
      'src/third_party/llvm-build/Release+Asserts': None,
      'src/third_party/rust-toolchain': None,
      'src/third_party/angle': 'https://github.com/phuang/angle.git@ohos_support',
      'src/third_party/dawn': 'https://github.com/phuang/dawn.git@ohos_support',
      'src/third_party/ffmpeg': 'https://github.com/phuang/FFmpeg.git@ohos_support',
      'src/third_party/perfetto': 'https://github.com/phuang/perfetto.git@ohos_support',
      'src/third_party/skia': 'https://github.com/phuang/skia.git@ohos_support',
      'src/third_party/webrtc': 'https://github.com/phuang/webrtc.git@ohos_support',
      'src/v8': 'https://github.com/phuang/v8.git@ohos_support',
    },
    "custom_vars": {
      "rbe_instance": "projects/rbe-chromium-untrusted/instances/default_instance",
      "checkout_rust_toolchain_deps": True,
    },
  },
]
target_os = [
  "linux",
]
```
6. Run `gclient sync` in ${HOME}/sources/chromium/src to sync the tree
7. Run `gn args out/Release` with below build arguments
```
target_os = "ohos"
target_cpu = "arm64"
cc_wrapper = "ccache"

is_debug = false
dcheck_always_on = false
is_component_build = false

# Replace ${HOME} with your home path
ohos_native_root = "${HOME}/sources/command-line-tools/sdk/default/openharmony/native"
clang_base_path = "${HOME}/sources/llvm-install"

# workaround some build errors
treat_warnings_as_errors = false

dawn_use_swiftshader = false

# cxx args
use_custom_libcxx = true
# use_libcxx_modules = true

chrome_root_store_only = true
use_kerberos = false


# disable symbol
v8_symbol_level = 0
blink_symbol_level = 0

enable_nacl = false

# use_ozone = true
use_aura = true

# try build graphite and dawn
use_dawn = true
skia_use_dawn = true

# //components/signin/features.gni
enable_bound_session_credentials  = true

enable_printing = false
use_zygote = true
# use_static_angle = true

# enable_base_tracing = false
enable_screen_capture = false

use_bundled_fontconfig = true
enable_pdf = false
enable_screen_ai_service = false
enable_extensions = false
use_swiftshader = false
dawn_use_swiftshader  = false
```

8. Build chromium content_shell
```shell
ninja -C out/Release content_shell
```

9. Build and entry-default-signed.hap
```shell
cd ${HOME}/sources/chromium/src/content/shell/ohos/shell_hap
make content_shell && make signed-hap
```

10. [Optional] Build llvm and rust-toolchains from source code instead of using prebuilt from step 2
```shell
cd ${HOME}/sources/chromium/src/tools/clang/scripts
./build.py --bootstrap --with-ohos --without-android --without-fuchsia --install-dir ${HOME}/sources/llvm-install

cd ${HOME}/sources/chromium/src/tools/rust
./build_rust.py
```