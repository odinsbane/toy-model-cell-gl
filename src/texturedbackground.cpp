#include "texturedbackground.h"
#include <stdio.h>
#include "error.h"
TexturedBackground::TexturedBackground(){

}

void TexturedBackground::initializeVertexBuffer(GLuint &theProgram){
    float positions[12]{
        -1, -1,
        1, -1,
        -1, 1,
        -1, 1,
        1, -1,
        1,1
    };

    GLubyte checkered[16]{
      255, 225, 255, 255,     255, 225, 225, 255,
      255, 225, 225, 255,    255, 225, 255, 255
    };


    glUseProgram(theProgram);


    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12, positions, GL_STREAM_DRAW);
    GLuint position_index = glGetAttribLocation(theProgram, "position");
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(position_index);
    glVertexAttribPointer(position_index, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    glUseProgram(theProgram);
    glGenTextures(1, &texBufferdObject);
    glBindTexture(GL_TEXTURE_2D, texBufferdObject);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_BGRA, GL_UNSIGNED_BYTE, checkered);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint samplerUniform = glGetUniformLocation(theProgram, "texSampler");
    glUniform1i(samplerUniform, texUnit);

    glUseProgram(0);

}

void TexturedBackground::draw(GLuint &theProgram){
    GLuint background = glGetUniformLocation(theProgram, "background");
    
    glUseProgram(theProgram);
    
    glUniform1f(background, 1.0);
    
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, texBufferdObject);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
