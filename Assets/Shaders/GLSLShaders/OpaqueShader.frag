#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec2 TexCoords;

uniform vec3 objectColor;
uniform sampler2D texture1;

void main()
{
    FragColor = vec4(objectColor, 1.0);
  // FragColor = texture(texture1, TexCoords) * vec4(objectColor, 1.0);
}
