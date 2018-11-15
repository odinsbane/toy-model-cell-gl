//version defined in program.

//input 
in vec2 position;



out vec2 texCoords;
void main() {
        gl_Position = vec4(position.x, position.y, 1.0, 1);
        texCoords=vec2(14*position.x, 14*position.y);
}

