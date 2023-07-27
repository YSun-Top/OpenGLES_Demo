//
// Created by Voidcom on 2023/7/27.
// 一个三角形示例，它使用了映射缓冲区对象的方式加载数据
//

#include "include/default.h"

#ifdef _Triangle_MapBuffer_

#include "EGLManager.h"
#include "ShaderManager.h"

extern EGLManager *eglManager;
extern ShaderManager *shaderManager;

const uint VERTEX_POS_SIZE = 3; //顶点坐标大小，x、y、z
const uint VERTEX_COLOR_SIZE = 4; //颜色大小ARGB
const uint VERTEX_POS_INDEX = 0;  //顶点坐标变量位置和 layout(location = 0) 对应
const uint VERTEX_COLOR_INDEX = 1;    //顶点颜色变量位置

/**
 * 让vbo与数据建立映射关系
 * @tparam T 数据的类型，比如 GLfloat; GLushort
 * @param type vbo类型，比如GL_ARRAY_BUFFER; GL_ELEMENT_ARRAY_BUFFER
 * @param vboID vbo id,用于告诉GPU 数据的位置，在后面`glBindBuffer()`过程中会用到
 * @param stride 一组数据的长度
 * @param data 待绑定的数据
 * @return 0：绑定成功
 */
template<typename T>
int bindMappedBuf(GLenum type, GLuint vboID, int stride, T data) {
    //将VBO和与目标类型绑定
    shaderManager->bindVBOBuffer(type, vboID, stride, nullptr);
    auto *vtxMappedBuf = static_cast<T>(glMapBufferRange(type, 0,
                                                         stride,
                                                         GL_MAP_WRITE_BIT |
                                                         GL_MAP_INVALIDATE_BUFFER_BIT));
    if (vtxMappedBuf == nullptr) return -1;
    std::memcpy(vtxMappedBuf, data, stride);
    if (glUnmapBuffer(type) == GL_FALSE) return -2;

    return 0;
}

void onDraw() {
    //顶点信息数组，包含了顶点坐标和颜色
    GLfloat vertices[3 * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE)] = {
            0.0f, 0.5f, 0.0f,        // v0
            1.0f, 0.0f, 0.0f, 1.0f,  // c0
            -0.5f, -0.5f, 0.0f,        // v1
            0.0f, 1.0f, 0.0f, 1.0f,  // c1
            0.5f, -0.5f, 0.0f,        // v2
            0.0f, 0.0f, 1.0f, 1.0f,  // c2
    };

    shaderManager->setViewPortAndUseProgram(eglManager->width, eglManager->height,
                                            GL_COLOR_BUFFER_BIT);

    int vtxStride = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE);

    if (shaderManager->vboIDs[0] == 0 && shaderManager->vboIDs[1] == 0) {
        glGenBuffers(2, shaderManager->vboIDs);
        int result = bindMappedBuf<GLfloat *>(GL_ARRAY_BUFFER, shaderManager->vboIDs[0],
                                              vtxStride * 3, vertices);
        if (result == -1) {
            LogE<char>(_Triangle_MapBuffer_, "映射顶点缓冲区对象时出错。");
            return;
        } else if (result == -2) {
            LogE<char>(_Triangle_MapBuffer_, "取消映射顶点缓冲区对象时出错。");
            return;
        }

        GLushort indices[3] = {0, 1, 2};
        result = bindMappedBuf<GLushort *>(GL_ELEMENT_ARRAY_BUFFER, shaderManager->vboIDs[1],
                                           sizeof(GLushort) * 3, indices);
        if (result == -1) {
            LogE<char>(_Triangle_MapBuffer_, "映射元素数组缓冲区对象时出错。");
            return;
        } else if (result == -2) {
            LogE<char>(_Triangle_MapBuffer_, "取消映射元素数组缓冲区对象时出错。");
            return;
        }
    }

    //position
    glBindBuffer(GL_ARRAY_BUFFER, shaderManager->vboIDs[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shaderManager->vboIDs[1]);
    //启用该顶点属性，不启用GPU无法访问该顶点属性
    glEnableVertexAttribArray(VERTEX_POS_INDEX);
    glEnableVertexAttribArray(VERTEX_COLOR_INDEX);
    //将数组数据传递到缓冲区对象
    glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, vtxStride,
                          nullptr);
    glVertexAttribPointer(VERTEX_COLOR_INDEX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, vtxStride,
                          (void *) (VERTEX_POS_SIZE * sizeof(GLfloat)));

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
//    GLenum errCode = glGetError();
//    while (errCode != GL_NO_ERROR) {
//        LogE<char>(_Triangle_MapBuffer_, "error code:%d", errCode);
//        errCode = glGetError();
//    }

    // unbind
    glDisableVertexAttribArray(VERTEX_POS_INDEX);
    glDisableVertexAttribArray(VERTEX_COLOR_INDEX);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void onShutDown() {
    glDeleteProgram(shaderManager->programObject);
    glDrawBuffers(2, shaderManager->vboIDs);
    delete shaderManager->vboIDs;
}

int esMain() {
    if (!eglManager->esCreateWindow(eglManager->ES_WINDOW_RGB)) {
        LogE<char>(_Triangle_MapBuffer_, "EGL初始化失败");
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
    shaderManager->vboIDs = new GLuint[2];
    shaderManager->vboIDs[0] = 0;
    shaderManager->vboIDs[1] = 0;

    if (!shaderManager->init(vShaderStr, fShaderStr))return false;
    shaderManager->drawFunc = onDraw;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif
