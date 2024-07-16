
#ifndef RABBIT_OPENGL_CHARTS_H
#define RABBIT_OPENGL_CHARTS_H

#include <rabbit/opengl/drawer.h>

namespace rabbit 
{
		class opengl_viewport
	{
		int l, r, b, t;

	public:
		void set_chart_viewport(int width, int height)
		{
			l = 200;
			t = 20;
			r = width - l;
			b = height - 50 - t;
		}

		void apply_viewport() const
		{
			glViewport(l, t, r, b);
		}
	};

		const char * chart_vertex_shader  = R"""(
#version 300 es
layout (location = 0) in vec2 position;

void main()
{
    gl_Position = vec4(position, 0.0f, 1.0f);
}
)""";

		const char * chart_fragment_shader = R"""(
#version 300 es

uniform lowp vec3 Color;
out lowp vec4 color;

void main()
{
    color = vec4(Color, 1.0f);
} 
)""";		

	class opengl_chart_drawer : public opengl_drawer
	{
	public:
		opengl_chart_drawer()
		{
			chart_program.open(
			    chart_vertex_shader,
			    chart_fragment_shader
			);
		}

		opengl_shader_program chart_program;

		void draw_xyseries(
			xypoint * points, 
			int count, 
			const linalg::vec<float,3> & color, 
			const opengl_viewport & viewport)
		{
			viewport.apply_viewport();

			set_vertices_stride(2);

			chart_program.use();
			chart_program.uniform_vec3f("Color", color);
			draw_lines((float*)points, count, GL_LINE_STRIP);
			glUseProgram(0);
		}
	};
}

#endif