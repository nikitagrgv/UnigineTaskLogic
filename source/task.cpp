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

    template<unsigned int T>
    static Shadow fromProjectedPoints(const std::array<float, T>& projected_points)
    {
        return {
                *std::min_element(projected_points.begin(), projected_points.end()),
                *std::max_element(projected_points.begin(), projected_points.end())
        };
    }

    static bool isShadowsIntersects(const Shadow& shadow1, const Shadow& shadow2)
    {
        return (shadow1.begin <= shadow2.end) && (shadow1.end >= shadow2.begin);
    }
};


bool isTrianglesIntersectsRelativelyToFirstTriangle(const Triangle& tri1, const Triangle& tri2)
{

    {
        Vector2D vector_AB(tri1.a, tri1.b);
        Vector2D normal_AB = vector_AB.getNormal();

        float projection_AC = normal_AB.projectVector({tri1.a, tri1.c});

        Shadow shadow_tri1 = Shadow::fromProjectedPoints<2>(std::array<float, 2>{0, projection_AC});

        float projection_tri2_A = normal_AB.projectVector({tri1.a, tri2.a});
        float projection_tri2_B = normal_AB.projectVector({tri1.a, tri2.b});
        float projection_tri2_C = normal_AB.projectVector({tri1.a, tri2.c});

        Shadow shadow_tri2 = Shadow::fromProjectedPoints<3>(std::array<float, 3>{projection_tri2_A,
                                                                                 projection_tri2_B,
                                                                                 projection_tri2_C});

        if (!Shadow::isShadowsIntersects(shadow_tri1, shadow_tri2))
        {
            return false;
        }
    }


    {
        Vector2D vector_BC(tri1.b, tri1.c);
        Vector2D normal_BC = vector_BC.getNormal();

        float projection_BA = normal_BC.projectVector({tri1.b, tri1.a});

        Shadow shadow_tri1 = Shadow::fromProjectedPoints<2>(std::array<float, 2>{0, projection_BA});

        float projection_tri2_A = normal_BC.projectVector({tri1.b, tri2.a});
        float projection_tri2_B = normal_BC.projectVector({tri1.b, tri2.b});
        float projection_tri2_C = normal_BC.projectVector({tri1.b, tri2.c});

        Shadow shadow_tri2 = Shadow::fromProjectedPoints<3>(std::array<float, 3>{projection_tri2_A,
                                                                                 projection_tri2_B,
                                                                                 projection_tri2_C});

        if (!Shadow::isShadowsIntersects(shadow_tri1, shadow_tri2))
        {
            return false;
        }
    }


    {
        Vector2D vector_CA(tri1.c, tri1.a);
        Vector2D normal_CA = vector_CA.getNormal();

        float projection_CB = normal_CA.projectVector({tri1.c, tri1.b});

        Shadow shadow_tri1 = Shadow::fromProjectedPoints<2>(std::array<float, 2>{0, projection_CB});

        float projection_tri2_A = normal_CA.projectVector({tri1.c, tri2.a});
        float projection_tri2_B = normal_CA.projectVector({tri1.c, tri2.b});
        float projection_tri2_C = normal_CA.projectVector({tri1.c, tri2.c});

        Shadow shadow_tri2 = Shadow::fromProjectedPoints<3>(std::array<float, 3>{projection_tri2_A,
                                                                                 projection_tri2_B,
                                                                                 projection_tri2_C});

        if (!Shadow::isShadowsIntersects(shadow_tri1, shadow_tri2))
        {
            return false;
        }
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
    out_count.resize(in_triangles.size(), 0);

    for (int i = 0; i < in_triangles.size() - 1; ++i)
    {
        for (int j = i + 1; j < in_triangles.size(); ++j)
        {
            auto tri1 = in_triangles[i];
            auto tri2 = in_triangles[j];

            bool is_intersects = isTrianglesIntersects(tri1, tri2);

            std::cout << i << " " << j << " = " << is_intersects << std::endl;

            out_count[i] += is_intersects;
            out_count[j] += is_intersects;
        }
    }
}
