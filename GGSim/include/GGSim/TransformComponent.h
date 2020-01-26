#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <ECSpp/Component.h>
#include <GGSim/Transformable.h>


struct TransformComponent : public epp::Component, public Transformable
{};

#endif // TRANSFORMCOMPONENT_H