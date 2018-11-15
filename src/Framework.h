#ifndef FRAMEWORK_H
#define FRAMEWORK_H
#ifndef __APPLE__
    #include <GL/glew.h>
    #include "GLFW/glfw3.h"
    #define SHADER_VERSION_STRING "#version 130\n\n"
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
    #include "GLFW/glfw3.h"
    #define SHADER_VERSION_STRING "#version 150\n\n"
#endif


#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "MeshObject.h"

#include "error.h"
#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

namespace Framework
{
	float DegToRad(float fAngDeg);

	GLuint CreateShader(GLenum eShaderType,
		const std::string &strShaderFile, const std::string &strShaderName);
	GLuint LoadShader(GLenum eShaderType, const std::string &strShaderFilename);
	GLuint CreateProgram(const std::vector<GLuint> &shaderList);
    void multiplyMatrix4x4(float* A, float* B, float* C);
    float* multiplyMatrixVector4(float* A, float* b);
    GLFWwindow* initializeWindow();
    GLuint loadProgram();
    GLuint loadGroundProgram();
}

#endif //FRAMEWORK_H
