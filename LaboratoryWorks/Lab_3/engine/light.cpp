#include "light.hpp"
#include <string>

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
    std::cerr << "fade constr\n";
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
{std::cerr << "point constr\n";}

void PointLight :: place(Shader& shader){
    //for dynamic lights rework needed i feel
    GLuint lightIndex = shader.getBoundLights();
    std::string prefix = "pointLights[" + std::to_string(lightIndex) + "].";
    
    shader.setVec3(prefix + "position", getVector());
    shader.setVec3(prefix + "ambient", getAmbient());
    shader.setVec3(prefix + "diffuse", getDiffuse());
    shader.setVec3(prefix + "specular", getSpecular());

    shader.setFloat(prefix + "constant", getConstant());
    shader.setFloat(prefix + "lin", getLinear());
    shader.setFloat(prefix + "quad", getQuad());

    shader.BindLight();

        std::cerr << "point placed " + std::to_string(lightIndex) + " to " << shader.getID() << "\n";

};

DefaultLamp :: DefaultLamp(vec3 position):
    PointLight(
        LightColors(
            vec3(0.1f,0.1f,0.1f),
            vec3(0.6f,0.6f,0.6f),
            vec3(1.0f,1.0f,1.0f)
            ),
        position,
        50.0
    )
{std::cerr << "default constructed\n";}