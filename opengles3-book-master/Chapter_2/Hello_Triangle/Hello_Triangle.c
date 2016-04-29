// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this
//    example is to demonstrate the basic concepts of
//    OpenGL ES 3.0 rendering.
#include "esUtil.h"

typedef struct {
    GLuint programObject;
} UserData;

GLuint LoadShader(GLenum type,const char * shaderStr) {
    
    GLuint shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderStr, NULL);
    glCompileShader(shader);
    
    GLint compiled;
    
    glGetShaderiv(shader, GL_SHADER_COMPILER, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char * infoLog = malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            esLogMessage("error shader compiled failed %s",infoLog);
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

int Init(ESContext * esContext) {
    UserData * userData = esContext->userData;
    
    GLuint vetrexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint  linked;
    
    char vShader[] =
    "#version 300 es                                \n"
    "layout(location = 0) in vec4 vPosition;        \n"
    "void main()                                    \n"
    "{                                              \n"
    "    gl_Position = vPosition;                   \n"
    "}                                              \n";
    
    char fShader[] =
    "#version 300 es                                \n"
    "precision mediump float;                       \n"
    "out vec4 fragColor;                            \n"
    "void main()                                    \n"
    "{                                              \n"
    "    fragColor = vec4(1.0f,0.0f,0.0f,1.0f);     \n"
    "}";
    
    vetrexShader = LoadShader(GL_VERTEX_SHADER, vShader);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShader);
    
    programObject = glCreateProgram();
    if (programObject == 0) {
        return GL_FALSE;
    }
    
    glAttachShader(programObject, vetrexShader);
    glAttachShader(programObject, fragmentShader);
    
    glLinkProgram(programObject);
    
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject,GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char * infoLog = malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return GL_FALSE;
    }
    
    userData->programObject = programObject;
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return GL_TRUE;
}

void Draw (ESContext * esContext) {
    UserData * userData = esContext->userData;
    GLfloat vVertex[] = {0.0f,0.5f,0.0f,
                        -0.5f,-0.5f,0.0f,
                        0.5f,-0.5f,0.0f};
    
    glViewport(0, 0, esContext->width, esContext->height);
    
    glUseProgram(userData->programObject);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertex);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
}

void ShutDown (ESContext * esContext) {
    UserData * userData = esContext->userData;
    glDeleteProgram(userData->programObject);
}

int esMain(ESContext * context) {
    context->userData = malloc(sizeof(UserData));
    
    esCreateWindow(context, "hhha", 320, 240, 0);
    
    if (!Init(context)) {
        return GL_FALSE;
    }
    
    esRegisterDrawFunc(context, Draw);
    esRegisterShutdownFunc(context, ShutDown);
    
    return GL_TRUE;
    
}




















