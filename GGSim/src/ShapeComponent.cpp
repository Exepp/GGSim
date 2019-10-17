#include <GGSim/ShapeComponent.h>


ShapeComponent::ShapeComponent()
    : cObj(std::make_unique<CObj_t>())
{
    cObj->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    setShape(Sphere(1.f));
}

void ShapeComponent::setTransform(Transform const& t)
{
    cObj->setWorldTransform(toCTransform(t));
}

bool ShapeComponent::isShapeChanged() const
{
    return changedShape;
}

bool ShapeComponent::confirmShapeChange()
{
    bool temp    = changedShape;
    changedShape = false;
    return temp;
}

ShapeComponent::CObj_t* ShapeComponent::getCollisionObj()
{
    return cObj.get();
}

ShapeComponent::CObj_t const* ShapeComponent::getCollisionObj() const
{
    return cObj.get();
}

Shape* ShapeComponent::getShape()
{
    return shape.get();
}

Shape const* ShapeComponent::getShape() const
{
    return shape.get();
}