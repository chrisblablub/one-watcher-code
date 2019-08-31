#include <Engine/Entity/GunEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/SmallRocketEntity.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Engine.h>

namespace env
{
    ENTITY_TYPEID CGunEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_GUN;

    //-----------------------------------------------------------------------------------
    CGunEntity::CGunEntity()
        : m_bGunEnabled(true)
        , m_bGunAlwaysEnabled(true)
        , m_timeRotationLocked(0.0f)
        , m_pTarget(NULL)
        , m_fireRate(1.0f)
        , m_radius(450.0f)
        , m_currentLaserLifetime(0.0f)
        , m_rotationSpeed(1.0f)
        , m_bulletType(BULLET_TYPE_ROCKET_SMALL)
        , m_bLaser(false)
        , m_laserType(LASER_TYPE_STANDARD)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Gun] Enable", GUN_EVENT_ENABLE);
        RegisterEvent("[Gun] Disable", GUN_EVENT_DISABLE);

        m_laserSprites.resize(LASER_NUM_BOUNCES);
    }

    //-----------------------------------------------------------------------------------
    CGunEntity::~CGunEntity()
    {
        for (int i = 0; i < m_laserSprites.size(); ++i)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_laserSprites[i]->GetName());
            m_laserSprites[i] = NULL;
        }
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("Gun01.x");

        CCircle c;
        c.SetRadius(m_radius);
        AddCircle(c);

        m_pNode->SetOrientation(CQuaternion(0.0f, 0.0f, PI));

        //SetBulletType(BULLET_TYPE_ROCKET_SMALL);
        SetLaserType(LASER_TYPE_STANDARD);
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        for (int i = 0; i < m_laserSprites.size(); ++i)
        {
            if (m_laserSprites[i])
            {
                m_laserSprites[i]->SetRender(false);
            }
        }

        m_bLaserActive = false;
        m_currentLaserLifetime = -1.0f;
        m_timeRotationLocked = 0.0f;
        m_bGunEnabled = true;
        m_bGunAlwaysEnabled = true;
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::OnSerialization(bool bLoad)
    {
        CMeshEntity::OnSerialization(bLoad);
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::SetRotationSpeed(float speed)
    {
        m_rotationSpeed = speed;
    }

    //-----------------------------------------------------------------------------------
    float CGunEntity::GetRotationSpeed()
    {
        return m_rotationSpeed;
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        /*ApplyTransform(m_pNode->GetDerivedRST());

        m_time += dt;
        m_fireAccum += dt;


        CCharacterEntity* pCharacter =
        static_cast< CCharacterEntity* >(gEngine->GetEntityMgr()->GetFirstEntityOfType(CCharacterEntity::ENTITY_META_TYPEID));
        if(!pCharacter || !m_pTarget)
        {
        return;
        }

        float distance = glm::length(pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition());
        if(distance > m_radius)
        {
        for(int i = 0; i < m_laserSprites.size(); ++i)
        {
        if(m_laserSprites[i])
        m_laserSprites[i]->SetRender(false);
        }
        return;
        }

        VECTOR3 vTarget = m_pTarget->GetNode()->GetDerivedPosition();



        if(!m_bLaser)
        {
        if(m_bTargetDetection && gEngine->GetWorldMgr()->GetLevel()->IsRayIntersecting(vTarget, m_pNode->GetDerivedPosition(), NULL))
        return;

        VECTOR3 vTargetDirection = vTarget - m_pNode->GetDerivedPosition();

        float maxRotationAngle = m_rotationSpeed * dt;

        #pragma message("[ERROR] RotateToDirection not implemented!")
        //m_pNode->RotateToDirection(vTargetDirection, m_rotationSpeed * dt);

        vTargetDirection = glm::normalize(vTargetDirection);
        float angle = acos(glm::dot(m_pNode->GetDerivedDirection(), vTargetDirection));
        if(abs(angle) < 0.1)
        {

        if(m_fireAccum >= (1.0f / m_fireRate))
        {
        // Emit new bullets
        //
        m_fireAccum = 0.0f;

        CSmallRocketEntity* pRocket =
        static_cast< CSmallRocketEntity* >(gEngine->GetEntityMgr()->CreateEntity(CSmallRocketEntity::ENTITY_META_TYPEID));
        pRocket->GetNode()->SetPosition(m_pNode->GetDerivedPosition());
        CQuaternion rocketOrienation(pRocket->GetNode()->GetOrientation());
        rocketOrienation.SetDirection(vTargetDirection);
        pRocket->GetNode()->SetOrientation(rocketOrienation);
        //pRocket->GetNode()->SetDirection(vTargetDirection);
        pRocket->SetTarget(m_pTarget);
        pRocket->SetBulletType(m_bulletType);
        }
        }
        }
        else // if(m_bLaser)
        {
        //m_pNode->SetDirection(vTarget - m_pNode->GetDerivedPosition());
        CQuaternion rocketOrienation(m_pNode->GetOrientation());
        rocketOrienation.SetDirection(vTarget - m_pNode->GetDerivedPosition());
        m_pNode->SetOrientation(rocketOrienation);

        if(m_bLaserActive && m_currentLaserLifetime > 0.0f)
        {
        VECTOR3 vCurrentPos = m_pNode->GetDerivedPosition();

        CLevel* pLevel = gEngine->GetWorldMgr()->GetLevel();
        for(int i = 0; i < m_laserSprites.size(); ++i)
        {
        CSpriteEntity* pCurrentLaser = m_laserSprites[i];
        pCurrentLaser->SetRender(false);
        }

        for(int i = 0; i < m_laserSprites.size(); ++i)
        {
        CSpriteEntity* pCurrentLaser = m_laserSprites[i];
        pCurrentLaser->SetRender(true);
        VECTOR3 vIntersection, vNormal;
        bool bIntersection = false;
        if(pLevel->IsRayIntersecting(vCurrentPos, vTarget, &vIntersection, &vNormal))
        {
        vTarget = vIntersection;
        bIntersection = true;
        }

        float length = glm::length(vTarget - vCurrentPos);
        float scale = length / 128.0f;
        pCurrentLaser->GetNode()->SetScale(VECTOR3(scale, 1.0f, 1.0f));

        // Rotate laser sprite to target
        VECTOR3 vRight = glm::normalize(vTarget - vCurrentPos);
        VECTOR3 vDir(0, 0, 1);
        vDir = glm::normalize(glm::cross(VECTOR3(0, 1, 0), vRight));

        //pCurrentLaser->GetNode()->SetDirection(vDir, vRight);
        CQuaternion laserOrientation(pCurrentLaser->GetNode()->GetOrientation());
        laserOrientation.SetDirection(vDir, vRight);
        pCurrentLaser->GetNode()->SetOrientation(laserOrientation);

        pCurrentLaser->GetNode()->SetPosition(vCurrentPos + 0.5f * (vTarget - vCurrentPos));

        CGeometryCollidable cTarget = *pCharacter;
        CGeometryCollidable* pLaser = pCurrentLaser;
        pLaser->RemovePolygons();
        CPolygon polygon;
        polygon.SetNumPoints(2);
        polygon.AddPoint(VECTOR3(-64.0f, 0.0f, 0.0f));
        polygon.AddPoint(VECTOR3(64.0f, 0.0f, 0.0f));
        pLaser->AddPolygon(polygon);
        pLaser->ApplyTransform(pCurrentLaser->GetNode()->GetDerivedRST());
        MATRIX mtxChar = glm::translate(MATRIX(1.0f), pCharacter->GetNode()->GetDerivedPosition());
        cTarget.ApplyTransform(mtxChar);
        if(pLaser->HasCollision(cTarget))
        {
        //gEngine->GetWorldMgr()->SetPlayerKilled(true);
        m_currentLaserLifetime = 0.0f;
        pCurrentLaser->SetRenderGame(false);
        break;
        }
        else
        {
        if(!bIntersection)
        break;

        vCurrentPos = vTarget;
        VECTOR3 vReflected(0, 0, 0);
        ComputeReflectedVector(vReflected, vRight, vNormal);
        vTarget = vTarget + 5000.0f * vReflected;
        }


        }
        m_currentLaserLifetime -= dt;
        }
        else if(m_bLaserActive && m_currentLaserLifetime < 0.0f)
        {
        for(int i = 0; i < m_laserSprites.size(); ++i)
        {
        m_laserSprites[i]->SetRender(false);
        }
        m_currentLaserLifetime = 0.0f;
        m_bLaserActive = false;

        }
        else
        {
        m_fireAccum += dt;
        if(m_fireAccum >= (1.0f / m_fireRate))
        {
        // 					for(int i = 0; i < m_laserSprites.size(); ++i)
        // 					{
        // 						m_laserSprites[i]->SetRender(true);
        // 					}
        m_currentLaserLifetime = 2;
        m_bLaserActive = true;
        m_fireAccum = 0.0f;

        }
        }
        } */
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case  GUN_EVENT_ENABLE:
            m_bGunEnabled = true;
            m_bGunAlwaysEnabled = false;
            break;
        case GUN_EVENT_DISABLE:
            m_bGunEnabled = false;
            m_bGunAlwaysEnabled = false;
            break;
        default:
            CMeshEntity::OnInput(eventId, eventParameter);
        }
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::SetRadius(float radius)
    {
        m_radius = radius;

        RemoveCircles();
        CCircle c;
        c.SetRadius(m_radius);
        AddCircle(c);
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::SetFireRate(float rate)
    {
        m_fireRate = rate;
    }

    //-----------------------------------------------------------------------------------
    float CGunEntity::GetFireRate()
    {
        return m_fireRate;
    }

    //-----------------------------------------------------------------------------------
    float CGunEntity::GetRadius()
    {
        return m_radius;
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::CreateLaserSprite()
    {
        for (int i = 0; i < m_laserSprites.size(); ++i)
        {
            if (m_laserSprites[i])
                gEngine->GetEntityMgr()->RemoveEntity(m_laserSprites[i]->GetName());
            m_laserSprites[i] = NULL;
        }

        m_laserSprites.clear();

        if (m_laserType == LASER_TYPE_BOUNCING)
            m_laserSprites.resize(LASER_NUM_BOUNCES);
        else
            m_laserSprites.resize(1);

        for (int i = 0; i < m_laserSprites.size(); ++i)
        {
            CSpriteEntity* pLaser = (CSpriteEntity*)gEngine->GetEntityMgr()->CreateEntity(CSpriteEntity::ENTITY_META_TYPEID);
            pLaser->GetSprite().SetMaterialFilename("Sprites/laser.png");
            pLaser->SetPickable(false);
            pLaser->SetAutoDelete(false);
            pLaser->SetRender(false);
            m_laserSprites[i] = pLaser;
        }

    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::SetLaserType(LASER_TYPE type)
    {
        m_laserType = type;
        m_bLaser = true;

        CreateLaserSprite();
    }

    //-----------------------------------------------------------------------------------
    void CGunEntity::SetBulletType(BULLET_TYPE type)
    {
        m_bulletType = type;
        m_bLaser = false;

        for (int i = 0; i < m_laserSprites.size(); ++i)
        {
            if (m_laserSprites[i])
                gEngine->GetEntityMgr()->RemoveEntity(m_laserSprites[i]->GetName());
            m_laserSprites[i] = NULL;
        }
    }

} // env