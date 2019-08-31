/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SMALL_ROCKET_ENTITY_H
#define SMALL_ROCKET_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/MeshEntity.h>

namespace env
{
    struct Bullet
    {
        Bullet()
            : lifeTime(0.0f),
            speed(0.0f),
            rotationSpeed(0.0f),
            numBounces(0) {}

        Bullet(const std::string& strFileName, float lifeTime, float speed, float rotationSpeed, unsigned int numBounces)
            : strFile(strFileName),
            lifeTime(lifeTime),
            speed(speed),
            rotationSpeed(rotationSpeed),
            numBounces(numBounces) {}

        std::string strFile;
        float lifeTime;
        float speed;
        float rotationSpeed;
        unsigned int numBounces;
    };


    enum BULLET_TYPE
    {
        BULLET_TYPE_ROCKET_SMALL = 0,
        BULLET_TYPE_ROCKET_BIG,
        BULLET_TYPE_BOUNCING,
        BULLET_TYPE_FIREBALL,
        BULLET_TYPE_END
    };

    class CSmallRocketEntity : public CMeshEntity
    {
    public:
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        static std::vector< Bullet > m_bulletTypes;

        CSmallRocketEntity();
        virtual ~CSmallRocketEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual bool HasCollision(CEntity& e);

        void SetBulletType(BULLET_TYPE type);
        BULLET_TYPE GetBulletType() const { return m_bulletType; }

        void SetTarget(CEntity* pNode) { m_pTarget = pNode; }
        CEntity* GetTarget() const { return m_pTarget; }

    private:

        float m_currentLifeTime;

        CEntity* m_pTarget;

        BULLET_TYPE m_bulletType;
    };
} // env
#endif // SMALL_ROCKET_ENTITY_H