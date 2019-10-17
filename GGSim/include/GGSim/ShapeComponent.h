#ifndef SHAPECOMPONENT_H
#define SHAPECOMPONENT_H

#include <ECSpp/Component.h>
#include <GGSim/Shape.h>


class ShapeComponent : public epp::Component
{
private:
    using CObj_t     = btCollisionObject;
    using CObjPtr_t  = std::unique_ptr<CObj_t>;
    using ShapePtr_t = std::unique_ptr<Shape>;

public:
    ShapeComponent();
    RULEOF4_BAN_COPY(ShapeComponent)

    template<class T>
    void setShape(T newShape);
    void setTransform(Transform const& t);

    bool confirmShapeChange(); // sets changedShape to false, returns previous value
    bool isShapeChanged() const;

    CObj_t*       getCollisionObj();
    CObj_t const* getCollisionObj() const;

    Shape*       getShape();
    Shape const* getShape() const;

private:
    ShapePtr_t shape;

    CObjPtr_t cObj;

    bool changedShape = false;
};


template<class T>
void ShapeComponent::setShape(T newShape)
{
    shape = std::make_unique<T>(std::move(newShape));
    cObj->setCollisionShape(shape->getCollisionShape());
    changedShape = true;
}

#endif // SHAPECOMPONENT_H