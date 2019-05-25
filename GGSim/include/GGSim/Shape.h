#ifndef SHAPE_H
#define SHAPE_H

#include <GGSim/Transformable.h>
#include <GGSim/VertexArray.h>

class WindowModule;


struct Drawable
{
    virtual ~Drawable() = default;

    virtual void draw(WindowModule& win, Transform model = Transform()) const = 0;
};


class Shape : public Transformable, public Drawable
{
    using VertArr_t = VertexArray;

public:
    virtual void draw(WindowModule& win, Transform model = Transform()) const override;

protected:
    VertArr_t verts;
};


struct Box : public Shape
{
    Box(Vec3_t const& size);

    Vec3_t const size;
};

struct Sphere : public Shape
{
    Sphere(float radius);

    float const radius;
};

#endif // SHAPE_H