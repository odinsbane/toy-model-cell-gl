//version defined in program

//position ordinates are located in gl_FragCoord
//and are in 'window coordinates'
out vec4 outputColor;
uniform sampler2D texSampler;

in vec2 texCoords;
smooth in vec4 meshColor;
void main() {
    //outputColor = meshColor;
    outputColor =  texture(texSampler, texCoords);
}


