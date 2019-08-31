#include <Engine/Entity/SawbladeBossEntity.h>
#include <Engine/Entity/EntityMgr.h>

namespace env
{
    ENTITY_TYPEID CSawbladeBossEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_SAWBLADEBOSS;

    CSawbladeBossEntity::CSawbladeBossEntity()
        : m_pFan(nullptr)
        , m_pCore(nullptr)
        , m_bWake(false)
        , m_currentWakeTime(-1.0f)
        , m_illumination(1.0f)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[SawbladeBoss] Wake", SAWBLADE_BOSS_EVENT_WAKE);
        RegisterEvent("[SawbladeBoss] Sleep", SAWBLADE_BOSS_EVENT_SLEEP);

    }

    CSawbladeBossEntity::~CSawbladeBossEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pFan);
        gEngine->GetEntityMgr()->RemoveEntity(m_pCore);
    }

    void CSawbladeBossEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("SawbladeNew03.x");

        m_energyShield.m_pEntity = this;
        m_energyShield.m_illumination = 1.0f;

        Initialize();
    }

    void CSawbladeBossEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        m_energyShield.m_pEntity = this;
        m_energyShield.m_illumination = 1.0f;

        Initialize();
    }

    void CSawbladeBossEntity::Initialize()
    {
        if (m_pFan)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_pFan);
            m_pFan = nullptr;
        }

        if (m_pCore)
            gEngine->GetEntityMgr()->RemoveEntity(m_pCore->GetName());

        SetRotationSpeedZ(-2.0f);

        RemovePolygons();

        for (int i = 0; i < 6; ++i)
        {
            float angle = 60.0f / 180.0f * PI;

            Matrix4x4 mtxRot = glm::transpose(glm::rotate(Matrix4x4(1.0f), angle * i, Vec3(0.0f, 0.0f, 1.0f)));


            CPolygon p;
            p.SetNumPoints(8);

            Vec3 p0(100.0f, 360.0f, 0.0f);
            p0 = Vec3(Vec4(p0, 1.0f) * mtxRot);

            Vec3 p1(32.0f, 402.0f, 0.0f);
            p1 = Vec3(Vec4(p1, 1.0f) * mtxRot);

            Vec3 p2(90.0f, 220.0f, 0.0f);
            p2 = Vec3(Vec4(p2, 1.0f) * mtxRot);

            Vec3 p3(210.0f, 135.0f, 0.0f);
            p3 = Vec3(Vec4(p3, 1.0f) * mtxRot);

            Vec3 p4(250.0f, 320.0f, 0.0f);
            p4 = Vec3(Vec4(p4, 1.0f) * mtxRot);

            Vec3 p5(200.0f, 380.0f, 0.0f);
            p5 = Vec3(Vec4(p5, 1.0f) * mtxRot);

            Vec3 p6(200.0f, 424.0f, 0.0f);
            p6 = Vec3(Vec4(p6, 1.0f) * mtxRot);

            Vec3 p7(56.0f, 520.0f, 0.0f);
            p7 = Vec3(Vec4(p7, 1.0f) * mtxRot);

            p.AddPoint(p0);
            p.AddPoint(p1);
            p.AddPoint(p2);
            p.AddPoint(p3);
            p.AddPoint(p4);
            p.AddPoint(p5);
            p.AddPoint(p6);
            p.AddPoint(p7);

            AddPolygon(p);
        }


        RemoveCircles();
        CCircle c;
        c.SetRadius(250.0f);
        AddCircle(c);

        m_bGhost = false;

    }

    void CSawbladeBossEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);

        if (m_bWake)
        {
            if (m_currentWakeTime >= 0.0f)
            {
                m_currentWakeTime -= dt;


                float alpha = 1.0f - m_currentWakeTime / 2.0f;

                SetRotationSpeedZ(SAWBLADE_ROTATION_SPEED * alpha);


                m_illumination = alpha;
            }
            else
            {

                SetRotationSpeedZ(SAWBLADE_ROTATION_SPEED);

                m_illumination = 1.0f;

                m_bWake = false;
            }

        }

        //ApplyTransform(GetMeshInstanceNode()->GetDerivedRST());
    }

    void CSawbladeBossEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        SetRotationSpeedZ(SAWBLADE_ROTATION_SPEED);

        //GetMeshInstanceNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));

        m_bWake = false;
        m_currentWakeTime = -1.0f;
        m_illumination = 1.0f;

        //ApplyTransform(GetMeshInstanceNode()->GetDerivedRST());
    }

    void CSawbladeBossEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        CMeshEntity::AddToRenderQueue(dt, renderQueue);

        Matrix4x4 mtxTranslation = glm::translate(Matrix4x4(1.0f), Vec3(0.0f, 0.0f, -10.0f));

        m_energyShield.mtxTransform = mtxTranslation;
        m_energyShield.m_illumination = m_illumination;
    }

    void CSawbladeBossEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case SAWBLADE_BOSS_EVENT_WAKE:
            m_bWake = true;
            m_currentWakeTime = 2.0f;
            m_illumination = 0.0f;
            break;

        case SAWBLADE_BOSS_EVENT_SLEEP:

            SetRotationSpeedZ(0.0f);

            m_illumination = 0.0f;

            m_bWake = false;
            m_currentWakeTime = -1.0f;

            break;

        default:
            CMeshEntity::OnInput(eventId, eventParameter);
            break;
        }
    }
} // env