#ifndef SHAPE_H
#define SHAPE_H

#include <GGSim/Transformable.h>
#include <GGSim/VertexArray.h>
#include <bullet3/btBulletCollisionCommon.h>
#include <memory>

class WindowModule;


struct Drawable
{
    virtual ~Drawable() = default;

    virtual void draw(WindowModule& win, Transform model = Transform()) const = 0;
};


//
//////////////////////////////// Shape ////////////////////////////////
//


class Shape : public Drawable
{
protected:
    using VertArr_t = VertexArray;

    using CSBase_t = btCollisionShape;

public:
    RULEOF5_BAN_COPY(Shape)
    virtual ~Shape() = default;


    virtual void draw(WindowModule& win, Transform model = Transform()) const override;


    VertArr_t& getVertexArray();

    VertArr_t const& getVertexArray() const;

    CSBase_t* getCollisionShape() { return const_cast<CSBase_t*>(const_cast<Shape const*>(this)->getCollisionShape()); }

    virtual CSBase_t const* getCollisionShape() const = 0;

    virtual Mat3_t computeInertiaTensorNoMass() const = 0;

protected:
    VertArr_t verts;

    friend class ShapeComponent;
};


//
//////////////////////////////// Box ////////////////////////////////
//


class Box : public Shape
{
protected:
    using CShape_t = btBoxShape;

public:
    explicit Box(Vec3_t const& size);

    virtual CSBase_t const* getCollisionShape() const override { return &cShape; }

    virtual Mat3_t computeInertiaTensorNoMass() const override;

private:
    Vec3_t const size;

    CShape_t cShape;
};


//
//////////////////////////////// Sphere ////////////////////////////////
//


class Sphere : public Shape
{
protected:
    using CShape_t = btSphereShape;

public:
    explicit Sphere(float radius);

    virtual CSBase_t const* getCollisionShape() const override { return &cShape; }

    virtual Mat3_t computeInertiaTensorNoMass() const override;

private:
    float const radius;

    CShape_t cShape;
};


//
//////////////////////////////// Polyhedron ////////////////////////////////
//


// class Polyhedron : public Shape
// {
// protected:
//     using CShape_t = rp3d::ConvexMeshShape;

//     using PolyhMesh_t = rp3d::PolyhedronMesh;

//     using CVertArr_t = rp3d::PolygonVertexArray;

//     using Face_t = CVertArr_t::PolygonFace;

//     using FaceArr_t = std::vector<Face_t>;

// public:
//     Polyhedron();

//     virtual CSBase_t const* getCollisionShape() const override { return &cShape; }

//     virtual Mat3_t computeInertiaTensorNoMass() const override;

// protected:
//     void updateCollisionArr();

//     virtual void updateFaces();

// private:
//     FaceArr_t faces;

//     CVertArr_t cvArr = CVertArr_t(0, nullptr, 0, nullptr, 0, 0, nullptr,
//                                   CVertArr_t::VertexDataType::VERTEX_FLOAT_TYPE,
//                                   CVertArr_t::IndexDataType::INDEX_INTEGER_TYPE);

//     PolyhMesh_t mesh = PolyhMesh_t(&cvArr);

//     CShape_t cShape = CShape_t(&mesh);
// };


#endif // SHAPE_H