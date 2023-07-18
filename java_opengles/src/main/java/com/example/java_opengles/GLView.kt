//package com.example.java_opengles
//
//import android.content.Context
//import android.opengl.GLSurfaceView
//import android.util.AttributeSet
//
///**
// * OpenGL ES
// */
//class GLView : GLSurfaceView {
//    private lateinit var myGLRender: MyGLRender
//    lateinit var myNativeRender: MyNativeRender
//        private set
//
//    constructor(context: Context) : super(context)
//
//    constructor(context: Context, attr: AttributeSet?) : super(context, attr) {
//        this.setEGLContextClientVersion(3)
//        myNativeRender = MyNativeRender()
//        myGLRender = MyGLRender(myNativeRender)
//        setRenderer(myGLRender)
//        renderMode = RENDERMODE_CONTINUOUSLY
//    }
//
//}