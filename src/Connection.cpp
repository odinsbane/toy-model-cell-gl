#include "CellObject.h"

Connection::Connection(t_index a_index, t_index b_index){
    indices[0] = a_index;
    indices[1] = b_index;
    stiffness = 0.1;
    concentration=1.0;
    length=1;
    lnot=1;
}

bool Connection::hasIndex(t_index t){
    return (t-indices[0]==0)||(t-indices[1]==0);
}

void Connection::setPositions(float* positions){
    
    a = &positions[3*indices[0]];
    b = &positions[3*indices[1]];
    length = 0;
    float v;
    for(int i = 0; i<3; i++){
        center[i] = (a[i] +b[i])*0.5;
        v = a[i] - b[i];
        length += v*v;
    }
    length = sqrt(length);
    lnot = 0.02;
    stiffness = stiffness/lnot;

}

void Connection::setSeparatePositions(float *_a, float *_b){
    a = _a;
    b = _b;
    length = 0;
    float v;
    
    for(int i = 0; i<3; i++){
        center[i] = (a[i] +b[i])*0.5;
        v = a[i] - b[i];
        length += v*v;
    }
    
    length = sqrt(length);
    lnot = length*0.5;
    stiffness = stiffness/lnot;

}

void Connection::update(){
    
    length = 0;
    float v;
    for(int i = 0; i<3; i++){
        v = 0;
        center[i] = (a[i] + b[i])*0.5;
        v = a[i] - b[i];
        length += (v*v);
        
    }
    length = sqrt(length);
    if(length==0){
        printf("%d, %d, %f, %f, %f, %f, %f\n", indices[0], indices[1], a[0], a[1], b[0], b[1], length);
        exit(5);
    }
    
}

t_index Connection::getIndex(int a){
    return indices[a];
}
