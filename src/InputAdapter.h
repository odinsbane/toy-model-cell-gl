//
//  This class if for updating input as it arrives from glfw
//  it controls the camera and cell at this point
//  InputAdapter.h
//  CellGL
//
//  Created by msmith on 5/9/13.
//
//

#ifndef __CellGL__InputAdapter__
#define __CellGL__InputAdapter__
#include "CellCam.h"
#include "CellObject.h"

#ifndef __APPLE__
    #include <GL/glew.h>
    #include "GLFW/glfw3.h"
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
    #include "GLFW/glfw3.h"
#endif

#include <iostream>
class InputAdapter{
    CellCam* camera;
    CellObject* cell;
    GLFWwindow* window;
    public:
        InputAdapter(CellCam* cam, CellObject* c, GLFWwindow* w);
        void update();
        int running;
        bool draw_axis;
    
};


#endif /* defined(__CellGL__InputAdapter__) */
