#include "light.hpp"

using namespace glm;

const vec3& Light :: getAmbient() const{
    return colors.ambient;
}
const vec3& Light :: getDiffuse() const{
    return colors.diffuse;
}
const vec3& Light :: getSpecular() const{
    return colors.specular;
}
const vec3& Light :: getVector() const{
    return lightVector;
}

FadingLight :: FadingLight(
    LightColors colors_,
    glm::vec3 lightVector_,
    float distance 
    ): Light(colors_, lightVector_)
{
    //haha tricked ya, have only one distance for now
    constants = LightConstants(1.0, 0.09, 0.0032);
}

float FadingLight :: getConstant() const{
    return constants.constant;
}
float FadingLight :: getLinear() const{
    return constants.linear;
}
float FadingLight :: getQuad() const{
    return constants.quad;
}

PointLight :: PointLight(
    LightColors colors_,
    glm::vec3 lightVector_,
    float distance 
    ): FadingLight(colors_, lightVector_, distance)
{}

void PointLight :: place(Shader& shader){
    shader.setVec3("light.position", getVector());
    shader.setVec3("light.ambient", getAmbient());
    shader.setVec3("light.diffuse", getDiffuse());
    shader.setVec3("light.specular", getSpecular());

    shader.setFloat("light.constant", getConstant());
    shader.setFloat("light.lin", getLinear());
    shader.setFloat("light.quad", getQuad());
};

DefaultLamp :: DefaultLamp():
    PointLight(
        LightColors(
            vec3(0.1f,0.1f,0.1f),
            vec3(0.6f,0.6f,0.6f),
            vec3(1.0f,1.0f,1.0f)
            ),
        vec3(4.0f,4.0f,2.0f),
        50.0
    )
{}