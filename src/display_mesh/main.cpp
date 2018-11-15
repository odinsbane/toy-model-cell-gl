#ifndef __APPLE__
#include <GL/glew.h>
#define DEBUG
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_NO_GLU
#endif

#include "GLFW/glfw3.h"

#include "error.h"

#include "MeshObject.h"
#include "CellObject.h"
#include "Framework.h"
#include "CellCam.h"
#include "AxisObject.h"
#include "TiffWriter.h"
#include "InputAdapter.h"


#include <stdio.h>
#include <vector>

int main(int argc, const char * argv[])
{
    if(argc>1){
        
        printf("starting\n");
        
    } else{
        
        printf("show mesh by: view_mesh path/tofile.mesh \n");
        return 0;
    }
    
    GLFWwindow* window = Framework::initializeWindow();
    
    GLuint theProgram = Framework::loadProgram();

    const std::string meshName = std::string(argv[1]);
    
    MeshObject* mesh = MeshObject::LoadMesh(meshName);
    
    
    //glUseProgram(theProgram);
    //GLuint mesh_color = glGetUniformLocation(theProgram,"useMeshColor");
    //glUniform1ui(mesh_color, GL_TRUE);

    //glUseProgram(0);
    
    mesh->initializeVertexBuffer(theProgram);
    GetError();
    
    //Create the camera
    CellCam* camera = new CellCam(theProgram);
    GetError();
    int height, width;
    glfwGetWindowSize(window, &width, &height );

    GetError();
    
    camera->resizeWindow((float)width, (float)height);
    
    //delete mesh;
    
    AxisObject* axis = new AxisObject();
    axis->initializeVertexBuffer(theProgram);
    
    for(int i = 0 ; i < 150; i++){
        camera->rotate(0, -0.01f);
    }

    InputAdapter* input = new InputAdapter(camera, NULL, window);
    
    while( input->running==GL_TRUE )
    {
        
        
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        input->update();
        
        if(input->draw_axis){
            axis->draw(theProgram);
        }
        
        mesh->draw(theProgram);
        GetError();
        
        // Swap front and back rendering buffers
        glfwSwapBuffers(window);
        
        
    }
    
    
    // Close window and terminate GLFW
    glfwTerminate();
    
    // Exit program
    exit( EXIT_SUCCESS );
    
    
    return 0;
}

