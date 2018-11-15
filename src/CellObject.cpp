#include "CellObject.h"

/*
  triangle data is an array of floats containing the positions and color for each triangle.
  triangle_data[0:3*vertex_count] - positions
  triangle_data[4*vertex_count:8*vertex_count] - colors
*/
CellObject::CellObject(float* triangle_data, int vertex_count){


    float sep = 1e-10;
    verticesCount = vertex_count;
    int colorIndex = vertex_count*3;
    
    t_index not_initialized = (t_index)NOT_INIT;
    int triangle_count = vertex_count/3;
    
    for(int i = 0;i<triangle_count;i++){
        triangles.push_back(new Triangle());
    }
    
    t_index count = 0;
    for(int i = 0; i<triangle_count; i++){
        Triangle* t = triangles[i];
        
        for(int j = 0; j<3; j++){
            t_index index = t->getNodeIndex(j);
            if( index==not_initialized ){
                float* pt = &triangle_data[9*i + 3*j];
                Node* n = new Node(pt);

                n->color = &triangle_data[colorIndex + 12*i + 4*j];
                n->index = count;
                count++;

                nodes.push_back(n);

                t->setNodeIndex(j,n->index);
                n->addTriangle(t);
                
                for(int k = i; k<triangle_count;k++){
                    Triangle* other_t = triangles[k];
                    for(int m = 0; m<3; m++){
                        t_index other_index = other_t->getNodeIndex(m);
                        
                        if(other_index==not_initialized){
                            //check the point.
                            float* other_pt = &triangle_data[9*k + 3*m];
                            float distance = n->distance(other_pt);
                            if(distance<sep){
                                other_t->setNodeIndex(m,n->index);
                                n->addTriangle(other_t);
                            } 
                        }
                    }
                }
                

            }
        }
        
    }
    
    printf("nodes %ld \t triangles %ld \n",nodes.size(), triangles.size());
    
    //one entry per coordinate
    int posEntries = 3*count;
    int colEntries = 4*count;
    int normalEntries = 3*count;
    int nodeEntries = posEntries + colEntries + normalEntries;
    int colorOffset = posEntries;
    
    node_data = new float[nodeEntries];
    
    for(t_index i = 0; i<count; i++){
        Node* n = nodes[i];
        float* old = n->getCoordinates();
        float* color = n->color;
        for(int j = 0; j<3; j++){
            //positions
            node_data[i*3 + j] = old[j];
            
            //colors
            node_data[colorOffset + i*4 + j] = color[j];
            
            //normals
            node_data[posEntries + colEntries + i*3 + j] = 0;

        }
        node_data[colorOffset + i*4 + 3] = color[3];

        n->setPosition(&node_data[i*3]);
        n->setNormal(&node_data[colorOffset + colEntries + i*3]);
        n->color = &node_data[colorOffset + i*4];
    }
    
    index_count = 3*triangle_count;
    indices = new t_index[2*index_count];
    node_count = count;

    //create index array and associate triangles to node data.
    for(int i = 0; i<triangle_count; i++){
        Triangle* triangle = triangles[i];
        for(int j = 0; j<3; j++){
            
            indices[i*3 + j] = triangle->getNodeIndex(j);
            
        }
        triangle->setPoints(node_data);
        triangle->update();
    }
    
    

    for(int i = 0; i<node_count; i++){
        Node* node = nodes[i];
        int tri_count = node->getTriangleCount();
        for(int j = 0; j<tri_count; j++){
            Triangle* tri = node->getTriangle(j);
            for(int k = 0; k<3; k++){
                t_index other_index = tri->getNodeIndex(k);
                if(other_index!=node->index){

                    if(!node->isConnected(other_index)){
                        Connection* con = new Connection(node->index, other_index);
                        node->addConnection(con);
                        Node* other_node = nodes[other_index];
                        other_node->addConnection(con);
                        connections.push_back(con);
                    }


                }
            }
        }

    }

    
    for(int i = 0; i<connections.size();i++){
        Connection* con = connections[i];
        for(int j = 0; j<2; j++){
            indices[2*i + j + index_count ] = con->getIndex(j);
        }
    }

    printf("%d connections\n\n",(int)connections.size());
    
    for(int i = 0; i<(int)connections.size(); i++){

        Connection* con = connections[i];
        con->setPositions(node_data);

    }
    
    Vnot = 2*calculateVolume();
    printf("Volume of: %2.4f\n",Vnot);
    
    draw_connections=false;
    working=false;
    perturb_request=false;
}

CellObject::CellObject(const std::string &meshfile){
    BinaryMeshReader* reader = new BinaryMeshReader(meshfile);
    BinaryMesh mesh = reader->getMesh(0);
    
    int count = mesh.position_count;
    
    //one entry per coordinate
    int posEntries = 3*count;
    int colEntries = 4*count;
    int normalEntries = 3*count;
    int nodeEntries = posEntries + colEntries + normalEntries;
    int colorOffset = posEntries;
    
    node_data = new float[nodeEntries];
    
    for(t_index i = 0; i<count; i++){
        Node* n = new Node();
        nodes.push_back(n);
        n->index = i;
        double* old = &mesh.positions[3*i];
        for(int j = 0; j<3; j++){
            //positions
            node_data[i*3 + j] = (float)old[j];
            
            //colors
            node_data[colorOffset + i*4 + j] = 1.0f;
            
            //normals
            node_data[posEntries + colEntries + i*3 + j] = 0;
            
        }
        node_data[colorOffset + i*4 + 3] = 1.0f;
        
        n->setPosition(&node_data[i*3]);
        n->setNormal(&node_data[colorOffset + colEntries + i*3]);
        n->color = &node_data[colorOffset + i*4];
    }
    
    //create index array and associate triangles to node data.
    
    int triangle_count = mesh.triangle_count;
    int connection_count = mesh.connection_count;
    
    index_count = 3*triangle_count;
    int con_index_count = 2*connection_count;
    
    indices = new t_index[index_count + con_index_count];
    node_count = count;
    
    for(int i = 0; i<triangle_count; i++){
        
        Triangle* triangle = new Triangle();
        triangles.push_back(triangle);
        int* indexes = &mesh.triangle_indicies[3*i];
        for(int j = 0; j<3; j++){
            t_index dex = indexes[j];
            triangle->setNodeIndex(j,dex);
            
            indices[i*3 + j] = dex;
            Node* n = nodes[dex];
            n->addTriangle(triangle);
        }
        
        triangle->setPoints(node_data);
        triangle->update();
    
    }
    
    
    t_index* con_dexes = &indices[index_count];
    
    for(int i = 0; i<connection_count; i++){
        int a = mesh.connection_indicies[2*i];
        int b = mesh.connection_indicies[2*i + 1];
        con_dexes[2*i] = a;
        con_dexes[2*i+1] = b;
        Connection* con = new Connection(a,b);
        nodes[a]->addConnection(con);
        nodes[b]->addConnection(con);
        connections.push_back(con);
        con->setPositions(node_data);
    }
    
    for(int i = 0; i<connections.size();i++){
        Connection* con = connections[i];
        for(int j = 0; j<2; j++){
            indices[2*i + j + index_count ] = con->getIndex(j);
        }
    }
    
    Vnot = 2*calculateVolume();
    printf("Volume of: %2.4f\n",Vnot);
    draw_connections=false;
    working=false;
    perturb_request=false;
    
    delete reader;
}

void CellObject::prepareBuffers(GLuint &theProgram){
    proggy = theProgram;
    glUseProgram(theProgram);

    size_t colorDataOffset = sizeof(float) * 3 * node_count;
    size_t colorDataSize = sizeof(float)*4*node_count;
    size_t normalDataOffset = colorDataOffset + colorDataSize;
    size_t normalDataSize = sizeof(float) * 3 * node_count;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, colorDataOffset + colorDataSize + normalDataSize, node_data, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(t_index)*index_count*2, indices, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    glGenVertexArrays(1, &vaoObject);
    glBindVertexArray(vaoObject);
    
    GLuint positionAttribute = glGetAttribLocation(theProgram, "position");
    GLuint colorAttribute = glGetAttribLocation(theProgram, "inputColor");
    GLuint normalAttribute = glGetAttribLocation(theProgram, "normal");
    printf("pos %d, col %d, norm %d \n", positionAttribute, colorAttribute, normalAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(colorAttribute);
    glEnableVertexAttribArray(normalAttribute);
    
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
    glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)normalDataOffset);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    
    glBindVertexArray(0);
    glUseProgram(0);
    
}

void CellObject::draw(GLuint &theProgram){
    
    glUseProgram(theProgram);
    
    glBindVertexArray(vaoObject);
    GLuint colorMode = glGetUniformLocation(theProgram, "colorMode");
    glUniform1i(colorMode,1);
    glDrawElements(GL_TRIANGLES, (int)index_count, GL_UNSIGNED_INT, 0);
    GetError();
    if(draw_connections==0){
        glUniform1i(colorMode,0);
        glDrawElements(GL_LINES, (int)index_count, GL_UNSIGNED_INT, (void*)(sizeof(t_index)*index_count));
        GetError();
    }
    glBindVertexArray(0);
    
    glUseProgram(0);
    
}
void CellObject::toggleDrawConnections(){
    
    draw_connections=!draw_connections;
    
}
/**
 * @brief CellObject::calculateVolume calculates volume of the current postions by
 *           dotting each triangles normal with the x position. The x-position is approximated
 *           by the center x-value of the triangles.
 * @return volume estimate.
 */
float CellObject::calculateVolume(){
    float sum = 0.0;
    Triangle* t;
    for(size_t i = 0; i<triangles.size(); i++){
        t = triangles[i];
        sum += t->normal[0]*(t->points[0][0] + t->points[1][0] + t->points[2][0]);
    }
    return sum/6.0;
}

void CellObject::update(){
    working=true;
    for(int j = 0; j<SUB_SAMPLE; j++){
        for(int i = 0; i<(int)triangles.size(); i++){
            triangles[i]->update();
        }
        
        float volume = calculateVolume();
        //printf("volume: %2.4f",volume);

        for(int i = 0; i<(int)connections.size(); i++){
            Connection* c = connections[i];
            c->update();
            c->concentration = (nodes[c->getIndex(0)]->concentration + nodes[c->getIndex(1)]->concentration)*0.5;
        }
        for(int i = 0; i<(int)nodes.size(); i++){
            nodes[i]->update(Vnot - volume);
        }
        if(perturb_request){
            perturb();
        }
    }
    working=false;
    
}

void CellObject::updateBuffers(){
    glUseProgram(proggy);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    //glBufferSubData​(enum target, intptr offset, sizeiptr size, const void *data)
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*10*node_count, node_data);
    
    GetError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

float contractile_ring_concentration = 1.0;
float max_concentration = 100.0;
void CellObject::constrict(){
    contractile_ring_concentration = contractile_ring_concentration*0.9;
    printf("contracting: %f\n", max_concentration - (max_concentration-1)*contractile_ring_concentration);
    for(int i = 0 ; i<stationaries.size();i++){
        stationaries[i]->setConcentration(
            max_concentration - (max_concentration-1)*contractile_ring_concentration
                                        );
        for(int k = 0; k<connections.size(); k++){
            Connection* con = connections[k];
            if(con->hasIndex(stationaries[i]->index)){
                for(int j = i+1; j<stationaries.size(); j++){
                    if(con->hasIndex(stationaries[j]->index)){
                        con->lnot = 0;
                    }
                }

            }
        }
    }
}

void CellObject::perturb(){
    printf("perturbed\n");
    for(int i = 0; i<nodes.size(); i++){
        nodes[i]->setConcentration(0);
    }
    perturb_request=false;

}

void CellObject::requestPerturbation(){
    perturb_request=true;
}

bool CellObject::isWorking(){
    
    return working;
    
}
