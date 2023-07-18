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
