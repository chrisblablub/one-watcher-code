
#include <Engine/Entity/ExplosionEntity.h>
#include <Engine/Entity/CameraEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <algorithm>

namespace env
{
    ENTITY_TYPEID CExplosionEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_EXPLOSION;

    //-----------------------------------------------------------------------------------
    CExplosionEntity::CExplosionEntity()
        : m_pLight(nullptr)
        , m_pCamera(nullptr)
        , m_pCharacter(nullptr)
        , m_radius(70.0f)
        , m_pSound(nullptr)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    CExplosionEntity::~CExplosionEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pLight);
        gEngine->GetEntityMgr()->RemoveEntity(m_pSound);
    }

    //-----------------------------------------------------------------------------------
    void CExplosionEntity::InitializeAtCreation()
    {
        CParticleSystemEntity::InitializeAtCreation();

        LoadFromTemplate("EXPLOSION_T1");

        m_pLight = static_cast<CLightSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_LIGHTSPRITE));
        m_pLight->SetAutoDelete(false);
        m_pLight->GetLight()->SetType(LT_POINT);
        m_pLight->GetLight()->SetInitialDiffuseColor(Vec4(1.0f, 0.2f, 0.0f, 1.0f));
        m_pLight->GetLight()->SetRadius(470.0f);
        m_pLight->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -15.0f));
        m_pLight->SetRenderGame(false);
        m_pLight->SetRenderEditor(false);
        m_pLight->GetLight()->SetInitialEnergy(15.0f);
        m_pLight->GetLight()->SetSpecular(true);
        m_pLight->GetLight()->SetInitialSpecularColor(Vec4(1.0f, 0.85f, 0.5f, 1.0f));

        m_pNode->AttachChildNode(m_pLight->GetNode());

        SetRenderGame(false);

        RemovePolygons();

        CCircle c;
        c.SetRadius(1.0f);
        AddCircle(c);

        SetPauseParticles(true);

        m_bExploding = false;

        m_bGhost = true;

        SetupSounds();
    }

    //-----------------------------------------------------------------------------------
    void CExplosionEntity::InitializeAtLoad()
    {
        CParticleSystemEntity::InitializeAtLoad();

        LoadFromTemplate("EXPLOSION_T1");

        m_pLight->SetRenderGame(false);
        m_pLight->SetRenderEditor(false);

        m_bExploding = false;

        SetGhost(true);

        SetupSounds();
    }

    void CExplosionEntity::SetupSounds()
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
        m_pSound->SetVolume(0.8f);
        m_pSound->LoadSound("Entities/Explosion.wav");
    }

    //-----------------------------------------------------------------------------------
    void CExplosionEntity::SetPickable(bool bPickable)
    {
        CParticleSystemEntity::SetPickable(bPickable);

        m_pLight->SetPickable(bPickable);
    }

    //-----------------------------------------------------------------------------------
    void CExplosionEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CExplosionEntity::AddToRenderQueue);

        CParticleSystemEntity::AddToRenderQueue(dt, queue);

//         if (!m_bExploding)
//         {
//             return;
//         }
//
//         if (m_pCharacter)
//         {
//             float distance = glm::length(m_pNode->GetDerivedPosition() - m_pCharacter->GetNode()->GetDerivedPosition());
// 
//             if (distance < 400.0f)
//             {
//                 if (m_timer.GetTime() < 0.1f)
//                 {
// 
//                 }
//                 else if (m_timer.GetTime() < 0.25f)
//                 {
//                     float alpha = (m_timer.GetTime() - 0.1f) / 0.15f;
// 
//                     float radius = alpha * 1000.0f;
// 
//                     queue.m_postProcessRenderObject.m_silhouetteCharacterOpacity = std::max(alpha, queue.m_postProcessRenderObject.m_silhouetteCharacterOpacity);
//                     queue.m_postProcessRenderObject.m_silhouetteCharacterRadius = std::max(radius, queue.m_postProcessRenderObject.m_silhouetteCharacterRadius);
//                 }
//                 else if (m_timer.GetTime() < 0.8f)
//                 {
//                     float alpha = 1.0f - (m_timer.GetTime() - 0.25f) / 0.55f;
// 
//                     float radius = alpha * 1000.0f;
// 
//                     queue.m_postProcessRenderObject.m_silhouetteCharacterOpacity = std::max(alpha, queue.m_postProcessRenderObject.m_silhouetteCharacterOpacity);
//                     queue.m_postProcessRenderObject.m_silhouetteCharacterRadius = std::max(radius, queue.m_postProcessRenderObject.m_silhouetteCharacterRadius);
//                 }
// 
//                 static float m_distortionAmount = GetRandomFloat(0.6f, 1.5f);
//                 static float m_distortionOffset = GetRandomFloat(0.0f, 25.0f);
//                 static Vec2  m_distortionDirection;
//                 static Vec2  m_distortionScaling;
// 
//                 if (m_timer.GetTime() < 0.1f)
//                 {
//                     m_distortionAmount = GetRandomFloat(0.7f, 1.3f);
//                     m_distortionOffset = GetRandomFloat(15.0f, 25.0f);
// 
//                     if (sgn(GetRandomFloat(-1, 1)) >= 0)
//                     {
//                         // x
//                         m_distortionDirection = Vec2(0.1f, 8.0f);
//                         m_distortionScaling = Vec2(sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.1f, 0.7f),
//                                                       sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.3f, 0.3f));
//                     }
//                     else
//                     {
//                         // y
//                         m_distortionDirection = Vec2(8.0f, 0.1f);
//                         m_distortionScaling = Vec2(sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.3f, 0.3f),
//                                                       sgn(GetRandomFloat(-1, 1)) * GetRandomFloat(0.1f, 0.7f));
//                     }
//                 }
//                 else if (m_timer.GetTime() < 0.25f)
//                 {
//                     float alpha = (m_timer.GetTime() - 0.15f) / 0.15f;
// 
//                     queue.m_postProcessRenderObject.m_imageDistortion.alpha = alpha;
//                     queue.m_postProcessRenderObject.m_imageDistortion.amount = m_distortionAmount;
//                     queue.m_postProcessRenderObject.m_imageDistortion.scaling = m_distortionScaling;
//                     queue.m_postProcessRenderObject.m_imageDistortion.dir = m_distortionDirection;
//                 }
//                 else if (m_timer.GetTime() < 0.5f)
//                 {
//                     float alpha = 1.0f - (m_timer.GetTime() - 0.25f) / 0.25f;
// 
//                     queue.m_postProcessRenderObject.m_imageDistortion.alpha = alpha;
//                     queue.m_postProcessRenderObject.m_imageDistortion.amount = m_distortionAmount;
//                     queue.m_postProcessRenderObject.m_imageDistortion.scaling = m_distortionScaling;
//                     queue.m_postProcessRenderObject.m_imageDistortion.dir = m_distortionDirection;
//                 }
//             }
//         }
    }

    //-----------------------------------------------------------------------------------
    void CExplosionEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CExplosionEntity::Update);

        CParticleSystemEntity::Update(dt);

//         if (gEngine->GetInput()->IsKeyJustDown('H'))
//         {
//             SetEnabled(true);
//         }

        if (!GetGameMode())
            return;

        if (!m_bExploding)
            return;

        m_timer.Update(dt);

//         if (m_timer.GetTime() < 0.1f)
//         {
//         }
//         else 
            if (m_timer.GetTime() < 0.15f)
        {
            if (m_timer.GetTime() < 0.18f)
            {

                if (m_pCharacter)
                {
                    float distance = glm::length(m_pNode->GetDerivedPosition() - m_pCharacter->GetNode()->GetDerivedPosition());

                    if (distance < 400.0f)
                    {
                        if (m_pCamera)
                            m_pCamera->Shake(0.05f, (1.0f - distance / 400.0f) * Vec2(-950.0f, 950.0f));
                    }
                }
            }

            SetGhost(false);

            float alpha = (m_timer.GetTime() - 0.0f) / 0.15f;
            ApplyTransform(m_pNode->GetDerivedRST(), alpha * m_radius);

            m_pLight->GetLight()->SetRenderInGame(true);
            m_pLight->GetLight()->SetEnergy(m_pLight->GetLight()->GetInitialEnergy() * alpha);

        }
        else if (m_timer.GetTime() < 0.3f)
        {
            float alpha = (1.0f - (m_timer.GetTime() - 0.15f) / 0.15f);
            m_pLight->GetLight()->SetEnergy(m_pLight->GetLight()->GetInitialEnergy() * alpha);

            ApplyTransform(m_pNode->GetDerivedRST(), alpha * m_radius);

            SetGhost(true);
        }
        else if (m_timer.GetTime() >= 2.5f)
        {
            m_pLight->GetLight()->SetRenderInGame(false);

            SetRenderParticles(false);
            SetPauseParticles(true);

            SetGhost(true);
        }
    }

    //-----------------------------------------------------------------------------------
    void CExplosionEntity::SetEnabled(bool bEnable)
    {
        if(m_bExploding && bEnable)
            return;

        if (bEnable)
        {
            for (mapEmitters::iterator it = m_emitters.begin(); it != m_emitters.end(); ++it)
                it->second->OnGameModeChanged(GetGameMode());


            m_pLight->GetLight()->SetEnergy(0.0f);
            m_pLight->GetLight()->SetRenderInGame(true);

            m_timer.Reset();

            m_bExploding = true;

            m_bGhost = true;

            m_pSound->PlaySound();

            SetRenderParticles(true);
            SetPauseParticles(false);
        }
        else
        {
            SetRenderParticles(false);
            SetPauseParticles(true);

            m_pLight->GetLight()->SetEnergy(0.0f);
            m_pLight->GetLight()->SetRenderInGame(false);

            m_bExploding = false;
        }
    }

    //-----------------------------------------------------------------------------------
    void CExplosionEntity::OnGameModeChanged(bool bGameMode)
    {
        CParticleSystemEntity::OnGameModeChanged(bGameMode);

        m_pCharacter = gEngine->GetEntityMgr()->GetCharacter();
        m_pCamera = static_cast<CCameraEntity*>(gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_PLAYERCAMERA));

        m_pLight->GetLight()->SetEnergy(0.0f);

        SetEnabled(false);

        SetGhost(true);
    }
} // env