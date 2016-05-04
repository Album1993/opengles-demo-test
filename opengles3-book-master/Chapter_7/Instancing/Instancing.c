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
#include <stdio.h>


#ifdef _WIN32
#define srandom srand
#define random rand
#endif

#define NUM_INSTANCE    100
#define POSITION_LOC    0
#define COLOR_LOC       1
#define MVP_LOC         2

typedef struct
{
    GLuint programObject;
    
    GLuint positionVBO;
    GLuint colorVBO;
    GLuint mvpVBO;
    GLuint indicesIBO;
    
    int numIndices;
    
    GLfloat angle[NUM_INSTANCE];
    
} UserData;

char * ReadFile (char * filename)
{
    char * buffer;
    long stringSize,readSize;
    FILE * handler;
    handler = fopen(filename, "r");
    
    {
        fseek(handler, 0, SEEK_END);
        stringSize = ftell(handler);
        
        rewind(handler);
        
        buffer = (char *)malloc(sizeof(char) * (stringSize + 1));
        readSize = fread(buffer, sizeof(char), stringSize, handler);
        if (stringSize !=  readSize) {
            free(buffer);
            buffer = NULL;
        }
    }
  
    return buffer;
}


int Init (ESContext * esContext)
{
    GLfloat * positions;
    GLuint * indices;
    
    UserData * userData = esContext->userData;
    
    const char * vShaderStr = ReadFile("/Users/jiangchenrui/opengl-es-practise/opengles3-book-master/Chapter_7/Instancing/iOS/Instancing/Instancing/Shade.vsh");
    
    const char * fShaderStr = ReadFile("/Users/jiangchenrui/opengl-es-practise/opengles3-book-master/Chapter_7/Instancing/iOS/Instancing/Instancing/Shade.fsh");
    
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
        GLubyte colors[NUM_INSTANCE][4];
        int instance;
        
        srandom(0);
        
        for (instance = 0; instance < NUM_INSTANCE; instance ++) {
            colors[instance][0] = random() % 255;
            colors[instance][1] = random() % 255;
            colors[instance][2] = random() % 255;
            colors[instance][3] = 0;
        }
        
        glGenBuffers(1, &userData->colorVBO);
        glBindBuffer(GL_ARRAY_BUFFER, userData->colorVBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCE * 4, colors, GL_STATIC_DRAW);
        
    }
    {
        int instance;
        for (instance = 0; instance < NUM_INSTANCE; instance ++) {
            userData->angle[instance] = (float) (random() % 32768) / 32767.0f * 360.f;
        }
        glGenBuffers(1, &userData->mvpVBO);
        
        glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCE * sizeof(ESMatrix), NULL, GL_DYNAMIC_DRAW);
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glClearColor(1.0f,1.0f , 1.0f, 0.0f);
    return GL_TRUE;
}


void Updata (ESContext * esContext,float deltaTime) {
    UserData * userData = (UserData *)esContext->userData;
    ESMatrix *matrixBuf;
    ESMatrix perspective;
    
    float aspect;
    int instance = 0;
    int numRows;
    int numColumns;
    
    aspect = (GLfloat)esContext->width/(GLfloat)esContext->height;
    esMatrixLoadIdentity(&perspective);
    esPerspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
    
    matrixBuf = (ESMatrix *)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(ESMatrix) * NUM_INSTANCE, GL_MAP_WRITE_BIT);
    
    numRows = (int) sqrt(NUM_INSTANCE);
    
    numColumns = numRows;
    
    for (instance = 0; instance < NUM_INSTANCE; instance ++) {
        ESMatrix modelview;
        float translateX = ((float) (instance % numRows) / (float)numRows) * 2.0f - 1.0f;
        float translateY = ((float) (instance / numColumns)/(float)numColumns) * 2.0f - 1.0f;
        
        esMatrixLoadIdentity(&modelview);
        
        esTranslate(&modelview, translateX, translateY, -2.0f);
        
        userData->angle[instance] += (deltaTime * 40.0f);
        
        if (userData->angle[instance] > 360.0f) {
            userData->angle[instance] -= 360.f;
        }
        
        esRotate(&modelview, userData->angle[instance], 1.0, 0.0f, 1.0);
        
        esMatrixMultiply(&matrixBuf[instance], &modelview, &perspective);
        
    }
    
    glUnmapBuffer(GL_ARRAY_BUFFER);
    
    
}


























