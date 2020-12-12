#pragma once
#include "gl_includes.hpp"

#include "shader.hpp"
#include <iostream>

struct LightColors{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    LightColors(
        glm::vec3 ambient_,
        glm::vec3 diffuse_,
        glm::vec3 specular_
    ): ambient(ambient_), diffuse(diffuse_), specular(specular_) {}
};

struct LightConstants{
    float constant;
    float linear;
    float quad;
    LightConstants() = default;
    LightConstants(
        float constant_,
        float linear_,
        float quad_
    ): constant(constant_), linear(linear_), quad(quad_) {}
};

class Light{
private:
    LightColors colors;
    glm::vec3 lightVector; //position OR direction
public:
    Light(
        LightColors colors_,
        glm::vec3 lightVector_
        ): colors(colors_), lightVector(lightVector_){};
    virtual ~Light() = default;
    virtual void place(Shader& shader) = 0;
    const glm::vec3& getAmbient() const;
    const glm::vec3& getDiffuse() const;
    const glm::vec3& getSpecular() const;
    const glm::vec3& getVector() const;
};

class FadingLight : public Light{
private:
    LightConstants constants;
public: 
    FadingLight(
        LightColors colors_,
        glm::vec3 lightVector_,
        float distance //fetching constants based on closed pre-calculated distance
        );
    ~FadingLight() = default;
    float getConstant() const;
    float getLinear() const;
    float getQuad() const;
};

class PointLight : public FadingLight{
public:
    PointLight(
        LightColors colors_,
        glm::vec3 lightVector_,
        float distance
        );
    ~PointLight() = default;
    void place(Shader& shader);
};
class DefaultLamp : public PointLight{
public:
    DefaultLamp(glm::vec3 position);
    ~DefaultLamp() = default;
};
// class DirectionalLight : public Light{};
// class SpotLight : public Light{};
// class FlashLight : public FadingLight{};