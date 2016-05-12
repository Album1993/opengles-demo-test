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
// Simple_VertexShader.c
//
//    This is a simple example that draws a rotating cube in perspective
//    using a vertex shader to transform the object
//
#include <stdlib.h>
#include "esUtil.h"

#include <stdio.h>

char * ReadFile (char * filename) {
    
    char * buffer;
    
    long read_size,string_size;
    
    FILE * handler ;
    
    handler = fopen(filename, "r");
    
    if (!handler) {
        return NULL;
    }
    
    fseek(handler, 0, SEEK_END);
    
    string_size = ftell(handler);
    
    rewind(handler);
    
    buffer = malloc((string_size + 1) * sizeof(string_size));
    
    read_size = fread(buffer, 1, string_size, handler);
    
    buffer[string_size + 1] = '\0';
    
    if (read_size != string_size) {
        
        buffer = NULL;
        
        free(buffer);
    }
    
    return buffer;
    
}


typedef struct
{
    GLuint      programObject;
    
    GLint       mvpLoc;
    
    GLfloat     *vertices;
    
    GLuint     *indices;
    
    int         numIndices;
    
    GLfloat     angle;
    
    ESMatrix    mvpMatrix;
    
}UserData;

int Init (ESContext * esContext) {
    
    UserData * userData = esContext->userData;
    
    char * vShaderStr = ReadFile("/Users/jiangchenrui/opengl-es-practise/opengles3-book-master/Chapter_8/Simple_VertexShader/iOS/Simple_VertexShader/Simple_VertexShader/Shader.vsh");
    
    char * fShaderStr = ReadFile("/Users/jiangchenrui/opengl-es-practise/opengles3-book-master/Chapter_8/Simple_VertexShader/iOS/Simple_VertexShader/Simple_VertexShader/Shader.fsh");
    
    userData->programObject = esLoadProgram(vShaderStr, fShaderStr);
    
    userData->mvpLoc = glGetUniformLocation(userData->programObject, "u_mvpMatrix");
    
    userData->numIndices = esGenCube(1.0f, &userData->vertices, NULL, NULL, &userData->indices);
    
    userData->angle = 45.0f;
    
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    return GL_TRUE;
    
}

void Update(ESContext * esContext,float deltaTime) {
    
    UserData * userData = esContext->userData;
    
    ESMatrix perspective;
    
    ESMatrix modelview;
    
    float aspect;
    
    userData->angle += (deltaTime * 40.0f);
    
    if (userData->angle >= 360.0f) {
        userData->angle -= 360.0f;
    }
    
    aspect = (GLfloat)esContext->width / (GLfloat) esContext->height;
    
    esMatrixLoadIdentity(&perspective);
    
    esPerspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);
    
    esMatrixLoadIdentity(&modelview);
    
    esTranslate(&modelview, 0.0, 0.0f, -2.0f);
    
    esRotate(&modelview, userData->angle, 1.0, 0.0f, 1.0f);
    
    esMatrixMultiply(&userData->mvpMatrix, &modelview, &perspective);
    
}

void Draw (ESContext * esContext) {
    
    UserData * userData = esContext->userData;
    
    glViewport(0, 0, esContext->width, esContext->height);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(userData->programObject);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), userData->vertices);
    
    glEnableVertexAttribArray(0);
    
    glVertexAttrib4f(1, 1.0f, 0.0f, 0.0f, 1.0f);
    
    glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat *) &userData->mvpMatrix.m[0][0]);
    
    glDrawElements(GL_TRIANGLES, userData->numIndices, GL_UNSIGNED_INT, userData->indices);
    
}


void ShutDown(ESContext * esContext) {
    
    UserData * userData = esContext->userData;
    
    if (userData->vertices != NULL) {
        free(userData->vertices);
    }
    
    if (userData->indices != NULL) {
        free(userData->indices);
    }
    
    glDeleteProgram(userData->programObject);
    
}

int esMain(ESContext * esContext) {
    esContext->userData = malloc(sizeof(UserData));
    
    esCreateWindow(esContext, "Sample_VertesShader", 320, 240, ES_WINDOW_RGB | ES_WINDOW_DEPTH );
    
    if (!Init(esContext)) {
        return GL_FALSE;
    }
    
    esRegisterShutdownFunc(esContext, ShutDown);
    
    esRegisterUpdateFunc(esContext, Update);
    
    esRegisterDrawFunc(esContext, Draw);
    
    return GL_TRUE;
    
}

































