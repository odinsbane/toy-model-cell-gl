#ifndef __APPLE__
    #include <GL/glew.h>
    #include "GLFW/glfw3.h"
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
    #include "GLFW/glfw3.h"
#endif

#include "error.h"

#include "MeshObject.h"
#include "CellObject.h"
#include "Framework.h"
#include "CellCam.h"
#include "AxisObject.h"
#include "TiffWriter.h"
#include "InputAdapter.h"
#include "Simulation.h"
#include "texturedbackground.h"
#include <pthread.h>
#include <unistd.h>
#define STEPS_PER_FRAME 500

void showMesh(CellObject* obj);
void* update_cell_function( void* ptr);

MeshObject* createGround();
int main(int arg_no, char** args){
    if(arg_no==2){
        
        printf("starting\n");
        
       const std::string meshName = std::string(args[1]);
       MeshObject* obj = MeshObject::LoadMesh(meshName);
       CellObject* cell = new CellObject(obj->getVertexPositions(), obj->getVertexCount());
       delete obj;

       showMesh(cell);

    } else if(arg_no==3){
        const std::string binarymesh = std::string(args[2]);
        CellObject* cell = new CellObject(binarymesh);
        showMesh(cell);
    } else{
    
        printf("show mesh by: view_mesh path/tofile.mesh \n");
        printf("or binary mesh: view_mesh -b path/tofile.bmf \n");
    
    }
}

/**
 * Takes a mesh object, then creates a gl context for the mesh object 
 * and displays it in its own glfw window. For debuggin meshes.
 * 
 * */
 
 GLuint vao;

 pthread_mutex_t lock;
 pthread_mutex_t update_lock;

 int steps = 0;
void showMesh(CellObject* obj){
    printf("initting glfw\n");
    CellObject cell = *obj;
    GLFWwindow* window = Framework::initializeWindow();
    GLuint theProgram = Framework::loadProgram();
    GLuint groundProgram = Framework::loadGroundProgram();
    
    glUseProgram(theProgram);
    GLuint mesh_color = glGetUniformLocation(theProgram,"useMeshColor");
    glUniform1ui(mesh_color, GL_TRUE);
    glUseProgram(0);
    //mesh->initializeVertexBuffer(theProgram);
    GetError();
    cell.createContractileRing();
#ifdef DEBUG
    GLuint id2 = glGetAttribLocation(theProgram, "inputColor");
    GLuint id = glGetAttribLocation(theProgram, "position");
    printf("Attribute locations: positions - %d // colors - %d\n",id, id2);
#endif
    MeshObject* ground = createGround();
    ground->initializeVertexBuffer(theProgram);
    
    GetError();
    //Create the camera
    CellCam* camera = new CellCam(theProgram);
    GetError();
    printf("finished camera\n");
    cell.prepareBuffers(theProgram);
    GetError();
    printf("buffer prepared\n");

    int width,height;
    //glfwGetWindowSize(window, &width, &height);
    glfwGetFramebufferSize(window, &width, &height);
    
    camera->resizeWindow((float)width, (float)height);
    
    TexturedBackground* bg = new TexturedBackground();
    bg->initializeVertexBuffer(groundProgram);

    AxisObject* axis = new AxisObject();
    axis->initializeVertexBuffer(theProgram);

    TiffWriter* writer = new TiffWriter("testing.tiff",height, width);
    
    int images = 0;
    int counter = 0;
    char* pixbuf = new char[height*width*3];
    for(int i = 0 ; i < 150; i++){
        camera->rotate(0, -0.01f);
    }
    InputAdapter* input = new InputAdapter(camera, &cell, window);


    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&update_lock, NULL);

    pthread_mutex_lock(&update_lock);

    pthread_t updater;
    pthread_create(&updater, NULL, update_cell_function,(void*)&cell);
    pthread_detach(updater);

    while( input->running==GL_TRUE )
    {

        glfwPollEvents();
        //glClearColor(1.0f, 0.9f, 0.9f, 0.9f);
        glClearDepth(1.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        
        input->update();
        
        if(input->draw_axis){
            axis->draw(theProgram);
        }

        cell.draw(theProgram);

        ground->draw(theProgram);
        
        bg->draw(groundProgram);

        GetError();

        // Swap front and back rendering buffers
        glfwSwapBuffers(window);

        
        
        
        if(images<200&&counter>STEPS_PER_FRAME){
            glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixbuf);
            writer->writeFrame(pixbuf);
            images++;
            counter = 0;
            
            if(images==800){
                printf("finished recording\n");
            }
        }
        counter++;
        
        cell.updateBuffers();
        //pthread_mutex_unlock(&update_lock);
        //int trylock = pthread_mutex_trylock(&update_lock);
        //while(trylock!=0){
        //    trylock = pthread_mutex_trylock(&update_lock);
        //}
        
    }
    writer->close();
    
    
    // Close window and terminate GLFW
    glfwTerminate();
    
    // Exit program
    exit( EXIT_SUCCESS );
    
}

void* update_cell_function( void* ptr){
    int acquire = 0;
    int release = 0;
    while((acquire==0)&(release==0)){

        ((CellObject*)ptr)->update();
        
    }
    pthread_exit(NULL);
    return 0;
    
}

MeshObject* createGround(){
    float points[24] = {
        -2, -2, -0.5,
        2, -2, -0.5,
        2, 2, -0.5,
        
        -2, 2, -0.5,
        -2, -2, -0.5,
        2, 2, -0.5

    };
    float color[4] = {1.0f,1.0f,1.0f,1.0f};
    MeshObject* ground = MeshObject::SolidColorMesh(points, color, 6);
    ground->setDrawType(MeshObject::SHADED_SURFACE);
    return ground;
}
