cmake_minimum_required(VERSION 3.4.1)
project("opengl_common")

# Enable C++11
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

#将native_app_glue构建为静态库
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(
        opengl_common

        STATIC

        esShader.c
        esShapes.c
        esTransform.c
        esUtil.c
)
target_include_directories(opengl_common PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(
        opengl_common
        android
        native_app_glue
        EGL
        GLESv3
        log)
