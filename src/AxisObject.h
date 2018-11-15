#ifndef __APPLE__
    #include <GL/glew.h>
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
#endif
#include "GLFW/glfw3.h"
#include <stdlib.h>
#ifndef AXISOBJECT_H
#define AXISOBJECT_H

class AxisObject
{
    float* positions;
    GLuint positionBufferObject, indexBufferObject, vaoObject;
    unsigned short* indices;


public:
    AxisObject();
    void initializeVertexBuffer(GLuint &theProgram);
    void draw(GLuint &theProgram);

};

#endif // AXISOBJECT_H
