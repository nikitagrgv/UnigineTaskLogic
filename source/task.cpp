#include "task.h"

#include<x86intrin.h>

struct Vector2D
{
    float x;
    float y;

    Vector2D(float x, float y) : x(x), y(y)
    {}

    Vector2D(const Point& a, const Point& b) : x(b.x - a.x), y(b.y - a.y)
    {}

    Vector2D getNormal() const
    {
        return {-y, x};
    }

    // projection of vector a onto vector b = (a*b)/|b|
    // pseudo projection removes division on |b|
    // so result of this function is a projection, multiplied by some constant (|b|)
    // this multiplication doesn't affect further calculation
    float getPseudoProjection(const Vector2D& projected_vector) const
    {
        return dotProduct(*this, projected_vector);
    }

    static float dotProduct(const Vector2D& vec1, const Vector2D& vec2)
    {
        return vec1.x * vec2.x +
               vec1.y * vec2.y;
    }
};

// shadow is the borders of the projection of the triangle on a given vector
class Shadow
{
    float _begin;
    float _end;

    Shadow(float begin, float end) : _begin(begin), _end(end)
    {
    }

public:
    static Shadow fromProjectedPoints(float p1, float p2)
    {
        return {
                std::min(p1, p2),
                std::max(p1, p2)
        };
    }

    static Shadow fromProjectedPoints(float p1, float p2, float p3)
    {
        return {
                std::min(std::min(p1, p2), p3),
                std::max(std::max(p1, p2), p3)
        };
    }

    static bool isIntersect(const Shadow& shadow1, const Shadow& shadow2)
    {
        return (shadow1._begin <= shadow2._end) && (shadow1._end >= shadow2._begin);
    }
};

// find shadows of tri1 and tri2, projected on the vector in tri1, and check if they intersect
// shadows are calculated relatively to the point side_begin
bool isIntersectRelativelyToSide(const Point& side_begin, const Point& side_end,
                                 const Point& last_point_of_triangle, const Triangle& tri2)
{
    Vector2D vector(side_begin, side_end);
    Vector2D normal = vector.getNormal();

    float projection_of_third_point = normal.getPseudoProjection({side_begin, last_point_of_triangle});

    // we don't need to find projection of side_begin and side_end because they are both zero

    auto shadow_tri1 = Shadow::fromProjectedPoints(0, projection_of_third_point);

    auto projection_tri2_a = normal.getPseudoProjection({side_begin, tri2.a});
    auto projection_tri2_b = normal.getPseudoProjection({side_begin, tri2.b});
    auto projection_tri2_c = normal.getPseudoProjection({side_begin, tri2.c});

    auto shadow_tri2 = Shadow::fromProjectedPoints(projection_tri2_a,
                                                   projection_tri2_b,
                                                   projection_tri2_c);

    return Shadow::isIntersect(shadow_tri1, shadow_tri2);
}


// check if shadows of the triangles intersects when projected on normals of all sides of tri1
bool isIntersectRelativelyToFirstTriangle(const Triangle& tri1, const Triangle& tri2)
{
    if (!isIntersectRelativelyToSide(tri1.a, tri1.b, tri1.c, tri2))
    {
        return false;
    }

    if (!isIntersectRelativelyToSide(tri1.b, tri1.c, tri1.a, tri2))
    {
        return false;
    }

    if (!isIntersectRelativelyToSide(tri1.c, tri1.a, tri1.b, tri2))
    {
        return false;
    }

    return true;
}


bool isIntersect(const Triangle& tri1, const Triangle& tri2)
{
    if (!isIntersectRelativelyToFirstTriangle(tri1, tri2))
    {
        return false;
    }

    if (!isIntersectRelativelyToFirstTriangle(tri2, tri1))
    {
        return false;
    }

    return true;
}


void Task::checkIntersections(const std::vector<Triangle>& in_triangles, std::vector<int>& out_count)
{
    const auto triangles_count = in_triangles.size();

    out_count.resize(triangles_count, 0);

    for (int i = 0; i < triangles_count - 1; ++i)
    {
        for (int j = i + 1; j < triangles_count; ++j)
        {
            const auto& tri1 = in_triangles[i];
            const auto& tri2 = in_triangles[j];

            bool is_intersect = isIntersect(tri1, tri2);

            out_count[i] += is_intersect;
            out_count[j] += is_intersect;
        }
    }
}
