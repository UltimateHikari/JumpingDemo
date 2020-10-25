#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../engine/shader.hpp"

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
};

struct Texture{
    GLuint id;
    std::string type;
};

class Mesh{
private:
    GLuint VAO, VBO, EBO; //vertex/element array/buffer
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <Texture> textures;
    void prepareMesh();    
public:
    std::vector<Vertex>& getVertices(){return vertices;};
    std::vector<GLuint>& getIndices(){return indices;};
    std::vector<Texture>& getTextures(){return textures;};
    Mesh(
        std::vector<Vertex> vertices_,
        std::vector<GLuint> indices_,
        std::vector<Texture> textures_
        );
    void Draw(Shader shader);
};
