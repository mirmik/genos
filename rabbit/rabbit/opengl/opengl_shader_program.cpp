#include <rabbit/opengl/opengl_shader_program.h>
#include <rabbit/opengl/util.h>

rabbit::opengl_shader_program::opengl_shader_program() {}

rabbit::opengl_shader_program::opengl_shader_program(const char *vShaderCode,
                                                     const char *fShaderCode)
{
    open(vShaderCode, fShaderCode);
}

void rabbit::opengl_shader_program::open(const char *vShaderCode,
                                         const char *fShaderCode)
{
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        abort();
    }
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        abort();
    }
    // Shader ProgramId
    this->ProgramId = glCreateProgram();
    GLCHECK(glAttachShader(this->ProgramId, vertex));
    GLCHECK(glAttachShader(this->ProgramId, fragment));
    GLCHECK(glLinkProgram(this->ProgramId));
    // Print linking errors if any
    GLCHECK(glGetProgramiv(this->ProgramId, GL_LINK_STATUS, &success));
    if (!success)
    {
        glGetProgramInfoLog(this->ProgramId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::ProgramId::LINKING_FAILED\n"
                  << infoLog << std::endl;
        abort();
    }
    // Delete the shaders as they're linked into our ProgramId now and no longer
    // necessery
    GLCHECK(glDeleteShader(vertex));
    GLCHECK(glDeleteShader(fragment));
}

// Uses the current shader
void rabbit::opengl_shader_program::use() const
{
    GLCHECK(glUseProgram(this->ProgramId));
}

void rabbit::opengl_shader_program::unuse() const
{
    glUseProgram(0);
}

void rabbit::opengl_shader_program::uniform_vec3f(
    const char *locname, const linalg::vec<float, 3> &v) const
{
    GLint modelLoc = glGetUniformLocation(ProgramId, locname);
    GLCHECK(glUniform3fv(modelLoc, 1, (GLfloat *)&v));
}

void rabbit::opengl_shader_program::uniform_mat4f(
    const char *locname, const linalg::mat<float, 4, 4> &v) const
{
    GLint modelLoc = glGetUniformLocation(ProgramId, locname);
    GLCHECK(glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)&v));
}

void rabbit::opengl_shader_program::draw_arrays(GLenum mode,
                                                GLint first,
                                                GLsizei count) const
{
    GLCHECK(glDrawArrays(mode, first, count));
}

void rabbit::opengl_shader_program::draw_elements(GLenum mode,
                                                  GLsizei count,
                                                  GLenum type,
                                                  const void *indices) const
{
    GLCHECK(glDrawElements(mode, count, type, indices));
}