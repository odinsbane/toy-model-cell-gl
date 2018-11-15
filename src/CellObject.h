#include <vector>
#include <string>
#include <stdio.h>
#ifndef __APPLE__
    #include <GL/glew.h>
    #include "GLFW/glfw3.h"
#else
    #define GLFW_INCLUDE_GLCOREARB
    #define GLFW_NO_GLU
    #include "GLFW/glfw3.h"
#endif

#include "math.h"
#include "error.h"
#include "BinaryMeshReader.h"

#ifndef _CELL_OBJECT
#define _CELL_OBJECT

#define t_index unsigned int
#define NOT_INIT -1
#define DT 1e-2
#define SUB_SAMPLE 10

class Connection{
    t_index indices[2];

    float* a;
    float* b;
    public:
        Connection(t_index a_index, t_index b_index);
        void setPositions(float* positions);
        void setSeparatePositions(float* _a, float* _b);
        bool hasIndex(t_index t);
        void update();
        float length;
        float center[3];
        float stiffness;
        t_index getIndex(int a);
        float concentration;
        float lnot;

};

class Triangle{
    t_index indices[3];
    //float tau_c;
    
    public:
        Triangle();
        t_index getNodeIndex(int i);
        void setNodeIndex(int i , t_index index);
        void setPoints(float* data);
        void update();
        float* normal;
        float* points[3];
        float c, area;
        float actin;
        void calculateNormal();
        //float c_not;
        float tau_c;
        float c_not;



};

class Node{
    

    public:
    static float DRAG;
    static float ACTIVE_TENSION;
    static float PNOT;
    static float SPRING;
        Node();
        std::vector<Triangle*> triangles;
        std::vector<Connection*> connections;
        t_index index;
        float* color;
        Node(float* r);
        void setPosition(float* r);
        void setNormal(float* n);
        float distance(Node* n);
        float distance(float* r);
        void addTriangle(Triangle* t);
        Triangle* getTriangle(int dex);
        int getTriangleCount();
        float* getCoordinates();
        void addConnection(Connection* connection);
        bool isConnected(t_index other);
        virtual void update(float dv);
        float* xyz;
        float* normal;
        void setConcentration(float v);
        void setColor(float* c);
        float concentration;

};

class CellObject{
    
    GLuint proggy;
    std::vector<Node*> nodes;
    float* node_data;
    std::vector<Triangle*> triangles;
    std::vector<Connection*> connections;
    t_index* indices;
    GLuint vertexBufferObject, indexBufferObject, vaoObject;
    int verticesCount;
    size_t index_count, node_count;
    int working;
    CellObject();
    bool draw_connections;
    bool perturb_request;

    std::vector<Node*> stationaries;
    public:
        CellObject(float* triangle_data, int vertex_count);
        CellObject(const std::string &meshFileName);
        void draw(GLuint &theProgram);
        void prepareBuffers(GLuint &theProgram);
        void update();
        void updateBuffers();
        float calculateVolume();
        float Vnot;
        void constrict();
        void toggleDrawConnections();
        bool isWorking();
        void requestPerturbation();
        void createContractileRing();
    private:
        void perturb();
};


class StationaryNode : public Node{
    
    public:
        StationaryNode(float* r);
        void update(float dv);
    
};



#endif
