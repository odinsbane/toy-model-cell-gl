#include "CellCam.h"
#include "error.h"
#include "stdio.h"
CellCam::CellCam(GLuint &program){
    theProgram = program;
    aspect = 1.0f;
    setPerspectiveMatrix();
    
    xx = new float[4];
    yy = new float[4];
    zz = new float[4];

    r = 1;
    for(int i = 0; i<4; i++){
        quarternion[i] = 0.0;
        xx[i] = 0;
        yy[i] = 0;
        zz[i] = 0;
    }
    quarternion[3] = 1.0;

    updatePosition();
    printf("initializing lights\n");
    light_position = new float[3]{-0.5,-0.5,1.0};
    light_intensity = new float[4]{0.9,0.9,0.9,1};
    ambient_light = new float[4]{0.1, 0.1, 0.1, 1.0};
    GetError();
    updateLights();
    
}

void CellCam::setPosition(){
    
    xx[0] = r;
    yy[1] = r;
    zz[2] = r;

    x = -Framework::multiplyMatrixVector4(orientationMatrix,xx)[2];
    y = -Framework::multiplyMatrixVector4(orientationMatrix,yy)[2];
    z = -Framework::multiplyMatrixVector4(orientationMatrix,zz)[2];

    GLuint cam_offset = glGetUniformLocation(theProgram, "cam_offset");
    glUseProgram(theProgram);
    glUniform3f(cam_offset, x, y, z);
    glUseProgram(0);    
       
    }
    
void CellCam::zoom(double dr){
    
    r += dr;
    
    updatePosition();
    
    
}

void CellCam::updatePosition(){
    setRotation();
    setPosition();
}


void CellCam::setPerspectiveMatrix(){    
    float fFrustumScale = 1.0f; 
    float fzNear = 0.5f;
    
    float fzFar = 10.0f;
    
    //column
    perspectiveMatrix[0] = fFrustumScale/aspect;
    perspectiveMatrix[1] = 0;
    perspectiveMatrix[2] = 0;
    perspectiveMatrix[3] = 0;
    
    //column
    perspectiveMatrix[4] = 0;
    perspectiveMatrix[5] = fFrustumScale;
    perspectiveMatrix[6] = 0;
    perspectiveMatrix[7] = 0;
    
    //column
    perspectiveMatrix[8] = 0;
    perspectiveMatrix[9] = 0;
    perspectiveMatrix[10] = (fzFar + fzNear) / (fzNear - fzFar);
    perspectiveMatrix[11] = -1.0f;
    
    //column
    perspectiveMatrix[12] = 0;
    perspectiveMatrix[13] = 0;
    perspectiveMatrix[14] = 2.f*fzFar*fzNear / (fzNear - fzFar);
    perspectiveMatrix[15] = 0;
    
    
    
    GLuint perspectiveMatrixUnif = glGetUniformLocation(theProgram, "perspectiveMatrix");

    glUseProgram(theProgram);
    glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, perspectiveMatrix);
    glUseProgram(0);
    
}


void CellCam::rotate(float dtheta, float dphi){
    if(dtheta==0){
        float axis[4];
        float sdp = sin(dphi/2.0);
        axis[0] = sdp*xx[0];
        axis[1] = sdp*xx[1];
        axis[2] = sdp*xx[2];
        axis[3] = cos(dphi/2.0);
        rotate(axis);
    } else{
        
        float axis[4];
        float sdp = sin(dtheta/2.0);
        axis[0] = sdp*yy[0];
        axis[1] = sdp*yy[1];
        axis[2] = sdp*yy[2];
        axis[3] = cos(dtheta/2.0);
        rotate(axis);
        
    }
    updatePosition();
}

void CellCam::rotate(float* quat){
    float next[4];
    float sum = 0;
    next[0] = quarternion[3]*quat[0] + quarternion[0]*quat[3] + quarternion[1]*quat[2] - quarternion[2]*quat[1];
    sum += next[0]*next[0];
    next[1] = quarternion[3]*quat[1] + quarternion[1]*quat[3] + quarternion[2]*quat[0] - quarternion[0]*quat[2];
    sum += next[1]*next[1];
    next[2] = quarternion[3]*quat[2] + quarternion[2]*quat[3] + quarternion[0]*quat[1] - quarternion[1]*quat[0];
    sum += next[2]*next[2];
    next[3] = quarternion[3]*quat[3] - quarternion[0]*quat[0] - quarternion[1]*quat[1] - quarternion[2]*quat[2];
    sum += next[3]*next[3];
    sum = sqrt(sum);
    for(int i = 0; i<4; i++){
        quarternion[i] = next[i]/sum;
    }
    
}

void CellCam::setRotation(){
    orientationMatrix[0] = 1 - 2*quarternion[1]*quarternion[1]-2*quarternion[2]*quarternion[2];
    orientationMatrix[1] = 2*quarternion[0]*quarternion[1] + 2*quarternion[3]*quarternion[2];
    orientationMatrix[2] = 2*quarternion[0]*quarternion[2] - 2*quarternion[3]*quarternion[1];
    orientationMatrix[3] = 0;
    
    orientationMatrix[4] = 2*quarternion[0]*quarternion[1] - 2*quarternion[3]*quarternion[2];
    orientationMatrix[5] = 1 - 2*quarternion[0]*quarternion[0]-2*quarternion[2]*quarternion[2];
    orientationMatrix[6] = 2*quarternion[1]*quarternion[2] + 2*quarternion[3]*quarternion[0];
    orientationMatrix[7] = 0;
    
    orientationMatrix[8] = 2*quarternion[0]*quarternion[2] + 2*quarternion[3]*quarternion[1];
    orientationMatrix[9] = 2*quarternion[1]*quarternion[2] - 2*quarternion[3]*quarternion[0];
    orientationMatrix[10] = 1 - 2*quarternion[0]*quarternion[0]-2*quarternion[1]*quarternion[1];
    orientationMatrix[11] = 0;
    
    orientationMatrix[12] = 0;
    orientationMatrix[13] = 0;
    orientationMatrix[14] = 0;
    orientationMatrix[15] = 1;
    for(int i = 0; i<3; i++){
        for(int j = 0; j<3; j++){
            normalMatrix[i + 3*j] = orientationMatrix[i + 4*j];
        }
    }
    GLuint orientationMatrixUnif = glGetUniformLocation(theProgram, "orientationMatrix");
    GLuint normalMatrixUniform = glGetUniformLocation(theProgram, "normalModelToCameraMatrix");
    
    glUseProgram(theProgram);
    
    glUniformMatrix4fv(orientationMatrixUnif, 1, GL_FALSE, orientationMatrix);
    glUniformMatrix3fv(normalMatrixUniform, 1, GL_FALSE, normalMatrix);
    glUseProgram(0);
    
}

/*
 * Changes the display aspect ratio, such that more of the image is shown in a 
 * particular direction.
 *  
 */
void CellCam::resizeWindow(float w, float h){
    aspect = w/h;
    
}

void CellCam::updateLights(){
    float cast[2]{0,0};
    cast[0] = -light_position[0]*0.5/light_position[2];
    cast[1] = -light_position[1]*0.5/light_position[2];
    
    GLuint lightPositionUniform = glGetUniformLocation(theProgram, "lightPos");
    GLuint lightIntensityUniform = glGetUniformLocation(theProgram, "lightIntensity");
    GLuint ambientIntensityUniform = glGetUniformLocation(theProgram, "ambientIntensity");
    GLuint radiusUniform = glGetUniformLocation(theProgram, "RADIUS");
    GLuint castUniform = glGetUniformLocation(theProgram, "CAST_LOCATION");
    glUseProgram(theProgram);
    
    printf("ru: %d; cl: %d\n", radiusUniform, castUniform);
    
    glUniform1f(radiusUniform, 0.5);
    glUniform2fv(castUniform, 1, cast);
    
    
    
    glUniform3fv(lightPositionUniform, 1, light_position);
    GetError();
    glUniform4fv(lightIntensityUniform, 1, light_intensity);
    
    glUniform4fv(ambientIntensityUniform, 1, ambient_light);
    printf("pos: %d int: %d amb: %d\n", lightPositionUniform, lightIntensityUniform, ambientIntensityUniform);
    
    glUseProgram(0);
    
    
}
