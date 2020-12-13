#version 330 core
out vec4 FragColor;
 
in vec3 TexCoords;
 
uniform samplerCube skybox;
 
void main()
{    
    FragColor = 0.5*texture(skybox, TexCoords); //spooky darkey day
}