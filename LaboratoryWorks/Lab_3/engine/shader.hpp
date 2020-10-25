#pragma once
#include <string>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

class Shader{ //dummy lol
    private:
        int dummy;
    public:
    void setFloat(const std::string& name, float value);
};
