#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <string.h>

#include "shader.hpp"

Shader :: Shader(const char * vertex_file_path,const char * fragment_file_path, std::string& type_): boundLights(0){
	type = type_;

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		std::cerr << "Impossible to open " <<  vertex_file_path;
		//mb do sth?
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	std::cerr << "Compiling shader: " <<  vertex_file_path << std::endl;
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		for(auto i:VertexShaderErrorMessage){
			std::cerr << i;
		}
		std::cerr << std::endl;
	}

	std::cerr << "Compiling shader: " <<  fragment_file_path << std::endl;
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		for(auto i:FragmentShaderErrorMessage){
			std::cerr << i;
		}
		std::cerr << std::endl;
	}

	std::cerr << "Linking program\n";
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		for(auto i:ProgramErrorMessage){
			std::cerr << i;
		}
		std::cerr << std::endl;
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

GLuint Shader :: getID(){
	return ProgramID;
}

void Shader :: use(){
	glUseProgram(ProgramID);
}

GLuint Shader :: getBoundLights(){
	return boundLights;
}
void Shader :: BindLight(){
	boundLights++;
}

void Shader :: finalizeLight(){
	glUniform1i(glGetUniformLocation(ProgramID, "actualPointLights"), boundLights);
}

void Shader :: resetLight(){
	boundLights = 0;
}

void Shader :: setBool(const std::string &name, bool value) const {         
    glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), (int)value); 
}
void Shader :: setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), value); 
}
void Shader :: setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(ProgramID, name.c_str()), value); 
} 

void Shader :: setMat4(const std::string& name, const glm::mat4& mat) const{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader :: setVec3(const std::string& name, const glm::vec3& value) const{
	glUniform3fv(glGetUniformLocation(ProgramID, name.c_str()), 1, &value[0]);
}