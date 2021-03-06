#version 330 core

layout(location = 0) in vec3 mPos;
layout(location = 1) in vec3 mNormal; //normal
layout(location = 2) in vec2 mTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fNormal;
out vec3 fPosition;
out vec2 fTexCoords;

void main(){
    gl_Position = projection*view*model*vec4(mPos,1);
    fNormal = mNormal;
    fPosition = vec3(model*vec4(mPos,1));
    fTexCoords = mTexCoords;
}

