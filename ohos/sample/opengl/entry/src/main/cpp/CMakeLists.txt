# the minimum version of CMake.
cmake_minimum_required(VERSION 3.5.0)

# project
project(Tetrahedron)

set(NATIVE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# include directory
include_directories(${NATIVE_ROOT_PATH}
                    ${NATIVE_ROOT_PATH}/include
                    ${NATIVE_ROOT_PATH}/include/util)

# build shared library
add_library(tetrahedron_napi SHARED
            module.cpp
            app_napi.cpp
            tetrahedron.cpp
            napi_manager.cpp
            napi_util.cpp)

# link OpenHarmony native library （Use OpenGL）
target_link_libraries(tetrahedron_napi PUBLIC EGL)
target_link_libraries(tetrahedron_napi PUBLIC GLESv3)
target_link_libraries(tetrahedron_napi PUBLIC hilog_ndk.z)
target_link_libraries(tetrahedron_napi PUBLIC ace_ndk.z)
target_link_libraries(tetrahedron_napi PUBLIC ace_napi.z)
target_link_libraries(tetrahedron_napi PUBLIC libc++.a)
target_link_libraries(tetrahedron_napi PUBLIC z)
target_link_libraries(tetrahedron_napi PUBLIC uv)
target_link_libraries(tetrahedron_napi PUBLIC libace_napi.z.so)
