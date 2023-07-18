// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// Instancing.c
//
//    Demonstrates drawing multiple objects in a single draw call with
//    geometry instancing
//
#include "include/default.h"

#ifdef TEST

#include <stdlib.h>
#include <math.h>
#include "esUtil.h"

#ifdef _WIN32
#define srandom srand
#define random rand
#endif


#define NUM_INSTANCES   6
#define POSITION_LOC    0
#define COLOR_LOC       1
#define MVP_LOC         2

typedef struct {
    // Handle to a program object
    GLuint programObject;

    // VBOs
    GLuint positionVBO;
    GLuint colorVBO;
    GLuint mvpVBO;
    GLuint indicesIBO;
} UserData;

///
// Initialize the shader and program object
//
int Init(ESContext *esContext) {

    UserData *userData = esContext->userData;
    const char vShaderStr[] =
            "#version 300 es                             \n"
            "layout(location = 0) in vec4 a_position;    \n"
            "layout(location = 1) in vec4 a_color;       \n"
            "layout(location = 2) in mat4 a_mvpMatrix;   \n"
            "out vec4 v_color;                           \n"
            "void main()                                 \n"
            "{                                           \n"
            "   v_color = a_color;                       \n"
            "   gl_Position = a_mvpMatrix * a_position;  \n"
            //            "   float u = float(gl_InstanceID / 3);  \n"
            //            "   float v = float(gl_InstanceID % 3);  \n"
            //            "   gl_Position = a_position + vec4(v * 0.5f, u * -0.5f, 0, 0);  \n"
            "}                                           \n";

    const char fShaderStr[] =
            "#version 300 es                                \n"
            "precision mediump float;                       \n"
            "in vec4 v_color;                               \n"
            "layout(location = 0) out vec4 outColor;        \n"
            "void main()                                    \n"
            "{                                              \n"
            "  outColor = v_color;                          \n"
            "}                                              \n";

    // Load the shaders and get a linked program object
    userData->programObject = esLoadProgram(vShaderStr, fShaderStr);

    GLfloat cubeVerts[] = {
            -1.5f, 1.5f, 0,
            -2.0f, 1.0f, 0,
            -1.0f, 1.0f, 0,

            -1.5f + 1, 1.5f, 0,
            -2.0f + 1, 1.0f, 0,
            -1.0f + 1, 1.0f, 0,

            -1.5f + 2, 1.5f, 0,
            -2.0f + 2, 1.0f, 0,
            -1.0f + 2, 1.0f, 0,

            -1.5f, 1.5f - 1, 0,
            -2.0f, 1.0f - 1, 0,
            -1.0f, 1.0f - 1, 0,

            -1.5f + 1, 1.5f - 1, 0,
            -2.0f + 1, 1.0f - 1, 0,
            -1.0f + 1, 1.0f - 1, 0,

            -1.5f + 2, 1.5f - 1, 0,
            -2.0f + 2, 1.0f - 1, 0,
            -1.0f + 2, 1.0f - 1, 0,
    };
    for (int i = 0; i < 54; ++i) {
        if ((i + 1) % 3 != 0) {
            cubeVerts[i] = cubeVerts[i] * 0.5f;
        }
    }
    GLuint indices[18];
    for (int i = 0; i < 18; ++i) {
        indices[i] = i;
    }
    // 定义顶点元素索引缓冲区对象，并赋值
    glGenBuffers(1, &userData->indicesIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->indicesIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 18, indices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glGenBuffers(1, &userData->positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->positionVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * 18 * sizeof(GLfloat), cubeVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    {
        GLfloat colors[24] = {
                0.5f, 0.0f, 0.0f, 1.0f,
                0.0f, 0.5f, 0.0f, 1.0f,
                0.0f, 0.0f, 0.5f, 1.0f,

                0.0f, 0.0f, 1.0f, 1.0f,
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 1.0f
        };
        glGenBuffers(1, &userData->colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, userData->colorVBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    {
        glGenBuffers(1, &userData->mvpVBO);
        glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(ESMatrix), NULL, GL_DYNAMIC_DRAW);
    }

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return GL_TRUE;
}

/**
 * 根据时间更新MVP矩阵
 * @param deltaTime 与上次更新的时间差值
 */
void Update(ESContext *esContext, float deltaTime) {
    UserData *userData = (UserData *) esContext->userData;
    ESMatrix *matrixBuf;
    ESMatrix perspective;
    float aspect;
    int instance = 0;
    int numRows = 2;
    int numColumns = 3;


    // 计算窗口的横纵比
    aspect = (GLfloat) esContext->width / (GLfloat) esContext->height;

    // 创建一个投影矩阵
    esMatrixLoadIdentity(&perspective);
    //设置这个矩阵角度为60
    esPerspective(&perspective, 0.0f, aspect, 1.0f, 20.0f);

    glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
    //映射矩阵缓冲区对象，获取GPU内存中矩阵数据指针
    matrixBuf = (ESMatrix *) glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(ESMatrix) * NUM_INSTANCES,
                                              GL_MAP_WRITE_BIT);

    // 计算每个实例的 MVP矩阵，以不同的方式平移和旋转每个实例
    for (instance = 0; instance < NUM_INSTANCES; instance++) {
        ESMatrix modelview;
        float translateX = ((float) (instance % numRows) / (float) numRows) * 2.0f - 1.0f;
        float translateY = ((float) (instance / numColumns) / (float) numColumns) * 2.0f - 1.0f;
//        LOGD("--translate--", "translateX:%e; translateY:%e", translateX, translateY);

        // 生成模型视图矩阵以旋转平移立方体
        esMatrixLoadIdentity(&modelview);

        // Per-instance translation
        esTranslate(&modelview, translateX, translateY, -2.0f);

        //通过将模型矩阵、视图矩阵和投影矩阵相乘来计算最终的 MVP
        esMatrixMultiply(&matrixBuf[instance], &modelview, &perspective);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(ESContext *esContext) {
    UserData *userData = esContext->userData;

    // Set the viewport
    glViewport(esContext->width, esContext->height, esContext->width, esContext->height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(userData->programObject);

    // 加载顶点位置缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, userData->positionVBO);
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT,
                          GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(POSITION_LOC);

    // 加载颜色缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, userData->colorVBO);
    glVertexAttribPointer(COLOR_LOC, 4, GL_FLOAT,
                          GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(COLOR_LOC);
    glVertexAttribDivisor(COLOR_LOC, 1); // 每个实例一种颜色

    // 加载矩阵实例缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);

    // Load each matrix row of the MVP.  Each row gets an increasing attribute location.
    glVertexAttribPointer(MVP_LOC + 0, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) NULL);
    glVertexAttribPointer(MVP_LOC + 1, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) (sizeof(GLfloat) * 4));
    glVertexAttribPointer(MVP_LOC + 2, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) (sizeof(GLfloat) * 8));
    glVertexAttribPointer(MVP_LOC + 3, 4, GL_FLOAT, GL_FALSE, sizeof(ESMatrix),
                          (const void *) (sizeof(GLfloat) * 12));
    glEnableVertexAttribArray(MVP_LOC + 0);
    glEnableVertexAttribArray(MVP_LOC + 1);
    glEnableVertexAttribArray(MVP_LOC + 2);
    glEnableVertexAttribArray(MVP_LOC + 3);

    // One MVP per instance
    glVertexAttribDivisor(MVP_LOC + 0, 1);
    glVertexAttribDivisor(MVP_LOC + 1, 1);
    glVertexAttribDivisor(MVP_LOC + 2, 1);
    glVertexAttribDivisor(MVP_LOC + 3, 1);

    // 绑定 元素索引缓冲区对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->indicesIBO);


    glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
                            (const void *) NULL, 6);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

///
// Cleanup
//
void Shutdown(ESContext *esContext) {
    UserData *userData = esContext->userData;

    glDeleteBuffers(1, &userData->positionVBO);
    glDeleteBuffers(1, &userData->colorVBO);
    glDeleteBuffers(1, &userData->indicesIBO);

    // Delete program object
    glDeleteProgram(userData->programObject);
}


int esMain(ESContext *esContext) {
    esContext->userData = malloc(sizeof(UserData));

    esCreateWindow(esContext, "Instancing", 640, 480, ES_WINDOW_RGB);

    if (!Init(esContext)) {
        return GL_FALSE;
    }

    esRegisterShutdownFunc(esContext, Shutdown);
    //在Main.cpp方法允许顺序：Update -> Draw
    esRegisterUpdateFunc(esContext, Update);
    esRegisterDrawFunc(esContext, Draw);
    return GL_TRUE;
}

#endif