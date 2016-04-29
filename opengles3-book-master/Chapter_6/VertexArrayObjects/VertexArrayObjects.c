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
// VertexArrayObjects.c
//
//    This example demonstrates drawing a primitive with
//    Vertex Array Objects (VAOs)
//
#include "esUtil.h"
#include "stdlib.h"
#include <stdio.h>
//
//char* ReadFile(char *filename)
//{
//    char *buffer = NULL;
//    int string_size,read_size;
//    FILE *handler = fopen(filename,"r");
//    
//    if (handler)
//    {
//        //seek the last byte of the file
//        fseek(handler,0,SEEK_END);
//        //offset from the first to the last byte, or in other words, filesize
//        string_size = ftell (handler);
//        //go back to the start of the file
//        rewind(handler);
//        
//        //allocate a string that can hold it all
//        buffer = (char*) malloc (sizeof(char) * (string_size + 1) );
//        //read it all in one operation
//        read_size = fread(buffer,sizeof(char),string_size,handler);
//        //fread doesnt set it so put a \0 in the last position
//        //and buffer is now officialy a string
//        buffer[string_size+1] = '\0';
//        
//        if (string_size != read_size) {
//            //something went wrong, throw away the memory and set
//            //the buffer to NULL
//            free(buffer);
//            buffer = NULL;
//        }
//    }
//    
//    return buffer;
//}

char * ReadFile(char * filename) {
    char * buffer = NULL;
    int string_size ,read_size;
    FILE * handler = fopen(filename, "r");
    if (handler) {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        rewind(handler);
        buffer = (char *)malloc(sizeof(char) * (string_size + 1));
        read_size = fread(buffer, sizeof(char), string_size, handler);
        buffer[string_size + 1] = '\0';
        if (string_size != read_size) {
            free(buffer);
            buffer = NULL;
        }
    }
    return buffer;
}

typedef struct
{
    GLuint programObject;
    
    GLuint vboIds[2];
    
    GLuint vaoId;
} UserData;

#define VERTEX_POS_SIZE     3
#define VERTEX_COLOR_SIZE   4

#define VERTEX_POS_INDX     0
#define VERTEX_COLOR_INDX   1

#define VERTEX_STRIDE       (sizeof(GLfloat) *  \
                                (VERTEX_POS_SIZE +  \
                                VERTEX_COLOR_SIZE))

int Init(ESContext * esContext)
{
    UserData * userData = esContext -> userData;
    const char * vShaderStr = ReadFile("./Users/jiangchenrui/Desktop/opengles3-book-master/Chapter_6/VertexArrayObjects/iOS/VertexArrayObjects/VertexArrayObjects/Shader.vsh");
    const char * fShaderStr = ReadFile("./Users/jiangchenrui/Desktop/opengles3-book-master/Chapter_6/VertexArrayObjects/iOS/VertexArrayObjects/VertexArrayObjects/Shader.fsh");
    
    GLuint programObject;
    
    GLfloat vertices[3 * (VERTEX_POS_SIZE  + VERTEX_COLOR_SIZE)] =
    {
        0.0f,  0.5f, 0.0f,        // v0
        1.0f,  0.0f, 0.0f, 1.0f,  // c0
        -0.5f, -0.5f, 0.0f,        // v1
        0.0f,  1.0f, 0.0f, 1.0f,  // c1
        0.5f, -0.5f, 0.0f,        // v2
        0.0f,  0.0f, 1.0f, 1.0f,  // c2
    };
    
    GLushort indices[3] = {0,1,2};
    
    programObject = esLoadProgram(vShaderStr, fShaderStr);
    
    if (programObject == 0) {
        return GL_FALSE;
    }
    
    userData->programObject = programObject;
    
    glGenBuffers(2, userData->vboIds);
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[1]);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &userData->vaoId);
    
    glBindVertexArray(userData->vaoId);
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[1]);
    
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);
    
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE,  VERTEX_STRIDE, (const void *)0);
    glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, (const void *)(VERTEX_POS_SIZE * sizeof(GLfloat)));
    glBindVertexArray(0);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return GL_TRUE;
}

void Draw (ESContext * esContext)
{
    UserData * userData = esContext->userData;
    glViewport(0, 0, esContext->width, esContext->height);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(userData->programObject);
    glBindVertexArray(userData->vaoId);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (const void *)0);
    glBindVertexArray(0);
}

void ShutDown (ESContext * esContext) {
    UserData * userData = esContext->userData;
    glDeleteProgram(userData->programObject);
    glDeleteBuffers(2, userData->vboIds);
    glDeleteVertexArrays(1, &userData->vaoId);
}

int esMain(ESContext * esContext)
{
    esContext->userData = malloc(sizeof(UserData));
    esCreateWindow(esContext, "vertexArrayobject", 320, 240, ES_WINDOW_RGB);
    if (!Init(esContext)) {
        return GL_FALSE;
    }
    esRegisterShutdownFunc(esContext, ShutDown);
    esRegisterDrawFunc(esContext, Draw);
    
    return GL_TRUE;
    
}






























