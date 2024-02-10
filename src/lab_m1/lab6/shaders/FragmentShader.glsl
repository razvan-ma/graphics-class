#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_color;
in vec3 frag_normal;
// Output
layout(location = 0) out vec4 out_color;

uniform vec3 object_color;
void main()
{
    out_color = vec4(object_color, 1);
}
