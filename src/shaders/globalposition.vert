//version defined in program.

//input 
in vec4 position;
in vec4 inputColor;
in vec3 normal;

uniform vec3 cam_offset;
uniform mat4 perspectiveMatrix;
uniform mat4 orientationMatrix;
uniform mat3 normalModelToCameraMatrix;

uniform bool useMeshColor;

//vec4 camera(vec4 raw_position);

uniform vec3 lightPos;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform int colorMode;


smooth out vec4 meshColor;
smooth out vec3 planePosition;
void main() {
        vec4 camPosition = orientationMatrix*(position+vec4(cam_offset,0));
        //dirToLight = vec3(0,0,1);
    
        gl_Position = perspectiveMatrix*camPosition;
        //gl_Position.z = camPosition.z;
        planePosition = vec3(0,0,0);
    switch (colorMode){
        case 0:
            float f = 0.001;
            meshColor = vec4(f, f, f, f)*vec4(normal,1);
            break;
        case 1:
            vec4 color = inputColor;
            vec3 nn = normalize(normal);
            vec3 dirToLight = normalize(lightPos - vec3(position));
            float cosAngIncidence = dot(nn, dirToLight)*0.5 + 0.5;
            
            meshColor = (color* lightIntensity*cosAngIncidence ) + color * ambientIntensity;
            break;
        case 2:
            meshColor = inputColor;
            break;
        case 3:
            planePosition = vec3(position.x, position.y, 1);
            meshColor = vec4(1.0,1.0,1.0,1);
            break;
    }
   

}



/*
*   Camera is looking in the -1 direction
*/
vec4 camera(vec4 raw_position){
    vec4 cameraPos;
    
    //transforms into camera space where +z is away from the camera.
    cameraPos.xyz = raw_position.xyz + cam_offset.xyz;
    cameraPos.w = raw_position.w;
    
    vec4 rot;
    rot = orientationMatrix*cameraPos;
    
    vec4 clipPos;
    
    clipPos = perspectiveMatrix*rot;
    return clipPos;
}
    

