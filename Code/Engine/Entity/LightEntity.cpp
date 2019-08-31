
#include <Engine/Entity/LightEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Scene/Light.h>
#include <Engine/Core/Scenario.h>

namespace env
{
    ENTITY_TYPEID CLightEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_LIGHT;

    CLightEntity::CLightEntity()
        : CMeshEntity(),
        m_pTargetEntity(NULL),
        m_pLight(0),
        m_bFollowTarget(false),
        m_bFollowPlayer(false),
        m_bFollowPlayerWhileInSpot(false),
        m_maxRotationSpeed(1.0f),
        m_bFlicker(false),
        m_bFlickering(false),
        m_bSprite(true),
        m_curTime(0.0f),
        m_targetTime(0.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Light] On", LIGHT_EVENT_ON);
        RegisterEvent("[Light] Off", LIGHT_EVENT_OFF);
        RegisterEvent("[Light] Follow Target", LIGHT_EVENT_FOLLOWTARGET);
        RegisterEvent("[Light] Follow Target (Spot)", LIGHT_EVENT_FOLLOWTARGETWHILEINSPOT);

        m_bGhost = true;
    }

    CLightEntity::~CLightEntity()
    {
        gEngine->GetSceneMgr()->RemoveLight(m_pLight->GetName());
        //sceneMgr->RemoveNode(m_pObjectNode->GetName());
    }

    void CLightEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        m_pLight = gEngine->GetSceneMgr()->CreateLight();
        m_pLight->Serialize();
        m_pLight->AttachNode(m_pNode);

        SetMeshFilename("spotlight.x");
        m_pLight->SetType(env::LT_SPOT);

        m_sprite.SetMaterialFilename("Sprites/light01.png");
        m_sprite.AttachNode(m_pNode);



    }

    void CLightEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        m_sprite.InitializeAtLoad();
        m_sprite.AttachNode(m_pNode);

        m_bGhost = true;
    }

    void CLightEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        //m_currentEnergy = m_pLight->GetEnergy();
    }

    bool CLightEntity::HasCollision(CEntity& e)
    {
        if (e.GetMetaTypeId() == CCharacter2DEntity::ENTITY_META_TYPEID)
        {
            const Vec3& charPos = e.GetNode()->GetDerivedPosition();
            Vec3 lightPos = m_pNode->GetDerivedPosition();
            Vec3 lightDir = m_pNode->GetDerivedDirection();

            float param = ComputePointOnLineProjection(charPos, lightPos, lightDir);

            Vec3 projCharPos = lightPos + param * lightDir;
            float charDistance = glm::length(charPos - projCharPos);
            float maxDistance = tan(m_pLight->GetSpotInnerAngle()) * glm::length(projCharPos - lightPos);
            if (charDistance < maxDistance)
            {
                return true;
            }
        }


        return false;
    }

    void CLightEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case LIGHT_EVENT_ON:
            if (m_pLight)
            {
                m_pLight->SetRenderInEditor(true);
                m_pLight->SetRenderInGame(true);
            }

            break;
        case LIGHT_EVENT_OFF:
            if (m_pLight)
            {
                m_pLight->SetRenderInEditor(false);
                m_pLight->SetRenderInGame(false);
            }
            break;
            // 		case LIGHT_EVENT_FOLLOWPLAYER:
            // 			m_bFollowPlayer = true;
            // 			m_bFollowTarget = false;
            // 			m_bFollowPlayerWhileInSpot = false;
            // 			break;
        case LIGHT_EVENT_FOLLOWTARGET:
            m_bFollowTarget = true;
            m_bFollowPlayer = false;
            m_bFollowPlayerWhileInSpot = false;
            break;
        case LIGHT_EVENT_FOLLOWTARGETWHILEINSPOT:
            m_bFollowPlayerWhileInSpot = true;
            m_bFollowPlayer = false;
            break;
        default:
            CMeshEntity::OnInput(eventId, eventParameter);
            break;
        }
    }




    void CLightEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        CMeshEntity::AddToRenderQueue(dt, queue);

//         if (m_bSprite && m_pMeshInstance && ((m_pMeshInstance->GetRenderInGame() && GetGameMode()) || (!GetGameMode() && m_pMeshInstance->GetRenderInEditor())))
//         {
//             m_sprite.SetAlphaBlending(true);
//             m_sprite.SetBillboard(true);
// 
//             Vec4 clr = m_pLight->GetDiffuseColor();
//             clr = 0.3f * clr + 0.7f * Vec4(1.0f, 1.0f, 1.0f, 1.0f);
//             clr.w = m_pLight->GetCurrentEnergy() / 2.0f;
//             clr *= 1.0f;
//             m_sprite.SetColor(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
//             //m_sprite.SetColor(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
// 
// 
//             queue.m_spriteRenderObjects.push_back(m_sprite.GetRenderObject());
//         }

    }

    void CLightEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);


//         m_curTime -= dt;
//         if (m_bFlicker)
//         {
//             if (m_bFlickering)
//             {
//                 m_pLight->SetEnergy(m_curTime / m_targetTime);
//             }
//             else
//             {
//                 m_pLight->SetEnergy(m_pLight->GetInitialEnergy());
//             }
// 
//             if (m_curTime < 0.0f)
//             {
//                 float c = GetRandomFloat(0.0f, 1.0f);
//                 if (c > 0.6f)
//                     m_bFlickering = false;
//                 else
//                     m_bFlickering = true;
// 
//                 if (m_bFlickering)
//                 {
//                     m_curTime = GetRandomFloat(0.0f, 0.3f);
//                 }
//                 else
//                 {
//                     m_curTime = GetRandomFloat(0.3f, 1.5f);
//                 }
//                 m_targetTime = m_curTime;
// 
//             }
//         }
// 
//         if (!gEngine->GetEntityMgr()->IsValid(m_pTargetEntity))
//         {
//             m_pTargetEntity = NULL;
//             m_bFollowTarget = false;
//         }
//         if (m_bFollowTarget && m_pTargetEntity)
//         {
//             //m_pNode->RotateToDirection(m_pTargetEntity->GetNode()->GetDerivedPosition() - m_pNode->GetDerivedPosition(), rotSpeed);
//         }
    }

    CLight* CLightEntity::GetLight()
    {
        return m_pLight;
    }





    ENTITY_TYPEID CSimpleLightEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SIMPLELIGHT;

    CSimpleLightEntity::CSimpleLightEntity()
    {
        SetMetaTypeID(ENTITY_META_TYPEID);
    }

    CSimpleLightEntity::~CSimpleLightEntity()
    {
        if (m_pLight)
            gEngine->GetSceneMgr()->RemoveLight(m_pLight->GetName());
    }

    void CSimpleLightEntity::InitializeAtCreation()
    {
        CSpriteEntity::InitializeAtCreation();

        m_pLight = gEngine->GetSceneMgr()->CreateLight();
        m_pLight->Serialize();
        m_pLight->AttachNode(m_pNode);

        m_pLight->SetType(env::LT_POINT);

        SetRenderGame(false);
    }

    void CSimpleLightEntity::InitializeAtLoad()
    {
        CSpriteEntity::InitializeAtLoad();
    }

    void CSimpleLightEntity::OnGameModeChanged(bool bGameMode)
    {
        CSpriteEntity::OnGameModeChanged(bGameMode);
    }



} /* End namespace */