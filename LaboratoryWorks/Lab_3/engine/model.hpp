#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../engine/stb_image.h"
#include "../engine/shader.hpp"

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
};

struct Texture{
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh{
private:
    GLuint VAO, VBO, EBO; //vertex/element array/buffer
    std::vector <Vertex> vertices;
    std::vector <unsigned short> indices;
    std::vector <Texture> textures;
    void prepareMesh();    
public:
    std::vector<Vertex>& getVertices(){return vertices;};
    std::vector<unsigned short>& getIndices(){return indices;};
    std::vector<Texture>& getTextures(){return textures;};
    Mesh(
        std::vector<Vertex> vertices_,
        std::vector<unsigned short> indices_,
        std::vector<Texture> textures_
        );
    void Draw(Shader shader);
};

class Model {
    public:
        Model(std::string const& path){
            loadModel(path);
        }
        void Draw(Shader &shader);	
    private:
        std::vector<Mesh> meshes;
        std::string directory;
 
        void loadModel(std::string const& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};

class TextureManager {
private:
    std::vector <Texture> textures_loaded;
    TextureManager(){};
public:
    static TextureManager & getInstance();
    std::vector<Texture> loadMaterials(
        aiMaterial *mat, 
        aiTextureType type, 
        std::string typeName, 
        std::string directory);
};