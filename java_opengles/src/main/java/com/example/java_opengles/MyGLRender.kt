//package com.example.java_opengles
//
//import android.opengl.GLSurfaceView
//import android.util.Log
//import javax.microedition.khronos.egl.EGLConfig
//import javax.microedition.khronos.opengles.GL10
//
//class MyGLRender(private val nativeRender: MyNativeRender) : GLSurfaceView.Renderer {
//
//    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
//        Log.d(TAG, "onSurfaceCreated:")
//        nativeRender.native_onSurfaceCreated()
//    }
//
//    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
//        Log.d(TAG, "onSurfaceChanged; width:$width; height:$height")
//        nativeRender.native_onSurfaceChanged(width, height)
//    }
//
//    override fun onDrawFrame(gl: GL10?) {
//        Log.d(TAG, "onDrawFrame:")
//        nativeRender.native_onDrawFrame()
//    }
//
//    companion object {
//        private val TAG = MyGLRender::class.java.simpleName
//    }
//}