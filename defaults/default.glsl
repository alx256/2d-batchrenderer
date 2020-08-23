// This is the default texture shader, used for
// objects that have textures, with support for
// tinted textures

VERTEX SHADER
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texcoords;
layout(location = 3) in float texid;

out vec4 vcolor;
out vec2 vtexcoords;
out float vtexid;

uniform mat4 uvp;

void main() {
	gl_Position = uvp * position;
	vcolor = color;
	vtexcoords = texcoords;
	vtexid = texid;
}

FRAGMENT SHADER
#version 330 core

layout(location = 0) out vec4 color;

in vec4 vcolor;
in vec2 vtexcoords;
in float vtexid;

uniform sampler2D utextures[32];

void main() {
	int index = int(vtexid);
	if (index != 0)
		color = texture2D(utextures[index], vtexcoords) * vcolor;
	else
		color = vcolor;
}