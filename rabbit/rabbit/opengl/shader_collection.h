#ifndef RABBIT_MESH_SHADER_H
#define RABBIT_MESH_SHADER_H

namespace rabbit 
{
	extern const char * mesh_vertex_shader;
	extern const char * mesh_fragment_shader;
	
	extern const char * mesh_colored_vertex_shader;
	extern const char * mesh_colored_fragment_shader;

	extern const char * simple_vertex_shader;
	extern const char * simple_2d_vertex_shader;
	extern const char * simple_fragment_shader;

	extern const char * space_3d_vertex_shader;

	extern const char * onecolored_texture_vertex_shader;
	extern const char * onecolored_texture_fragment_shader;

	extern const char * rgb_texture_vertex_shader;
	extern const char * rgb_texture_fragment_shader;

	extern const char * grayscale_texture_vertex_shader;
	extern const char * grayscale_texture_fragment_shader;
}

#endif