### Build with CMake

```console
$ cmake -H. -Bbuild_mips \
    -DCMAKE_BUILD_TYPE=Release \
    -DPLATFORM_SDK_DIR=<PATH_TO_SDK> \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_TOOLCHAIN_FILE=tools/cmake/toolchains/mipsel-openwrt-linux-musleabi.cmake
$ cmake --build build_mips
```
