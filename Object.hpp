//
//  Object.hpp
//
//
//  Created by Maciej Mościcki on 20.05.2018.
//

#ifndef Object_hpp
#define Object_hpp

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <vector>
#include <string>
using namespace std;

class ModelO {
private:
    vector<glm::vec4> vertices;
    vector<glm::vec2> uvs;
    vector<glm::vec4> normals;
public:
    ModelO();

    bool loadFromOBJFile(const string &path);

    vector<glm::vec4> & getVertices();

    vector<glm::vec2> & getUvs();

    vector<glm::vec4> & getNormals();
};
#endif /* Object_hpp */
