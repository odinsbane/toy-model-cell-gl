//version defined in program

//position ordinates are located in gl_FragCoord
//and are in 'window coordinates'
out vec4 outputColor;

uniform float RADIUS=0.5;
uniform vec2 CAST_LOCATION;

const float B = -0.6 - 0.3*sqrt(2);
const float A = 0.2 - B;

smooth in vec4 meshColor;
smooth in vec3 planePosition;
void main() {
    if(planePosition.z!=0){
        float dx = planePosition.x - CAST_LOCATION.x;
        float dy = planePosition.y - CAST_LOCATION.y;
        
        float d = dx*dx + dy*dy;
        
        if(d<2*RADIUS*RADIUS){
            d = sqrt(d)/RADIUS;
            //d = A*d*d + B*d;
            d = d*d*d*d*0.25;
            outputColor=vec4(d,d,d,1);
        } else{
            outputColor=vec4(1,1,1,1);
        }
    } else{
        outputColor = meshColor;
    }
}


