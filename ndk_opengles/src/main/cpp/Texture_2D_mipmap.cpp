//
// Created by Voidcom on 2023/7/26.
// 生成Mipmap贴图链，并使用mipmap贴图链进行渲染

#include "include/default.h"

#ifdef _Texture_2D_mipmap_

#include "EGLManager.h"
#include "ShaderManager.h"
#include "TransformUtils.h"

extern EGLManager *eglManager;
extern ShaderManager *shaderManager;

struct Image {
    GLubyte *preImage= nullptr;
    GLubyte *newImage= nullptr;
};

/**
 * 生成 RGB8 的棋盘图形
 * @param w 宽，棋盘宽度
 * @param h 高，棋盘高度
 * @param size 棋盘格子的大小
 * @return
 */
GLubyte *genCheckImage(int w, int h, int size) {
    auto *pixels = new GLubyte[w * h * 3];
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            GLubyte rColor, bColor;
            if (x / size % 2 == 0) {
                rColor = (y / size % 2) * 255;
                bColor = (1 - (y / size % 2)) * 255;
            } else {
                rColor = (1 - (y / size % 2)) * 255;
                bColor = (y / size % 2) * 255;
            }
            pixels[(y * w + x) * 3] = rColor;
            pixels[(y * w + x) * 3 + 1] = 0;
            pixels[(y * w + x) * 3 + 2] = bColor;
        }
    }
    return pixels;
}

GLboolean genMipMap2D(Image *image, int w, int h, int *dstWidth,
                      int *dstHeight) {
    int texelSize = 3;
    *dstWidth = w / 2;
    *dstHeight = h / 2;
    if (*dstWidth <= 0)*dstWidth = 1;
    if (*dstHeight <= 0)*dstHeight = 1;

    if (image->newImage== nullptr){
        image->newImage = new GLubyte[sizeof(GLubyte) * texelSize * (*dstWidth) * (*dstHeight)];
    }

    int srcIndex[4];
    for (int y = 0; y < *dstHeight; ++y) {
        for (int x = 0; x < *dstWidth; ++x) {
            GLubyte r, g, b;
            r = g = b = 0;
            //计算上一张图像中 2x2 像素网格的偏移量以执行盒式过滤器
            srcIndex[0] = (y * 2 * w + x * 2) * texelSize;
            srcIndex[1] = (y * 2 * w + x * 2 + 1) * texelSize;
            srcIndex[2] = ((y * 2 + 1) * w + x * 2) * texelSize;
            srcIndex[3] = ((y * 2 + 1) * w + x * 2 + 1) * texelSize;

            //对所有像素求和
            for (int i: srcIndex) {
                r += image->preImage[i];
                g += image->preImage[i] + 1;
                b += image->preImage[i] + 2;
            }
            //计算平均结果
            r /= 4;
            g /= 4;
            b /= 4;
            image->newImage[(y * (*dstWidth) + x) * texelSize] = r;
            image->newImage[(y * (*dstWidth) + x) * texelSize + 1] = g;
            image->newImage[(y * (*dstWidth) + x) * texelSize + 2] = b;
        }
    }
    return GL_TRUE;
}

GLuint createMipMappedTexture2d() {
    GLuint textureID;
    int width = 256, height = 256;
    //生成棋盘的颜色数组
    GLubyte *pixels = genCheckImage(width, height, 8);
    //创建一个空的纹理变量
    glGenTextures(1, &textureID);
    //绑定纹理类型
    glBindTexture(GL_TEXTURE_2D, textureID);
    //加载纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    int level = 1;
    Image image;
    image.preImage = &pixels[0];
    while (width > 1 && height > 1) {
        int newWidth, newHeight;
        genMipMap2D(&image, width, height, &newWidth, &newHeight);
        glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, newWidth, newHeight,
                     0, GL_RGB, GL_UNSIGNED_BYTE, image.newImage);
        // 为下一次迭代设置对比图像
        image.preImage = image.newImage;
        level++;
        // 宽度和高度的一半
        width = newWidth;
        height = newHeight;
    }
    delete image.preImage;
    delete image.newImage;

    //设置纹理过滤类型
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    delete pixels;
    return textureID;
}

/**
 * 绘制棋盘，根据不同的纹理环绕模式，呈现不同的边缘效果。
 */
void onDraw() {
    //顶点和纹理坐标，其中position的第四个值可以理解为深度值，就是与摄像机的距离。
    //如果需要图像变成没有深度效果的矩形，将值设置为1.0f
    GLfloat vVertices[] = {
            -0.5f, 0.5f, 0.0f, 1.5f,  // Position 0
            0.0f, 0.0f,              // TexCoord 0
            -0.5f, -0.5f, 0.0f, 0.75f, // Position 1
            0.0f, 1.0f,              // TexCoord 1
            0.5f, -0.5f, 0.0f, 0.75f, // Position 2
            1.0f, 1.0f,              // TexCoord 2
            0.5f, 0.5f, 0.0f, 1.5f,  // Position 3
            1.0f, 0.0f               // TexCoord 3
    };
    GLushort indices[] = {0, 1, 2, 0, 2, 3};
    shaderManager->setViewPortAndUseProgram(eglManager->width, eglManager->height,
                                            GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), vVertices);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), &vVertices[4]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shaderManager->textureID);

    glUniform1i(shaderManager->samplerLoc, 0);
//    glUniform1f(shaderManager->offset_Y_Value, 0.0f);

    //GL_NEAREST:直接去原始纹理限定区域最接近的一个像素信息。
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glUniform1f(shaderManager->offset_X_Value, -0.55f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    //选择两张与映射图片大小最接近的mipmap层，然后从这两个mipmap层中，分别取四个最接近的像素的信息，分别计算加权平均值，然后根据这两个加权平均值，再计算出来一个加权平均值，作为映射点的信息
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glUniform1f(shaderManager->offset_X_Value, 0.55f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void onShutDown() {
    glDeleteProgram(shaderManager->programObject);
}

int esMain() {
    if (!eglManager->esCreateWindow(eglManager->ES_WINDOW_RGB)) {
        LogE<char>(_Texture_2D_mipmap_, "EGL初始化失败");
        return false;
    }
    //顶点着色器
    char vShaderStr[] =
            "#version 300 es                            \n"
            "uniform float u_offset_x;                    \n"
            "uniform float u_offset_y;                    \n"
            "layout(location = 0) in vec4 a_position;   \n"
            "layout(location = 1) in vec2 a_texCoord;   \n"
            "out vec2 v_texCoord;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = a_position;               \n"
            "   gl_Position.x += u_offset_x;              \n"
            "   gl_Position.y += u_offset_y;              \n"
            "   v_texCoord = a_texCoord;                \n"
            "}                                          \n";
    //片段着色器
    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec2 v_texCoord;                                 \n"
            "layout(location = 0) out vec4 outColor;             \n"
            "uniform sampler2D s_texture;                        \n"
            "void main()                                         \n"
            "{                                                   \n"
            "   outColor = texture( s_texture, v_texCoord );     \n"
            "}                                                   \n";
    if (!shaderManager->init(vShaderStr, fShaderStr))return false;
    //获取统一变量位置
    shaderManager->offset_X_Value = glGetUniformLocation(shaderManager->programObject,
                                                         "u_offset_x");
    shaderManager->offset_Y_Value = glGetUniformLocation(shaderManager->programObject,
                                                         "u_offset_y");
    shaderManager->samplerLoc = glGetUniformLocation(shaderManager->programObject, "s_texture");
    shaderManager->textureID = createMipMappedTexture2d();

    shaderManager->drawFunc = onDraw;
    shaderManager->shutDownFunc = onShutDown;
    return true;
}

#endif
