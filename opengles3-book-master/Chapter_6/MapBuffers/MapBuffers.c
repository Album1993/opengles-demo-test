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
// MapBuffers.c
//
//    This example demonstrates mapping buffer objects
//
#include "esUtil.h"
#include <string.h>
#include <stdio.h>
typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // VertexBufferObject Ids
   GLuint vboIds[2];

} UserData;

#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a

#define VERTEX_POS_INDX       0
#define VERTEX_COLOR_INDX     1

//char * ReadFile(char * filename) {
//    char * buffer = NULL;
//    int string_size ,read_size;
//    FILE * handler = fopen(filename, "r");
//    if (handler) {
//        fseek(handler, 0, SEEK_END);
//        string_size = ftell(handler);
//        rewind(handler);
//        buffer = (char *)malloc(sizeof(char) * (string_size + 1));
//        read_size = fread(buffer, sizeof(char), string_size, handler);
//        buffer[string_size + 1] = '\0';
//        if (string_size != read_size) {
//            free(buffer);
//            buffer = NULL;
//        }
//    }
//    return buffer;
//}


char * ReadFile (char * filename) {
    char * buffer = NULL;
    long string_size,read_size;
    FILE * handler = fopen(filename, "r");
    if (handler) {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        rewind(handler);
        read_size = fread(buffer, sizeof(char), string_size, handler);
        if (read_size != string_size) {
            free(buffer);
            buffer = NULL;
        }
    }
    return buffer;
}

int Init (ESContext * esContext)
{
    UserData * userData = esContext->userData;
    const char * vShaderStr = ReadFile("/Users/jiangchenrui/Desktop/opengles3-book-master/Chapter_6/MapBuffers/iOS/MapBuffers/MapBuffers/Shader.vsh");
    const char * fShaderStr = ReadFile("/Users/jiangchenrui/Desktop/opengles3-book-master/Chapter_6/MapBuffers/iOS/MapBuffers/MapBuffers/Shader.fsh");
    
    GLuint programObject = esLoadProgram(vShaderStr, fShaderStr);
    if (programObject == 0) {
        return GL_FALSE;
    }
    
    userData->programObject = programObject;
    userData->vboIds[0] = 0;
    userData->vboIds[1] = 0;
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return GL_TRUE;
}


void DrawPrimitiveWithVBOsMapBuffers(ESContext *esContext,
                                     GLint numVertices,
                                     GLfloat * vtxBuf,
                                     GLint vtxStride,
                                     GLint numIndices,
                                     GLushort * indices) {
    UserData * userData = esContext->userData;
    GLuint offset = 0;
    if (userData->vboIds[0] == 0 && userData->vboIds[1] == 0) {
        GLfloat * vtxMappedBuf;
        GLushort * idxMappedBuf;
        gl
    }
}





















































