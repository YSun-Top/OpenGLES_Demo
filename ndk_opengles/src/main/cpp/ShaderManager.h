//
// Created by Voidcom on 2023/7/17.
//

#ifndef MY_APPLICATION_OPENGLES_SHADERMANAGER_H
#define MY_APPLICATION_OPENGLES_SHADERMANAGER_H

#include <GLES3/gl3.h>
#include "LogUtils.h"

class ShaderManager {
public:
    GLuint programObject;
    void *platformData;

    void (*updateFunc)(float);

    void (*drawFunc)(void);

    void (*shutDownFunc)(void);

    GLuint *vboIDs;
    GLuint textureID;

    //采样器位置
    GLint samplerLoc;
    //分别在x和y偏移多少值
    GLint offset_X_Value;
    GLint offset_Y_Value;

    /**
     * 加载、编译着色器源码，并检查错误。
     * @param type 着色器类型，如：GL_VERTEX_SHADER、GL_FRAGMENT_SHADER
     * @param shaderCode 着色器源码
     * @return 创建成功将返回着色器对象，失败返回 0
     */
    GLuint loadShader(GLenum type, const char *shaderCode);

    /**
     * 对着色器初始化，内部会完成着色器加载，Program创建和链接
     * @param vShaderStr 顶点着色器代码
     * @param fShaderStr 片段着色器代码
     * @return
     */
    int init(const char *vShaderStr, const char *fShaderStr);

    void setViewPortAndUseProgram(GLint width, GLint height, GLbitfield mask);

    /**
     * 生成立方体的顶点数据，并分配内存，最后将结果储存在数组中并为 TRIANGLE_STRIP 生成索引列表
     * @param numSlices 立方体切片的数量
     * @param radius    半径
     * @param vertices 返回的顶点数组
     * @param normals   返回的法线数组
     * @param texCoords 返回的纹理数组
     * @param indices 索引数据
     * @return
     */
    int genSphere(int numSlices, float radius, GLfloat **vertices, GLfloat **normals,
                  GLfloat **texCoords, GLuint **indices);

    /**
     * 将顶点数据和缓冲区对象绑定
     * @param type 类型，比如GL_ARRAY_BUFFER、GL_ELEMENT_ARRAY_BUFFER
     * @param vboIDS 顶点缓冲区对象
     * @param size 分配的大小，每组数据个数 * 数据类型 * 多少组数据
     * @param data 顶点数据
     */
    template<typename T>
    void bindVBOBuffer(GLenum type, GLuint vboIDS, uint size, T data) {
        glBindBuffer(type, vboIDS);
        glBufferData(type, size, data, GL_STATIC_DRAW);
    }
};

#endif //MY_APPLICATION_OPENGLES_SHADERMANAGER_H
