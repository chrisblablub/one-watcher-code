#include <Engine/Entity/BumperEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/EntityMgr.h>

namespace env
{
    ENTITY_TYPEID CBumperEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_BUMPER;

    //-----------------------------------------------------------------------------------
    CBumperEntity::CBumperEntity()
        : m_pCharacter(nullptr)
        , m_bumpTime(-1.0f)
        , m_pFan(nullptr)
        , m_version(0)
    {
        m_bGhost = false;

        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    //-----------------------------------------------------------------------------------
    CBumperEntity::~CBumperEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pFan);
    }

    //-----------------------------------------------------------------------------------
    void CBumperEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        RemovePolygons();

        AddCircle(CCircle(Vec3(0.0, 0.0, 0.0), 100.0));

        SetMeshFilename("Bumper.x");

//         m_pFan = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
//         m_pFan->SetMeshFilename("FanLight01.x");
//         m_pFan->SetAutoDelete(false);
//         m_pFan->SetPickable(true);
//         m_pFan->SetRotationSpeedZ(-1.0);
//         m_pFan->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -3.64f));
//         m_pFan->GetNode()->SetScale(Vec3(0.4f, 0.4f, 0.4f));
//         m_pNode->AttachChildNode(m_pFan->GetNode());

        m_sprite.SetMaterialFilename("Sprites/bumper.png");
        m_sprite.AttachNode(m_pNode);
        m_sprite.SetColor(Vec4(0.0f, 0.8f, 1.0f, 1.0f));

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CBumperEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        if (m_pFan)
            gEngine->GetEntityMgr()->RemoveEntity(m_pFan->GetName());

        m_pFan = nullptr;

//         if (m_version <= 1)
//             m_pFan->SetMeshFilename("FanLight01.x");
// 
//         m_pFan->SetAutoDelete(false);
//         m_pFan->SetPickable(true);
//         m_pFan->SetRotationSpeedZ(-1.0);
//         m_pFan->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -3.64f));
//         m_pFan->GetNode()->SetScale(Vec3(0.4f, 0.4f, 0.4f));

        m_sprite.SetMaterialFilename("Sprites/bumper.png");
        m_sprite.AttachNode(m_pNode);
        m_sprite.SetColor(Vec4(0.0f, 0.8f, 1.0f, 2.0f));

        m_bGhost = false;
    }

    void CBumperEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));         
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }
 
        m_pSound->SetIsAutoplay(false);
        m_pSound->SetVolume(1);
        m_pSound->LoadSound("Entities/Bumper.wav");
    }

    //-----------------------------------------------------------------------------------
    Vec3 CBumperEntity::GetForce()
    {
        if (!m_pCharacter)
            return Vec3(0.0f, 1.0f, 0.0f);
        else
        {
            Vec3 force(glm::normalize(m_pCharacter->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition()));

            return force;
        }
    }

    //-----------------------------------------------------------------------------------
    void CBumperEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        queue.m_spriteRenderObjects.push_back(m_sprite.GetRenderObject());
    }

    //-----------------------------------------------------------------------------------
    void CBumperEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        if (m_bumpTime > 0.0f)
        {
            float bumpScaling = 1.0f + 0.2f * (1.0f - (abs(m_bumpTime - BUMPER_BUMP_TIME * 0.5f) / (BUMPER_BUMP_TIME * 0.5f)));

            m_pNode->SetScale(Vec3(bumpScaling, bumpScaling, bumpScaling));

            m_bumpTime -= dt;
        }

        ApplyTransform(m_pNode->GetDerivedRST(), 1);
    }

    //-----------------------------------------------------------------------------------
    void CBumperEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_pCharacter = gEngine->GetEntityMgr()->GetCharacter();

        m_pNode->SetScale(Vec3(1, 1, 1));

        m_bumpTime = -1.0f;
    }

    void CBumperEntity::Bump()
    {
        m_bumpTime = BUMPER_BUMP_TIME;
        m_pSound->PlaySound();
    }
} // env