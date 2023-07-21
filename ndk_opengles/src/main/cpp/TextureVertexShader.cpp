//
// Created by Voidcom on 2023/7/21.
// 使用顶点着色器来 transform 对象以透视方式绘制旋转立方体
//

#include "include/default.h"

#ifdef _TextureVertexShader_

#include <cstring>
#include "EGLManager.h"
#include "ShaderManager.h"
#include "TransformUtils.h"

extern EGLManager *eglManager;
extern ShaderManager *shaderManager;
TransformUtils transformUtils;

int numIndices;
//顶点数组
GLfloat *vertices;
//索引
GLuint *indices;
GLfloat angle = 45.0f;
//GLfloat mvpMatrix[4][5];
ESMatrix mvpMatrix;

/**
 * 生成立方体纹理顶点
 * @param numSlices 切片数量
 * @param radius 半径
 * @return
 */
int genCube(float scale) {
    int i;
    int numVertices = 24;
    int numIndices_ = 36;

    GLfloat cubeVerts[] = {
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
    };

    // Allocate memory for buffers
    vertices = new GLfloat[sizeof(GLfloat) * 3 * numVertices];
    std::memcpy(vertices, cubeVerts, sizeof(cubeVerts));

    for (i = 0; i < numVertices * 3; i++) {
        vertices[i] *= scale;
    }

    // Generate the indices
    GLuint cubeIndices[] = {
            0, 2, 1,
            0, 3, 2,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 15, 14,
            12, 14, 13,
            16, 17, 18,
            16, 18, 19,
            20, 23, 22,
            20, 22, 21
    };
    indices = new GLuint[sizeof(GLuint) * numIndices_];
    std::memcpy(indices, cubeIndices, sizeof(cubeIndices));

    return numIndices_;
}

void onDraw() {
    shaderManager->setViewPortAndUseProgram(eglManager->width, eglManager->height,
                                            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertices);
    glEnableVertexAttribArray(0);

    glVertexAttrib4f(1, 1.0f, 0.0f, 0.0f, 1.0f);

    glUniformMatrix4fv(shaderManager->mvpLoc, 1, GL_FALSE, &mvpMatrix.m[0][0]);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

    glDisableVertexAttribArray(0);
}

void onUpdate(float time) {
    ESMatrix perspective;
    ESMatrix modelview;

    angle += time * 40.0f;
    if (angle >= 360.0f)angle -= 360.0f;
    //计算窗口宽高比
    float aspect = (GLfloat) eglManager->width / (GLfloat) eglManager->height;
//    Matrix perspective, modelView;
//    transformUtils.matrixLoadIdentity(&perspective);
//    transformUtils.perspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);
//
//    transformUtils.matrixLoadIdentity(&modelView);
//    transformUtils.translate(&modelView, 0.0, 0.0, -2.0);
//    transformUtils.rotate(&modelView, angle, 1, 0, 1);
//    transformUtils.matrixMultiply((Matrix *) &mvpMatrix, modelView, perspective);


    // Generate a perspective matrix with a 60 degree FOV
    transformUtils.matrixLoadIdentity(&perspective);
    transformUtils.perspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);

    // Generate a model view matrix to rotate/translate the cube
    transformUtils.matrixLoadIdentity(&modelview);

    // Translate away from the viewer
    transformUtils.translate(&modelview, 0.0, 0.0, -2.0);

    // Rotate the cube
    transformUtils.rotate(&modelview, angle, 1.0, 0.0, 1.0);

    // Compute the final MVP by multiplying the
    // modevleiw and perspective matrices together
    transformUtils.matrixMultiply(&mvpMatrix, &modelview, &perspective);
}

void onShutDown() {
    glDeleteProgram(shaderManager->programObject);
    delete vertices;
    delete indices;
}

int esMain() {
    if (!eglManager->esCreateWindow(eglManager->ES_WINDOW_RGB | eglManager->ES_WINDOW_DEPTH)) {
        LogE<char>(_TextureVertexShader_, "EGL初始化失败");
        return false;
    }
    //顶点着色器
    const char vShaderStr[] =
            "#version 300 es                             \n"
            "uniform mat4 u_mvpMatrix;                   \n"
            "layout(location = 0) in vec4 a_position;    \n"
            "layout(location = 1) in vec4 a_color;       \n"
            "out vec4 v_color;                           \n"
            "void main()                                 \n"
            "{                                           \n"
            "   v_color = a_color;                       \n"
            "   gl_Position = u_mvpMatrix * a_position;  \n"
            "}                                           \n";
    //片段着色器
    const char fShaderStr[] =
            "#version 300 es                                \n"
            "precision mediump float;                       \n"
            "in vec4 v_color;                               \n"
            "layout(location = 0) out vec4 outColor;        \n"
            "void main()                                    \n"
            "{                                              \n"
            "  outColor = v_color;                          \n"
            "}                                              \n";
    transformUtils = TransformUtils();
    if (!shaderManager->init(vShaderStr, fShaderStr))return false;
    //获取统一变量位置
    shaderManager->mvpLoc = glGetUniformLocation(shaderManager->programObject, "u_mvpMatrix");

    numIndices = genCube(1.0f);

    shaderManager->drawFunc = onDraw;
    shaderManager->updateFunc = onUpdate;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif