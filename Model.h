#ifndef MODEL_H
#define MODEL_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <vector>
#include <string>
#include "shaderprogram.hpp"
#include "lodepng.h"
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
using namespace std;

class Model {
private:
    vector<glm::vec4> vertices;
    vector<glm::vec2> uvs;
    vector<glm::vec4> normals;
    GLuint tex0;
    GLuint vao;
    GLuint bufVertices; //handle for VBO buffer which stores vertex coordinates
    GLuint bufColors;  //handle for VBO buffer which stores vertex colors
    GLuint bufNormals; //handle for VBO buffer which stores vertex normals
    GLuint bufTexCoords;
    ShaderProgram *shaderProgram;
    float* verticesArray;
    float* colorsArray;
    float* normalsArray;
    float* texCoordsArray;
    int vertexCount;

public:
    Model();

    ~Model();

    void freeOpenGLProgram();

    void init(char* vertexShader, char* fragmentShader, char* textureFile);

    void drawObject(glm::mat4 mP, glm::mat4 mV, glm::mat4 mM);

    GLuint readTexture(char* filename);

    GLuint makeBuffer(void *data, int vertexCount, int vertexSize);

    void assignVBOtoAttribute(ShaderProgram *shaderProgram, const char* attributeName, GLuint bufVBO, int vertexSize);

    void prepareObject();

    bool loadFromOBJFile(const string &path);

    vector<glm::vec4> & getVertices();

    vector<glm::vec2> & getUvs();

    vector<glm::vec4> & getNormals();
};

#endif // MODEL_H
