//
// Created by svjat on 7/24/2025.
//

#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    // glm::vec3 normal;
};

class VBO {
public:
    GLuint ID;
    VBO();
    VBO(std::vector<Vertex>& vertices);

    void SetData(std::vector<Vertex>& vertices);
    void Bind();
    void Unbind();
    void Delete();
};


#endif // VBO_H
