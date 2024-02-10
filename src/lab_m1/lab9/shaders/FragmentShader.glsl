#version 330

// Input
in vec2 texcoord;
in float alpha;
// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform bool mixTextures;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    
    vec4 color = texture2D(texture_1, texcoord);  
    vec4 color2 = texture2D(texture_2, texcoord);
    
    if (mixTextures == true)
	{
		out_color = mix(color, color2, .5f);
	} else
	{
		out_color = color;
	}
    if (out_color.a < .5f)
	{
		discard;
	}

}
