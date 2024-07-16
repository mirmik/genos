#include <rabbit/opengl/shader_collection.h>

const char * rabbit::mesh_vertex_shader = R"""(
#version 300 es
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
)""";

const char * rabbit::mesh_fragment_shader = R"""(
#version 300 es

uniform lowp vec4 vertexColor;
out lowp vec4 color;

void main()
{
    color = vertexColor;
} 
)""";

const char * rabbit::mesh_colored_vertex_shader = R"""(
#version 300 es
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 Color;

out lowp vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vertexColor = Color;
}
)""";

const char * rabbit::mesh_colored_fragment_shader = R"""(
#version 300 es

in lowp vec4 vertexColor;
out lowp vec4 color;

void main()
{
    color = vertexColor;
} 
)""";

const char * rabbit::simple_vertex_shader = R"""(
#version 300 es
layout (location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position, 1.0f);
}
)""";

const char * rabbit::space_3d_vertex_shader = R"""(
#version 300 es
layout (location = 0) in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
)""";

const char * rabbit::simple_2d_vertex_shader = R"""(
#version 300 es
layout (location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0f, 1.0f);
}
)""";

const char * rabbit::simple_fragment_shader = R"""(
#version 300 es

out lowp vec4 color;

void main()
{
    color = vec4(1,1,1,1);
} 
)""";

const char * rabbit::onecolored_texture_vertex_shader = R"""(
#version 300 es
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texcoords;

out vec2 TexCoords;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vertex, 1);
    TexCoords = texcoords;
}  
)""";

const char * rabbit::onecolored_texture_fragment_shader = R"""(
#version 300 es
in lowp vec2 TexCoords;
out lowp vec4 color;

uniform sampler2D ourTexture;
uniform lowp vec3 textColor;

void main()
{    
	lowp float transparenty = texture(ourTexture, TexCoords).r;

	if (transparenty < 0.5)
		discard;

    color = vec4(textColor, transparenty);
}  
)""";


const char * rabbit::rgb_texture_vertex_shader = R"""(
#version 300 es
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texcoords;

out vec2 TexCoords;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vertex, 1);
    TexCoords = texcoords;
}  
)""";

const char * rabbit::rgb_texture_fragment_shader = R"""(
#version 300 es
in lowp vec2 TexCoords;
out lowp vec4 color;

uniform sampler2D ourTexture;

void main()
{    
    color = texture(ourTexture, TexCoords);
}  
)""";


const char * rabbit::grayscale_texture_vertex_shader = R"""(
#version 300 es
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texcoords;

out vec2 TexCoords;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vertex, 1);
    TexCoords = texcoords;
}  
)""";

const char * rabbit::grayscale_texture_fragment_shader = R"""(
#version 300 es
in lowp vec2 TexCoords;
out lowp vec4 color;

uniform sampler2D ourTexture;

void main()
{    
    mediump float r = texture(ourTexture, TexCoords).r;
    color = vec4(r, r, r, 1);
}  
)""";

