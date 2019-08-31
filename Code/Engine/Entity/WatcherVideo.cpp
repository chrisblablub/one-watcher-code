#include <Engine/Entity/WatcherVideo.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/WorldMgr.h>


namespace env
{
    ENTITY_TYPEID CWatcherVideoEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_WATCHERVIDEO;

    //-----------------------------------------------------------------------------------
    CWatcherVideoEntity::CWatcherVideoEntity()
        : m_pEye(0)
        , m_pEyeFan(0)
        , m_pEarLeftFanOuter(0)
        , m_pEarLeftFanInner(0)
        , m_pEarRightFanOuter(0)
        , m_pEarRightFanInner(0)
        , m_pEyeParticles(nullptr)
        , m_currentWakeEyeTime(-1.0f)
        , m_currentSleepEyeTime(-1.0f)
        , m_currentIllumFadeInTime(-1.0f)
        , m_currentIllumFadeOutTime(-1.0f)
        , m_currentRotationPlanetTime(-1.0f)
        , m_rotationPlanetDuration(1.0f)
        , m_currentRotationEyeTime(-1.0f)
        , m_rotationEyeDuration(1.0f)
        , m_bIllumFadingIn(false)
        , m_bIllumFadingOut(false)
        , m_bWakeEye(false)
        , m_bSleepEye(false)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        /*
        WATCHER_VIDEO_EVENT_WAKE_EYE = MESH_EVENT_END,
        WATCHER_VIDEO_EVENT_SLEEP_EYE,
        WATCHER_VIDEO_EVENT_START_EYE_FAN,
        WATCHER_VIDEO_EVENT_STOP_EYE_FAN,
        WATCHER_VIDEO_EVENT_START_LEFT_EAR_FAN,
        WATCHER_VIDEO_EVENT_STOP_LEFT_EAR_FAN,
        WATCHER_VIDEO_EVENT_START_RIGHT_EAR_FAN,
        WATCHER_VIDEO_EVENT_STOP_RIGHT_EAR_FAN,
        WATCHER_VIDEO_EVENT_START_ILLUMINATION,
        WATCHER_VIDEO_EVENT_STOP_ILLUMINATION*/

        RegisterEvent("[WatcherVideo] Wake Eye", WATCHER_VIDEO_EVENT_WAKE_EYE);
        RegisterEvent("[WatcherVideo] Sleep Eye", WATCHER_VIDEO_EVENT_SLEEP_EYE);
        RegisterEvent("[WatcherVideo] Start Eye Fan", WATCHER_VIDEO_EVENT_START_EYE_FAN);
        RegisterEvent("[WatcherVideo] Stop Eye Fan", WATCHER_VIDEO_EVENT_STOP_EYE_FAN);
        RegisterEvent("[WatcherVideo] Start Left Ear Fan", WATCHER_VIDEO_EVENT_START_LEFT_EAR_FAN);
        RegisterEvent("[WatcherVideo] Stop Left Ear Fan", WATCHER_VIDEO_EVENT_STOP_LEFT_EAR_FAN);
        RegisterEvent("[WatcherVideo] Start Right Ear Fan", WATCHER_VIDEO_EVENT_START_RIGHT_EAR_FAN);
        RegisterEvent("[WatcherVideo] Stop Right Ear Fan", WATCHER_VIDEO_EVENT_STOP_RIGHT_EAR_FAN);
        RegisterEvent("[WatcherVideo] Start Illumination", WATCHER_VIDEO_EVENT_START_ILLUMINATION);
        RegisterEvent("[WatcherVideo] Stop Illumination", WATCHER_VIDEO_EVENT_STOP_ILLUMINATION);


        RegisterEvent("[WatcherVideo] Rotate Planet", WATCHER_VIDEO_EVENT_ROTATE_PLANET);
        RegisterEventParameter(WATCHER_VIDEO_EVENT_ROTATE_PLANET, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 5.0f, 0.0f, 100000.0f);

        RegisterEvent("[WatcherVideo] Rotate Eye", WATCHER_VIDEO_EVENT_ROTATE_EYE);
        RegisterEventParameter(WATCHER_VIDEO_EVENT_ROTATE_EYE, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f, 0.0f, 100000.0f);

    }

    //-----------------------------------------------------------------------------------
    CWatcherVideoEntity::~CWatcherVideoEntity()
    {
        if (m_pEarRightFanInner)
            gEngine->GetEntityMgr()->RemoveEntity(m_pEarRightFanInner->GetName());

        if (m_pEarRightFanOuter)
            gEngine->GetEntityMgr()->RemoveEntity(m_pEarRightFanOuter->GetName());

        if (m_pEarLeftFanInner)
            gEngine->GetEntityMgr()->RemoveEntity(m_pEarLeftFanInner->GetName());

        if (m_pEarLeftFanOuter)
            gEngine->GetEntityMgr()->RemoveEntity(m_pEarLeftFanOuter->GetName());

        if (m_pEyeFan)
            gEngine->GetEntityMgr()->RemoveEntity(m_pEyeFan->GetName());

        if (m_pEye)
            gEngine->GetEntityMgr()->RemoveEntity(m_pEye->GetName());

        gEngine->GetEntityMgr()->RemoveEntity(m_pEyeParticles);
    }

    //-----------------------------------------------------------------------------------
    void CWatcherVideoEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        SetMeshFilename("watcher.x");

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(true); });

        Initialize();
    }

    //-----------------------------------------------------------------------------------
    void CWatcherVideoEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        GetMeshInstanceContainer().CallForEach([](const auto& pMeshInst) { pMeshInst->GetMaterial().SetRender(true); });

        Initialize();
    }

    //-----------------------------------------------------------------------------------
    void CWatcherVideoEntity::Initialize()
    {
        if (!m_pEye)
        {
            m_pEye = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pEye->SetMeshFilename("watcherEye.x");
            m_pEye->SetAutoDelete(false);
            m_pEye->SetDeletable(false);
            m_pNode->AttachChildNode(m_pEye->GetNode());
        }

        m_pEye->SetPickable(true);
        m_pEye->SetDeletable(false);
        m_pEye->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -460.0f));
        m_pEye->GetNode()->SetOrientation(CQuaternion(0.0f, WATCHER_SLEEPING_EYE_PITCH, 0));

        if (!m_pEyeFan)
        {
            m_pEyeFan = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pEyeFan->SetMeshFilename("watcherEyeFan.x");
            m_pEyeFan->SetAutoDelete(false);
            m_pEyeFan->SetPickable(true);
            m_pEyeFan->SetDeletable(false);
            m_pNode->AttachChildNode(m_pEyeFan->GetNode());
        }

        m_pEyeFan->SetDeletable(false);
        m_pEyeFan->SetPickable(true);
        m_pEyeFan->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -410.0f));

        if (!m_pEarLeftFanOuter)
        {
            m_pEarLeftFanOuter = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pEarLeftFanOuter->SetMeshFilename("watcherEarFanOuter.x");
            m_pEarLeftFanOuter->SetAutoDelete(false);
            m_pEarLeftFanOuter->SetPickable(true);
            m_pEarLeftFanOuter->SetDeletable(false);
            m_pNode->AttachChildNode(m_pEarLeftFanOuter->GetNode());
        }

        m_pEarLeftFanOuter->SetDeletable(false);
        m_pEarLeftFanOuter->SetPickable(true);
//        m_pEarLeftFanOuter->SetRotationSpeedZ(0.0f);
//         m_pEarLeftFanOuter->GetNode()->SetPosition(Vec3(-460.0f, 0.0f, 0.0f));
//         m_pEarLeftFanOuter->GetNode()->SetOrientation(CQuaternion(PI, 0.0f, 0.0f));

        if (!m_pEarLeftFanInner)
        {
            m_pEarLeftFanInner = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pEarLeftFanInner->SetMeshFilename("watcherEarFanInner.x");
            m_pEarLeftFanInner->SetAutoDelete(false);
            m_pEarLeftFanInner->SetPickable(true);
            m_pEarLeftFanInner->SetDeletable(false);
            m_pNode->AttachChildNode(m_pEarLeftFanInner->GetNode());
        }

        m_pEarLeftFanInner->SetDeletable(false);
        m_pEarLeftFanInner->SetPickable(true);
/*        m_pEarLeftFanInner->SetRotationSpeedZ(-0.0f);*/
//         m_pEarLeftFanInner->GetNode()->SetPosition(Vec3(-470.0f, 0.0f, 0.0f));
//         m_pEarLeftFanInner->GetNode()->SetOrientation(CQuaternion(PI * 0.5f, 0.0f, 0.0f));

        if (!m_pEarRightFanOuter)
        {
            m_pEarRightFanOuter = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pEarRightFanOuter->SetMeshFilename("watcherEarFanOuter.x");
            m_pEarRightFanOuter->SetAutoDelete(false);
            m_pEarRightFanOuter->SetPickable(true);
            m_pEarRightFanOuter->SetDeletable(false);
            m_pNode->AttachChildNode(m_pEarRightFanOuter->GetNode());
        }

        m_pEarRightFanOuter->SetDeletable(false);
        m_pEarRightFanOuter->SetPickable(true);
/*        m_pEarRightFanOuter->SetRotationSpeedZ(0.0f);*/
//         m_pEarRightFanOuter->GetNode()->SetPosition(Vec3(460.0f, 0.0f, 0.0f));
//         m_pEarRightFanOuter->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));

        if (!m_pEarRightFanInner)
        {
            m_pEarRightFanInner = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
            m_pEarRightFanInner->SetMeshFilename("watcherEarFanInner.x");
            m_pEarRightFanInner->SetAutoDelete(false);
            m_pEarRightFanInner->SetPickable(true);
            m_pEarRightFanInner->SetDeletable(false);
            m_pNode->AttachChildNode(m_pEarRightFanInner->GetNode());
        }

        m_pEarRightFanInner->SetDeletable(false);
        m_pEarRightFanInner->SetPickable(true);
/*        m_pEarRightFanInner->SetRotationSpeedZ(-0.0f);*/
//         m_pEarRightFanInner->GetNode()->SetPosition(Vec3(470.0f, 0.0f, 0.0f));
//         m_pEarRightFanInner->GetNode()->SetOrientation(CQuaternion(-PI * 0.5f, 0.0f, 0.0f));

        if (!m_pEyeParticles)
        {
            m_pEyeParticles = static_cast<CParticleSystemEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_PARTICLESYSTEM));
            m_pEyeParticles->LoadFromTemplate("ENERGY_CORE_ORANGE");
            m_pEyeParticles->SetAutoDelete(false);
            m_pEyeParticles->SetPickable(true);
            m_pEyeParticles->SetDeletable(false);
            m_pEyeParticles->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pNode->AttachChildNode(m_pEyeParticles->GetNode());
        }
        //        GetMeshInstance()->GetInstanceAndMeshesRenderObject(m_watcherVideoRenderObject.m_meshesRenderObject);

        m_watcherVideoRenderObject.m_illumination = m_illumination;
    }

    //-----------------------------------------------------------------------------------
    void CWatcherVideoEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CWatcherVideoEntity::Update);

        CMeshEntity::Update(dt);

        if (m_currentRotationPlanetTime >= 0.0f)
        {
            m_currentRotationPlanetTime -= dt;

            float alpha = 0.0f;
            
            if (m_rotationPlanetDuration > 0.0f)
                alpha = Lerp(0.0f, 1.0f, m_currentRotationPlanetTime / m_rotationPlanetDuration);

            alpha = alpha * alpha;
            alpha = 1.0f - alpha;

            m_pNode->SetOrientation(CQuaternion(PI * 0.5f, PI * alpha, 0.0f));
        }

        if (m_currentRotationEyeTime >= 0.0f)
        {
            m_currentRotationEyeTime -= dt;

            float alpha = 0.0f;
            
            if (m_rotationEyeDuration > 0.0f)
                alpha = Lerp(0.0f, 1.0f, m_currentRotationEyeTime / m_rotationEyeDuration);

            alpha = alpha * alpha * alpha;

            m_pEye->GetNode()->SetOrientation(CQuaternion(0.0f, WATCHER_SLEEPING_EYE_PITCH * alpha, 0.0f));
        }

        if (m_currentWakeEyeTime >= 0.0f)
        {
            float alpha = m_currentWakeEyeTime / 1.0f;
            alpha = alpha * alpha * alpha;
            float pitch = alpha  * WATCHER_SLEEPING_EYE_PITCH;

            CQuaternion quat = m_pEye->GetNode()->GetOrientation();
            quat.SetPitch(pitch);
            m_pEye->GetNode()->SetOrientation(quat);

            m_currentWakeEyeTime -= dt;
        }
        else if (m_bWakeEye)
        {
            CQuaternion quat = m_pEye->GetNode()->GetOrientation();
            quat.SetPitch(0.0f);
            m_pEye->GetNode()->SetOrientation(quat);

            m_bWakeEye = false;
        }

        if (m_currentSleepEyeTime >= 0.0f)
        {
            float alpha = m_currentSleepEyeTime / 1.0f;
            alpha *= alpha;
            alpha = 1.0f - alpha;
            float pitch = alpha  * WATCHER_SLEEPING_EYE_PITCH;

            CQuaternion quat = m_pEye->GetNode()->GetOrientation();
            quat.SetPitch(pitch);
            m_pEye->GetNode()->SetOrientation(quat);

            m_currentSleepEyeTime -= dt;
        }
        else if (m_bSleepEye)
        {
            CQuaternion quat = m_pEye->GetNode()->GetOrientation();
            quat.SetPitch(WATCHER_SLEEPING_EYE_PITCH);
            m_pEye->GetNode()->SetOrientation(quat);

            m_bSleepEye = false;
        }

        if (m_bIllumFadingIn)
        {
            if (m_currentIllumFadeInTime >= 0.0f)
            {
                m_illumination = 1.0f - m_currentIllumFadeInTime / 3.0f;

                m_currentIllumFadeInTime -= dt;
            }
            else
            {
                m_illumination = 1.0f;

                m_bIllumFadingIn = false;
            }
        }
        else if (m_bIllumFadingOut)
        {
            if (m_currentIllumFadeOutTime >= 0.0f)
            {
                m_illumination = m_currentIllumFadeOutTime / 3.0f;

                m_currentIllumFadeOutTime -= dt;
            }
            else
            {
                m_illumination = 0.0f;

                m_bIllumFadingOut = false;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CWatcherVideoEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case WATCHER_VIDEO_EVENT_WAKE_EYE:
            m_currentWakeEyeTime = 1.0f;
            m_currentSleepEyeTime = -1.0f;
            m_bWakeEye = true;
            m_bSleepEye = false;
            break;

        case WATCHER_VIDEO_EVENT_SLEEP_EYE:
            m_currentWakeEyeTime = -1.0f;
            m_currentSleepEyeTime = 1.0f;
            m_bWakeEye = false;
            m_bSleepEye = true;
            break;

        case WATCHER_VIDEO_EVENT_START_EYE_FAN:
            m_pEyeFan->SetRotationSpeedZ(0.6f);
            break;

        case WATCHER_VIDEO_EVENT_STOP_EYE_FAN:
            m_pEyeFan->SetRotationSpeedZ(0.0f);
            break;

        case WATCHER_VIDEO_EVENT_START_LEFT_EAR_FAN:
            m_pEarLeftFanOuter->SetRotationSpeedZ(0.5f);
            m_pEarLeftFanInner->SetRotationSpeedZ(-0.9f);
            break;

        case WATCHER_VIDEO_EVENT_STOP_LEFT_EAR_FAN:
            m_pEarLeftFanOuter->SetRotationSpeedZ(0.0f);
            m_pEarLeftFanInner->SetRotationSpeedZ(0.0f);
            break;

        case WATCHER_VIDEO_EVENT_START_RIGHT_EAR_FAN:
            m_pEarRightFanOuter->SetRotationSpeedZ(0.5f);
            m_pEarRightFanInner->SetRotationSpeedZ(-0.9f);
            break;

        case WATCHER_VIDEO_EVENT_STOP_RIGHT_EAR_FAN:
            m_pEarRightFanOuter->SetRotationSpeedZ(0.0f);
            m_pEarRightFanInner->SetRotationSpeedZ(0.0f);
            break;

        case WATCHER_VIDEO_EVENT_START_ILLUMINATION:
            m_illumination = 0.0f;

            m_currentIllumFadeInTime = 3.0f;
            m_currentIllumFadeOutTime = -1.0f;

            m_bIllumFadingIn = true;
            m_bIllumFadingOut = false;
            break;

        case WATCHER_VIDEO_EVENT_STOP_ILLUMINATION:
            m_illumination = 1.0f;

            m_currentIllumFadeOutTime = 3.0f;
            m_currentIllumFadeInTime = -1.0f;

            m_bIllumFadingIn = false;
            m_bIllumFadingOut = true;
            
            break;

        case WATCHER_VIDEO_EVENT_ROTATE_PLANET:

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_rotationPlanetDuration);

            m_currentRotationPlanetTime = m_rotationPlanetDuration;

            break;

        case WATCHER_VIDEO_EVENT_ROTATE_EYE:

            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_rotationEyeDuration);

            m_currentRotationEyeTime = m_rotationEyeDuration;

            break;

        default:
            CMeshEntity::OnInput(eventId, eventParameter);
            break;
        }
    }

    //-----------------------------------------------------------------------------------
    void CWatcherVideoEntity::OnGameModeChanged(bool bGameMode)
    {
        CMeshEntity::OnGameModeChanged(bGameMode);

        m_currentWakeEyeTime = -1.0f;
        m_currentSleepEyeTime = -1.0f;
        m_currentRotationPlanetTime = -1.0f;
        m_currentRotationEyeTime = -1.0f;

        m_bWakeEye = false;
        m_bSleepEye = false;

        m_illumination = 0.0f;

        m_currentIllumFadeInTime = -1.0f;
        m_currentIllumFadeOutTime = -1.0f;

        m_bIllumFadingIn = false;
        m_bIllumFadingOut = false;

        m_pNode->SetOrientation(CQuaternion(PI * 0.5f, 0.0f, 0.0f));
        m_pEye->GetNode()->SetOrientation(CQuaternion(0.0f, WATCHER_SLEEPING_EYE_PITCH, 0.0f));
    }

    //-----------------------------------------------------------------------------------
    void CWatcherVideoEntity::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        //CMeshEntity::AddToRenderQueue(dt, renderQueue);

//         const Vec3& scale = GetMeshInstance()->GetAttachedNode()->GetDerivedScale();
//         Matrix4x4 mtxScaling = glm::scale(Matrix4x4(1.0f), Vec3(scale.x * 1.05f, scale.y * 1.05f, scale.z * 1.05f));
// 
//         m_watcherVideoRenderObject.m_mtxTransform = mtxScaling * GetMeshInstance()->GetAttachedNode()->GetDerivedRST();
//         m_watcherVideoRenderObject.m_illumination = m_illumination;

        renderQueue.m_pWatcherVideoRenderObject = &m_watcherVideoRenderObject;

    }
} // env