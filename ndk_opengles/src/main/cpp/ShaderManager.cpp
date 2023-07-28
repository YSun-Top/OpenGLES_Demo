//
// Created by Voidcom on 2023/7/17.
//

#include "ShaderManager.h"

GLuint ShaderManager::loadShader(GLenum type, const char *shaderCode) {
    GLuint shader;
    //创建对应类型的着色器对象
    shader = glCreateShader(type);
    if (shader == 0)return 0;
    //加载源码
    glShaderSource(shader, 1, &shaderCode, nullptr);
    //编译
    glCompileShader(shader);
    GLint compiled;
    //获取编译状态
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    //编译成功
    if (compiled)return shader;

    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1) {
        char *log = new char[infoLen * sizeof(char)];
        glGetShaderInfoLog(shader, infoLen, nullptr, log);
        LogE<char>("Error compiling shader:\n%s\n", log);
        delete[] log;
    }
    glDeleteShader(shader);
    //失败返回 0
    return 0;
}

int ShaderManager::init(const char *vShaderStr, const char *fShaderStr) {
    // 创建一个程序对象
    GLuint programObj;
    programObj = glCreateProgram();
    if (programObj == 0)return 0;

    // 加载顶点和片段着色器
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vShaderStr);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderStr);

    //给程序对象附加着色器
    glAttachShader(programObj, vertexShader);
    glAttachShader(programObj, fragmentShader);

    glLinkProgram(programObj);

    GLint linked;
    //检查链接状态
    glGetProgramiv(programObj, GL_LINK_STATUS, &linked);

    //释放着色器资源
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //链接成功
    if (linked) {
        programObject = programObj;
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        return true;
    }
    GLint msgLen = 0;
    glGetProgramiv(programObj, GL_INFO_LOG_LENGTH, &msgLen);
    if (msgLen > 1) {
        char *msgLog = new char[msgLen];
        glGetProgramInfoLog(programObj, msgLen, nullptr, msgLog);
        LogE<char>("Error linking program:\n%s\n", msgLog);
        delete[] msgLog;
    }
    //失败需要删除程序对象，成功不需要
    glDeleteProgram(programObj);
    return 0;
}

void ShaderManager::setViewPortAndUseProgram(GLint width, GLint height, GLbitfield mask) {
    // 设置视图窗口，仅窗口内的内容会被绘制，窗口外的内容将被抛弃
    glViewport(0, 0, width, height);
    // 清除颜色缓冲区
    glClear(mask);
    glUseProgram(programObject);
}

void ShaderManager::genCube(float scale, CubeData *data) {
    int numVertices = 24;
    numIndices = 36;
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


    GLfloat cubeNormals[] = {
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
    };

    GLfloat cubeTex[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };

    data->vertices = new GLfloat[sizeof(GLfloat) * 3 * numVertices];
    memcpy(data->vertices, cubeVerts, sizeof(cubeVerts));
    for (int i = 0; i < numVertices * 3; ++i) {
        data->vertices[i] *= scale;
    }

    data->normals = new GLfloat[sizeof(GLfloat) * 3 * numVertices];
    memcpy(data->normals, cubeNormals, sizeof(cubeNormals));

    data->texCoords = new GLfloat[sizeof(GLfloat) * 2 * numVertices];
    memcpy(data->texCoords, cubeNormals, sizeof(cubeTex));

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
    data->indices = new GLfloat[sizeof(GLuint) * numVertices];
    memcpy(data->indices, cubeIndices, sizeof(cubeIndices));
}
