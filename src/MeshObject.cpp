#include "MeshObject.h"

const int MeshObject::SOLID_COLOR = 2;
const int MeshObject::SHADED_SURFACE = 3;

MeshObject* MeshObject::LoadMesh(const std::string &meshFileName){
    
    #ifdef MESH_DEBUG
    printf("loading mesh from file: %s \n", meshFileName.c_str());
    #endif
    
    std::ifstream meshFileStream(meshFileName.c_str());
    std::stringstream meshFileData;
    meshFileData << meshFileStream.rdbuf();
    meshFileStream.close();

    MeshObject* ret = new MeshObject(meshFileData.str());

    return ret;
}

MeshObject* MeshObject::SolidColorMesh(float* verticies, float* color, int count){
    MeshObject* ret = new MeshObject();
    ret->vertexPositions = new float[count*7];
    for(int i = 0; i<count*3; i++){
        ret->vertexPositions[i] = verticies[i];
    }
    for(int i = 0; i<count*4; i++){
        ret->vertexPositions[i+3*count] = color[i%4];
    }

    ret->vertexCount = count;
    return ret;
    
}

MeshObject::MeshObject(const std::string &meshFileData){
    const std::vector<std::string*> vertexLines = MeshObject::SplitMeshFileString(meshFileData, '\n');
    
    
    // mesh data only contains 3 currently
    vertexCount = (int)vertexLines.size();
    printf("%d vertecies\n",vertexCount);
    //3 floats per vertex position and four per color.
    vertexPositions = new float[vertexCount*(3 + 4)];
    
    #ifdef DEBUG
    printf("%d lines of mesh loaded\n", (int)vertexLines.size());
    #endif

    int lines = (int)vertexLines.size();
    
    //size of the position vertex array
    int OFFSET = (vertexCount*3);
    
    for(int i = 0; i<lines; i++){
        
        std::string* line = vertexLines[i];
        
        const std::vector<std::string*> ordinates = SplitMeshFileString(*line,' ');
        
        if(ordinates.size()<6){
            printf("bad ordinate line at %d with %d members\n", i, (int)ordinates.size());
            printf("%s\n",ordinates[0]->c_str());
            printf("trying to continue\n");
        
        } 
        //first three points are the coordinates.
        for(int j = 0; j<3; j++){
            float ord;
            sscanf(ordinates[j]->c_str(),"%f", &ord);
            vertexPositions[i*3 + j] = ord;
            sscanf(ordinates[j+3]->c_str(), "%f",&ord);
            vertexPositions[OFFSET + i*4 + j] = ord;
        }
        
        vertexPositions[i*4 + 3 + OFFSET] = 1.f;

    }
    buffered=0;
    
}

MeshObject::MeshObject(){
    buffered=0;
} 

/**
 * Breaks up a vertex file data.
 * */
const std::vector<std::string*> MeshObject::SplitMeshFileString(const std::string &fileString, char c){
    std::vector<std::string*> pieces;
    
    const char* fileChars = fileString.c_str();
        
    int length = (int)fileString.length();
    int i,j;

    for(i = 0;i<length;i++){

        //starting a vertex.
        for(j = 0; j<length-i; j++){
                
            //found the splitter create a new string.
            if(fileChars[i+j]==c){

                if(j>0&&fileChars[i]!='#'){
                    char piece[j+1];
                    piece[j] = 0;           
                    for(int k = 0; k<j; k++){
                        piece[k] = fileChars[i+k];
                    }
                    std::string* sp = new std::string(piece);
                    pieces.push_back(sp);
                }
                //move to next word 
                i+=(j+1);
                
                //start over
                j = -1;
            }
        }
        
        //check if there is a last piece that needs to be added.
        if((j>0)&&(fileChars[i]!='#')){
            
            //create a null terminated string.
            char piece[j+1];
            piece[j] = 0;           
            for(int k = 0; k<j; k++){
                piece[k] = fileChars[i+k];
            }
            std::string* sp = new std::string(piece);
            pieces.push_back(sp);
            
        }
    }
    
    return pieces;
}

void MeshObject::shutdown(){
    delete vertexPositions;
    if(buffered==1){
        glDeleteBuffers(1,&positionBufferObject);
    }
    //positionBufferObject = 0;

}

GLuint MeshObject::getBufferObject(){
    return positionBufferObject;
}

float* MeshObject::getVertexPositions(){
    return vertexPositions;
}

int MeshObject::getVertexCount(){
    return vertexCount;
}

/**
 * The number of floats stored in the mesh data.
 * */
int MeshObject::getSize(){
    //each vertex has four position and four color
    return vertexCount*7;
}

void MeshObject::initializeVertexBuffer(GLuint &theProgram){
    glUseProgram(theProgram);
    
    
    glGenBuffers(1, &positionBufferObject);
    
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexCount*7, vertexPositions, GL_STREAM_DRAW);
    
    
    GLuint position_index = glGetAttribLocation(theProgram, "position");
    GLuint color_index = glGetAttribLocation(theProgram, "inputColor");
    printf("pos: %d col: %d \n", position_index, color_index);
    
    GetError();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GetError();
    
    glEnableVertexAttribArray(position_index);
    glEnableVertexAttribArray(color_index);
    
    glVertexAttribPointer(position_index, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    //index, elements, type, ordering, dunno, offset
    glVertexAttribPointer(color_index, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float)*vertexCount*3));
    
    GetError();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    buffered=1;
}

void MeshObject::draw(GLuint &theProgram){
    
    glUseProgram(theProgram);
    GLuint colorMode = glGetUniformLocation(theProgram, "colorMode");
    glUniform1i(colorMode, drawType);
    
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    
    glBindVertexArray(0);
    glUseProgram(0);

}


void MeshObject::setColor(int vertex, float* color){
    /*
     * First, the buffer objects containing the positions is bound to the context. 
     * Then the new function glBufferSubData is called to transfer 
     * this data to the buffer object.
     * */
    
    int offset = vertexCount*3 + vertex*4;
    for(int i = 0; i<3; i++){
        vertexPositions[offset+i] = color[i];
    }
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(float), sizeof(float)*4, vertexPositions);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void MeshObject::setDrawType(int type){
    drawType=type;
}
