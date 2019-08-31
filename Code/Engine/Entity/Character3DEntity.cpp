
#include <Engine/Entity/Character3DEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Entity/PlatformEntity.h>
#include <Engine/Entity/BumperEntity.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/PlayerMgr.h>
#include <Engine/Core/PolygonMousePicker.h>
#include <Engine/Scene/TerrainMgr.h>


namespace env
{

    //-----------------------------------------------------------------------------------
    float ComputeAngle2DXAxis(const Vec2& v)
    {
        const Vec2 axis(1.0f, 0.0f);
        Vec2 vMagnetDir(glm::normalize(v));
        float angle = acos(glm::dot(vMagnetDir, axis));
        if (vMagnetDir.y < 0.00f)
            angle = -angle;

        return angle;
    }

    const Vec3 CHARACTER_SCALING = Vec3(0.37f, 0.37f, 0.37f);

    //-----------------------------------------------------------------------------------
    CCharacter3DEntity::MovementParams::MovementParams()
        : m_playerAcceleration(CHAR_ACCELERATION)
        , m_playerMaxSpeedX(CHAR_MAX_SPEED_X)
        , m_playerMaxSpeedY(CHAR_MAX_SPEED_Y)
        , m_playerSuperSpeed(CHAR_SUPER_SPEED)
        , m_jumpGravity(CHAR_JUMP_GRAVITY)
        , m_jumpTime(CHAR_JUMP_TIME)
        , m_gravity(CHAR_GRAVITY)
        , m_initialPlayerMaxSpeedX(CHAR_MAX_SPEED_X)
        , m_initialPlayerMaxSpeedY(CHAR_MAX_SPEED_Y)
        , m_initialJumpGravity(CHAR_JUMP_GRAVITY)
        , m_initialPlayerAcceleration(CHAR_ACCELERATION) {}

    //-----------------------------------------------------------------------------------
    CCharacter3DEntity::MovementState::MovementState()
        : m_bJumpedFromWallFromRight(false)
        , m_bJumpedFromWallFromLeft(false)
        , m_bJumpedFromFloorOrRamp(false)
        , m_bSecondJump(false)
        , m_bSecondJumpBlocked(false)
        , m_bFirstJumpFrame(false)
        , m_bJumping(false)
        , m_bJumpingUp(false)
        , m_bJumpLocked(false)
        , m_bWatchingRight(true)
        , m_bMovementFlipInput(false)
        , m_bMovementDisabled(false)
        , m_bMovementSprintDisabled(false)
        , m_bMovementJumpDisabled(false)
        , m_bMovementDoubleJumpDisabled(false)
        , m_bMovementWallGrabDisabled(false)
        , m_bMovementWallJumpDisabled(false)
        , m_bFrozen(false)
        , m_bInForceField(false)
        , m_bOnMagnet(false)
        , m_bOnPlatform(false) {}

    //-----------------------------------------------------------------------------------
    CCharacter3DEntity::TransformState::TransformState()
        : m_bodyPosition(0.0f, 0.0f, 0.0f)
        , m_leg01Position(0.0f, 0.0f, 0.0f)
        , m_leg02Position(0.0f, 0.0f, 0.0f)
        , m_chestPosition(0.0f, 0.0f, 0.0f)
        , m_arm01Position(0.0f, 0.0f, 0.0f)
        , m_arm02Position(0.0f, 0.0f, 0.0f)
        , m_headPosition(0.0f, 0.0f, 0.0f) {}

    //-----------------------------------------------------------------------------------
    CCharacter3DEntity::AnimationState::AnimationState()
        : m_legRotation(0.0f)
        , m_armRotation(0.0f)
        , m_chestBreathe(0.0f)
        , m_timeShape(0.0f)
        , m_charHeadRoll(0.0f)
        , m_charChestRoll(0.0f)
        , m_charLeg01Roll(0.0f)
        , m_charLeg02Roll(0.0f)
        , m_bArmRightRotation(false)
        , m_bLegRightRotation(false) {}


    //-----------------------------------------------------------------------------------
    CCharacter3DEntity::LifeState::LifeState()
        : m_bKilled(false)
        , m_bGodmode(false)
        , m_killTime(CHAR_KILL_TIME)
        , m_spawnTime(CHAR_SPAWN_TIME)
        , m_currentKillTime(-1.0f)
        , m_currentSpawnTime(-1.0f) {}

    const float     CCharacter3DEntity::CHAR_SHAPE_TIME = 0.2f;
    const float     CCharacter3DEntity::CHAR_ACCELERATION = 1000;
    const float     CCharacter3DEntity::CHAR_GRAVITY = 1200;
    const float     CCharacter3DEntity::CHAR_JUMP_GRAVITY = 485;
    const float     CCharacter3DEntity::CHAR_JUMP_TIME = 0.50f;
    const float     CCharacter3DEntity::MAX_SPEED_ACCELERATION_LINE = 2.0f;
    const float     CCharacter3DEntity::CHAR_MAX_SPEED_X = 400.0f;
    const float     CCharacter3DEntity::CHAR_MAX_SPEED_Y = 1000.0f;
    const float     CCharacter3DEntity::CHAR_SUPER_SPEED = 1.55f;
    const float     CCharacter3DEntity::CHAR_KILL_TIME = 0.4f;
    const float     CCharacter3DEntity::CHAR_SPAWN_TIME = 0.4f;
    ENTITY_TYPEID   CCharacter3DEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_CHARACTER3D;

    //-----------------------------------------------------------------------------------
    CCharacter3DEntity::CCharacter3DEntity()
        : m_pBodyNode(nullptr)
        , m_pChestNode(nullptr)
        , m_pHeadNode(nullptr)
        , m_pLeg01Node(nullptr)
        , m_pLeg02Node(nullptr)
        , m_pArm01Node(nullptr)
        , m_pArm02Node(nullptr)
        //         , m_pHeadMesh(nullptr)
        //         , m_pChestMesh(nullptr)
        //         , m_pBodyMesh(nullptr)
        //         , m_pArm01Mesh(nullptr)
        //         , m_pArm02Mesh(nullptr)
        //         , m_pLeg01Mesh(nullptr)
        //         , m_pLeg02Mesh(nullptr)
        , m_pParticlesTerrainTrace(nullptr)
        , m_pParticlesDistortHitField(nullptr)
        , m_pParticlesDistortSuperField(nullptr)
        , m_pParticlesDistortTrace(nullptr)
        , m_pParticlesSuper(nullptr)
        , m_pParticlesSuperBlack(nullptr)
        , m_pParticlesDistortSpawn(nullptr)
        , m_pParticlesDistortDeath(nullptr)
        , m_pPlatformNode(nullptr)
        , m_pForceField(nullptr)
        , m_velocity(0.0f, 0.0f)
        , m_bCollision(true)
        , m_pMagnet(nullptr)
        , m_width(8)
        , m_height(19)
        , m_startPos(0.0f, 0.0f, 0.0f)
        , m_characterDoubleJumpColor(1.0f, 156.0f / 255.0f, 0.0f, 1.0f)
        , m_acceleratorTime(0.0f)
        , m_currentAcceleratorTime(0.0f)
        , m_diffuseMtrl(0.0f, 1.0f, 1.0f)
        , m_diffuseAlpha(1.0f)
        , m_silhouetteRadius(0.0f)
        , m_silhouetteOpacity(0.0f)
        , m_currentMagnetEnterTime(0.0f)
        , m_gainStrengthTime(-1.0f)
        , m_gainStrengthDuration(1.0f)
        , m_gainStrengthTime1(-1.0f)
        , m_gainStrengthDuration1(1.0f)
        , m_bFirstStart(true)
        , m_bTrace(true)
        , m_bBlackSkin(false)
        , m_bCells(true)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        RegisterEvent("[Character] Enable Movement", CHARACTER_EVENT_ENABLE_MOVEMENT);
        RegisterEvent("[Character] Disable Movement", CHARACTER_EVENT_DISABLE_MOVEMENT);
        RegisterEvent("[Character] Enable Sprint", CHARACTER_EVENT_ENABLE_SPRINT);
        RegisterEvent("[Character] Disable Sprint", CHARACTER_EVENT_DISABLE_SPRINT);
        RegisterEvent("[Character] Enable Jump", CHARACTER_EVENT_ENABLE_JUMP);
        RegisterEvent("[Character] Disable Jump", CHARACTER_EVENT_DISABLE_JUMP);
        RegisterEvent("[Character] Enable DoubleJump", CHARACTER_EVENT_ENABLE_DOUBLEJUMP);
        RegisterEvent("[Character] Disable DoubleJump", CHARACTER_EVENT_DISABLE_DOUBLEJUMP);
        RegisterEvent("[Character] Enable WallGrab", CHARACTER_EVENT_ENABLE_WALLGRAB);
        RegisterEvent("[Character] Disable WallGrab", CHARACTER_EVENT_DISABLE_WALLGRAB);
        RegisterEvent("[Character] Enable WallJump", CHARACTER_EVENT_ENABLE_WALLJUMP);
        RegisterEvent("[Character] Disable WallJump", CHARACTER_EVENT_DISABLE_WALLJUMP);
        RegisterEvent("[Character] Start Playback", CHARACTER_EVENT_STARTPLAYBACK);
        RegisterEvent("[Character] Flip Input", CHARACTER_EVENT_FLIP_INPUT);
        RegisterEvent("[Character] Disable Trace", CHARACTER_EVENT_DISABLE_TRACE);
        RegisterEvent("[Character] Enable Black Skin", CHARACTER_EVENT_ENABLE_BLACKSKIN);
        RegisterEvent("[Character] Enable Trace", CHARACTER_EVENT_ENABLE_TRACE);
        RegisterEvent("[Character] Disable Black Skin", CHARACTER_EVENT_DISABLE_BLACKSIN);
        RegisterEvent("[Character] Gain Strength 0", CHARACTER_EVENT_GAIN_STRENGTH_0);
        RegisterEventParameter(CHARACTER_EVENT_GAIN_STRENGTH_0, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f);
        RegisterEvent("[Character] Gain Strength 1", CHARACTER_EVENT_GAIN_STRENGTH_1);
        RegisterEventParameter(CHARACTER_EVENT_GAIN_STRENGTH_1, "Duration", CBaseParam::PARAMETER_TYPE_FLOAT, 1.0f);
        RegisterEvent("[Character] Disable Cells", CHARACTER_EVENT_DISABLE_CELLS);
        m_charState.m_bGodmode = false;

        //SetGhost(m_charState.m_bGodmode);
    }

    //-----------------------------------------------------------------------------------
    CCharacter3DEntity::~CCharacter3DEntity()
    {
        std::vector< std::string > characterSoundFiles;
        characterSoundFiles.resize(CHARACTER_SOUND_END);

        characterSoundFiles[CHARACTER_SOUND_MOVE] = "CharacterMove.wav";
        characterSoundFiles[CHARACTER_SOUND_JUMP] = "CharacterJump.wav";
        characterSoundFiles[CHARACTER_SOUND_SECONDJUMP] = "CharacterSecondJump.wav";
        characterSoundFiles[CHARACTER_SOUND_HITFLOOR] = "CharacterHitFloor.wav";
        characterSoundFiles[CHARACTER_SOUND_SLIDEWALL] = "CharacterSlideWall.wav";
        characterSoundFiles[CHARACTER_SOUND_START] = "CharacterStart.wav";
        characterSoundFiles[CHARACTER_SOUND_FINISH] = "CharacterFinish.wav";
        characterSoundFiles[CHARACTER_SOUND_DIE] = "CharacterDie.wav";
        characterSoundFiles[CHARACTER_SOUND_FLY] = "CharacterFly.wav";
        characterSoundFiles[CHARACTER_SOUND_MAGNET_HOVER] = "CharacterMagnetHover.wav";
        characterSoundFiles[CHARACTER_SOUND_MAGNET_LAUNCH] = "CharacterMagnetLaunch.wav";

        for (int i = 0; i < m_characterSounds.size(); ++i)
        {
            gEngine->GetAudioSystem()->Release(characterSoundFiles[i]);
        }

        CEntityMgr* pEntityMgr = gEngine->GetEntityMgr();
        CSceneMgr*  pSceneMgr = gEngine->GetSceneMgr();

        //         pSceneMgr->RemoveMeshInstance(m_pHeadMesh);
        //         pSceneMgr->RemoveMeshInstance(m_pChestMesh);
        //         pSceneMgr->RemoveMeshInstance(m_pBodyMesh);
        //         pSceneMgr->RemoveMeshInstance(m_pArm01Mesh);
        //         pSceneMgr->RemoveMeshInstance(m_pArm02Mesh);
        //         pSceneMgr->RemoveMeshInstance(m_pLeg01Mesh);
        //         pSceneMgr->RemoveMeshInstance(m_pLeg02Mesh);
        pSceneMgr->RemoveNode(m_pLeg01Node);
        pSceneMgr->RemoveNode(m_pLeg02Node);
        pSceneMgr->RemoveNode(m_pArm01Node);
        pSceneMgr->RemoveNode(m_pArm02Node);
        pSceneMgr->RemoveNode(m_pHeadNode);
        pSceneMgr->RemoveNode(m_pChestNode);
        pSceneMgr->RemoveNode(m_pBodyNode);
        pSceneMgr->RemoveNode(m_pNode);

        pEntityMgr->RemoveEntity(m_pParticlesDistortDeath);
        pEntityMgr->RemoveEntity(m_pParticlesDistortSpawn);
        pEntityMgr->RemoveEntity(m_pParticlesDistortSuperField);
        pEntityMgr->RemoveEntity(m_pParticlesDistortTrace);
        pEntityMgr->RemoveEntity(m_pParticlesDistortHitField);
        pEntityMgr->RemoveEntity(m_pParticlesTerrainTrace);
        pEntityMgr->RemoveEntity(m_pParticlesSuperBlack);
        pEntityMgr->RemoveEntity(m_pParticlesSuper);

        gEngine->GetEntityMgr()->SetColliderEntity(nullptr);
        gEngine->GetEntityMgr()->SetCharacterEntity(nullptr);
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::OnLevelFinished()
    {
        OnInput(CCharacter3DEntity::CHARACTER_EVENT_ENABLE_FREEZE, EventParameter());
        SetGhost(true);
        ForceShape();
        m_characterSounds[CHARACTER_SOUND_FINISH]->PlaySound();
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::LoadScriptValues()
    {
        m_movementParams.m_playerAcceleration = CHAR_ACCELERATION;
        m_movementParams.m_initialPlayerAcceleration = m_movementParams.m_playerAcceleration;

        m_movementParams.m_playerMaxSpeedX = CHAR_MAX_SPEED_X;
        m_movementParams.m_initialPlayerMaxSpeedX = m_movementParams.m_playerMaxSpeedX;

        m_movementParams.m_playerMaxSpeedY = CHAR_MAX_SPEED_Y;
        m_movementParams.m_initialPlayerMaxSpeedY = m_movementParams.m_playerMaxSpeedY;

        m_movementParams.m_jumpGravity = CHAR_JUMP_GRAVITY;
        m_movementParams.m_initialJumpGravity = m_movementParams.m_jumpGravity;

        m_movementParams.m_jumpTime = CHAR_JUMP_TIME;
        m_movementParams.m_gravity = CHAR_GRAVITY;
        m_movementParams.m_playerSuperSpeed = CHAR_SUPER_SPEED;

    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::InitializeAtCreation()
    {
        CEntity::InitializeAtCreation();

        m_pNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pNode->Serialize();
        gEngine->GetSceneMgr()->GetRootNode()->AttachChildNode(m_pNode);

        m_pLevelMgr = gEngine->GetWorldMgr()->GetLevelMgr();

        LoadScriptValues();

        m_pBodyNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pChestNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pHeadNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pLeg01Node = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pLeg02Node = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pArm01Node = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pArm02Node = gEngine->GetSceneMgr()->CreateSceneNode();

        m_pNode->AttachChildNode(m_pBodyNode);
        m_pBodyNode->AttachChildNode(m_pChestNode);
        m_pBodyNode->AttachChildNode(m_pLeg01Node);
        m_pBodyNode->AttachChildNode(m_pLeg02Node);
        m_pChestNode->AttachChildNode(m_pHeadNode);
        m_pChestNode->AttachChildNode(m_pArm01Node);
        m_pChestNode->AttachChildNode(m_pArm02Node);

        InitHitbox();

        InitMeshes();
        InitSprites();

        InitParticleSystems();

        InitSounds();

        m_startPos = m_pNode->GetDerivedPosition();

        gEngine->GetEntityMgr()->SetColliderEntity(this);
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::InitializeAtLoad()
    {
        CEntity::InitializeAtLoad();

        m_pLevelMgr = gEngine->GetWorldMgr()->GetLevelMgr();

        LoadScriptValues();

        InitHitbox();

        InitMeshes();
        InitSprites();

        InitParticleSystems();

        InitSounds();

        gEngine->GetEntityMgr()->SetColliderEntity(this);

        SetGhost(false);

        m_charMtrl = UndoGamma(Vec3(19.0f / 255.0f, 30.0f / 255.0f, 177.0f / 255.0f));
        m_diffuseMtrl = UndoGamma(Vec3(74.0f / 255.0f, 222.0f / 255.0f, 229.0f / 255.0f));
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::InitHitbox(bool bCreate)
    {
        if (bCreate && GetNumPolygons() > 0)
            return;

        RemoveCircles();
        RemovePolygons();

        if (bCreate)
        {
            CPolygon p(4);
            p.SetPoint(0, Vec3(-m_width, -m_height, 0.0));
            p.SetPoint(1, Vec3(-m_width, m_height, 0.0));
            p.SetPoint(2, Vec3(m_width, m_height, 0.0));
            p.SetPoint(3, Vec3(m_width, -m_height, 0.0));
            AddPolygon(p);
        }
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::InitParticleSystems()
    {
        if (m_pParticlesTerrainTrace == nullptr)
            m_pParticlesTerrainTrace = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);

        m_pParticlesTerrainTrace->LoadFromTemplate("CHARACTER_FLOOR_TRACE");
        m_pParticlesTerrainTrace->SetRender(false);
        m_pParticlesTerrainTrace->SetPickable(false);
        m_pParticlesTerrainTrace->SetEmitNewParticles(false);


        if (m_pParticlesDistortHitField == nullptr)
            m_pParticlesDistortHitField = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);

        m_pParticlesDistortHitField->LoadFromTemplate("CHAR_DIST_FIELD");
        m_pParticlesDistortHitField->SetRender(false);
        m_pParticlesDistortHitField->SetPickable(false);
        m_pParticlesDistortHitField->SetEmitNewParticles(false);


        if (m_pParticlesDistortTrace == nullptr)
            m_pParticlesDistortTrace = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);

        m_pParticlesDistortTrace->LoadFromTemplate("CHAR_DIST_SUPER_TRACE");
        m_pParticlesDistortTrace->SetRender(false);
        m_pParticlesDistortTrace->SetPickable(false);
        m_pParticlesDistortTrace->SetEmitNewParticles(false);


        if (m_pParticlesDistortSuperField == nullptr)
            m_pParticlesDistortSuperField = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);

        m_pParticlesDistortSuperField->LoadFromTemplate("CHAR_COMB_FIELD");
        m_pParticlesDistortSuperField->SetRender(false);
        m_pParticlesDistortSuperField->SetPickable(false);
        m_pParticlesDistortSuperField->SetEmitNewParticles(false);


        if (m_pParticlesSuper == nullptr)
            m_pParticlesSuper = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);

        m_pParticlesSuper->LoadFromTemplate("CHARACTER_SUPER");
        m_pParticlesSuper->SetRender(false);
        m_pParticlesSuper->SetPickable(false);
        m_pParticlesSuper->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));

        if (m_pParticlesSuperBlack == nullptr)
            m_pParticlesSuperBlack = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);

        m_pParticlesSuperBlack->LoadFromTemplate("CHARACTER_SUPER_BLACK");
        m_pParticlesSuperBlack->SetRender(false);
        m_pParticlesSuperBlack->SetPickable(false);
        m_pParticlesSuperBlack->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));


        if (m_pParticlesDistortSpawn == nullptr)
            m_pParticlesDistortSpawn = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);

        m_pParticlesDistortSpawn->LoadFromTemplate("CHAR_SPAWN");
        m_pParticlesDistortSpawn->SetRender(false);
        m_pParticlesDistortSpawn->SetPickable(false);
        m_pParticlesDistortSpawn->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));


        if (m_pParticlesDistortDeath == nullptr)
            m_pParticlesDistortDeath = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);

        m_pParticlesDistortDeath->LoadFromTemplate("CHAR_DEATH");
        m_pParticlesDistortDeath->SetRender(false);
        m_pParticlesDistortDeath->SetPickable(false);
        m_pParticlesDistortDeath->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::InitSounds()
    {
        std::vector< std::string > characterSoundFiles;
        characterSoundFiles.resize(CHARACTER_SOUND_END);

        characterSoundFiles[CHARACTER_SOUND_MOVE] = "CharacterMove.wav";
        characterSoundFiles[CHARACTER_SOUND_JUMP] = "CharacterJump.wav";
        characterSoundFiles[CHARACTER_SOUND_SECONDJUMP] = "CharacterSecondJump.wav";
        characterSoundFiles[CHARACTER_SOUND_HITFLOOR] = "CharacterHitFloor.wav";
        characterSoundFiles[CHARACTER_SOUND_SLIDEWALL] = "CharacterSlideWall.wav";
        characterSoundFiles[CHARACTER_SOUND_START] = "CharacterStart.wav";
        characterSoundFiles[CHARACTER_SOUND_FINISH] = "CharacterFinish.wav";
        characterSoundFiles[CHARACTER_SOUND_DIE] = "CharacterDie.wav";
        characterSoundFiles[CHARACTER_SOUND_FLY] = "CharacterFly.wav";
        characterSoundFiles[CHARACTER_SOUND_MAGNET_HOVER] = "CharacterMagnetHover.wav";
        characterSoundFiles[CHARACTER_SOUND_MAGNET_LAUNCH] = "CharacterMagnetLaunch.wav";

        m_characterSounds.resize(CHARACTER_SOUND_END);


        for (int i = 0; i < m_characterSounds.size(); ++i)
        {
            m_characterSounds[i] = gEngine->GetAudioSystem()->AddGetPtr(characterSoundFiles[i]);
            m_characterSounds[i]->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);
            m_characterSounds[i]->SetVolume(1.0f);
        }

        m_characterSounds[CHARACTER_SOUND_MOVE]->SetVolume(8.0f);
        m_characterSounds[CHARACTER_SOUND_DIE]->SetVolume(0.5f);

        m_characterSounds[CHARACTER_SOUND_START]->SetVolume(0.4f);
        m_characterSounds[CHARACTER_SOUND_FINISH]->SetVolume(0.6f);
		m_characterSounds[CHARACTER_SOUND_JUMP]->SetVolume(0.3f);
		m_characterSounds[CHARACTER_SOUND_SECONDJUMP]->SetVolume(0.4f);
        m_characterSounds[CHARACTER_SOUND_SECONDJUMP]->SetSpeed(1.3f);
        m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->SetVolume(0.8f);
//		m_characterSounds[CHARACTER_SOUND_HITFLOOR]->SetVolume(0.3f);
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::InitMeshes()
    {
        m_meshInstContainer.Destroy();
        m_meshInstContainer.InitFromMeshContainer("character.x");

        m_meshInstContainer.CallForEach([](const auto& meshInst) { meshInst->GetMaterial().SetRenderCategory(MESH_RENDER_CATEGORY_CHARACTER); });

        m_meshInstContainer.GetMeshInstance("Head")->AttachNode(m_pHeadNode);
        m_meshInstContainer.GetMeshInstance("Body")->AttachNode(m_pBodyNode);
        m_meshInstContainer.GetMeshInstance("Chest")->AttachNode(m_pChestNode);
        m_meshInstContainer.GetMeshInstance("Arm01")->AttachNode(m_pArm01Node);
        m_meshInstContainer.GetMeshInstance("Arm02")->AttachNode(m_pArm02Node);
        m_meshInstContainer.GetMeshInstance("Leg01")->AttachNode(m_pLeg01Node);
        m_meshInstContainer.GetMeshInstance("Leg02")->AttachNode(m_pLeg02Node);

        m_meshInstContainer.CallForEach([](const auto& meshInst) { meshInst->GetMaterial().SetRender(false); });
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::InitSprites()
    {
        InitSprites("Sprites/character.png",
                    m_spriteCharHead, m_spriteCharChest,
                    m_spriteCharBody, m_spriteCharArm01, 
                    m_spriteCharArm02, m_spriteCharLeg01, 
                    m_spriteCharLeg02);

        InitSprites("Sprites/characterBlack.png",
                    m_spriteCharHeadBlack, m_spriteCharChestBlack,
                    m_spriteCharBodyBlack, m_spriteCharArm01Black,
                    m_spriteCharArm02Black, m_spriteCharLeg01Black,
                    m_spriteCharLeg02Black);
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::InitSprites(const std::string& strCharMaterial, 
                                         CSprite& spriteHead, 
                                         CSprite& spriteChest, 
                                         CSprite& spriteBody, 
                                         CSprite& spriteArm01, 
                                         CSprite& spriteArm02, 
                                         CSprite& spriteLeg01, 
                                         CSprite& spriteLeg02)
    {
        spriteHead.SetMaterialFilename(strCharMaterial);
        spriteChest.SetMaterialFilename(strCharMaterial);
        spriteBody.SetMaterialFilename(strCharMaterial);
        spriteArm01.SetMaterialFilename(strCharMaterial);
        spriteArm02.SetMaterialFilename(strCharMaterial);
        spriteLeg01.SetMaterialFilename(strCharMaterial);
        spriteLeg02.SetMaterialFilename(strCharMaterial);

        spriteHead.SetUpdateRectByAnimation(false);
        spriteChest.SetUpdateRectByAnimation(false);
        spriteBody.SetUpdateRectByAnimation(false);
        spriteArm01.SetUpdateRectByAnimation(false);
        spriteArm02.SetUpdateRectByAnimation(false);
        spriteLeg01.SetUpdateRectByAnimation(false);
        spriteLeg02.SetUpdateRectByAnimation(false);

        Vec3 color = Vec3(186.0f / 255.0f, 1.0f, 1.0f);
        spriteHead.SetColor(m_diffuseMtrl);
        spriteChest.SetColor(m_diffuseMtrl);
        spriteBody.SetColor(m_diffuseMtrl);
        spriteArm01.SetColor(m_diffuseMtrl);
        spriteArm02.SetColor(m_diffuseMtrl);
        spriteLeg01.SetColor(m_diffuseMtrl);
        spriteLeg02.SetColor(m_diffuseMtrl);

        //m_spriteCharGrapple.SetTextureFilename("Sprites/character.png");

        unsigned int width = m_spriteCharHead.GetMaterial()->GetWidth();
        unsigned int height = m_spriteCharHead.GetMaterial()->GetHeight();

        Vec3 center(0.0f, 0.0f, 0.0f);
        TextureRect rect;

        // Head
        rect.top = 0.0f;
        rect.left = 0.0f;
        rect.right = 64.0f / width;
        rect.bottom = 64.0f / height;
        spriteHead.SetLogicalRect(rect);
        center.x = 32.0f;
        center.y = 58.0f;
        spriteHead.SetCenter(center);

        // Chest
        rect.top = 0.0f;
        rect.left = 64.0f / width;
        rect.right = 128.0f / width;
        rect.bottom = 64.0f / height;
        spriteChest.SetLogicalRect(rect);
        spriteChest.SetCenter(center);

        // Body
        rect.top = 64.0f / height;
        rect.left = 0.0f;
        rect.right = 64.0f / width;
        rect.bottom = 128.0f / height;
        spriteBody.SetLogicalRect(rect);
        spriteBody.SetCenter(center);

        // Arm01
        rect.top = 64.0f / height;
        rect.left = 64.0f / height;
        rect.right = 128.0f / width;
        rect.bottom = 128.0f / height;
        spriteArm01.SetLogicalRect(rect);
        center.x = 32.0f;
        center.y = 18.0f;
        spriteArm01.SetCenter(center);

        // Arm02
        spriteArm02.SetLogicalRect(rect);
        spriteArm02.SetCenter(center);

        // Leg01
        rect.top = 128.0f / height;
        rect.left = 0.0f;
        rect.right = 64.0f / width;
        rect.bottom = 196.0f / height;
        spriteLeg01.SetLogicalRect(rect);
        center.x = 32.0f;
        center.y = 4.0f;
        spriteLeg01.SetCenter(center);

        // Leg02
        rect.top = 128.0f / height;
        rect.left = 0.0f;
        rect.right = 64.0f / width;
        rect.bottom = 196.0f / height;
        spriteLeg02.SetLogicalRect(rect);
        spriteLeg02.SetCenter(center);

        spriteBody.AttachNode(m_pBodyNode);
        spriteChest.AttachNode(m_pChestNode);
        spriteArm01.AttachNode(m_pArm01Node);
        spriteArm02.AttachNode(m_pArm02Node);
        spriteHead.AttachNode(m_pHeadNode);
        spriteLeg01.AttachNode(m_pLeg01Node);
        spriteLeg02.AttachNode(m_pLeg02Node);
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::SetPosition(const Vec3& position, bool bUpdatePrevFramePos)
    {
        m_pNode->SetPosition(position);

        const Vec3& charPos = m_pNode->GetDerivedPosition();
        Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), charPos));

        ApplyTransform(mtxTranslation);

        m_collisionState.pos = Convert2Fixed(position.x, position.y);

        if (bUpdatePrevFramePos)
            m_fixedPositionPrevFrame = m_collisionState.pos;
    }


    void CCharacter3DEntity::SetFixedPosition(const VECTOR2FIXED& pos)
    {
        m_collisionState.pos = pos;

        m_pNode->SetPosition(Vec3((float)m_collisionState.pos.x / (float)CELL_SCALE, (float)m_collisionState.pos.y / (float)CELL_SCALE, 0.0f));

        const Vec3& charPos = m_pNode->GetDerivedPosition();
        Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), charPos));
        ApplyTransform(mtxTranslation);
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::SetCollisionState(const CollisionState& collisionState)
    {
        m_collisionState = collisionState;
        SetFixedPosition(m_collisionState.pos);
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::OnGameModeChanged(bool bGameMode)
    {
        CEntity::OnGameModeChanged(bGameMode);
        timeReshape = -1.0f;
        m_bForceShape = false;
        m_movementState.m_bJumpingUp = false;
        if (gEngine->GetInput()->IsActionTriggered(ACTION_PRIMARY))
            m_movementState.m_bJumpLocked = true;

        m_pNode->SetScale(CHARACTER_SCALING);

        m_pBodyNode->SetOrientation(CQuaternion());

        m_movementInputState.m_bPrimaryKeyPressed = false;
        m_movementInputState.m_bSecondaryKeyPressed = false;
        m_movementInputState.m_bKeyLeftPressed = false;
        m_movementInputState.m_bKeyRightPressed = false;

        m_movementState.m_bSecondJump = false;
        m_movementState.m_bSecondJumpBlocked = false;
        m_movementState.m_bOnPlatform = false;
        m_movementState.m_bMovementFlipInput = false;
        m_movementState.m_bMovementDisabled = false;
        m_movementState.m_bMovementSprintDisabled = false;
        m_movementState.m_bMovementJumpDisabled = false;
        m_movementState.m_bMovementDoubleJumpDisabled = false;
        m_movementState.m_bMovementWallGrabDisabled = false;
        m_movementState.m_bMovementWallJumpDisabled = false;
        m_movementState.m_bFrozen = false;
        m_movementState.m_bInForceField = false;

        m_movementState.m_wallStickTimer.Reset();
        m_movementState.m_jumpTimer.Reset();
        m_movementState.m_jumpDelayTimer.Reset();
        m_movementState.m_jumpDelayTimer.Start();
        m_movementState.m_jumpDelayTimer.SetTime(5.0f);

        m_charState.m_bKilled = false;
        m_charState.m_currentKillTime = -1.0f;
        m_charState.m_currentSpawnTime = 0.25f;

        m_bAcceleratorBoss = false;
        m_bossAcceleratorTime = -1.0f;
        m_bossAcceleratorDuration = 5.0f;
        m_pAcceleratorBoss = nullptr;

        m_pPlatformNode = NULL;

        m_velocity.x = 0.0f;
        m_velocity.y = 0.0f;

        m_gainStrengthTime = -1.0f;
        m_gainStrengthDuration = 1.0f;

        m_gainStrengthTime1 = -1.0f;
        m_gainStrengthDuration1 = 1.0f;

        m_silhouetteRadius = 0.0f;
        m_silhouetteRadius = 0.0f;

        if (bGameMode)
        {
            CollisionState colRes;
            colRes.pos = Convert2Fixed(m_startPos.x, m_startPos.y);
            SetCollisionState(colRes);
        }

        if (bGameMode)
            InitHitbox(m_bCollision);
        else
            InitHitbox(true);

        m_acceleratorTime = 0.0f;
        m_currentAcceleratorTime = 0.0f;
        m_acceleratorForce = Vec2(0.0f, 0.0f);
        m_movementState.m_bOnMagnet = false;
        m_pMagnet = NULL;
        m_movementState.m_bOnPlatform = false;
        m_pPlatformNode = NULL;

        m_fixedPositionPrevFrame = m_collisionState.pos;

        m_platforms.clear();
        gEngine->GetEntityMgr()->GetEntities(m_platforms, ENTITY_TYPEID_PLATFORMBASE);
        gEngine->GetEntityMgr()->GetEntities(m_platforms, ENTITY_TYPEID_PLATFORMDRONE);

        if (bGameMode && CHARACTER_SOUND_START < m_characterSounds.size() && m_bFirstStart)
            m_characterSounds[CHARACTER_SOUND_START]->PlaySound();

        m_bFirstStart = false;

        m_prevMovementState = m_movementState;
        m_prevCollisionState = m_collisionState;

        if (bGameMode == false)
        {
            for (int i = 0; i < m_characterSounds.size(); ++i)
            {
                m_characterSounds[i]->StopSound();
            }
        }

        m_pParticlesDistortDeath->SetRender(false);
        m_pParticlesDistortHitField->SetRender(false);
        m_pParticlesDistortSpawn->SetRender(false);
        m_pParticlesTerrainTrace->SetRender(false);
        m_pParticlesDistortSuperField->SetRender(false);
        m_pParticlesSuper->SetRender(false);
        m_pParticlesSuperBlack->SetRender(false);
    }

    //-----------------------------------------------------------------------------------
    bool CCharacter3DEntity::HasCollision(CEntity& e)
    {
        if (GetGhost())
            return false;

        if (e.GetMetaTypeId() == ENTITY_TYPEID_CHECKPOINT && CGeometryCollidable::HasCollision(e))
        {
            return true;
        }
        if (e.GetMetaTypeId() == ENTITY_TYPEID_CHECKPOINT_ORIGIN && CGeometryCollidable::HasCollision(e))
        {
            return true;
        }
        if (e.GetMetaTypeId() == ENTITY_TYPEID_CHECKPOINT_DESTINATION && CGeometryCollidable::HasCollision(e))
        {
            return true;
        }
        if (e.GetMetaTypeId() == ENTITY_TYPEID_ACCELERATOR)
        {
            if (CGeometryCollidable::HasCollision(e))
                return true;
            else
                return false;
        }
        if (e.GetMetaTypeId() == ENTITY_TYPEID_ACCELERATOR_BOSS)
        {
            if (CGeometryCollidable::HasCollision(e))
                return true;
            else
                return false;
        }
        if (e.GetMetaTypeId() == ENTITY_TYPEID_MAGNET && CGeometryCollidable::HasCollision(e))
        {
            return true;
        }
        if (e.GetMetaTypeId() == ENTITY_TYPEID_BUMPER && CGeometryCollidable::HasCollision(e))
        {
            return true;
        }
        if (e.GetMetaTypeId() == ENTITY_TYPEID_TRIGGER && CGeometryCollidable::HasCollision(e))
        {
            return true;
        }
        if (e.GetMetaTypeId() == ENTITY_TYPEID_MESHSWITCH && CGeometryCollidable::HasCollision(e))
        {
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_SAWBLADE && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_LASERRADIATOR && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_LASERRADIATOR_AIMING && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_SPIKE && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_STREAM && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_SAWBLADEBOSS && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_WEAPON && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_HUNTINGHUNTER && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_EXPLODINGHUNTER && CGeometryCollidable::HasCollision(e))
        {
            //SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_FIREBALL && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_METALSHARD && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_RACKWHEEL && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_MAGNETCLUSTER && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_GATE && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_MAGNET)
        {
            if (!CGeometryCollidable::HasCollision(e))
            {
                /*if((m_bOnMagnet && m_pMagnet == &e) || !m_bOnMagnet)
                {
                m_pMagnet = NULL;
                m_bOnMagnet = false;
                }*/


                return false;
            }
            else
                return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_SPIKEBALL && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_FORCEFIELD && CGeometryCollidable::HasCollision(e))
        {
            m_movementState.m_bJumpingUp = false;
            m_movementState.m_bJumping = false;

            m_movementState.m_bInForceField = true;
            m_pForceField = &static_cast<CForceFieldEntity&>(e);

            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_BURNER && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);

            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_SCYTHE && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);

            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_EXPLOSION && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);

            return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    bool CCharacter3DEntity::HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode)
    {
        if (!GetPickable())
            return false;

        if (CEntity::HasCollisionWithRay(vRayOrigin, vRayDirection, distance, ppNode))
            return true;

        CPolygonMousePicker mousePicker;
        mousePicker.AddPoint(Vec3(-m_width, -m_height, 0.0));
        mousePicker.AddPoint(Vec3(-m_width, m_height, 0.0));
        mousePicker.AddPoint(Vec3(m_width, m_height, 0.0));
        mousePicker.AddPoint(Vec3(m_width, -m_height, 0.0));

        const Vec3& charPos = m_pNode->GetDerivedPosition();
        Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), charPos));

        *ppNode = m_pNode;
        return mousePicker.CheckPicked(mtxTranslation, gEngine->GetWorldMgr()->GetActiveCamera(), &distance);
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::OnTriggerEntered(CEntity& e)
    {
        if (e.GetMetaTypeId() == ENTITY_TYPEID_ACCELERATOR || e.GetMetaTypeId() == ENTITY_TYPEID_BUMPER)
        {
            if (e.GetMetaTypeId() == ENTITY_TYPEID_BUMPER)
            {
                CBumperEntity& bumper = static_cast<CBumperEntity&>(e);
                if (bumper.GetCurrentBumpTime() > 0.0f)
                    return;

                bumper.Bump();
            }

            CAcceleratorEntity& accelBeam = static_cast<CAcceleratorEntity&>(e);
            Vec3 vForce = accelBeam.GetForce() * accelBeam.GetStrength();
            m_collisionState.bOnFloor = false;
            m_collisionState.bHitCeiling = false;
            m_collisionState.bGrabbedWallFromLeft = false;
            m_collisionState.bGrabbedWallFromRight = false;

            m_acceleratorTime = accelBeam.GetDuration();
            m_currentAcceleratorTime = m_acceleratorTime;
            m_acceleratorForce = Vec2(vForce);

            m_movementState.m_bJumping = false;
            m_movementState.m_bJumpingUp = false;
            m_movementState.m_bSecondJump = false;
            m_movementState.m_bSecondJumpBlocked = false;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_ACCELERATOR_BOSS)
        {
            CAcceleratorBossEntity& accelBeam = static_cast<CAcceleratorBossEntity&>(e);
            accelBeam.SetDisabled(true);

            Vec3 vForce = accelBeam.GetForce() * accelBeam.GetStrength();
            m_collisionState.bOnFloor = false;
            m_collisionState.bHitCeiling = false;
            m_collisionState.bGrabbedWallFromLeft = false;
            m_collisionState.bGrabbedWallFromRight = false;

            m_movementState.m_bJumping = false;
            m_movementState.m_bJumpingUp = false;
            m_movementState.m_bSecondJump = false;
            m_movementState.m_bSecondJumpBlocked = false;

            m_acceleratorTime = accelBeam.GetDuration();
            m_currentAcceleratorTime = m_acceleratorTime;
            m_acceleratorForce = Vec2(vForce);

            SetGhost(true);
            m_movementState.m_bFrozen = false;
            m_movementState.m_bMovementDisabled = true;

            m_pAcceleratorBoss = &e;
            m_bAcceleratorBoss = true;
            m_bossAcceleratorTime = m_bossAcceleratorDuration;
        }
        else if (e.GetMetaTypeId() == CMagnetEntity::ENTITY_META_TYPEID)
        {
            m_movementState.m_bOnMagnet = true;
            m_pMagnet = static_cast<CMagnetEntity*>(&e);

            m_magnetEnterPosition = m_pMagnet->GetNode()->GetDerivedPosition();
            m_currentMagnetEnterTime = 0.0f;

            m_collisionState.bOnFloor = true;

            m_movementState.m_bJumping = false;
            m_movementState.m_bJumpingUp = false;
            m_movementState.m_bSecondJump = false;
            m_movementState.m_bSecondJumpBlocked = false;

        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_FORCEFIELD)
        {
            m_movementState.m_bJumping = false;
        }
    }

    //-----------------------------------------------------------------------------------
    bool CCharacter3DEntity::CheckPlayerOutsideLevel()
    {
        const LevelBorderOffsets& offsets = gEngine->GetWorldMgr()->GetScenario()->GetLevelBorderOffsets();
        float levelWidth = static_cast<float>(m_pLevelMgr->GetLevelGrid().GetNumCellsX() * CELL_SIZE - (int)offsets.right * CELL_SIZE + GetWidth());
        float levelHeight = static_cast<float>(m_pLevelMgr->GetLevelGrid().GetNumCellsY() * CELL_SIZE - (int)offsets.top * CELL_SIZE + GetHeight());
        float levelLeft = static_cast<float>((int)offsets.left * CELL_SIZE - GetWidth());
        float levelBottom = static_cast<float>((int)offsets.bottom * CELL_SIZE - GetHeight());

        Vec3 vCharPos = GetNode()->GetDerivedPosition();

        if (gEngine->GetWorldMgr()->GetGameMode() &&
            (vCharPos.x < levelLeft ||
             vCharPos.y < levelBottom ||
             vCharPos.x > levelWidth ||
             vCharPos.y > levelHeight))
        {

            SetKilled(true);

            return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CCharacter3DEntity::Update);

        //#ifndef ENV_FINAL
        if (gEngine->GetInput()->IsKeyJustDown('G'))
        {
            m_charState.m_bGodmode = !m_charState.m_bGodmode;

            SetGhost(false);
        }
        //#endif // ENV_FINAL

        if (gEngine->GetInput()->IsActionJustReleased(ACTION_PRIMARY))
        {
            m_movementState.m_bJumpLocked = false;
        }

        if (m_bPicked)
        {
            CollisionState colRes;
            m_startPos = m_pNode->GetDerivedPosition();
            colRes.pos = Convert2Fixed(m_startPos.x, m_startPos.y);
            SetCollisionState(colRes);
            return;
        }
        else if (!GetGameMode())
        {
            CollisionState colRes;
            colRes.pos = Convert2Fixed(m_startPos.x, m_startPos.y);
            SetCollisionState(colRes);
            return;
        }

        if (gEngine->GetWorldMgr()->GetLevelJustStarted())
        {
            m_pParticlesSuper->SetEmitNewParticles(false);
            m_pParticlesSuperBlack->SetEmitNewParticles(false);
            m_pParticlesDistortSuperField->SetEmitNewParticles(false);
            m_pParticlesDistortTrace->SetEmitNewParticles(false);
            m_pParticlesDistortHitField->SetEmitNewParticles(false);
            m_pParticlesDistortDeath->SetEmitNewParticles(false);

            m_pParticlesDistortSpawn->Reset();
            m_pParticlesDistortSpawn->SetEmitNewParticles(true);
            m_pParticlesTerrainTrace->SetEmitNewParticles(true);
        }

        UpdateSounds(dt);
        UpdateParticleSystems(dt);

        if (m_charState.m_bKilled)
        {
            return;
        }

        if (m_charState.m_currentSpawnTime >= 0.0f)
        {
            return;
        }

        CEntity::Update(dt);

        if (m_movementState.m_bFrozen)
        {
            const Vec3& charPos = m_pNode->GetDerivedPosition();
            Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), charPos));

            ApplyTransform(mtxTranslation);

            return;
        }

        CheckPlayerOutsideLevel();


        if (m_bossAcceleratorTime >= 0.0f)
        {
            CBossEntity* pEntity = static_cast<CBossEntity*>(gEngine->GetEntityMgr()->GetFirstEntityOfType(ENTITY_TYPEID_BOSS));
            if (pEntity && m_pAcceleratorBoss)
            {
                pEntity->SetState(CBossEntity::BOSS_STATE_CHARACTER_JUMP);

                Vec3 bosPosition = pEntity->GetNode()->GetDerivedPosition() + Vec3(0.0f, 0.0f, -30.0f);
                Vec3 charPosition = m_pAcceleratorBoss->GetNode()->GetDerivedPosition();

                float alpha = Clamp(m_bossAcceleratorTime / m_bossAcceleratorDuration, 0.0f, 1.0f);

                Vec3 newPosition = Lerp(bosPosition, charPosition, alpha);

                SetPosition(newPosition);

                m_bossAcceleratorTime -= dt;

                if (m_bossAcceleratorTime < 0.0f)
                {
                    pEntity->SetDead(true);
                    //m_pAcceleratorBoss->OnFired(0);
                    m_movementState.m_bFrozen = true;
                }

                return;
            }

        }

        if (m_movementState.m_bInForceField && m_pForceField)
        {
            Vec2 force(glm::normalize(m_pForceField->GetNode()->GetDerivedUp()));
            force *= static_cast<CForceFieldEntity*>(m_pForceField)->GetAcceleration() * dt;

            //m_vVelocity += force;

            Vec2 targetVelo = Vec2(m_pForceField->GetNode()->GetDerivedUp()) * m_pForceField->GetTargetVelocity();
            float proj = ComputePointOnLineProjection(Vec3(m_velocity.x, m_velocity.y, 0), Vec3(0, 0, 0), Vec3(targetVelo.x, targetVelo.y, 0));
            if (proj < 1.0f)
                m_velocity += force;

            m_acceleratorTime = m_currentAcceleratorTime = 0.0f;
            if (abs(m_currentAcceleratorForce.x) > 0.0f)
                m_velocity.x += m_currentAcceleratorForce.x;
            if (abs(m_currentAcceleratorForce.y) > 0.0f)
                m_velocity.y += m_currentAcceleratorForce.y;
            m_currentAcceleratorForce = Vec2(0.0f, 0.0f);
        }

        m_movementState.m_bFirstJumpFrame = false;

        float stickWallTime = 0.25f;
        float floorFrictionParameter = 0.2f;
        float rampFrictionParameter = 0.7f;
        float jumpDelayTimer = 0.08f;

        if ((!m_prevCollisionState.bOnFloor && m_collisionState.bOnFloor) ||
            (!m_prevCollisionState.bHitCeiling && m_collisionState.bHitCeiling) /*|| m_collision.bHitRamp*/)
        {
            m_movementState.m_bJumpingUp = false;
            m_velocity.y = 0.0f;
        }
        else if (gEngine->GetInput()->IsActionJustReleased(ACTION_PRIMARY) &&
                 m_movementState.m_bJumpingUp &&
                 (m_movementState.m_jumpTimer.GetTime() >= 0.00f ||
                  m_movementState.m_bJumpedFromWallFromLeft ||
                  m_movementState.m_bJumpedFromWallFromRight))
        {
            m_movementState.m_bJumpingUp = false;
        }
        else if (m_movementState.m_jumpTimer.GetTime() > m_movementParams.m_jumpTime && m_movementState.m_bJumpingUp)
        {
            m_movementState.m_bJumpingUp = false;
        }

        if (m_collisionState.bOnFloor ||
            m_collisionState.bOnRamp ||
            ((m_collisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight) && !m_movementState.m_bMovementWallGrabDisabled) ||
            (m_collisionState.bHitCeiling && m_collisionState.bStickyCeiling))
        {
            m_movementState.m_bJumpedFromWallFromLeft = false;
            m_movementState.m_bJumpedFromWallFromRight = false;
            m_movementState.m_bSecondJumpBlocked = false;
            m_movementState.m_bJumping = false;
        }

        if (m_collisionState.bHitCeiling)
        {
            m_acceleratorTime = 0.0f;
            m_currentAcceleratorTime = m_acceleratorTime;
            m_acceleratorForce = Vec2(0.0f, 0.0f);
        }

        if (m_collisionState.bHitCeiling && m_collisionState.bStickyCeiling && m_collisionState.bHitRamp)
        {
            m_collisionState.bHitCeiling = false;
            m_collisionState.bStickyCeiling = false;
            m_velocity.y = -abs(m_velocity.x) * 0.35f;
        }


        if (m_movementState.m_bMovementDoubleJumpDisabled || m_movementState.m_bMovementDisabled)
            m_movementState.m_bSecondJumpBlocked = true;

        static bool bInFF = false;

        if (!m_movementState.m_bInForceField && bInFF)
        {
            m_movementState.m_bSecondJumpBlocked = false;
            m_movementState.m_bSecondJump = false;
        }

        bInFF = m_movementState.m_bInForceField;

        m_currentMagnetEnterTime += dt;


        if (m_movementState.m_bOnMagnet)
        {
            if (gEngine->GetInput()->IsActionJustTriggered(ACTION_PRIMARY) && !m_movementState.m_bMovementDisabled)
            {
                Vec3 vForce = m_pMagnet->GetNode()->GetDerivedUp() * m_pMagnet->GetSpeed();
                m_collisionState.bOnFloor = false;
                m_collisionState.bHitCeiling = false;
                m_collisionState.bGrabbedWallFromLeft = false;
                m_collisionState.bGrabbedWallFromRight = false;
                m_velocity = Vec2(0.0f, 0.0f);

                m_acceleratorTime = m_pMagnet->GetTime();
                m_currentAcceleratorTime = m_acceleratorTime;
                m_acceleratorForce = Vec2(vForce);

                m_movementState.m_bJumpLocked = true;
                m_movementState.m_bJumpingUp = false;
                m_movementState.m_bOnMagnet = false;
                m_pMagnet = NULL;

                m_characterSounds[CHARACTER_SOUND_MAGNET_LAUNCH]->PlaySound();
            }

        }
        else
        {
            if ((gEngine->GetInput()->IsActionTriggered(ACTION_PRIMARY)) &&
                !m_movementState.m_bMovementDisabled &&
                !m_movementState.m_bInForceField &&
                !m_movementState.m_bJumpLocked &&
                (m_collisionState.bOnFloor ||
                 m_collisionState.bOnRamp ||
                 (m_collisionState.bGrabbedWallFromLeft && !m_movementState.m_bMovementWallJumpDisabled) ||
                 (m_collisionState.bGrabbedWallFromRight && !m_movementState.m_bMovementWallJumpDisabled) ||
                 m_collisionState.bHitCeiling ||
                 m_movementState.m_jumpDelayTimer.GetTime() < jumpDelayTimer ||
                 (!m_movementState.m_bSecondJumpBlocked && !m_movementState.m_bMovementDoubleJumpDisabled && !m_collisionState.bGrabbedWallFromLeft && !m_collisionState.bGrabbedWallFromRight)))
            {

                if (m_collisionState.bOnFloor || m_collisionState.bOnRamp || m_collisionState.bGrabbedWallFromLeft ||
                    m_collisionState.bGrabbedWallFromRight || m_collisionState.bHitCeiling || m_movementState.m_jumpDelayTimer.GetTime() < jumpDelayTimer)
                {
                    m_movementState.m_bSecondJump = false;
                    m_movementState.m_jumpDelayTimer.SetTime(5.0f);
                }
                else
                {
                    m_movementState.m_bSecondJump = true;
                    m_movementState.m_bSecondJumpBlocked = true;

                    if ((m_movementState.m_bJumpedFromWallFromLeft && m_movementInputState.m_bKeyRightPressed) ||
                        (m_movementState.m_bJumpedFromWallFromRight && m_movementInputState.m_bKeyLeftPressed))
                    {
                        m_velocity *= 0.0f;
                    }
                }

                m_movementState.m_bJumpedFromWallFromLeft = false;
                m_movementState.m_bJumpedFromWallFromRight = false;


                m_movementState.m_bJumpingUp = true;
                m_movementState.m_bJumping = true;
                m_movementState.m_bFirstJumpFrame = true;
                m_movementState.m_jumpTimer.Reset();
                m_movementState.m_jumpTimer.Stop();

                if (m_collisionState.bHitCeiling)
                {
                    m_collisionState.bHitCeiling = false;
                    m_movementState.m_bJumpingUp = false;
                    m_movementState.m_bFirstJumpFrame = false;
                }
            }
            if (gEngine->GetInput()->IsActionTriggered(ACTION_PRIMARY) && !m_movementState.m_bMovementDisabled)
                m_movementState.m_bJumpLocked = true;
        }

        m_movementState.m_jumpDelayTimer.Update(dt);
        m_movementState.m_jumpTimer.Update(dt);
        m_movementState.m_wallStickTimer.Update(dt);

        if (gEngine->GetInput()->IsActionTriggered(ACTION_RIGHT) && !m_movementState.m_bMovementDisabled)
        {
            m_movementInputState.m_bKeyRightPressed = true;
            m_movementInputState.m_bKeyLeftPressed = false;
        }
        else if (gEngine->GetInput()->IsActionTriggered(ACTION_LEFT) && !m_movementState.m_bMovementDisabled)
        {
            m_movementInputState.m_bKeyLeftPressed = true;
            m_movementInputState.m_bKeyRightPressed = false;
        }

        if (!m_movementState.m_bMovementSprintDisabled && gEngine->GetInput()->IsActionTriggered(ACTION_SECONDARY) && !m_movementState.m_bMovementDisabled)
            m_movementInputState.m_bSecondaryKeyPressed = true;
        else
        {
            m_movementInputState.m_bSecondaryKeyPressed = false;
        }

        if (m_movementState.m_bMovementFlipInput)
        {
            std::swap(m_movementInputState.m_bKeyRightPressed, m_movementInputState.m_bKeyLeftPressed);
        }

        //         if (m_movementInputState.m_bSecondaryKeyPressed)
        //             gEngine->GetWorldMgr()->SetTimeMultiplier(0.4f);
        //         else
        //             gEngine->GetWorldMgr()->SetTimeMultiplier(1.0f);

        if (m_movementInputState.m_bSecondaryKeyPressed)
        {
            m_movementParams.m_playerMaxSpeedX = m_movementParams.m_initialPlayerMaxSpeedX * m_movementParams.m_playerSuperSpeed;
            m_movementParams.m_playerAcceleration = m_movementParams.m_initialPlayerAcceleration * m_movementParams.m_playerSuperSpeed;
        }
        else
        {
            m_movementParams.m_playerMaxSpeedX = m_movementParams.m_initialPlayerMaxSpeedX;
            m_movementParams.m_playerAcceleration = m_movementParams.m_initialPlayerAcceleration;
            m_movementParams.m_jumpGravity = m_movementParams.m_initialJumpGravity;
        }


        // Check if stick wall timer needs to be started / stopped
        //
        if (m_collisionState.bHitWallFromRight)
        {
            if (m_movementInputState.m_bKeyRightPressed && !m_collisionState.bOnFloor && !m_collisionState.bHitRamp)
                m_movementState.m_wallStickTimer.Start();
            else
                m_movementState.m_wallStickTimer.Stop();
        }
        else if (m_collisionState.bHitWallFromleft)
        {
            if (m_movementInputState.m_bKeyLeftPressed && !m_collisionState.bOnFloor && !m_collisionState.bHitRamp)
                m_movementState.m_wallStickTimer.Start();
            else
                m_movementState.m_wallStickTimer.Stop();
        }
        else
            m_movementState.m_wallStickTimer.Stop();

        if ((!m_prevCollisionState.bHitWallFromleft && m_collisionState.bHitWallFromleft) ||
            (!m_prevCollisionState.bHitWallFromRight && m_collisionState.bHitWallFromRight))
        {
            m_movementState.m_wallStickTimer.Reset();
        }


        // Update acceleration based on new collision state
        //
        if (m_collisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight || m_collisionState.bHitWallFromleft || m_collisionState.bHitWallFromRight)
            m_velocity.x = 0.0f;

        if (m_movementInputState.m_bKeyRightPressed)
        {
            if (m_collisionState.bHitWallFromRight &&
                m_movementState.m_wallStickTimer.GetTime() < stickWallTime &&
                !m_movementState.m_bInForceField &&
                !m_collisionState.bOnFloor &&
                !m_collisionState.bHitRamp)
                // Check if player should stick on wall
            {
                m_velocity.x = 0.0f;
            }
            else if (!m_collisionState.bGrabbedWallFromLeft)
                // Apply acceleration
            {
                if (m_collisionState.bOnFloor && m_velocity.x < -m_movementParams.m_playerMaxSpeedX)
                    m_velocity.x += m_movementParams.m_playerAcceleration * 4 * dt;
                else if (m_velocity.x < 0.0f)
                    m_velocity.x += m_movementParams.m_playerAcceleration * 2 * dt;
                else
                {
                    if (m_velocity.x > m_movementParams.m_playerMaxSpeedX && !m_movementState.m_bInForceField)
                        m_velocity.x = std::max(m_velocity.x - m_movementParams.m_playerAcceleration * 2 * dt, m_movementParams.m_playerMaxSpeedX);
                    else if (m_velocity.x < m_movementParams.m_playerMaxSpeedX)
                        m_velocity.x += m_movementParams.m_playerAcceleration * dt;
                }
            }

        }
        else if (m_movementInputState.m_bKeyLeftPressed)
        {
            if (m_collisionState.bHitWallFromleft &&
                m_movementState.m_wallStickTimer.GetTime() < stickWallTime &&
                !m_movementState.m_bInForceField &&
                !m_collisionState.bOnFloor &&
                !m_collisionState.bHitRamp)
                // Check if player should stick on wall
            {
                m_velocity.x = 0.0f;
            }
            else if (!m_collisionState.bGrabbedWallFromRight)
                // Apply acceleration
            {
                if (m_collisionState.bOnFloor && m_velocity.x > m_movementParams.m_playerMaxSpeedX)
                    m_velocity.x -= m_movementParams.m_playerAcceleration * 4 * dt;
                else if (m_velocity.x > 0.0f)
                    m_velocity.x -= m_movementParams.m_playerAcceleration * 2 * dt;
                else
                {
                    if (m_velocity.x < -m_movementParams.m_playerMaxSpeedX && !m_movementState.m_bInForceField)
                        m_velocity.x = std::min(m_velocity.x + m_movementParams.m_playerAcceleration * 2 * dt, -m_movementParams.m_playerMaxSpeedX);
                    else if (m_velocity.x > -m_movementParams.m_playerMaxSpeedX)
                        m_velocity.x -= m_movementParams.m_playerAcceleration * dt;
                }
            }
        }
        else
        {

            if ((m_collisionState.bOnFloor || m_collisionState.bOnRamp))
            {
                if (m_velocity.x > m_movementParams.m_playerMaxSpeedX)
                {
                    m_velocity.x -= m_movementParams.m_playerAcceleration * 4 * dt;
                }
                else if (m_velocity.x < -m_movementParams.m_playerMaxSpeedX)
                {
                    m_velocity.x += m_movementParams.m_playerAcceleration * 4 * dt;
                }
                else
                    m_velocity.x = 0.0f;
            }
            else
            {
                if (m_collisionState.bHitCeiling && m_collisionState.bStickyCeiling)
                {
                    float friction = m_movementParams.m_initialPlayerAcceleration;
                    if (m_velocity.x > 0.0f)
                        m_velocity.x -= std::min(friction * dt, m_velocity.x);
                    else if (m_velocity.x < 0.0f)
                        m_velocity.x += std::min(friction * dt, -m_velocity.x);
                }
                else
                {
                    float friction = abs(m_velocity.x) * floorFrictionParameter;
                    if (m_velocity.x > 0.0f)
                        m_velocity.x -= friction * dt;
                    else if (m_velocity.x < 0.0f)
                        m_velocity.x += friction * dt;
                }


            }

        }

        if (m_currentAcceleratorTime > 0.0f)
        {
            if (Equals(m_currentAcceleratorTime, m_acceleratorTime))
            {
                m_velocity.x = m_acceleratorForce.x;
                m_velocity.y = m_acceleratorForce.y;
            }

            m_currentAcceleratorTime -= dt;

            CQuaternion bodyOrientation = m_pBodyNode->GetOrientation();
            if (abs(bodyOrientation.GetRoll()) > 0.0001f)
            {
                float roll = bodyOrientation.GetRoll();
                bodyOrientation.SetRoll(roll - sgn(roll) * dt * 5.0);
                m_pBodyNode->SetOrientation(bodyOrientation);
            }

            bodyOrientation.SetRoll(0.0f);
            m_pBodyNode->SetOrientation(bodyOrientation);
        }
        else
        {
            m_currentAcceleratorForce.x = m_currentAcceleratorForce.y = 0.0f;
        }


        // Update Acceleration vector in y direction
        //
        if (m_collisionState.bOnFloor && !m_collisionState.bOnRamp)
        {
            if (m_velocity.y < 0.0f)
                m_velocity.y = 0.0f;
        }

        if (!m_collisionState.bOnFloor &&
            !m_collisionState.bOnRamp &&
            m_velocity.y > -m_movementParams.m_playerMaxSpeedY &&
            (!m_collisionState.bGrabbedWallFromLeft || m_movementState.m_bMovementWallGrabDisabled) &&
            (!m_collisionState.bGrabbedWallFromRight || m_movementState.m_bMovementWallGrabDisabled) &&
            m_currentAcceleratorTime <= 0.0f)
        {
            if (m_velocity.y > 0.0f && m_movementState.m_bJumping)
            {
                if (m_movementState.m_bJumpedFromWallFromRight || m_movementState.m_bJumpedFromWallFromLeft)
                    m_velocity.y -= m_movementParams.m_gravity * 3.0f * dt;
                else
                    m_velocity.y -= m_movementParams.m_gravity * 3.0f * dt;
            }
            else
                m_velocity.y -= m_movementParams.m_gravity * dt;
        }

        if (m_collisionState.bHitCeiling && m_collisionState.bStickyCeiling && gEngine->GetInput()->IsActionTriggered(ACTION_PRIMARY))
        {
            m_velocity.y = 0.0f;
        }

        if ((m_collisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight))
        {
            if (!m_movementState.m_bMovementWallGrabDisabled)
            {
                if ((!m_prevCollisionState.bGrabbedWallFromLeft && m_collisionState.bGrabbedWallFromLeft) || (!m_prevCollisionState.bGrabbedWallFromRight && m_collisionState.bGrabbedWallFromRight))
                {
                    if (m_velocity.y < 0.0f)
                        m_velocity.y *= 0.5f;
                }

                if (m_movementState.m_bInForceField)
                {
                    m_velocity.y = std::max(-m_movementParams.m_playerMaxSpeedY, m_velocity.y - m_movementParams.m_gravity * dt);
                }
                else
                {

                    if (m_velocity.y > 0.0f)
                        m_velocity.y = std::max(-m_movementParams.m_playerMaxSpeedY, m_velocity.y - m_movementParams.m_gravity * 3.0f * dt);
                    else
                    {
                        m_velocity.y = std::max(-m_movementParams.m_playerMaxSpeedY, m_velocity.y - m_movementParams.m_gravity * dt * 0.5f);

                    }
                }
            }
        }
        else if (!m_collisionState.bOnRamp)
        {
            // Just left ramp
            //
            if (m_prevCollisionState.bOnRamp)
                m_velocity.y *= 0.1f;
        }
        else
        {
            // Handle on-ramp situations
            //

            if (!m_prevCollisionState.bOnRamp)
                m_velocity.y *= 0.1f;

            m_velocity.y = -m_movementParams.m_playerMaxSpeedY;


            if (m_velocity.x <= 0.0)
            {
                m_velocity.x = std::max(m_velocity.x, -m_movementParams.m_playerMaxSpeedX * rampFrictionParameter);

                if (m_collisionState.onRampLayout == CELL_LAYOUT_BOTLEFT)
                    m_velocity.y = rampFrictionParameter * abs(m_velocity.x);
            }
            else if (m_velocity.x >= 0.0f)
            {
                m_velocity.x = std::min(m_velocity.x, m_movementParams.m_playerMaxSpeedX * rampFrictionParameter);

                if (m_collisionState.onRampLayout == CELL_LAYOUT_BOTRIGHT)
                    m_velocity.y = rampFrictionParameter * abs(m_velocity.x);
            }
        }

        if (m_velocity.y < 0.0f && !m_movementState.m_bInForceField && !m_movementState.m_bMovementWallGrabDisabled)
        {
            if (m_collisionState.bGrabbedWallFromLeft && gEngine->GetInput()->IsActionTriggered(ACTION_RIGHT) && m_velocity.y < -m_movementParams.m_gravity * 0.1f)
                m_velocity.y = std::min(-m_movementParams.m_gravity * 0.1f, m_velocity.y + m_movementParams.m_gravity * 3.0f * dt);

            if (m_collisionState.bGrabbedWallFromRight && gEngine->GetInput()->IsActionTriggered(ACTION_LEFT) && m_velocity.y < -m_movementParams.m_gravity * 0.1f)
                m_velocity.y = std::min(-m_movementParams.m_gravity * 0.1f, m_velocity.y + m_movementParams.m_gravity * 3.0f * dt);
        }

        //static bool bTimedGrabbedFromLeft = false;

        // Jumps
        //
        if (m_movementState.m_bJumpingUp)
        {
            float jumpTime = m_movementParams.m_jumpTime;
            float factor = 1.0f;

            if (m_movementInputState.m_bSecondaryKeyPressed)
                factor = m_movementParams.m_playerSuperSpeed;

            if (m_movementState.m_bFirstJumpFrame)
            {

                if (m_movementState.m_bSecondJump)
                    m_animState.m_timeShape = CHAR_SHAPE_TIME;

                m_velocity.y = std::max(m_velocity.y, 0.0f);
                // Initial jump frame
                //
                m_movementState.m_jumpTimer.Start();

                m_movementState.m_bJumpedFromFloorOrRamp = m_collisionState.bOnFloor || m_collisionState.bOnRamp;

                if (m_collisionState.bGrabbedWallFromRight)
                {
                    if (!m_movementState.m_bJumpedFromFloorOrRamp)
                    {
                        if (!m_movementInputState.m_bSecondaryKeyPressed)
                            m_velocity.x = m_movementParams.m_playerMaxSpeedX * 1.0f;
                        else
                            m_velocity.x = m_movementParams.m_initialPlayerMaxSpeedX * 1.30f;
                    }
                    m_movementState.m_bJumpedFromWallFromRight = true;
                }
                else
                    m_movementState.m_bJumpedFromWallFromRight = false;

                if (m_collisionState.bGrabbedWallFromLeft /*|| bTimedGrabbedFromLeft*/)
                {
                    if (!m_movementState.m_bJumpedFromFloorOrRamp)
                    {
                        if (!m_movementInputState.m_bSecondaryKeyPressed)
                            m_velocity.x = -m_movementParams.m_playerMaxSpeedX * 1.0f;
                        else
                            m_velocity.x = -m_movementParams.m_initialPlayerMaxSpeedX * 1.30f;
                    }
                    m_movementState.m_bJumpedFromWallFromLeft = true;
                }
                else
                    m_movementState.m_bJumpedFromWallFromLeft = false;
            }


            if (m_movementState.m_bJumpedFromWallFromRight || m_movementState.m_bJumpedFromWallFromLeft)
            {
                //jumpTime *= 1.45f;
                if (m_movementInputState.m_bSecondaryKeyPressed)
                    jumpTime *= 0.85f;
            }


            static bool bPrevFrameJumping = false;

            if (m_movementState.m_jumpTimer.GetTime() <= jumpTime)
            {
                bPrevFrameJumping = true;

                //m_pNode->SetScale(VECTOR3(0.34f, 0.40f, 0.37f));

                //float alpha = std::pow(1.0f - m_jumpTimer.GetTime() / (jumpTime * 1.34f), 3.0f);
                float alpha = 1.0 - m_movementState.m_jumpTimer.GetTime() / jumpTime;

                if (m_movementState.m_bJumpedFromWallFromLeft || m_movementState.m_bJumpedFromWallFromRight)
                {
                    if (!m_movementState.m_bSecondJump)
                    {
                        if (!m_movementInputState.m_bSecondaryKeyPressed)
                        {
                            if (m_velocity.y < (m_movementParams.m_jumpGravity * 1.15f))
                                m_velocity.y = alpha * m_movementParams.m_jumpGravity * 1.15f;
                        }
                        else
                        {
                            if (m_velocity.y < (m_movementParams.m_jumpGravity * 1.3f))
                                m_velocity.y = alpha * m_movementParams.m_jumpGravity * 1.3f;
                        }
                    }
                }
                else
                {
                    if (!m_movementState.m_bSecondJump)
                    {
                        if (m_velocity.y < (m_movementParams.m_jumpGravity * 1.0f))
                            m_velocity.y = alpha * m_movementParams.m_jumpGravity * 1.0f;
                    }
                    else
                    {
                        if (m_velocity.y < (m_movementParams.m_jumpGravity * 1.20f))
                            m_velocity.y = alpha * m_movementParams.m_jumpGravity * 1.20f;
                    }
                }


                if (m_movementState.m_bJumpedFromWallFromRight && !m_movementState.m_bJumpedFromFloorOrRamp)
                {
                    if (m_movementInputState.m_bKeyLeftPressed)
                    {
                        m_velocity.x += m_movementParams.m_playerAcceleration * dt * 1.1f;
                    }
                }
                else if (m_movementState.m_bJumpedFromWallFromLeft && !m_movementState.m_bJumpedFromFloorOrRamp)
                {
                    if (m_movementInputState.m_bKeyRightPressed)
                    {
                        m_velocity.x -= m_movementParams.m_playerAcceleration * dt * 1.1f;
                    }

                }
            }
        }


        Vec2 vSpeedStripVelo = m_velocity;
        if (m_collisionState.bOnFloor)
            vSpeedStripVelo.y = 0.0f;


        // Slide along upper ramps
        //
        if (m_velocity.y > 0.0 && m_collisionState.bHitRamp)
        {
            if (Equals(m_velocity.x, 0.0f))
            {
                m_velocity.y = 0.0f;
            }
            if (m_velocity.x <= 0.0 && m_collisionState.hitRampLayout == CELL_LAYOUT_TOPLEFT)
            {
                if (abs(m_velocity.y) > m_movementParams.m_initialPlayerMaxSpeedX * 0.85f && abs(m_velocity.x) > m_movementParams.m_initialPlayerMaxSpeedX * 0.85f)
                {
                    m_velocity.x = m_velocity.y;
                }
                else
                {
                    if (m_velocity.y > 0.0)
                        m_velocity.y = 0.0f;
                }
            }
            else if (m_velocity.x >= 0.0 && m_collisionState.hitRampLayout == CELL_LAYOUT_TOPRIGHT)
            {
                if (abs(m_velocity.y) > m_movementParams.m_initialPlayerMaxSpeedX * 0.85f && abs(m_velocity.x) > m_movementParams.m_initialPlayerMaxSpeedX * 0.85f)
                {
                    m_velocity.x = -m_velocity.y;
                }
                else
                {
                    if (m_velocity.y > 0.0)
                        m_velocity.y = 0.0f;
                }
            }
        }

        std::stringstream str;
        /*str << "Velocity x: " << m_vVelocity.x << std::endl << "               y: " << m_vVelocity.y << std::endl;
        str << "Hit From Left: " << m_collisionState.bGrabbedWallFromLeft << std::endl;
        gEngine->DrawText(str.str());*/

        /*if(m_prevCollision.bGrabbedWallFromLeft != m_collision.bGrabbedWallFromLeft ||
        m_prevCollision.bGrabbedWallFromRight != m_collision.bGrabbedWallFromRight ||
        m_prevCollision.bHitCeiling != m_collision.bHitCeiling ||
        m_prevCollision.bOnFloor != m_collision.bOnFloor ||
        m_prevCollision.bOnRamp != m_collision.bOnRamp ||
        m_prevCollision.bHitRamp != m_collision.bHitRamp)
        {
        std::stringstream str2;
        str2 << "HitWallFromRight: " << m_collision.bGrabbedWallFromRight << std::endl;
        str2 << "HitWallFromLeft: " << m_collision.bGrabbedWallFromLeft << std::endl;
        str2 << "OnRamp: " << m_collision.bOnRamp << std::endl;
        str2 << "OnFloor: " << m_collision.bOnFloor << std::endl;
        str2 << "HitCeiling: " << m_collision.bHitCeiling << std::endl;
        str2 << "HitRamp: " << m_collision.bHitRamp << std::endl;
        ENV_LOG_INFO(str2.str());
        }*/



        // Save current collision state for next frame
        //
        m_prevCollisionState = m_collisionState;
        m_prevMovementState = m_movementState;

        VECTOR3FIXED platformCurMove(0, 0, 0);

        if (m_movementState.m_bOnPlatform)
        {
            if (m_velocity.y > 0.0f && m_platformCollision.bHitCeiling)
                m_velocity.y = 0.0f;
            else if (m_velocity.y < 0.0f && m_platformCollision.bOnFloor)
                m_velocity.y = 0.0f;
            if (m_velocity.x < 0.0f && m_platformCollision.bGrabbedWallFromRight)
                m_velocity.x = 0.0f;
            else if (m_velocity.x > 0.0f && m_platformCollision.bGrabbedWallFromLeft)
                m_velocity.x = 0.0f;
        }

        VECTOR2FIXED velocityMovePlatform = Convert2Fixed((m_velocity.x + m_currentAcceleratorForce.x) * dt,
            (m_velocity.y + m_currentAcceleratorForce.y) * dt);



        if (m_movementState.m_bOnPlatform && m_pPlatformNode)
        {
            if (!gEngine->GetSceneMgr()->IsValid(m_pPlatformNode))
            {
                m_pPlatformNode = NULL;
                m_movementState.m_bOnPlatform = false;
            }
            const Vec3& platformCurPos = m_pPlatformNode->GetDerivedPosition();
            VECTOR3FIXED platformCurPosFixed = Convert2Fixed(platformCurPos.x, platformCurPos.y, 0.0f);
            platformCurMove = platformCurPosFixed - m_fixedPlatformPrevPos;
            m_fixedPlatformPrevPos = platformCurPosFixed;

            velocityMovePlatform.x += platformCurMove.x;
            velocityMovePlatform.y += platformCurMove.y;

        }

        bool bOnFloorOrWallOrCeiling = m_collisionState.bOnFloor || m_collisionState.bGrabbedWallFromLeft || m_collisionState.bHitWallFromleft;

        if (m_movementState.m_bOnMagnet)
        {
            velocityMovePlatform = VECTOR2FIXED(0, 0);


            float alpha = std::min(m_currentMagnetEnterTime / 0.3f, 1.0f);

            Vec3 magnetPos = m_pMagnet->GetNode()->GetDerivedPosition();

            Vec3 charPos = (1.0f - alpha) * m_pNode->GetDerivedPosition() + alpha * magnetPos;

            CollisionState colState;
            colState.pos = Convert2Fixed(charPos.x, charPos.y);

            SetCollisionState(colState);

            m_pBodyNode->SetOrientation(CQuaternion());

        }
        else
        {
            velocityMovePlatform = velocityMovePlatform;

            CTerrainCollidable collidable;
            collidable.height = GetHeight() * 2.0f;
            collidable.width = GetWidth() * 2.0f;
            collidable.position = m_collisionState.pos;
            collidable.velocity = velocityMovePlatform;
            CollisionState res = m_pLevelMgr->ComputeMovement(collidable);

            SetCollisionState(res);

            CheckPlatforms();



            if (m_movementState.m_bOnPlatform && m_pPlatformNode)
            {
                if (!gEngine->GetSceneMgr()->IsValid(m_pPlatformNode))
                {
                    m_pPlatformNode = nullptr;
                    m_movementState.m_bOnPlatform = false;
                }
                const Vec3& platformCurPos = m_pPlatformNode->GetDerivedPosition();
                VECTOR3FIXED platformCurPosFixed = Convert2Fixed(platformCurPos.x, platformCurPos.y, 0.0f);
                platformCurMove = platformCurPosFixed - m_fixedPlatformPrevPos;
                m_fixedPlatformPrevPos = platformCurPosFixed;

                m_collisionState.pos.x += m_fixedPlatformRevertMove.x;
                m_collisionState.pos.y += m_fixedPlatformRevertMove.y;

                SetFixedPosition(m_collisionState.pos);
            }

            m_fixedPositionPrevFrame = m_collisionState.pos;
        }

        m_pForceField = nullptr;

        if (bOnFloorOrWallOrCeiling && !(m_collisionState.bOnFloor || m_collisionState.bGrabbedWallFromLeft || m_collisionState.bHitWallFromleft))
        {
            m_movementState.m_jumpDelayTimer.Reset();
            m_movementState.m_jumpDelayTimer.Start();

            if (m_movementState.m_bJumpingUp)
                m_movementState.m_jumpDelayTimer.SetTime(5.0f);
        }
    }

    //-----------------------------------------------------------------------------------
    template<typename Platform>
    bool CCharacter3DEntity::CheckPlatform(Platform* pPlatform)
    {
        if (pPlatform && !pPlatform->GetDisabled())
        {
            SCALARFIXED charMinX = 0;
            SCALARFIXED charMaxX = 0;
            SCALARFIXED charMinY = 0;
            SCALARFIXED charMaxY = 0;
            SCALARFIXED platformMinX = 0;
            SCALARFIXED platformMaxX = 0;
            SCALARFIXED platformMinY = 0;
            SCALARFIXED platformMaxY = 0;

            Vec3 platformpos = pPlatform->GetNode()->GetDerivedPosition();

            VECTOR3FIXED platformPos = Convert2Fixed(platformpos.x, platformpos.y, 0.0f);

            platformMinX = platformPos.x - Convert2Fixed(pPlatform->GetSize().x * pPlatform->GetNode()->GetDerivedScale().x);
            platformMaxX = platformPos.x + Convert2Fixed(pPlatform->GetSize().x * pPlatform->GetNode()->GetDerivedScale().x);
            platformMinY = platformPos.y - Convert2Fixed(pPlatform->GetSize().y * pPlatform->GetNode()->GetDerivedScale().y);
            platformMaxY = platformPos.y + Convert2Fixed(pPlatform->GetSize().y * pPlatform->GetNode()->GetDerivedScale().y);

            charMinX = std::min(m_fixedPositionPrevFrame.x, m_collisionState.pos.x) - Convert2Fixed(m_width);
            charMaxX = std::max(m_fixedPositionPrevFrame.x, m_collisionState.pos.x) + Convert2Fixed(m_width);
            charMinY = std::min(m_fixedPositionPrevFrame.y, m_collisionState.pos.y) - Convert2Fixed(m_height);
            charMaxY = std::max(m_fixedPositionPrevFrame.y, m_collisionState.pos.y) + Convert2Fixed(m_height);

            m_movementState.m_bOnPlatform = false;

            m_fixedPlatformRevertMove = VECTOR3FIXED(0, 0, 0);

            if (!m_platformCollision.bHitCeiling &&
                !m_platformCollision.bGrabbedWallFromLeft &&
                !m_platformCollision.bGrabbedWallFromRight &&
                charMaxY >= platformMaxY && charMinY <= platformMaxY &&
                charMinX <= platformMaxX && charMaxX >= platformMinX &&
                ((charMinX >= platformMinX && charMaxX <= platformMaxX) ||
                (charMinX <= platformMinX && charMaxX >= platformMinX && (platformMaxY - charMinY) < (charMaxX - platformMinX)) ||
                 (charMaxX >= platformMaxX && charMinX <= platformMaxX && (platformMaxY - charMinY) < (platformMaxX - charMinX))) && m_velocity.y <= 0.0f)
            {
                //floor
                charMinY = m_collisionState.pos.y - Convert2Fixed(m_height);

                m_platformCollision.bOnFloor = true;
                m_fixedPlatformPrevPos = platformPos;
                m_fixedPlatformRevertMove = VECTOR3FIXED(0, platformMaxY - charMinY, 0);
                m_movementState.m_bOnPlatform = true;
                m_pPlatformNode = pPlatform->GetNode();
            }
            else if (!m_platformCollision.bOnFloor && !m_platformCollision.bGrabbedWallFromLeft && !m_platformCollision.bGrabbedWallFromRight &&
                (charMaxY >= platformMinY && charMinY <= platformMinY) &&
                     ((charMinX >= platformMinX && charMaxX <= platformMaxX) ||
                     (charMinX <= platformMinX && charMaxX >= platformMinX && (charMaxY - platformMinY) < (charMaxX - platformMinX)) ||
                      (charMaxX >= platformMaxX && charMinX <= platformMaxX && (charMaxY - platformMinY) < (platformMaxX - charMinX))) && m_velocity.y >= 0.0f)
            {
                //ceiling
                charMaxY = m_collisionState.pos.y + Convert2Fixed(m_height);

                m_platformCollision.bHitCeiling = true;
                m_platformCollision.bStickyCeiling = true;
                m_fixedPlatformPrevPos = platformPos;
                m_fixedPlatformRevertMove = VECTOR3FIXED(0, platformMinY - charMaxY, 0);
                m_movementState.m_bOnPlatform = true;
                m_pPlatformNode = pPlatform->GetNode();
            }
            else if (!m_platformCollision.bHitCeiling && !m_platformCollision.bOnFloor && !m_platformCollision.bGrabbedWallFromRight &&
                (charMaxX >= platformMinX && charMinX <= platformMinX) && (charMinY <= platformMaxY && charMaxY >= platformMinY) && m_velocity.x >= 0.0f)
            {
                // Hitfromleft
                charMaxX = m_collisionState.pos.x + Convert2Fixed(m_width);

                m_platformCollision.bGrabbedWallFromLeft = true;
                m_fixedPlatformPrevPos = platformPos;
                m_fixedPlatformRevertMove = VECTOR3FIXED(platformMinX - charMaxX, 0, 0);
                m_movementState.m_bOnPlatform = true;
                m_pPlatformNode = pPlatform->GetNode();
            }
            else if (!m_platformCollision.bHitCeiling && !m_platformCollision.bGrabbedWallFromLeft && !m_platformCollision.bOnFloor &&
                (charMaxX >= platformMaxX && charMinX <= platformMaxX) && (charMinY <= platformMaxY && charMaxY >= platformMinY) && m_velocity.x <= 0.0f)
            {
                // hit from right
                charMinX = m_collisionState.pos.x - Convert2Fixed(m_width);

                m_platformCollision.bGrabbedWallFromRight = true;
                m_fixedPlatformPrevPos = platformPos;
                m_fixedPlatformRevertMove = VECTOR3FIXED(platformMaxX - charMinX, 0, 0);
                m_movementState.m_bOnPlatform = true;
                m_pPlatformNode = pPlatform->GetNode();
            }
            else
            {
                m_pPlatformNode = nullptr;
                m_platformCollision.bHitCeiling
                    = m_platformCollision.bGrabbedWallFromLeft
                    = m_platformCollision.bGrabbedWallFromRight
                    = m_platformCollision.bOnFloor = false;
            }

            //             if (m_platformCollision.bGrabbedWallFromLeft ||
            //                 m_platformCollision.bGrabbedWallFromRight)
            //                 m_platformCollision.bOnFloor = false;

            if (m_movementState.m_bOnPlatform)
            {
                pPlatform->OnPlayerOnPlatform();
                return true;
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::CheckPlatforms()
    {
        // Sort platforms by distance to the player
        struct Sorter
        {
            bool operator()(CEntity* p0, CEntity* p1)
            {
                float d0 = glm::length(p0->GetNode()->GetDerivedPosition() - m_Pos);
                float d1 = glm::length(p1->GetNode()->GetDerivedPosition() - m_Pos);

                return d0 < d1;
            }

            Vec3 m_Pos;
        };

        Sorter s;
        s.m_Pos = m_pNode->GetDerivedPosition();
        m_platforms.sort(s);

        m_movementState.m_bOnPlatform = false;
        for (std::list< CEntity* >::iterator it = m_platforms.begin(); it != m_platforms.end(); ++it)
        {
            if (dynamic_cast<CPlatformBaseEntity*>(*it))
            {
                CPlatformBaseEntity* pPlatform = static_cast<CPlatformBaseEntity*>(*it);

                if (CheckPlatform(pPlatform))
                {
                    break;
                }
            }
            else if (dynamic_cast<CPlatformEntity*>(*it))
            {
                CPlatformEntity* pPlatform = static_cast<CPlatformEntity*>(*it);

                if (CheckPlatform(pPlatform))
                {
                    break;
                }
            }
        }

        // Update collision flags, if player is on a platform
        //
        if (m_movementState.m_bOnPlatform)
        {
            m_collisionState.bGrabbedWallFromLeft = m_platformCollision.bGrabbedWallFromLeft;
            m_collisionState.bGrabbedWallFromRight = m_platformCollision.bGrabbedWallFromRight;
            m_collisionState.bOnFloor = m_platformCollision.bOnFloor;
            m_collisionState.bHitCeiling = m_platformCollision.bHitCeiling;
            m_collisionState.bStickyCeiling = m_platformCollision.bStickyCeiling;

        }

    }


    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::DumpCollisionInfo()
    {
        //std::stringstream str;
        //str << "Velocity x: " << m_vVelocity.x << std::endl << "               y: " << m_vVelocity.y << std::endl;
        //gEngine->DrawText(str.str());

        /*if(m_prevCollision.bGrabbedWallFromLeft != m_collision.bGrabbedWallFromLeft ||
        m_prevCollision.bGrabbedWallFromRight != m_collision.bGrabbedWallFromRight ||
        m_prevCollision.bHitCeiling != m_collision.bHitCeiling ||
        m_prevCollision.bOnFloor != m_collision.bOnFloor ||
        m_prevCollision.bOnRamp != m_collision.bOnRamp ||
        m_prevCollision.bHitRamp != m_collision.bHitRamp)
        {
        std::stringstream str2;
        str2 << "HitWallFromRight: " << m_collision.bGrabbedWallFromRight << std::endl;
        str2 << "HitWallFromLeft: " << m_collision.bGrabbedWallFromLeft << std::endl;
        str2 << "OnRamp: " << m_collision.bOnRamp << std::endl;
        str2 << "OnFloor: " << m_collision.bOnFloor << std::endl;
        str2 << "HitCeiling: " << m_collision.bHitCeiling << std::endl;
        str2 << "HitRamp: " << m_collision.bHitRamp << std::endl;
        ENV_LOG_INFO(str2.str());
        }*/
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::UpdateSounds(float dt)
    {
        //m_characterSounds[CHAR]
        if (m_collisionState.bOnFloor && abs(m_velocity.x) > 10 && !m_movementState.m_bFrozen)
        {
            if (m_movementInputState.m_bSecondaryKeyPressed)
                m_characterSounds[CHARACTER_SOUND_MOVE]->SetSpeed(3.0f);
            else
                m_characterSounds[CHARACTER_SOUND_MOVE]->SetSpeed(2.0f);

            if (!m_characterSounds[CHARACTER_SOUND_MOVE]->IsPlaying())
                m_characterSounds[CHARACTER_SOUND_MOVE]->PlaySound(-1);
        }
        else
            m_characterSounds[CHARACTER_SOUND_MOVE]->StopSound();


        if (!m_movementState.m_bMovementWallGrabDisabled)
        {
            m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->SetSpeed(1.0f);

            float speed = glm::length(m_velocity);

            if (speed > 50.0f && m_collisionState.bOnFloor == false && (m_collisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight || (m_collisionState.bStickyCeiling && m_collisionState.bHitCeiling)))
            {
                if (!m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->IsPlaying())
                    m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->PlaySound(-1);
            }
            else
                m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->StopSound();
        }
        else
        {
            m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->StopSound();
        }

//         if (m_prevCollisionState.bOnFloor == false && m_collisionState.bOnFloor)
//         {
//             if (!m_characterSounds[CHARACTER_SOUND_HITFLOOR]->IsPlaying())
//                 m_characterSounds[CHARACTER_SOUND_HITFLOOR]->PlaySound(0);
//         }

        if (m_movementState.m_bFirstJumpFrame)
        {
            if (m_movementState.m_bSecondJump)
                m_characterSounds[CHARACTER_SOUND_SECONDJUMP]->PlaySound();
            else
                m_characterSounds[CHARACTER_SOUND_JUMP]->PlaySound();
        }

        if (m_movementState.m_bOnMagnet)
        {
            if (!m_characterSounds[CHARACTER_SOUND_MAGNET_HOVER]->IsPlaying())
                m_characterSounds[CHARACTER_SOUND_MAGNET_HOVER]->PlaySound();
        }

        if ((m_movementState.m_bOnMagnet == false &&
             m_collisionState.bOnFloor == false &&
             m_collisionState.bGrabbedWallFromLeft == false &&
             m_collisionState.bGrabbedWallFromRight == false &&
             m_collisionState.bHitCeiling == false &&
             abs(m_velocity.x + m_velocity.y) > 10 &&
             m_movementState.m_bSecondJump && m_movementState.m_bJumping) ||
            m_movementState.m_bInForceField)
        {
            if (!m_characterSounds[CHARACTER_SOUND_FLY]->IsPlaying())
                m_characterSounds[CHARACTER_SOUND_FLY]->PlaySound();

            float volume = m_characterSounds[CHARACTER_SOUND_FLY]->GetVolume() + dt;
            volume = Clamp(volume, 0.0f, 1.0f);
            m_characterSounds[CHARACTER_SOUND_FLY]->SetVolume(volume);
        }
        else
        {
            float volume = m_characterSounds[CHARACTER_SOUND_FLY]->GetVolume() - dt * 5;
            volume = Clamp(volume, 0.0f, 1.0f);
            m_characterSounds[CHARACTER_SOUND_FLY]->SetVolume(volume);
        }

    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::UpdateParticleSystems(float dt)
    {
        Vec3 charPos = m_pNode->GetDerivedPosition();

        float angle = 0.0f;
        float speed = glm::length(m_velocity);

        if (speed)
        {
            Vec2 velocity2D(glm::normalize(m_velocity));
            Vec2 vXAxis(1.0f, 0.0f);

            angle = acos(glm::dot(velocity2D, vXAxis));

            if (velocity2D.y < 0.00f)
                angle = -angle;
        }

        const CParticleSystemEntity::mapEmitters& emitters = m_pParticlesTerrainTrace->GetEmitters();
        for (CParticleSystemEntity::mapEmitters::const_iterator it = emitters.begin(); it != emitters.end(); ++it)
        {
            if ((m_collisionState.bOnFloor || (m_collisionState.bHitCeiling && !m_movementState.m_bOnPlatform)) && speed < 150)
            {
                it->second->SetEmitRate(30.0f);
            }
            else
            {
                it->second->SetEmitRate(50.0f);
            }
        }

        if (m_movementState.m_bOnMagnet || m_movementState.m_bOnPlatform)
        {
            m_pParticlesTerrainTrace->SetEmitNewParticles(false);
        }
        else if ((m_collisionState.bOnFloor && !m_movementState.m_bOnPlatform) || (m_collisionState.bHitCeiling && !m_movementState.m_bOnPlatform))
        {
            m_pParticlesTerrainTrace->SetEmitNewParticles(true);
            //m_pParticlesTerrainTrace->GetNode()->SetPosition(Vec3(charPos.x, charPos.y - 20.0f, -5.0f));
            //m_pParticlesTerrainTrace->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));
            m_pParticlesTerrainTrace->GetNode()->SetPositionAndOrientation(Vec3(charPos.x, charPos.y - 20.0f, charPos.z - 5.0f), CQuaternion(0.0f, 0.0f, 0.0f));
        }
        else if (m_collisionState.bGrabbedWallFromLeft)
        {
            m_pParticlesTerrainTrace->SetEmitNewParticles(true);
            //m_pParticlesTerrainTrace->GetNode()->SetPosition(Vec3(charPos.x + 8.0f, charPos.y, -5.0f));
            //m_pParticlesTerrainTrace->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, PI * 0.5f));
            m_pParticlesTerrainTrace->GetNode()->SetPositionAndOrientation(Vec3(charPos.x + 8.0f, charPos.y, charPos.z - 5.0f), CQuaternion(0.0f, 0.0f, PI * 0.5f));
        }
        else if (m_collisionState.bGrabbedWallFromRight)
        {
            m_pParticlesTerrainTrace->SetEmitNewParticles(true);
            //m_pParticlesTerrainTrace->GetNode()->SetPosition(Vec3(charPos.x - 8.0f, charPos.y, -5.0f));
            //m_pParticlesTerrainTrace->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, -PI * 0.5f));
            m_pParticlesTerrainTrace->GetNode()->SetPositionAndOrientation(Vec3(charPos.x - 8.0f, charPos.y, charPos.z - 5.0f), CQuaternion(0.0f, 0.0f, -PI * 0.5f));
        }
        else
        {
            m_pParticlesTerrainTrace->GetNode()->SetPositionAndOrientation(Vec3(charPos.x, charPos.y, charPos.z - 10.0f), CQuaternion(0.0f, 0.0f, angle));

            if (speed > 350)
                m_pParticlesTerrainTrace->SetEmitNewParticles(true);
            else
                m_pParticlesTerrainTrace->SetEmitNewParticles(false);
        }


        if (m_collisionState.bHitCeiling && !m_movementState.m_bOnPlatform)
        {
            m_pParticlesTerrainTrace->SetEmitNewParticles(true);
            m_pParticlesTerrainTrace->GetNode()->SetPosition(Vec3(charPos.x + 12.0f, charPos.y + 20.0f, -5.0f));
        }

        static bool bEmitTrace = true;
        static bool bEmitField = true;

        if (gEngine->GetInput()->IsKeyJustDown(VK_F3))
            bEmitTrace = !bEmitTrace;

        if (gEngine->GetInput()->IsKeyJustDown(VK_F4))
            bEmitField = !bEmitField;

        m_pParticlesDistortHitField->GetNode()->SetPosition(Vec3(charPos.x, charPos.y, charPos.z));
        m_pParticlesDistortHitField->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, angle));
        m_pParticlesDistortSuperField->GetNode()->SetPosition(Vec3(charPos.x, charPos.y, charPos.z));
        m_pParticlesDistortSuperField->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0));
        m_pParticlesDistortTrace->GetNode()->SetPosition(Vec3(charPos.x, charPos.y, charPos.z));
        m_pParticlesDistortTrace->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0));
        m_pParticlesDistortSpawn->GetNode()->SetPosition(Vec3(charPos.x, charPos.y, charPos.z));
        m_pParticlesDistortSpawn->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0));
        m_pParticlesDistortDeath->GetNode()->SetPosition(Vec3(charPos.x, charPos.y, charPos.z));
        m_pParticlesDistortDeath->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0));

        bool bWasSuperTransform = m_prevMovementState.m_bSecondJump && m_prevMovementState.m_bJumping;
        bool bIsSuperTransform = m_movementState.m_bSecondJump && m_movementState.m_bJumping;

        if (true == m_prevMovementState.m_bJumping &&
            false == m_movementState.m_bJumping &&
            false == bWasSuperTransform &&
            false == m_collisionState.bHitWallFromleft &&
            false == m_collisionState.bHitWallFromRight)
        {
            m_pParticlesDistortHitField->Reset();
            m_pParticlesDistortHitField->SetEmitNewParticles(true);
        }

        if (gEngine->GetInput()->IsKeyJustDown(VK_F3))
        {
            m_pParticlesDistortHitField->Reset();
            m_pParticlesDistortHitField->SetEmitNewParticles(true);
        }

        static float curTimeSpawn = 0.0f;
        //static float curTimeDeath = 0.0f;

//         if (gEngine->GetInput()->IsKeyJustDown(VK_F6))
//         {
//             m_pParticlesDistortSpawn->Reset();
//             m_pParticlesDistortSpawn->SetEmitNewParticles(true);
// 
//             curTimeSpawn = 0.25f;
//         }
//         else
        {
            if (curTimeSpawn >= 0.0f)
            {
                float alpha = 1.0f - curTimeSpawn / 0.25f;

                m_pNode->SetScale(CHARACTER_SCALING * alpha);

                curTimeSpawn -= dt;
            }
        }

        //         if (gEngine->GetInput()->IsKeyJustDown(VK_F7))
        //         {
        //             m_pParticlesDeath->Reset();
        //             m_pParticlesDeath->SetEmitNewParticles(true);
        //             m_pParticlesTrace->SetEmitNewParticles(false);
        // 
        //             curTimeDeath = 0.25f;
        //         }
        //         else
        //         {
        //             if (curTimeDeath >= 0.0f)
        //             {
        //                 float alpha = curTimeDeath / 0.25f;
        // 
        //                 m_pNode->SetScale(VECTOR3(1.0f, 1.0f, 1.0f) * alpha);
        // 
        //                 curTimeDeath -= dt;
        //             }
        //         }

        if (m_movementInputState.m_bSecondaryKeyPressed && speed > 50.0f)
        {
            m_pParticlesDistortTrace->SetEmitNewParticles(true);
        }
        else
        {
            m_pParticlesDistortTrace->SetEmitNewParticles(false);
        }

        if (bIsSuperTransform)
        {
            m_pParticlesDistortSuperField->SetEmitNewParticles(bEmitField);
        }
        else
        {
            m_pParticlesDistortSuperField->SetEmitNewParticles(false);
        }

        if (!m_bTrace)
        {
            m_pParticlesTerrainTrace->SetEmitNewParticles(false);
            m_pParticlesDistortTrace->SetEmitNewParticles(false);
        }
// 
//         if (m_bBlackSkin && speed > 50.0f)
//         {
//             m_pParticlesDistortTrace->SetEmitNewParticles(true);
//         }

    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::UpdateColors(float dt)
    {
        float speed = glm::length(m_velocity);

        static float hearBeatTime = 2.0f;

        float factor = std::max(speed / 200.0f, 0.75f);
        hearBeatTime -= dt * factor;
        float alpha = 0.0f;
        if (hearBeatTime >= 1.0f)
        {
            alpha = hearBeatTime - 1.0f;
        }
        else if (hearBeatTime >= 0.0f)
        {
            alpha = 1.0f - hearBeatTime;
        }
        else
        {
            hearBeatTime = 2.0f;
            alpha = 1.0f;
        }

        m_meshInstContainer.CallForEach([&](const auto& pMeshInst) { pMeshInst->GetMaterial().SetDiffuseColor(m_diffuseMtrl); });

        m_diffuseAlpha = 1.4f;
        Vec4 clr = Vec4(m_diffuseMtrl * m_diffuseAlpha, 1.0f);
        m_spriteCharHead.SetColor(clr);
        m_spriteCharChest.SetColor(clr);
        m_spriteCharBody.SetColor(clr);
        m_spriteCharArm01.SetColor(clr);
        m_spriteCharArm02.SetColor(clr);
        m_spriteCharLeg01.SetColor(clr);
        m_spriteCharLeg02.SetColor(clr);
    }


    float CHARACTER_ARM_HEIGHT = 22.0f;
    float CHARACTER_ARM_DEPTH = 6.0f;
    float CHARACTER_ARM_PITCH = 0.65f;

    float CHARCTER_AIR_LEG_REAR_BEGIN = 0.3f;
    float CHARCTER_AIR_LEG_REAR_END = 0.4f;
    float CHARCTER_AIR_LEG_FRONT_BEGIN = 0.4f;
    float CHARCTER_AIR_LEG_FRONT_END = 0.8f;

    Vec3 CHARACTER_CHEST_POSITION = Vec3(0.0f, 8.0f, -2.0f);

    float CHARACTER_CHEST_BREATHE_FACTOR = 0.25f;

    float CHARACTER_HEAD_HEIGHT = 19.0f;

    float CHARACTER_LEG_HEIGHT = 9.0f;

    float CHARACTER_BODY_HEIGHT = -15.0f;


    //-----------------------------------------------------------------------------------
    void ResetCharacterTransform(CCharacter3DEntity::TransformState& characterTransform,
                                 CCharacter3DEntity::MovementInputState& m_movementInputState,
                                 CCharacter3DEntity::MovementParams& m_movementParams,
                                 CCharacter3DEntity::MovementState& movementState,
                                 CCharacter3DEntity::AnimationState& animState,
                                 const Vec2& velocity,
                                 const CollisionState& colRes,
                                 float dt,
                                 float acceleratorTime)
    {
        bool bSwitch = m_movementInputState.m_bKeyRightPressed || m_movementInputState.m_bKeyLeftPressed;

        if (colRes.bGrabbedWallFromLeft)
        {
            bSwitch = true;
            movementState.m_bWatchingRight = true;
        }
        else if (colRes.bGrabbedWallFromRight)
        {
            bSwitch = true;
            movementState.m_bWatchingRight = false;
        }
        else if (movementState.m_bJumpedFromWallFromLeft && movementState.m_bJumpingUp)
        {
            if (!m_movementInputState.m_bKeyRightPressed)
                movementState.m_bWatchingRight = false;
            else
                movementState.m_bWatchingRight = true;

            bSwitch = true;
        }
        else if (movementState.m_bJumpedFromWallFromRight && movementState.m_bJumpingUp)
        {
            if (!m_movementInputState.m_bKeyLeftPressed)
                movementState.m_bWatchingRight = true;
            else
                movementState.m_bWatchingRight = false;

            bSwitch = true;
        }
        else if (m_movementInputState.m_bKeyRightPressed)
            movementState.m_bWatchingRight = true;
        else if (m_movementInputState.m_bKeyLeftPressed)
            movementState.m_bWatchingRight = false;




        //         if (velocity.x < 0.0f && m_movementInputState.m_bKeyRightPressed)
        //         {
        //             // turn to yaw = PI / 2
        //             float alpha = abs(velocity.x) / m_movementParams.m_playerMaxSpeedX;
        // 
        //             float yaw = Lerp(PI * 0.5f, PI, alpha);
        // 
        //             characterTransform.m_rootOrientation.SetYawPitchRoll(yaw, 0.0f, 0.0f);
        //         }
        //         else if (velocity.x > 0.0f && m_movementInputState.m_bKeyRightPressed &&
        //                  abs(characterTransform.m_rootOrientation.GetYaw()) > 0.1f)
        //         {
        //             // turn to yaw = 0*
        // 
        //             float alpha = velocity.x / m_movementParams.m_playerMaxSpeedX;
        // 
        //             float yaw = Lerp(PI * 0.5f, 0.0f, alpha);
        // 
        //             characterTransform.m_rootOrientation.SetYawPitchRoll(yaw, 0.0f, 0.0f);
        //         }
        //         else
        {
            if (movementState.m_bWatchingRight && bSwitch)
            {
                characterTransform.m_rootOrientation = CQuaternion();
            }
            else if (bSwitch)
            {
                characterTransform.m_rootOrientation.SetYawPitchRoll(PI, 0.0f, 0.0f);
            }
        }

        //characterTransform.m_rootPosition.z = 0.0f;
        characterTransform.m_bodyPosition = Vec3(0.0f, CHARACTER_BODY_HEIGHT, 0.0f);

        if (!movementState.m_bOnMagnet && (acceleratorTime < 0.0f || Equals(acceleratorTime, 0.0f)))
        {
            characterTransform.m_bodyOrientation = CQuaternion();
        }

        if (movementState.m_bFirstJumpFrame && !movementState.m_bSecondJump)
        {
            characterTransform.m_leg01Orientation = CQuaternion();
            characterTransform.m_leg02Orientation = CQuaternion();
        }
    }

    //-----------------------------------------------------------------------------------
    void UpdateCharacterTransformOnFloor(CCharacter3DEntity::TransformState& characterTransform,
                                         CCharacter3DEntity::MovementInputState& m_movementInputState,
                                         CCharacter3DEntity::MovementParams& m_movementParams,
                                         CCharacter3DEntity::MovementState& movementState,
                                         CCharacter3DEntity::AnimationState& animState,
                                         const Vec2& velocity,
                                         const CollisionState& colRes,
                                         float dt)
    {
        float factor = 1.0f;
        if (m_movementInputState.m_bSecondaryKeyPressed)
            factor = m_movementParams.m_playerSuperSpeed;


        if (abs(velocity.x) > 10.0f)
        {
            if (m_movementInputState.m_bSecondaryKeyPressed)
                characterTransform.m_bodyOrientation.SetRoll(-0.175f);
            else
                characterTransform.m_bodyOrientation.SetRoll(-0.1f);
        }
        else
        {
            characterTransform.m_bodyOrientation.SetRoll(0.0f);
        }


        if (abs(velocity.x) > 10)
        {
            animState.m_chestBreathe += dt * 24;
            characterTransform.m_chestOrientation = CQuaternion(0.0f, 0.0f, -0.15f);
            characterTransform.m_chestPosition = CHARACTER_CHEST_POSITION + Vec3(0.0f, CHARACTER_CHEST_BREATHE_FACTOR * sin(animState.m_chestBreathe), 0.0f);
        }
        else
        {
            animState.m_chestBreathe += dt * 4;
            characterTransform.m_chestOrientation = CQuaternion(0.0f, 0.0f, 0.0f);
            characterTransform.m_chestPosition = CHARACTER_CHEST_POSITION + Vec3(0.0f, CHARACTER_CHEST_BREATHE_FACTOR * sin(animState.m_chestBreathe), 0.0f);
        }



        if (animState.m_armRotation > 0.85f)
        {
            animState.m_bArmRightRotation = !animState.m_bArmRightRotation;
            animState.m_armRotation = 0.85f;
        }
        else if (animState.m_armRotation < -0.85f)
        {
            animState.m_bArmRightRotation = !animState.m_bArmRightRotation;
            animState.m_armRotation = -0.85f;
        }

        if (abs(velocity.x) > 10.0f)
        {
            if (animState.m_bArmRightRotation)
                animState.m_armRotation += 5.0f * factor * dt;
            else
                animState.m_armRotation -= 5.0f * factor * dt;
        }
        else
            animState.m_armRotation = 0.0f;

        //         float armXRot = 0.0f;
        //         if (abs(velocity.x) > 10)
        //             armXRot = 0.85f;

        float rotationFactor = 1.0;
        if (m_movementInputState.m_bSecondaryKeyPressed)
            rotationFactor = 1.0f;

        if (animState.m_legRotation > (0.85f * rotationFactor))
        {
            animState.m_bLegRightRotation = !animState.m_bLegRightRotation;
            animState.m_legRotation = (0.85f * rotationFactor);
        }
        else if (animState.m_legRotation < (-0.85f * rotationFactor))
        {
            animState.m_bLegRightRotation = !animState.m_bLegRightRotation;
            animState.m_legRotation = (-0.85f * rotationFactor);
        }

        float runFactor = 1.05f;
        if (m_movementInputState.m_bSecondaryKeyPressed)
            runFactor = 1.32f;
        if (abs(velocity.x) > 10.0f)
        {
            if (animState.m_bLegRightRotation)
                animState.m_legRotation += 7.5 * runFactor * dt;
            else
                animState.m_legRotation -= 7.5 * runFactor * dt;
        }
        else
            animState.m_legRotation = 0.0f;

        float rotLeg01 = animState.m_legRotation;
        float rotLeg02 = -animState.m_legRotation;

        characterTransform.m_arm01Position = Vec3(0.0f, CHARACTER_ARM_HEIGHT, -CHARACTER_ARM_DEPTH);
        characterTransform.m_arm01Orientation.SetPitch(CHARACTER_ARM_PITCH);
        characterTransform.m_arm01Orientation.SetRoll(animState.m_armRotation);

        characterTransform.m_arm02Position = Vec3(0.0f, CHARACTER_ARM_HEIGHT, CHARACTER_ARM_DEPTH);
        characterTransform.m_arm02Orientation.SetPitch(-CHARACTER_ARM_PITCH);
        characterTransform.m_arm02Orientation.SetRoll(-animState.m_armRotation);

        if (abs(velocity.x) > 10)
        {
            characterTransform.m_headPosition = Vec3(0.0f, CHARACTER_HEAD_HEIGHT, 0.0f);
            characterTransform.m_headOrientation.SetRoll(0.0f);
        }
        else
        {
            characterTransform.m_headPosition = Vec3(0.0f, CHARACTER_HEAD_HEIGHT, 0.0f);
            characterTransform.m_headOrientation.SetRoll(0.0f);
        }

        if ((velocity.x > 0.0 && m_movementInputState.m_bKeyLeftPressed) ||
            (velocity.x < 0.0f && m_movementInputState.m_bKeyRightPressed))
        {
            rotLeg01 = 0.4f;
            rotLeg02 = -0.65f;
            characterTransform.m_bodyOrientation = CQuaternion(0.0f, 0, -0.3f);
            characterTransform.m_headOrientation = CQuaternion(0.0f, 0.0f, 0.1f);
            characterTransform.m_arm01Orientation.SetRoll(-1.0f);
            characterTransform.m_arm02Orientation.SetRoll(0.85f);
        }


        characterTransform.m_leg01Position = Vec3(0.0f, CHARACTER_LEG_HEIGHT, 0.0f);
        characterTransform.m_leg01Orientation.SetRoll(rotLeg01);

        characterTransform.m_leg02Position = Vec3(0.0f, CHARACTER_LEG_HEIGHT, 0.0f);
        characterTransform.m_leg02Orientation.SetRoll(rotLeg02);
    }

    //-----------------------------------------------------------------------------------
    void UpdateCharacterTransformOnWall(CCharacter3DEntity::TransformState& characterTransform,
                                        CCharacter3DEntity::MovementInputState& m_movementInputState,
                                        CCharacter3DEntity::MovementParams& m_movementParams,
                                        CCharacter3DEntity::MovementState& movementState,
                                        CCharacter3DEntity::AnimationState& animState,
                                        const Vec2& velocity,
                                        const CollisionState& colRes,
                                        float dt)
    {
        characterTransform.m_bodyOrientation.SetRoll(0.2f);

        if (abs(velocity.x) > 10)
            animState.m_chestBreathe += dt * 24;
        else
            animState.m_chestBreathe += dt * 4;

        characterTransform.m_chestPosition = CHARACTER_CHEST_POSITION + Vec3(0.0f, sin(animState.m_chestBreathe), 0.0f);
        characterTransform.m_chestOrientation = CQuaternion();

        characterTransform.m_arm01Position = Vec3(-2.0f, CHARACTER_ARM_HEIGHT, -(CHARACTER_ARM_DEPTH + 2.0f));
        characterTransform.m_arm01Orientation.SetYawPitchRoll(0.0f, 0.0f, 1.5f);

        characterTransform.m_arm02Position = Vec3(-2.0f, CHARACTER_ARM_HEIGHT, CHARACTER_ARM_DEPTH);
        characterTransform.m_arm02Orientation.SetYawPitchRoll(0.0f, 0.0f, 1.5f);

        characterTransform.m_headPosition = Vec3(0.0f, CHARACTER_HEAD_HEIGHT, 0.0f);
        characterTransform.m_headOrientation.SetRoll(0.1f);

        characterTransform.m_leg01Position = Vec3(0.0f, CHARACTER_LEG_HEIGHT, 0.0f);
        characterTransform.m_leg01Orientation.SetRoll(0.5f);

        characterTransform.m_leg02Position = Vec3(0.0f, CHARACTER_LEG_HEIGHT, 0.0f);
        characterTransform.m_leg02Orientation.SetRoll(1.0f);
    }

    //-----------------------------------------------------------------------------------
    void UpdateCharacterTransformOnCeiling(CCharacter3DEntity::TransformState& characterTransform,
                                           CCharacter3DEntity::MovementInputState& m_movementInputState,
                                           CCharacter3DEntity::MovementParams& m_movementParams,
                                           CCharacter3DEntity::MovementState& movementState,
                                           CCharacter3DEntity::AnimationState& animState,
                                           const Vec2& velocity,
                                           const CollisionState& colRes,
                                           float dt)
    {
        animState.m_legRotation = 0.0f;

        float factor = 1.0f;
        if (m_movementInputState.m_bSecondaryKeyPressed)
            factor = m_movementParams.m_playerSuperSpeed;


        if (abs(velocity.x) > 10.0f)
        {
            if (m_movementInputState.m_bSecondaryKeyPressed)
                characterTransform.m_bodyOrientation.SetRoll(0.2f);
            else
                characterTransform.m_bodyOrientation.SetRoll(0.1f);
        }
        else
        {
            characterTransform.m_bodyOrientation.SetRoll(0.0f);
        }


        if (abs(velocity.x) > 10)
        {
            characterTransform.m_chestOrientation = CQuaternion(0.0f, 0.0f, -0.3f);
        }
        else
        {
            characterTransform.m_chestOrientation = CQuaternion(0.0f, 0.0f, 0.0f);
        }

        characterTransform.m_chestPosition = CHARACTER_CHEST_POSITION;


        if (abs(velocity.x) > 10.0f)
            animState.m_legRotation = 0.5f;
        else
            animState.m_legRotation = 0.0f;

        float rotLeg01 = animState.m_legRotation;
        float rotLeg02 = animState.m_legRotation * 0.5f;


        characterTransform.m_arm01Position = Vec3(0.0f, CHARACTER_ARM_HEIGHT, -(CHARACTER_ARM_DEPTH + 2.0f));
        characterTransform.m_arm01Orientation.SetYawPitchRoll(0.0f, 3.05f, 0.0f);

        characterTransform.m_arm02Position = Vec3(0.0f, CHARACTER_ARM_HEIGHT, CHARACTER_ARM_DEPTH);
        characterTransform.m_arm02Orientation.SetYawPitchRoll(0.0f, -3.05f, 0.0f);


        if (abs(velocity.x) > 10)
        {
            characterTransform.m_headPosition = Vec3(0.0f, CHARACTER_HEAD_HEIGHT, 0.0f);
            characterTransform.m_headOrientation.SetRoll(0.3f);
        }
        else
        {
            characterTransform.m_headPosition = Vec3(0.0f, CHARACTER_HEAD_HEIGHT, 0.0f);
            characterTransform.m_headOrientation.SetRoll(0.1f);
        }

        if ((velocity.x > 0.0 && m_movementInputState.m_bKeyLeftPressed) ||
            (velocity.x < 0.0f && m_movementInputState.m_bKeyRightPressed))
        {
            rotLeg01 = -0.4f;
            rotLeg02 = -0.15f;
            characterTransform.m_bodyOrientation = CQuaternion(0.0f, 0, 0.4f);
            characterTransform.m_headOrientation = CQuaternion(0.0f, 0.0f, 0.5f);
        }

        characterTransform.m_leg01Position = Vec3(0.0f, CHARACTER_LEG_HEIGHT, 0.0f);
        characterTransform.m_leg01Orientation.SetRoll(rotLeg01);

        characterTransform.m_leg02Position = Vec3(0.0f, CHARACTER_LEG_HEIGHT, 0.0f);
        characterTransform.m_leg02Orientation.SetRoll(rotLeg02);
    }



    //-----------------------------------------------------------------------------------
    void UpdateCharacterTransformInAir(CCharacter3DEntity::TransformState& characterTransform,
                                       CCharacter3DEntity::MovementInputState& m_movementInputState,
                                       CCharacter3DEntity::MovementParams& m_movementParams,
                                       CCharacter3DEntity::MovementState& movementState,
                                       CCharacter3DEntity::AnimationState& animState,
                                       const Vec2& velocity,
                                       const CollisionState& colRes,
                                       float dt)
    {
        float x = (abs(velocity.x) / (m_movementParams.m_playerMaxSpeedX));

        if (movementState.m_bJumping)
            x = std::min(1.0f, movementState.m_jumpTimer.GetTime() / m_movementParams.m_jumpTime);

        characterTransform.m_chestPosition = CHARACTER_CHEST_POSITION;
        characterTransform.m_chestOrientation = CQuaternion(0.0f, 0.0f, 0.0f);

        characterTransform.m_arm01Position = Vec3(0.0f, CHARACTER_ARM_HEIGHT, 0.0f);
        characterTransform.m_arm01Orientation.SetYawPitchRoll(0.0f, (1 - x) * 0.8f, x * 1.2f);

        characterTransform.m_arm02Position = Vec3(0.0f, CHARACTER_ARM_HEIGHT, 0.0f);
        characterTransform.m_arm02Orientation.SetYawPitchRoll(0.0f, (1 - x) * -0.8f, x * -1.2f);

        characterTransform.m_headPosition = Vec3(0.0f, CHARACTER_HEAD_HEIGHT, 0.0f);
        characterTransform.m_leg01Position = Vec3(0.0f, CHARACTER_LEG_HEIGHT, 0.0f);
        characterTransform.m_leg02Position = Vec3(0.0f, CHARACTER_LEG_HEIGHT, 0.0f);

        if (movementState.m_bOnMagnet)
        {
            characterTransform.m_leg01Orientation = CQuaternion();
            characterTransform.m_leg02Orientation = CQuaternion();
            characterTransform.m_headOrientation = CQuaternion();
        }
        else
        {
            float velocityInterp = abs(velocity.x);
            velocityInterp /= m_movementParams.m_playerMaxSpeedX;

            float currentFrontLegRoll = Lerp(CHARCTER_AIR_LEG_FRONT_BEGIN, CHARCTER_AIR_LEG_FRONT_END, velocityInterp);
            float currentRearLegRoll = Lerp(CHARCTER_AIR_LEG_REAR_BEGIN, CHARCTER_AIR_LEG_REAR_END, velocityInterp);
            float currentHeadRoll = -0.2f;

            if (movementState.m_jumpTimer.GetTime() < m_movementParams.m_jumpTime * 0.4f)
            {
                float jumpTimerInterp = movementState.m_jumpTimer.GetTime() / (m_movementParams.m_jumpTime * 0.4f);

                characterTransform.m_leg01Orientation.SetRoll(currentFrontLegRoll * jumpTimerInterp);
                characterTransform.m_leg02Orientation.SetRoll(-currentRearLegRoll * jumpTimerInterp);
                characterTransform.m_headOrientation.SetRoll(currentHeadRoll * jumpTimerInterp);
            }
            else
            {
                characterTransform.m_leg01Orientation.SetRoll(currentFrontLegRoll);
                characterTransform.m_leg02Orientation.SetRoll(-currentRearLegRoll);
                characterTransform.m_headOrientation.SetRoll(currentHeadRoll);
            }
        }

    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::UpdateCharacterShape(CCharacter3DEntity::TransformState& characterTransform,
                                                  CCharacter3DEntity::MovementInputState& m_movementInputState,
                                                  CCharacter3DEntity::MovementParams& m_movementParams,
                                                  CCharacter3DEntity::MovementState& movementState,
                                                  CCharacter3DEntity::AnimationState& animState,
                                                  const Vec2& velocity,
                                                  const CollisionState& colRes,
                                                  float dt,
                                                  float acceleratorTime)
    {

        CParticleSystemEntity* pParticlesSuper = m_movementInputState.m_bSecondaryKeyPressed ? m_pParticlesSuper : m_pParticlesSuperBlack;

        static bool bShaped = false;

        animState.m_timeShape -= dt;
        timeReshape -= dt;

        if (m_charState.m_bKilled)
        {
            if (m_charState.m_currentKillTime >= 0.0f)
            {
                float alpha = m_charState.m_currentKillTime / m_charState.m_killTime;

                m_pNode->SetScale(CHARACTER_SCALING * alpha);

                pParticlesSuper->SetRenderParticles(false);

                m_pParticlesTerrainTrace->SetRenderParticles(false);

                m_charState.m_currentKillTime -= dt;
            }

            return;
        }

        if (m_charState.m_currentSpawnTime >= 0.0f)
        {
            float alpha = 1.0f - m_charState.m_currentSpawnTime / m_charState.m_spawnTime;

            m_pNode->SetScale(CHARACTER_SCALING * alpha);

            pParticlesSuper->SetRenderParticles(true);
            m_pParticlesTerrainTrace->SetRenderParticles(true);

            m_charState.m_currentSpawnTime -= dt;

            return;
        }





        if ((false == movementState.m_bSecondJump || false == movementState.m_bJumping) &&
            (false == movementState.m_bInForceField) &&
            (false == movementState.m_bOnMagnet) &&
            (acceleratorTime <= 0.0f) &&
            !m_bForceShape)
        {
//             pParticlesSuper->SetEmitNewParticles(false);
//             pParticlesSuper->SetRenderParticles(false);

            m_pParticlesSuper->SetEmitNewParticles(false);
            m_pParticlesSuper->SetRenderParticles(false);
            m_pParticlesSuperBlack->SetEmitNewParticles(false);
            m_pParticlesSuperBlack->SetRenderParticles(false);

            if (bShaped)
            {
                if (timeReshape >= 0.0f)
                {
                    float alpha = 1.0f - timeReshape / CHAR_SHAPE_TIME;
                    m_pNode->SetScale((1.0f - alpha) * Vec3(0.2f, 0.2f, 0.2f) + alpha * CHARACTER_SCALING);

                    m_pParticlesSuper->GetNode()->SetScale((1.0f - alpha) * Vec3(1, 1, 1));
                }
                else
                {
                    m_pNode->SetScale(CHARACTER_SCALING);
                    bShaped = false;
                }
            }
            else
            {
                m_pNode->SetScale(CHARACTER_SCALING);
            }

            m_animState.m_charChestRoll = m_pChestNode->GetOrientation().GetRoll();
            m_animState.m_charHeadRoll = m_pHeadNode->GetOrientation().GetRoll();
            m_animState.m_charLeg01Roll = m_pLeg01Node->GetOrientation().GetRoll();
            m_animState.m_charLeg02Roll = m_pLeg02Node->GetOrientation().GetRoll();
        }
        else
        {
            if (m_animState.m_timeShape >= 0.0f)
            {
                float alpha = m_animState.m_timeShape / CHAR_SHAPE_TIME;
                //float alpha2 = std::pow(m_animState.m_timeShape / CHAR_SHAPE_TIME, 2.0f);
                m_pNode->SetScale(CHARACTER_SCALING * alpha + (1.0f - alpha) * Vec3(0.0f, 0.0f, 0.0f));

                pParticlesSuper->GetNode()->SetScale(alpha * Vec3(0.5f, 0.5f, 0.5f) + (1.0f - alpha) * Vec3(1, 1, 1));

                CQuaternion q = m_pBodyNode->GetOrientation();
                q.SetRoll((1.0f - alpha) * -1.3f);
                m_pBodyNode->SetOrientation(q);

                //alpha = 0.0f;
                m_pChestNode->SetOrientation(CQuaternion(0.0f, 0.0f, alpha * m_animState.m_charChestRoll + (1.0f - alpha) * -0.8f));
                m_pHeadNode->SetOrientation(CQuaternion(0.0f, 0.0f, alpha * m_animState.m_charHeadRoll + (1.0f - alpha) * -0.3f));
                m_pLeg01Node->SetOrientation(CQuaternion(0.0f, 0.0f, alpha * m_animState.m_charLeg01Roll + (1.0f - alpha) * 1.0f));
                m_pLeg02Node->SetOrientation(CQuaternion(0.0f, 0.0f, alpha * m_animState.m_charLeg02Roll + (1.0f - alpha) * 0.9f));
            }
            else
            {
                m_pNode->SetScale(Vec3(0.0f, 0.0f, 0.0f));

                pParticlesSuper->GetNode()->SetScale(Vec3(1, 1, 1));

                bShaped = true;
            }

            if (bShaped)
            {
                CParticleSystemEntity::mapEmitters& emitters = pParticlesSuper->GetEmitters();
                for (CParticleSystemEntity::mapEmitters::iterator it = emitters.begin(); it != emitters.end(); ++it)
                {
                    if (m_velocity.x >= 0.0f)
                    {
                        it->second->SetOrientationSpeedMin(-10.5f);
                        it->second->SetOrientationSpeedMax(-12.5f);
                    }
                    else
                    {
                        it->second->SetOrientationSpeedMin(10.5f);
                        it->second->SetOrientationSpeedMax(12.5f);
                    }
                }
            }

            timeReshape = CHAR_SHAPE_TIME;

            Vec3 p = m_pNode->GetDerivedPosition();



            Vec2 vXAxis(1.0f, 0.0f);
            Vec2 vMagnetDir(glm::normalize(m_velocity));
            float angle = acos(glm::dot(vMagnetDir, vXAxis));
            if (vMagnetDir.y < 0.00f)
                angle = -angle;

            angle = ComputeAngle2DXAxis(m_velocity);
            angle = ComputeRoll(m_velocity);

            m_pParticlesSuperBlack->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, angle));
            m_pParticlesSuperBlack->GetNode()->SetPosition(Vec3(p.x, p.y, p.z - 10.0f));
            
            m_pParticlesSuper->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, angle));
            m_pParticlesSuper->GetNode()->SetPosition(Vec3(p.x, p.y, p.z - 10.0f));

            if (m_movementInputState.m_bSecondaryKeyPressed)
            {
                m_pParticlesSuper->SetEmitNewParticles(true);
                m_pParticlesSuperBlack->SetEmitNewParticles(false);
            }
            else
            {
                m_pParticlesSuper->SetEmitNewParticles(false);
                m_pParticlesSuperBlack->SetEmitNewParticles(true);
            }

            pParticlesSuper->SetRenderParticles(true);
        }


//         if (bShaped)
//         {
//             if (m_movementInputState.m_bSecondaryKeyPressed)
//             {
//                 m_pParticlesSuperBlack->SetEmitNewParticles(false);
//             }
//             else
//             {
//                 m_pParticlesSuper->SetEmitNewParticles(false);
//             }
//         }
    }

    //-----------------------------------------------------------------------------------
    void UpdateCharacterColor(CCharacter3DEntity::MovementState& movementState, float dt)
    {
        if (movementState.m_jumpTimer.GetTime() <= 0.3f && movementState.m_bSecondJump)
        {
            float alpha = movementState.m_jumpTimer.GetTime() / 0.3f;

            gEngine->GetWorldMgr()->GetScenario()->SetColorBrightness(Vec3(0.0f, 0.0f, alpha * 0.1f));
        }
        else
        {
            float offset = gEngine->GetWorldMgr()->GetScenario()->GetColorBrightness().z;
            offset += (-offset) * dt * 7.0f;

            gEngine->GetWorldMgr()->GetScenario()->SetColorBrightness(Vec3(0.0f, 0.0f, offset));
        }
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CCharacter3DEntity::AddToRenderQueue);

        CEntity::AddToRenderQueue(dt, queue);

        UpdateColors(dt);

        float velo = glm::length(m_velocity);

        if (velo > 400.0f)
        {
            float alpha = (velo - 400.0f) / (400.0f * 1.55f);

            float targetRadius = 0.0f * (1.0f - alpha) + 1000.0f * alpha;
            float targetOpacity = 0.0f * (1.0f - alpha) + 1.0f * alpha;

            m_silhouetteRadius += (targetRadius - m_silhouetteRadius) * gEngine->GetTimeElapsed() * 3.0f;
            m_silhouetteOpacity += (targetOpacity - m_silhouetteOpacity) * gEngine->GetTimeElapsed() * 3.0f;
        }
        else
        {
            float targetRadius = 0.0f;
            float targetOpacity = 0.0f;

            m_silhouetteRadius += (targetRadius - m_silhouetteRadius) * gEngine->GetTimeElapsed() * 3.0f;
            m_silhouetteOpacity += (targetOpacity - m_silhouetteOpacity) * gEngine->GetTimeElapsed() * 3.0f;
        }

        queue.m_postProcessRenderObject.m_silhouetteCharacterOpacity = std::max(m_silhouetteOpacity, queue.m_postProcessRenderObject.m_silhouetteCharacterOpacity);
        queue.m_postProcessRenderObject.m_silhouetteCharacterRadius = std::max(m_silhouetteRadius, queue.m_postProcessRenderObject.m_silhouetteCharacterRadius);


        CollisionState collisionState = m_collisionState;

        TransformState characterTransform;
        characterTransform.m_rootOrientation = m_pNode->GetOrientation();
        characterTransform.m_rootPosition = m_pNode->GetPosition();
        characterTransform.m_bodyOrientation = m_pBodyNode->GetOrientation();
        characterTransform.m_bodyPosition = m_pBodyNode->GetPosition();
        characterTransform.m_chestOrientation = m_pChestNode->GetOrientation();
        characterTransform.m_chestPosition = m_pChestNode->GetPosition();
        characterTransform.m_headOrientation = m_pHeadNode->GetOrientation();
        characterTransform.m_headPosition = m_pHeadNode->GetPosition();
        characterTransform.m_leg01Orientation = m_pLeg01Node->GetOrientation();
        characterTransform.m_leg01Position = m_pLeg01Node->GetPosition();
        characterTransform.m_leg02Orientation = m_pLeg02Node->GetOrientation();
        characterTransform.m_leg02Position = m_pLeg02Node->GetPosition();
        characterTransform.m_arm01Orientation = m_pArm01Node->GetOrientation();
        characterTransform.m_arm01Position = m_pArm01Node->GetPosition();
        characterTransform.m_arm02Orientation = m_pArm02Node->GetOrientation();
        characterTransform.m_arm02Position = m_pArm02Node->GetPosition();

        ResetCharacterTransform(characterTransform,
                                m_movementInputState,
                                m_movementParams,
                                m_movementState,
                                m_animState,
                                m_velocity,
                                collisionState,
                                dt,
                                m_currentAcceleratorTime);

        if (!m_movementState.m_bFrozen)
        {
            if (collisionState.bOnFloor || collisionState.bOnRamp)
            {
                UpdateCharacterTransformOnFloor(characterTransform,
                                                m_movementInputState,
                                                m_movementParams,
                                                m_movementState,
                                                m_animState,
                                                m_velocity,
                                                collisionState,
                                                dt);
            }
            else if ((m_movementState.m_bMovementWallGrabDisabled == false) &&
                (collisionState.bGrabbedWallFromLeft || collisionState.bGrabbedWallFromRight))
            {
                UpdateCharacterTransformOnWall(characterTransform,
                                               m_movementInputState,
                                               m_movementParams,
                                               m_movementState,
                                               m_animState,
                                               m_velocity,
                                               collisionState,
                                               dt);
            }
            else if ((collisionState.bHitCeiling && collisionState.bStickyCeiling))
            {
                UpdateCharacterTransformOnCeiling(characterTransform,
                                                  m_movementInputState,
                                                  m_movementParams,
                                                  m_movementState,
                                                  m_animState,
                                                  m_velocity,
                                                  collisionState,
                                                  dt);
            }
            else
            {
                UpdateCharacterTransformInAir(characterTransform,
                                              m_movementInputState,
                                              m_movementParams,
                                              m_movementState,
                                              m_animState,
                                              m_velocity,
                                              collisionState,
                                              dt);
            }
        }

        //         static bool bLooping = false;
        //         static float loopingTime = -1.0f;
        // 
        //         if (gEngine->GetInput()->IsKeyJustDown('L'))
        //         {
        //             bLooping = !bLooping;
        //             loopingTime = 0.5f;
        //         }
        // 
        //         if (loopingTime >= 0.0f)
        //         {
        //             float rootRoll = 0.0f;
        //             float bodyRoll = 0.0f;
        //             float chestRoll = 0.0f;
        //             float headRoll = 0.0f;
        // 
        //             if (loopingTime >= 0.4f)
        //             {
        //                 float loopingAlpha = 1.0f - (loopingTime - 0.4f) / 0.1f;
        // 
        //                 bodyRoll = -1.35f * loopingAlpha;
        //                 chestRoll = -1.35f * loopingAlpha;
        //                 headRoll = -1.35f * loopingAlpha;
        //             }
        //             else if (loopingTime >= 0.1f)
        //             {
        //                 bodyRoll = -1.35f;
        //                 chestRoll = -1.35f;
        //                 headRoll = -1.35f;
        //             }
        //             else
        //             {
        //                 float loopingAlpha = (loopingTime) / 0.1f;
        // 
        //                 bodyRoll = -1.35f * loopingAlpha;
        //                 chestRoll = -1.35f * loopingAlpha;
        //                 headRoll = -1.35f * loopingAlpha;
        //             }
        // 
        //             float loopingRootAlpha = 1.0f - loopingTime / 0.5f;
        // 
        //             characterTransform.m_bodyOrientation.SetRoll(bodyRoll);
        //             characterTransform.m_chestOrientation.SetRoll(chestRoll);
        //             characterTransform.m_headOrientation.SetRoll(headRoll);
        // 
        //             characterTransform.m_rootOrientation.SetRoll(-2.0f * PI * loopingRootAlpha);
        // 
        //             loopingTime -= dt;
        //         }

        //m_pNode->SetOrientation(characterTransform.m_rootOrientation);
        //m_pNode->SetPosition(characterTransform.m_rootPosition);
        //m_pBodyNode->SetOrientation(characterTransform.m_bodyOrientation);
        //m_pBodyNode->SetPosition(characterTransform.m_bodyPosition);
        //m_pChestNode->SetOrientation(characterTransform.m_chestOrientation);
        //m_pChestNode->SetPosition(characterTransform.m_chestPosition);
        //m_pHeadNode->SetOrientation(characterTransform.m_headOrientation);
        //m_pHeadNode->SetPosition(characterTransform.m_headPosition);
        //m_pLeg01Node->SetOrientation(characterTransform.m_leg01Orientation);
        //m_pLeg01Node->SetPosition(characterTransform.m_leg01Position);
        //m_pLeg02Node->SetOrientation(characterTransform.m_leg02Orientation);
        //m_pLeg02Node->SetPosition(characterTransform.m_leg02Position);
        //m_pArm01Node->SetOrientation(characterTransform.m_arm01Orientation);
        //m_pArm01Node->SetPosition(characterTransform.m_arm01Position);
        //m_pArm02Node->SetOrientation(characterTransform.m_arm02Orientation);
        //m_pArm02Node->SetPosition(characterTransform.m_arm02Position);

        // Fixes depth flickering
        characterTransform.m_rootPosition.z = 1.0f;

        m_pNode->SetPositionAndOrientation(characterTransform.m_rootPosition, characterTransform.m_rootOrientation);
        m_pBodyNode->SetPositionAndOrientation(characterTransform.m_bodyPosition, characterTransform.m_bodyOrientation);
        m_pChestNode->SetPositionAndOrientation(characterTransform.m_chestPosition, characterTransform.m_chestOrientation);
        m_pHeadNode->SetPositionAndOrientation(characterTransform.m_headPosition, characterTransform.m_headOrientation);
        m_pLeg01Node->SetPositionAndOrientation(characterTransform.m_leg01Position, characterTransform.m_leg01Orientation);
        m_pLeg02Node->SetPositionAndOrientation(characterTransform.m_leg02Position, characterTransform.m_leg02Orientation);
        m_pArm01Node->SetPositionAndOrientation(characterTransform.m_arm01Position, characterTransform.m_arm01Orientation);
        m_pArm02Node->SetPositionAndOrientation(characterTransform.m_arm02Position, characterTransform.m_arm02Orientation);


        UpdateCharacterColor(m_movementState, dt);

        UpdateCharacterShape(characterTransform,
                             m_movementInputState,
                             m_movementParams,
                             m_movementState,
                             m_animState,
                             m_velocity,
                             collisionState,
                             dt,
                             m_currentAcceleratorTime);

        m_charRenderObject.m_charParams.x = 1.0f;
        m_charRenderObject.m_charParams.y = 1.0f;
        m_charRenderObject.m_charMtrl = m_charMtrl;
        m_charRenderObject.m_position = m_pNode->GetDerivedPosition();
        m_charRenderObject.m_bRenderCells = m_bCells;

        queue.m_pCharacter3DRenderObject = &m_charRenderObject;

        bool m_bWatchingRight = true;

        static float curTransition = -1.0f;
        static bool bSuper = false;

        
        if (m_movementInputState.m_bSecondaryKeyPressed && !bSuper)
        {
            curTransition = 0.15f;
        }
        else if (!m_movementInputState.m_bSecondaryKeyPressed && bSuper)
        {
            curTransition = 0.15f;
        }

        curTransition -= dt;

        bSuper = m_movementInputState.m_bSecondaryKeyPressed;

        if (m_bWatchingRight)
        {
            float alpha = 0.0f;

            if (m_gainStrengthTime < 0.0f)
            {
                if (m_movementInputState.m_bSecondaryKeyPressed)
                {
                    alpha = 1.0f - Clamp(curTransition / 0.15f, 0.0f, 1.0f);
                }
                else
                {
                    alpha = Clamp(curTransition / 0.15f, 0.0f, 1.0f);
                }
            } 
            else 
            {
                float halfDuration = m_gainStrengthDuration * 0.5f;

                if (m_gainStrengthTime >= halfDuration)
                {
                    alpha = 1.0f - Clamp((m_gainStrengthTime - halfDuration) / halfDuration, 0.0f, 1.0f);
                }
                else
                {
                    alpha = Clamp(m_gainStrengthTime / halfDuration, 0.0f, 1.0f);
                }

                m_gainStrengthTime -= dt;
            }

            

            if (m_gainStrengthTime1 >= 0.0f)
            {
                float halfDuration = m_gainStrengthDuration1 * 0.5f;
                float scaling = 1.0f;

                if (m_gainStrengthTime1 >= halfDuration)
                {
                    scaling = Clamp((m_gainStrengthTime1 - halfDuration) / halfDuration, 0.0f, 1.0f);
                }
                else
                {
                    scaling = 1.0f - Clamp(m_gainStrengthTime1 / halfDuration, 0.0f, 1.0f);
                }

                m_pNode->SetScale(CHARACTER_SCALING * scaling);

                m_gainStrengthTime1 -= dt;
            }
            

            m_spriteCharArm02.SetAlpha(alpha);
            m_spriteCharLeg02.SetAlpha(alpha);
            m_spriteCharLeg01.SetAlpha(alpha);
            m_spriteCharHead.SetAlpha(alpha);
            m_spriteCharBody.SetAlpha(alpha);
            m_spriteCharChest.SetAlpha(alpha);
            m_spriteCharArm01.SetAlpha(alpha);

            queue.m_spriteRenderObjects.push_back(m_spriteCharArm02Black.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharLeg02Black.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharLeg01Black.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharHeadBlack.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharBodyBlack.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharChestBlack.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharArm01Black.GetRenderObject());

            queue.m_spriteRenderObjects.push_back(m_spriteCharArm02.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharLeg02.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharLeg01.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharHead.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharBody.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharChest.GetRenderObject());
            queue.m_spriteRenderObjects.push_back(m_spriteCharArm01.GetRenderObject());
            
        }
//         else
//         {
//             if (m_movementInputState.m_bSecondaryKeyPressed)
//             {
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharArm01.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharLeg02.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharLeg01.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharHead.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharBody.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharChest.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharArm02.GetRenderObject());
//             }
//             else
//             {
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharArm01Black.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharLeg02Black.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharLeg01Black.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharHeadBlack.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharBodyBlack.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharChestBlack.GetRenderObject());
//                 queue.m_spriteRenderObjects.push_back(m_spriteCharArm02Black.GetRenderObject());
//             }
//         }

        m_movementState.m_bInForceField = false;
        m_pForceField = NULL;

        m_movementInputState.m_bKeyRightPressed = false;
        m_movementInputState.m_bKeyLeftPressed = false;
    }


    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case CHARACTER_EVENT_DISABLE_CELLS:
            m_bCells = false;
            break;
        case CHARACTER_EVENT_GAIN_STRENGTH_0:
        {
            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_gainStrengthDuration);

            m_gainStrengthTime = m_gainStrengthDuration;
        } break;

        case CHARACTER_EVENT_GAIN_STRENGTH_1:
        {
            if (eventParameter.IsParameterSet("Duration"))
                eventParameter.GetParameter("Duration", m_gainStrengthDuration1);

            m_gainStrengthTime1 = m_gainStrengthDuration1;
        } break;

        case CHARACTER_EVENT_DISABLE_TRACE:
            m_bTrace = false;
            break;

        case CHARACTER_EVENT_ENABLE_TRACE:
            m_bTrace = true;
            break;

        case CHARACTER_EVENT_ENABLE_BLACKSKIN:
            m_bBlackSkin = true;

            InitSprites();
            break;

        case CHARACTER_EVENT_DISABLE_BLACKSIN:
            m_bBlackSkin = false;

            InitSprites();
            break;

        case CHARACTER_EVENT_FLIP_INPUT:
            m_movementState.m_bMovementFlipInput = true;
            break;
        case CHARACTER_EVENT_ENABLE_MOVEMENT:
            m_movementState.m_bMovementDisabled = false;
            // 			m_movementState.m_bMovementSprintDisabled = false;
            // 			m_movementState.m_bMovementJumpDisabled = false;
            // 			m_movementState.m_bMovementDoubleJumpDisabled = false;
            // 			m_movementState.m_bMovementWallGrabDisabled = false;
            // 			m_movementState.m_bMovementWallJumpDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_MOVEMENT:
            m_movementState.m_bMovementDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_SPRINT:
            m_movementState.m_bMovementSprintDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_SPRINT:
            m_movementState.m_bMovementSprintDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_JUMP:
            m_movementState.m_bMovementJumpDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_JUMP:
            m_movementState.m_bMovementJumpDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_DOUBLEJUMP:
            m_movementState.m_bMovementDoubleJumpDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_DOUBLEJUMP:
            m_movementState.m_bMovementDoubleJumpDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_WALLGRAB:
            m_movementState.m_bMovementWallGrabDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_WALLGRAB:
            m_movementState.m_bMovementWallGrabDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_WALLJUMP:
            m_movementState.m_bMovementWallJumpDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_WALLJUMP:
            m_movementState.m_bMovementWallJumpDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_FREEZE:
            m_movementState.m_bFrozen = true;
            break;

        case CHARACTER_EVENT_DISABLE_FREEZE:
            m_movementState.m_bFrozen = false;
            break;

        case CHARACTER_EVENT_STARTPLAYBACK:
            break;
        default:
            CEntity::OnInput(eventId, eventParameter);
        }
    }

    //-----------------------------------------------------------------------------------
    void CCharacter3DEntity::SetKilled(bool bKilled)
    {
        if (!m_charState.m_bKilled && bKilled && !m_charState.m_bGodmode)
        {
            m_pParticlesDistortDeath->Reset();
            m_pParticlesDistortDeath->SetEmitNewParticles(true);
            m_pParticlesTerrainTrace->SetEmitNewParticles(false);
            m_pParticlesDistortSuperField->SetEmitNewParticles(false);

            if (gEngine->GetWorldMgr()->GetGameMode())
            {
                m_characterSounds[CHARACTER_SOUND_DIE]->PlaySound(0);
            }

            m_charState.m_currentKillTime = m_charState.m_killTime;

            CSingletonPlayerMgr playerMgr;

            playerMgr->IncrementNumKills();

            gEngine->TriggerDeathCallback();

            m_charState.m_bKilled = !m_charState.m_bGodmode && bKilled;
        }

        
    }



    // 	void CCharacterEntity::SetMovementBlocked(bool bBlocked)
    // 	{
    // 		m_bMovementBlocked = bBlocked; 			
    // 		m_movementState.m_bJumpingUp = false;
    // 		m_collision.bOnFloor = true;
    // 		m_vVelocity = VECTOR2(0.0f, 0.0f);
    // 	}

}