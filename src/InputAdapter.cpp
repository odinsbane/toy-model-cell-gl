//
//  InputAdapter.cpp
//  CellGL
//
//  Created by msmith on 5/9/13.
//
//

#include "InputAdapter.h"

InputAdapter::InputAdapter(CellCam* cam, CellObject* c, GLFWwindow* w){
    camera = cam;
    cell = c;
    window = w;
    running = GL_TRUE;
    
}

void InputAdapter::update(){
    // Check if ESC key was pressed or window was closed
    running = (!glfwGetKey(window, GLFW_KEY_ESCAPE )) && !glfwWindowShouldClose(window);
    
    
    if(glfwGetKey(window,  GLFW_KEY_LEFT )){
        camera->rotate(-0.01f, 0);
        
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT)){
        camera->rotate(0.01f, 0);
        
    }
    
        
    if(glfwGetKey(window, GLFW_KEY_UP )){
        camera->rotate(0, 0.01f);
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN)){
        camera->rotate(0, -0.01f);
    }
    
    
    if( glfwGetKey(window, GLFW_KEY_A ) ){
        camera->zoom(-0.1f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_Z ) ){
        camera->zoom(0.1f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE)){
        draw_axis = !draw_axis;
    }
    
    
    
    
    if(cell !=NULL){
        if (glfwGetKey(window, GLFW_KEY_B )){
            cell->requestPerturbation();
        }
        if (glfwGetKey(window, GLFW_KEY_ENTER)){
            
            cell->constrict();
            
        }

        if(glfwGetKey(window, GLFW_KEY_C )){
            cell->toggleDrawConnections();
        }

    }
    
}
