//
// Created by Voidcom on 2023/7/17.
//

#ifndef MY_APPLICATION_OPENGLES_EGLMANAGER_H
#define MY_APPLICATION_OPENGLES_EGLMANAGER_H

#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <android_native_app_glue.h>
#include <android/asset_manager.h>

#include "ShaderManager.h"


class EGLManager {
public:
    // esCreateWindow flag - RGB color buffer
    uint ES_WINDOW_RGB = 0;
    // esCreateWindow flag - ALPHA color buffer
    uint ES_WINDOW_ALPHA = 1;
    // esCreateWindow flag - depth buffer
    uint ES_WINDOW_DEPTH = 2;
    // esCreateWindow flag - stencil buffer
    uint ES_WINDOW_STENCIL = 4;
    // esCreateWindow flat - multi-sample buffer
    uint ES_WINDOW_MULTISAMPLE = 8;

    //窗口的宽高，在Android中这等于屏幕的宽高
    GLint width;
    GLint height;

    EGLNativeDisplayType eglDisplayType{};
    EGLNativeWindowType eglWindowType{};
    EGLDisplay eglDisplay;
    EGLContext eglContext{};
    EGLSurface eglSurface{};

    /**
     * 对EGL初始化，获取EGLDisplay，EGLContext等。其主要是为OpenGL ES创建一个windows，打开与窗口系统的通信渠道
     * @param esContext
     * @param flags
     * @return
     */
    GLboolean esCreateWindow(GLuint flags);

    /**
     * 检查是否支持 EGL_KHR_create_context 扩展，
     * 如果支持返回 EGL_OPENGL_ES3_BIT_KHR  否则返回 EGL_OPENGL_ES2_BIT
     * @return
     */
    GLint GetContextRenderableType() const;
};


#endif //MY_APPLICATION_OPENGLES_EGLMANAGER_H
