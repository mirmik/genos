#include <igris/util/bug.h>
#include <nos/fprint.h>
#include <nos/print.h>
#include <rabbit/opengl/shader_collection.h>
#include <rabbit/opengl/util.h>

#include <rabbit/opengl/drawer.h>

void rabbit::opengl_drawer::init_opengl_context()
{
    opengl_mesh_program.open(rabbit::mesh_vertex_shader,
                             rabbit::mesh_fragment_shader);

    opengl_colored_mesh_program.open(rabbit::mesh_colored_vertex_shader,
                                     rabbit::mesh_colored_fragment_shader);

    opengl_simple_program.open(rabbit::simple_vertex_shader,
                               rabbit::simple_fragment_shader);

    opengl_simple_2d_program.open(rabbit::simple_2d_vertex_shader,
                                  rabbit::simple_fragment_shader);

    opengl_space_point3d_program.open(rabbit::space_3d_vertex_shader,
                                      rabbit::simple_fragment_shader);

    opengl_onecolored_texture.open(rabbit::onecolored_texture_vertex_shader,
                                   rabbit::onecolored_texture_fragment_shader);

    opengl_rgb_texture.open(rabbit::rgb_texture_vertex_shader,
                            rabbit::rgb_texture_fragment_shader);

    opengl_grayscale_texture.open(rabbit::grayscale_texture_vertex_shader,
                                  rabbit::grayscale_texture_fragment_shader);

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glLineWidth(2);
}

void rabbit::opengl_drawer::create_buffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void rabbit::opengl_drawer::draw_triangles(float *vertices,
                                           int vertices_total,
                                           GLuint *triangles,
                                           int triangles_total)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices_stride * vertices_total * sizeof(GLfloat),
                 vertices,
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          vertices_stride * sizeof(GLfloat),
                          (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 3 * sizeof(GLuint) * triangles_total,
                 triangles,
                 GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES,
                   3 * sizeof(GLuint) * triangles_total,
                   GL_UNSIGNED_INT,
                   NULL);

    glBindVertexArray(0);
}

void rabbit::opengl_drawer::draw_triangles(const std::vector<vec3> &vertices,
                                           const std::vector<ivec3> &triangles)
{
    draw_triangles((float *)vertices.data(),
                   vertices.size(),
                   (GLuint *)triangles.data(),
                   triangles.size());
}

void rabbit::opengl_drawer::draw_lines(float *vertices, int vertices_total)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices_stride * vertices_total * sizeof(GLfloat),
                 vertices,
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,
                          vertices_stride,
                          GL_FLOAT,
                          GL_FALSE,
                          vertices_stride * sizeof(GLfloat),
                          (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINES, 0, vertices_total);

    glBindVertexArray(0);
}

void rabbit::opengl_drawer::draw_lines(const std::vector<vec3f> &vertices)
{
    draw_lines((float *)vertices.data(), vertices.size());
}

void rabbit::opengl_drawer::destroy_opengl_context() {}

void rabbit::opengl_drawer::clean(real r, real g, real b, real a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rabbit::opengl_drawer::draw_mesh(const rabbit::mesh<float> &mesh,
                                      const mat4f &model,
                                      const mat4f &view,
                                      const mat4f &projection,
                                      const vec4f &color)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 mesh.vertices().size() * sizeof(float) * 3,
                 mesh.vertices().data(),
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mesh.triangles().size() * sizeof(int) * 3,
                 mesh.triangles().data(),
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint vertexColorLocation =
        glGetUniformLocation(opengl_mesh_program.id(), "vertexColor");
    opengl_mesh_program.use();

    uniform_mat4f("model", opengl_mesh_program.id(), model);
    uniform_mat4f("view", opengl_mesh_program.id(), view);
    uniform_mat4f("projection", opengl_mesh_program.id(), projection);

    glBindVertexArray(VAO);
    glEnable(GL_POLYGON_OFFSET_FILL);

    glPolygonOffset(1, 1);
    glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);
    glDrawElements(GL_TRIANGLES,
                   mesh.triangles().size() * sizeof(int) * 3,
                   GL_UNSIGNED_INT,
                   0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void rabbit::opengl_drawer::draw_mesh_edges(
    std::vector<linalg::vec<float, 3>> vertices,
    std::vector<linalg::vec<unsigned int, 2>> edges,
    const mat4f &model,
    const mat4f &view,
    const mat4f &projection,
    const vec4f &color)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float) * 3,
                 vertices.data(),
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 edges.size() * sizeof(int) * 2,
                 edges.data(),
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint vertexColorLocation =
        glGetUniformLocation(opengl_mesh_program.id(), "vertexColor");
    opengl_mesh_program.use();

    uniform_mat4f("model", opengl_mesh_program.id(), model);
    uniform_mat4f("view", opengl_mesh_program.id(), view);
    uniform_mat4f("projection", opengl_mesh_program.id(), projection);

    glBindVertexArray(VAO);
    glEnable(GL_POLYGON_OFFSET_FILL);

    glPolygonOffset(1, 1);
    glUniform4f(vertexColorLocation, color[0], color[1], color[2], color[3]);

    glDrawElements(
        GL_LINES, edges.size() * sizeof(int) * 2, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void rabbit::opengl_drawer::draw_points(const vec3f *pnts,
                                        int count,
                                        const mat4f &model,
                                        const mat4f &view,
                                        const mat4f &projection)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, count * sizeof(float) * 3, pnts, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint vertexColorLocation =
        glGetUniformLocation(opengl_mesh_program.id(), "vertexColor");
    opengl_mesh_program.use();

    uniform_mat4f("model", opengl_mesh_program.id(), model);
    uniform_mat4f("view", opengl_mesh_program.id(), view);
    uniform_mat4f("projection", opengl_mesh_program.id(), projection);

    glBindVertexArray(VAO);
    glEnable(GL_POLYGON_OFFSET_FILL);

    glPolygonOffset(1, 1);
    glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_POINTS, 0, count);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonOffset(0, 0);
    // glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    // glDrawElements(GL_TRIANGLES, mesh.triangles.size()*sizeof(int) * 3,
    // GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void rabbit::opengl_drawer::uniform_mat4f(
    const char *locname, int program, const linalg::mat<float, 4, 4> &matrix)
{
    GLint modelLoc = glGetUniformLocation(program, locname);
    GLCHECK(glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)&matrix));
}

void rabbit::opengl_drawer::uniform_mat4f(const char *locname,
                                          int program,
                                          const GLfloat *data)
{
    GLint modelLoc = glGetUniformLocation(program, locname);
    GLCHECK(glUniformMatrix4fv(modelLoc, 1, GL_FALSE, data));
}

void rabbit::opengl_drawer::uniform_mat4f(
    unsigned int location, const linalg::mat<float, 4, 4> &matrix)
{
    GLCHECK(glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat *)&matrix));
}

void rabbit::opengl_drawer::uniform_mat4f(unsigned int location,
                                          const GLfloat *data)
{
    GLCHECK(glUniformMatrix4fv(location, 1, GL_FALSE, data));
}

void rabbit::opengl_drawer::draw_onecolored_texture_2d(
    const std::vector<std::pair<linalg::vec<float, 3>, linalg::vec<float, 2>>>
        &vertices,
    const std::vector<ivec3> triangles,
    const rabbit::opengl_texture &texture,
    const linalg::vec<float, 3> &color,
    const linalg::mat<float, 4, 4> &transform)
{
    GLCHECK(glBindVertexArray(VAO));

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER,
                         vertices.size() * sizeof(float) * 5,
                         vertices.data(),
                         GL_DYNAMIC_DRAW));

    GLCHECK(glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0));
    GLCHECK(glVertexAttribPointer(1,
                                  2,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  5 * sizeof(GLfloat),
                                  (GLvoid *)(3 * sizeof(GLfloat))));
    GLCHECK(glEnableVertexAttribArray(0));
    GLCHECK(glEnableVertexAttribArray(1));

    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         triangles.size() * sizeof(int) * 3,
                         triangles.data(),
                         GL_DYNAMIC_DRAW));

    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    opengl_onecolored_texture.use();
    opengl_onecolored_texture.uniform_mat4f("transform", transform);
    texture.activate(opengl_onecolored_texture.id(), "ourTexture", 0);
    opengl_onecolored_texture.uniform_vec3f("textColor", color);

    GLCHECK(glBindVertexArray(VAO));
    GLCHECK(glDrawElements(
        GL_TRIANGLES, triangles.size() * sizeof(int) * 3, GL_UNSIGNED_INT, 0));

    GLCHECK(glBindVertexArray(0));
    GLCHECK(glUseProgram(0));
}

void rabbit::opengl_drawer::draw_rgb_texture_2d(
    const std::vector<std::pair<linalg::vec<float, 3>, linalg::vec<float, 2>>>
        &vertices,
    const std::vector<ivec3> triangles,
    const rabbit::opengl_texture &texture,
    const linalg::mat<float, 4, 4> &transform)
{
    GLCHECK(glBindVertexArray(VAO));

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER,
                         vertices.size() * sizeof(float) * 5,
                         vertices.data(),
                         GL_DYNAMIC_DRAW));

    GLCHECK(glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0));
    GLCHECK(glVertexAttribPointer(1,
                                  2,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  5 * sizeof(GLfloat),
                                  (GLvoid *)(3 * sizeof(GLfloat))));
    GLCHECK(glEnableVertexAttribArray(0));
    GLCHECK(glEnableVertexAttribArray(1));

    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         triangles.size() * sizeof(int) * 3,
                         triangles.data(),
                         GL_DYNAMIC_DRAW));

    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    opengl_rgb_texture.use();
    opengl_rgb_texture.uniform_mat4f("transform", transform);
    texture.activate(opengl_rgb_texture.id(), "ourTexture", 0);

    GLCHECK(glBindVertexArray(VAO));
    GLCHECK(glDrawElements(
        GL_TRIANGLES, triangles.size() * sizeof(int) * 3, GL_UNSIGNED_INT, 0));

    GLCHECK(glBindVertexArray(0));
    GLCHECK(glUseProgram(0));
}

void rabbit::opengl_drawer::draw_grayscale_texture(
    const std::vector<std::pair<linalg::vec<float, 3>, linalg::vec<float, 2>>>
        &vertices,
    const std::vector<ivec3> triangles,
    const rabbit::opengl_texture &texture,
    const linalg::mat<float, 4, 4> &transform)
{
    GLCHECK(glBindVertexArray(VAO));

    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER,
                         vertices.size() * sizeof(float) * 5,
                         vertices.data(),
                         GL_DYNAMIC_DRAW));

    GLCHECK(glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0));
    GLCHECK(glVertexAttribPointer(1,
                                  2,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  5 * sizeof(GLfloat),
                                  (GLvoid *)(3 * sizeof(GLfloat))));
    GLCHECK(glEnableVertexAttribArray(0));
    GLCHECK(glEnableVertexAttribArray(1));

    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GLCHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         triangles.size() * sizeof(int) * 3,
                         triangles.data(),
                         GL_DYNAMIC_DRAW));

    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    opengl_grayscale_texture.use();
    opengl_grayscale_texture.uniform_mat4f("transform", transform);
    texture.activate(opengl_grayscale_texture.id(), "ourTexture", 0);

    GLCHECK(glBindVertexArray(VAO));
    GLCHECK(glDrawElements(
        GL_TRIANGLES, triangles.size() * sizeof(int) * 3, GL_UNSIGNED_INT, 0));

    GLCHECK(glBindVertexArray(0));
    GLCHECK(glUseProgram(0));
}

void rabbit::opengl_drawer::print_text(const rabbit::font &font,
                                       rabbit::textzone_cursor &cursor,
                                       const std::string_view &view,
                                       const linalg::vec<float, 3> &color,
                                       const linalg::mat<float, 4, 4> &matrix)
{
    for (unsigned int i = 0; i < view.size(); ++i)
    {
        char c = view[i];
        auto &texture = font[c];
        auto cell = cursor.get();

        draw_onecolored_texture_2d(
            {
                {cell.ll, {0, 0}},
                {cell.lh, {0, 1}},
                {cell.hl, {1, 0}},
                {cell.hh, {1, 1}},
            },
            {{0, 1, 2}, {1, 3, 2}},
            texture,
            color,
            matrix);

        cursor.increment();
    }
}

void rabbit::opengl_drawer::draw_points2d(
    const igris::array_view<vec2f> &points, GLint style)
{
    int stride = 2 * sizeof(GLfloat);
    int total = points.size();
    opengl_simple_2d_program.use();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, stride * total, points.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(style, 0, total);
    glBindVertexArray(0);
    glUseProgram(0);
}

void rabbit::opengl_drawer::draw_points3d(
    const igris::array_view<vec3f> &points, GLint style)
{
    int stride = 3 * sizeof(GLfloat);
    int total = points.size();
    opengl_simple_program.use();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, stride * total, points.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(style, 0, total);
    glBindVertexArray(0);
    glUseProgram(0);
}

void rabbit::opengl_drawer::draw_points3d(
    const igris::array_view<rabbit::vec3f> &points,
    GLint style,
    const rabbit::mat4f &model,
    const rabbit::mat4f &view,
    const rabbit::mat4f &projection)
{
    int count = points.size();
    auto pnts = points.data();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, count * sizeof(float) * 3, pnts, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GLint vertexColorLocation =
        glGetUniformLocation(opengl_mesh_program.id(), "vertexColor");
    opengl_mesh_program.use();
    uniform_mat4f("model", opengl_mesh_program.id(), model);
    uniform_mat4f("view", opengl_mesh_program.id(), view);
    uniform_mat4f("projection", opengl_mesh_program.id(), projection);
    glBindVertexArray(VAO);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1, 1);
    glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    glDrawArrays(style, 0, count);
    glBindVertexArray(0);
    glUseProgram(0);
}

void rabbit::opengl_drawer::draw_line(linalg::vec<float, 3> p1,
                                      linalg::vec<float, 4> c1,
                                      linalg::vec<float, 3> p2,
                                      linalg::vec<float, 4> c2,
                                      const rabbit::mat4f &model,
                                      const rabbit::mat4f &view,
                                      const rabbit::mat4f &projection)
{
    int count = 2;
    std::pair<linalg::vec<float, 3>, linalg::vec<float, 4>> pnts[] = {{p1, c1},
                                                                      {p2, c2}};
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 count * sizeof(float) * (3 + 4),
                 pnts,
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *)0);
    glVertexAttribPointer(
        1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid *)12);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    opengl_colored_mesh_program.use();
    uniform_mat4f("model", opengl_colored_mesh_program.id(), model);
    uniform_mat4f("view", opengl_colored_mesh_program.id(), view);
    uniform_mat4f("projection", opengl_colored_mesh_program.id(), projection);
    glBindVertexArray(VAO);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1, 1);
    glDrawArrays(GL_LINES, 0, count);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    GLCHECK(glUseProgram(0));
}