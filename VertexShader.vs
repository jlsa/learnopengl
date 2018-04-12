#version 330 core
layout (location = 0) in vec3 aPos; // position variable has attribute position 0
layout (location = 1) in vec3 aColor; // color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord; // texture variable has attribute position 2

out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	ourColor = aColor; 
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}