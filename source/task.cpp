#include "task.h"

#include <array>
#include <algorithm>
#include <iostream>

struct Vector2D
{
    float x;
    float y;

    Vector2D(float x, float y) : x(x), y(y)
    {}

    Vector2D(const Point& a, const Point& b) : x(a.x - b.x), y(a.y - b.y)
    {}

    Vector2D getNormal() const
    {
        return {-y, x};
    }

    float projectVector(const Vector2D& projected_vector) const
    {
        return dotProduct(*this, projected_vector);
    }

    static float dotProduct(const Vector2D& vec1, const Vector2D& vec2)
    {
        return vec1.x * vec2.x +
               vec1.y * vec2.y;
    }

};

struct Shadow
{
    Shadow() = delete;

    float begin;
    float end;

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


    static bool isShadowsIntersects(const Shadow& shadow1, const Shadow& shadow2)
    {
        return (shadow1.begin <= shadow2.end) && (shadow1.end >= shadow2.begin);
    }
};


bool isShadowsIntersectsForVectorInTriangle(const Point& vector_begin, const Point& vector_end,
                                            const Point& third_point_of_triangle, const Triangle& tri2)
{
    Vector2D vector(vector_begin, vector_end);
    Vector2D normal = vector.getNormal();

    float projection_of_third_point = normal.projectVector({vector_begin, third_point_of_triangle});

    Shadow shadow_tri1 = Shadow::fromProjectedPoints(0, projection_of_third_point);

    float projection_tri2_A = normal.projectVector({vector_begin, tri2.a});
    float projection_tri2_B = normal.projectVector({vector_begin, tri2.b});
    float projection_tri2_C = normal.projectVector({vector_begin, tri2.c});

    Shadow shadow_tri2 = Shadow::fromProjectedPoints(projection_tri2_A,
                                                     projection_tri2_B,
                                                     projection_tri2_C);

    return Shadow::isShadowsIntersects(shadow_tri1, shadow_tri2);
}

bool isTrianglesIntersectsRelativelyToFirstTriangle(const Triangle& tri1, const Triangle& tri2)
{
    if (!isShadowsIntersectsForVectorInTriangle(tri1.a, tri1.b, tri1.c, tri2))
    {
        return false;
    }

    if (!isShadowsIntersectsForVectorInTriangle(tri1.b, tri1.c, tri1.a, tri2))
    {
        return false;
    }

    if (!isShadowsIntersectsForVectorInTriangle(tri1.c, tri1.a, tri1.b, tri2))
    {
        return false;
    }

    return true;
}


bool isTrianglesIntersects(const Triangle& tri1, const Triangle& tri2)
{
    if (!isTrianglesIntersectsRelativelyToFirstTriangle(tri1, tri2))
    {
        return false;
    }


    if (!isTrianglesIntersectsRelativelyToFirstTriangle(tri2, tri1))
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

            bool is_intersects = isTrianglesIntersects(tri1, tri2);

            out_count[i] += is_intersects;
            out_count[j] += is_intersects;
        }
    }
}
