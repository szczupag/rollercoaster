//
//  main.cpp
//  rollercoaster
//
//  Created by Maciej Mościcki on 20.05.2018.
//  Copyright © 2018 Maciej Mościcki. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "shaderprogram.hpp"
#include "lodepng.h"
#include "cube.hpp"
#include "constants.h"
#include "Object.hpp"
using namespace glm;

float speed_x = 1.14; // [radians/s]
float speed_y = 1.14; // [radians/s]

float aspect=1; //Ratio of width to height

//Shader program object
ShaderProgram *shaderProgram;

//VAO and VBO handles
GLuint vao;
GLuint bufVertices; //handle for VBO buffer which stores vertex coordinates
GLuint bufColors;  //handle for VBO buffer which stores vertex colors
GLuint bufNormals; //handle for VBO buffer which stores vertex normals
GLuint bufTexCoords;

GLuint tex0;
//GLuint tex1;
//Cube
//float* vertices=Models::CubeInternal::vertices;
//float* colors=Models::CubeInternal::colors;
//float* normals=Models::CubeInternal::normals;
//int vertexCount=Models::CubeInternal::vertexCount;
//float* texCoords=Models::CubeInternal::texCoords;

float* vertices;
float* colors;
float* normals;
float* texCoords;
int vertexCount;

//Teapot
/*float* vertices=Models::TeapotInternal::vertices;
 float* colors=Models::TeapotInternal::colors;
 float* normals=Models::TeapotInternal::normals;
 int vertexCount=Models::TeapotInternal::vertexCount;*/


//Error handling procedure
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

//Key event processing procedure
void key_callback(GLFWwindow* window, int key,
                  int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) speed_y = -3.14;
        if (key == GLFW_KEY_RIGHT) speed_y = 3.14;
        if (key == GLFW_KEY_UP) speed_x = -3.14;
        if (key == GLFW_KEY_DOWN) speed_x = 3.14;
    }


    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT) speed_y = 0;
        if (key == GLFW_KEY_RIGHT) speed_y = 0;
        if (key == GLFW_KEY_UP) speed_x = 0;
        if (key == GLFW_KEY_DOWN) speed_x = 0;
    }
}

//Window resize event processing procedure
void windowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //Window coordinates
    if (height!=0) {
        aspect=(float)width/(float)height; //Compute window size aspect ratio
    } else {
        aspect=1;
    }
}


//Creates a VBO buffer from an array
GLuint makeBuffer(void *data, int vertexCount, int vertexSize) {
    GLuint handle;

    glGenBuffers(1,&handle);//Generate handle for VBO buffer
    glBindBuffer(GL_ARRAY_BUFFER,handle);  //Active the handle
    glBufferData(GL_ARRAY_BUFFER, vertexCount*vertexSize, data, GL_STATIC_DRAW);//Copy the array to VBO

    return handle;
}

//Assigns VBO buffer handle to an attribute of a given name
void assignVBOtoAttribute(ShaderProgram *shaderProgram,const char* attributeName, GLuint bufVBO, int vertexSize) {
    GLuint location=shaderProgram->getAttribLocation(attributeName); //Get slot number for the attribute
    glBindBuffer(GL_ARRAY_BUFFER,bufVBO);  //Activate VBO handle
    glEnableVertexAttribArray(location); //Turn on using of an attribute of a number passed as an argument
    glVertexAttribPointer(location,vertexSize,GL_FLOAT, GL_FALSE, 0, NULL); //Data for the slot should be taken from the current VBO buffer
}


//Preparation for drawing of a single object
void prepareObject(ShaderProgram* shaderProgram) {
    //Build VBO buffers with object data
    bufVertices=makeBuffer(vertices, vertexCount, sizeof(float)*4); //VBO with vertex coordinates
    bufColors=makeBuffer(colors, vertexCount, sizeof(float)*4);//VBO with vertes colors
    bufNormals=makeBuffer(normals, vertexCount, sizeof(float)*4);//VBO with vertex normals
    bufTexCoords=makeBuffer(texCoords, vertexCount, sizeof(float)*2);

    //Create VAO which associates VBO with attributes in shading program
    glGenVertexArrays(1,&vao); //Generate VAO handle and store it in the global variable

    glBindVertexArray(vao); //Activate newly created VAO

    assignVBOtoAttribute(shaderProgram,"vertex",bufVertices,4); //"vertex" refers to the declaration "in vec4 vertex;" in vertex shader
    assignVBOtoAttribute(shaderProgram,"color",bufColors,4); //"color" refers to the declaration "in vec4 color;" in vertex shader
    assignVBOtoAttribute(shaderProgram,"normal",bufNormals,4); //"normal" refers to the declaration "in vec4 normal;" w vertex shader
    assignVBOtoAttribute(shaderProgram, "texCoord0", bufTexCoords, 2);

    glBindVertexArray(0); //Deactivate VAO
}
GLuint readTexture(char* filename) {
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

//Initialization procedure
void initOpenGLProgram(GLFWwindow* window) {
    //************Insert initialization code here************
    glClearColor(0.9, 0.5, 0.5, 1); //Clear the screen to black
    glEnable(GL_DEPTH_TEST); //Turn on Z-Buffer
    glfwSetKeyCallback(window, key_callback); //Register key event processing procedure
    glfwSetFramebufferSizeCallback(window,windowResize);

    shaderProgram=new ShaderProgram("vshader.glsl",NULL,"fshader.glsl"); //Read, compile and link the shader program
    tex0=readTexture("metal.png");

    prepareObject(shaderProgram);

}

//Freeing of resources
void freeOpenGLProgram() {
    delete shaderProgram; //Delete shader program

    glDeleteVertexArrays(1,&vao); //Delete VAO
    //Delete VBOs
    glDeleteBuffers(1,&bufVertices);
    glDeleteBuffers(1,&bufColors);
    glDeleteBuffers(1,&bufNormals);

    glDeleteTextures(1, &tex0);
    //glDeleteTextures(1, &tex1);

}

void drawObject(GLuint vao, ShaderProgram *shaderProgram, mat4 mP, mat4 mV, mat4 mM) {
    //Turn on the shading program that will be used for drawing.
    //While in this program it would be perfectly correct to execute this once in the initOpenGLProgram,
    //in most cases more than one shading program is used and hence, it should be switched between drawing of objects
    //while we render a single scene.
    shaderProgram->use();

    //Set uniform variables P,V and M in the vertex shader by assigning the appropriate matrices
    //In the lines below, expression:
    //  shaderProgram->getUniformLocation("P")
    //Retrieves the slot number corresponding to a uniform variable of a given name.
    //WARNING! "P" in the instruction above corresponds to the declaration "uniform mat4 P;" in the vertex shader,
    //while mP in glm::value_ptr(mP) corresponds to the argument "mat4 mP;" in THIS file.
    //The whole line below copies data from variable mP to the uniform variable P in the vertex shader. The rest of the instructions work similarly.
    glUniformMatrix4fv(shaderProgram->getUniformLocation("P"),1, false, glm::value_ptr(mP));
    glUniformMatrix4fv(shaderProgram->getUniformLocation("V"),1, false, glm::value_ptr(mV));
    glUniformMatrix4fv(shaderProgram->getUniformLocation("M"),1, false, glm::value_ptr(mM));
    glUniform1i(shaderProgram->getUniformLocation("textureMap0"),0);
    //glUniform1i(shaderProgram->getUniformLocation("textureMap1"),1);

    //Activation of VAO and therefore making all associations of VBOs and attributes current
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex0);

    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, tex1);

    glBindVertexArray(vao);

    //Drawing of an object
    glDrawArrays(GL_TRIANGLES,0,vertexCount);

    //Tidying up after ourselves (not needed if we use VAO for every object)
    glBindVertexArray(0);
}

//Procedure which draws the scene
void drawScene(GLFWwindow* window, float angle_x, float angle_y) {
    //************Place the drawing code here******************l

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

    glm::mat4 P = glm::perspective(50 * PI / 180,aspect, 1.0f, 50.0f); //Compute projection matrix

    glm::mat4 V = glm::lookAt( //Compute view matrix
                              glm::vec3(0.0f, 0.0f, -20.0f),
                              glm::vec3(0.0f, 0.0f, 0.0f),
                              glm::vec3(0.0f, 1.0f, 0.0f));


    //Compute model matrix
    glm::mat4 M = glm::mat4(1.0f);
    M = glm::translate(M, glm::vec3(cos(angle_x)*10.0f, 0.0f, 0.0f));
    M = glm::translate(M, glm::vec3(0.0f, sin(angle_y)*10.0f, 0.0f));
    M = glm::translate(M, glm::vec3(0.0f, 0.0f, sin(angle_y)*15.0f));
    M = glm::rotate(M, angle_x *0.9f, glm::vec3(1, 0, 0));
    M = glm::rotate(M, angle_y *0.5f, glm::vec3(0, 1, 0));


    //Draw object
    drawObject(vao,shaderProgram,P,V,M);

    //Swap front and back buffers
    glfwSwapBuffers(window);

}



int main(void)
{
    ModelO seat;
    if(!seat.loadFromOBJFile("seat1.obj")) {
        return 1;
    }
/*
    cout<<"wierzcholkow: "<<seat.getVertices().size()<<endl;

    int vertexCount1 = seat.getVertices().size();

    float *vertices1 = &seat.getVertices().data()->x;
    float *colors1 = &seat.getVertices().data()->x;
    float *normals1 = &seat.getNormals().data()->x;
    float *texCoords1 = &seat.getUvs().data()->x;
    texCoords = &seat.getUvs().data()->x;

    float * verticesTmp = new float[vertexCount1 * 4];
    float * normalsTmp = new float[vertexCount1 * 4];
    float * colorsTmp = new float[vertexCount1 * 4];

    int k = 0;
    for(int i = 0; i < vertexCount1*3; i+=3){
        verticesTmp[k] = vertices1[i];
        verticesTmp[k + 1] = vertices1[i + 1];
        verticesTmp[k + 2] = vertices1[i + 2];
        verticesTmp[k + 3] = 1.0f;

        normalsTmp[k] = normals1[i];
        normalsTmp[k + 1] = normals1[i + 1];
        normalsTmp[k + 2] = normals1[i + 2];
        normalsTmp[k + 3] = 1.0f;

        colorsTmp[k] = colors1[i];
        colorsTmp[k + 1] = colors1[i + 1];
        colorsTmp[k + 2] = colors1[i + 2];
        colorsTmp[k + 3] = 1.0f;

        k += 4;
    }

    cout<< "zakonczono "<<endl;
*/
    vertexCount = seat.getVertices().size();
    vertices = &seat.getVertices().data()->x;
    colors = &seat.getVertices().data()->x;
    normals = &seat.getNormals().data()->x;
    texCoords = &seat.getUvs().data()->x;


    GLFWwindow* window; //Pointer to window object

    glfwSetErrorCallback(error_callback);//Register error callback procedure


    if (!glfwInit()) { //Initialize GLFW procedure
        fprintf(stderr, "Can't initialize GLFW GLFW.\n");
        exit(EXIT_FAILURE);
    }


    window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Create 500x500 window with "OpenGL" as well as OpenGL context.

    if (!window) //If window could not be created, then end the program
    {
        fprintf(stderr, "Can't create window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window); //Since this moment, the window context is active and OpenGL commands will work with it.
    glfwSwapInterval(1); //Synchronize with the first VBLANK signal

    if (glewInit() != GLEW_OK) { //Initialize GLEW library
        fprintf(stderr, "Can't initialize GLEW.\n");
        exit(EXIT_FAILURE);
    }

    initOpenGLProgram(window); //Initialization procedure

    float angle_x = 0; //Object rotation angle
    float angle_y = 0; //Object rotation angle


//
//
//    cout<<"LOL "<<vertexCount<<endl;
//

    glfwSetTime(0); //Zero time counter

    //Main loop
    cout<<"FSasfa"<<endl;
    while (!glfwWindowShouldClose(window)) //As long as window shouldnt be closed...
    {
        angle_x += speed_x*glfwGetTime(); //Increase angle by the angle speed times the time passed since the previous frame
        angle_y += speed_y*glfwGetTime(); //Increase angle by the angle speed times the time passed since the previous frame
        glfwSetTime(0); //Zero time counter
        drawScene(window,angle_x,angle_y); //Execute drawing procedure
        glfwPollEvents(); //Execute callback procedures which process events
    }

    freeOpenGLProgram(); //Free resources

    glfwDestroyWindow(window); //Delete OpenGL context and window
    glfwTerminate(); //Free GLFW resources
    exit(EXIT_SUCCESS);
}
