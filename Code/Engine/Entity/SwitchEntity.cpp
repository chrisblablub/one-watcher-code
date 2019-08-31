#include <Engine/Entity/SwitchEntity.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/LightEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Scene/SceneMgr.h>

namespace env
{
    ENTITY_TYPEID CMeshSwitchEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_MESHSWITCH;

    CMeshSwitchEntity::CMeshSwitchEntity()
        : m_currentTriggerTime(0.0f),
        m_pLight(NULL)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CMeshSwitchEntity::~CMeshSwitchEntity()
    {
        if (m_pLight)
            gEngine->GetEntityMgr()->RemoveEntity(m_pLight->GetName());
    }

    void CMeshSwitchEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("Switch.x");

        SetFireOnce(true);

        CPolygon p;
        p.SetNumPoints(4);
        p.AddPoint(Vec3(-16.0f, -16.0f, 0.0f));
        p.AddPoint(Vec3(-16.0f, 16.0f, 0.0f));
        p.AddPoint(Vec3(16.0f, 16.0f, 0.0f));
        p.AddPoint(Vec3(16.0f, -16.0f, 0.0f));
        AddPolygon(p);

        m_bGhost = false;

        m_pLight = static_cast<CLightSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_LIGHTSPRITE));
        m_pLight->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 65.0f));
        m_pLight->SetAutoDelete(false);
        m_pLight->GetSprite().SetBillboard(true);

        m_pLight->GetLight()->SetRadius(300.0f);
        m_pLight->GetLight()->SetType(LT_POINT);
        m_pLight->GetLight()->SetInitialDiffuseColor(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
        m_pLight->GetSprite().SetColor(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
        m_pLight->GetLight()->SetInitialEnergy(2.5f);

        m_pLight->SetFlicker(true);
        m_pLight->SetFlickerTime(0.5f);
        m_pLight->SetFlickerPauseTime(0.5f);
        m_pLight->SetFlickerAmount(0.5f);
        m_pLight->SetFlickerRandomness(0.0f);



        /*CLightEntity* pLight = static_cast< CLightEntity* >(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_LIGHT));
        pLight->GetNode()->SetPosition(VECTOR3(0.0f, 0.0f, -35.0f));
        pLight->GetLight()->SetRadius(500.0f);
        pLight->GetLight()->SetType(LT_POINT);
        pLight->GetLight()->SetDiffuseColor(VECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
        pLight->GetLight()->SetEnergy(5.0f);
        pLight->GetMeshInstance*/


        m_pNode->AttachChildNode(m_pLight->GetNode());

    }

    void CMeshSwitchEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        m_bGhost = false;
    }

    void CMeshSwitchEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_bSwitchTriggered = false;

        m_currentTriggerTime = -1.0f;

        m_pLight->GetLight()->SetRenderInGame(true);

        m_pLight->SetRenderGame(true);
    }

    void CMeshSwitchEntity::OnTriggerEntered(CEntity& e)
    {
        m_bSwitchTriggered = true;

        m_currentTriggerTime = 1.0f;

    }

    void CMeshSwitchEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        if (m_currentTriggerTime >= 0.0f)
        {
            m_pLight->GetLight()->SetRenderInGame(false);

            m_pLight->SetRenderGame(false);

            m_currentTriggerTime = -1.0f;
        }

        ApplyTransform(m_pNode->GetDerivedRST());
    }

} // env