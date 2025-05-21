# Build Upstream Chromium for OHOS

1. Follow steps to [install depot_tools](https://chromium.googlesource.com/chromium/src/+/main/docs/linux/build_instructions.md#install).
2. Checkout chromium source code branch ohos_support at https://github.com/phuang/chromium/tree/ohos_support_133
```shell
mkdir -p ${HOME}/sources/chromium
cd ${HOME}/sources/chromium
git clone --branch ohos_support https://github.com/phuang/chromium.git src
# Use --depth=1 to clone with only 1 commit history to reduce clone time  
# git clone --depth=1 --branch ohos_support_133 https://github.com/phuang/chromium.git src
```
2. Download [command-line-tools-linux-x86_64-5.0.3.900.tar.xz](https://drive.usercontent.google.com/download?id=1LbtM7iGqF5q5o9abVBWMKTlT1NglOM5Q&export=download&authuser=2&confirm=t&uuid=2bf26830-39e0-489c-bc2e-28144ca8e8ce&at=APvzH3qpLjDx5fMNf0e0BLae-MWF:1735589467950) and [rust-toolchain.tar.xz](https://drive.usercontent.google.com/download?id=1SEcXeEAJcYWKPIotm9o0Afe3Gw_YcYLs&export=download&authuser=0&confirm=t&uuid=9b1d4552-4a67-4855-9a4e-3c0999630396&at=APvzH3rTOR2-9-sPJZSOgoS3RJly:1735003699551) from [Google Drive](https://drive.google.com/drive/folders/19mz5nA8PWqA3e0wEk1cQNbgEnaTkZ4GB?usp=drive_link)
3. Extract `command-line-tools-linux-x86_64-5.0.3.900.tar.xz` to `${HOME}/sources` (You can put it to other place as well).
```shell
xz -d -T 0 -c command-line-tools-linux-x86_64-5.0.3.900.tar.xz | tar -xv  -C ${HOME}/sources
```
4. Extract `rust-toolchain.tar.xz` to `${HOME}/chromium/src/third_party` folder
```shell
xz -d -T 0 -c rust-toolchain.tar.xz | tar -xv -C ${HOME}/sources/chromium/src/third_party
```
5. Create ${HOME}/chromium/.gclient with below content
```python
vars = {
  'branch': 'ohos_support_135',
  # old branches
  # 'branch': 'ohos_support_133',
  # 'branch': 'ohos_support',
}

solutions = [
  {
    'name': 'src',
    'url': 'https://github.com/phuang/chromium.git' + '@' + vars['branch'],
    "managed": False,
    "custom_deps": {
      'src/third_party/llvm-build/Release+Asserts': None,
      'src/third_party/rust-toolchain': None,
      'src/third_party/angle': 'https://github.com/phuang/angle.git' + '@' + vars['branch'],
      'src/third_party/dawn': 'https://github.com/phuang/dawn.git' + '@' + vars['branch'],
      'src/third_party/ffmpeg': 'https://github.com/phuang/FFmpeg.git' + '@' + vars['branch'],
      'src/third_party/perfetto': 'https://github.com/phuang/perfetto.git' + '@' + vars['branch'],
      'src/third_party/skia': 'https://github.com/phuang/skia.git' + '@' + vars['branch'],
      'src/third_party/vulkan-headers/src': 'https://github.com/phuang/Vulkan-Headers.git' + '@' + vars['branch'],
      'src/third_party/webrtc': 'https://github.com/phuang/webrtc.git' + '@' + vars['branch'],
      'src/v8': 'https://github.com/phuang/v8.git' + '@' + vars['branch'],

      # Removing below deps can reduce gclient sync time
      'src/third_party/angle/third_party/VK-GL-CTS/src': None,
      'src/third_party/chromium-variations': None,
      'src/third_party/dawn/third_party/webgpu-cts': None,
    },
    'custom_vars': {
      'checkout_rust_toolchain_deps': True,
    },
    'custom_hooks': []
  },
]
```
6. Run `gclient sync` or `gclient sync --shallow` in ${HOME}/sources/chromium to sync the tree, using `--shallow` can reduce sync up time.
7. Run `gn args out/Release` with below build arguments
```shell
target_os = "ohos"
target_cpu = "arm64"
cc_wrapper = "ccache"

is_debug = false
dcheck_always_on = false
is_component_build = false

# Replace ${HOME} with your home path
ohos_native_root = "/home/penghuang/sources/command-line-tools/sdk/default/openharmony/native"
clang_base_path = "/home/penghuang/sources/command-line-tools/sdk/default/openharmony/native/llvm"

# workaround some build errors
treat_warnings_as_errors = false

# build graphite and dawn
use_dawn = true
skia_use_dawn = true
skia_use_vulkan = true
dawn_use_swiftshader = false
dawn_enable_vulkan = true

# cxx args
# use_custom_libcxx = true
# use_libcxx_modules = true
# libcxx_is_shared = false

chrome_root_store_only = true
use_kerberos = false

# disable symbol
v8_symbol_level = 0
blink_symbol_level = 0
enable_nacl = false

# use_ozone = true
use_aura = true

# //components/signin/features.gni
enable_bound_session_credentials  = true

enable_printing = false
use_zygote = true

# ANGLE args
# use_static_angle = true
angle_enable_vulkan = true
angle_enable_gl = true
angle_enable_swiftshader = false
angle_use_custom_libvulkan = false

# Build validating command decoder
enable_validating_command_decoder = true

# enable_base_tracing = false

use_bundled_fontconfig = true
enable_pdf = false
enable_screen_ai_service = false
enable_extensions = false

# Workaround v8 crash
v8_control_flow_integrity = false

```

8. Build chromium content_shell
```shell
ninja -C out/Release content_shell
```

9. Build entry-default-signed.hap
```shell
cd ${HOME}/sources/chromium/src/content/shell/ohos/shell_hap
make content_shell && make signed-hap
# The hap file entry-default-signed.hap will be created in this folder
# You can install it to a OHOS device.
```
10. [Optional] Config ccache for faster build, put below content in ` ${HOME}/.ccache/ccache.conf`
```shell
max_size = 10G
sloppiness = include_file_mtime

# change it to ${HOME}/sources
base_dir = /home/penghuang/sources

# Set it to true, if you have multiple chromium trees. So the same cache can be hit within multiple chromium trees
hash_dir = false
```
11. [Optional] Build llvm and rust-toolchains from source code instead of using prebuilt from step 2
```shell
cd ${HOME}/sources/chromium/src/tools/clang/scripts
./build.py --bootstrap --with-ohos --without-android --without-fuchsia --with-ccache --install-dir ${HOME}/sources/llvm-install
# ./build.py --bootstrap --with-ohos --without-android --without-fuchsia --with-ccache --skip-checkout --install-dir ${HOME}/sources/llvm-install

cd ${HOME}/sources/chromium/src/tools/rust
./build_rust.py
```

Note:
```shell
# tar llvm to a compressed llvm-x86_64.tar.xz
tar cvf - llvm | xz -z -T 0 > llvm-x86_64.tar.xz

# untar the compressed llvm-x86_64.tar.xz
xz -d -T 0 -c llvm-x86_64.tar.xz| tar -xv
```
