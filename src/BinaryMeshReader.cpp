#include "BinaryMeshReader.h"
int getInt(char* integer);
/*
 * test.bmf
 * frame: 0 with 2274 position indices with 4536 connection indices and 4536 triangle indices
 * */

int nextInt(std::ifstream* stream);
double nextDouble(std::ifstream* stream);
BinaryMeshReader::BinaryMeshReader(const std::string &filename){
    
    std::ifstream* inputStream = new std::ifstream(filename, std::ifstream::binary);
    
    int count = nextInt(inputStream);
    connection_count = new int[count];
    triangle_count = new int[count];
    position_count = new int[count];
    printf("%d meshes included \n", count);
    
    for(int i = 0; i<count; i++){

        int frame = nextInt(inputStream);
        printf("loading mesh for frame %d \n",frame);
          
        int p_count = nextInt(inputStream);
        printf("with %d position doubles\n", p_count);
        double* positions = new double[p_count];
          
        for(int j = 0; j<p_count; j++){
            
            positions[j] = nextDouble(inputStream);
        }
        
        int c_count = nextInt(inputStream);
        int* connections = new int[c_count];
        
        for(int j = 0; j<c_count; j++){
            connections[j] = nextInt(inputStream);
        }
          
        
        int t_count = nextInt(inputStream);
        int* triangles = new int[t_count];
        for(int j = 0; j<t_count; j++){
            triangles[j] = nextInt(inputStream);
        }
        
        printf("frame %d: %d position doubles, %d connection indices, %d triangle indices.\n",frame, p_count, c_count, t_count);
        
        connection_count[i] = c_count/2;
        triangle_count[i] = t_count/3;
        position_count[i] = p_count/3;
        all_positions.push_back(positions);
        all_triangles.push_back(triangles);
        all_connections.push_back(connections);
        
    }
}

 char* first = new char[8];
 char* second = new char[8];
 
 int nextInt(std::ifstream* stream){
        stream->read(first, 4);
        for(int i = 0; i<4; i++){
            second[i] = first[3 - i];
        }
        return ((int*)second)[0];
 }
 
 double nextDouble(std::ifstream* stream){
     stream->read(first, 8);
     for(int i = 0; i<8; i++){
        second[i] = first[7-i];
     }
     return ((double*)second)[0];
 }

 int getInt(char* integer){
        
        /*unsigned int v = 0;
        for(int i = 0; i<4; i++){
                unsigned char c = (unsigned char)integer[i];
                //v = v + ((integer[i]&0xff)<<(8*(3-i)));
                v = (v<<8) + c;
        }
        printf(" %ud \n",v);*/
        return ((int*)integer)[0];
}

void BinaryMeshReader::shutdown(){
    for(double* positions: all_positions){
        delete[] positions;
    }
    for(int* triangles: all_triangles){
        delete[] triangles;
    }
    for(int* connections: all_connections){
        delete[] connections;
    }
}

BinaryMesh BinaryMeshReader::getMesh(int frame){

    BinaryMesh b;
    b.position_count = position_count[frame];
    b.triangle_count = triangle_count[frame];
    b.connection_count = connection_count[frame];
    b.positions = all_positions[frame];
    b.connection_indicies = all_connections[frame];
    b.triangle_indicies = all_triangles[frame];
    return b;
    
}


