
#include "rabbit/mesh/mesh.h"

std::vector<linalg::vec<unsigned int, 2>> rabbit::edges_from_triangles(
    const std::vector<linalg::vec<unsigned int, 3>> &triangles)
{
    std::vector<linalg::vec<unsigned int, 2>> edges;

    for (auto &t : triangles)
    {
        edges.emplace_back(t.x, t.y);
        edges.emplace_back(t.y, t.z);
        edges.emplace_back(t.z, t.x);
    }

    std::sort(edges.begin(), edges.end());
    edges.erase(std::unique(edges.begin(), edges.end()), edges.end());

    return edges;
}