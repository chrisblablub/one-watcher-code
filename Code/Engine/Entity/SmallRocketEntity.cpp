#include <Engine/Entity/SmallRocketEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Engine.h>

namespace env
{
    ENTITY_TYPEID CSmallRocketEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_ROCKET;

    std::vector< Bullet > CSmallRocketEntity::m_bulletTypes = std::vector< Bullet >();

    CSmallRocketEntity::CSmallRocketEntity()
        : m_currentLifeTime(0.0f)
        , m_pTarget(nullptr)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        if (m_bulletTypes.empty())
        {
            m_bulletTypes.resize(BULLET_TYPE_END);
            m_bulletTypes[BULLET_TYPE_ROCKET_SMALL] = Bullet("SmallRocket.x", 2.0f, 200.0f, 2.0f, 0);
            m_bulletTypes[BULLET_TYPE_ROCKET_BIG] = Bullet("BigRocket.x", 3.0f, 100.0f, 1.0f, 0);
            m_bulletTypes[BULLET_TYPE_BOUNCING] = Bullet("BouncingBullet.x", 2.0f, 200.0f, 2.0f, 3);
        }

        SetPersistent(false);

        m_bGhost = false;
    }

    CSmallRocketEntity::~CSmallRocketEntity()
    {

    }

    void CSmallRocketEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        CPolygon poly;
        poly.SetNumPoints(4);
        poly.AddPoint(Vec3(0.0f, -5.0f, 10.0f));
        poly.AddPoint(Vec3(0.0f, 5.0f, 10.0f));
        poly.AddPoint(Vec3(0.0f, 5.0f, -10.0f));
        poly.AddPoint(Vec3(0.0f, -5.0f, -10.0f));
        AddPolygon(poly);

        //m_pNode->SetDirection(VECTOR3(-1.0f, 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f));
    }

    void CSmallRocketEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();
    }

    bool CSmallRocketEntity::HasCollision(CEntity& e)
    {
        if (e.GetMetaTypeId() == CCharacter3DEntity::ENTITY_META_TYPEID)
        {
            if (CGeometryCollidable::HasCollision(e))
            {
                //gEngine->GetWorldMgr()->SetPlayerKilled(true);

                //gEngine->GetEntityMgr()->KillEntity(this);
                return true;
            }
        }
        else if (m_pTarget && e.GetMetaTypeId() == m_pTarget->GetMetaTypeId())
        {
            if (CGeometryCollidable::HasCollision(e))
            {
                gEngine->GetEntityMgr()->KillEntity(this);
                return true;
            }
        }

        return false;
    }

    void CSmallRocketEntity::Update(float dt)
    {
        /*CMeshEntity::Update(dt);

        m_currentLifeTime += dt;

        if(m_currentLifeTime >= m_bulletTypes[m_bulletType].lifeTime)
        {
        gEngine->GetEntityMgr()->KillEntity(this);
        return;
        }


        if(m_pTarget)
        {
        const VECTOR3& targetPos = m_pTarget->GetNode()->GetDerivedPosition();
        const VECTOR3& rocketPos = m_pNode->GetDerivedPosition();


        // 			if(CGeometryCollidable::HasCollision(*m_pTarget))
        // 			{
        // 				entityMgr->KillEntity(this);
        // 			}
        // 			else
        {
        if(m_bulletType != BULLET_TYPE_BOUNCING)
        m_pNode->RotateToDirection(targetPos - rocketPos, m_bulletTypes[m_bulletType].rotationSpeed * dt);

        VECTOR3 rocketDir = m_pNode->GetDerivedDirection();
        rocketDir = glm::normalize(rocketDir);

        VECTOR3 newRocketPos = rocketPos + rocketDir * (m_bulletTypes[m_bulletType].speed * dt);
        m_pNode->SetPosition(newRocketPos);

        CLevel* pLevel = gEngine->GetWorldMgr()->GetLevel();
        VECTOR3 intersection(0.0f, 0.0f, 0.0f), normal(0.0f, 1.0f, 0.0f);
        if(pLevel->IsRayIntersecting(rocketPos, newRocketPos, &intersection, &normal))
        {
        if(m_bulletType == BULLET_TYPE_BOUNCING && m_numCurrentBounces < 4)
        {
        VECTOR3 newDir(0.0f, 1.0f, 0.0f);
        ComputeReflectedVector(newDir, newRocketPos - rocketPos, normal);
        m_pNode->SetDirection(newDir);
        m_pNode->SetPosition(intersection);

        ++m_numCurrentBounces;
        }
        else
        gEngine->GetEntityMgr()->KillEntity(this);
        }
        }
        }

        ApplyTransform(m_pNode->GetDerivedRST());*/
    }

    void CSmallRocketEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_currentLifeTime = 0.0f;
    }

    void CSmallRocketEntity::SetBulletType(BULLET_TYPE type)
    {
        m_bulletType = type;
        SetMeshFilename(m_bulletTypes[m_bulletType].strFile);

        // 		switch(m_bulletType)
        // 		{
        // 		case BULLET_TYPE_ROCKET_SMALL:
        // 			SetMeshFilename(m_bulletTypes[m_bulletType].strFile);
        // 			break;
        // 
        // 		case BULLET_TYPE_ROCKET_BIG:
        // 			SetMeshFilename("BigRocket.x");
        // 			break;
        // 
        // 		case BULLET_TYPE_BOUNCING:
        // 			SetMeshFilename("BouncingBullet.x");
        // 			break;
        // 		}
    }

} // env