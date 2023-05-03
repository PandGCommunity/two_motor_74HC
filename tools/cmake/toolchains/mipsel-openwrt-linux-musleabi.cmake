# /media/ussh/B/OPENIPC/openwrt_18_clean/staging_dir/toolchain-mipsel_24kc_gcc-7.3.0_musl/bin

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_PROCESSOR mips)

# Specify the cross compiler.
set(CMAKE_C_COMPILER mipsel-openwrt-linux-musl-gcc CACHE FILEPATH "C compiler")
set(CMAKE_CXX_COMPILER mipsel-openwrt-linux-musl-g++ CACHE FILEPATH "C++ compiler")
set(CMAKE_ASM_COMPILER mipsel-openwrt-linux-musl-gcc CACHE FILEPATH "ASM compiler")

# Search libraries only under *target* paths.
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
