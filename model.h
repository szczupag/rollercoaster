//
//  model.h
//  
//
//  Created by Maciej Mościcki on 20.05.2018.
//

#ifndef model_h
#define model_h

#include <vector>
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>
#include "constants.h"

namespace Models {
    
    class Model {
    public:
        int vertexCount;
        float *vertices;
        float *normals;
        float *vertexNormals;
        float *texCoords;
        float *colors;
        
        virtual void drawSolid()=0;
        virtual void drawWire();
    };
}

#endif
