#version 330

out vec4 pixelColor; //Fragment shader output variable. Returns the (almost) final pixel color.

//Interpolated variables
in vec4 l; //light vector in eye space
in vec4 n; //normal vector in eye space
in vec4 v; //view vector in eye space
in vec2 iTexCoord0; //texturing coordinates

//Variables representing texturing units
uniform sampler2D textureMap0;
//uniform sampler2D textureMap1;

void main(void) {
    //vec4 ka=vec4(0,0,0,0); //Object color in the ambient light
    vec4 kd=texture(textureMap0,iTexCoord0); //Object color in the diffused light
    //vec4 ks=texture(textureMap0,iTexCoord0); //Object color in the specular light

    vec4 la=vec4(0.5,0.2,0,0); //Ambient light color
    vec4 ld=vec4(1,1,1,1); //Diffuse light color
    vec4 ls=vec4(1,1,1,0); //Specular light color

    vec4 ml=normalize(l);
    vec4 mn=normalize(n);
    vec4 mv=normalize(v);
    vec4 mr=reflect(-ml,mn);

    float nl=max(dot(n,l),0); //cosine of the angle between normal and light vectors
    float rv=pow(max(dot(mr,mv),0),10); //cosine of the angle between reflected and view vectors to the power of 10

    pixelColor=la+kd*ld*vec4(nl,nl,nl,1)+ls*rv; //Phong shading model
}
