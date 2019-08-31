
#include <Engine/Entity/CharacterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Entity/PlatformEntity.h>
#include <Engine/Entity/BumperEntity.h>
#include <Engine/Entity/LightEntity.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/PlayerMgr.h>
#include <Engine/Core/PolygonMousePicker.h>
#include <Engine/Scene/TerrainMgr.h>


namespace env
{
    ENTITY_TYPEID CCharacter2DEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_CHARACTER;

    CCharacter2DEntity::CCharacter2DEntity()
        : m_pParticlesEnergyParticles(NULL),
        m_pTraceParticles(NULL),
        m_pJumpParticles(NULL),
        m_pTraceFloorParticles(NULL),
        m_pCharacterBodyNode(NULL),
        m_pCharacterChestNode(NULL),
        m_pCharacterHeadNode(NULL),
        m_pCharacterLeg01Node(NULL),
        m_pCharacterLeg02Node(NULL),
        m_pCharacterArm01Node(NULL),
        m_pCharacterArm02Node(NULL),
        m_pParticlesTrace(NULL),
        m_pParticlesDoubleJump(NULL),
        m_pParticlesDeath(NULL),
        m_pParticlesBirth(NULL),
        m_pParticlesSuper(NULL),
        m_pLight(NULL),
        m_pPlatformNode(NULL),
        m_pForceField(NULL),
        m_vVelocity(0.0f, 0.0f),
        m_bJumpedFromWallFromRight(false),
        m_bJumpedFromWallFromLeft(false),
        m_bJumpedFromFloorOrRamp(false),
        m_bJumpLocked(false),
        m_bJumpingUp(false),
        m_bSecondaryKeyPressed(false),
        m_bPrimaryKeyPressed(false),
        m_bFirstJumpFrame(false),
        m_bWatchingRight(true),
        m_bOnPlatform(false),
        m_bArmRightRotation(true),
        m_bLegRightRotation(true),
        m_bMovementFlipInput(false),
        m_bMovementDisabled(false),
        m_bMovementSprintDisabled(false),
        m_bMovementJumpDisabled(false),
        m_bMovementDoubleJumpDisabled(false),
        m_bMovementWallGrabDisabled(false),
        m_bMovementWallJumpDisabled(false),
        m_bFrozen(false),
        m_bPlayer(true),
        m_bSuccessFinisher(true),
        m_bFailFinisher(false),
        m_bCollision(true),
        m_bOnMagnet(false),
        m_bKilled(false),
        m_pMagnet(NULL),
        m_width(8),
        m_height(19),
        m_initialPlayerMaxSpeedX(0.0f),
        m_initialPlayerMaxSpeedY(0.0f),
        m_initialJumpGravity(0.0f),
        m_initialPlayerAcceleration(0.0f),
        m_startPos(0.0f, 0.0f, 0.0f),
        m_legRotation(0.0f),
        m_armRotation(0.0f),
        m_chestBreathe(0.0f),
        m_characterDoubleJumpColor(1.0f, 156.0f / 255.0f, 0.0f, 1.0f),
        m_acceleratorTime(0.0f),
        m_currentAcceleratorTime(0.0f),
        m_characterPlayerColor(0.0f, 1.0f, 1.0f, 1),
        m_characterShadowColor(0.3f, 0.3f, 0.3f, 0.3f),
        m_timeShape(-1.0f),
        m_silhouetteRadius(0.0f),
        m_silhouetteOpacity(0.0f),
        m_currentMagnetEnterTime(0.0f)
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

        m_bGhost = false;
    }

    CCharacter2DEntity::~CCharacter2DEntity()
    {
        std::vector< std::string > characterSoundFiles;
        characterSoundFiles.resize(CHARACTER_SOUND_END);

        characterSoundFiles[CHARACTER_SOUND_MOVE] = "CharacterMove.wav";
        characterSoundFiles[CHARACTER_SOUND_JUMP] = "CharacterJump.wav";
        characterSoundFiles[CHARACTER_SOUND_SECONDJUMP] = "CharacterSecondJump.wav";
        characterSoundFiles[CHARACTER_SOUND_HITFLOOR] = "CharacterHitFloor.wav";
        characterSoundFiles[CHARACTER_SOUND_SLIDEWALL] = "CharacterSlideWall.wav";
        characterSoundFiles[CHARACTER_SOUND_START] = "CharacterStart.wav";


        for (int i = 0; i < m_characterSounds.size(); ++i)
        {
            gEngine->GetAudioSystem()->Release(characterSoundFiles[i]);
        }

        if (m_pNode)
            gEngine->GetSceneMgr()->RemoveNode(m_pNode->GetName());

        if (m_pCharacterBodyNode)
            gEngine->GetSceneMgr()->RemoveNode(m_pCharacterBodyNode->GetName());

        if (m_pCharacterChestNode)
            gEngine->GetSceneMgr()->RemoveNode(m_pCharacterChestNode->GetName());

        if (m_pCharacterHeadNode)
            gEngine->GetSceneMgr()->RemoveNode(m_pCharacterHeadNode->GetName());

        if (m_pCharacterLeg01Node)
            gEngine->GetSceneMgr()->RemoveNode(m_pCharacterLeg01Node->GetName());

        if (m_pCharacterLeg02Node)
            gEngine->GetSceneMgr()->RemoveNode(m_pCharacterLeg02Node->GetName());

        if (m_pCharacterArm01Node)
            gEngine->GetSceneMgr()->RemoveNode(m_pCharacterArm01Node->GetName());

        if (m_pCharacterArm02Node)
            gEngine->GetSceneMgr()->RemoveNode(m_pCharacterArm02Node->GetName());

        if (m_pParticlesEnergyParticles)
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticlesEnergyParticles->GetName());

        if (m_pTraceFloorParticles)
            gEngine->GetEntityMgr()->RemoveEntity(m_pTraceFloorParticles->GetName());

        if (m_pJumpParticles)
            gEngine->GetEntityMgr()->RemoveEntity(m_pJumpParticles->GetName());

        if (m_pParticlesDeath)
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticlesDeath->GetName());

        if (m_pParticlesBirth)
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticlesBirth->GetName());

        if (m_pParticlesTrace)
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticlesTrace->GetName());

        if (m_pParticlesDoubleJump)
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticlesDoubleJump->GetName());

        if (m_pParticlesSuper)
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticlesSuper->GetName());

        if (m_pLight)
            gEngine->GetEntityMgr()->RemoveEntity(m_pLight->GetName());

        gScriptMgr->Release("../../Bin/game.txt");

        gEngine->GetEntityMgr()->SetColliderEntity(NULL);
    }


    void CCharacter2DEntity::LoadScriptValues()
    {
        float acceleration = 1000;
        float gravity = 1200;
        float jumpGravity = 485;
        float jumpTime = 0.50f;
        float maxSpeedX = 400.0f;
        float maxSpeedY = 1000.0f;
        float superSpeed = 1.55f;

        m_playerAcceleration = acceleration;
        m_initialPlayerAcceleration = m_playerAcceleration;


        m_playerMaxSpeedX = maxSpeedX;
        m_initialPlayerMaxSpeedX = m_playerMaxSpeedX;


        m_playerMaxSpeedY = maxSpeedY;
        m_initialPlayerMaxSpeedY = m_playerMaxSpeedY;


        m_jumpGravity = jumpGravity;
        m_initialJumpGravity = m_jumpGravity;


        m_jumpTime = jumpTime;

        m_gravity = gravity;

        m_playerSuperSpeed = superSpeed;
    }

    void CCharacter2DEntity::InitializeAtCreation()
    {
        CEntity::InitializeAtCreation();

        m_pNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pNode->Serialize();
        gEngine->GetSceneMgr()->GetRootNode()->AttachChildNode(m_pNode);

        m_pLevel = gEngine->GetWorldMgr()->GetLevelMgr();

        LoadScriptValues();

        m_pCharacterBodyNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pCharacterChestNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pCharacterHeadNode = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pCharacterLeg01Node = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pCharacterLeg02Node = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pCharacterArm01Node = gEngine->GetSceneMgr()->CreateSceneNode();
        m_pCharacterArm02Node = gEngine->GetSceneMgr()->CreateSceneNode();


        m_pNode->AttachChildNode(m_pCharacterBodyNode);
        m_pCharacterBodyNode->AttachChildNode(m_pCharacterChestNode);
        m_pCharacterBodyNode->AttachChildNode(m_pCharacterLeg01Node);
        m_pCharacterBodyNode->AttachChildNode(m_pCharacterLeg02Node);
        m_pCharacterChestNode->AttachChildNode(m_pCharacterHeadNode);
        m_pCharacterChestNode->AttachChildNode(m_pCharacterArm01Node);
        m_pCharacterChestNode->AttachChildNode(m_pCharacterArm02Node);




        m_pNode->SetScale(Vec3(0.37f, 0.37f, 0.37f));


        InitHitbox();

        InitSprites();

        InitParticleSystems();

        InitSounds();

        GetSprites(m_characterRenderObject.m_sprites);

        m_startPos = m_pNode->GetDerivedPosition();

        gEngine->GetEntityMgr()->SetColliderEntity(this);
    }

    void CCharacter2DEntity::InitializeAtLoad()
    {
        CEntity::InitializeAtLoad();

        m_pLevel = gEngine->GetWorldMgr()->GetLevelMgr();

        LoadScriptValues();

        InitHitbox();

        InitSprites();

        if (m_pTraceParticles)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_pTraceParticles->GetName());
            m_pTraceParticles = NULL;
        }

        if (m_pTraceFloorParticles)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_pTraceFloorParticles->GetName());
            m_pTraceFloorParticles = NULL;
        }

        if (m_pJumpParticles)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_pJumpParticles->GetName());
            m_pJumpParticles = NULL;
        }

        InitParticleSystems();

        InitSounds();

        GetSprites(m_characterRenderObject.m_sprites);

        gEngine->GetEntityMgr()->SetColliderEntity(this);

    }

    void CCharacter2DEntity::InitHitbox(bool bCreate)
    {
        if (bCreate && GetNumPolygons() > 0)
            return;

        RemoveCircles();
        RemovePolygons();

        if (bCreate)
        {
            CPolygon p;
            p.SetNumPoints(4);
            p.AddPoint(Vec3(-m_width, -m_height, 0.0));
            p.AddPoint(Vec3(-m_width, m_height, 0.0));
            p.AddPoint(Vec3(m_width, m_height, 0.0));
            p.AddPoint(Vec3(m_width, -m_height, 0.0));
            AddPolygon(p);
        }

    }

    void CCharacter2DEntity::InitParticleSystems()
    {
        if (!m_pParticlesTrace)
        {
            m_pParticlesTrace = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);
            m_pParticlesTrace->LoadFromTemplate("CHARACTER_FLOOR_TRACE");
            m_pParticlesTrace->SetRenderGame(false);
            m_pParticlesTrace->SetRenderEditor(false);
            m_pParticlesTrace->SetEmitNewParticles(true);
            m_pParticlesTrace->SetPickable(false);
            m_pParticlesTrace->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
        }
        else
            m_pParticlesTrace->LoadFromTemplate("CHARACTER_FLOOR_TRACE");

        if (!m_pParticlesDeath)
        {
            m_pParticlesDeath = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);
            m_pParticlesDeath->LoadFromTemplate("CHARACTER_DEATH");
            m_pParticlesDeath->SetRenderGame(false);
            m_pParticlesDeath->SetRenderEditor(false);
            m_pParticlesDeath->SetEmitNewParticles(false);
            m_pParticlesDeath->SetPickable(false);
            m_pParticlesDeath->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
        }
        else
            m_pParticlesDeath->LoadFromTemplate("CHARACTER_DEATH");

        if (!m_pParticlesBirth)
        {
            m_pParticlesBirth = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);
            m_pParticlesBirth->LoadFromTemplate("CHARACTER_BIRTH");
            m_pParticlesBirth->SetRenderGame(false);
            m_pParticlesBirth->SetRenderEditor(false);
            m_pParticlesBirth->SetEmitNewParticles(false);
            m_pParticlesBirth->SetPickable(false);
            m_pParticlesBirth->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
        }
        else
            m_pParticlesBirth->LoadFromTemplate("CHARACTER_BIRTH");

        if (m_pParticlesEnergyParticles)
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticlesEnergyParticles->GetName());

        m_pParticlesEnergyParticles = NULL;

        if (m_pParticlesDoubleJump)
            gEngine->GetEntityMgr()->RemoveEntity(m_pParticlesDoubleJump->GetName());

        m_pParticlesDoubleJump = NULL;

        if (!m_pParticlesSuper)
        {
            m_pParticlesSuper = (CParticleSystemEntity*)gEngine->GetEntityMgr()->CreateEntity(CParticleSystemEntity::ENTITY_META_TYPEID);
            m_pParticlesSuper->LoadFromTemplate("CHARACTER_SUPER");
            m_pParticlesSuper->SetRenderGame(false);
            m_pParticlesSuper->SetRenderEditor(false);
            m_pParticlesSuper->SetEmitNewParticles(false);
            m_pParticlesSuper->SetPickable(false);
            m_pParticlesSuper->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
        }
        else
        {
            m_pParticlesSuper->LoadFromTemplate("CHARACTER_SUPER");
        }

        m_pParticlesTrace->SetRenderEditor(false);
        m_pParticlesDeath->SetRenderEditor(false);
        m_pParticlesBirth->SetRenderEditor(false);
        m_pParticlesDeath->SetRenderEditor(false);
        m_pParticlesTrace->SetRenderGame(false);
        m_pParticlesDeath->SetRenderGame(false);
        m_pParticlesBirth->SetRenderGame(false);
        m_pParticlesDeath->SetRenderGame(false);
        m_pParticlesDeath->SetStartDisabled(true);


        m_pParticlesTrace->SetPickable(false);
        m_pParticlesDeath->SetPickable(false);
        m_pParticlesBirth->SetPickable(false);
        m_pParticlesDeath->SetPickable(true);

        //m_pParticlesTrace->SetRenderer("Default");
        m_pParticlesSuper->SetRenderer("ParticleCharacterRenderer");
        m_pParticlesBirth->SetRenderer("ParticleCharacterRenderer");
        m_pParticlesDeath->SetRenderer("ParticleCharacterRenderer");
        m_pParticlesBirth->SetStartDisabled(true);
    }


    void CCharacter2DEntity::InitSounds()
    {
        std::vector< std::string > characterSoundFiles;
        characterSoundFiles.resize(CHARACTER_SOUND_END);

        characterSoundFiles[CHARACTER_SOUND_MOVE] = "CharacterMove.wav";
        characterSoundFiles[CHARACTER_SOUND_JUMP] = "CharacterJump.wav";
        characterSoundFiles[CHARACTER_SOUND_SECONDJUMP] = "CharacterSecondJump.wav";
        characterSoundFiles[CHARACTER_SOUND_HITFLOOR] = "CharacterHitFloor.wav";
        characterSoundFiles[CHARACTER_SOUND_SLIDEWALL] = "CharacterSlideWall.wav";
        characterSoundFiles[CHARACTER_SOUND_START] = "CharacterStart.wav";

        m_characterSounds.resize(CHARACTER_SOUND_END);


        for (int i = 0; i < m_characterSounds.size(); ++i)
        {
            m_characterSounds[i] = gEngine->GetAudioSystem()->AddGetPtr(characterSoundFiles[i]);
            m_characterSounds[i]->SetChannelGroup(AUDIO_CHANNELGROUP_EFFECTS);
        }
    }

    void CCharacter2DEntity::InitSprites()
    {
        m_spriteCharHead.SetMaterialFilename("Sprites/character.png");
        m_spriteCharChest.SetMaterialFilename("Sprites/character.png");
        m_spriteCharBody.SetMaterialFilename("Sprites/character.png");
        m_spriteCharArm01.SetMaterialFilename("Sprites/character.png");
        m_spriteCharArm02.SetMaterialFilename("Sprites/character.png");
        m_spriteCharLeg01.SetMaterialFilename("Sprites/character.png");
        m_spriteCharLeg02.SetMaterialFilename("Sprites/character.png");


        m_spriteCharHead.SetUpdateRectByAnimation(false);
        m_spriteCharChest.SetUpdateRectByAnimation(false);
        m_spriteCharBody.SetUpdateRectByAnimation(false);
        m_spriteCharArm01.SetUpdateRectByAnimation(false);
        m_spriteCharArm02.SetUpdateRectByAnimation(false);
        m_spriteCharLeg01.SetUpdateRectByAnimation(false);
        m_spriteCharLeg02.SetUpdateRectByAnimation(false);



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
        m_spriteCharHead.SetLogicalRect(rect);
        center.x = 32.0f;
        center.y = 58.0f;
        m_spriteCharHead.SetCenter(center);

        // Chest
        rect.top = 0.0f;
        rect.left = 64.0f / width;
        rect.right = 128.0f / width;
        rect.bottom = 64.0f / height;
        m_spriteCharChest.SetLogicalRect(rect);
        m_spriteCharChest.SetCenter(center);

        // Body
        rect.top = 64.0f / height;
        rect.left = 0.0f;
        rect.right = 64.0f / width;
        rect.bottom = 128.0f / height;
        m_spriteCharBody.SetLogicalRect(rect);
        m_spriteCharBody.SetCenter(center);

        // Arm01
        rect.top = 64.0f / height;
        rect.left = 64.0f / height;
        rect.right = 128.0f / width;
        rect.bottom = 128.0f / height;
        m_spriteCharArm01.SetLogicalRect(rect);
        center.x = 32.0f;
        center.y = 18.0f;
        m_spriteCharArm01.SetCenter(center);

        // Arm02
        m_spriteCharArm02.SetLogicalRect(rect);
        m_spriteCharArm02.SetCenter(center);

        // Leg01
        rect.top = 128.0f / height;
        rect.left = 0.0f;
        rect.right = 64.0f / width;
        rect.bottom = 196.0f / height;
        m_spriteCharLeg01.SetLogicalRect(rect);
        center.x = 32.0f;
        center.y = 4.0f;
        m_spriteCharLeg01.SetCenter(center);

        // Leg02
        rect.top = 128.0f / height;
        rect.left = 0.0f;
        rect.right = 64.0f / width;
        rect.bottom = 196.0f / height;
        /*rect.top = 128.0f / height;
        rect.left = 64.0f / width;
        rect.right = 128.0f / width;
        rect.bottom = 196.0f / height;*/
        m_spriteCharLeg02.SetLogicalRect(rect);
        m_spriteCharLeg02.SetCenter(center);

        m_spriteCharBody.AttachNode(m_pCharacterBodyNode);
        m_spriteCharChest.AttachNode(m_pCharacterChestNode);
        m_spriteCharArm01.AttachNode(m_pCharacterArm01Node);
        m_spriteCharArm02.AttachNode(m_pCharacterArm02Node);
        m_spriteCharHead.AttachNode(m_pCharacterHeadNode);
        m_spriteCharLeg01.AttachNode(m_pCharacterLeg01Node);
        m_spriteCharLeg02.AttachNode(m_pCharacterLeg02Node);
    }

    void CCharacter2DEntity::SetPlayer(bool bSet)
    {
        m_bPlayer = bSet;

        InitParticleSystems();

        m_spriteCharHead.SetColor(m_bPlayer ? m_characterPlayerColor : m_characterShadowColor);
        m_spriteCharBody.SetColor(m_bPlayer ? m_characterPlayerColor : m_characterShadowColor);
        m_spriteCharHead.SetColor(m_bPlayer ? m_characterPlayerColor : m_characterShadowColor);
        m_spriteCharArm01.SetColor(m_bPlayer ? m_characterPlayerColor : m_characterShadowColor);
        m_spriteCharArm02.SetColor(m_bPlayer ? m_characterPlayerColor : m_characterShadowColor);
        m_spriteCharLeg01.SetColor(m_bPlayer ? m_characterPlayerColor : m_characterShadowColor);
        m_spriteCharLeg02.SetColor(m_bPlayer ? m_characterPlayerColor : m_characterShadowColor);
    }


    void CCharacter2DEntity::SetPosition(const Vec3& vPos)
    {
        m_pNode->SetPosition(vPos);

        const Vec3& charPos = m_pNode->GetDerivedPosition();
        Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), charPos));

        ApplyTransform(mtxTranslation);

        m_pos = VECTOR2FIXED((int)(vPos.x * CELL_SCALE), (int)(vPos.y * CELL_SCALE));

        m_collisionState.pos = m_pos;
    }

    void CCharacter2DEntity::SetNewCollisionState(const CollisionState& col)
    {
        m_collisionState = col;
        m_pos = m_collisionState.pos;
        m_pNode->SetPosition(Vec3((float)m_collisionState.pos.x / (float)CELL_SCALE, (float)m_collisionState.pos.y / (float)CELL_SCALE, 0.0f));

        const Vec3& charPos = m_pNode->GetDerivedPosition();
        Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), charPos));
        ApplyTransform(mtxTranslation);
    }


    void CCharacter2DEntity::OnGameModeChanged(bool bGameMode)
    {
        CEntity::OnGameModeChanged(bGameMode);

        m_bJumpingUp = false;
        if (gEngine->GetInput()->IsActionTriggered(ACTION_PRIMARY))
            m_bJumpLocked = true;

        m_pNode->SetScale(Vec3(0.37f, 0.37f, 0.37f));

        m_pCharacterBodyNode->SetOrientation(CQuaternion());

        m_bVisible = true;
        m_bPrimaryKeyPressed = false;
        m_bSecondaryKeyPressed = false;
        m_bKeyLeftPressed = false;
        m_bKeyRightPressed = false;
        m_bSecondJump = false;
        m_bSecondJumpBlocked = false;
        m_bOnPlatform = false;
        m_bKilled = false;
        m_bMovementFlipInput = false;
        m_bMovementDisabled = false;
        m_bMovementSprintDisabled = false;
        m_bMovementJumpDisabled = false;
        m_bMovementDoubleJumpDisabled = false;
        m_bMovementWallGrabDisabled = false;
        m_bMovementWallJumpDisabled = false;
        m_bFrozen = false;
        m_bInForceField = false;

        m_pPlatformNode = NULL;

        m_vVelocity.x = 0.0f;
        m_vVelocity.y = 0.0f;

        m_velocity.x = 0;
        m_velocity.y = 0;

        m_silhouetteRadius = 0.0f;
        m_silhouetteRadius = 0.0f;

        m_timeShape = 0.0f;

        m_wallStickTimer.Reset();
        m_jumpTimer.Reset();

        m_jumpDelayTimer.Reset();
        m_jumpDelayTimer.Start();
        m_jumpDelayTimer.SetTime(5.0f);


        if (bGameMode)
        {
            CollisionState colRes;
            colRes.pos = VECTOR2FIXED(m_startPos.x * CELL_SCALE, m_startPos.y * CELL_SCALE);
            SetNewCollisionState(colRes);
        }


        if (bGameMode)
            InitHitbox(m_bCollision);
        else
            InitHitbox(true);

        m_acceleratorTime = 0.0f;
        m_currentAcceleratorTime = 0.0f;
        m_acceleratorForce = Vec2(0.0f, 0.0f);
        m_bOnMagnet = false;
        m_pMagnet = NULL;
        m_bOnPlatform = false;
        m_pPlatformNode = NULL;

        m_pParticlesBirth->GetNode()->SetPosition(m_pNode->GetDerivedPosition() + Vec3(0.0f, 0.0f, -10.0f));
        //m_pParticlesBirth->EmitAllParticlesOnce();

        m_pParticlesDeath->SetRenderParticles(false);
        m_pParticlesDeath->SetEmitNewParticles(false);

        m_pParticlesTrace->SetEmitNewParticles(false);
        //m_pParticlesDoubleJump->SetEmitNewParticles(false);

        m_pParticlesDeath->SetEmitNewParticles(false);
        m_pParticlesTrace->SetEmitNewParticles(false);
        //m_pParticlesDoubleJump->SetEmitNewParticles(false);
        //m_pParticlesJump->SetEmitNewParticles(false);

        if (CHARACTER_SOUND_START < m_characterSounds.size())
            m_characterSounds[CHARACTER_SOUND_START]->PlaySound();
    }

    bool CCharacter2DEntity::HasCollision(CEntity& e)
    {
        if (e.GetMetaTypeId() == ENTITY_TYPEID_CHECKPOINT && CGeometryCollidable::HasCollision(e))
        {
            return true;
        }
        if (e.GetMetaTypeId() == ENTITY_TYPEID_ACCELERATOR && CGeometryCollidable::HasCollision(e))
        {
            return true;
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
        /*if(e.GetMetaTypeId() == ENTITY_TYPEID_MISSILE && CGeometryCollidable::HasCollision(e))
        {
        SetKilled(true);
        return true;
        }*/
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
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_HUNTINGHUNTER && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
            return true;
        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_EXPLODINGHUNTER && CGeometryCollidable::HasCollision(e))
        {
            SetKilled(true);
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
            m_bJumpingUp = false;
            m_bJumping = false;

            m_bInForceField = true;
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

    bool CCharacter2DEntity::HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode)
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

    void CCharacter2DEntity::OnTriggerEntered(CEntity& e)
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

            m_bJumping = false;
            m_bJumpingUp = false;
            m_bSecondJump = false;
            m_bSecondJumpBlocked = false;


        }
        else if (e.GetMetaTypeId() == CMagnetEntity::ENTITY_META_TYPEID)
        {
            m_bOnMagnet = true;
            m_pMagnet = static_cast<CMagnetEntity*>(&e);

            m_magnetEnterPosition = m_pMagnet->GetNode()->GetDerivedPosition();
            m_currentMagnetEnterTime = 0.0f;

            m_collisionState.bOnFloor = true;

            m_bJumping = false;
            m_bJumpingUp = false;
            m_bSecondJump = false;
            m_bSecondJumpBlocked = false;

        }
        else if (e.GetMetaTypeId() == ENTITY_TYPEID_FORCEFIELD)
        {
            m_bJumping = false;
        }
    }


    bool CCharacter2DEntity::CheckPlayerOutsideLevel()
    {
        const LevelBorderOffsets& offsets = gEngine->GetWorldMgr()->GetScenario()->GetLevelBorderOffsets();
        float levelWidth    = static_cast<float>(m_pLevel->GetLevelGrid().GetNumCellsX() * CELL_SIZE - (int)offsets.right * CELL_SIZE + GetWidth());
        float levelHeight   = static_cast<float>(m_pLevel->GetLevelGrid().GetNumCellsY() * CELL_SIZE - (int)offsets.top * CELL_SIZE + GetHeight());
        float levelLeft     = static_cast<float>((int)offsets.left * CELL_SIZE - GetWidth());
        float levelBottom   = static_cast<float>((int)offsets.bottom * CELL_SIZE - GetHeight());
        
        const Vec3& vCharPos = GetNode()->GetDerivedPosition();

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

    void CCharacter2DEntity::Update(float dt)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CCharacter2DEntity::Update);

        if (gEngine->GetInput()->IsActionJustReleased(ACTION_PRIMARY))
        {
            m_bJumpLocked = false;
        }

        if (m_bPicked)
        {
            CollisionState colRes;
            m_startPos = m_pNode->GetDerivedPosition();
            colRes.pos = VECTOR2FIXED(m_startPos.x * CELL_SCALE, m_startPos.y * CELL_SCALE);
            SetNewCollisionState(colRes);
            return;
        }
        else if (!GetGameMode())
        {
            CollisionState colRes;
            colRes.pos = VECTOR2FIXED(m_startPos.x * CELL_SCALE, m_startPos.y * CELL_SCALE);
            SetNewCollisionState(colRes);
            return;
        }

        if (m_bKilled)
        {

            return;
        }

        CEntity::Update(dt);



        if (m_bFrozen)
        {
            const Vec3& charPos = m_pNode->GetDerivedPosition();
            Matrix4x4 mtxTranslation = glm::transpose(glm::translate(Matrix4x4(1.0f), charPos));

            ApplyTransform(mtxTranslation);

            return;
        }

        float levelTime = gEngine->GetWorldMgr()->GetLevelTime();
        if (levelTime < 0.1f)
        {
            m_pNode->SetScale(Vec3(0.0f, 0.0f, 0.0f));
            return;
        }
        else if (levelTime < 0.3f)
        {
            float alpha = (levelTime - 0.1f) / 0.2f;
            m_pNode->SetScale(Lerp(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.37f, 0.37f, 0.37f), alpha));
            return;
        }



        CheckPlayerOutsideLevel();





        //if(m_bOnMagnet)
        //	m_pParticles->SetEmitNewParticles(false);

        if (m_bInForceField && m_pForceField)
        {
            Vec2 force(glm::normalize(m_pForceField->GetNode()->GetDerivedUp()));
            force *= static_cast<CForceFieldEntity*>(m_pForceField)->GetAcceleration() * dt;

            //m_vVelocity += force;

            Vec2 targetVelo = Vec2(m_pForceField->GetNode()->GetDerivedUp()) * m_pForceField->GetTargetVelocity();
            float proj = ComputePointOnLineProjection(Vec3(m_vVelocity.x, m_vVelocity.y, 0), Vec3(0, 0, 0), Vec3(targetVelo.x, targetVelo.y, 0));
            if (proj < 1.0f)
                m_vVelocity += force;

            m_acceleratorTime = m_currentAcceleratorTime = 0.0f;
            if (abs(m_currentAcceleratorForce.x) > 0.0f)
                m_vVelocity.x += m_currentAcceleratorForce.x;
            if (abs(m_currentAcceleratorForce.y) > 0.0f)
                m_vVelocity.y += m_currentAcceleratorForce.y;
            m_currentAcceleratorForce = Vec2(0.0f, 0.0f);

        }



        m_bFirstJumpFrame = false;

        float stickWallTime = 0.25f;
        float floorFrictionParameter = 0.2f;
        float rampFrictionParameter = 0.7f;
        float jumpDelayTimer = 0.08f;



        if ((!m_prevCollisionState.bOnFloor && m_collisionState.bOnFloor) ||
            (!m_prevCollisionState.bHitCeiling && m_collisionState.bHitCeiling) /*|| m_collision.bHitRamp*/)
        {
            m_bJumpingUp = false;
            m_vVelocity.y = 0.0f;
        }
        else if (gEngine->GetInput()->IsActionJustReleased(ACTION_PRIMARY) &&
                 m_bJumpingUp &&
                 (m_jumpTimer.GetTime() >= 0.00f ||
                 m_bJumpedFromWallFromLeft ||
                 m_bJumpedFromWallFromRight))
        {

            if (m_bJumpedFromWallFromLeft ||
                m_bJumpedFromWallFromRight)
            {
                //m_vVelocity.y *= 0.5f;
            }
            else
            {
                //m_vVelocity.y *= jumpStoppedDampingFactor;
            }


            m_bJumpingUp = false;
        }
        else if (m_jumpTimer.GetTime() > m_jumpTime && m_bJumpingUp)
        {
            m_bJumpingUp = false;
        }

        if (m_collisionState.bOnFloor ||
            m_collisionState.bOnRamp ||
            m_collisionState.bGrabbedWallFromLeft ||
            m_collisionState.bGrabbedWallFromRight ||
            (m_collisionState.bHitCeiling && m_collisionState.bStickyCeiling))
        {
            m_bJumpedFromWallFromLeft = false;
            m_bJumpedFromWallFromRight = false;
            m_bSecondJumpBlocked = false;
            m_bJumping = false;
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
            m_vVelocity.y = -abs(m_vVelocity.x) * 0.35f;
        }


        if (m_bMovementDoubleJumpDisabled || m_bMovementDisabled)
            m_bSecondJumpBlocked = true;

        static bool bInFF = false;

        if (!m_bInForceField && bInFF)
        {
            m_bSecondJumpBlocked = false;
            m_bSecondJump = false;
        }

        bInFF = m_bInForceField;

        m_currentMagnetEnterTime += dt;

        //if (m_bOnMagnet && m_currentMagnetEnterTime < 0.3f)
        //	m_bJumpLocked = true;

        if (m_bOnMagnet)
        {
            if (gEngine->GetInput()->IsActionJustTriggered(ACTION_PRIMARY) && !m_bMovementDisabled)
            {
                Vec3 vForce = m_pMagnet->GetNode()->GetDerivedUp() * m_pMagnet->GetSpeed();
                m_collisionState.bOnFloor = false;
                m_collisionState.bHitCeiling = false;
                m_collisionState.bGrabbedWallFromLeft = false;
                m_collisionState.bGrabbedWallFromRight = false;
                m_vVelocity = Vec2(0.0f, 0.0f);

                m_acceleratorTime = m_pMagnet->GetTime();
                m_currentAcceleratorTime = m_acceleratorTime;
                m_acceleratorForce = Vec2(vForce);

                m_bJumpLocked = true;
                m_bJumpingUp = false;
                m_bOnMagnet = false;
                m_pMagnet = NULL;
            }

        }
        else
        {


            //if(!((m_collisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight) && 
            //	  m_bMovementWallJumpDisabled && m_bMovementDisabled &&
            //	  !(m_collisionState.bOnFloor || m_collisionState.bOnRamp || (m_collisionState.bHitCeiling && m_collisionState.bStickyCeiling))))
            {
                if ((gEngine->GetInput()->IsActionTriggered(ACTION_PRIMARY)) &&
                    !m_bMovementDisabled &&
                    !m_bInForceField &&
                    !m_bJumpLocked &&
                    (m_collisionState.bOnFloor ||
                    m_collisionState.bOnRamp ||
                    (m_collisionState.bGrabbedWallFromLeft && !m_bMovementWallJumpDisabled) ||
                    (m_collisionState.bGrabbedWallFromRight && !m_bMovementWallJumpDisabled) ||
                    m_collisionState.bHitCeiling ||
                    m_jumpDelayTimer.GetTime() < jumpDelayTimer ||
                    (!m_bSecondJumpBlocked && !m_bMovementDoubleJumpDisabled && !m_collisionState.bGrabbedWallFromLeft && !m_collisionState.bGrabbedWallFromRight)))
                {

                    if (m_collisionState.bOnFloor || m_collisionState.bOnRamp || m_collisionState.bGrabbedWallFromLeft ||
                        m_collisionState.bGrabbedWallFromRight || m_collisionState.bHitCeiling || m_jumpDelayTimer.GetTime() < jumpDelayTimer)
                    {
                        m_bSecondJump = false;
                        m_jumpDelayTimer.SetTime(5.0f);
                    }
                    else
                    {
                        m_bSecondJump = true;
                        m_bSecondJumpBlocked = true;

                        if ((m_bJumpedFromWallFromLeft && m_bKeyRightPressed) ||
                            (m_bJumpedFromWallFromRight && m_bKeyLeftPressed))
                        {
                            m_vVelocity *= 0.0f;
                        }



                    }

                    m_bJumpedFromWallFromLeft = false;
                    m_bJumpedFromWallFromRight = false;


                    m_bJumpingUp = true;
                    m_bJumping = true;
                    m_bFirstJumpFrame = true;
                    m_jumpTimer.Reset();
                    m_jumpTimer.Stop();

                    if (m_collisionState.bHitCeiling)
                    {
                        m_collisionState.bHitCeiling = false;
                        m_bJumpingUp = false;
                        m_bFirstJumpFrame = false;
                    }
                }
                if (gEngine->GetInput()->IsActionTriggered(ACTION_PRIMARY) && !m_bMovementDisabled)
                    m_bJumpLocked = true;
            }
        }


        UpdateSounds();
        UpdateParticleSystems(dt);



        m_jumpDelayTimer.Update(dt);
        m_jumpTimer.Update(dt);
        m_wallStickTimer.Update(dt);

        if (gEngine->GetInput()->IsActionTriggered(ACTION_RIGHT) && !m_bMovementDisabled)
        {
            m_bKeyRightPressed = true;
            m_bKeyLeftPressed = false;
        }
        else if (gEngine->GetInput()->IsActionTriggered(ACTION_LEFT) && !m_bMovementDisabled)
        {
            m_bKeyLeftPressed = true;
            m_bKeyRightPressed = false;
        }

        if (!m_bMovementSprintDisabled && gEngine->GetInput()->IsActionTriggered(ACTION_SECONDARY) && !m_bMovementDisabled)
            m_bSecondaryKeyPressed = true;
        else
        {
            m_bSecondaryKeyPressed = false;
        }

        if (m_bMovementFlipInput)
        {
            std::swap(m_bKeyRightPressed, m_bKeyLeftPressed);
        }

        if (m_bSecondaryKeyPressed)
        {
            m_playerMaxSpeedX = m_initialPlayerMaxSpeedX * m_playerSuperSpeed;
            m_playerAcceleration = m_initialPlayerAcceleration * m_playerSuperSpeed;
        }
        else
        {
            m_playerMaxSpeedX = m_initialPlayerMaxSpeedX;
            //m_playerMaxSpeedY = m_initialPlayerMaxSpeedY;
            m_playerAcceleration = m_initialPlayerAcceleration;
            m_jumpGravity = m_initialJumpGravity;
        }


        // Check if stick wall timer needs to be started / stopped
        //
        if (m_collisionState.bHitWallFromRight)
        {
            if (m_bKeyRightPressed && !m_collisionState.bOnFloor && !m_collisionState.bHitRamp)
                m_wallStickTimer.Start();
            else
                m_wallStickTimer.Stop();
        }
        else if (m_collisionState.bHitWallFromleft)
        {
            if (m_bKeyLeftPressed && !m_collisionState.bOnFloor && !m_collisionState.bHitRamp)
                m_wallStickTimer.Start();
            else
                m_wallStickTimer.Stop();
        }
        else
            m_wallStickTimer.Stop();

        if ((!m_prevCollisionState.bHitWallFromleft && m_collisionState.bHitWallFromleft) ||
            (!m_prevCollisionState.bHitWallFromRight && m_collisionState.bHitWallFromRight))
        {
            m_wallStickTimer.Reset();
        }


        // Update acceleration based on new collision state
        //
        if (m_collisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight || m_collisionState.bHitWallFromleft || m_collisionState.bHitWallFromRight)
            m_vVelocity.x = 0.0f;

        /*if(m_vVelocity.x > m_playerMaxSpeedX)
            m_vVelocity.x = m_playerMaxSpeedX;
            else if(m_vVelocity.x < -m_playerMaxSpeedX)
            m_vVelocity.x = -m_playerMaxSpeedX;*/

        if (m_bKeyRightPressed)
        {
            if (m_collisionState.bHitWallFromRight &&
                m_wallStickTimer.GetTime() < stickWallTime &&
                !m_bInForceField &&
                !m_collisionState.bOnFloor &&
                !m_collisionState.bHitRamp)
                // Check if player should stick on wall
            {
                m_vVelocity.x = 0.0f;
            }
            else if (!m_collisionState.bGrabbedWallFromLeft)
                // Apply acceleration
            {
                if (m_collisionState.bOnFloor && m_vVelocity.x < -m_playerMaxSpeedX)
                    m_vVelocity.x += m_playerAcceleration * 4 * dt;
                else if (m_vVelocity.x < 0.0f)
                    m_vVelocity.x += m_playerAcceleration * 2 * dt;
                else
                {
                    if (m_vVelocity.x > m_playerMaxSpeedX && !m_bInForceField)
                        m_vVelocity.x = std::max(m_vVelocity.x - m_playerAcceleration * 2 * dt, m_playerMaxSpeedX);
                    else if (m_vVelocity.x < m_playerMaxSpeedX)
                        m_vVelocity.x += m_playerAcceleration * dt;
                }
            }

            /*if(m_bJumpingUp && m_vVelocity.x < 0.0f && m_jumpTimer.GetTime() < 0.1f)
            {
            m_vVelocity.x += m_playerAcceleration * 2.0f * dt;
            }*/
        }
        else if (m_bKeyLeftPressed)
        {
            if (m_collisionState.bHitWallFromleft &&
                m_wallStickTimer.GetTime() < stickWallTime &&
                !m_bInForceField &&
                !m_collisionState.bOnFloor &&
                !m_collisionState.bHitRamp)
                // Check if player should stick on wall
            {
                m_vVelocity.x = 0.0f;
            }
            else if (!m_collisionState.bGrabbedWallFromRight)
                // Apply acceleration
            {
                if (m_collisionState.bOnFloor && m_vVelocity.x > m_playerMaxSpeedX)
                    m_vVelocity.x -= m_playerAcceleration * 4 * dt;
                else if (m_vVelocity.x > 0.0f)
                    m_vVelocity.x -= m_playerAcceleration * 2 * dt;
                else
                {
                    if (m_vVelocity.x < -m_playerMaxSpeedX && !m_bInForceField)
                        m_vVelocity.x = std::min(m_vVelocity.x + m_playerAcceleration * 2 * dt, -m_playerMaxSpeedX);
                    else if (m_vVelocity.x > -m_playerMaxSpeedX)
                        m_vVelocity.x -= m_playerAcceleration * dt;
                }
            }
        }
        else
        {

            if ((m_collisionState.bOnFloor || m_collisionState.bOnRamp))
            {
                if (m_vVelocity.x > m_playerMaxSpeedX)
                {
                    m_vVelocity.x -= m_playerAcceleration * 4 * dt;
                }
                else if (m_vVelocity.x < -m_playerMaxSpeedX)
                {
                    m_vVelocity.x += m_playerAcceleration * 4 * dt;
                }
                else
                    m_vVelocity.x = 0.0f;

                /*float friction = m_initialPlayerAcceleration;
                if(m_vVelocity.x > 0.0f)
                m_vVelocity.x -= min(friction * dt, m_vVelocity.x);
                else if(m_vVelocity.x < 0.0f)
                m_vVelocity.x += min(friction * dt, -m_vVelocity.x);*/
            }
            else
            {
                if (m_collisionState.bHitCeiling && m_collisionState.bStickyCeiling)
                {
                    float friction = m_initialPlayerAcceleration;
                    if (m_vVelocity.x > 0.0f)
                        m_vVelocity.x -= std::min(friction * dt, m_vVelocity.x);
                    else if (m_vVelocity.x < 0.0f)
                        m_vVelocity.x += std::min(friction * dt, -m_vVelocity.x);
                }
                else
                {
                    float friction = abs(m_vVelocity.x) * floorFrictionParameter;
                    if (m_vVelocity.x > 0.0f)
                        m_vVelocity.x -= friction * dt;
                    else if (m_vVelocity.x < 0.0f)
                        m_vVelocity.x += friction * dt;
                }


            }

        }

        if (m_currentAcceleratorTime > 0.0f)
        {
            if (Equals(m_currentAcceleratorTime, m_acceleratorTime))
            {
                m_vVelocity.x = m_acceleratorForce.x;
                m_vVelocity.y = m_acceleratorForce.y;
            }

            m_currentAcceleratorTime -= dt;

            CQuaternion bodyOrientation = m_pCharacterBodyNode->GetOrientation();
            if (abs(bodyOrientation.GetRoll()) > 0.0001f)
            {
                float roll = bodyOrientation.GetRoll();
                bodyOrientation.SetRoll(roll - sgn(roll) * dt * 5.0);
                m_pCharacterBodyNode->SetOrientation(bodyOrientation);
            }

            bodyOrientation.SetRoll(0.0f);
            m_pCharacterBodyNode->SetOrientation(bodyOrientation);
        }
        else
        {
            m_currentAcceleratorForce.x = m_currentAcceleratorForce.y = 0.0f;
        }


        // Update Acceleration vector in y direction
        //
        if (m_collisionState.bOnFloor && !m_collisionState.bOnRamp)
        {
            if (m_vVelocity.y < 0.0f)
                m_vVelocity.y = 0.0f;
        }

        if (!m_collisionState.bOnFloor &&
            !m_collisionState.bOnRamp &&
            m_vVelocity.y > -m_playerMaxSpeedY &&
            (!m_collisionState.bGrabbedWallFromLeft || m_bMovementWallGrabDisabled) &&
            (!m_collisionState.bGrabbedWallFromRight || m_bMovementWallGrabDisabled) &&
            m_currentAcceleratorTime <= 0.0f)
        {
            if (m_vVelocity.y > 0.0f && m_bJumping)
            {
                if (m_bJumpedFromWallFromRight || m_bJumpedFromWallFromLeft)
                    m_vVelocity.y -= m_gravity * 3.0f * dt;
                else
                    m_vVelocity.y -= m_gravity * 3.0f * dt;
            }
            else
                m_vVelocity.y -= m_gravity * dt;
        }

        if (m_collisionState.bHitCeiling && m_collisionState.bStickyCeiling && gEngine->GetInput()->IsActionTriggered(ACTION_PRIMARY))
        {
            m_vVelocity.y = 0.0f;
        }

        if ((m_collisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight))
        {
            if (!m_bMovementWallGrabDisabled)
            {
                if ((!m_prevCollisionState.bGrabbedWallFromLeft && m_collisionState.bGrabbedWallFromLeft) || (!m_prevCollisionState.bGrabbedWallFromRight && m_collisionState.bGrabbedWallFromRight))
                {
                    if (m_vVelocity.y < 0.0f)
                        m_vVelocity.y *= 0.5f;
                }

                if (m_bInForceField)
                {
                    m_vVelocity.y = std::max(-m_playerMaxSpeedY, m_vVelocity.y - m_gravity * dt);
                }
                else
                {

                    if (m_vVelocity.y > 0.0f)
                        m_vVelocity.y = std::max(-m_playerMaxSpeedY, m_vVelocity.y - m_gravity * 3.0f * dt);
                    else
                    {
                        /*if(m_collisionState.bGrabbedWallFromLeft && m_bKeyRightPressed)
                        {
                        m_vVelocity.y = 0.0f;
                        }
                        else if(m_collisionState.bGrabbedWallFromRight && m_bKeyLeftPressed)
                        {
                        m_vVelocity.y = 0.0f;
                        }
                        else*/

                        m_vVelocity.y = std::max(-m_playerMaxSpeedY, m_vVelocity.y - m_gravity * dt * 0.5f);

                    }
                }

            }
            //else
            //	m_vVelocity.y = max(-m_gravity * wallFrictionParameter, m_vVelocity.y - m_gravity * dt * 0.3f);
        }
        else if (!m_collisionState.bOnRamp)
        {
            // Just left ramp
            //
            if (m_prevCollisionState.bOnRamp)
                m_vVelocity.y *= 0.1f;
        }
        else
        {
            // Handle on-ramp situations
            //

            if (!m_prevCollisionState.bOnRamp)
                m_vVelocity.y *= 0.1f;

            m_vVelocity.y = -m_playerMaxSpeedY;


            if (m_vVelocity.x <= 0.0)
            {
                m_vVelocity.x = std::max(m_vVelocity.x, -m_playerMaxSpeedX * rampFrictionParameter);

                if (m_collisionState.onRampLayout == CELL_LAYOUT_BOTLEFT)
                    m_vVelocity.y = rampFrictionParameter * abs(m_vVelocity.x);
            }
            else if (m_vVelocity.x >= 0.0f)
            {
                m_vVelocity.x = std::min(m_vVelocity.x, m_playerMaxSpeedX * rampFrictionParameter);

                if (m_collisionState.onRampLayout == CELL_LAYOUT_BOTRIGHT)
                    m_vVelocity.y = rampFrictionParameter * abs(m_vVelocity.x);
            }
        }


        //static bool bTimedGrabbedFromLeft = false;


        // Jumps
        //


        m_secondJumpAlpha = 1.0f;

        if (m_bJumpingUp)
        {
            float jumpTime = m_jumpTime;
            float factor = 1.0f;

            if (m_bSecondaryKeyPressed)
                factor = m_playerSuperSpeed;

            if (m_bFirstJumpFrame)
            {

                if (m_bSecondJump)
                    m_timeShape = CHAR_SHAPE_TIME;

                m_vVelocity.y = std::max(m_vVelocity.y, 0.0f);
                // Initial jump frame
                //
                m_jumpTimer.Start();

                m_bJumpedFromFloorOrRamp = m_collisionState.bOnFloor || m_collisionState.bOnRamp;

                if (m_collisionState.bGrabbedWallFromRight)
                {
                    if (!m_bJumpedFromFloorOrRamp)
                    {
                        if (!m_bSecondaryKeyPressed)
                            m_vVelocity.x = m_playerMaxSpeedX * 1.0f;
                        else
                            m_vVelocity.x = m_initialPlayerMaxSpeedX * 1.30f;
                    }
                    m_bJumpedFromWallFromRight = true;
                }
                else
                    m_bJumpedFromWallFromRight = false;

                if (m_collisionState.bGrabbedWallFromLeft /*|| bTimedGrabbedFromLeft*/)
                {
                    if (!m_bJumpedFromFloorOrRamp)
                    {
                        if (!m_bSecondaryKeyPressed)
                            m_vVelocity.x = -m_playerMaxSpeedX * 1.0f;
                        else
                            m_vVelocity.x = -m_initialPlayerMaxSpeedX * 1.30f;
                    }
                    m_bJumpedFromWallFromLeft = true;
                }
                else
                    m_bJumpedFromWallFromLeft = false;
            }


            if (m_bJumpedFromWallFromRight || m_bJumpedFromWallFromLeft)
            {
                //jumpTime *= 1.45f;
                if (m_bSecondaryKeyPressed)
                    jumpTime *= 0.85f;
            }


            static bool bPrevFrameJumping = false;

            if (m_jumpTimer.GetTime() <= jumpTime)
            {
                bPrevFrameJumping = true;

                //m_pNode->SetScale(VECTOR3(0.34f, 0.40f, 0.37f));

                //float alpha = std::pow(1.0f - m_jumpTimer.GetTime() / (jumpTime * 1.34f), 3.0f);
                float alpha = 1.0 - m_jumpTimer.GetTime() / jumpTime;
                m_secondJumpAlpha = alpha;
                if (m_bJumpedFromWallFromLeft || m_bJumpedFromWallFromRight)
                {
                    if (!m_bSecondJump)
                    {
                        if (!m_bSecondaryKeyPressed)
                        {
                            if (m_vVelocity.y < (m_jumpGravity * 1.15f))
                                m_vVelocity.y = alpha * m_jumpGravity * 1.15f;
                        }
                        else
                        {
                            if (m_vVelocity.y < (m_jumpGravity * 1.3f))
                                m_vVelocity.y = alpha * m_jumpGravity * 1.3f;
                        }
                    }
                    /*else
                    {
                    if(!m_bSecondaryKeyPressed)
                    {
                    if(m_vVelocity.y < (m_jumpGravity * 1.6f))
                    m_vVelocity.y = alpha * m_jumpGravity * 1.6f;
                    }
                    else
                    {
                    if(m_vVelocity.y < (m_jumpGravity * 1.45f))
                    m_vVelocity.y = alpha * m_jumpGravity * 1.3f;
                    }
                    }*/
                }
                else
                {
                    if (!m_bSecondJump)
                    {
                        if (m_vVelocity.y < (m_jumpGravity * 1.0f))
                            m_vVelocity.y = alpha * m_jumpGravity * 1.0f;
                    }
                    else
                    {
                        if (m_vVelocity.y < (m_jumpGravity * 1.20f))
                            m_vVelocity.y = alpha * m_jumpGravity * 1.20f;
                    }
                }


                if (m_bJumpedFromWallFromRight && !m_bJumpedFromFloorOrRamp)
                {
                    if (m_bKeyLeftPressed)
                    {
                        m_vVelocity.x += m_playerAcceleration * dt * 1.1f;
                    }
                }
                else if (m_bJumpedFromWallFromLeft && !m_bJumpedFromFloorOrRamp)
                {
                    if (m_bKeyRightPressed)
                    {
                        m_vVelocity.x -= m_playerAcceleration * dt * 1.1f;
                    }

                }
            }
        }


        Vec2 vSpeedStripVelo = m_vVelocity;
        if (m_collisionState.bOnFloor)
            vSpeedStripVelo.y = 0.0f;


        // Slide along upper ramps
        //
        if (m_vVelocity.y > 0.0 && m_collisionState.bHitRamp)
        {
            if (Equals(m_vVelocity.x, 0.0f))
            {
                m_vVelocity.y = 0.0f;
            }
            if (m_vVelocity.x <= 0.0 && m_collisionState.hitRampLayout == CELL_LAYOUT_TOPLEFT)
            {
                if (abs(m_vVelocity.y) > m_initialPlayerMaxSpeedX * 0.85f && abs(m_vVelocity.x) > m_initialPlayerMaxSpeedX * 0.85f)
                {
                    m_vVelocity.x = m_vVelocity.y;
                }
                else
                {
                    if (m_vVelocity.y > 0.0)
                        m_vVelocity.y = 0.0f;
                }
            }
            else if (m_vVelocity.x >= 0.0 && m_collisionState.hitRampLayout == CELL_LAYOUT_TOPRIGHT)
            {
                if (abs(m_vVelocity.y) > m_initialPlayerMaxSpeedX * 0.85f && abs(m_vVelocity.x) > m_initialPlayerMaxSpeedX * 0.85f)
                {
                    m_vVelocity.x = -m_vVelocity.y;
                }
                else
                {
                    if (m_vVelocity.y > 0.0)
                        m_vVelocity.y = 0.0f;
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

        VECTOR3FIXED platformCurMove(0, 0, 0);

        if (m_bOnPlatform)
        {
            if (m_vVelocity.y > 0.0f && m_platformCollision.bHitCeiling)
                m_vVelocity.y = 0.0f;
            else if (m_vVelocity.y < 0.0f && m_platformCollision.bOnFloor)
                m_vVelocity.y = 0.0f;
            if (m_vVelocity.x < 0.0f && m_platformCollision.bGrabbedWallFromRight)
                m_vVelocity.x = 0.0f;
            else if (m_vVelocity.x > 0.0f && m_platformCollision.bGrabbedWallFromLeft)
                m_vVelocity.x = 0.0f;
        }

        VECTOR2FIXED velocityMovePlatform((m_vVelocity.x + m_currentAcceleratorForce.x)* dt * CELL_SCALE,
                                          (m_vVelocity.y + m_currentAcceleratorForce.y) * dt * CELL_SCALE);

        if (m_bOnPlatform && m_pPlatformNode)
        {
            if (!gEngine->GetSceneMgr()->IsValid(m_pPlatformNode))
            {
                m_pPlatformNode = NULL;
                m_bOnPlatform = false;
            }
            const Vec3& platformCurPos = m_pPlatformNode->GetDerivedPosition();
            VECTOR3FIXED platformCurPosFixed(platformCurPos.x * CELL_SCALE, platformCurPos.y * CELL_SCALE, 0.0f);
            platformCurMove = platformCurPosFixed - m_platformPrevPos;
            m_platformPrevPos = platformCurPosFixed;

            velocityMovePlatform.x += platformCurMove.x;
            velocityMovePlatform.y += platformCurMove.y;
            velocityMovePlatform.x += m_vPlatformRevertMove.x;
            velocityMovePlatform.y += m_vPlatformRevertMove.y;
        }

        bool bOnFloorOrWallOrCeiling = m_collisionState.bOnFloor || m_collisionState.bGrabbedWallFromLeft || m_collisionState.bHitWallFromleft;
        //bool bGrabbedFromleft = m_collisionState.bGrabbedWallFromLeft;

        if (m_bOnMagnet)
        {
            m_velocity = VECTOR2FIXED(0, 0);


            float alpha = std::min(m_currentMagnetEnterTime / 0.3f, 1.0f);

            Vec3 magnetPos = m_pMagnet->GetNode()->GetDerivedPosition();

            Vec3 charPos = (1.0f - alpha) * m_pNode->GetDerivedPosition() + alpha * magnetPos;

            CollisionState colState;
            colState.pos = VECTOR2FIXED(charPos.x * CELL_SCALE,
                                        charPos.y * CELL_SCALE);

            SetNewCollisionState(colState);

            m_pCharacterBodyNode->SetOrientation(CQuaternion());

        }
        else
        {
            m_velocity = velocityMovePlatform;

            CTerrainCollidable collidable;
            collidable.height   = GetHeight() * 2.0f;
            collidable.width    = GetWidth() * 2.0f;
            collidable.position = m_pos;
            collidable.velocity = m_velocity;
            CollisionState res  = m_pLevel->ComputeMovement(collidable);

            SetNewCollisionState(res);

            if (m_bOnPlatform)
            {
                m_velocity = VECTOR2FIXED((int)(m_vVelocity.x * dt * CELL_SCALE), (int)(m_vVelocity.y * dt * CELL_SCALE));
            }

            CheckPlatforms();
        }

        m_pForceField = NULL;

        if (bOnFloorOrWallOrCeiling && !(m_collisionState.bOnFloor || m_collisionState.bGrabbedWallFromLeft || m_collisionState.bHitWallFromleft))
        {
            //bTimedGrabbedFromLeft = bGrabbedFromleft;

            m_jumpDelayTimer.Reset();
            m_jumpDelayTimer.Start();
            if (m_bJumpingUp)
                m_jumpDelayTimer.SetTime(5.0f);
        }

        //if(m_jumpDelayTimer.GetTime() > 0.08f)
        //	bTimedGrabbedFromLeft = false;
    }


    void CCharacter2DEntity::CheckPlatforms()
    {
        std::list< CEntity* > platforms;
        gEngine->GetEntityMgr()->GetEntities(platforms, ENTITY_TYPEID_PLATFORM);
        gEngine->GetEntityMgr()->GetEntities(platforms, ENTITY_TYPEID_PLATFORMDRONE);

        CPlatformEntity* pPlatform = 0;
        m_bOnPlatform = false;
        for (std::list< CEntity* >::iterator it = platforms.begin(); it != platforms.end(); ++it)
        {
            pPlatform = static_cast<CPlatformEntity*>(*it);
            if (pPlatform && !pPlatform->GetDisabled())
            {
                int charMinX = 0, charMaxX = 0, charMinY = 0, charMaxY = 0,
                    platformMinX = 0, platformMaxX = 0, platformMinY = 0, platformMaxY = 0;

                VECTOR3FIXED platformPos(0, 0, 0);
                /*				if(!m_bOnPlatform)*/
                {
                    Vec3 platformpos = pPlatform->GetNode()->GetDerivedPosition();
                    platformPos = VECTOR3FIXED((int)(platformpos.x * CELL_SCALE), (int)(platformpos.y * CELL_SCALE), 0);
                }

                platformMinX = static_cast<int>(platformPos.x - pPlatform->GetSize().x * pPlatform->GetNode()->GetDerivedScale().x * CELL_SCALE);
                platformMaxX = static_cast<int>(platformPos.x + pPlatform->GetSize().x * pPlatform->GetNode()->GetDerivedScale().x * CELL_SCALE);
                platformMinY = static_cast<int>(platformPos.y - pPlatform->GetSize().y * pPlatform->GetNode()->GetDerivedScale().y * CELL_SCALE);
                platformMaxY = static_cast<int>(platformPos.y + pPlatform->GetSize().y * pPlatform->GetNode()->GetDerivedScale().y * CELL_SCALE);

                charMinX = m_pos.x - m_width * CELL_SCALE;
                charMaxX = m_pos.x + m_width * CELL_SCALE;
                charMinY = m_pos.y - m_height * CELL_SCALE;
                charMaxY = m_pos.y + m_height * CELL_SCALE;

                m_bOnPlatform = false;

                m_vPlatformRevertMove = VECTOR3FIXED(0, 0, 0);

                if (!m_platformCollision.bHitCeiling && !m_platformCollision.bGrabbedWallFromLeft && !m_platformCollision.bGrabbedWallFromRight &&
                    charMaxY >= platformMaxY && charMinY <= platformMaxY &&
                    charMinX <= platformMaxX && charMaxX >= platformMinX &&
                    ((charMinX >= platformMinX && charMaxX <= platformMaxX) ||
                    (charMinX <= platformMinX && charMaxX >= platformMinX && (platformMaxY - charMinY) < (charMaxX - platformMinX)) ||
                    (charMaxX >= platformMaxX && charMinX <= platformMaxX && (platformMaxY - charMinY) < (platformMaxX - charMinX)))
                    )
                {
                    //floor
                    m_platformCollision.bOnFloor = true;
                    m_platformPrevPos = platformPos;
                    m_vPlatformRevertMove = VECTOR3FIXED(0, platformMaxY - charMinY, 0);
                    m_bOnPlatform = true;
                    m_pPlatformNode = pPlatform->GetNode();
                    break;
                }
                else if (!m_platformCollision.bOnFloor && !m_platformCollision.bGrabbedWallFromLeft && !m_platformCollision.bGrabbedWallFromRight &&
                         (charMaxY >= platformMinY && charMinY <= platformMinY) &&
                         ((charMinX >= platformMinX && charMaxX <= platformMaxX) ||
                         (charMinX <= platformMinX && charMaxX >= platformMinX && (charMaxY - platformMinY) < (charMaxX - platformMinX)) ||
                         (charMaxX >= platformMaxX && charMinX <= platformMaxX && (charMaxY - platformMinY) < (platformMaxX - charMinX)))
                         )
                {
                    //ceiling
                    m_platformCollision.bHitCeiling = true;
                    m_platformCollision.bStickyCeiling = true;
                    m_platformPrevPos = platformPos;
                    m_vPlatformRevertMove = VECTOR3FIXED(0, platformMinY - charMaxY, 0);
                    m_bOnPlatform = true;
                    m_pPlatformNode = pPlatform->GetNode();
                    break;
                }
                else if (!m_platformCollision.bHitCeiling && !m_platformCollision.bOnFloor && !m_platformCollision.bGrabbedWallFromRight &&
                         (charMaxX >= platformMinX && charMinX <= platformMinX) && (charMinY <= platformMaxY && charMaxY >= platformMinY))
                {
                    // Hitfromleft
                    m_platformCollision.bGrabbedWallFromLeft = true;
                    m_platformPrevPos = platformPos;
                    m_vPlatformRevertMove = VECTOR3FIXED(platformMinX - charMaxX, 0, 0);
                    m_bOnPlatform = true;
                    m_pPlatformNode = pPlatform->GetNode();
                    break;
                }
                else if (!m_platformCollision.bHitCeiling && !m_platformCollision.bGrabbedWallFromLeft && !m_platformCollision.bOnFloor &&
                         (charMaxX >= platformMaxX && charMinX <= platformMaxX) && (charMinY <= platformMaxY && charMaxY >= platformMinY))
                {
                    // hit from right
                    m_platformCollision.bGrabbedWallFromRight = true;
                    m_platformPrevPos = platformPos;
                    m_vPlatformRevertMove = VECTOR3FIXED(platformMaxX - charMinX, 0, 0);
                    m_bOnPlatform = true;
                    m_pPlatformNode = pPlatform->GetNode();
                    break;
                }
                else
                {
                    m_pPlatformNode = NULL;
                    m_platformCollision.bHitCeiling
                        = m_platformCollision.bGrabbedWallFromLeft
                        = m_platformCollision.bGrabbedWallFromRight
                        = m_platformCollision.bOnFloor = false;
                }


            }
        }

        // Update collision flags, if player is on a platform
        //
        if (m_bOnPlatform)
        {
            m_collisionState.bGrabbedWallFromLeft = m_platformCollision.bGrabbedWallFromLeft;
            m_collisionState.bGrabbedWallFromRight = m_platformCollision.bGrabbedWallFromRight;
            m_collisionState.bOnFloor = m_platformCollision.bOnFloor;
            m_collisionState.bHitCeiling = m_platformCollision.bHitCeiling;
            m_collisionState.bStickyCeiling = m_platformCollision.bStickyCeiling;

            if (pPlatform)
            {
                pPlatform->OnPlayerOnPlatform();
            }
        }

    }



    void CCharacter2DEntity::DumpCollisionInfo()
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

    void CCharacter2DEntity::UpdateSounds()
    {
        m_characterSounds[CHARACTER_SOUND_MOVE]->SetVolume(3.5f);
        //m_characterSounds[CHAR]
        if (m_collisionState.bOnFloor && abs(m_vVelocity.x) > 10)
        {
            if (m_bSecondaryKeyPressed)
                m_characterSounds[CHARACTER_SOUND_MOVE]->SetSpeed(3.0f);
            else
                m_characterSounds[CHARACTER_SOUND_MOVE]->SetSpeed(2.0f);

            if (!m_characterSounds[CHARACTER_SOUND_MOVE]->IsPlaying())
                m_characterSounds[CHARACTER_SOUND_MOVE]->PlaySound(-1);
        }
        else
            m_characterSounds[CHARACTER_SOUND_MOVE]->StopSound();


        m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->SetSpeed(1.0f);

        if (m_collisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight || (m_collisionState.bStickyCeiling && m_collisionState.bHitCeiling))
        {
            if (!m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->IsPlaying())
                m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->PlaySound(-1);
        }
        else
            m_characterSounds[CHARACTER_SOUND_SLIDEWALL]->StopSound();


        if ((!m_prevCollisionState.bOnFloor && m_collisionState.bOnFloor) ||
            !((m_prevCollisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight) && (m_prevCollisionState.bGrabbedWallFromLeft || m_collisionState.bGrabbedWallFromRight)))
        {
            if (!m_characterSounds[CHARACTER_SOUND_HITFLOOR]->IsPlaying())
                m_characterSounds[CHARACTER_SOUND_HITFLOOR]->PlaySound(0);
        }
        else
        {
            //m_characterSounds[CHARACTER_SOUND_HITFLOOR]->StopSound();
        }


        if (m_bFirstJumpFrame)
        {
            m_characterSounds[CHARACTER_SOUND_SECONDJUMP]->SetVolume(0.75f);
            if (m_bSecondJump)
                m_characterSounds[CHARACTER_SOUND_SECONDJUMP]->PlaySound();
            else
                m_characterSounds[CHARACTER_SOUND_JUMP]->PlaySound();
        }
    }

    void CCharacter2DEntity::UpdateParticleSystems(float dt)
    {
        Vec3 p = m_pNode->GetDerivedPosition();

        Vec2 vXAxis(1.0f, 0.0f);
        Vec2 vMagnetDir(glm::normalize(m_vVelocity));
        float angle = acos(glm::dot(vMagnetDir, vXAxis));
        if (vMagnetDir.y < 0.00f)
            angle = -angle;

        float ve = glm::length(m_vVelocity);


        const CParticleSystemEntity::mapEmitters& emitters = m_pParticlesTrace->GetEmitters();
        for (CParticleSystemEntity::mapEmitters::const_iterator it = emitters.begin(); it != emitters.end(); ++it)
        {
            if ((m_collisionState.bOnFloor || (m_collisionState.bHitCeiling && !m_bOnPlatform)) && ve < 150)
            {
                it->second->SetEmitRate(30.0f);
            }
            else
            {
                it->second->SetEmitRate(50.0f);
            }
        }

        if (m_bOnMagnet || m_bOnPlatform)
        {
            m_pParticlesTrace->SetEmitNewParticles(false);
        }
        else if ((m_collisionState.bOnFloor && !m_bOnPlatform) || (m_collisionState.bHitCeiling && !m_bOnPlatform))
        {
            m_pParticlesTrace->SetEmitNewParticles(true);
            m_pParticlesTrace->GetNode()->SetPosition(Vec3(p.x, p.y - 20.0f, -5.0f));
            m_pParticlesTrace->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));
        }
        else if (m_collisionState.bGrabbedWallFromLeft)
        {
            m_pParticlesTrace->SetEmitNewParticles(true);
            m_pParticlesTrace->GetNode()->SetPosition(Vec3(p.x + 8.0f, p.y, -5.0f));
            m_pParticlesTrace->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, PI * 0.5f));
        }
        else if (m_collisionState.bGrabbedWallFromRight)
        {
            m_pParticlesTrace->SetEmitNewParticles(true);
            m_pParticlesTrace->GetNode()->SetPosition(Vec3(p.x - 8.0f, p.y, -5.0f));
            m_pParticlesTrace->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, -PI * 0.5f));
        }
        else
        {

            //m_pParticlesFloorTrace->SetEmitNewParticles(false);
            if (ve > 350)
            {
                m_pParticlesTrace->SetEmitNewParticles(true);
                m_pParticlesTrace->GetNode()->SetPosition(Vec3(p.x, p.y, -10.0f));
                m_pParticlesTrace->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, angle));
            }
            else
                m_pParticlesTrace->SetEmitNewParticles(false);
        }


        if (m_collisionState.bHitCeiling && !m_bOnPlatform)
        {
            m_pParticlesTrace->SetEmitNewParticles(true);
            m_pParticlesTrace->GetNode()->SetPosition(Vec3(p.x + 12.0f, p.y + 20.0f, -5.0f));
        }


    }

    void CCharacter2DEntity::UpdateColors(float dt)
    {
        float speed = glm::length(m_vVelocity);

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

        m_spriteCharBody.SetColor(m_characterPlayerColor);
        m_spriteCharArm01.SetColor(m_characterPlayerColor);
        m_spriteCharArm02.SetColor(m_characterPlayerColor);
        m_spriteCharChest.SetColor(m_characterPlayerColor);
        m_spriteCharLeg01.SetColor(m_characterPlayerColor);
        m_spriteCharLeg02.SetColor(m_characterPlayerColor);
        m_spriteCharHead.SetColor(m_characterPlayerColor);
        m_spriteCharChest.SetColor(m_characterPlayerColor);

        /*static float redTime = 0.0f;
        if(m_bSecondJump && m_bJumpingUp && m_jumpTimer.GetTime() < (m_jumpTime * 0.5f))
        {
        m_spriteCharBody.SetColor(m_characterDoubleJumpColor);
        m_spriteCharArm01.SetColor(m_characterDoubleJumpColor);
        m_spriteCharArm02.SetColor(m_characterDoubleJumpColor);
        m_spriteCharChest.SetColor(m_characterDoubleJumpColor);
        m_spriteCharLeg01.SetColor(m_characterDoubleJumpColor);
        m_spriteCharLeg02.SetColor(m_characterDoubleJumpColor);
        m_spriteCharHead.SetColor(m_characterDoubleJumpColor);
        m_spriteCharChest.SetColor(m_characterDoubleJumpColor);
        redTime = 0.4f;
        }
        else if(redTime >= 0.0f)
        {
        redTime -= dt;
        float alpha = min(1.0f - redTime / 0.4f, 1.0f);

        m_spriteCharBody.SetColor(m_characterDoubleJumpColor * (1.0f - alpha) + m_characterPlayerColor * alpha);
        m_spriteCharArm01.SetColor(m_characterDoubleJumpColor * (1.0f - alpha) + m_characterPlayerColor * alpha);
        m_spriteCharArm02.SetColor(m_characterDoubleJumpColor * (1.0f - alpha) + m_characterPlayerColor * alpha);
        m_spriteCharChest.SetColor(m_characterDoubleJumpColor * (1.0f - alpha) + m_characterPlayerColor * alpha);
        m_spriteCharLeg01.SetColor(m_characterDoubleJumpColor * (1.0f - alpha) + m_characterPlayerColor * alpha);
        m_spriteCharLeg02.SetColor(m_characterDoubleJumpColor * (1.0f - alpha) + m_characterPlayerColor * alpha);
        m_spriteCharHead.SetColor(m_characterDoubleJumpColor * (1.0f - alpha) + m_characterPlayerColor * alpha);
        m_spriteCharChest.SetColor(m_characterDoubleJumpColor * (1.0f - alpha) + m_characterPlayerColor * alpha);
        }*/
    }


    void CCharacter2DEntity::AddToRenderQueue(float dt, RenderQueue& queue)
    {
        ENV_SCOPED_CPU_PROFILING(Entity, CCharacter2DEntity::AddToRenderQueue);

        CEntity::AddToRenderQueue(dt, queue);

        UpdateColors(dt);

        float velo = glm::length(m_vVelocity);

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


        bool bSwitch = m_bKeyRightPressed || m_bKeyLeftPressed;

        CollisionState colRes = m_collisionState;

        if (colRes.bGrabbedWallFromLeft)
        {
            bSwitch = true;
            m_bWatchingRight = true;
        }
        else if (colRes.bGrabbedWallFromRight)
        {
            bSwitch = true;
            m_bWatchingRight = false;
        }
        else if (m_bJumpedFromWallFromLeft && m_bJumpingUp)
        {
            if (!m_bKeyRightPressed)
                m_bWatchingRight = false;
            else
                m_bWatchingRight = true;
            bSwitch = true;
        }
        else if (m_bJumpedFromWallFromRight && m_bJumpingUp)
        {
            if (!m_bKeyLeftPressed)
                m_bWatchingRight = true;
            else
                m_bWatchingRight = false;
            bSwitch = true;
        }
        else if (m_bKeyRightPressed)
            m_bWatchingRight = true;
        else if (m_bKeyLeftPressed)
            m_bWatchingRight = false;

        CQuaternion rootOrientation = m_pNode->GetOrientation();
        if (m_bWatchingRight && bSwitch)
        {
            rootOrientation.SetYawPitchRoll(0.0f, 0.0f, 0.0f);
        }
        else if (bSwitch)
        {
            rootOrientation.SetYawPitchRoll(PI, 0.0f, 0.0f);
        }
        m_pNode->SetOrientation(rootOrientation);


        CQuaternion arm01Orientation = m_pCharacterArm01Node->GetOrientation();
        arm01Orientation.SetYawPitchRoll(0.0f, 0.0f, 0.0f);
        m_pCharacterArm01Node->SetOrientation(arm01Orientation);

        CQuaternion arm02Orientation = m_pCharacterArm02Node->GetOrientation();
        arm02Orientation.SetYawPitchRoll(0.0f, 0.0f, 0.0f);
        m_pCharacterArm02Node->SetOrientation(arm02Orientation);

        if (!m_bOnMagnet && (m_acceleratorTime < 0.0f || Equals(m_acceleratorTime, 0.0f)))
        {
            m_pCharacterBodyNode->SetOrientation(CQuaternion());
        }

        m_pCharacterBodyNode->SetPosition(Vec3(0.0f, -15.0f, 0.0f));

        if (m_bFirstJumpFrame && !m_bSecondJump)
        {
            m_pCharacterLeg01Node->SetOrientation(CQuaternion());
            m_pCharacterLeg02Node->SetOrientation(CQuaternion());
        }


        float zFactor = 1.0f;
        if (!m_bWatchingRight)
            zFactor = -1.0f;


        float armHeight = 22.0f;
        float armDepth = 26.0f;
        float chestHeight = 8.0f;
        float headHeight = 20.0f;
        float legHeight = 7.0f;

        if (!m_bFrozen)
        {
            if (colRes.bOnFloor || colRes.bOnRamp)
            {
                if (colRes.bHitCeiling)
                    zFactor = -zFactor;

                float factor = 1.0f;
                if (m_bSecondaryKeyPressed)
                    factor = m_playerSuperSpeed;


                CQuaternion bodyOrientation = m_pCharacterBodyNode->GetOrientation();
                if (abs(m_vVelocity.x) > 10.0f)
                {
                    if (m_bSecondaryKeyPressed)
                        bodyOrientation.SetRoll(-0.2f);
                    else
                        bodyOrientation.SetRoll(-0.1f);
                }
                else
                {
                    bodyOrientation.SetRoll(0.0f);
                }
                m_pCharacterBodyNode->SetOrientation(bodyOrientation);


                if (abs(m_vVelocity.x) > 10)
                {
                    m_chestBreathe += dt * 24;
                    m_pCharacterChestNode->SetOrientation(CQuaternion(0.0f, 0.0f, -0.15f));
                    m_pCharacterChestNode->SetPosition(Vec3(-2.0f, chestHeight + sin(m_chestBreathe), -1.0f * zFactor));
                }
                else
                {
                    m_chestBreathe += dt * 4;
                    m_pCharacterChestNode->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));
                    m_pCharacterChestNode->SetPosition(Vec3(0.0f, chestHeight + sin(m_chestBreathe), -1.0f * zFactor));
                }



                if (m_armRotation > 0.85f)
                {
                    m_bArmRightRotation = !m_bArmRightRotation;
                    m_armRotation = 0.85f;
                }
                else if (m_armRotation < -0.85f)
                {
                    m_bArmRightRotation = !m_bArmRightRotation;
                    m_armRotation = -0.85f;
                }

                if (abs(m_vVelocity.x) > 10.0f)
                {
                    if (m_bArmRightRotation)
                        m_armRotation += 7 * factor * dt;
                    else
                        m_armRotation -= 7 * factor * dt;
                }
                else
                    m_armRotation = 0.0f;

                float armXRot = 0.0f;
                if (abs(m_vVelocity.x) > 10)
                    armXRot = 0.85f;

                float rotationFactor = 1.0;
                if (m_bSecondaryKeyPressed)
                    rotationFactor = 1.0f;

                if (m_legRotation > (0.85f * rotationFactor))
                {
                    m_bLegRightRotation = !m_bLegRightRotation;
                    m_legRotation = (0.85f * rotationFactor);
                }
                else if (m_legRotation < (-0.85f * rotationFactor))
                {
                    m_bLegRightRotation = !m_bLegRightRotation;
                    m_legRotation = (-0.85f * rotationFactor);
                }

                float runFactor = 1.05f;
                if (m_bSecondaryKeyPressed)
                    runFactor = 1.32f;
                if (abs(m_vVelocity.x) > 10.0f)
                {
                    if (m_bLegRightRotation)
                        m_legRotation += 7.5 * runFactor * dt;
                    else
                        m_legRotation -= 7.5 * runFactor * dt;
                }
                else
                    m_legRotation = 0.0f;

                float rotLeg01 = m_legRotation;
                float rotLeg02 = -m_legRotation;

                m_pCharacterArm01Node->SetPosition(Vec3(0.0f, armHeight, -(armDepth + 2.0f)));
                CQuaternion arm01Orientation = m_pCharacterArm01Node->GetOrientation();
                arm01Orientation.SetPitch(armXRot);
                arm01Orientation.SetRoll(m_armRotation);
                m_pCharacterArm01Node->SetOrientation(arm01Orientation);

                m_pCharacterArm02Node->SetPosition(Vec3(0.0f, armHeight, armDepth));
                CQuaternion arm02Orientation = m_pCharacterArm02Node->GetOrientation();
                arm02Orientation.SetPitch(-armXRot);
                arm02Orientation.SetRoll(-m_armRotation);
                m_pCharacterArm02Node->SetOrientation(arm02Orientation);

                if (abs(m_vVelocity.x) > 10)
                {
                    m_pCharacterHeadNode->SetPosition(Vec3(0.0f, headHeight, 2.0f * zFactor));
                    CQuaternion headOrientation = m_pCharacterHeadNode->GetOrientation();
                    headOrientation.SetRoll(0.0f);
                    m_pCharacterHeadNode->SetOrientation(headOrientation);
                }
                else
                {
                    m_pCharacterHeadNode->SetPosition(Vec3(0.0f, headHeight + 1.0f, 2.0f * zFactor));
                    CQuaternion headOrientation = m_pCharacterHeadNode->GetOrientation();
                    headOrientation.SetRoll(0.0f);
                    m_pCharacterHeadNode->SetOrientation(headOrientation);
                }

                if ((m_vVelocity.x > 0.0 && m_bKeyLeftPressed) || (m_vVelocity.x < 0.0f && m_bKeyRightPressed))
                {
                    rotLeg01 = 0.75f;
                    rotLeg02 = -0.15f;
                    m_pCharacterBodyNode->SetOrientation(CQuaternion(0.0f, 0, -0.6f));
                    m_pCharacterHeadNode->SetOrientation(CQuaternion(0.0f, 0.0f, 0.5f));
                }


                m_pCharacterLeg01Node->SetPosition(Vec3(-3.0f, legHeight, 1.0f * zFactor));
                CQuaternion leg01Orientation = m_pCharacterLeg01Node->GetOrientation();
                leg01Orientation.SetRoll(rotLeg01);
                m_pCharacterLeg01Node->SetOrientation(leg01Orientation);

                m_pCharacterLeg02Node->SetPosition(Vec3(-3.0f, legHeight, 2.0f * zFactor));
                CQuaternion leg02Orientation = m_pCharacterLeg02Node->GetOrientation();
                leg02Orientation.SetRoll(rotLeg02);
                m_pCharacterLeg02Node->SetOrientation(leg02Orientation);
            }
            else if (!m_bMovementWallGrabDisabled && (colRes.bGrabbedWallFromLeft || colRes.bGrabbedWallFromRight))
            {
                CQuaternion bodyOrientation = m_pCharacterBodyNode->GetOrientation();
                bodyOrientation.SetRoll(0.2f);
                m_pCharacterBodyNode->SetOrientation(bodyOrientation);

                if (abs(m_vVelocity.x) > 10)
                {
                    m_chestBreathe += dt * 24;
                }
                else
                    m_chestBreathe += dt * 4;

                m_pCharacterChestNode->SetPosition(Vec3(0.0f, chestHeight + sin(m_chestBreathe), -1.0f * zFactor));
                m_pCharacterChestNode->SetOrientation(CQuaternion());

                m_pCharacterArm01Node->SetPosition(Vec3(-2.0f, armHeight, -(armDepth + 2.0f)));
                CQuaternion arm01Orientation = m_pCharacterArm01Node->GetOrientation();
                arm01Orientation.SetYawPitchRoll(0.0f, 0.0f, 1.5f);
                m_pCharacterArm01Node->SetOrientation(arm01Orientation);

                m_pCharacterArm02Node->SetPosition(Vec3(-2.0f, armHeight, armDepth));
                CQuaternion arm02Orientation = m_pCharacterArm02Node->GetOrientation();
                arm02Orientation.SetYawPitchRoll(0.0f, 0.0f, 1.5f);
                m_pCharacterArm02Node->SetOrientation(arm02Orientation);

                m_pCharacterHeadNode->SetPosition(Vec3(0.0f, headHeight, 1.0f * zFactor));
                CQuaternion headOrientation = m_pCharacterHeadNode->GetOrientation();
                headOrientation.SetRoll(0.1f);
                m_pCharacterHeadNode->SetOrientation(headOrientation);

                m_pCharacterLeg01Node->SetPosition(Vec3(-3.0f, legHeight, 1.0f * zFactor));
                CQuaternion leg01Orientation = m_pCharacterLeg01Node->GetOrientation();
                leg01Orientation.SetRoll(0.5f);
                m_pCharacterLeg01Node->SetOrientation(leg01Orientation);

                m_pCharacterLeg02Node->SetPosition(Vec3(-3.0f, legHeight, 2.0f * zFactor));
                CQuaternion leg02Orientation = m_pCharacterLeg02Node->GetOrientation();
                leg02Orientation.SetRoll(1.0f);
                m_pCharacterLeg02Node->SetOrientation(leg02Orientation);
            }
            else if ((colRes.bHitCeiling && colRes.bStickyCeiling))
            {
                m_legRotation = 0.0f;

                float factor = 1.0f;
                if (m_bSecondaryKeyPressed)
                    factor = m_playerSuperSpeed;


                CQuaternion bodyOrientation = m_pCharacterBodyNode->GetOrientation();
                if (abs(m_vVelocity.x) > 10.0f)
                {
                    if (m_bSecondaryKeyPressed)
                        bodyOrientation.SetRoll(0.2f);
                    else
                        bodyOrientation.SetRoll(0.1f);
                }
                else
                {
                    bodyOrientation.SetRoll(0.0f);
                }
                m_pCharacterBodyNode->SetOrientation(bodyOrientation);


                if (abs(m_vVelocity.x) > 10)
                {
                    m_pCharacterChestNode->SetOrientation(CQuaternion(0.0f, 0.0f, -0.3f));
                }
                else
                {
                    m_pCharacterChestNode->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));
                }

                m_pCharacterChestNode->SetPosition(Vec3(0.0f, chestHeight, -1.0f * zFactor));


                if (abs(m_vVelocity.x) > 10.0f)
                    m_legRotation = 0.5f;
                else
                    m_legRotation = 0.0f;

                float rotLeg01 = m_legRotation;
                float rotLeg02 = m_legRotation * 0.5f;


                m_pCharacterArm01Node->SetPosition(Vec3(0.0f, armHeight, -(armDepth + 2.0f)));
                CQuaternion arm01Orientation = m_pCharacterArm01Node->GetOrientation();
                arm01Orientation.SetYawPitchRoll(0.0f, 3.05f, 0.0f);
                m_pCharacterArm01Node->SetOrientation(arm01Orientation);

                m_pCharacterArm02Node->SetPosition(Vec3(0.0f, armHeight, armDepth));
                CQuaternion arm02Orientation = m_pCharacterArm02Node->GetOrientation();
                arm02Orientation.SetYawPitchRoll(0.0f, -3.05f, 0.0f);
                m_pCharacterArm02Node->SetOrientation(arm02Orientation);


                if (abs(m_vVelocity.x) > 10)
                {
                    m_pCharacterHeadNode->SetPosition(Vec3(0.0f, headHeight, 1.0f * zFactor));
                    CQuaternion headOrientation = m_pCharacterHeadNode->GetOrientation();
                    headOrientation.SetRoll(0.3f);
                    m_pCharacterHeadNode->SetOrientation(headOrientation);
                }
                else
                {
                    m_pCharacterHeadNode->SetPosition(Vec3(0.0f, headHeight, 1.0f * zFactor));
                    CQuaternion headOrientation = m_pCharacterHeadNode->GetOrientation();
                    headOrientation.SetRoll(0.1f);
                    m_pCharacterHeadNode->SetOrientation(headOrientation);
                }

                if ((m_vVelocity.x > 0.0 && m_bKeyLeftPressed) || (m_vVelocity.x < 0.0f && m_bKeyRightPressed))
                {
                    rotLeg01 = -0.4f;
                    rotLeg02 = -0.15f;
                    m_pCharacterBodyNode->SetOrientation(CQuaternion(0.0f, 0, 0.4f));
                    m_pCharacterHeadNode->SetOrientation(CQuaternion(0.0f, 0.0f, 0.5f));
                }

                m_pCharacterLeg01Node->SetPosition(Vec3(0.0f, legHeight, 1.0f * zFactor));
                CQuaternion leg01Orientation = m_pCharacterLeg01Node->GetOrientation();
                leg01Orientation.SetRoll(rotLeg01);
                m_pCharacterLeg01Node->SetOrientation(leg01Orientation);

                m_pCharacterLeg02Node->SetPosition(Vec3(0.0f, legHeight, 2.0f * zFactor));
                CQuaternion leg02Orientation = m_pCharacterLeg02Node->GetOrientation();
                leg02Orientation.SetRoll(rotLeg02);
                m_pCharacterLeg02Node->SetOrientation(leg02Orientation);
            }
            else
            {
                //m_pCharacterBodyNode->SetOrientationm(CQuaternion());
                m_pCharacterChestNode->SetPosition(Vec3(-2.0f, chestHeight, -1.0f * zFactor));

                float x = (abs(m_vVelocity.x) / (m_playerMaxSpeedX));

                if (m_bJumping)
                    x = std::min(1.0f, m_jumpTimer.GetTime() / m_jumpTime);

                m_pCharacterArm01Node->SetPosition(Vec3(0.0f, armHeight + 2.0f, -(armDepth + 2.0f)));
                CQuaternion arm01Orientation = m_pCharacterArm01Node->GetOrientation();
                arm01Orientation.SetYawPitchRoll(0.0f, (1 - x) * 0.8f, x * 1.2f * zFactor);
                m_pCharacterArm01Node->SetOrientation(arm01Orientation);

                m_pCharacterArm02Node->SetPosition(Vec3(0.0f, armHeight + 2.0f, armDepth));
                CQuaternion arm02Orientation = m_pCharacterArm02Node->GetOrientation();
                arm02Orientation.SetYawPitchRoll(0.0f, (1 - x) * -0.8f, x * -1.2f * zFactor);
                m_pCharacterArm02Node->SetOrientation(arm02Orientation);

                m_pCharacterHeadNode->SetPosition(Vec3(0.0f, headHeight, 1.0f * zFactor));
                CQuaternion headOrientation = m_pCharacterHeadNode->GetOrientation();
                headOrientation.SetRoll(-0.15f);
                m_pCharacterHeadNode->SetOrientation(headOrientation);

                m_pCharacterLeg01Node->SetPosition(Vec3(-3.0f, legHeight, 1.0f * zFactor));
                CQuaternion leg01Orientation = m_pCharacterLeg01Node->GetOrientation();

                m_pCharacterLeg02Node->SetPosition(Vec3(-3.0f, legHeight, 2.0f * zFactor));
                CQuaternion leg02Orientation = m_pCharacterLeg02Node->GetOrientation();

                if (m_bOnMagnet)
                {
                    m_pCharacterLeg01Node->SetOrientation(CQuaternion());
                    m_pCharacterLeg02Node->SetOrientation(CQuaternion());
                }
                else
                {
                    float leg01 = 0.8f, leg01Start = 0.4f, leg02 = -0.4f, leg02Start = -0.3f;

                    float veloAlpha = abs(m_vVelocity.x);
                    veloAlpha /= m_playerMaxSpeedX;

                    leg01 = (1.0f - veloAlpha) * leg01Start + veloAlpha * leg01;
                    leg02 = (1.0f - veloAlpha) * leg02Start + veloAlpha * leg02;


                    if (m_jumpTimer.GetTime() < m_jumpTime * 0.4f)
                    {

                        float alpha = m_jumpTimer.GetTime() / (m_jumpTime * 0.4f);

                        leg01Orientation.SetRoll(leg01 * alpha);
                        m_pCharacterLeg01Node->SetOrientation(leg01Orientation);

                        leg02Orientation.SetRoll(leg02 * alpha);
                        m_pCharacterLeg02Node->SetOrientation(leg02Orientation);
                    }
                    else// if( !m_bSecondJump)
                    {
                        //float alpha = (m_jumpTimer.GetTime() - m_jumpTime * 0.1f) / (m_jumpTime * 0.4f);

                        leg01Orientation.SetRoll(leg01);
                        m_pCharacterLeg01Node->SetOrientation(leg01Orientation);

                        leg02Orientation.SetRoll(leg02);
                        m_pCharacterLeg02Node->SetOrientation(leg02Orientation);
                    }
                }

            }

        }

        if (m_jumpTimer.GetTime() <= 0.3f && m_bSecondJump)
        {
            float alpha = m_jumpTimer.GetTime() / 0.3f;

            gEngine->GetWorldMgr()->GetScenario()->SetColorBrightness(Vec3(0.0f, 0.0f, alpha * 0.1f));
        }
        else
        {
            float offset = gEngine->GetWorldMgr()->GetScenario()->GetColorBrightness().z;
            offset += (-offset) * dt * 7.0f;

            gEngine->GetWorldMgr()->GetScenario()->SetColorBrightness(Vec3(0.0f, 0.0f, offset));
        }


        static float timeReshape = -1.0f;

        static bool bShaped = false;

        m_timeShape -= dt;
        timeReshape -= dt;

        if ((!m_bSecondJump || !m_bJumping) && !m_bInForceField && !m_bOnMagnet && m_currentAcceleratorTime <= 0.0f)
        {
            m_pParticlesSuper->SetEmitNewParticles(false);
            m_pParticlesSuper->SetRenderParticles(false);

            SetVisible(true);

            if (bShaped)
            {
                if (timeReshape >= 0.0f)
                {
                    float alpha = 1.0f - timeReshape / CHAR_SHAPE_TIME;
                    m_pNode->SetScale((1.0f - alpha) * Vec3(0.2f, 0.2f, 0.2f) + alpha * Vec3(0.37f, 0.37f, 0.37f));

                    m_pParticlesSuper->GetNode()->SetScale((1.0f - alpha) * Vec3(1, 1, 1));
                }
                else
                {
                    m_pNode->SetScale(Vec3(0.37f, 0.37f, 0.37f));
                    bShaped = false;
                }

            }
            else
            {
                m_pNode->SetScale(Vec3(0.37f, 0.37f, 0.37f));
            }

            m_charChestRoll = m_pCharacterChestNode->GetOrientation().GetRoll();
            m_charHeadRoll = m_pCharacterHeadNode->GetOrientation().GetRoll();
            m_charLeg01Roll = m_pCharacterLeg01Node->GetOrientation().GetRoll();
            m_charLeg02Roll = m_pCharacterLeg02Node->GetOrientation().GetRoll();
        }
        else
        {
            if (m_timeShape >= 0.0f)
            {
                float alpha = m_timeShape / CHAR_SHAPE_TIME;

               // float alpha2 = std::pow(m_timeShape / CHAR_SHAPE_TIME, 2.0f);
                m_pNode->SetScale(Vec3(0.37f, 0.37f, 0.37f) * alpha + (1.0f - alpha) * Vec3(0.0f, 0.0f, 0.0f));

                m_pParticlesSuper->GetNode()->SetScale(alpha * Vec3(0.5f, 0.5f, 0.5f) + (1.0f - alpha) * Vec3(1, 1, 1));

                CQuaternion q = m_pCharacterBodyNode->GetOrientation();
                q.SetRoll((1.0f - alpha) * -1.3f);
                m_pCharacterBodyNode->SetOrientation(q);

                //alpha = 0.0f;
                m_pCharacterChestNode->SetOrientation(CQuaternion(0.0f, 0.0f, alpha * m_charChestRoll + (1.0f - alpha) * -0.8f));
                m_pCharacterHeadNode->SetOrientation(CQuaternion(0.0f, 0.0f, alpha * m_charHeadRoll + (1.0f - alpha) * -0.3f));
                m_pCharacterLeg01Node->SetOrientation(CQuaternion(0.0f, 0.0f, alpha * m_charLeg01Roll + (1.0f - alpha) * 1.0f));
                m_pCharacterLeg02Node->SetOrientation(CQuaternion(0.0f, 0.0f, alpha * m_charLeg02Roll + (1.0f - alpha) * 0.9f));
            }
            else
            {
                m_pNode->SetScale(Vec3(0.05f, 0.05f, 0.05f));

                m_pParticlesSuper->GetNode()->SetScale(Vec3(1, 1, 1));

                SetVisible(false);

                bShaped = true;
            }





            if (bShaped)
            {
                CParticleSystemEntity::mapEmitters& emitters = m_pParticlesSuper->GetEmitters();
                for (CParticleSystemEntity::mapEmitters::iterator it = emitters.begin(); it != emitters.end(); ++it)
                {
                    if (m_vVelocity.x >= 0.0f)
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
            Vec2 vMagnetDir(glm::normalize(m_vVelocity));
            float angle = acos(glm::dot(vMagnetDir, vXAxis));
            if (vMagnetDir.y < 0.00f)
                angle = -angle;

            m_pParticlesSuper->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, 0.0f));


            m_pParticlesSuper->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, angle));
            m_pParticlesSuper->GetNode()->SetPosition(Vec3(p.x, p.y, -10.0f));

            //if(m_timeShape < (CHAR_SHAPE_TIME * 0.5f))
            m_pParticlesSuper->SetEmitNewParticles(true);
            m_pParticlesSuper->SetRenderParticles(true);

        }


        if (m_bVisible)
        {
            if (!queue.m_pCharacterRenderObject)
                queue.m_pCharacterRenderObject = &m_characterRenderObject;

            GetSprites(queue.m_pCharacterRenderObject->m_sprites);
        }

        m_bInForceField = false;
        m_pForceField = NULL;


        m_bKeyRightPressed = false;
        m_bKeyLeftPressed = false;
    }


    void CCharacter2DEntity::GetSprites(listSpriteRenderObjects& sprites)
    {
        sprites.clear();
        if (m_bWatchingRight)
        {
            sprites.push_back(m_spriteCharArm02.GetRenderObject());
            sprites.push_back(m_spriteCharLeg02.GetRenderObject());
            sprites.push_back(m_spriteCharLeg01.GetRenderObject());
            sprites.push_back(m_spriteCharHead.GetRenderObject());
            sprites.push_back(m_spriteCharBody.GetRenderObject());
            sprites.push_back(m_spriteCharChest.GetRenderObject());
            sprites.push_back(m_spriteCharArm01.GetRenderObject());
        }
        else
        {
            sprites.push_back(m_spriteCharArm01.GetRenderObject());
            sprites.push_back(m_spriteCharLeg02.GetRenderObject());
            sprites.push_back(m_spriteCharLeg01.GetRenderObject());
            sprites.push_back(m_spriteCharHead.GetRenderObject());
            sprites.push_back(m_spriteCharBody.GetRenderObject());
            sprites.push_back(m_spriteCharChest.GetRenderObject());
            sprites.push_back(m_spriteCharArm02.GetRenderObject());
        }



    }

    void CCharacter2DEntity::OnInput(unsigned int eventId, const EventParameter& eventParameter)
    {
        switch (eventId)
        {
        case CHARACTER_EVENT_FLIP_INPUT:
            m_bMovementFlipInput = true;
            break;
        case CHARACTER_EVENT_ENABLE_MOVEMENT:
            m_bMovementDisabled = false;
            // 			m_bMovementSprintDisabled = false;
            // 			m_bMovementJumpDisabled = false;
            // 			m_bMovementDoubleJumpDisabled = false;
            // 			m_bMovementWallGrabDisabled = false;
            // 			m_bMovementWallJumpDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_MOVEMENT:
            m_bMovementDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_SPRINT:
            m_bMovementSprintDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_SPRINT:
            m_bMovementSprintDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_JUMP:
            m_bMovementJumpDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_JUMP:
            m_bMovementJumpDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_DOUBLEJUMP:
            m_bMovementDoubleJumpDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_DOUBLEJUMP:
            m_bMovementDoubleJumpDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_WALLGRAB:
            m_bMovementWallGrabDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_WALLGRAB:
            m_bMovementWallGrabDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_WALLJUMP:
            m_bMovementWallJumpDisabled = false;
            break;

        case CHARACTER_EVENT_DISABLE_WALLJUMP:
            m_bMovementWallJumpDisabled = true;
            break;

        case CHARACTER_EVENT_ENABLE_FREEZE:
            m_bFrozen = true;
            break;

        case CHARACTER_EVENT_DISABLE_FREEZE:
            m_bFrozen = false;
            break;

        case CHARACTER_EVENT_STARTPLAYBACK:
            break;
        default:
            CEntity::OnInput(eventId, eventParameter);
        }
    }

    void CCharacter2DEntity::SetLight(bool bSet)
    {
        if (!bSet && m_pLight)
        {
            gEngine->GetEntityMgr()->RemoveEntity(m_pLight->GetName());
            m_pLight = NULL;
        }

        if (bSet && !m_pLight)
        {
            m_pLight = static_cast<CLightEntity*>(gEngine->GetEntityMgr()->CreateEntity(CLightEntity::ENTITY_META_TYPEID));
            m_pLight->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -50.0f));
            m_pLight->SetSprite(false);
            m_pLight->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->GetMaterial().SetRenderInGame(false); });
        }

    }

    void CCharacter2DEntity::SetKilled(bool bKilled)
    {
//         if (!m_bKilled && bKilled)
//         {
//             m_pParticlesDeath->GetNode()->SetPosition(m_pNode->GetDerivedPosition() + VECTOR3(0.0f, 0.0f, -10.0f));
//             m_pParticlesDeath->EmitAllParticlesOnce();
//             m_pParticlesDeath->SetRenderParticles(true);
// 
//             m_pParticlesTrace->SetEmitNewParticles(false);
//             //m_pParticlesDoubleJump->SetEmitNewParticles(false);
// 
//             m_pNode->SetScale(VECTOR3(0.0f, 0.0f, 0.0f));
// 
//             m_characterSounds[CHARACTER_SOUND_HITFLOOR]->PlaySound(0);
//             m_characterSounds[CHARACTER_SOUND_HITFLOOR]->SetVolume(1.4f);
//         }
// 
//         m_bKilled = bKilled;
// 
//         if (m_bKilled)
//         {
//             CSingletonPlayerMgr playerMgr;
// 
//             if (playerMgr->GetNumLevels(playerMgr->GetCurrentChapter()) > playerMgr->GetCurrentLevel())
//             {
//                 LevelInfo& levelInfo = playerMgr->GetLevelinfo(playerMgr->GetCurrentChapter(), playerMgr->GetCurrentLevel());
//                 ++levelInfo.numKills;
//             }
//             
//         }
    }



    // 	void CCharacterEntity::SetMovementBlocked(bool bBlocked)
    // 	{
    // 		m_bMovementBlocked = bBlocked; 			
    // 		m_bJumpingUp = false;
    // 		m_collision.bOnFloor = true;
    // 		m_vVelocity = VECTOR2(0.0f, 0.0f);
    // 	}

}