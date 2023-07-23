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

const int numIndices = 36;
const int numVertices = 24;
const GLfloat cubeColor[]={1.0f, 1.0f, 0.0f, 1.0f};

//顶点数组
GLfloat *vertices;
//渲染立方体所需的索引数量
GLuint *indices;
GLfloat angle = 0.0f;
Matrix mvpMatrix;

/**
 * 生成立方体纹理顶点
 * @param scale 立方体大小，使用1.0作为单位立方体
 * @return
 */
void genCube(float scale) {
    int i;
    // 因为立方体有6个面，所以需要24个顶点数据
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

//    std::push_heap(vertices.begin(), vertices.end(),cubeVerts);
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
    indices = new GLuint[sizeof(GLuint) * numIndices];
    std::memcpy(indices, cubeIndices, sizeof(cubeIndices));
}

void onDraw() {
    shaderManager->setViewPortAndUseProgram(eglManager->width, eglManager->height,
                                            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertices);
    glEnableVertexAttribArray(0);

    //立方体颜色
    glVertexAttrib4fv(1,cubeColor);

    glUniformMatrix4fv(shaderManager->mvpLoc, 1, GL_FALSE, &mvpMatrix[0][0]);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);

    glDisableVertexAttribArray(0);
}

void onUpdate(float time) {
    angle += time * 20.0f;
    if (angle >= 360.0f)angle -= 360.0f;
    //计算窗口宽高比
    float aspect = (GLfloat) eglManager->width / (GLfloat) eglManager->height;
    Matrix perspective, modelView;
    //生成具有60° FOV的透视矩阵
    transformUtils.matrixLoadIdentity(perspective);
    transformUtils.perspective(perspective, 60.0f, aspect, 1.0f, 20.0f);
    //生成模型视图以旋转平移立方体
    transformUtils.matrixLoadIdentity(modelView);
    //远离窗口
    transformUtils.translate(modelView, 0.0, 0.0, -3.0);
    //旋转立方体
    transformUtils.rotate(modelView, angle, 1, 1, 1);
    //将模型矩阵和透视矩阵相乘得到最终的MVP
    transformUtils.matrixMultiply(mvpMatrix, modelView, perspective);
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

    genCube(0.5f);

    shaderManager->drawFunc = onDraw;
    shaderManager->updateFunc = onUpdate;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif