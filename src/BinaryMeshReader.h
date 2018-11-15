#ifndef BINARYMESHREADER
#define BINARYMESHREADER

#include <stdio.h>
#include <fstream>
#include <vector>

struct BinaryMesh{
    double* positions;
    int* triangle_indicies;
    int* connection_indicies;
    int triangle_count, connection_count, position_count;
};
class BinaryMeshReader{
    std::vector<double*> all_positions;
    std::vector<int*> all_triangles;
    std::vector<int*> all_connections;
    int *connection_count, *triangle_count, *position_count;
    void shutdown();
public:
    BinaryMeshReader(const std::string &filename);
    
    BinaryMesh getMesh(int frame);
    ~BinaryMeshReader(){
        shutdown();
    }
};









#endif