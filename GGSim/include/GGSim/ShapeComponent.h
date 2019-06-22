#ifndef SHAPE_H
#define SHAPE_H

#include <GGSim/TransformComponent.h>
#include <GGSim/VertexArray.h>
#include <memory>

class WindowModule;


struct Drawable
{
    virtual ~Drawable() = default;

    virtual void draw(WindowModule& win, Transform model = Transform()) const = 0;
};


class Shape : public Drawable
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


struct ShapeComponent : public epp::Component
{
    template<class T>
    using UniqPtr_t = std::unique_ptr<T>;

    using ShapePtr_t = UniqPtr_t<Shape>;

    template<class T>
    void setShape(T newShape)
    {
        shape = std::make_unique<T>(std::move(newShape));
    }

    ShapePtr_t shape;
};

#endif // SHAPE_H