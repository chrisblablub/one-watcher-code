
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/ParticleSystemMgr.h>
#include <Engine/Core/Input.h>

// Include boost serialization headers, afterwards
// serialized derived classes
//
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/MeshEntity.h>
#include <Engine/Entity/LightEntity.h>
#include <Engine/Entity/PathWaypointEntity.h>
#include <Engine/Entity/PathEntity.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/RotatorEntity.h>
#include <Engine/Entity/SawbladeEntity.h>
#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Entity/ParticleEmitterEntity.h>
#include <Engine/Entity/HunterEntity.h>
#include <Engine/Entity/GunEntity.h>
#include <Engine/Entity/CheckpointEntity.h>
#include <Engine/Entity/StreamerEntity.h>
#include <Engine/Entity/CounterEntity.h>
#include <Engine/Entity/SpikeBallEntity.h>
#include <Engine/Entity/PolygonEntity.h>
#include <Engine/Entity/ParticleEmitterEntity.h>
#include <Engine/Entity/AcceleratorEntity.h>
#include <Engine/Entity/PortalEntity.h>
#include <Engine/Entity/PlatformEntity.h>
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Entity/SmallRocketEntity.h>
#include <Engine/Entity/FadeEntity.h>
#include <Engine/Entity/LaserRadiatorEntity.h>
#include <Engine/Entity/ForceFieldEntity.h>
#include <Engine/Entity/MagnetEntity.h>
#include <Engine/Entity/RackWheelEntity.h>
#include <Engine/Entity/LightningEntity.h>
#include <Engine/Entity/RobotArmEntity.h>
#include <Engine/Entity/BombEntity.h>
#include <Engine/Entity/ExplosionEntity.h>
#include <Engine/Entity/BumperEntity.h>
#include <Engine/Entity/SwitchEntity.h>
#include <Engine/Entity/SkyDomeEntity.h>
#include <Engine/Entity/SkyPlaneEntity.h>
#include <Engine/Entity/PlanetEntity.h>
#include <Engine/Entity/SunEntity.h>
#include <Engine/Entity/LightSpriteEntity.h>
#include <Engine/Entity/SoundSpriteEntity.h>
#include <Engine/Entity/PostProcessingEntity.h>
#include <Engine/Entity/CameraEntity.h>
#include <Engine/Entity/SkyBoxEntity.h>
#include <Engine/Entity/StreamerGunEntity.h>
#include <Engine/Entity/MissileEntity.h>
#include <Engine/Entity/EnergyFieldEntity.h>
#include <Engine/Entity/WatcherVideo.h>
#include <Engine/Entity/TextEntity.h>
#include <Engine/Entity/SawbladeBossEntity.h>
#include <Engine/Entity/EnvMapEntity.h>
#include <Engine/Entity/StripesEntity.h>
#include <Engine/Entity/TimelapseEntity.h>
#include <Engine/Entity/BossEntity.h>
#include <Engine/Entity/WeaponEntity.h>
#include <Engine/Entity/ProjectileEntity.h>

#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/RenderObjects/TerrainRenderObject.h>

#ifdef ENV_PLATFORM_WINDOWS
BOOST_CLASS_EXPORT_GUID(env::CMeshEntity, "CMeshEntity")
BOOST_CLASS_EXPORT_GUID(env::CSawbladeEntity, "CSawbladeEntity")
BOOST_CLASS_EXPORT_GUID(env::CSpriteEntity, "CSpriteEntity")
BOOST_CLASS_EXPORT_GUID(env::CCheckpointEntity, "CCheckpointEntity")
BOOST_CLASS_EXPORT_GUID(env::CHunterBaseEntity, "CHunterEntity")
BOOST_CLASS_EXPORT_GUID(env::CGunEntity, "CGunEntity")
BOOST_CLASS_EXPORT_GUID(env::CLightEntity, "CLightEntity")
BOOST_CLASS_EXPORT_GUID(env::CParticleSystemEntity, "CParticleSystemEntity")
BOOST_CLASS_EXPORT_GUID(env::CPathEntity, "CPathEntity")
BOOST_CLASS_EXPORT_GUID(env::CPathWaypointEntity, "CPathWaypointEntity")
BOOST_CLASS_EXPORT_GUID(env::CRotatorEntity, "CRotatorEntity")
BOOST_CLASS_EXPORT_GUID(env::CCounterEntity, "CCounterEntity")
BOOST_CLASS_EXPORT_GUID(env::CSpikeBallEntity, "CSpikeBallEntity")
BOOST_CLASS_EXPORT_GUID(env::CPolygonEntity, "CPolygonEntity")
BOOST_CLASS_EXPORT_GUID(env::CParticleEmitterEntity, "CPointEmitter")
BOOST_CLASS_EXPORT_GUID(env::IParticleAffector, "IParticleAffector")
BOOST_CLASS_EXPORT_GUID(env::CParticleSunlightAffector, "CParticleSunlightAffector")
BOOST_CLASS_EXPORT_GUID(env::CParticleTextureVelocityRotator, "CParticleTextureRotator")
BOOST_CLASS_EXPORT_GUID(env::CParticleTextureRandomRotator, "CParticleTextureRandomRotator")
BOOST_CLASS_EXPORT_GUID(env::CCharacter2DEntity, "CCharacterEntity")
BOOST_CLASS_EXPORT_GUID(env::CCharacter3DEntity, "CCharacter3DEntity")
BOOST_CLASS_EXPORT_GUID(env::CSmallRocketEntity, "CSmallRocketEntity")
BOOST_CLASS_EXPORT_GUID(env::CPlatformEntity, "CPlatformEntity")
BOOST_CLASS_EXPORT_GUID(env::CPlatformBaseEntity, "CPlatformBaseEntity")
BOOST_CLASS_EXPORT_GUID(env::CFadeEntity, "CFadeEntity")
BOOST_CLASS_EXPORT_GUID(env::CLaserRadiatorEntity, "CLaserRadiatorEntity")
BOOST_CLASS_EXPORT_GUID(env::CPortalEntity, "CPortalEntity")
BOOST_CLASS_EXPORT_GUID(env::CForceFieldEntity, "CForceFieldEntity")
BOOST_CLASS_EXPORT_GUID(env::CStreamerEntity, "CBurnerEntity")
BOOST_CLASS_EXPORT_GUID(env::CAcceleratorEntity, "CAcceleratorEntity")
BOOST_CLASS_EXPORT_GUID(env::CMagnetEntity, "CMagnetEntity")
BOOST_CLASS_EXPORT_GUID(env::CMagnetClusterEntity, "CMagnetClusterEntity")
BOOST_CLASS_EXPORT_GUID(env::CRackWheelEntity, "CRackWheelEntity")
BOOST_CLASS_EXPORT_GUID(env::CTriggerEntity, "CSwitchEntity")
BOOST_CLASS_EXPORT_GUID(env::CSphereParticleEmitterEntity, "CSphereParticleEmitterEntity");
BOOST_CLASS_EXPORT_GUID(env::CBoxParticleEmitterEntity, "CBoxParticleEmitterEntity");
BOOST_CLASS_EXPORT_GUID(env::CExplodingHunterEntity, "CExplodingHunterEntity");
BOOST_CLASS_EXPORT_GUID(env::CExplodingHunterEmitterEntity, "CExplodingHunterEmitterEntity");
BOOST_CLASS_EXPORT_GUID(env::CExplosionEntity, "CExplosionEntity");
BOOST_CLASS_EXPORT_GUID(env::CBumperEntity, "CBumperEntity");
BOOST_CLASS_EXPORT_GUID(env::CBombEntityGun, "CBombEntityGun");
BOOST_CLASS_EXPORT_GUID(env::CPlatformDrone, "CPlatformDrone");
BOOST_CLASS_EXPORT_GUID(env::CSimpleLightEntity, "CSimpleLightEntity");
BOOST_CLASS_EXPORT_GUID(env::CRobotArmEntity, "CRobotArmEntity");
BOOST_CLASS_EXPORT_GUID(env::CRobotArmT2Entity, "CRobotArmT2Entity");
BOOST_CLASS_EXPORT_GUID(env::CMeshSwitchEntity, "CMeshSwitchEntity");
BOOST_CLASS_EXPORT_GUID(env::CAcceleratorClusterEntity, "CAcceleratorClusterEntity");
BOOST_CLASS_EXPORT_GUID(env::CSkyDomeEntity, "CSkyDomeEntity");
BOOST_CLASS_EXPORT_GUID(env::CSkyPlaneEntity, "CSkyPlaneEntity");
BOOST_CLASS_EXPORT_GUID(env::CPlanetEntity, "CPlanetEntity");
BOOST_CLASS_EXPORT_GUID(env::CLightSpriteEntity, "CLightSpriteEntity");
BOOST_CLASS_EXPORT_GUID(env::CSunEntity, "CSunEntity");
BOOST_CLASS_EXPORT_GUID(env::CPostProcessingEntity, "CPostProcessingEntity");
BOOST_CLASS_EXPORT_GUID(env::CCameraEntity, "CCameraEntity");
BOOST_CLASS_EXPORT_GUID(env::CPlayerCameraEntity, "CPlayerCameraEntity");
BOOST_CLASS_EXPORT_GUID(env::CHaloEntity, "CHaloEntity");
BOOST_CLASS_EXPORT_GUID(env::CSkyBoxEntity, "CSkyBoxEntity");
BOOST_CLASS_EXPORT_GUID(env::CStreamEntity, "CStreamEntity");
BOOST_CLASS_EXPORT_GUID(env::CStreamerGunEntity, "CStreamerGunEntity");
BOOST_CLASS_EXPORT_GUID(env::CBombEntity, "CBombEntity");
BOOST_CLASS_EXPORT_GUID(env::CMissileEntity, "CMissileEntity");
BOOST_CLASS_EXPORT_GUID(env::CMissileGunEntity, "CMissileGunEntity");
BOOST_CLASS_EXPORT_GUID(env::CPlanarPointParticleEmitterEntity, "CPlanarPointParticleEmitterEntity");
BOOST_CLASS_EXPORT_GUID(env::CLightningEntity, "CLightningEntity");
BOOST_CLASS_EXPORT_GUID(env::CHuntingHunterEntity, "CHuntingHunterEntity");
BOOST_CLASS_EXPORT_GUID(env::CRobotArmT3Entity, "CRobotArmT3Entity");
BOOST_CLASS_EXPORT_GUID(env::CMissileGunAimingEntity, "CMissileGunAimingEntity");
BOOST_CLASS_EXPORT_GUID(env::CWatcherVideoEntity, "CWatcherVideoEntity");
BOOST_CLASS_EXPORT_GUID(env::CTextEntity, "CTextEntity");
BOOST_CLASS_EXPORT_GUID(env::CSawbladeBossEntity, "CSawbladeBossEntity");
BOOST_CLASS_EXPORT_GUID(env::CBoolParamStorage, "CBoolParamStorage");
BOOST_CLASS_EXPORT_GUID(env::CFloatParamStorage, "CFloatParamStorage");
BOOST_CLASS_EXPORT_GUID(env::CStringParamStorage, "CStringParamStorage");
BOOST_CLASS_EXPORT_GUID(env::CIntParamStorage, "CIntParamStorage");
BOOST_CLASS_EXPORT_GUID(env::CVector2ParamStorage, "CVector2ParamStorage");
BOOST_CLASS_EXPORT_GUID(env::CVector3ParamStorage, "CVector3ParamStorage");
BOOST_CLASS_EXPORT_GUID(env::CVector4ParamStorage, "CVector4ParamStorage");
BOOST_CLASS_EXPORT_GUID(env::CVec4ParamStorage, "CVec4ParamStorage");
BOOST_CLASS_EXPORT_GUID(env::CCheckpointOriginEntity, "CCheckpointOriginEntity")
BOOST_CLASS_EXPORT_GUID(env::CCheckpointDestinatonEntity, "CCheckpointDestinatonEntity")
BOOST_CLASS_EXPORT_GUID(env::CEnvMapEntity, "CEnvMapEntity")
BOOST_CLASS_EXPORT_GUID(env::CStripesEntity, "CStripesEntity")
BOOST_CLASS_EXPORT_GUID(env::CTimelapseEntity, "CTimelapseEntity")
BOOST_CLASS_EXPORT_GUID(env::CSoundSpriteEntity, "CSoundSpriteEntity")
BOOST_CLASS_EXPORT_GUID(env::CBossEntity, "CBossEntity")
BOOST_CLASS_EXPORT_GUID(env::CWeaponEntity, "CWeaponEntity")
BOOST_CLASS_EXPORT_GUID(env::CProjectileEntity, "CProjectileEntity")
BOOST_CLASS_EXPORT_GUID(env::CProjectileGunEntity, "CProjectileGunEntity")
BOOST_CLASS_EXPORT_GUID(env::CLaserRadiatorAimingEntity, "CLaserRadiatorAimingEntity")
BOOST_CLASS_EXPORT_GUID(env::CCounterV2Entity, "CCounterV2Entity")
BOOST_CLASS_EXPORT_GUID(env::CAcceleratorBossEntity, "CAcceleratorBossEntity")



BOOST_CLASS_VERSION(env::CPlatformEntity, 3);
BOOST_CLASS_VERSION(env::CScenario, 9);
BOOST_CLASS_VERSION(env::CWorldMgr, 1);
BOOST_CLASS_VERSION(env::CTerrainMgrCPU, 3);
BOOST_CLASS_VERSION(env::CLaserRadiatorEntity, 8);
BOOST_CLASS_VERSION(env::PainterVertex, 2);
BOOST_CLASS_VERSION(env::LevelHalfCell, 3);
BOOST_CLASS_VERSION(env::CLightEntity, 6);
BOOST_CLASS_VERSION(env::CLight, 9);
BOOST_CLASS_VERSION(env::CCharacter2DEntity, 8);
BOOST_CLASS_VERSION(env::CMeshInstance, 10);
BOOST_CLASS_VERSION(env::CCheckpointEntity, 4);
BOOST_CLASS_VERSION(env::CCheckpointDestinatonEntity, 2);
BOOST_CLASS_VERSION(env::CParticleEmitterEntity, 11);
BOOST_CLASS_VERSION(env::CStreamerEntity, 1);
BOOST_CLASS_VERSION(env::Skyline, 3);
BOOST_CLASS_VERSION(env::CParticleSystemEntity, 2);
BOOST_CLASS_VERSION(env::CMagnetEntity, 2);
BOOST_CLASS_VERSION(env::CEntity, 4);
BOOST_CLASS_VERSION(env::CHunterBaseEntity, 1);
BOOST_CLASS_VERSION(env::Train, 2);
BOOST_CLASS_VERSION(env::CRobotArmEntity, 3);
BOOST_CLASS_VERSION(env::CPostProcessingEntity, 1);
BOOST_CLASS_VERSION(env::CFadeEntity, 3);
BOOST_CLASS_VERSION(env::Fog, 1);
BOOST_CLASS_VERSION(env::CLightSpriteEntity, 2);
BOOST_CLASS_VERSION(env::CExplosionEntity, 3);
BOOST_CLASS_VERSION(env::CBombEntityGun, 2);
BOOST_CLASS_VERSION(env::CStreamerGunEntity, 2);
BOOST_CLASS_VERSION(env::CStreamEntity, 2);
BOOST_CLASS_VERSION(env::CForceFieldEntity, 2);
BOOST_CLASS_VERSION(env::CBumperEntity, 2);
BOOST_CLASS_VERSION(env::CPlatformDrone, 2);
BOOST_CLASS_VERSION(env::CPlatformBaseEntity, 1);
BOOST_CLASS_VERSION(env::CSkyDomeEntity, 2);
BOOST_CLASS_VERSION(env::CPlanetEntity, 2);
BOOST_CLASS_VERSION(env::CSkyPlaneEntity, 1);
BOOST_CLASS_VERSION(env::CRobotArmT3Entity, 2);
BOOST_CLASS_VERSION(env::CPathWaypointEntity, 1);
BOOST_CLASS_VERSION(env::CSawbladeEntity, 1)
BOOST_CLASS_VERSION(env::CHuntingHunterEntity, 1)
BOOST_CLASS_VERSION(env::CMissileGunAimingEntity, 1)
BOOST_CLASS_VERSION(env::CAcceleratorClusterEntity, 2)
BOOST_CLASS_VERSION(env::CMagnetClusterEntity, 2)
BOOST_CLASS_VERSION(env::CMissileGunEntity, 1)
BOOST_CLASS_VERSION(env::CWatcherVideoEntity, 2);
BOOST_CLASS_VERSION(env::CSprite, 3);
BOOST_CLASS_VERSION(env::CSpriteEntity, 2);
BOOST_CLASS_VERSION(env::CSawbladeBossEntity, 1);
BOOST_CLASS_VERSION(env::CCameraEntity, 1);
BOOST_CLASS_VERSION(env::CTerrainMaterial, 2);
BOOST_CLASS_VERSION(env::CTextEntity, 1);
BOOST_CLASS_VERSION(env::CEntity::EntityOutput, 1);
BOOST_CLASS_VERSION(env::CCharacter3DEntity, 8);
BOOST_CLASS_VERSION(env::CEnvMapEntity, 1);
BOOST_CLASS_VERSION(env::StripeParameters, 1);
BOOST_CLASS_VERSION(env::CEntitySlider, 3);
BOOST_CLASS_VERSION(env::CRackWheelEntity, 1);
BOOST_CLASS_VERSION(env::CSpikeBallEntity, 1);
BOOST_CLASS_VERSION(env::CMeshEntity, 4);
BOOST_CLASS_VERSION(env::CMeshInstanceContainer, 2);
BOOST_CLASS_VERSION(env::CMaterial, 1);
BOOST_CLASS_VERSION(env::CStripesEntity, 1);
BOOST_CLASS_VERSION(env::CFadeEntity::FadeFrameParams, 1);
BOOST_CLASS_VERSION(env::CSoundSpriteEntity, 1);
BOOST_CLASS_VERSION(env::CBossEntity, 5);
BOOST_CLASS_VERSION(env::CWeaponEntity, 4);
BOOST_CLASS_VERSION(env::CProjectileEntity, 1);
BOOST_CLASS_VERSION(env::CBoxParticleEmitterEntity, 1);
BOOST_CLASS_VERSION(env::CExplodingHunterEntity, 1);
BOOST_CLASS_VERSION(env::CProjectileGunEntity, 1);
BOOST_CLASS_VERSION(env::CLaserRadiatorAimingEntity, 3);
BOOST_CLASS_VERSION(env::CCheckpointOriginEntity, 1);

BOOST_CLASS_VERSION(env::CPlayerMgr, 2);
#endif // ENV_PLATFORM_WINDOWS

namespace env
{
    //-----------------------------------------------------------------------------------
    CWorldMgr::CWorldMgr()
        : m_pTerrainMgr(nullptr)
        , m_pScenario(nullptr)
        , m_pActiveCamera(nullptr)
        , m_pDefaultCamera(nullptr)
        , m_bRunning(false)
        , m_bGameMode(true)
        , m_bPaused(false)
        , m_bLevelJustStarted(false)
        , m_bGameModeChanged(false)
        , m_bLevelFirstRun(false)
        , m_levelTime(0.0f)
        , m_timeMultiplier(1.0f)
        , m_version(0)
    {
        RegisterEntityTypes();
        RegisterParticleSystemTypes();
    }

    //-----------------------------------------------------------------------------------
    CWorldMgr::~CWorldMgr()
    {
        if (m_bRunning)
            Unload();
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::Load(const std::string& strLevelFilename, bool bGameMode)
    {
        m_bGameMode = bGameMode;
        m_bPaused = false;
        m_bGameModeChanged = false;

        LoadMap(strLevelFilename);
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::InitializeAtLoad()
    {
        ENV_LOG_INFO("INITIALIZING DATA STRUCTURES");

        m_pScenario->InitializeAtLoad();

        m_pTerrainMgr->GetLevelGrid().InitializeAtLoad();

        const RenderQualityParams& videoParams = gRenderSystem->GetRenderQualityParams();
        m_pDefaultCamera = new CCamera((float)videoParams.screenWidth / (float)videoParams.screenHeight, PI * 0.25f,
                                       videoParams.zNear, videoParams.zFar);

        gEngine->GetSceneMgr()->InitializeAtLoad();
        CSceneNode* camNode = gEngine->GetSceneMgr()->GetSceneNode("camNode");
        camNode->SetPosition(m_pDefaultCamera->GetDefaultPosition());
        m_pDefaultCamera->AttachNode(camNode);

        SetActiveCamera(m_pDefaultCamera);

        m_entitySlider.InitializeAtLoad();

        gEngine->GetEntityMgr()->InitializeAtLoad();


        SetGameMode(m_bGameMode);

        m_bRunning = true;
        m_bLevelJustStarted = true;
        m_bLevelFirstRun = true;

        OnResetDevice();


        ENV_LOG_INFO("INITIALIZING DATA STRUCTURES SUCCEEDED");
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::Load(int numGridsX, int numGridsY, bool bGameMode)
    {
        m_bGameMode = bGameMode;
        m_bPaused = false;
        m_bGameModeChanged = false;


        const RenderQualityParams& videoParams = gRenderSystem->GetRenderQualityParams();

        m_pDefaultCamera = new CCamera((float)videoParams.screenWidth /
                                       (float)videoParams.screenHeight, PI * 0.25f,
                                       videoParams.zNear, videoParams.zFar);

        CSceneNode* camNode = gEngine->GetSceneMgr()->CreateSceneNode("camNode", "rootNode");
        camNode->SetPosition(m_pDefaultCamera->GetDefaultPosition());
        m_pDefaultCamera->AttachNode(camNode);

        SetActiveCamera(m_pDefaultCamera);

        m_pScenario = new CScenario();


        m_pTerrainMgr = new CTerrainMgr();
        m_pTerrainMgr->GetLevelGrid().InitializeAtCreation(numGridsX, numGridsY);
        unsigned int cellsX, cellsY;
        float cellSize;
        m_pTerrainMgr->GetLevelGrid().GetSize(cellsX, cellsY, cellSize);

        m_entitySlider.InitializeAtCreation();

        gEngine->GetEntityMgr()->InitializeAtCreation();

        m_pScenario->InitializeAtCreation();


        SetGameMode(m_bGameMode);

        m_bRunning = true;

        ENV_LOG_INFO("WorldManager loaded.");


        OnResetDevice();

    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::InitializeAtCreation()
    {

    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::Unload()
    {
//         gMeshMgr->IncreaseRefCounter();
//         gTextureMgr->IncreaseRefCounter();
//         gMaterialMgr->IncreaseRefCounter();

        m_levelTime = 0.0f;
        m_version = 0;

        m_entitySlider.Reset();

        ENV_DELETE(m_pDefaultCamera);
        ENV_DELETE(m_pScenario);

        gEngine->GetEntityMgr()->Reset();
        gEngine->GetSceneMgr()->Reset(true);

        ENV_DELETE(m_pTerrainMgr);

        m_bRunning = false;
        m_bLevelJustStarted = false;

        ENV_LOG_INFO("WorldMgr unloaded.");
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::OnLostDevice()
    {
        if (!m_bRunning)
            return;

        gEngine->GetEntityMgr()->OnLostDevice();

        m_pTerrainMgr->GetLevelGrid().GetLevelRenderObject()->OnLostDevice();

    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::SetActiveCamera(CCamera* pCam)
    {
        m_pActiveCamera = pCam;
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::OnResetDevice()
    {
        ENV_LOG_INFO("WorldMgr::OnResetDevice");

        if (!m_bRunning)
            return;

        const RenderQualityParams& videoParams = gRenderSystem->GetRenderQualityParams();
        if (m_pDefaultCamera)
        {
            CFrustum& frustum = m_pDefaultCamera->GetFrustum();
            frustum.SetWidth(static_cast<float>(videoParams.screenWidth));
            frustum.SetHeight(static_cast<float>(videoParams.screenHeight));
            frustum.SetAspectRatio((float)(videoParams.screenWidth / (float)videoParams.screenHeight));
            m_pDefaultCamera->BuildProjectionMatrix();
        }

        gEngine->GetEntityMgr()->OnResetDevice();

        m_pTerrainMgr->GetLevelGrid().GetLevelRenderObject()->OnResetDevice();

        ENV_LOG_INFO("WorldMgr::OnResetDevice SUCCEEDED");
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::SetGameMode(bool bGameMode)
    {
        m_levelTime = 0.0f;
        m_bGameMode = bGameMode;

        gEngine->GetEntityMgr()->SetGameMode(m_bGameMode);

        gEngine->GetSceneMgr()->SetGameMode(m_bGameMode);

        m_timeMultiplier = 1.0f;

        if (m_pScenario)
            m_pScenario->OnGameModeChanged(m_bGameMode);

        m_bLevelJustStarted = true;

        SetPaused(false);
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::Update(float time, float dt, RenderQueue& renderQueue)
    {
        if (!m_bRunning)
            return;

        ENV_SCOPED_CPU_PROFILING(Engine, WorldMgr::Update);

        Vec3 vRayOrigin(m_pActiveCamera->GetAttachedNode()->GetDerivedPosition()),
            vRayDirection(0.0, 0.0, 0.0);

        ComputeViewingRayDirection(m_pActiveCamera->GetProjectionMatrix(), m_pActiveCamera->GetViewMatrix(),
                                   gRenderSystem->GetRenderQualityParams().screenWidth,
                                   gRenderSystem->GetRenderQualityParams().screenHeight,
                                   (float)gEngine->GetInput()->GetMouseX(), (float)gEngine->GetInput()->GetMouseY(), vRayDirection);


        m_levelTime += dt;

        // Update all subsystems 
        m_entitySlider.Update(dt, vRayOrigin, vRayDirection, m_pActiveCamera);

        dt *= m_timeMultiplier;

        if (!m_bPaused)
            gEngine->GetEntityMgr()->Update(dt, renderQueue);
        else
            gEngine->GetEntityMgr()->Update(0, renderQueue);

        gEngine->GetSceneMgr()->Update(dt);

        m_pActiveCamera->Update();

        m_pActiveCamera->AddToRenderQueue(dt, renderQueue);

        gEngine->GetSceneMgr()->AddToRenderQueue(renderQueue, dt);

        m_pScenario->AddToRenderQueue(renderQueue);

        m_pTerrainMgr->AddToRenderQueue(renderQueue);

        m_bLevelJustStarted = false;
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::SaveMap(const std::string& strMapFilepath)
    {
        SetGameMode(false);

        ENV_LOG_INFO("SAVING LEVEL: " + strMapFilepath);


        gEngine->GetEntityMgr()->PrepareToSave();

#ifdef ENV_PLATFORM_WINDOWS
        try
        {
#ifdef ENV_SERIALIZATION_SAVE_TEXT_ARCHIVE
            std::ofstream ofs(strMapFilepath);
            boost::archive::text_oarchive oa(ofs);
#endif 
#ifdef ENV_SERIALIZATION_SAVE_BINARY_ARCHIVE
            std::ofstream ofs(strMapFilepath, std::ios::binary);
            boost::archive::binary_oarchive oa(ofs);
#endif
            oa & *gEngine->GetSceneMgr();
            oa & *gEngine->GetEntityMgr();
            oa & *this;
        }
        catch (std::exception& exc)
        {
            ENV_LOG_ERROR(std::string("SAVING LEVEL FAILED! (") + exc.what() + ")");

            MessageBox(0, "[ERROR] Saving level failed.", "ERROR", MB_OK);
        }
#endif // ENV_PLATFORM_WINDOWS

        ENV_LOG_INFO("SAVING LEVEL FILE SUCCEEDED");
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::LoadMap(const std::string& strLevelFilename)
    {
        ENV_LOG_INFO("READING LEVEL FILE: " + strLevelFilename);

#ifdef ENV_PLATFORM_WINDOWS
        try
        {
#ifdef ENV_SERIALIZATION_LOAD_TEXT_ARCHIVE
            std::ifstream ifs(strLevelFilename);
            boost::archive::text_iarchive ia(ifs);
#endif 
#ifdef ENV_SERIALIZATION_LOAD_BINARY_ARCHIVE
            std::ifstream ifs(strLevelFilename, std::ios::binary);
            boost::archive::binary_iarchive ia(ifs);
#endif
            ia & *gEngine->GetSceneMgr();
            ia & *gEngine->GetEntityMgr();
            ia & *this;
        }
        catch (boost::archive::archive_exception  exc)
        {
            ENV_LOG_ERROR(std::string("READING LEVEL FILE FAILED! (") + exc.what() + ")");
        }
#endif // ENV_PLATFORM_WINDOWS

        ENV_LOG_INFO("READING LEVEL FILE SUCCEEDED");

        gEngine->GetSceneMgr()->GetRootNode()->PropagateTransform();
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::RegisterEntityTypes()
    {
        gEngine->GetEntityMgr()->RegisterEntityType< CSawbladeEntity >("Sawblade");
        gEngine->GetEntityMgr()->RegisterEntityType< CMeshEntity >("Mesh");
        gEngine->GetEntityMgr()->RegisterEntityType< CSpriteEntity >("Sprite");
        gEngine->GetEntityMgr()->RegisterEntityType< CParticleSystemEntity >("Particle System");
        gEngine->GetEntityMgr()->RegisterEntityType< CPathWaypointEntity >("Waypoint");
        gEngine->GetEntityMgr()->RegisterEntityType< CPathEntity >("Path");
        gEngine->GetEntityMgr()->RegisterEntityType< CCheckpointEntity >("Checkpoint[Old]");
        gEngine->GetEntityMgr()->RegisterEntityType< CCheckpointOriginEntity >("Checkpoint[Origin]");
        gEngine->GetEntityMgr()->RegisterEntityType< CCheckpointDestinatonEntity >("Checkpoint[Destination]");
        gEngine->GetEntityMgr()->RegisterEntityType< CRotatorEntity >("Rotator");
        gEngine->GetEntityMgr()->RegisterEntityType< CHuntingHunterEntity >("Hunter");
        gEngine->GetEntityMgr()->RegisterEntityType< CStreamerEntity >("Streamer");
        gEngine->GetEntityMgr()->RegisterEntityType< CSpikeBallEntity >("SpikeBall");
        gEngine->GetEntityMgr()->RegisterEntityType< CPolygonEntity >("Polygon");
        gEngine->GetEntityMgr()->RegisterEntityType< CParticleEmitterEntity >("PE: Point");
        gEngine->GetEntityMgr()->RegisterEntityType< CAcceleratorEntity >("Accelerator");
        gEngine->GetEntityMgr()->RegisterEntityType< CPlatformBaseEntity >("Platform");
        gEngine->GetEntityMgr()->RegisterEntityType< CCharacter2DEntity >("Character[2D]");
        gEngine->GetEntityMgr()->RegisterEntityType< CCharacter3DEntity >("Character[3D]");
        gEngine->GetEntityMgr()->RegisterEntityType< CSmallRocketEntity >("Rocket", false);
        gEngine->GetEntityMgr()->RegisterEntityType< CFadeEntity >("Screen Fade");
        gEngine->GetEntityMgr()->RegisterEntityType< CLaserRadiatorEntity >("LaserRadiator");
        gEngine->GetEntityMgr()->RegisterEntityType< CForceFieldEntity >("Force Field");
        gEngine->GetEntityMgr()->RegisterEntityType< CMagnetEntity >("Magnet");
        gEngine->GetEntityMgr()->RegisterEntityType< CMagnetClusterEntity >("Magnet Cluster");
        gEngine->GetEntityMgr()->RegisterEntityType< CRackWheelEntity >("Rack Wheel");
        gEngine->GetEntityMgr()->RegisterEntityType< CTriggerEntity >("Trigger", true);
        gEngine->GetEntityMgr()->RegisterEntityType< CLightningEntity >("Lightning");
        gEngine->GetEntityMgr()->RegisterEntityType< CBumperEntity >("Bumper");
        gEngine->GetEntityMgr()->RegisterEntityType< CSphereParticleEmitterEntity >("PE: Sphere");
        gEngine->GetEntityMgr()->RegisterEntityType< CBoxParticleEmitterEntity >("PE: Box");
        gEngine->GetEntityMgr()->RegisterEntityType< CPlanarPointParticleEmitterEntity >("PE: Planar Point");
        gEngine->GetEntityMgr()->RegisterEntityType< CExplodingHunterEntity >("Exploding Hunter");
        gEngine->GetEntityMgr()->RegisterEntityType< CExplosionEntity >("Explosion", true);
        gEngine->GetEntityMgr()->RegisterEntityType< CExplodingHunterEmitterEntity >("Exploding Hunter Emitter: T1");
        gEngine->GetEntityMgr()->RegisterEntityType< CPlatformDrone >("Platform Drone");
        gEngine->GetEntityMgr()->RegisterEntityType< CRobotArmT2Entity >("Robot Arm: T2", false);
        gEngine->GetEntityMgr()->RegisterEntityType< CExplodingHunterEmitterEntityT2 >("Exploding Hunter Emitter: T2");
        gEngine->GetEntityMgr()->RegisterEntityType< CMeshSwitchEntity >("Switch");
        gEngine->GetEntityMgr()->RegisterEntityType< CAcceleratorClusterEntity >("Accelerator Cluster");
        gEngine->GetEntityMgr()->RegisterEntityType< CSkyDomeEntity >("Sky Dome");
        gEngine->GetEntityMgr()->RegisterEntityType< CSkyPlaneEntity >("Sky Plane");
        gEngine->GetEntityMgr()->RegisterEntityType< CPlanetEntity >("Planet");
        gEngine->GetEntityMgr()->RegisterEntityType< CSunEntity >("Sun");
        gEngine->GetEntityMgr()->RegisterEntityType< CLightSpriteEntity >("Light");
		gEngine->GetEntityMgr()->RegisterEntityType< CSoundSpriteEntity >("Sound");
        gEngine->GetEntityMgr()->RegisterEntityType< CPostProcessingEntity >("Post Processing");
        gEngine->GetEntityMgr()->RegisterEntityType< CCameraEntity >("Camera");
        gEngine->GetEntityMgr()->RegisterEntityType< CPlayerCameraEntity >("Player Camera");
        gEngine->GetEntityMgr()->RegisterEntityType< CHaloEntity >("Halo");
        gEngine->GetEntityMgr()->RegisterEntityType< CSkyBoxEntity >("Sky Box");
        gEngine->GetEntityMgr()->RegisterEntityType< CStreamEntity >("Stream", false);
        gEngine->GetEntityMgr()->RegisterEntityType< CStreamerGunEntity >("Streamer Gun");
        gEngine->GetEntityMgr()->RegisterEntityType< CMissileEntity >("Missile", false);
        gEngine->GetEntityMgr()->RegisterEntityType< CMissileGunEntity >("Missile Gun");
        gEngine->GetEntityMgr()->RegisterEntityType< CEnergyFieldEntity >("Energy Field");
        gEngine->GetEntityMgr()->RegisterEntityType< CRobotArmT3Entity >("Robot Arm: T3");
        gEngine->GetEntityMgr()->RegisterEntityType< CUniversePlanetEntity >("Universe Planet", false);
        gEngine->GetEntityMgr()->RegisterEntityType< CMissileGunAimingEntity >("Missile Gun Aiming");
        gEngine->GetEntityMgr()->RegisterEntityType< CWatcherVideoEntity >("Watcher Video");
        gEngine->GetEntityMgr()->RegisterEntityType< CTextEntity >("Text");
        gEngine->GetEntityMgr()->RegisterEntityType< CSawbladeBossEntity >("Sawblade (Boss)");
        gEngine->GetEntityMgr()->RegisterEntityType< CEnvMapEntity >("Env Map");
        gEngine->GetEntityMgr()->RegisterEntityType< CStripesEntity >("Screen Stripes");
        gEngine->GetEntityMgr()->RegisterEntityType< CTimelapseEntity >("Timelapse");
        gEngine->GetEntityMgr()->RegisterEntityType< CBossEntity >("Boss");
        gEngine->GetEntityMgr()->RegisterEntityType< CWeaponEntity >("Weapon");
        gEngine->GetEntityMgr()->RegisterEntityType< CProjectileEntity >("Projectile");
        gEngine->GetEntityMgr()->RegisterEntityType< CProjectileGunEntity >("Projectile Gun");
        gEngine->GetEntityMgr()->RegisterEntityType< CLaserRadiatorAimingEntity >("Laser Radiator Aiming");
        gEngine->GetEntityMgr()->RegisterEntityType< CCounterV2Entity >("Counter");
        gEngine->GetEntityMgr()->RegisterEntityType< CAcceleratorBossEntity>("Accelerator Boss");
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::RegisterParticleSystemTypes()
    {
        gEngine->GetParticleSystemMgr()->RegisterAffector< CParticleSunlightAffector >("SunlightAffector");
        gEngine->GetParticleSystemMgr()->RegisterAffector< CParticleTextureVelocityRotator >("VelocityRotator");
        gEngine->GetParticleSystemMgr()->RegisterAffector< CParticleTextureRandomRotator >("RandomRotator");
    }

    //-----------------------------------------------------------------------------------
    void CWorldMgr::SetPaused(bool bPause)
    {
        m_bPaused = bPause;
    }

} // env