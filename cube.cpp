
#include "cube.hpp"

namespace Models {
    
    Cube cube;
    
    Cube::Cube() {
        vertices=CubeInternal::vertices;
        normals=CubeInternal::normals;
        vertexNormals=CubeInternal::vertexNormals;
        texCoords=CubeInternal::texCoords;
        colors=CubeInternal::colors;
        vertexCount=CubeInternal::vertexCount;
    }
    
    Cube::~Cube() {
    }
    
    void Cube::drawSolid() {
        glEnable(GL_NORMALIZE);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        //glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glVertexPointer(4,GL_FLOAT,0,vertices);
        //glColorPointer(4,GL_FLOAT,0,colors);
        glNormalPointer(GL_FLOAT,sizeof(float)*4,vertexNormals);
        glTexCoordPointer(2,GL_FLOAT,0,texCoords);
        
        glDrawArrays(GL_TRIANGLES,0,vertexCount);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        //glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    namespace CubeInternal {
        unsigned int vertexCount=36;
        
        float vertices[]={
            1.0f,-1.0f,-1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            -1.0f,-1.0f,-1.0f,1.0f,
            
            1.0f,-1.0f,-1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            
            
            -1.0f,-1.0f, 1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            1.0f,-1.0f, 1.0f,1.0f,
            
            -1.0f,-1.0f, 1.0f,1.0f,
            -1.0f, 1.0f, 1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            
            1.0f,-1.0f, 1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            1.0f,-1.0f,-1.0f,1.0f,
            
            1.0f,-1.0f, 1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            
            -1.0f,-1.0f,-1.0f,1.0f,
            -1.0f, 1.0f, 1.0f,1.0f,
            -1.0f,-1.0f, 1.0f,1.0f,
            
            -1.0f,-1.0f,-1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            -1.0f, 1.0f, 1.0f,1.0f,
            
            -1.0f,-1.0f,-1.0f,1.0f,
            1.0f,-1.0f, 1.0f,1.0f,
            1.0f,-1.0f,-1.0f,1.0f,
            
            -1.0f,-1.0f,-1.0f,1.0f,
            -1.0f,-1.0f, 1.0f,1.0f,
            1.0f,-1.0f, 1.0f,1.0f,
            
            -1.0f, 1.0f, 1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            
            -1.0f, 1.0f, 1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            
        };
        
        float colors[]={
            1.0f,0.0f,0.0f,1.0f,
            1.0f,0.0f,0.0f,1.0f,
            1.0f,0.0f,0.0f,1.0f,
            
            1.0f,0.0f,0.0f,1.0f,
            1.0f,0.0f,0.0f,1.0f,
            1.0f,0.0f,0.0f,1.0f,
            
            0.0f,1.0f,0.0f,1.0f,
            0.0f,1.0f,0.0f,1.0f,
            0.0f,1.0f,0.0f,1.0f,
            
            0.0f,1.0f,0.0f,1.0f,
            0.0f,1.0f,0.0f,1.0f,
            0.0f,1.0f,0.0f,1.0f,
            
            0.0f,0.0f,1.0f,1.0f,
            0.0f,0.0f,1.0f,1.0f,
            0.0f,0.0f,1.0f,1.0f,
            
            0.0f,0.0f,1.0f,1.0f,
            0.0f,0.0f,1.0f,1.0f,
            0.0f,0.0f,1.0f,1.0f,
            
            1.0f,1.0f,0.0f,1.0f,
            1.0f,1.0f,0.0f,1.0f,
            1.0f,1.0f,0.0f,1.0f,
            
            1.0f,1.0f,0.0f,1.0f,
            1.0f,1.0f,0.0f,1.0f,
            1.0f,1.0f,0.0f,1.0f,
            
            0.0f,1.0f,1.0f,1.0f,
            0.0f,1.0f,1.0f,1.0f,
            0.0f,1.0f,1.0f,1.0f,
            
            0.0f,1.0f,1.0f,1.0f,
            0.0f,1.0f,1.0f,1.0f,
            0.0f,1.0f,1.0f,1.0f,
            
            1.0f,1.0f,1.0f,1.0f,
            1.0f,1.0f,1.0f,1.0f,
            1.0f,1.0f,1.0f,1.0f,
            
            1.0f,1.0f,1.0f,1.0f,
            1.0f,1.0f,1.0f,1.0f,
            1.0f,1.0f,1.0f,1.0f,
        };
        
        float normals[]={
            0.0f, 0.0f,-1.0f,0.0f,
            0.0f, 0.0f,-1.0f,0.0f,
            0.0f, 0.0f,-1.0f,0.0f,
            
            0.0f, 0.0f,-1.0f,0.0f,
            0.0f, 0.0f,-1.0f,0.0f,
            0.0f, 0.0f,-1.0f,0.0f,
            
            0.0f, 0.0f, 1.0f,0.0f,
            0.0f, 0.0f, 1.0f,0.0f,
            0.0f, 0.0f, 1.0f,0.0f,
            
            0.0f, 0.0f, 1.0f,0.0f,
            0.0f, 0.0f, 1.0f,0.0f,
            0.0f, 0.0f, 1.0f,0.0f,
            
            1.0f, 0.0f, 0.0f,0.0f,
            1.0f, 0.0f, 0.0f,0.0f,
            1.0f, 0.0f, 0.0f,0.0f,
            
            1.0f, 0.0f, 0.0f,0.0f,
            1.0f, 0.0f, 0.0f,0.0f,
            1.0f, 0.0f, 0.0f,0.0f,
            
            -1.0f, 0.0f, 0.0f,0.0f,
            -1.0f, 0.0f, 0.0f,0.0f,
            -1.0f, 0.0f, 0.0f,0.0f,
            
            -1.0f, 0.0f, 0.0f,0.0f,
            -1.0f, 0.0f, 0.0f,0.0f,
            -1.0f, 0.0f, 0.0f,0.0f,
            
            0.0f,-1.0f, 0.0f,0.0f,
            0.0f,-1.0f, 0.0f,0.0f,
            0.0f,-1.0f, 0.0f,0.0f,
            
            0.0f,-1.0f, 0.0f,0.0f,
            0.0f,-1.0f, 0.0f,0.0f,
            0.0f,-1.0f, 0.0f,0.0f,
            
            0.0f, 1.0f, 0.0f,0.0f,
            0.0f, 1.0f, 0.0f,0.0f,
            0.0f, 1.0f, 0.0f,0.0f,
            
            0.0f, 1.0f, 0.0f,0.0f,
            0.0f, 1.0f, 0.0f,0.0f,
            0.0f, 1.0f, 0.0f,0.0f,
        };
        
        float vertexNormals[]={
            1.0f,-1.0f,-1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            -1.0f,-1.0f,-1.0f,1.0f,
            
            1.0f,-1.0f,-1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            
            
            -1.0f,-1.0f, 1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            1.0f,-1.0f, 1.0f,1.0f,
            
            -1.0f,-1.0f, 1.0f,1.0f,
            -1.0f, 1.0f, 1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            
            1.0f,-1.0f, 1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            1.0f,-1.0f,-1.0f,1.0f,
            
            1.0f,-1.0f, 1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            
            -1.0f,-1.0f,-1.0f,1.0f,
            -1.0f, 1.0f, 1.0f,1.0f,
            -1.0f,-1.0f, 1.0f,1.0f,
            
            -1.0f,-1.0f,-1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            -1.0f, 1.0f, 1.0f,1.0f,
            
            -1.0f,-1.0f,-1.0f,1.0f,
            1.0f,-1.0f, 1.0f,1.0f,
            1.0f,-1.0f,-1.0f,1.0f,
            
            -1.0f,-1.0f,-1.0f,1.0f,
            -1.0f,-1.0f, 1.0f,1.0f,
            1.0f,-1.0f, 1.0f,1.0f,
            
            -1.0f, 1.0f, 1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
            1.0f, 1.0f, 1.0f,1.0f,
            
            -1.0f, 1.0f, 1.0f,1.0f,
            -1.0f, 1.0f,-1.0f,1.0f,
            1.0f, 1.0f,-1.0f,1.0f,
        };
        
        float texCoords[]={
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
            
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
            
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
            
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
            
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
            
            1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
            1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
        };
    }
}
