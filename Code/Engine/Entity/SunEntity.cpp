#include <Engine/Entity/SunEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Scene/Light.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/WorldMgr.h>

namespace env
{
    ENTITY_TYPEID CSunEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SUN;

    //-----------------------------------------------------------------------------------
    CSunEntity::CSunEntity()
    {
        SetMetaTypeID(CSunEntity::ENTITY_META_TYPEID);

        m_bGhost = true;
    }

    //-----------------------------------------------------------------------------------
    CSunEntity::~CSunEntity()
    {
    }

    //-----------------------------------------------------------------------------------
    void CSunEntity::InitializeAtCreation()
    {
        CLightSpriteEntity::InitializeAtCreation();

        //m_sprite.SetBillboard(true);

        m_pLight->SetType(env::LT_DIRECTIONAL);
    }

    //-----------------------------------------------------------------------------------
    void CSunEntity::InitializeAtLoad()
    {
        CLightSpriteEntity::InitializeAtLoad();

        m_bGhost = true;
    }

    //-----------------------------------------------------------------------------------
    void CSunEntity::OnGameModeChanged(bool bGameMode)
    {
        CLightSpriteEntity::OnGameModeChanged(bGameMode);
    }

    //-----------------------------------------------------------------------------------
    void CSunEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CLightSpriteEntity::AddToRenderQueue(dt, queue);

        m_pNode->SetPosition(Vec3(-250.0f, 150.0f, 2500.0f) + gEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition());

        m_sunRenderObject.m_position = m_pNode->GetDerivedPosition();
        m_sunRenderObject.m_diffuseColor = Vec3(GetLight()->GetDiffuseColor());

        queue.m_pSunRenderObject = &m_sunRenderObject;
    }

    //-----------------------------------------------------------------------------------
    void CSunEntity::Update(float dt)
    {
        CLightSpriteEntity::Update(dt);

        Vec4 clr = m_sprite.GetColor();
        float brightnessDiff = std::min(1.0f - clr.x, std::min(1.0f - clr.y, 1.0f - clr.z));
        clr += Vec4(1.0f, 1.0f, 1.0f, 0.0f) * brightnessDiff;
        clr.w = 1.0f;
        m_sprite.SetColor(clr);
    }

} // env