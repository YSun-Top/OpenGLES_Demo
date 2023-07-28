//
// Created by Silen on 2023/7/28.
// 使用几何实例在调用一次API时绘制多个对象
//
#include "include/default.h"

#ifdef _Texture_2D_Instancing_

#include "EGLManager.h"
#include "ShaderManager.h"
#include "TransformUtils.h"

extern EGLManager *eglManager;
extern ShaderManager *shaderManager;
TransformUtils transformUtils;

//实例个数
const uint NUM_INSTANCES = 100;

const uint POSITION_LOC = 0;
const uint COLOR_LOC = 1;
const uint MVP_LOC = 2;

GLfloat angle[NUM_INSTANCES];

//buffer数组，包含VBO和IBO。 0:vertices; 1:color; 2:indices; 3:mvp
GLuint bufferObj[4];

void onUpdate(float time) {
    Matrix *matrixBuf, perspective;
    int numRows, numColumns;
    float aspect = eglManager->getWindowsAspect();
    //创建一个投影矩阵
    transformUtils.matrixLoadIdentity(perspective);
    //设置矩阵角度为60°
    transformUtils.perspective(perspective, 60.0f, aspect, 1.0f, 20.0f);
    glBindBuffer(GL_ARRAY_BUFFER, bufferObj[3]);
    //映射矩阵缓冲区对象，获取GPU内存中矩阵数据指针
    matrixBuf = static_cast<Matrix *>(glMapBufferRange(GL_ARRAY_BUFFER, 0,
                                                       sizeof(Matrix) * NUM_INSTANCES,
                                                       GL_MAP_WRITE_BIT));

    numColumns = numRows = static_cast<int>(sqrtf(NUM_INSTANCES));
    //计算每个实例的MVP矩阵，以不同的方式平移和旋转每个实例
    for (int instance = 0; instance < NUM_INSTANCES; ++instance) {
        Matrix modelView;
        float translateX = (float) (instance % numRows) / (float) numRows * 2.0f - 1.0f;
        float translateY = (float) (instance / numColumns) / (float) numColumns * 2.0f - 1.0f;

        // 生成模型视图矩阵
        transformUtils.matrixLoadIdentity(modelView);
        transformUtils.translate(modelView, translateX, translateY, -2.0f);
        //根据更新时间得到旋转角度
        angle[instance] += time * 40.0f;
        if (angle[instance] >= 360.0f) {
            angle[instance] -= 360.0f;
        }
        // 旋转立方体
        transformUtils.rotate(modelView, angle[instance], 1.0, 0.0, 1.0);
        // 通过讲模型矩阵和投影矩阵相乘得到最终的mvp矩阵
        transformUtils.matrixMultiply(matrixBuf[instance], modelView, perspective);
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void onDraw() {
    shaderManager->setViewPortAndUseProgram(eglManager->width, eglManager->height,
                                            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //加载顶点位置缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, bufferObj[0]);
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(POSITION_LOC);
    // 加载颜色缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, bufferObj[1]);
    glVertexAttribPointer(COLOR_LOC, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                          4 * sizeof(GLubyte), nullptr);
    glEnableVertexAttribArray(COLOR_LOC);

    //图元重启：用于控制顶点属性的复用，index在渲染第一个颜色属性后，之后每隔divisor个加载下一个属性。index和layout(location =index)有关
    glVertexAttribDivisor(COLOR_LOC, 1);

    // 加载矩阵实例缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, bufferObj[3]);
    for (int i = 0; i < 4; ++i) {
        glVertexAttribPointer(MVP_LOC + i, 4, GL_FLOAT, GL_FALSE,
                              sizeof(Matrix), (void *) (sizeof(GLfloat) * 4 * i));
        glEnableVertexAttribArray(MVP_LOC + i);
        glVertexAttribDivisor(MVP_LOC + i, 1);
    }


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObj[2]);
    glDrawElementsInstanced(GL_TRIANGLES, shaderManager->numIndices, GL_UNSIGNED_INT,
                            nullptr, NUM_INSTANCES);
}

void onShutDown() {
    glDeleteProgram(shaderManager->programObject);
    glDeleteBuffers(4, bufferObj);
}

/**
 * 初始化和绑定VBO、IBO
 */
void bindBufferObject() {
    CubeData cubeData{};
    shaderManager->genCube(0.1f, &cubeData);

    glGenBuffers(4, bufferObj);
    shaderManager->bindVBOBuffer(GL_ARRAY_BUFFER, bufferObj[0],
                                 24 * sizeof(GLfloat) * 3, cubeData.vertices);
    shaderManager->bindVBOBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObj[2],
                                 sizeof(GLuint) * shaderManager->numIndices, cubeData.indices);
    //生成随机颜色和随机角度
    {
        //颜色二维数组，每一行都是一组数据
        vector<vector<GLubyte>> colors(NUM_INSTANCES, vector<GLubyte>(4));
        srandom(0);
        for (int instance = 0; instance < NUM_INSTANCES; ++instance) {
            colors[instance][0] = random() % 255;//r
            colors[instance][1] = random() % 255;//g
            colors[instance][2] = random() % 255;//b
            colors[instance][3] = 0;

            angle[instance] = (float) (random() % 32768) / 32767.0f * 360.0f;
        }
        shaderManager->bindVBOBuffer(GL_ARRAY_BUFFER, bufferObj[1],
                                     NUM_INSTANCES * 4, &colors);
        shaderManager->bindVBOBuffer(GL_ARRAY_BUFFER, bufferObj[3],
                                     NUM_INSTANCES * sizeof(Matrix), nullptr);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    delete cubeData.vertices;
    delete cubeData.normals;
    delete cubeData.texCoords;
    delete cubeData.indices;
}

int esMain() {
    if (!eglManager->esCreateWindow(eglManager->ES_WINDOW_RGB)) {
        LogE<char>(_Texture_2D_Instancing_, "EGL初始化失败");
        return false;
    }
    //顶点着色器
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
    if (!shaderManager->init(vShaderStr, fShaderStr))return false;

    bindBufferObject();

    shaderManager->updateFunc = onUpdate;
    shaderManager->drawFunc = onDraw;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif