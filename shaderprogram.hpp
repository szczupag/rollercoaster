//
//  shaderprogram.hpp
//  
//
//  Created by Maciej Mościcki on 20.05.2018.
//

#ifndef shaderprogram_hpp
#define shaderprogram_hpp

#include "GL/glew.h"
#include "stdio.h"

class ShaderProgram {
private:
    GLuint shaderProgram; //Shader program handle
    GLuint vertexShader; //Vertex shader handle
    GLuint geometryShader; //Geometry shader handle
    GLuint fragmentShader; //Fragment shader handle
    char* readFile(const char* fileName); //File reading method
    GLuint loadShader(GLenum shaderType,const char* fileName); //Method reads shader source file, compiles it and returns the corresponding handle
public:
    ShaderProgram(const char* vertexShaderFile,const char* geometryShaderFile,const char* fragmentShaderFile);
    ~ShaderProgram();
    void use(); //Turns on the shader program
    GLuint getUniformLocation(const char* variableName); //Returns the slot number corresponding to the uniform variableName
    GLuint getAttribLocation(const char* variableName); //Returns the slot number corresponding to the attribute variableName
};



#endif
