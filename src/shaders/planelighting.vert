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

vec4 camera(vec4 raw_position);

uniform vec3 lightPos;
uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;



smooth out vec4 meshColor;
void main() {
        vec3 normCameraSpace = normalize(normalModelToCameraMatrix*normal);

        vec4 camPosition = orientationMatrix*(position+vec4(cam_offset,0));
        vec3 dirToLight = normalize(lightPos - vec3(camPosition));

        float cosAngIncidence = dot(normCameraSpace, dirToLight);



        gl_Position = perspectiveMatrix*camPosition;
   
   if(useMeshColor){
      meshColor = inputColor;
      //meshColor = (inputColor * lightIntensity * cosAngIncidence) + (inputColor * ambientIntensity);
      meshColor =  inputColor;
   } else{
      float f = 0.001;
      meshColor = vec4(f, f, f, f)*vec4(normal,1);
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
    

