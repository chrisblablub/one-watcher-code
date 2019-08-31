#include <Engine/Entity/PortalEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Engine.h>
#include <Engine/Entity/EntityMgr.h>

namespace env
{

    ENTITY_TYPEID CPortalEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_PORTAL;

    CPortalEntity::CPortalEntity()
        : m_pPortalMate(NULL),
        m_bTriggerDisabled(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_bGhost = false;
    }

    CPortalEntity::~CPortalEntity()
    {
        if (m_pPortalMate)
            m_pPortalMate->SetPortalMate(NULL);
    }

    bool CPortalEntity::HasCollision(CEntity& entity)
    {
        if (entity.GetMetaTypeId() == CCharacter2DEntity::ENTITY_META_TYPEID)
        {
            if (CGeometryCollidable::HasCollision(entity))
            {
                if (m_pPortalMate && !m_bTriggerDisabled)
                {

                    CCharacter2DEntity* pChar = static_cast<CCharacter2DEntity*>(gEngine->GetEntityMgr()->GetFirstEntityOfType(CCharacter2DEntity::ENTITY_META_TYPEID));



                    m_pPortalMate->SetTriggerDisabled(true);
                    pChar->SetPosition(m_pPortalMate->GetNode()->GetDerivedPosition());

                }

                return true;
            }
        }

        return false;
    }

    void CPortalEntity::OnTriggerLeft()
    {
        m_bTriggerDisabled = false;
    }

    void CPortalEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        SetMaterialFilename("Sprites/portal.png");

        // 		RemovePolygons();
        // 		CPolygon poly;
        // 		poly.SetNumPoints(4);
        // 		poly.AddPoint(VECTOR3(-150.0f, -5.0f, 0.0f));
        // 		poly.AddPoint(VECTOR3(-150.0f, 5.0f, 0.0f));
        // 		poly.AddPoint(VECTOR3(150.0f, 5.0f, 0.0f));
        // 		poly.AddPoint(VECTOR3(150.0f, -5.0f, 0.0f));
        // 		AddPolygon(poly);
    }

    void CPortalEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();
    }

    void CPortalEntity::Update(float dt)
    {
        CSpriteEntity::Update(dt);
    }

    void CPortalEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }

    void CPortalEntity::SetPortalMate(CPortalEntity* pMate)
    {
        m_pPortalMate = pMate;
    }

} // env