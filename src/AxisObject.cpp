#include "AxisObject.h"

AxisObject::AxisObject()
{
}

void AxisObject::initializeVertexBuffer(GLuint &theProgram){
    positions = new float[7*4];
    positions[0] = 0.0;
    positions[1] = 0.0;
    positions[2] = 0.0;

    positions[3] = 1.0;
    positions[4] = 0.0;
    positions[5] = 0.0;

    positions[6] = 0.0;
    positions[7] = 1.0;
    positions[8] = 0.0;

    positions[9] = 0.0;
    positions[10] = 0.0;
    positions[11] = 1.0;
    for(int i = 0; i<4; i++){

        positions[12 + 4*i + 0] = (1 - 0.25*i);
        positions[12 + 4*i + 1] = 0.0;
        positions[12 + 4*i + 2] = 0.25*i;
        positions[12 + 4*i + 3] = 0.5;
    }

    indices = new unsigned short[6];

    for(int i = 0; i<3; i++){
        indices[2*i] = 0;
        indices[2*i+1] = i+1;
    }

    glUseProgram(theProgram);

    size_t colorDataOffset = sizeof(float) * 3 * 4;

    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*7*4, positions, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short)*6, indices, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &vaoObject);
    glBindVertexArray(vaoObject);

    GLuint positionAttribute = glGetAttribLocation(theProgram, "position");
    GLuint colorAttribute = glGetAttribLocation(theProgram, "inputColor");

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(colorAttribute);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

    glBindVertexArray(0);

    glUseProgram(0);

}

void AxisObject::draw(GLuint &theProgram){
    glUseProgram(theProgram);
    GLuint colorMode = glGetUniformLocation(theProgram,"colorMode");
    glUniform1i(colorMode, 2);
    glBindVertexArray(vaoObject);

    glDrawElements(GL_LINES, 6, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

    glUseProgram(0);
}
