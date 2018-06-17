//
//  main.cpp
//  rollercoaster
//
//  Created by Maciej Mościcki on 20.05.2018.
//  Copyright © 2018 Maciej Mościcki. All rights reserved.
//

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
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

float speed_x = 0.0001f; // [radians/s]
float speed_y = 0; // [radians/s]
vec3 cameraPos = vec3(0.0f, 10.0f, -60.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

float trans[207] = {
-6.82f,	-8.29f,	-0.59f,
-5.16f,	-11.66f,-1.48f,
-3.81f,	-13.90f,-1.97f,
-2.79f,	-15.01f,-2.05f,
-2.09f,	-14.98f,-1.71f,
-1.70f,	-13.81f,-0.97f,
-1.64f,	-11.51f,0.18f,
-1.26f,	-9.32f,	1.84f,
-0.49f,	-8.17f,	3.31f,
0.07f,	-7.29f,	4.14f,
0.42f,	-6.68f,	4.31f,
0.56f,	-6.35f,	3.83f,
0.49f,	-6.29f,	2.71f,
0.21f,	-6.50f,	0.93f,
-0.20f,	-6.60f,	-1.29f,
-0.46f,	-6.42f,	-3.09f,
-0.53f,	-6.26f,	-4.22f,
-0.40f,	-6.12f,	-4.69f,
-0.07f,	-6.01f,	-4.49f,
0.46f,	-5.92f,	-3.61f,
1.19f,	-5.86f,	-2.08f,
1.54f,	-5.66f,	-0.34f,
1.58f,	-5.40f,	0.87f,
1.94f,	-5.26f,	1.66f,
2.61f,	-5.25f,	2.03f,
3.60f,	-5.36f,	1.97f,
4.90f,	-5.60f,	1.49f,
6.52f,	-5.97f,	0.59f,
8.16f,	-5.14f,	-0.35f,
9.28f,	-3.12f,	-0.91f,
9.88f,	-1.27f,	-1.25f,
9.97f,	0.42f,	-1.36f,
9.55f,	1.95f,	-1.25f,
8.62f,	3.31f,	-0.91f,
7.18f,	4.51f,	-0.35f,
5.50f,	7.07f,	0.47f,
4.10f,	10.38f,	1.28f,
2.97f,	12.61f,	1.91f,
2.13f,	13.77f,	2.34f,
1.56f,	13.85f,	2.59f,
1.27f,	12.86f,	2.65f,
1.26f,	10.80f,	2.53f,
0.99f,	8.94f,	2.88f,
0.38f,	8.10f,	3.56f,
-0.05f,	7.41f,	3.83f,
-0.33f,	6.88f,	3.67f,
-0.44f,	6.51f,	3.10f,
-0.38f,	6.29f,	2.12f,
-0.16f,	6.22f,	0.72f,
0.16f,	5.87f,	-0.97f,
0.37f,	5.28f,	-2.39f,
0.42f,	4.93f,	-3.38f,
0.32f,	4.82f,	-3.94f,
0.05f,	4.93f,	-4.07f,
-0.37f,	5.28f,	-3.77f,
-0.95f,	5.86f,	-3.05f,
-1.05f,	6.56f,	-2.64f,
-0.84f,	7.11f,	-2.72f,
-1.06f,	7.44f,	-2.63f,
-1.72f,	7.58f,	-2.36f,
-2.82f,	7.50f,	-1.90f,
-4.36f,	7.23f,	-1.27f,
-6.33f,	6.74f,	-0.46f,
-7.96f,	5.60f,	0.61f,
-8.76f,	3.90f,	1.57f,
-9.26f,	2.15f,	2.15f,
-9.45f,	0.37f,	2.34f,
-9.34f,	-1.46f,	2.15f,
-8.93f,	-3.33f,	1.57f
};

float lastX = 400, lastY = 300;
bool firstMouse = true;
float pitch2 = 0.0f, yaw2 = 0.0f;
bool first = true;

float aspect=1; //Ratio of width to height

int index = 0;

glm::mat4 Mstart = glm::mat4(1.0f);


void moveCarClick(){
    index+=3;
    Mstart = translate(Mstart, glm::vec3(trans[index],trans[index + 1],trans[index + 2]));
    cout<<to_string(Mstart)<<endl;
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
        if (key == GLFW_KEY_SPACE) moveCarClick();
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
void drawScene(GLFWwindow* window, vector<Model*> & models) {
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

    //g//lm::mat4 M2;
    //if(first) {
      //  M2 = glm::mat4(1.0f);
    //} else {
        //M2 = moveCar(M2);
   // }

    for(int i = 0; i < models.size(); i++){
        if(i == 1){
            models[i]->drawObject(P2, V2, Mstart);

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
        //angle_x += speed_x*glfwGetTime(); //Increase angle by the angle speed times the time passed since the previous frame
        //angle_y += speed_y*glfwGetTime(); //Increase angle by the angle speed times the time passed since the previous frame
        //glfwSetTime(0); //Zero time counter
        //cout<<glfwGetTime()<<endl;
        drawScene(window,models); //Execute drawing procedure
        glfwPollEvents(); //Execute callback procedures which process events
    }

    freeOpenGLProgram(models); //Free resources

    glfwDestroyWindow(window); //Delete OpenGL context and window
    glfwTerminate(); //Free GLFW resources
    exit(EXIT_SUCCESS);
}
