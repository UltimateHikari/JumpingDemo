#pragma once
#include <string>

class Shader{
private:
    GLuint ProgramID;
public:
    GLuint getID();
    Shader(const char* vertex_file_path, const char* fragment_file_path);
    void use();
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
};