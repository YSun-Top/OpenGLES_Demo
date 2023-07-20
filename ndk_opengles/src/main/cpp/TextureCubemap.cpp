//
// Created by Voidcom on 2023/7/18.
// 一个简单的立方体贴图图像球体
//

#include "include/default.h"

#ifdef _TextureCubemap

#include <cstdlib>
#include "EGLManager.h"
#include "ShaderManager.h"

extern EGLManager *eglManager;
extern ShaderManager *shaderManager;

int numIndices;
GLfloat *vertices;
GLfloat *normals;
GLuint *indices;

/**
 * 创建一个1x1面的立方体，每个面有不同的颜色
 * 纹理坐标原点(0,0)默认是在右下角，因此生成的2x2纹理为：
 * (0,1)blue | (1,1)yellow
 * ----------------------
 * (0,0)red  | (1,0) green
 *
 * @return
 */
GLuint createTextureCubemap() {
    GLuint textureID;
    //立方体有6个面,每个面一个颜色
    GLubyte cubePixels[6][3] = {
            // Face 0 - Red
            {255, 0,   0},
            // Face 1 - Green,
            {0,   255, 0},
            // Face 2 - Blue
            {0,   0,   255},
            // Face 3 - Yellow
            {255, 255, 0},
            // Face 4 - Purple
            {255, 0,   255},
            // Face 5 - White
            {0,   0,   0}
    };
    //创建一个空的纹理变量
    glGenTextures(1, &textureID);
    //绑定纹理类型
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    //加载纹理
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[0]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[1]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[2]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[3]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[4]);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[5]);
    //设置纹理过滤类型
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return textureID;
}

void onDraw() {
    glViewport(0, 0, eglManager->width, eglManager->height);
    // 清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glUseProgram(shaderManager->programObject);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, normals);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shaderManager->textureID);

    glUniform1i(shaderManager->samplerLoc, 0);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

//    glDisableVertexAttribArray(0);
//    glDisableVertexAttribArray(1);
}

void onShutDown() {
    glDeleteTextures(1, &shaderManager->textureID);
    glDeleteProgram(shaderManager->programObject);
    delete vertices;
    delete normals;
//    delete indices;
}

int esMain() {
    if (!eglManager->esCreateWindow(eglManager->ES_WINDOW_RGB)) {
        LogE<char>(_TextureCubemap, "EGL初始化失败");
        return false;
    }
    //顶点着色器
    char vShaderStr[] =
            "#version 300 es                            \n"
            "layout(location = 0) in vec4 a_position;   \n"
            "layout(location = 1) in vec3 a_normal;     \n"
            "out vec3 v_normal;                         \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = a_position;               \n"
            "   v_normal = a_normal;                    \n"
            "}                                          \n";
    //片段着色器
    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec3 v_normal;                                   \n"
            "layout(location = 0) out vec4 outColor;             \n"
            "uniform samplerCube s_texture;                      \n"
            "void main()                                         \n"
            "{                                                   \n"
            "   outColor = texture( s_texture, v_normal );       \n"
            "}                                                   \n";

    if (!shaderManager->init(vShaderStr, fShaderStr))return false;
    //获取统一变量位置
    shaderManager->samplerLoc = glGetUniformLocation(shaderManager->programObject, "s_texture");

    shaderManager->textureID = createTextureCubemap();
    numIndices = shaderManager->genSphere(20, 0.75f, &vertices, &normals, nullptr, &indices);

    shaderManager->drawFunc = onDraw;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif