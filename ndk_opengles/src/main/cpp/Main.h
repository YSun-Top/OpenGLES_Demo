//
// Created by Voidcom on 2023/6/14.
//

#ifndef LEARNINGPROJECT_MAIN_H
#define LEARNINGPROJECT_MAIN_H

#include "include/default.h"
#include <android/sensor.h>
#include <string>
#include <time.h>

#include "ShaderManager.h"
#include "EGLManager.h"
//#include <cassert>

class MainAPP{
public:
    struct android_app *android_app_data;
    //region 传感器
    //传感器Manager
    ASensorManager *sensorManager;
    //加速计传感器
    const ASensor *accelerometerSensor;
    //传感器时间队列
    ASensorEventQueue *sensorEventQueue;
    int animating;
    friend void android_main(struct android_app *app);
    friend void onAPPCallback(struct android_app *app, int32_t cmd);
};
#endif //LEARNINGPROJECT_MAIN_H
