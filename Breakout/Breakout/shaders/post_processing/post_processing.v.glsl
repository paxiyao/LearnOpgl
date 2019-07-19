#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
	vec2 texCoords = vertex.zw;
	if(chaos)
	{
		float strength = 0.3;
		vec2 pos = vec2(texCoords.x + sin(time) * strength, texCoords.y + cos(time) * strength);
		TexCoords = pos; 
	}
	else if(confuse)
	{
		TexCoords = vec2(1.0 - texCoords.x, 1.0 - texCoords.y);
	}
	else 
	{
		TexCoords = texCoords;
	}

	if(shake)
	{
		float strength = 0.01;
		gl_Position.x += cos(time * 10) * strength;
		gl_Position.y += cos(time * 15) * strength;
	}
}