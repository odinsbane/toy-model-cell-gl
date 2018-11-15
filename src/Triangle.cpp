//
//  Triangle.cpp
//  CellGL
//
//  Created by msmith on 6/28/13.
//
//

#include "CellObject.h"

Triangle::Triangle(){
    c_not = 1;
    tau_c = 250.0;
    for(int i = 0; i<3; i++){
        indices[i] = (t_index)NOT_INIT;
    }
}

t_index Triangle::getNodeIndex(int i){
    return indices[i];
}

void Triangle::setNodeIndex(int i, t_index index){
    indices[i] = index;
}

void Triangle::calculateNormal(){
    float* a = new float[3];
    float* b = new float[3];
    
    for(int i = 0; i<3;i++){
        a[i] = points[1][i] - points[0][i];
        b[i] = points[2][i] - points[0][i];
    }
    normal[0] = (a[1]*b[2] - a[2]*b[1]);
    normal[1] = (a[2]*b[0] - a[0]*b[2]);
    normal[2] = (a[0]*b[1] - a[1]*b[0]);
    
    
}

void Triangle::update(){
    float a[3];
    float b[3];
    
    for(int i = 0; i<3;i++){
        a[i] = points[1][i] - points[0][i];
        b[i] = points[2][i] - points[0][i];
    }
    normal[0] = (a[1]*b[2] - a[2]*b[1]);
    normal[1] = (a[2]*b[0] - a[0]*b[2]);
    normal[2] = (a[0]*b[1] - a[1]*b[0]);
    
    area = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
    c = actin/area;
    
    double dc = -(c - c_not)*DT/tau_c;
    c += dc;
    actin = area*c;
    
    
    
}

void Triangle::setPoints(float* data){
    for(int i = 0; i<3; i++){
        points[i] = &data[3*indices[i]];
    }
    normal = new float[3];
    
    float z = 0;
    for(int i = 0; i<3; i++){
        z += points[i][2];
    }
    
    calculateNormal();
    area = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
    actin = c_not*area;
    
}
