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
// Instancing.c
//
//    Demonstrates drawing multiple objects in a single draw call with
//    geometry instancing
//
#include <stdlib.h>
#include <math.h>
#include "esUtil.h"

#ifdef _WIN32
#define srandom srand
#define random rand
#endif


#define NUM_INSTANCES   100
#define POSITION_LOC    0
#define COLOR_LOC       1
#define MVP_LOC         2

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // VBOs
   GLuint positionVBO;
   GLuint colorVBO;
   GLuint mvpVBO;
   GLuint indicesIBO;

   // Number of indices
   int       numIndices;

   // Rotation angle
   GLfloat   angle[NUM_INSTANCES];

} UserData;

///
int Init(ESContext * esContext) {
    GLfloat * positions;
    GLuint * indices;
    
    UserData * userData = esContext->userData;
    const char vShaderStr[] =
    "#version 300 es                                \n"
    "layout(location = 0) in vec4 a_position;       \n"
    "layout(location = 1) in vec4 a_color;          \n"
    "layout(location = 2) in mat4 a_mvpMatrix;      \n"
    "out vec4 v_color;                              \n"
    "void main()                                    \n"
    "{                                              \n"
    "   v_color = a_color;                          \n"
    "   gl_position = a_mvpMatrix * a_position;     \n"
    "}                                              \n";
    const char fShaderStr[] =
    "#version 300 es                                \n"
    "precision mediump float;                       \n"
    "in vec4 v_color;                               \n"
    "layout(location = 0) out vec4 outColor;        \n"
    "void main()                                    \n"
    "{                                              \n"
    "   outColor = v_color;                         \n"
    "}                                              \n";
    
    userData->programObject = esLoadProgram(vShaderStr, fShaderStr);
    userData->numIndices = esGenCube(0.1f, &positions, NULL, NULL, &indices);
    
    glGenBuffers(1, &userData->indicesIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->indicesIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * userData->numIndices, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    free(indices);
    
    glGenBuffers(1, &userData->positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->positionVBO);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat) * 3, positions, GL_STATIC_DRAW);
    free(positions);
    
    {
        GLubyte colors[NUM_INSTANCES][4];
        int instance;
        
        srandom(0);
        for (instance = 0; instance < NUM_INSTANCES; instance) {
            colors[instance][0] = random() % 255;
            colors[instance][1] = random() % 255;
            colors[instance][2] = random() % 255;
            colors[instance][3] = 0;
        }
        
        glGenBuffers(1, &userData->colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, userData->colorVBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 4, colors, GL_STATIC_DRAW);
    }
    
    {
        int instance;
        
        for (instance = 0; instance < NUM_INSTANCES; instance ++) {
            userData->angle[instance] = (float)(random() * 32768) / 32767.0f * 360.0f;
        }
        
        glGenBuffers(1, &userData->mvpVBO);
        glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * sizeof(ESMatrix), NULL, GL_DYNAMIC_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return GL_TRUE;
    
}

void Update(ESContext * esContext) {
    UserData * userData = (UserData *)esContext->userData;
    ESMatrix * matrixBuf;
    ESMatrix  perspective;
    float aspect;
    int instance = 0;
    int numRows;
    int numColums;
    aspect = (GLfloat)esContext->width / (GLfloat) esContext->height;
    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);
    glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
    matrixBuf = (ESMatrix *)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(ESMatrix)* NUM_INSTANCES, GL_MAP_WRITE_BIT);
    numRows = (int)sqrt(NUM_INSTANCES);
    numColums = numRows;
    for (instance = 0; instance < NUM_INSTANCES; instance ++) {
        ESMatrix modelview;
        float translateX = ((float) (instance % numRows) / (float)numRows) * 2.0f - 1.0f;
        float translateY = ((float) (instance / numColums) / (float)numColums) * 2.0f - 1.0f;
        esma
    }
}
///
// Cleanup
//
void Shutdown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   glDeleteBuffers ( 1, &userData->positionVBO );
   glDeleteBuffers ( 1, &userData->colorVBO );
   glDeleteBuffers ( 1, &userData->mvpVBO );
   glDeleteBuffers ( 1, &userData->indicesIBO );

   // Delete program object
   glDeleteProgram ( userData->programObject );
}


int esMain ( ESContext *esContext )
{
   esContext->userData = malloc ( sizeof ( UserData ) );

   esCreateWindow ( esContext, "Instancing", 640, 480, ES_WINDOW_RGB | ES_WINDOW_DEPTH );

   if ( !Init ( esContext ) )
   {
      return GL_FALSE;
   }

   esRegisterShutdownFunc ( esContext, Shutdown );
   esRegisterUpdateFunc ( esContext, Update );
   esRegisterDrawFunc ( esContext, Draw );

   return GL_TRUE;
}

