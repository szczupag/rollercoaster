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
vec3 cameraPos = vec3(0.0f, 50.0f, -120.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

float trans[627] = {
-2.2701f,	-0.200100000000001f,	2.77629999999999f,
-2.0733f,	-0.3149f,	3.1934f,
-1.8883f,	-0.4146f,	3.5684f,
-1.7152f,	-0.4992f,	3.9015f,
-1.554f,	-0.5684f,	4.19250000000002f,
-1.4046f,	-0.622499999999999f,	4.44149999999999f,
-1.2671f,	-0.6616f,	4.64829999999999f,
-1.1415f,	-0.685300000000002f,	4.81310000000001f,
-1.0277f,	-0.693899999999999f,	4.9359f,
-0.925800000000002f,	-0.6874f,	5.0167f,
-0.835799999999999f,	-0.6656f,	5.0553f,
-0.757600000000004f,	-0.6287f,	5.05199999999999f,
-0.691299999999998f,	-0.5766f,	5.00660000000001f,
-0.636899999999997f,	-0.5093f,	4.9191f,
-0.594300000000004f,	-0.426900000000001f,	4.78959999999999f,
-0.563499999999998f,	-0.329199999999999f,	4.61799999999999f,
-0.544699999999999f,	-0.2165f,	4.40440000000001f,
-0.537800000000004f,	-0.0884f,	4.14879999999999f,
-0.5426f,	0.0546999999999995f,	3.8511f,
-0.5593f,	0.213f,	3.5113f,
-0.518499999999996f,	0.411f,	3.25f,
-0.417400000000001f,	0.6226f,	3.102f,
-0.324100000000001f,	0.81f,	2.964f,
-0.238500000000002f,	0.9735f,	2.8363f,
-0.160799999999995f,	1.1129f,	2.7185f,
-0.0907000000000053f,	1.2283f,	2.6108f,
-0.0285000000000011f,	1.3195f,	2.5133f,
0.0259f,	1.3869f,	2.4258f,
0.0726000000000013f,	1.43f,	2.3484f,
0.111499999999999f,	1.4493f,	2.2812f,
0.142600000000002f,	1.4444f,	2.224f,
0.165900000000001f,	1.4155f,	2.1769f,
0.1815f,	1.3626f,	2.1399f,
0.1892f,	1.2856f,	2.113f,
0.189300000000003f,	1.1845f,	2.0963f,
0.1815f,	1.0595f,	2.0895f,
0.165900000000001f,	0.910399999999999f,	2.0929f,
0.142599999999995f,	0.737200000000001f,	2.1064f,
0.111500000000007f,	0.540099999999999f,	2.1299f,
0.0725999999999942f,	0.3188f,	2.1637f,
0.0259f,	0.0735000000000028f,	2.2074f,
-0.0244999999999962f,	-0.190000000000001f,	2.2213f,
-0.0686000000000035f,	-0.439399999999999f,	2.1992f,
-0.105399999999996f,	-0.664000000000001f,	2.1779f,
-0.134800000000006f,	-0.863900000000001f,	2.1577f,
-0.156799999999997f,	-1.0391f,	2.1381f,
-0.171500000000002f,	-1.1895f,	2.1196f,
-0.178899999999999f,	-1.3153f,	2.1019f,
-0.178899999999999f,	-1.4163f,	2.0851f,
-0.171500000000002f,	-1.4926f,	2.0692f,
-0.156799999999997f,	-1.5442f,	2.0542f,
-0.134799999999998f,	-1.5711f,	2.04f,
-0.105400000000003f,	-1.5732f,	2.0268f,
-0.0686000000000035f,	-1.5506f,	2.0143f,
-0.0244999999999962f,	-1.5034f,	2.0029f,
0.027000000000001f,	-1.4313f,	1.9923f,
0.0856999999999957f,	-1.3346f,	1.9826f,
0.152000000000001f,	-1.2132f,	1.9737f,
0.2254f,	-1.067f,	1.9658f,
0.3063f,	-0.896100000000001f,	1.9587f,
0.394500000000001f,	-0.7006f,	1.9525f,
0.490099999999998f,	-0.4802f,	1.9473f,
0.527100000000004f,	-0.2736f,	1.924f,
0.508600000000001f,	-0.1077f,	1.8853f,
0.5017f,	0.0425f,	1.8514f,
0.506399999999999f,	0.1772f,	1.8221f,
0.5229f,	0.2961f,	1.7974f,
0.550899999999999f,	0.3994f,	1.7775f,
0.590800000000002f,	0.4871f,	1.7622f,
0.642299999999999f,	0.5591f,	1.7516f,
0.705300000000001f,	0.6155f,	1.7457f,
0.780200000000001f,	0.6561f,	1.7444f,
0.866599999999998f,	0.6812f,	1.7478f,
0.964799999999997f,	0.6907f,	1.7559f,
1.0746f,	0.684300000000001f,	1.7687f,
1.1961f,	0.6625f,	1.7861f,
1.3293f,	0.6249f,	1.8083f,
1.4741f,	0.5717f,	1.835f,
1.6306f,	0.502899999999999f,	1.8665f,
1.7987f,	0.4183f,	1.9026f,
1.9786f,	0.318300000000001f,	1.9434f,
2.1702f,	0.202399999999999f,	1.9889f,
2.3733f,	0.0709f,	2.039f,
2.5659f,	-0.0419999999999998f,	1.9457f,
2.7273f,	-0.1206f,	1.70959999999999f,
2.87f,	-0.190799999999999f,	1.47970000000001f,
2.9936f,	-0.2529f,	1.25579999999999f,
3.0983f,	-0.3066f,	1.0378f,
3.1841f,	-0.352f,	0.825999999999993f,
3.251f,	-0.3893f,	0.620199999999997f,
3.299f,	-0.418200000000001f,	0.420500000000004f,
3.3281f,	-0.438899999999999f,	0.226700000000008f,
3.3382f,	-0.4512f,	0.0390999999999906f,
3.3294f,	-0.455500000000001f,	-0.142499999999998f,
3.3018f,	-0.4512f,	-0.318200000000004f,
3.2551f,	-0.4389f,	-0.48769999999999f,
3.1896f,	-0.4182f,	-0.651200000000003f,
3.1051f,	-0.3893f,	-0.808599999999998f,
3.0018f,	-0.352f,	-0.960099999999997f,
2.8795f,	-0.3066f,	-1.1054f,
2.7383f,	-0.252899999999999f,	-1.2448f,
2.5781f,	-0.1908f,	-1.378f,
2.3992f,	-0.1206f,	-1.50530000000001f,
2.2011f,	-0.0419999999999998f,	-1.62649999999999f,
2.0045f,	0.0543999999999993f,	-1.92190000000001f,
1.82839999999999f,	0.1585f,	-2.369f,
1.6627f,	0.2557f,	-2.7764f,
1.5073f,	0.3459f,	-3.14400000000001f,
1.362f,	0.4293f,	-3.4716f,
1.2271f,	0.5056f,	-3.7597f,
1.1024f,	0.575f,	-4.0078f,
0.988f,	0.637499999999999f,	-4.2161f,
0.883899999999997f,	0.693f,	-4.3846f,
0.789900000000003f,	0.7416f,	-4.5134f,
0.706299999999999f,	0.783300000000001f,	-4.6024f,
0.632899999999999f,	0.8179f,	-4.6515f,
0.569800000000001f,	0.845700000000001f,	-4.6608f,
0.5169f,	0.8665f,	-4.6304f,
0.474299999999999f,	0.8804f,	-4.5602f,
0.442099999999996f,	0.8872f,	-4.45f,
0.419900000000005f,	0.8873f,	-4.3003f,
0.408199999999994f,	0.880299999999998f,	-4.1105f,
0.406600000000005f,	0.866400000000002f,	-3.8812f,
0.415300000000002f,	0.845499999999998f,	-3.6118f,
0.4343f,	0.817800000000002f,	-3.3028f,
0.4056f,	0.8598f,	-3.0809f,
0.326499999999996f,	0.966200000000001f,	-2.9764f,
0.253599999999999f,	1.0571f,	-2.8777f,
0.186600000000006f,	1.1326f,	-2.7845f,
0.125699999999995f,	1.1927f,	-2.6973f,
0.0710000000000051f,	1.2373f,	-2.6155f,
0.0222999999999942f,	1.2667f,	-2.5396f,
-0.0202999999999989f,	1.2805f,	-2.4693f,
-0.0567999999999955f,	1.279f,	-2.4048f,
-0.0872000000000028f,	1.262f,	-2.346f,
-0.111499999999999f,	1.2297f,	-2.2928f,
-0.129800000000003f,	1.1819f,	-2.2453f,
-0.141999999999996f,	1.1187f,	-2.2036f,
-0.148099999999999f,	1.04f,	-2.16759999999999f,
-0.148000000000003f,	0.946099999999994f,	-2.1372f,
-0.141999999999996f,	0.836600000000004f,	-2.1127f,
-0.129800000000003f,	0.7119f,	-2.0936f,
-0.111499999999999f,	0.5715f,	-2.0805f,
-0.087299999999999f,	0.415900000000001f,	-2.0729f,
-0.0566999999999993f,	0.244900000000001f,	-2.0711f,
-0.0202999999999989f,	0.0582999999999956f,	-2.075f,
0.019599999999997f,	-0.140000000000001f,	-2.035f,
0.0549999999999997f,	-0.329299999999996f,	-1.953f,
0.0844000000000023f,	-0.502700000000004f,	-1.8796f,
0.107999999999997f,	-0.660299999999999f,	-1.81480000000001f,
0.125599999999999f,	-0.802f,	-1.75869999999999f,
0.1374f,	-0.927799999999998f,	-1.7111f,
0.143300000000004f,	-1.0376f,	-1.6722f,
0.1434f,	-1.1317f,	-1.6418f,
0.1374f,	-1.2098f,	-1.6202f,
0.125599999999999f,	-1.272f,	-1.607f,
0.108000000000004f,	-1.3184f,	-1.60249999999999f,
0.0843999999999951f,	-1.3489f,	-1.6066f,
0.0549999999999997f,	-1.3634f,	-1.61930000000001f,
0.0196000000000041f,	-1.3622f,	-1.64059999999999f,
-0.0215999999999994f,	-1.345f,	-1.6705f,
-0.0686999999999998f,	-1.3119f,	-1.70910000000001f,
-0.121700000000004f,	-1.2629f,	-1.75619999999999f,
-0.180599999999998f,	-1.1982f,	-1.812f,
-0.245400000000004f,	-1.1173f,	-1.8764f,
-0.316099999999999f,	-1.0208f,	-1.94929999999999f,
-0.392599999999995f,	-0.908300000000001f,	-2.0308f,
-0.399600000000007f,	-0.860199999999999f,	-2.11490000000001f,
-0.343899999999998f,	-0.8827f,	-2.1904f,
-0.304299999999998f,	-0.8985f,	-2.2582f,
-0.280900000000003f,	-0.9079f,	-2.31870000000001f,
-0.273600000000002f,	-0.910400000000001f,	-2.37129999999999f,
-0.282499999999999f,	-0.906499999999999f,	-2.4165f,
-0.307499999999997f,	-0.896000000000001f,	-2.45400000000001f,
-0.348799999999997f,	-0.8787f,	-2.48389999999999f,
-0.406200000000005f,	-0.854999999999999f,	-2.50620000000001f,
-0.479799999999997f,	-0.8245f,	-2.521f,
-0.569400000000002f,	-0.787500000000001f,	-2.52809999999999f,
-0.6753f,	-0.7439f,	-2.52759999999999f,
-0.797399999999996f,	-0.6937f,	-2.5196f,
-0.935600000000001f,	-0.6368f,	-2.50390000000002f,
-1.0899f,	-0.5734f,	-2.48060000000001f,
-1.2605f,	-0.503299999999999f,	-2.44969999999998f,
-1.4471f,	-0.426600000000001f,	-2.41130000000001f,
-1.65000000000001f,	-0.3433f,	-2.36519999999999f,
-1.869f,	-0.2534f,	-2.31150000000002f,
-2.1042f,	-0.1569f,	-2.25019999999998f,
-2.3555f,	-0.0538000000000003f,	-2.1814f,
-2.5459f,	0.0724f,	-2.05200000000002f,
-2.6571f,	0.2077f,	-1.8663f,
-2.7571f,	0.3288f,	-1.6788f,
-2.846f,	0.4355f,	-1.48990000000001f,
-2.9236f,	0.5281f,	-1.29929999999999f,
-2.9899f,	0.6065f,	-1.10720000000001f,
-3.0452f,	0.670500000000001f,	-0.913399999999996f,
-3.0892f,	0.7204f,	-0.718099999999993f,
-3.1219f,	0.756f,	-0.521300000000025f,
-3.1436f,	0.7774f,	-0.322699999999998f,
-3.1539f,	0.7844f,	-0.122799999999984f,
-3.1531f,	0.7774f,	0.0789999999999793f,
-3.1411f,	0.756f,	0.282200000000017f,
-3.1179f,	0.7204f,	0.486899999999991f,
-3.0835f,	0.670500000000001f,	0.693399999999997f,
-3.0378f,	0.606499999999999f,	0.90130000000002f,
-2.981f,	0.5281f,	1.11089999999999f,
-2.9129f,	0.435500000000001f,	1.32210000000001f,
-2.8337f,	0.328799999999999f,	1.53479999999999f,
-2.7433f,	0.207700000000001f,	1.749f,
21.0943f,	-14.2855f,	154.2289f
};

ShaderProgram * shaderProgramCommon;

float lastX = 400, lastY = 300;
bool firstMouse = true;
float pitch2 = 0.0f, yaw2 = 0.0f;
bool first = true;

float aspect=1; //Ratio of width to height

int index = 0;

glm::mat4 Mstart = glm::mat4(1.0f);

glm::mat4 Mcloud = glm::mat4(1.0f);


void moveCarClick(){
    glm::vec3 transVec = glm::vec3(trans[index],trans[index + 1] ,trans[index + 2] );
    cout<<"translacja o "<<to_string(transVec)<<endl;
    Mstart = translate(Mstart, transVec);
    cout<<to_string(Mstart)<<endl;
    cout<<index<<endl;
    index= (index+3);
        if(index == 627) {
        index = 0;
        Mstart =  glm::mat4(1.0f);
    }
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
    glClearColor(135 / 255.0f, 206 /255.0f, 235/255.0f, 1); //Clear the screen to black
    glEnable(GL_DEPTH_TEST); //Turn on Z-Buffer
    glfwSetKeyCallback(window, key_callback); //Register key event processing procedure
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window,windowResize);
    shaderProgramCommon = new ShaderProgram("vshader.glsl",NULL,"fshader.glsl");
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


    glm::mat4 M = glm::mat4(1.0f);

    glm::mat4 P2 = glm::perspective(50 * PI / 180,aspect, 1.0f, 1000.0f); //Compute projection matrix

    glm::mat4 V2 = glm::lookAt( //Compute view matrix
                              cameraPos, cameraPos + cameraFront, cameraUp);

    if( ((int)(glfwGetTime() * 20)% 1) == 0){
        moveCarClick();
    }

    Mcloud = glm::rotate(Mcloud, (float)glfwGetTime() * -0.0001f , glm::vec3(0.0f, 1.0f, 0.0f));




    for(int i = 0; i < models.size(); i++){
        if(i == 1){
            models[i]->drawObject(P2, V2, Mstart);

        } else if (i == 2){
            models[i]->drawObject(P2, V2, Mcloud);
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

    Model support;
      if(!support.loadFromOBJFile("support.obj")){
        return 1;
    }

    Model sun;
      if(!sun.loadFromOBJFile("sun.obj")){
        return 1;
    }

    models.push_back(&rails);
    models.push_back(&car);
    models.push_back(&cloud);
    models.push_back(&mountains);
    models.push_back(&trees);
    models.push_back(&support);
    models.push_back(&sun);

    GLFWwindow* window; //Pointer to window object

    glfwSetErrorCallback(error_callback);//Register error callback procedure


    if (!glfwInit()) { //Initialize GLFW procedure
        fprintf(stderr, "Can't initialize GLFW GLFW.\n");
        exit(EXIT_FAILURE);
    }


    window = glfwCreateWindow(1024, 600, "OpenGL", NULL, NULL);  //Create 500x500 window with "OpenGL" as well as OpenGL context.

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
    rails.init("vshader.glsl", "fshader.glsl", "metal.png", shaderProgramCommon);
    car.init("vshader.glsl", "fshader.glsl", "bricks.png", shaderProgramCommon);
    cloud.init("vshader.glsl", "fshader.glsl", "white.png", shaderProgramCommon);
    mountains.init("vshader.glsl", "fshader.glsl", "mountains_tex.png", shaderProgramCommon);
    trees.init("vshader.glsl", "fshader.glsl", "ground_blured.png", shaderProgramCommon);
    support.init("vshader.glsl", "fshader.glsl", "metal.png", shaderProgramCommon);
    sun.init("vshader.glsl", "fshader.glsl", "cwiek.png", shaderProgramCommon);



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
