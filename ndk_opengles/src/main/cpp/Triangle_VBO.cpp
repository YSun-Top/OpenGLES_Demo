//
// Created by Voidcom on 2023/7/17.
// 一个使用缓冲区对象（VBO）的三角形
//
#include "include/default.h"

#ifdef _Triangle_VBO

#include "EGLManager.h"
#include "ShaderManager.h"

extern EGLManager *eglManager;
extern ShaderManager *shaderManager;

const uint VERTEX_POS_SIZE = 3; //顶点坐标大小，x、y、z
const uint VERTEX_COLOR_SIZE = 4; //颜色大小ARGB
const uint VERTEX_POS_INDEX = 0;  //顶点坐标变量位置和 layout(location = 0) 对应
const uint VERTEX_COLOR_INDEX = 1;    //顶点颜色变量位置

void onDraw() {
    //顶点颜色
    GLfloat color[4 * VERTEX_COLOR_SIZE] = {
            1.0f, 0.0f, 0.0f, 1.0f,   // c0
            0.0f, 1.0f, 0.0f, 1.0f,   // c1
            0.0f, 0.0f, 1.0f, 1.0f    // c2
    };
    //绘制图形的顶点坐标
    GLfloat vertexPos[3 * VERTEX_POS_SIZE] = {
            0.0f, 0.5f, 0.0f,        // v0
            -0.5f, -0.5f, 0.0f,        // v1
            0.5f, -0.5f, 0.0f         // v2
    };

    shaderManager->setViewPortAndUseProgram(eglManager->width, eglManager->height,
                                            GL_COLOR_BUFFER_BIT);

    //用于分配VBO大小，计算方式：每组数据个数 * 数据类型
    GLint vtxStrides[2] = {
            VERTEX_POS_SIZE * sizeof(GLfloat),
            VERTEX_COLOR_SIZE * sizeof(GLfloat)
    };

    if (shaderManager->vboIDs[0] == 0 &&
        shaderManager->vboIDs[1] == 0 &&
        shaderManager->vboIDs[2] == 0) {
        glGenBuffers(3,shaderManager->vboIDs);
        //顶点坐标
        shaderManager->bindVBOBuffer(GL_ARRAY_BUFFER, shaderManager->vboIDs[0],
                                     vtxStrides[0] * 3, vertexPos);
        //顶点颜色
        shaderManager->bindVBOBuffer(GL_ARRAY_BUFFER, shaderManager->vboIDs[1],
                                     vtxStrides[1] * 3, color);
        //元素索引，比如要画有3个顶点的三角形，则元素是3个。数据类型必须为GLubyte或GLushort
        GLushort indices[3] = {0, 1, 2};
        //EBO，用于储存顶点索引，glDrawElements通过索引去相应的缓冲区拿数据
        shaderManager->bindVBOBuffer(GL_ELEMENT_ARRAY_BUFFER, shaderManager->vboIDs[2],
                                     sizeof(GLushort) * 3, indices);
    }


    //position
    glBindBuffer(GL_ARRAY_BUFFER, shaderManager->vboIDs[0]);
    //启用该顶点属性，不启用GPU无法访问该顶点属性
    glEnableVertexAttribArray(VERTEX_POS_INDEX);
    //将数组数据传递到缓冲区对象
    glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, vtxStrides[0], nullptr);

    //color
    glBindBuffer(GL_ARRAY_BUFFER, shaderManager->vboIDs[1]);
    glEnableVertexAttribArray(VERTEX_COLOR_INDEX);
    glVertexAttribPointer(VERTEX_COLOR_INDEX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, vtxStrides[1], nullptr);

    //绑定元素索引
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shaderManager->vboIDs[2]);

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);

    glDisableVertexAttribArray(VERTEX_POS_INDEX);
    glDisableVertexAttribArray(VERTEX_COLOR_INDEX);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void onShutDown() {
    glDeleteProgram(shaderManager->programObject);
    glDrawBuffers(3,shaderManager->vboIDs);
}

int esMain() {
    if (!eglManager->esCreateWindow(eglManager->ES_WINDOW_RGB)) {
        LogE<char>(_Triangle_VBO, "EGL初始化失败");
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
    shaderManager->vboIDs = new GLuint[3];
    shaderManager->vboIDs[0] = 0;
    shaderManager->vboIDs[1] = 0;
    shaderManager->vboIDs[2] = 0;

    if (!shaderManager->init(vShaderStr, fShaderStr))return false;
    shaderManager->drawFunc = onDraw;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif