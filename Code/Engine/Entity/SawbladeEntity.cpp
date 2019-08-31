#include <Engine/Entity/SawbladeEntity.h>
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>


namespace env
{
    ENTITY_TYPEID CSawbladeEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SAWBLADE;

    //-----------------------------------------------------------------------------------
    CSawbladeEntity::CSawbladeEntity()
        : CMeshEntity()
        , m_currentType(SAWBLADE_TYPE_STANDARD)
        , m_initialRotationSpeedZ(1.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Sawblade] Start", SAWBLADE_EVENT_START);
        RegisterEvent("[Sawblade] Stop", SAWBLADE_EVENT_STOP);


        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    CSawbladeEntity::~CSawbladeEntity()
    {

    }

    //-----------------------------------------------------------------------------------
    void CSawbladeEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();


        SetMeshFilename("SawbladeNew01.x");


        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CSawbladeEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CSawbladeEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        m_pSound->LoadSound("Entities/Sawblade.wav");
        m_pSound->SetVolume(1.2f);
    }

    //-----------------------------------------------------------------------------------
    bool CSawbladeEntity::HasCollision(CEntity& e)
    {
        if (e.GetMetaTypeId() == CCharacter3DEntity::ENTITY_META_TYPEID)
        {
            if (CGeometryCollidable::HasCollision(e))
            {
                //gEngine->GetWorldMgr()->SetPlayerKilled(true);
                return true;
            }
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    void CSawbladeEntity::OnGameModeChanged(bool bGameMode)
    {

        CMeshEntity::OnGameModeChanged(bGameMode);

        m_prevPosition = m_pNode->GetDerivedPosition();
    }

    //-----------------------------------------------------------------------------------
    void CSawbladeEntity::SetRotationSpeedZ(float rot)
    {
        CMeshEntity::SetRotationSpeedZ(rot);

        if (!GetGameMode())
            m_initialRotationSpeedZ = rot;
    }

    //-----------------------------------------------------------------------------------
    void CSawbladeEntity::SetMeshFilename(const std::string& strName)
    {
        RemoveCircles();
        RemovePolygons();

        if (strName == "SawbladeNew01.x")
        {
            CMeshEntity::SetMeshFilename(strName);

            SetRotationSpeedZ(m_initialRotationSpeedZ);

            for (int i = 0; i < 6; ++i)
            {
                Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), PI * 2 / 6.0f * i, Vec3(0.0f, 0.0f, 1.0)));

                Vec3 p1(8.0f, 62.0f, 0.0f),
                    p2(-53.0f, 31.0f, 0.0f),
                    p3(4.0f, 94.0f, 0.0f);

                p1 = Vec3(Vec4(p1, 1.0f) * mtxRot);
                p2 = Vec3(Vec4(p2, 1.0f) * mtxRot);
                p3 = Vec3(Vec4(p3, 1.0f) * mtxRot);

                CPolygon p;
                p.SetNumPoints(3);
                p.AddPoint(p1);
                p.AddPoint(p2);
                p.AddPoint(p3);
                AddPolygon(p);
            }
        }
        else if (strName == "SawbladeNew02.x")
        {
            CMeshEntity::SetMeshFilename(strName);

            SetRotationSpeedZ(m_initialRotationSpeedZ);

            Matrix4x4 mtxMirror(1.0f);
            mtxMirror[0][0] = -1.0f;

            for (int i = 0; i < 6; ++i)
            {

                Matrix4x4 mtxRot = glm::rotate(Matrix4x4(1.0f), PI * 2 / 6.0f * i, Vec3(0.0f, 0.0f, 1.0));


                Vec3 p1(8.0f, 62.0f, 0.0f),
                    p2(-53.0f, 31.0f, 0.0f),
                    p3(4.0f, 94.0f, 0.0f);

                p1 = Vec3(Vec4(p1, 1.0f) * mtxRot * mtxMirror);
                p2 = Vec3(Vec4(p2, 1.0f) * mtxRot * mtxMirror);
                p3 = Vec3(Vec4(p3, 1.0f) * mtxRot * mtxMirror);

                CPolygon p;
                p.SetNumPoints(3);
                p.AddPoint(p1);
                p.AddPoint(p2);
                p.AddPoint(p3);
                AddPolygon(p);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CSawbladeEntity::ScrollType(bool bUp)
    {
        if (bUp)
            m_currentType = (SAWBLADE_TYPE)(((int)m_currentType + 1) % (int)SAWBLADE_TYPE_END);
        else
            m_currentType == 0 ? m_currentType = (SAWBLADE_TYPE)((int)SAWBLADE_TYPE_END - 1) : m_currentType = (SAWBLADE_TYPE)((int)m_currentType - 1);

        switch (m_currentType)
        {
        case SAWBLADE_TYPE_STANDARD:
            SetMeshFilename("SawbladeNew01.x");
            SetRotationSpeedZ(-2.0f);
            break;

        case SAWBLADE_TYPE_EXPANDING:
            SetMeshFilename("Hunter03.x");
            SetRotationSpeedZ(-2.0f);
            break;

        default:
            ENV_ASSERT_DBG(false);
            break;
        }
    }


    //-----------------------------------------------------------------------------------
    void CSawbladeEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        if (GetGameMode())
        {
            float velocity = glm::length(m_pNode->GetDerivedPosition() - m_prevPosition);

            if (m_initialRotationSpeedZ > 0.0f)
                SetRotationSpeedZ(m_initialRotationSpeedZ + velocity * 0.5f);
            else
                SetRotationSpeedZ(m_initialRotationSpeedZ - velocity * 0.5f);
        }
        else
        {
            if (gEngine->GetEntityMgr()->GetActiveEntity() == this)
            {
                 m_initialRotationSpeedZ = GetMeshInstanceContainer().GetMeshInstance(0)->GetRotationSpeedZ();
            }
        }

        m_prevPosition = m_pNode->GetDerivedPosition();

        ApplyTransform(GetMeshInstanceNode(0)->GetDerivedRST(), m_pNode->GetDerivedScale().x);
    }

    //-----------------------------------------------------------------------------------
    void CSawbladeEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case SAWBLADE_EVENT_START:
            //m_pMesh->GetAnimationController().SetTrackAnimationPaused(0, false);
            break;

        case SAWBLADE_EVENT_STOP:
            //m_pMesh->GetAnimationController().SetTrackAnimationPaused(0, true);
            break;

        default:
            CMeshEntity::OnInput(eventId, eventParameter);
            break;
        }
    }
} // env