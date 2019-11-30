#include <GGSim/Application.h>
#include <GGSim/PhysicsSystem.h>


float const PhysicsSystem::GravitationalAcc(9.80665f);

static CVec3_t const  sceneSize(5e3f, 5e3f, 5e3f);
static uint32_t const maxObjNum(2e4);


PhysicsSystem::PhysicsSystem()
    : cfg()
    , disp(&cfg)
    , broadPh(-sceneSize, sceneSize, maxObjNum, nullptr, true) // true for disabling raycast accelerator)
    , cWorld(&disp, &broadPh, &cfg)
{
}


void PhysicsSystem::init(epp::EntityManager& entityManager)
{
    entityManager.requestCGroup(entities);
    entities.addSubscriber(this, &PhysicsSystem::entityCreationCallback, epp::SpawnerNotifier_t::EvType_t::Creation);
    entities.addSubscriber(this, &PhysicsSystem::entityDestructionCallback, epp::SpawnerNotifier_t::EvType_t::Destruction);
}

void PhysicsSystem::entityCreationCallback(epp::EntityEvent& event)
{
    auto* obj = event.entity.getComponent<ShapeComponent>()->getCollisionObj();
    obj->setUserPointer(event.entity.getEntityPtr().get());
    cWorld.addCollisionObject(obj);
}

void PhysicsSystem::entityDestructionCallback(epp::EntityEvent& event)
{
    cWorld.removeCollisionObject(event.entity.getComponent<ShapeComponent>()->getCollisionObj());
}

void PhysicsSystem::preUpdate(epp::EntityManager& entityManager, float dt, bool catchUpTick)
{
    for (auto entity : entities)
    {
        TransformComponent& transfC = entity.get<TransformComponent&>();
        PhysicsComponent&   phyC    = entity.get<PhysicsComponent&>();
        ShapeComponent&     shapeC  = entity.get<ShapeComponent&>();

        shapeC.setTransform(transfC);

        if (shapeC.confirmShapeChange())
        {
            phyC.setInertiaTensor(shapeC.getShape()->computeInertiaTensorNoMass());
            // cWorld.setForceUpdateAllAabbs(false);
            cWorld.updateSingleAabb(shapeC.getCollisionObj());
        }
    }
}

void PhysicsSystem::update(epp::EntityManager& entityManager, float dt, bool catchUpTick)
{
    applyGravity();
    resolveCollisions();
    apply(dt);
}

void PhysicsSystem::applyGravity()
{
    for (auto entity : entities)
    {
        TransformComponent& transfC = entity.get<TransformComponent&>();
        PhysicsComponent&   phyC    = entity.get<PhysicsComponent&>();

        phyC.applyForce(Vec3_t(0, -GravitationalAcc * phyC.mass(), 0));
    }
}

void PhysicsSystem::resolveCollisions()
{
    cWorld.performDiscreteCollisionDetection();

    size_t manifNum = cWorld.getDispatcher()->getNumManifolds();
    for (int i = 0; i < manifNum; ++i)
    {
        btPersistentManifold* mfld = cWorld.getDispatcher()->getManifoldByIndexInternal(i);
        mfld->refreshContactPoints(mfld->getBody0()->getWorldTransform(), mfld->getBody1()->getWorldTransform());
        size_t cNum = mfld->getNumContacts();
        if (cNum == 0)
            continue;

        epp::EntityRef      e1(((epp::Entity*)(mfld->getBody0()->getUserPointer()))->shared_from_this());
        epp::EntityRef      e2(((epp::Entity*)(mfld->getBody1()->getUserPointer()))->shared_from_this());
        TransformComponent& transfC1 = *e1.getComponent<TransformComponent>();
        TransformComponent& transfC2 = *e2.getComponent<TransformComponent>();
        PhysicsComponent&   phyC1    = *e1.getComponent<PhysicsComponent>();
        PhysicsComponent&   phyC2    = *e2.getComponent<PhysicsComponent>();

        Vec3_t maxPenN(0, 0, 0);
        float  maxPen = 0;
        for (int j = 0; j < cNum; ++j)
        {
            btManifoldPoint const& pt = mfld->getContactPoint(j);
            if (maxPen > pt.getDistance())
            {
                maxPen  = pt.getDistance();
                maxPenN = toVec3(pt.m_normalWorldOnB);
            }
        }
        float vrn1 = glm::dot(phyC1.vel, maxPenN);
        float vrn2 = glm::dot(phyC2.vel, maxPenN);

        if (phyC1.mass() != INFINITY && (-vrn1 >= vrn2 || phyC2.mass() == INFINITY))
            transfC1.pos += maxPenN * (-std::min(maxPen + 0.01f, 0.f));
        else if (phyC2.mass() != INFINITY)
            transfC2.pos += maxPenN * (std::min(maxPen + 0.01f, 0.f));
        else
            continue;

        // for (int k = 0; k < 8; ++k)
        {
            for (int j = 0; j < cNum; ++j)
            {
                btManifoldPoint const& pt = mfld->getContactPoint(j);
                Vec3_t                 n(0, 0, 0);
                Vec3_t                 r1(0, 0, 0);
                Vec3_t                 r2(0, 0, 0);

                n  = toVec3(pt.m_normalWorldOnB);
                r1 = transfC1.orient * toVec3(pt.m_localPointA);
                r2 = transfC2.orient * toVec3(pt.m_localPointB);

                Vec3_t v1  = phyC1.velocityAt(r1);
                Vec3_t v2  = phyC2.velocityAt(r2);
                Vec3_t vr  = v1 - v2;
                float  vrn = glm::dot(n, vr);

                if (vrn > 0)
                    continue;

                Vec3_t t;
                if (glm::length(vr - vrn * n) > 0.001f)
                {
                    t = vr - vrn * n;
                    t = glm::normalize(t);
                }
                else if (glm::length(phyC2.force - glm::dot(phyC2.force, n) * n) > 0.001f)
                {
                    t = phyC2.force - glm::dot(phyC2.force, n) * n;
                    t = glm::normalize(t);
                }
                else
                    t = Vec3_t(0, 0, 0);

                Mat3_t inertiaTInvW1 = glm::toMat3(transfC1.orient) *
                                       phyC1.tensorInversed() *
                                       glm::transpose(glm::toMat3(transfC1.orient));
                Mat3_t inertiaTInvW2 = glm::toMat3(transfC2.orient) *
                                       phyC2.tensorInversed() *
                                       glm::transpose(glm::toMat3(transfC2.orient));

                float e  = (phyC1.restitution + phyC2.restitution) * 0.5f;
                float uD = (phyC1.dyFrCo + phyC2.dyFrCo) * 0.5f;
                float uS = (phyC1.stFrCo + phyC2.stFrCo) * 0.5f;

                float jr = -(1.f + e) * vrn /
                           (phyC1.massInversed() + phyC2.massInversed() +
                            glm::dot(n, glm::cross(inertiaTInvW1 * glm::cross(r1, n), r1) + glm::cross(inertiaTInvW2 * glm::cross(r2, n), r2)));

                // jr /= cNum ;
                float jt = 0;
                if (phyC2.mass() * glm::dot(vr, t) <= jr * uS)
                    jt = -phyC2.mass() * glm::dot(vr, t);
                else
                    jt = -uD * jr;

                if (phyC1.mass() != INFINITY)
                {
                    phyC1.vel += phyC1.massInversed() * (jr * n + jt * t);
                    phyC1.angularVel += jr * inertiaTInvW1 * glm::cross(r1, n);
                    phyC1.angularVel += jt * inertiaTInvW1 * glm::cross(r1, t);
                }

                if (phyC2.mass() != INFINITY)
                {
                    phyC2.vel -= phyC2.massInversed() * (jr * n + jt * t);
                    phyC2.angularVel -= jr * inertiaTInvW2 * glm::cross(r2, n);
                    phyC2.angularVel -= jt * inertiaTInvW2 * glm::cross(r2, t);
                }
            }
        }
    }
}

void PhysicsSystem::apply(float dt)
{
    for (auto entity : entities)
    {
        TransformComponent& transfC = entity.get<TransformComponent&>();
        PhysicsComponent&   phyC    = entity.get<PhysicsComponent&>();

        if (phyC.mass() == INFINITY)
            continue;

        phyC.acc = phyC.force * phyC.massInversed();
        phyC.vel += phyC.acc * dt;
        transfC.pos += phyC.vel * dt;

        phyC.angularAcc = glm::toMat3(transfC.orient) * phyC.tensorInversed() *
                          glm::toMat3(glm::inverse(transfC.orient)) * phyC.torque;
        phyC.angularVel += phyC.angularAcc * dt;
        transfC.rotateGlobal(glm::length(phyC.angularVel) * dt, phyC.angularVel);
    }
}

void PhysicsSystem::postUpdate(epp::EntityManager& entityManager, float dt, bool catchUpTick)
{
    for (auto entity : entities)
        entity.get<PhysicsComponent&>().resetForNextTick();
}

void PhysicsSystem::drawContactPoints()
{
    for (int i = 0; i < disp.getNumManifolds(); ++i)
    {
        btPersistentManifold* mfld = disp.getManifoldByIndexInternal(i);
        // mfld->refreshContactPoints(mfld->getBody0()->getWorldTransform(), mfld->getBody1()->getWorldTransform());
        size_t cNum = mfld->getNumContacts();
        if (cNum == 0)
            continue;

        epp::EntityRef      e1(((epp::Entity*)(mfld->getBody0()->getUserPointer()))->shared_from_this());
        epp::EntityRef      e2(((epp::Entity*)(mfld->getBody1()->getUserPointer()))->shared_from_this());
        TransformComponent& transfC1 = *e1.getComponent<TransformComponent>();
        TransformComponent& transfC2 = *e2.getComponent<TransformComponent>();

        for (int j = 0; j < cNum; ++j)
        {
            btManifoldPoint const& pt = mfld->getContactPoint(j);

            Vec3_t pos = transfC1.pos + transfC1.orient * toVec3(pt.m_localPointA);
            App::getModules().windowM.drawPoint(Vertex(pos, { 0, 1, 1, 0 }), 0.2f);

            pos = transfC2.pos + transfC2.orient * toVec3(pt.m_localPointB);
            App::getModules().windowM.drawPoint(Vertex(pos, { 1, 1, 0, 0 }), 0.1f);
        }
    }
}
