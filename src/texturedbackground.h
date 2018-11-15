#ifndef TEXTUREDBACKGROUND_H
#define TEXTUREDBACKGROUND_H

#ifndef __APPLE__
    #include <GL/glew.h>
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
#endif
#include "GLFW/glfw3.h"

class TexturedBackground
{
    GLuint positionBufferObject, vao;
    GLuint texBufferdObject;
    const int texUnit=0;
public:
    TexturedBackground();
    void initializeVertexBuffer(GLuint &theProgram);
    void draw(GLuint &theProgram);
};

#endif // TEXTUREDBACKGROUND_H
