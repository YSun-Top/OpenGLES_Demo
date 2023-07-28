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

typedef GLfloat Matrix[4][4];


class TransformUtils {
public:
    /**
     * 初始化一个矩阵
     * @param result
     */
    void matrixLoadIdentity(Matrix& result);

    void perspective(Matrix& result, float fovy, float aspect, float nearZ, float farZ);

    void frustum(Matrix& result, float left, float right, float bottom, float top, float nearZ,
                 float farZ);

    /**
     * 将模型矩阵(modelView)和透视矩阵(perspective)相乘得到最终的MVP
     * @param result
     * @param srcA
     * @param srcB
     */
    void matrixMultiply(Matrix& result, Matrix& srcA, Matrix& srcB);

    /**
     * 远离摄像机
     * @param result 模型矩阵
     * @param x 修改该值会使图形偏离中心
     * @param y
     * @param z 当值为负时，远离摄像机，值为正时靠近摄像机。
     */
    void translate(Matrix& result, GLfloat x, GLfloat y, GLfloat z);

    /**
     * 计算立方体旋转
     * @param result 模型矩阵
     * @param angle 旋转的角度
     * @param x 在哪个轴旋转，比如 x=1时 ，图形会绕x旋转，形成前后滚动的效果
     * @param y
     * @param z
     */
    void rotate(Matrix& result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

};


#endif //OPENGLES_DEMO_TRANSFORMUTILS_H
