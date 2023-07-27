//
// Created by Voidcom on 2023/7/17.
// 一个普通的三角形
//
#include "include/default.h"

#ifdef _Triangle_

#include "EGLManager.h"
#include "ShaderManager.h"

extern EGLManager *eglManager;
extern ShaderManager *shaderManager;

void onDraw() {
    //顶点颜色
    GLfloat color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    //绘制图形的顶点坐标
    GLfloat vVertices[3 * 3] = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
    };
    shaderManager->setViewPortAndUseProgram(eglManager->width, eglManager->height,
                                            GL_COLOR_BUFFER_BIT);

    //将顶点坐标数组传给着色器，第一个参数是位置，和着色器代码的 layout(location = 0) 对应。
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);
    glVertexAttrib4fv(1, color);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
}

void onShutDown() {
    glDeleteProgram(shaderManager->programObject);
}

int esMain() {
    if (!eglManager->esCreateWindow(eglManager->ES_WINDOW_RGB)) {
        LogE<char>(_Triangle_, "EGL初始化失败");
        return false;
    }
    //顶点着色器
    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "layout(location = 1) in vec4 a_color;      \n"
            "out vec4 v_color;                          \n"
            "void main()                              \n"
            "{                                        \n"
            "    v_color = a_color;                     \n"
            "   gl_Position = vPosition;              \n"
            "}                                        \n";
    //片段着色器
    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "in vec4 v_color;                             \n"
            "out vec4 fragColor;                          \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = v_color;                      \n"
            "}                                            \n";

    if (!shaderManager->init(vShaderStr, fShaderStr))return false;
    shaderManager->drawFunc = onDraw;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif