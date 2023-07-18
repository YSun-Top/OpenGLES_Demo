//
// Created by Voidcom on 2023/7/17.
//

#include "EGLManager.h"

GLboolean EGLManager::esCreateWindow(GLuint flags) {
    width = ANativeWindow_getWidth(eglWindowType);
    height = ANativeWindow_getHeight(eglWindowType);
    eglDisplay = eglGetDisplay(eglDisplayType);
    //当返回 EGL_NO_DISPLAY 表示无法连接到窗口系统，这表示无法使用Opengl es
    if (eglDisplay == EGL_NO_DISPLAY)return GL_FALSE;

    EGLConfig config;
    //定义EGL实现的的主版本号，可能为NULL
    EGLint majorVersion;
    //定义EGL实现的的次要版本号，可能为NULL
    EGLint minorVersion;
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion))return GL_FALSE;

    {
        GLint numConfigs = 0;
        EGLint attribList[] =
                {
                        EGL_RED_SIZE, 5,
                        EGL_GREEN_SIZE, 6,
                        EGL_BLUE_SIZE, 5,
                        EGL_ALPHA_SIZE, (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
                        EGL_DEPTH_SIZE, (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
                        EGL_STENCIL_SIZE, (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
                        EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
                        // if EGL_KHR_create_context extension is supported, then we will use
                        // EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
                        EGL_RENDERABLE_TYPE, GetContextRenderableType(),
                        EGL_NONE
                };
        if (!eglChooseConfig(eglDisplay, attribList, &config, 1, &numConfigs))return GL_FALSE;
        //如果返回的配置个数小于1，说明没有匹配的配置
        if (numConfigs < 1)return GL_FALSE;
    }
    {
        EGLint format = 0;
        eglGetConfigAttrib(eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(eglWindowType, 0, 0, format);
    }
    eglSurface = eglCreateWindowSurface(eglDisplay, config, eglWindowType, nullptr);
    if (eglSurface == EGL_NO_SURFACE) {
        //获取失败的原因
        switch (eglGetError()) {
            case EGL_BAD_MATCH:
                break;
            case EGL_BAD_CONFIG:
                break;
            case EGL_BAD_NATIVE_WINDOW:
                break;
            case EGL_BAD_ALLOC:
                break;
        }
        return GL_FALSE;
    }
    // 创建EGL上下文
    eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, contextAttribs);
    if (eglContext == EGL_NO_CONTEXT)return GL_FALSE;
    // 设置当前的上下文
    if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext))return GL_FALSE;

    return GL_TRUE;
}

GLint EGLManager::GetContextRenderableType() const {
#ifdef EGL_KHR_create_context
    const char *extensions = eglQueryString(eglDisplay, EGL_EXTENSIONS);
    if (extensions != nullptr && strstr(extensions, "EGL_KHR_create_context"))
        return EGL_KHR_create_context;
#endif
    return EGL_OPENGL_ES3_BIT_KHR;
}
