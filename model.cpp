#include "model.h"

namespace Models {
    void Model::drawWire() {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        
        drawSolid();
        
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}
