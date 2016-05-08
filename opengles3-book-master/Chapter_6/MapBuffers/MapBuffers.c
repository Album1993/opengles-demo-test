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

char * ReadFile (char * filename) {
    char * buffer = NULL;
    long string_size,read_size;
    FILE * handler;
    handler = fopen(filename, "r");
    if (handler) {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        rewind(handler);
<<<<<<< HEAD
        buffer = (char *)malloc(string_size + 1);
        read_size = fread(buffer, sizeof(char), string_size, handler);
        buffer[string_size + 1] = '\0';
        if (string_size != read_size) {
=======
        buffer = (char *)malloc(sizeof(char) * (string_size + 1));
        read_size = fread(buffer, sizeof(char), string_size, handler);
        buffer[string_size + 1] = '\0';
        if (read_size != string_size) {
>>>>>>> c554208041468d277fdb6d8c40039264b743c4c1
            free(buffer);
            buffer = NULL;
        }
    }
    return buffer;
}

int Init (ESContext * esContext) {
    
    UserData * userData = esContext->userData;
<<<<<<< HEAD
    const char * vShaderStr = ReadFile("/Users/album/Documents/git/opengles-demo-test/opengles3-book-master/Chapter_6/MapBuffers/iOS/MapBuffers/MapBuffers/Shader.vsh");
    const char * fShaderStr = ReadFile("/Users/album/Documents/git/opengles-demo-test/opengles3-book-master/Chapter_6/MapBuffers/iOS/MapBuffers/MapBuffers/Shader.fsh");
    
    GLuint programObject;
    
    programObject = esLoadProgram(vShaderStr, fShaderStr);
    
=======
    const char * vShaderStr = ReadFile("/Users/jiangchenrui/opengl-es-practise/opengles3-book-master/Chapter_6/MapBuffers/iOS/MapBuffers/MapBuffers/Shader.vsh");
    const char * fShaderStr = ReadFile("/Users/jiangchenrui/opengl-es-practise/opengles3-book-master/Chapter_6/MapBuffers/iOS/MapBuffers/MapBuffers/Shader.fsh");
    
    
    GLuint programObject ;
    programObject = esLoadProgram(vShaderStr, fShaderStr);
>>>>>>> c554208041468d277fdb6d8c40039264b743c4c1
    if (programObject == 0) {
        return GL_FALSE;
    }

    userData->programObject = programObject;
    userData->vboIds[0] = 0;
    userData->vboIds[1] = 0;
    
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    return GL_TRUE;
    
}

<<<<<<< HEAD
void DrawPrimitiveWithVBOsMapBuffers (ESContext * esContext,
                                                GLuint numVertices,
                                                GLfloat * vtxBuf,
                                                GLint vtxStride,
                                                GLint numIndices,
                                                GLushort * indices) {
    UserData * userData = esContext->userData;
    GLuint offset = 0;
    
    if (userData->vboIds[0] == 0 && userData->vboIds[1] == 0) {
        GLfloat * vtxMappedBuf;
        GLushort * idxMappedBuf;
        
        glGenBuffers(2, userData->vboIds);
        glBindBuffer(GL_ARRAY_BUFFER, userData->vboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, vtxStride * numVertices, NULL, GL_STATIC_DRAW);
        
        vtxMappedBuf = (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, 0, vtxStride * numVertices, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        if ( vtxMappedBuf == NULL) {
            esLogMessage("error unmapping array buffer object");
            return;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vboIds[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, NULL, GL_STATIC_DRAW);
        
=======
/**
 *  使用map
 *
 *  @param esContext   escontext
 *  @param numVertices 3
 *  @param vtxBuf      vertices 位置和颜色的集合
 *  @param vtxStride   sizeof ( GLfloat ) * ( VERTEX_POS_SIZE + VERTEX_COLOR_SIZE )
 *  @param numIndices  3
 *  @param indices     ｛0，1，2｝
 */
void DrawPrimitiveWithVBOsMapBuffers (ESContext * esContext,
                                      GLuint numVertices,
                                      GLfloat * vtxBuf,
                                      GLfloat vtxStride,
                                      GLint numIndices,
                                      GLushort * indices){
    UserData * userdata = esContext->userData;
    GLuint offset = 0;
    if (userdata->vboIds[0] == 0 && userdata->vboIds[1] == 0) {
        GLfloat * vtxMappedBuf;
        GLushort * idxMappedBuf;
        glGenBuffers(2, userdata->vboIds);
        glBindBuffer(GL_ARRAY_BUFFER, userdata->vboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, vtxStride * numIndices, NULL, GL_STATIC_DRAW);
        
        vtxMappedBuf = (GLfloat *)glMapBufferRange(GL_ARRAY_BUFFER, 0, vtxStride * numVertices, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        if (vtxMappedBuf == NULL) {
            esLogMessage("error mapping vertex buffer object");
            return;
        }
        
        memcpy(vtxMappedBuf, vtxBuf, vtxStride * numVertices);
        
        if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE) {
            esLogMessage("error unmapping array buffer object");
            return;
        }
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userdata->vboIds[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, NULL, GL_STATIC_DRAW);
        idxMappedBuf = (GLushort *) glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * numIndices, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        if (idxMappedBuf == NULL) {
            esLogMessage("error mapping element array buffer object.");
            return;
        }
        
        memcpy(idxMappedBuf, indices, sizeof(GLushort) * numIndices);
        
        if(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER ) == GL_FALSE) {
            esLogMessage("error mapping element array buffer object.");
        }
>>>>>>> c554208041468d277fdb6d8c40039264b743c4c1
    }
    
    
}

void Draw (ESContext * esContext) {
    UserData * userData = esContext->userData;
    GLfloat vertices[3 * ( VERTEX_POS_SIZE + VERTEX_COLOR_SIZE )] =
    {
        0.0f,  0.5f, 0.0f,        // v0
        1.0f,  0.0f, 0.0f, 1.0f,  // c0
        -0.5f, -0.5f, 0.0f,        // v1
        0.0f,  1.0f, 0.0f, 1.0f,  // c1
        0.5f, -0.5f, 0.0f,        // v2
        0.0f,  0.0f, 1.0f, 1.0f,  // c2
    };
    
    GLushort indice[3] = {0,1,2};
    glViewport(0, 0, esContext->width, esContext->height);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(userData->programObject);
    DrawPrimitiveWithVBOsMapBuffers(esContext, 3,vertices
                                    , sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE), 3, indice);
    
}

void ShutDown (ESContext * esContext) {
    UserData * userdata = esContext->userData;
    glDeleteProgram(userdata->programObject);
    glDeleteBuffers(2, userdata->vboIds);
}

int esMain (ESContext * esContext) {
    esContext->userData = malloc(sizeof(UserData));
    esCreateWindow(esContext, "vertexBufferObjects", 320, 240, ES_WINDOW_RGB);
    if (!Init(esContext)) {
        return GL_FALSE;
    }
    esRegisterShutdownFunc(esContext, ShutDown);
    esRegisterDrawFunc(esContext, Draw);
    return GL_TRUE;
}















































