//
// Created by Voidcom on 2023/7/18.
// 一个简单的2D纹理图像
// 最终图像会生成一个2x2矩形的2D纹理图像。至于为什么是矩形，这就和纹理过滤类型有关了。
//

#include "include/default.h"

#ifdef _Texture2D

#include "EGLManager.h"
#include "ShaderManager.h"

extern EGLManager *eglManager;
extern ShaderManager *shaderManager;

/**
 * 创建2D纹理
 * 纹理坐标原点(0,0)默认是在右下角，因此生成的2x2纹理为：
 * (0,1)blue | (1,1)yellow
 * ----------------------
 * (0,0)red  | (1,0) green
 *
 * @return
 */
GLuint createTexture2D() {
    GLuint textureID;
    GLubyte pixels[4 * 3] = {
            255, 0, 0, // Red
            0, 255, 0, // Green
            0, 0, 255, // Blue
            255, 255, 0  // Yellow
    };
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //创建一个空的纹理变量
    glGenTextures(1, &textureID);
    //绑定纹理类型
    glBindTexture(GL_TEXTURE_2D, textureID);
    //加载纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    //设置纹理过滤类型
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return textureID;
}

void onDraw() {
    GLfloat vVertices[] = {
            -0.25f, 0.25f, 0.0f,  // 顶点坐标
            0.0f, 0.0f,        // 纹理坐标
            -0.25f, -0.25f, 0.0f,
            0.0f, 1.0f,
            0.25f, -0.25f, 0.0f,
            1.0f, 1.0f,
            0.25f, 0.25f, 0.0f,
            1.0f, 0.0f
    };
    shaderManager->setViewPortAndUseProgram(eglManager->width, eglManager->height,
                                            GL_COLOR_BUFFER_BIT);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shaderManager->textureID);

    //设置偏移值
    glUniform1f(shaderManager->offset_X_Value, -0.5f);
    glUniform1f(shaderManager->offset_Y_Value, 0.5f);

    glUniform1i(shaderManager->samplerLoc, 0);
    GLushort indices[] = {0, 1, 2, 0, 2, 3};
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void onShutDown() {
    glDeleteTextures(1, &shaderManager->textureID);
    glDeleteProgram(shaderManager->programObject);
}

int esMain() {
    if (!eglManager->esCreateWindow(eglManager->ES_WINDOW_RGB)) {
        LogE<char>(_Texture2D, "EGL初始化失败");
        return false;
    }
    //顶点着色器
    char vShaderStr[] =
            "#version 300 es                            \n"
            "uniform float u_offset_x;                    \n"
            "uniform float u_offset_y;                    \n"
            "layout(location = 0) in vec4 a_position;   \n"
            "layout(location = 1) in vec2 a_texCoord;   \n"
            "out vec2 v_texCoord;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = a_position;               \n"
            "   gl_Position.x += u_offset_x;              \n"
            "   gl_Position.y += u_offset_y;              \n"
            "   v_texCoord = a_texCoord;                \n"
            "}                                          \n";
    //片段着色器
    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec2 v_texCoord;                                 \n"
            "layout(location = 0) out vec4 outColor;             \n"
            "uniform sampler2D s_texture;                        \n"
            "void main()                                         \n"
            "{                                                   \n"
            "  outColor = texture( s_texture, v_texCoord );      \n"
            "}                                                   \n";

    if (!shaderManager->init(vShaderStr, fShaderStr))return false;
    //获取统一变量位置
    shaderManager->samplerLoc = glGetUniformLocation(shaderManager->programObject, "s_texture");
    shaderManager->offset_X_Value = glGetUniformLocation(shaderManager->programObject,
                                                         "u_offset_x");
    shaderManager->offset_Y_Value = glGetUniformLocation(shaderManager->programObject,
                                                         "u_offset_y");

    shaderManager->textureID = createTexture2D();
    shaderManager->drawFunc = onDraw;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif