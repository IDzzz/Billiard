#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 textura;
layout(location = 3) in vec3 normala;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 col;
uniform vec3 distance;
uniform float tac;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_normala;
out vec3 frag_position;
out vec2 frag_textura;


void main()
{
	// TODO: send output to fragment shader
	frag_color = vec3(col.x, col.y, (col.z + tac/3 * 0.9));
	frag_normala = normala;
	frag_textura = textura;
	frag_position = v_position;
	
	// TODO: compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position - distance, 1.0);
	//vec4(v_position - distance, 1.0);
}
