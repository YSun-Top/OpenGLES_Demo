package com.example.java_opengles

import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.util.Log
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class HelloTriangleRenderer : GLSurfaceView.Renderer {

    // Member variables
    private var mProgramObject = 0
    private var mWidth = 0
    private var mHeight = 0
    private var mVertices: FloatBuffer

    private val mVerticesData =
        floatArrayOf(0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f)

    init {
        mVertices = ByteBuffer.allocateDirect(mVerticesData.size * 4)
            .order(ByteOrder.nativeOrder()).asFloatBuffer()
        mVertices.put(mVerticesData).position(0)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        Log.d(TAG, "onSurfaceCreated:")
        val vShaderStr = "#version 300 es   \n" +
                "in vec4 vPosition; \n" +
                "void main()    \n" +
                "{  \n" +
                "gl_Position = vPosition;   \n" +
                "} \n"

        val fShaderStr = "#version 300 es	\n" +
                "precision mediump float;   \n" +
                "out vec4 fragColor;    \n" +
                "void main()    \n" +
                "{  \n" +
                "  fragColor = vec4 ( 1.0, 0.5, 0.2, 1.0 );	\n" +
                "}                                            \n"

        val linked = IntArray(1)

        // Load the vertex/fragment shaders

        // Load the vertex/fragment shaders
        val vertexShader: Int = loadShader(GLES30.GL_VERTEX_SHADER, vShaderStr)
        val fragmentShader: Int = loadShader(GLES30.GL_FRAGMENT_SHADER, fShaderStr)

        // Create the program object

        // Create the program object
        val programObject: Int = GLES30.glCreateProgram()

        if (programObject == 0) {
            return
        }

        GLES30.glAttachShader(programObject, vertexShader)
        GLES30.glAttachShader(programObject, fragmentShader)

        // Bind vPosition to attribute 0

        // Bind vPosition to attribute 0
        GLES30.glBindAttribLocation(programObject, 0, "vPosition")

        // Link the program

        // Link the program
        GLES30.glLinkProgram(programObject)

        // Check the link status

        // Check the link status
        GLES30.glGetProgramiv(programObject, GLES30.GL_LINK_STATUS, linked, 0)

        if (linked[0] == 0) {
            Log.e(TAG, "Error linking program:")
            Log.e(TAG, GLES30.glGetProgramInfoLog(programObject))
            GLES30.glDeleteProgram(programObject)
            return
        }

        // Store the program object

        // Store the program object
        mProgramObject = programObject

        GLES30.glClearColor(1.0f, 1.0f, 1.0f, 0.0f)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        Log.d(TAG, "onSurfaceChanged; width:$width; height:$height")
        mWidth = width
        mHeight = height
    }

    override fun onDrawFrame(gl: GL10?) {
        Log.d(TAG, "onDrawFrame:")
        // Set the viewport

        // Set the viewport
        GLES30.glViewport(0, 0, mWidth, mHeight)

        // Clear the color buffer

        // Clear the color buffer
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        // Use the program object

        // Use the program object
        GLES30.glUseProgram(mProgramObject)

        // Load the vertex data

        // Load the vertex data
        GLES30.glVertexAttribPointer(0, 3, GLES30.GL_FLOAT, false, 0, mVertices)
        GLES30.glEnableVertexAttribArray(0)

        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, 3)
    }

    private fun loadShader(type: Int, shaderSrc: String): Int {
        val compiled = IntArray(1)

        // Create the shader object
        val shader: Int = GLES30.glCreateShader(type)
        if (shader == 0) {
            return 0
        }

        // Load the shader source
        GLES30.glShaderSource(shader, shaderSrc)

        // Compile the shader
        GLES30.glCompileShader(shader)

        // Check the compile status
        GLES30.glGetShaderiv(shader, GLES30.GL_COMPILE_STATUS, compiled, 0)
        if (compiled[0] == 0) {
            Log.e(TAG, GLES30.glGetShaderInfoLog(shader))
            GLES30.glDeleteShader(shader)
            return 0
        }
        return shader
    }

    companion object {
        private val TAG = HelloTriangleRenderer::class.java.simpleName
    }
}