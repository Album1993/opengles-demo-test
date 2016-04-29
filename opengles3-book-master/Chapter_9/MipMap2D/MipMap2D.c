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
// MipMap2D.c
//
//    This is a simple example that demonstrates generating a mipmap chain
//    and rendering with it
//
#include <stdlib.h>
#include "esUtil.h"

typedef struct
{
    // Handle to a program object
    GLuint programObject;
    
    // Sampler location
    GLint samplerLoc;
    
    // Offset location
    GLint offsetLoc;
    
    // Texture handle
    GLuint textureId;
    
} UserData;

/**
 *  From an RGB8 source image, generate the next level mipmap
 */
GLboolean GenMipMap2d(GLubyte * src , GLubyte **dst, int srcWidth,int srcHeight,int *dstWidth,int * dstHeight) {
    int x,
        y;
    int texelSize = 3;
    
    *dstWidth = srcWidth / 2;
    
    if (*dstWidth <= 0)
    {
        *dstWidth = 1;
    }
    
    *dstHeight = srcHeight / 2;
    if (*dstHeight <= 0) {
        *dstHeight = 1;
    }
    
    *dst = malloc(sizeof(GLubyte) * texelSize * (*dstWidth) * (*dstHeight));
    if (*dst == NULL) {
        return GL_FALSE;
    }
    
    for (y = 0; y < *dstHeight; y++) {
        for (x = 0; x < * dstWidth; x++) {
            int srcIndex[4];
            float r = 0.0f,
                  g = 0.0f,
                  b = 0.0f;
            int sample;
            
            // Compute the offsets for 2 x 2 grid of pixels in previous
            // image to perform box filter
            srcIndex[0] = (((y * 2) * srcWidth) + (x * 2)) * texelSize;
            srcIndex[1] = ((y * 2) * srcWidth) + (x * 2 + 1)* texelSize;
            
        }
    }
    
}


















