#include <rabbit/mesh/mesh.h>

namespace rabbit
{
    rabbit::mesh<float> sphere_mesh(float rad, int x, int y);
    rabbit::mesh<float> cylinder_mesh(float r, float h, int x, int y);
    rabbit::mesh<float> box_mesh(float x, float y, float z);
}