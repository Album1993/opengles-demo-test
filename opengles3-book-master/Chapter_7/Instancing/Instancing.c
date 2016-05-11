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

char * ReadFile (char * fileName) {
    
    char * buffer;
    
    long string_size,read_size;
    
    FILE * handler;
    
    handler = fopen(fileName, "r");
    
    if (handler == 0) {
        return NULL;
    }
    
    fseek(handler, 0, SEEK_END);
    
    string_size = ftell(handler);
    
    rewind(handler);
    
    buffer = malloc(sizeof(char) * (string_size + 1));
    
    read_size = fread(buffer, 1, string_size, handler);
    
    buffer[string_size + 1] = '\0';
    
    if (string_size != read_size) {
        
        buffer = NULL;
        
        free(buffer);
        
    }
    
    return buffer;
    
}

int Init (ESContext * esContext) {
    
    GLfloat * positions;
    GLuint * indices;
    
    UserData *userDate = esContext->userData;
    const char * vShaderStr = ReadFile("/Users/jiangchenrui/opengl-es-practise/opengles3-book-master/Chapter_7/Instancing/iOS/Instancing/Instancing/Shader.vsh");
    const char * fShaderStr = ReadFile("/Users/jiangchenrui/opengl-es-practise/opengles3-book-master/Chapter_7/Instancing/iOS/Instancing/Instancing/Shader.fsh");
    
    userDate->programObject = esLoadProgram(vShaderStr, fShaderStr);
    
    userDate->numIndices = esGenCube(0.1, &positions, NULL, NULL, &indices);
    
    glGenBuffers(1, &userDate->indicesIBO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userDate->indicesIBO);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * userDate->numIndices, indices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    free(indices);
    
    glGenBuffers(1, &userDate->positionVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, userDate->positionVBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUM_INSTANCE * 4, positions, GL_STATIC_DRAW);
    
    free(positions);
    
    {
        
        int instance;
        
        GLubyte color[NUM_INSTANCE][4] ;
        
        srand(0);
        
        for (instance = 0; instance < NUM_INSTANCE; instance ++) {
            
            color[instance][0] = random() % 255;
            color[instance][1] = random() % 255;
            color[instance][2] = random() % 255;
            color[instance][3] = 0;
            
        }
        
        glGenBuffers(1, &userDate->colorVBO);
        
        glBindBuffer(GL_ARRAY_BUFFER, userDate->colorVBO);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLubyte) * 4 * NUM_INSTANCE, color, GL_STATIC_DRAW);
        
    }
    
    {
        int instance ;
        
        for (instance = 0; instance < NUM_INSTANCE; instance++) {
            userDate->angle[instance] = (GLfloat)(random() % 32768) / 32767.0f * 360.0f;
        }
        
        glGenBuffers(1, &userDate->mvpVBO);
        
        glBindBuffer(GL_ARRAY_BUFFER, userDate->mvpVBO);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(ESMatrix) * NUM_INSTANCE, userDate->angle, GL_DYNAMIC_DRAW);
        
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    return GL_TRUE;
    
}

void Update (ESContext * esContext,float deltaTime) {
    
    UserData * userData = (UserData *)esContext->userData;
    
    ESMatrix * matrixBuf = NULL ;
    
    ESMatrix perspective;
    
    float aspect;
    
    int numRows;
    
    int numColumns;
    
    int instances = 0;
    
    aspect = (float) esContext->width / (float) esContext->height;
    
    esMatrixLoadIdentity(&perspective);
    
    esPerspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
    
    matrixBuf = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(ESMatrix) * NUM_INSTANCE, GL_MAP_WRITE_BIT);
    
    numRows = (int)sqrtf(NUM_INSTANCE);
    
    numColumns = NUM_INSTANCE / numRows;
    
    for (instances = 0; instances < NUM_INSTANCE; instances++) {
        
        float translateX = ( ( float ) ( instances % numRows ) / ( float ) numRows ) * 2.0f - 1.0f;
        float translateY = ( ( float ) ( instances / numColumns ) / ( float ) numColumns ) * 2.0f - 1.0f;
        
        ESMatrix modelview ;
        
        esMatrixLoadIdentity(&modelview);
        
        esTranslate(&modelview, translateX, translateY, -2.0f);
        
        userData->angle[instances] += (deltaTime * 40.0f);
        
        if (userData->angle[instances] > 360.0f) {
            userData->angle[instances] -= 360.0f;
        }
        
        esRotate(&modelview, userData->angle[instances], 1.0f, 0.0f, 1.0f);
        
        esMatrixMultiply(&matrixBuf[instances], &modelview, &perspective);
    }
    
    glUnmapBuffer(GL_ARRAY_BUFFER);
    
}

void Draw(ESContext * esContext) {
    UserData * userData = esContext->userData;
    
    glViewport(0, 0, esContext->width, esContext->height);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BITS);
    
    glUseProgram(userData->programObject);
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->positionVBO);
    
    glVertexAttribPointer(POSITION_LOC,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3,
                          (const void *)NULL);
    
    glEnableVertexAttribArray(POSITION_LOC);
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->colorVBO);
    
    glVertexAttribPointer(COLOR_LOC, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4 * sizeof(GLubyte), (const void *) NULL);
    
    glEnableVertexAttribArray(COLOR_LOC);
    
    glBindBuffer(GL_ARRAY_BUFFER, userData->mvpVBO);
    
}

void Shutdown (ESContext * esContext) {
    
    UserData * userdata = esContext->userData;
    
    glDeleteBuffers(1, &userdata->positionVBO);
    
    glDeleteBuffers(1, &userdata->colorVBO);
    
    glDeleteBuffers(1, &userdata->mvpVBO);
    
    glDeleteBuffers(1, &userdata->indicesIBO);
    
    glDeleteProgram(userdata->programObject);
    
}

int esMain(ESContext * esContext) {
    
    esContext->userData = malloc(sizeof(UserData));
    
    esCreateWindow(esContext,
                   "instancing",
                   640,
                   480,
                   ES_WINDOW_RGB | ES_WINDOW_DEPTH);
    
    if (!Init(esContext)) {
        
        return GL_FALSE;
        
    }
    
    esRegisterShutdownFunc(esContext, Shutdown);
    
    esRegisterUpdateFunc(esContext, Update);
    
    esRegisterDrawFunc(esContext, Draw);
    
    return GL_TRUE;
    
}



















