#include "Model.h"
#include <stdlib.h>
#include <stdio.h>

Model::~Model()
{
    //dtor
}

Model::Model() {}

void Model::init(char* vertexShader, char* fragmentShader, char* textureFile ){
    cout<<vertexShader<<" "<<fragmentShader<<" "<<textureFile<<endl;
    shaderProgram = new ShaderProgram("vshader.glsl",NULL,"fshader.glsl");
    cout<<vertexShader<<" "<<fragmentShader<<" "<<textureFile<<endl;
    tex0=readTexture(textureFile);
    prepareObject();
}

GLuint Model::makeBuffer(void *data, int vertexCount, int vertexSize) {
    GLuint handle;

    glGenBuffers(1,&handle);//Generate handle for VBO buffer
    glBindBuffer(GL_ARRAY_BUFFER,handle);  //Active the handle
    glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Copy the array to VBO

    return handle;
}

void Model::drawObject(glm::mat4 mP, glm::mat4 mV, glm::mat4 mM) {
    shaderProgram->use();

    glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(mP));
    glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(mV));
    glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(mM));
    glUniform1i(shaderProgram->getUniformLocation("textureMap0"),0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex0);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    glBindVertexArray(0);
}

void Model::assignVBOtoAttribute(ShaderProgram *shaderProgram,const char* attributeName, GLuint bufVBO, int vertexSize) {
    GLuint location=shaderProgram->getAttribLocation(attributeName); //Get slot number for the attribute
    glBindBuffer(GL_ARRAY_BUFFER,bufVBO);  //Activate VBO handle
    glEnableVertexAttribArray(location); //Turn on using of an attribute of a number passed as an argument
    glVertexAttribPointer(location,vertexSize,GL_FLOAT, GL_FALSE, 0, NULL); //Data for the slot should be taken from the current VBO buffer
}

void Model::prepareObject() {
    //Build VBO buffers with object data
    bufVertices=makeBuffer(verticesArray, vertexCount, sizeof(float)*4); //VBO with vertex coordinates
    bufColors=makeBuffer(colorsArray, vertexCount, sizeof(float)*4);//VBO with vertes colors
    bufNormals=makeBuffer(normalsArray, vertexCount, sizeof(float)*4);//VBO with vertex normals
    bufTexCoords=makeBuffer(texCoordsArray, vertexCount, sizeof(float)*2);

    //Create VAO which associates VBO with attributes in shading program
    glGenVertexArrays(1,&vao); //Generate VAO handle and store it in the global variable

    glBindVertexArray(vao); //Activate newly created VAO

    assignVBOtoAttribute(shaderProgram,"vertex",bufVertices,4); //"vertex" refers to the declaration "in vec4 vertex;" in vertex shader
    assignVBOtoAttribute(shaderProgram,"color",bufColors,4); //"color" refers to the declaration "in vec4 color;" in vertex shader
    assignVBOtoAttribute(shaderProgram,"normal",bufNormals,4); //"normal" refers to the declaration "in vec4 normal;" w vertex shader
    assignVBOtoAttribute(shaderProgram, "texCoord0",bufTexCoords, 2);

    glBindVertexArray(0); //Deactivate VAO
}

GLuint Model::readTexture(char* filename) {
    GLuint tex;
    glActiveTexture(GL_TEXTURE0);

    //Wczytanie do pamiêci komputera
    std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
    unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
    //Wczytaj obrazek
    unsigned error = lodepng::decode(image, width, height, filename);

    //Import do pamiêci karty graficznej
    glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
    glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
    //Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return tex;
}

bool Model::loadFromOBJFile(const string &path) {
    vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    vector<glm::vec3> temp_vertices;
    vector<glm::vec2> temp_uvs;
    vector<glm::vec3> temp_normals;
    bool loadingUvs = false;
    FILE *file = fopen(path.c_str(), "r");
    if (file == NULL) {
        return false;
    }
    int res = 65;
    while (res != EOF) {
        char lineHeader[128] = "";
        res = fscanf(file, "%s", lineHeader);
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        } else if (strcmp(lineHeader, "vt") == 0) {
            loadingUvs = true;
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            //std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            if(loadingUvs) {
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                     &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2],
                                     &normalIndex[2]);
                if (matches != 9) {
                    printf("File cannot be imported. Make sure the object was exported as triangles.\n");
                    return false;
                }
            } else {
                int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
                                     &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                if (matches != 6) {
                    printf("File cannot be imported. Make sure the object was exported as triangles.\n");
                    return false;
                }
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            if(loadingUvs) {
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
            }
        }
    }
    for (unsigned int vertexIndex : vertexIndices) {
        glm::vec4 vertex = glm::vec4(temp_vertices[vertexIndex - 1].x,temp_vertices[vertexIndex - 1].y,temp_vertices[vertexIndex - 1].z, 1.f);
        vertices.push_back(vertex);
    }
    for (unsigned int normalIndex : normalIndices) {
        glm::vec4 normal = glm::vec4(temp_normals[normalIndex - 1].x,temp_normals[normalIndex - 1].y,temp_normals[normalIndex - 1].z, 0.f);
        normals.push_back(normal);
    }
    if(loadingUvs) {
        for (unsigned int uvIndex : uvIndices) {
            glm::vec2 uv = glm::vec2(temp_uvs[uvIndex - 1].x, temp_uvs[uvIndex - 1].y);
            uvs.push_back(uv);
        }
    }
    vertexCount = vertices.size();
    verticesArray = &vertices.data()->x;
    colorsArray = &vertices.data()->x;
    normalsArray = &normals.data()->x;
    texCoordsArray = &uvs.data()->x;

    return true;
}

void Model::freeOpenGLProgram() {
    delete shaderProgram; //Delete shader program

    glDeleteVertexArrays(1,&vao); //Delete VAO
    //Delete VBOs
    glDeleteBuffers(1,&bufVertices);
    glDeleteBuffers(1,&bufColors);
    glDeleteBuffers(1,&bufNormals);

    glDeleteTextures(1, &tex0);
    //glDeleteTextures(1, &tex1);

}


vector<glm::vec4> &Model::getVertices() {
    return vertices;
}

vector<glm::vec2> &Model::getUvs() {
    return uvs;
}

vector<glm::vec4> &Model::getNormals() {
    return normals;
}


