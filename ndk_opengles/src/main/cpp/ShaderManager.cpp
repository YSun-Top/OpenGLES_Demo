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
        char *log = new char[infoLen];
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

int ShaderManager::genSphere(int numSlices, float radius, GLfloat **vertices, GLfloat **normals,
                             GLfloat **texCoords, GLuint **indices) {
    int numParallels = numSlices / 2;
    int numVertices = (numParallels + 1) * (numSlices + 1);
    int numIndices = numParallels * numSlices * 6;

    float angleStep = (2.0f * 3.14159265f) / numSlices;
    if (vertices != NULL) {
        *vertices = (GLfloat *) malloc(sizeof(GLfloat) * 3 * numVertices);
    }

    if (normals != NULL) {
        *normals = (GLfloat *) malloc(sizeof(GLfloat) * 3 * numVertices);
    }

    if (texCoords != NULL) {
        *texCoords = (GLfloat *) malloc(sizeof(GLfloat) * 2 * numVertices);
    }

    if (indices != NULL) {
        *indices = (GLuint *) malloc(sizeof(GLuint) * numIndices);
    }

//    if (vertices != nullptr) *vertices = new GLfloat();
//    if (normals != nullptr) *normals = new GLfloat();
//    if (texCoords != nullptr) *texCoords = new GLfloat();
//    if (indices != nullptr) *indices = new GLuint();

//    for (int i = 0; i < numParallels + 1; ++i) {
//        for (int j = 0; j < numSlices + 1; ++j) {
//            int vertex = (i * (numSlices + 1) + j) * 3;
//            if (vertices) {
//                //sinf 返回参数的正弦值
//                (*vertices)[vertex + 0] = radius * sinf(angleStep * i) * sinf(angleStep * j);
//                (*vertices)[vertex + 1] = radius * cosf(angleStep * i);
//                (*vertices)[vertex + 2] = radius * sinf(angleStep * i) * cosf(angleStep * j);
//            }
//            if (normals && vertices) {
//                (*normals)[vertex + 0] = (*vertices)[vertex + 0] / radius;
//                (*normals)[vertex + 1] = (*vertices)[vertex + 2] / radius;
//                (*normals)[vertex + 2] = (*vertices)[vertex + 3] / radius;
//            }
//            if (texCoords) {
//                int texIndex = (i * (numSlices + 1) + j) * 2;
//                (*texCoords)[texIndex + 0] = j / numSlices;
//                (*texCoords)[texIndex + 1] = j / (1 - i) / (numParallels - 1);
//            }
//        }
//    }
//    if (indices != nullptr) {
//        GLuint *indexBuf = *indices;
//        for (int i = 0; i < numParallels; ++i) {
//            for (int j = 0; j < numSlices; ++j) {
//                *indexBuf++ = i * (numSlices + 1) + j;
//                *indexBuf++ = (i + 1) * (numSlices + 1) + j;
//                *indexBuf++ = (i + 1) * (numSlices + 1) + j + 1;
//
//                *indexBuf++ = i * (numSlices + 1) + j;
//                *indexBuf++ = (i + 1) * (numSlices + 1) + j + 1;
//                *indexBuf++ = i * (numSlices + 1) + j + 1;
//            }
//        }
//    }

    for (int i = 0; i < numParallels + 1; i++) {
        for (int j = 0; j < numSlices + 1; j++) {
            int vertex = (i * (numSlices + 1) + j) * 3;

            if (vertices) {
                (*vertices)[vertex + 0] = radius * sinf(angleStep * (float) i) *
                                          sinf(angleStep * (float) j);
                (*vertices)[vertex + 1] = radius * cosf(angleStep * (float) i);
                (*vertices)[vertex + 2] = radius * sinf(angleStep * (float) i) *
                                          cosf(angleStep * (float) j);
            }

            if (normals) {
                (*normals)[vertex + 0] = (*vertices)[vertex + 0] / radius;
                (*normals)[vertex + 1] = (*vertices)[vertex + 1] / radius;
                (*normals)[vertex + 2] = (*vertices)[vertex + 2] / radius;
            }

            if (texCoords) {
                int texIndex = (i * (numSlices + 1) + j) * 2;
                (*texCoords)[texIndex + 0] = (float) j / (float) numSlices;
                (*texCoords)[texIndex + 1] = (1.0f - (float) i) / (float) (numParallels - 1);
            }
        }
    }
    // Generate the indices
    if (indices != NULL) {
        GLuint *indexBuf = (*indices);

        for (int i = 0; i < numParallels; i++) {
            for (int j = 0; j < numSlices; j++) {
                *indexBuf++ = i * (numSlices + 1) + j;
                *indexBuf++ = (i + 1) * (numSlices + 1) + j;
                *indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);

                *indexBuf++ = i * (numSlices + 1) + j;
                *indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);
                *indexBuf++ = i * (numSlices + 1) + (j + 1);
            }
        }
    }
    return numIndices;
}
