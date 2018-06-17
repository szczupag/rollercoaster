#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


//Attributes
in vec4 vertex; //vertex coordinates in model space
in vec4 color;  //vertex color
in vec4 normal; //vertex normal in model space
in vec2 texCoord0; //texturing coordinates

//Interpolated variables
out vec4 l; //light vector in eye space
out vec4 n; //normal vector in eye space
out vec4 v; //view vector in eye space
out vec2 iTexCoord0; //texturing coordinates

void main(void) {

    vec4 lp=vec4(0,100,-6,1); //Light position in the world space

    l=normalize(V*lp-V*M*vertex); //Light vector, eye space
    n=normalize(V*M*normal); //Normal vector, eye space
    v=normalize(vec4(0,0,0,1)-V*M*vertex); //View wektor, eye space

    iTexCoord0=texCoord0; //Interpolate texturing coordinates

	gl_Position=P*V*M*vertex;
}
