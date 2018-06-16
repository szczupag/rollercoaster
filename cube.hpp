//
//  cube.hpp
//  
//
//  Created by Maciej Mościcki on 20.05.2018.
//

#ifndef cube_hpp
#define cube_hpp

#include "model.h"

namespace Models {
    namespace CubeInternal {
        extern float vertices[];
        extern float normals[];
        extern float vertexNormals[];
        extern float texCoords[];
        extern float colors[];
        extern unsigned int vertexCount;
    }
    
    class Cube: public Model {
    public:
        Cube();
        virtual ~Cube();
        virtual void drawSolid();
    };
    
    extern Cube cube;
}


#endif
