#include <QtWidgets>
#include <QtOpenGL>
#include <rabbit/chart/chart.h> 
#include <rabbit/types.h>
#include <igris/time/systime.h>

namespace rabbit
{
	class qtchart : public QOpenGLWidget, protected QOpenGLFunctions
	{
		QOpenGLShaderProgram * shader;
		int posAttribute;

		std::vector<rabbit::xypoint> series;
		bool realtime_shift_enable = true;
		float realtime_shift_max = -1; 
		float yscale = 1;
		float xscale = 1;
		bool autoscale = true;

		rabbit::vec4f borders;
		bool need_update = true;
		QTimer * update_timer;

		rabbit::vec2f cursor_position;
		bool cursor_visible = false;

		const char* vertexShaderSource =
		    "#version 330 core\n"
		    "in vec2 ver;\n"
			"uniform vec2 offset;\n"
			"uniform vec2 post_offset;\n"
			"uniform float xscale;\n"
			"uniform float yscale;\n"
		    "void main()\n" 
		    "{\n"
			"   float x = (ver.x - offset.x) / xscale - post_offset.x;\n"
			"   float y = (ver.y - offset.y) / yscale - post_offset.y;\n"
		    "   gl_Position = vec4(x, y, 0, 1) ;\n"
		    "}\n";


		const char* fragmentShaderSource =
    		"#version 330 core\n"
		    "out vec4 fragColor;\n"
			"uniform vec4 Color;\n"
		    "void main()\n"
		    "{\n"
        	"	fragColor = Color;\n"
    		"}\n";

	public:
		qtchart(QWidget *parent = 0)
		{
			setMouseTracking(true); 
		}

		void initializeGL()
		{    
			connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &qtchart::cleanup);

    		initializeOpenGLFunctions();
    		glClearColor(.0f, .0f, .0f, 1.0f);
    		shader = new QOpenGLShaderProgram(this);
		
    		shader->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    		shader->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
		
    		Q_ASSERT(shader->link());
    		posAttribute = shader->attributeLocation("ver");
		
    		//Q_ASSERT(shader->bind());
    		glEnable(GL_DEPTH_TEST);
    		glEnable(GL_CULL_FACE);

			update_timer = new QTimer(this);
			connect(update_timer, &QTimer::timeout, this, &qtchart::update_handle);
			update_timer->start(10);
		}

		void paintGL()
		{
			shader->bind();

		     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			set_viewport_borders(20,20,20,20);
		
		     //GLfloat vertices[] = { 0.0f, 0.707f, -0.5f, -0.5f, 0.5f, -0.5f };
			shader->setAttributeArray(posAttribute, (GLfloat*)series.data(), 2);
			shader->setUniformValue("offset", realtime_shift_max, 0.0f);
			shader->setUniformValue("post_offset", -1.0f, 0.0f);
			shader->setUniformValue("xscale", xscale);
			shader->setUniformValue("yscale", yscale);
			shader->setUniformValue("Color", 1,0,1,0);
		
		     glEnableVertexAttribArray(posAttribute);
		     glDrawArrays(GL_LINE_STRIP, 0, series.size());
		     glDisableVertexAttribArray(posAttribute);

			 if (cursor_visible) 
			 {
				shader->setUniformValue("offset", 0.0f, 0.0f);
				shader->setUniformValue("post_offset", 0.0f, 0.0f);
				shader->setUniformValue("xscale", 1.0f);
				shader->setUniformValue("yscale", 1.0f);
				shader->setUniformValue("Color", 1,1,1,0);

				GLfloat horizontal[] = { -1.0f, cursor_position.y, 1.0f, cursor_position.y };
				shader->setAttributeArray(posAttribute, horizontal, 2);
		     	glEnableVertexAttribArray(posAttribute);
		     	glDrawArrays(GL_LINE_STRIP, 0, 2);
		     	glDisableVertexAttribArray(posAttribute);

				GLfloat vertical[] = { cursor_position.x, -1.0f, cursor_position.x, 1.0f };
				shader->setAttributeArray(posAttribute, vertical, 2);
		     	glEnableVertexAttribArray(posAttribute);
		     	glDrawArrays(GL_LINE_STRIP, 0, 2);
		     	glDisableVertexAttribArray(posAttribute);
			 }

		     shader->release();
		}

		void cleanup() 
		{
			
		}

		void update_handle()
		{
			if (need_update)
			{
				update();
				need_update = false;
			}
		}

		void add_data(float x, float y)
		{
			series.push_back(rabbit::xypoint(x, y));
			if (x > realtime_shift_max)
				realtime_shift_max = x;

			if (autoscale)
			{
				if (fabs(y) > yscale)
					yscale = fabs(y);
			}
			need_update = true;
		}

		void set_xscale(float xscale)
		{
			this->xscale = xscale;
		}

		void multiply_xscale(float mul) 
		{
			xscale *= mul;
		}

		void set_yscale(float yscale)
		{
			this->yscale = yscale;
		}

		void multiply_yscale(float mul) 
		{
			yscale *= mul;
		}

		void set_autoscale(bool autoscale)
		{
			this->autoscale = autoscale;
		}

		// Convert coordinates from widget to chart
		rabbit::vec2f widget_to_chart(QPointF p)
		{
			auto width = this->width() - borders.x - borders.z;
			auto height = this->height() - borders.y - borders.w;
			p = p - QPointF(borders.x, borders.w);
			return rabbit::vec2f(p.x() / width * 2 - 1, -(p.y() / height * 2 - 1));
		}

		void mouseMoveEvent(QMouseEvent *event)
		{
			cursor_position = widget_to_chart(event->pos());
			cursor_visible = true;
			update();
		}

		void leaveEvent(QEvent *event)
		{
			cursor_visible = false;
			update();
		}

		void set_viewport_borders(int left, int top, int right, int bottom)
		{
			borders = rabbit::vec4f(left, bottom, right, top);
			glViewport(left, bottom, width()-right-left, height()-bottom-top);
		}
	};
}
