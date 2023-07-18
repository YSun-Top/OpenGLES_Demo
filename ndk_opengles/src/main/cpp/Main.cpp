//
// Created by Voidcom on 2023/6/14.
//

#include "Main.h"

extern int esMain();

EGLManager *eglManager;
ShaderManager *shaderManager;
MainAPP *mainApp;

float getCurrentTime() {
    struct timespec clockRealTime{};
    clock_gettime(CLOCK_MONOTONIC, &clockRealTime);
    return (float) clockRealTime.tv_sec + (float) clockRealTime.tv_nsec / 1e9f;
}

/**
 *  android_app中onAppCmd的状态回调
 */
void onAPPCallback(struct android_app *app, int32_t cmd) {
//    auto *engine = (Engine *) app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            //在这里保存应用状态
//            engine->android_app_data->savedState = new SaveState;
//            *((struct SaveState *) engine->android_app_data->savedState) = engine->state;
//            engine->android_app_data->savedStateSize = sizeof(struct SaveState);
            break;
        case APP_CMD_INIT_WINDOW:
//            if (engine->android_app_data->window != nullptr) {
//                engineInitDisplay(engine);
//            }
            //ANativeWindow创建好后触发这个状态，窗口初始化完成。
            eglManager->eglDisplayType = EGL_DEFAULT_DISPLAY;
            eglManager->eglWindowType = app->window;
            //调用OpenGLES的主入口，如果启动失败就退出程序
            if (esMain() != GL_TRUE) {
                exit(0);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            if (shaderManager->shutDownFunc != nullptr) {
                shaderManager->shutDownFunc();
            }
            delete &eglManager;
            delete &shaderManager;
            delete &mainApp;
            //ANativeWindow即将被终止，在调用android_app_exec_cmd之后将被设为 null
//            delete &engine->android_app_data->savedState;
//            delete engine;
            break;
        case APP_CMD_LOST_FOCUS:
            //当App失去焦点，比如黑屏、切换到其他应用。应该停止监控传感器
//            if (engine->accelerometerSensor != nullptr) {
//                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
//                                                engine->accelerometerSensor);
//            }
//            //停止动画
//            engine->animating = 0;
//            engineDrawFrame(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
//            //重新获得焦点，开始监控传感器
//            if (engine->accelerometerSensor != nullptr) {
//                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
//                                               engine->accelerometerSensor);
//                //设置每秒60个事件
//                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
//                                               engine->accelerometerSensor,
//                                               (1000L / 60) * 1000);
//            }
            break;
        default:
            break;
    }
}

/**
 * NativeApp必须实现这个函数，它是应用程序的主要入口
 */
void android_main(struct android_app *app) {
    eglManager = new EGLManager();
    shaderManager = new ShaderManager();
    mainApp = new MainAPP();
    //初始化上下文
    mainApp->android_app_data=app;
    shaderManager->platformData = (void *) app->activity->assetManager;
    app->onAppCmd = onAPPCallback;
    float lastTime = getCurrentTime();

    while (true) {
        int events;
        struct android_poll_source *pollSource;
        while ((ALooper_pollAll(0, nullptr, &events, (void **) &pollSource)) >= 0) {
            if (pollSource != nullptr) {
                pollSource->process(app, pollSource);
            }
            //当app即将退出时，该值不为零
            if (app->destroyRequested != 0)return;
        }

        if (eglManager->eglWindowType == nullptr)continue;
        if (shaderManager->updateFunc != nullptr) {
            float curTime = getCurrentTime();
            shaderManager->updateFunc(curTime - lastTime);
            lastTime = curTime;
        }
        if (shaderManager->drawFunc != nullptr) {
            shaderManager->drawFunc();
            //将后台缓冲区已经渲染完成的帧交换到前台缓冲区
            eglSwapBuffers(eglManager->eglDisplay, eglManager->eglSurface);
        }
    }
}

