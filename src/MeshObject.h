#ifndef __APPLE__
    #include <GL/glew.h>
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
#endif
#include "GLFW/glfw3.h"


#include <vector>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "error.h"
#ifndef _MESH_OBJECT
#define _MESH_OBJECT

class MeshObject{
    /**
     *  Version 0.1 of a MeshObject for loading predrawn meshes.
     *  Still unsure about the purpose of meshes.
     * 
     *  The meshfile contains a point for each vertex and a color, x,y,z,r,g,b
     *  
     * */
    void shutdown();
    float* vertexPositions;
    int vertexCount;
    const std::vector<std::string*> SplitMeshFileString(const std::string &meshFileString, char c);
    GLuint positionBufferObject, vao;
    int buffered;
    MeshObject(const std::string &meshFileData);
    MeshObject();
    int drawType=2;
    public:
        ~MeshObject(){
            shutdown();
        }
        
        static MeshObject* LoadMesh(const std::string &meshFileName);
        static MeshObject* SolidColorMesh(float* verticies, float* color, int count);
        GLuint getBufferObject();
        
        void initializeVertexBuffer(GLuint &theProgram);
        float* getVertexPositions();
        int getVertexCount();
        int getSize();
        void draw(GLuint &theProgram);
        void setColor(int vertex, float* value);
        void setDrawType(int type);
    
        static const int SOLID_COLOR;
        static const int SHADED_SURFACE;
};

#endif
