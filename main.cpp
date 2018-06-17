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
#include "constants.h"
#include "Object.hpp"
#include "Model.h"
#include <vector>

using namespace glm;

float speed_x = 1.0f; // [radians/s]
float speed_y = 0; // [radians/s]
vec3 cameraPos = vec3(0.0f, 10.0f, -60.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

float lastX = 400, lastY = 300;
bool firstMouse = true;
float pitch2 = 0.0f, yaw2 = 0.0f;

float aspect=1; //Ratio of width to height


glm::vec4 moveCar(){

}



void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

//Key event processing procedure
void key_callback(GLFWwindow* window, int key,
                  int scancode, int action, int mods) {
    float cameraSpeed = 6.5f;

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT)  cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
        if (key == GLFW_KEY_RIGHT)  cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
        if (key == GLFW_KEY_UP)  cameraPos += cameraSpeed * cameraFront;
        if (key == GLFW_KEY_DOWN)  cameraPos -= cameraSpeed * cameraFront;
    }


    if (action == GLFW_RELEASE) {
      //  if (key == GLFW_KEY_LEFT) speed_y = 0;
       // if (key == GLFW_KEY_RIGHT) speed_y = 0;
        //if (key == GLFW_KEY_UP)
        //if (key == GLFW_KEY_DOWN) speed_x = 0;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw2   += xoffset;
    pitch2 += yoffset;

    if(pitch2 > 89.0f)
        pitch2 = 89.0f;
    if(pitch2 < -89.0f)
        pitch2 = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw2)) * cos(glm::radians(pitch2));
    front.y = sin(glm::radians(pitch2));
    front.z = sin(glm::radians(yaw2)) * cos(glm::radians(pitch2));
    cameraFront = glm::normalize(front);
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

//Initialization procedure
void initOpenGLProgram(GLFWwindow* window) {
    //************Insert initialization code here************
    glClearColor(0.9, 0.5, 0.5, 1); //Clear the screen to black
    glEnable(GL_DEPTH_TEST); //Turn on Z-Buffer
    glfwSetKeyCallback(window, key_callback); //Register key event processing procedure
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window,windowResize);
}

//Freeing of resources
void freeOpenGLProgram(vector<Model*> & models){
 for(int i = 0; i < models.size(); i++){
        models[i]->freeOpenGLProgram();
    }
}
//Procedure which draws the scene
void drawScene(GLFWwindow* window, float angle_x, float angle_y, vector<Model*> & models) {
    //************Place the drawing code here******************l

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

    glm::mat4 P = glm::perspective(50 * PI / 180,aspect, 1.0f, 400.0f); //Compute projection matrix

    glm::mat4 V = glm::lookAt( //Compute view matrix
                              glm::vec3(0.0f, 0.0f, -60.0f),
                              glm::vec3(0.0f, 0.0f, 0.0f),
                              glm::vec3(0.0f, 10.0f, 10.0f));


    //Compute model matrix
    glm::mat4 M = glm::mat4(1.0f);

    glm::mat4 P2 = glm::perspective(50 * PI / 180,aspect, 1.0f, 360.0f); //Compute projection matrix

    glm::mat4 V2 = glm::lookAt( //Compute view matrix
                              cameraPos, cameraPos + cameraFront, cameraUp);




    //Compute model matrix
    glm::mat4 M2 = glm::mat4(1.0f);
  M2 = glm::translate(M2, glm::vec3(angle_x * 5, 0.0f, 0.0f));



    for(int i = 0; i < models.size(); i++){
        if(i == 1){
            models[i]->drawObject(P2, V2, M2);

        } else {
            models[i]->drawObject(P2, V2, M);

        }
    }

    glfwSwapBuffers(window);

}



int main(void)
{
    vector<Model*> models;

    Model rails;
    if(!rails.loadFromOBJFile("rails.obj")) {
        return 1;
    }

    Model car;
      if(!car.loadFromOBJFile("car.obj")) {
        return 1;
    }

    Model cloud;
    if(!cloud.loadFromOBJFile("clouds_attatched.obj")){
        return 1;
    }

    Model mountains;
    if(!mountains.loadFromOBJFile("mountains.obj")){
        return 1;
    }

    Model trees;
    if(!trees.loadFromOBJFile("trees_attatched.obj")){
        return 1;
    }

    models.push_back(&rails);
    models.push_back(&car);
    models.push_back(&cloud);
    models.push_back(&mountains);
    models.push_back(&trees);

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
    rails.init("vshader.glsl", "fshader.glsl", "metal.png");
    car.init("vshader.glsl", "fshader.glsl", "bricks.png");
    cloud.init("vshader.glsl", "fshader.glsl", "white.png");
    mountains.init("vshader.glsl", "fshader.glsl", "ground.png");
    trees.init("vshader.glsl", "fshader.glsl", "ground.png");



    float angle_x = 0; //Object rotation angle
    float angle_y = 0; //Object rotation angle

    glfwSetTime(0); //Zero time counter

    while (!glfwWindowShouldClose(window)) //As long as window shouldnt be closed...
    {
        angle_x += speed_x*glfwGetTime(); //Increase angle by the angle speed times the time passed since the previous frame
        //angle_y += speed_y*glfwGetTime(); //Increase angle by the angle speed times the time passed since the previous frame
        glfwSetTime(0); //Zero time counter
        cout<<glfwGetTime()<<endl;
        drawScene(window,angle_x,angle_y, models); //Execute drawing procedure
        glfwPollEvents(); //Execute callback procedures which process events
    }

    freeOpenGLProgram(models); //Free resources

    glfwDestroyWindow(window); //Delete OpenGL context and window
    glfwTerminate(); //Free GLFW resources
    exit(EXIT_SUCCESS);
}
