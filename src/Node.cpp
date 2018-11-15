#include "CellObject.h"

Node::Node(){
    concentration = 0;
}
Node::Node(float* r){
    xyz = r;
    concentration = 0;
}

float Node::distance(float* pt){
    float sum = 0;
    float v;
    for(int i = 0;i<3; i++){
        v = xyz[i] - pt[i];
        sum += v*v;
    }
    return sqrt(sum);
}

void Node::addTriangle(Triangle* t){
    triangles.push_back(t);
}

int Node::getTriangleCount(){
    return (int)triangles.size();
}

float* Node::getCoordinates(){
    return xyz;
}

void Node::setPosition(float* r){
    xyz = r;
}

Triangle* Node::getTriangle(int dex){
    
    return triangles[dex];
    
}

bool Node::isConnected(t_index other_index){
    
    for(int i = 0; i<(int)connections.size(); i++){
        Connection* con = connections[i];
        if(con->hasIndex(other_index)){
            return true;
        }
    }
    
    return false;
    
}

void Node::addConnection(Connection* con){
    connections.push_back(con);
}

float tension(float con){
    return ACTIVE_TENSION*con/(50.0 + con);
}

void Node::update(float dv){
    if(index==10 & isnan(xyz[0])){
        printf("before update\n");
        exit(1);
    }
    //sum forces and move points.
    float fx = 0;
    float fy = 0;
    float fz = 0;
    if(xyz[2]<-0.5){
        fz=0.05;
    } else{
        fz=0.0;
    }
    int cn = (int)connections.size();
    float max = -1;
    float min = 1e19;
    for(int i = 0; i < cn; i++){
        Connection* con = connections[i];
        float* center = con->center;

        float k = (con->stiffness*(-con->lnot  + con->length) + tension(con->concentration))*SPRING/(0.5*con->length);
        if(isinf(k)|isnan(k)){
            printf("\n\n%f, %f, %f, %f\n\n",con->length, k, con->lnot, con->stiffness, con->concentration);
            exit(4);
        }
        if(k<min){
            min = k;
        }
        if(k>max){
            max = k;
        }
        if(isnan(k)|isnan(center[0])){
            printf("k or center %f,%f\n",k, center[0]);
            exit(1);
        }
        fx -= (xyz[0] - center[0])*k;
        fy -= (xyz[1] - center[1])*k;
        fz -= (xyz[2] - center[2])*k;
    }
    
    float pressure = PNOT*dv;
    
    //printf("pressure %f\n", pressure);
    
    /*if(index==0){
     printf("pressure %f\n",
     pressure);
     }*/
    
    Triangle* t;
    float* n;
    concentration = 0;
    size_t tc = triangles.size();
    
    normal[0] = 0;
    normal[1] = 0;
    normal[2] = 0;
    for(size_t i = 0; i<tc; i++){
        t = triangles[i];
        n = t->normal;
        double iarea = 1.0/t->area;
        
        normal[0] += n[0]*iarea;
        normal[1] += n[1]*iarea;
        normal[2] += n[2]*iarea;
        
        fx += pressure*n[0];
        fy += pressure*n[1];
        fz += pressure*n[2];
        
        
        concentration += t->c;
        
    }
    normal[0] = normal[0]/tc;
    normal[1] = normal[1]/tc;
    normal[2] = normal[2]/tc;
    
    //printf("normal: %f %f %f \n", normal[0], normal[1], normal[2]);
    if((index==10)&isnan(fx)){
        printf("fx\n");
    }
    if((index==10)&isnan(fy)){
        printf("fy\n");
    }
    if((index==10)&isnan(fz)){
        printf("fz\n");
    }

    xyz[0] = xyz[0] + fx*DT*DRAG;
    xyz[1] = xyz[1] + fy*DT*DRAG;
    xyz[2] = xyz[2] + fz*DT*DRAG;
    
    concentration = concentration/tc;
    color[0] = concentration<1.0?1.0f:0.8;
    color[1] = 1.0;
    color[2] = concentration<1.0?1.0f:0.8;
    

}

void Node::setConcentration(float v){
    for(Triangle* t: triangles){
        t->c_not = v;
    }
}

void Node::setNormal(float* n){
    normal = n;
}

void Node::setColor(float* c){
    color=c;
}

void StationaryNode::update(float dv){
    
    //printf("pressure %f\n", pressure);
    
    Triangle* t;
    concentration = 0;
    for(int j = 0; j<3; j++){
        normal[j] = 0;
    }
    for(int i = 0; i<(int)triangles.size(); i++){
        t = triangles[i];
        
        concentration += t->c;
        for(int j = 0; j<3; j++){
            normal[j] += t->normal[j]/t->area;
        }
        
    }
    for(int j = 0; j<3; j++){
        normal[j] = normal[j]/triangles.size();
    }
    
    
    //printf("%f %f %f \n", fx*DT*DRAG, fy*DT*DRAG, fz*DT*DRAG);
    
    concentration = concentration/triangles.size();
    color[0] = 0.5;
    color[1] = 0.5 + concentration;
    color[2] = 0.5;
}

StationaryNode::StationaryNode(float* r){
    xyz = r;
    concentration = 0;
}

