//
// Created by Voidcom on 2023/7/21.
//

#include "TransformUtils.h"


//void TransformUtils::matrixLoadIdentity(Matrix *result) {
//    memset(result, 0x0, sizeof(Matrix));
//    *result[0][0] = 1.0f;
//    *result[1][1] = 1.0f;
//    *result[2][2] = 1.0f;
//    *result[3][3] = 1.0f;
//}
//
//void
//TransformUtils::perspective(Matrix *result, float fovy, float aspect, float nearZ, float farZ) {
//    GLfloat frustumW, frustumH;
//    frustumH = tanf(fovy / 360.0f * PI) * nearZ;
//    frustumW = frustumH * aspect;
//    frustum(result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
//}
//
//void TransformUtils::frustum(Matrix *result, float left, float right, float bottom, float top,
//                             float nearZ, float farZ) {
//    float deltaX = right - left;
//    float deltaY = top - bottom;
//    float deltaZ = farZ - nearZ;
//    Matrix frust;
//
//    if (nearZ <= 0.0f || farZ <= 0.0f || deltaX <= 0.0f || deltaY <= 0.0f || deltaZ <= 0.0f) return;
//
//
//    frust[0][0] = 2.0f * nearZ / deltaX;
//    frust[0][1] = frust[0][2] = frust[0][3] = 0.0f;
//
//    frust[1][1] = 2.0f * nearZ / deltaY;
//    frust[1][0] = frust[1][2] = frust[1][3] = 0.0f;
//
//    frust[2][0] = (right + left) / deltaX;
//    frust[2][1] = (top + bottom) / deltaY;
//    frust[2][2] = -(nearZ + farZ) / deltaZ;
//    frust[2][3] = -1.0f;
//
//    frust[3][2] = -2.0f * nearZ * farZ / deltaZ;
//    frust[3][0] = frust[3][1] = frust[3][3] = 0.0f;
//
//    matrixMultiply(result, frust, *result);
//}
//
//void TransformUtils::matrixMultiply(Matrix *result, Matrix srcA, Matrix srcB) {
//    Matrix tmp;
//    int i;
//
//    for (i = 0; i < 4; i++) {
//        tmp[i][0] = (srcA[i][0] * srcB[0][0]) +
//                    (srcA[i][1] * srcB[1][0]) +
//                    (srcA[i][2] * srcB[2][0]) +
//                    (srcA[i][3] * srcB[3][0]);
//
//        tmp[i][1] = (srcA[i][0] * srcB[0][1]) +
//                    (srcA[i][1] * srcB[1][1]) +
//                    (srcA[i][2] * srcB[2][1]) +
//                    (srcA[i][3] * srcB[3][1]);
//
//        tmp[i][2] = (srcA[i][0] * srcB[0][2]) +
//                    (srcA[i][1] * srcB[1][2]) +
//                    (srcA[i][2] * srcB[2][2]) +
//                    (srcA[i][3] * srcB[3][2]);
//
//        tmp[i][3] = (srcA[i][0] * srcB[0][3]) +
//                    (srcA[i][1] * srcB[1][3]) +
//                    (srcA[i][2] * srcB[2][3]) +
//                    (srcA[i][3] * srcB[3][3]);
//    }
//
//    std::memcpy(result, &tmp, sizeof(Matrix));
//}
//
//void TransformUtils::translate(Matrix *result, GLfloat x, GLfloat y, GLfloat z) {
//
//    *result[3][0] += (*result[0][0] * x + *result[1][0] * y + *result[2][0] * z);
//    *result[3][1] += (*result[0][1] * x + *result[1][1] * y + *result[2][1] * z);
//    *result[3][2] += (*result[0][2] * x + *result[1][2] * y + *result[2][2] * z);
//    *result[3][3] += (*result[0][3] * x + *result[1][3] * y + *result[2][3] * z);
//}
//
//void TransformUtils::rotate(Matrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
//    GLfloat sinAngle, cosAngle;
//    GLfloat mag = sqrtf(x * x + y * y + z * z);
//
//    sinAngle = sinf(angle * PI / 180.0f);
//    cosAngle = cosf(angle * PI / 180.0f);
//
//    if (mag > 0.0f) {
//        GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
//        GLfloat oneMinusCos;
//        Matrix rotMat;
//
//        x /= mag;
//        y /= mag;
//        z /= mag;
//
//        xx = x * x;
//        yy = y * y;
//        zz = z * z;
//        xy = x * y;
//        yz = y * z;
//        zx = z * x;
//        xs = x * sinAngle;
//        ys = y * sinAngle;
//        zs = z * sinAngle;
//        oneMinusCos = 1.0f - cosAngle;
//
//        rotMat[0][0] = (oneMinusCos * xx) + cosAngle;
//        rotMat[0][1] = (oneMinusCos * xy) - zs;
//        rotMat[0][2] = (oneMinusCos * zx) + ys;
//        rotMat[0][3] = 0.0F;
//
//        rotMat[1][0] = (oneMinusCos * xy) + zs;
//        rotMat[1][1] = (oneMinusCos * yy) + cosAngle;
//        rotMat[1][2] = (oneMinusCos * yz) - xs;
//        rotMat[1][3] = 0.0F;
//
//        rotMat[2][0] = (oneMinusCos * zx) - ys;
//        rotMat[2][1] = (oneMinusCos * yz) + xs;
//        rotMat[2][2] = (oneMinusCos * zz) + cosAngle;
//        rotMat[2][3] = 0.0F;
//
//        rotMat[3][0] = 0.0F;
//        rotMat[3][1] = 0.0F;
//        rotMat[3][2] = 0.0F;
//        rotMat[3][3] = 1.0F;
//
//        matrixMultiply(result, rotMat, *result);
//    }
//}
void TransformUtils::matrixLoadIdentity(ESMatrix *result) {
    memset(result, 0x0, sizeof(ESMatrix));
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}

void
TransformUtils::perspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ) {
    GLfloat frustumW, frustumH;

    frustumH = tanf(fovy / 360.0f * PI) * nearZ;
    frustumW = frustumH * aspect;

    frustum(result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
}

void TransformUtils::frustum(ESMatrix *result, float left, float right, float bottom, float top,
                             float nearZ, float farZ) {
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    ESMatrix frust;

    if ((nearZ <= 0.0f) || (farZ <= 0.0f) ||
        (deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f)) {
        return;
    }

    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = (right + left) / deltaX;
    frust.m[2][1] = (top + bottom) / deltaY;
    frust.m[2][2] = -(nearZ + farZ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    matrixMultiply(result, &frust, result);
}

void TransformUtils::matrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB) {
    ESMatrix tmp;
    int i;

    for (i = 0; i < 4; i++) {
        tmp.m[i][0] = (srcA->m[i][0] * srcB->m[0][0]) + (srcA->m[i][1] * srcB->m[1][0]) +
                      (srcA->m[i][2] * srcB->m[2][0]) + (srcA->m[i][3] * srcB->m[3][0]);

        tmp.m[i][1] = (srcA->m[i][0] * srcB->m[0][1]) +
                      (srcA->m[i][1] * srcB->m[1][1]) +
                      (srcA->m[i][2] * srcB->m[2][1]) +
                      (srcA->m[i][3] * srcB->m[3][1]);

        tmp.m[i][2] = (srcA->m[i][0] * srcB->m[0][2]) +
                      (srcA->m[i][1] * srcB->m[1][2]) +
                      (srcA->m[i][2] * srcB->m[2][2]) +
                      (srcA->m[i][3] * srcB->m[3][2]);

        tmp.m[i][3] = (srcA->m[i][0] * srcB->m[0][3]) +
                      (srcA->m[i][1] * srcB->m[1][3]) +
                      (srcA->m[i][2] * srcB->m[2][3]) +
                      (srcA->m[i][3] * srcB->m[3][3]);
    }

    memcpy(result, &tmp, sizeof(ESMatrix));
}

void TransformUtils::translate(ESMatrix *result, GLfloat x, GLfloat y, GLfloat z) {
    result->m[3][0] += (result->m[0][0] * x + result->m[1][0] * y + result->m[2][0] * z);
    result->m[3][1] += (result->m[0][1] * x + result->m[1][1] * y + result->m[2][1] * z);
    result->m[3][2] += (result->m[0][2] * x + result->m[1][2] * y + result->m[2][2] * z);
    result->m[3][3] += (result->m[0][3] * x + result->m[1][3] * y + result->m[2][3] * z);
}

void TransformUtils::rotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    GLfloat sinAngle, cosAngle;
    GLfloat mag = sqrtf(x * x + y * y + z * z);

    sinAngle = sinf(angle * PI / 180.0f);
    cosAngle = cosf(angle * PI / 180.0f);

    if (mag > 0.0f) {
        GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
        GLfloat oneMinusCos;
        ESMatrix rotMat;

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
        rotMat.m[0][1] = (oneMinusCos * xy) - zs;
        rotMat.m[0][2] = (oneMinusCos * zx) + ys;
        rotMat.m[0][3] = 0.0F;

        rotMat.m[1][0] = (oneMinusCos * xy) + zs;
        rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
        rotMat.m[1][2] = (oneMinusCos * yz) - xs;
        rotMat.m[1][3] = 0.0F;

        rotMat.m[2][0] = (oneMinusCos * zx) - ys;
        rotMat.m[2][1] = (oneMinusCos * yz) + xs;
        rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
        rotMat.m[2][3] = 0.0F;

        rotMat.m[3][0] = 0.0F;
        rotMat.m[3][1] = 0.0F;
        rotMat.m[3][2] = 0.0F;
        rotMat.m[3][3] = 1.0F;

        matrixMultiply(result, &rotMat, result);
    }
}
