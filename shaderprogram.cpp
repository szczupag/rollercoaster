//
//  shaderprogram.cpp
//  
//
//  Created by Maciej Mościcki on 20.05.2018.
//

#include "shaderprogram.hpp"

char* ShaderProgram::readFile(const char* fileName) {
    int filesize;
    FILE *plik;
    char* result;
    
    plik=fopen(fileName,"rb");
    fseek(plik,0,SEEK_END);
    filesize=ftell(plik);
    fseek(plik,0,SEEK_SET);
    result=new char[filesize+1];
    fread(result,1,filesize,plik);
    result[filesize]=0;
    fclose(plik);
    
    return result;
}

//The method reads a shader code, compiles it and returns a corresponding handle
GLuint ShaderProgram::loadShader(GLenum shaderType,const char* fileName) {
    //Create a shader handle
    GLuint shader=glCreateShader(shaderType);//shaderType to GL_VERTEX_SHADER, GL_GEOMETRY_SHADER lub GL_FRAGMENT_SHADER
    //Read a shader source file into an array of chars
    const GLchar* shaderSource=readFile(fileName);
    //Associate source code with the shader handle
    glShaderSource(shader,1,&shaderSource,NULL);
    //Compile source code
    glCompileShader(shader);
    //Delete source code from memory (it is no longer needed)
    delete []shaderSource;
    
    //Download a compilation error log and display it
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);
    
    if (infologLength > 1) {
        infoLog = new char[infologLength];
        glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        delete []infoLog;
    }
    
    //Return shader handle
    return shader;
}

ShaderProgram::ShaderProgram(const char* vertexShaderFile,const char* geometryShaderFile,const char* fragmentShaderFile) {
    //Load vertex shader
    printf("Loading vertex shader...\n");
    vertexShader=loadShader(GL_VERTEX_SHADER,vertexShaderFile);
    
    //Load geometry shader
    if (geometryShaderFile!=NULL) {
        printf("Loading geometry shader...\n");
        geometryShader=loadShader(GL_GEOMETRY_SHADER,geometryShaderFile);
    } else {
        geometryShader=0;
    }
    
    //Load fragment shader
    printf("Loading fragment shader...\n");
    fragmentShader=loadShader(GL_FRAGMENT_SHADER,fragmentShaderFile);
    
    //Generate shader program handle
    shaderProgram=glCreateProgram();
    
    //Attach shaders and link shader program
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    if (geometryShaderFile!=NULL) glAttachShader(shaderProgram,geometryShader);
    glLinkProgram(shaderProgram);
    
    //Download an error log and display it
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH,&infologLength);
    
    if (infologLength > 1)
    {
        infoLog = new char[infologLength];
        glGetProgramInfoLog(shaderProgram, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        delete []infoLog;
    }
    
    printf("Shader program created \n");
}

ShaderProgram::~ShaderProgram() {
    //Detach shaders from program
    glDetachShader(shaderProgram, vertexShader);
    if (geometryShader!=0) glDetachShader(shaderProgram, geometryShader);
    glDetachShader(shaderProgram, fragmentShader);
    
    //Delete shaders
    glDeleteShader(vertexShader);
    if (geometryShader!=0) glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
    
    //Delete program
    glDeleteProgram(shaderProgram);
}


//Make the shader program active
void ShaderProgram::use() {
    glUseProgram(shaderProgram);
}

//Get the slot number corresponding to the uniform variableName
GLuint ShaderProgram::getUniformLocation(const char* variableName) {
    return glGetUniformLocation(shaderProgram,variableName);
}

//Get the slot number corresponding to the attribute variableName
GLuint ShaderProgram::getAttribLocation(const char* variableName) {
    return glGetAttribLocation(shaderProgram,variableName);
}

