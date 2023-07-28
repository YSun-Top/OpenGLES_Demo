cmake_minimum_required(VERSION 3.4.1)
project("ndk_OpenGLES")
# Enable C++11
set(CMAKE_CXX_STANDARD 11)

set(lib_Source ../../../../opengl_common/src/main/cpp)
include_directories(${lib_Source}/include)
message("outPath: ${lib_Source}/include")
#开始构建app的共享库
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(
        ndk_OpenGLES
        SHARED
        old/Instancing.c

        EGLManager.cpp
        ShaderManager.cpp
        TransformUtils.cpp

        Triangle.cpp
        Triangle_VBO.cpp
        Triangle_MapBuffer.cpp

        Texture2D.cpp
        TextureCubemap.cpp
        TextureVertexShader.cpp
        Texture_2D_modes.cpp
        Texture_2D_mipmap.cpp
        Texture_2D_Instancing.cpp

        test.c
        Main.cpp
)

add_subdirectory(

        ../../../../opengl_common/src/main/cpp/
        ../../../../build/outLib/
)

target_include_directories(ndk_OpenGLES PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(
        ndk_OpenGLES
        opengl_common
)