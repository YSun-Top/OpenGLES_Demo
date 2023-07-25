//
// Created by Silen on 2023/7/25.
// 演示2D纹理不同模式的纹理环绕方式
// 纹理坐标范围通常是（0,0）到（1,1）。
// 有四种不同的环绕方式：
// 1. GL_REPEAT——对纹理的默认行为。重复纹理图像。
// 2. GL_MIRRORED_REPEAT——和GL_REPEAT一样，但每次重复图片是镜像放置的。
// 3. GL_CLAMP_TO_EDGE——纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
// 4. GL_CLAMP_TO_BORDER——超出的坐标为用户指定的边缘颜色。

#include "include/default.h"

#ifdef _Texture_2D_modes_

#include <cstring>
#include "EGLManager.h"
#include "ShaderManager.h"
#include "TransformUtils.h"

extern EGLManager *eglManager;
extern ShaderManager *shaderManager;

/**
 * 生成 RGB8 的棋盘图形
 * @param w 宽
 * @param h 高
 * @param size 大小
 * @return
 */
GLubyte *genCheckImage(int w, int h, int size) {
    auto *pixels = new GLubyte[w * h * 3];
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {

            GLubyte rColor, bColor;
            if (x / size % 2 == 0) {
                rColor = (y / size % 2) * 255;
                bColor = (1 - (y / size % 2)) * 255;
            } else {
                rColor = (1 - (y / size % 2)) * 255;
                bColor = (y / size % 2) * 255;
            }
            pixels[(y * w + x) * 3] = rColor;
            pixels[(y * w + x) * 3 + 1] = 0;
            pixels[(y * w + x) * 3 + 2] = bColor;
        }
    }
    return pixels;
}

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
    int width = 256, height = 256;
    GLubyte *pixels = genCheckImage(width, height, 64);

    //创建一个空的纹理变量
    glGenTextures(1, &textureID);
    //绑定纹理类型
    glBindTexture(GL_TEXTURE_2D, textureID);
    //加载纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    //设置纹理过滤类型
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete pixels;
    return textureID;
}

void onDraw() {
    GLfloat vVertices[] = {
            -0.3f, 0.3f, 0.0f, 1.0f,  // Position 0
            -1.0f, -1.0f,              // TexCoord 0
            -0.3f, -0.3f, 0.0f, 1.0f, // Position 1
            -1.0f, 2.0f,              // TexCoord 1
            0.3f, -0.3f, 0.0f, 1.0f, // Position 2
            2.0f, 2.0f,              // TexCoord 2
            0.3f, 0.3f, 0.0f, 1.0f,  // Position 3
            2.0f, -1.0f               // TexCoord 3
    };
    GLushort indices[] = {0, 1, 2, 0, 2, 3};
    shaderManager->setViewPortAndUseProgram(eglManager->width, eglManager->height,
                                            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), vVertices);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &vVertices[4]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shaderManager->textureID);

    glUniform1i(shaderManager->samplerLoc, 0);
    glUniform1f(shaderManager->offset_Y_Value, 0.0f);

    //默认行为，对边缘执行重复纹理图形的策略
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glUniform1f(shaderManager->offset_X_Value, -0.7f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    //纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1f(shaderManager->offset_X_Value, 0.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    //和GL_REPEAT一样，但每次重复图片是镜像放置的
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glUniform1f(shaderManager->offset_X_Value, 0.7f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void onShutDown() {
    glDeleteProgram(shaderManager->programObject);
//    delete vertices;
//    delete indices;
}

int esMain() {
    if (!eglManager->esCreateWindow(eglManager->ES_WINDOW_RGB)) {
        LogE<char>(_Texture_2D_modes_, "EGL初始化失败");
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
            "   outColor = texture( s_texture, v_texCoord );     \n"
            "}                                                   \n";
    if (!shaderManager->init(vShaderStr, fShaderStr))return false;
    //获取统一变量位置
    shaderManager->offset_X_Value = glGetUniformLocation(shaderManager->programObject, "u_offset_x");
    shaderManager->offset_Y_Value = glGetUniformLocation(shaderManager->programObject, "u_offset_y");
    shaderManager->samplerLoc = glGetUniformLocation(shaderManager->programObject, "s_texture");
    shaderManager->textureID = createTexture2D();

    shaderManager->drawFunc = onDraw;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif
