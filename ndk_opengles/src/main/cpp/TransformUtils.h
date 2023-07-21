//
// Created by Voidcom on 2023/7/21.
//

#ifndef OPENGLES_DEMO_TRANSFORMUTILS_H
#define OPENGLES_DEMO_TRANSFORMUTILS_H


#include <GLES3/gl3.h>
#include <cstdlib>
#include <cstring>
#include <vector>

using std::vector;

const float PI = 3.1415926535897932384626433832795f;
//typedef vector<vector<float>> Matrix;

//typedef GLfloat Matrix[4][4];
typedef struct {
    GLfloat m[4][4];
} ESMatrix;


class TransformUtils {
public:
    /**
     * 初始化一个矩阵
     * @param result
     */
//    void matrixLoadIdentity(Matrix *result);

    /**
     * 将指定的矩阵与透视矩阵相乘，并返回新的矩阵
     * @param result
     * @param fovy
     * @param aspect
     * @param nearZ
     * @param farZ
     */
//    void perspective(Matrix *result, float fovy, float aspect, float nearZ, float farZ);
//
//    void frustum(Matrix *result, float left, float right, float bottom, float top, float nearZ,
//                 float farZ);
//
//    void matrixMultiply(Matrix *result, Matrix srcA, Matrix srcB);
//
//    void translate(Matrix *result, GLfloat x, GLfloat y, GLfloat z);
//
//    void rotate(Matrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

    void matrixLoadIdentity(ESMatrix *result);

    void perspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ);

    void frustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ,
                 float farZ);

    void matrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);

    void translate(ESMatrix *result, GLfloat x, GLfloat y, GLfloat z);

    void rotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
};


#endif //OPENGLES_DEMO_TRANSFORMUTILS_H
