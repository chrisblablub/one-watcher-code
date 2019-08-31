/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CHARACTER3D_ENTITY_H
#define CHARACTER3D_ENTITY_H

#include <Engine/EngineCommon.h>
#include <Engine/Entity/Entity.h>
#include <Engine/Entity/MagnetEntity.h>
#include <Engine/Entity/ForceFieldEntity.h>
#include <Engine/Scene/TerrainCollidable.h>
#include <Engine/Scene/GeometryCollidable.h>
#include <Engine/Scene/Sprite.h>
#include <Engine/Audio/AudioSystem.h>
#include <Graphic/Core/RenderQueue.h>
#include <Graphic/RenderObjects/CharacterRenderObject.h>


namespace env
{
    class CParticleSystemEntity;
    class CTerrainMgr;
    class CTerrainMgr;
    class CBossEntity;

    class CCharacter3DEntity : public CEntity
    {
    public:

        struct MovementInputState
        {
            MovementInputState()
                : m_bPrimaryKeyPressed(false)
                , m_bSecondaryKeyPressed(false)
                , m_bKeyLeftPressed(false)
                , m_bKeyRightPressed(false) {}

            bool m_bPrimaryKeyPressed;
            bool m_bSecondaryKeyPressed;
            bool m_bKeyLeftPressed;
            bool m_bKeyRightPressed;
        };

        struct MovementParams
        {
            MovementParams();

            float m_playerAcceleration;
            float m_playerMaxSpeedX;
            float m_playerMaxSpeedY;
            float m_playerSuperSpeed;
            float m_jumpGravity;
            float m_jumpTime;
            float m_gravity;
            float m_initialPlayerMaxSpeedX;
            float m_initialPlayerMaxSpeedY;
            float m_initialJumpGravity;
            float m_initialPlayerAcceleration;
        };

        struct MovementState
        {
            MovementState();

            bool m_bJumpedFromWallFromRight;
            bool m_bJumpedFromWallFromLeft;
            bool m_bJumpedFromFloorOrRamp;
            bool m_bSecondJump;
            bool m_bSecondJumpBlocked;
            bool m_bFirstJumpFrame;
            bool m_bJumping;
            bool m_bJumpingUp;
            bool m_bJumpLocked;
            bool m_bWatchingRight;
            bool m_bMovementFlipInput;
            bool m_bMovementDisabled;
            bool m_bMovementSprintDisabled;
            bool m_bMovementJumpDisabled;
            bool m_bMovementDoubleJumpDisabled;
            bool m_bMovementWallGrabDisabled;
            bool m_bMovementWallJumpDisabled;
            bool m_bFrozen;
            bool m_bInForceField;
            bool m_bOnMagnet;
            bool m_bOnPlatform;

            CTimer m_jumpTimer;
            CTimer m_wallStickTimer;
            CTimer m_spacePressedTimer;
            CTimer m_jumpDelayTimer;
        };

        struct TransformState
        {
            TransformState();

            CQuaternion m_rootOrientation;
            CQuaternion m_bodyOrientation;
            CQuaternion m_leg01Orientation;
            CQuaternion m_leg02Orientation;
            CQuaternion m_chestOrientation;
            CQuaternion m_arm01Orientation;
            CQuaternion m_arm02Orientation;
            CQuaternion m_headOrientation;

            Vec3 m_rootPosition;
            Vec3 m_bodyPosition;
            Vec3 m_leg01Position;
            Vec3 m_leg02Position;
            Vec3 m_chestPosition;
            Vec3 m_arm01Position;
            Vec3 m_arm02Position;
            Vec3 m_headPosition;
        };

        struct AnimationState
        {
            AnimationState();

            float m_legRotation;
            float m_armRotation;
            float m_chestBreathe;
            float m_timeShape;

            float m_charHeadRoll;
            float m_charChestRoll;
            float m_charLeg01Roll;
            float m_charLeg02Roll;

            bool m_bArmRightRotation;
            bool m_bLegRightRotation;
        };

        struct LifeState
        {
            LifeState();

            bool m_bKilled;
            bool m_bGodmode;

            float m_killTime;
            float m_spawnTime;

            float m_currentKillTime;
            float m_currentSpawnTime;
        };

        enum TRIGGERABLE_EVENTS
        {
            CHARACTER_EVENT_ENABLE_MOVEMENT = ENTITY_EVENT_END,
            CHARACTER_EVENT_DISABLE_MOVEMENT,
            CHARACTER_EVENT_ENABLE_SPRINT,
            CHARACTER_EVENT_DISABLE_SPRINT,
            CHARACTER_EVENT_ENABLE_JUMP,
            CHARACTER_EVENT_DISABLE_JUMP,
            CHARACTER_EVENT_ENABLE_DOUBLEJUMP,
            CHARACTER_EVENT_DISABLE_DOUBLEJUMP,
            CHARACTER_EVENT_ENABLE_WALLGRAB,
            CHARACTER_EVENT_DISABLE_WALLGRAB,
            CHARACTER_EVENT_ENABLE_WALLJUMP,
            CHARACTER_EVENT_DISABLE_WALLJUMP,
            CHARACTER_EVENT_ENABLE_FREEZE,
            CHARACTER_EVENT_DISABLE_FREEZE,
            CHARACTER_EVENT_STARTPLAYBACK,
            CHARACTER_EVENT_FLIP_INPUT,
            CHARACTER_EVENT_DISABLE_TRACE,
            CHARACTER_EVENT_ENABLE_BLACKSKIN,
            CHARACTER_EVENT_DISABLE_BLACKSIN,
            CHARACTER_EVENT_ENABLE_TRACE,
            CHARACTER_EVENT_GAIN_STRENGTH_0,
            CHARACTER_EVENT_GAIN_STRENGTH_1,
            CHARACTER_EVENT_DISABLE_CELLS
        };

        static const float CHAR_SHAPE_TIME;
        static const float CHAR_ACCELERATION;
        static const float CHAR_GRAVITY;
        static const float CHAR_JUMP_GRAVITY;
        static const float CHAR_JUMP_TIME;
        static const float MAX_SPEED_ACCELERATION_LINE;
        static const float CHAR_MAX_SPEED_X;
        static const float CHAR_MAX_SPEED_Y;
        static const float CHAR_SUPER_SPEED;
        static const float CHAR_KILL_TIME;
        static const float CHAR_SPAWN_TIME;
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CCharacter3DEntity();
        virtual ~CCharacter3DEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual bool HasCollision(CEntity& e);
        virtual bool HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode);
        virtual void OnTriggerEntered(CEntity& e);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        virtual void OnLevelFinished();

        void SetCollision(bool bSet) { m_bCollision = bSet; }
        bool GetCollision() const { return m_bCollision; }

        inline const VECTOR2FIXED& GetPositionFixedPoint() const { return m_collisionState.pos; }
        inline const Vec2& GetVelocityFloatingPoint() const { return m_velocity; }

        inline const int GetWidth() const { return m_width; }
        inline const int GetHeight() const { return m_height; }

        inline void SetVelocity(const Vec2& vVelocity) { m_velocity = vVelocity; }

        void SetPosition(const Vec3& vPos, bool bUpdatePrevFramePos = false);
        void SetFixedPosition(const VECTOR2FIXED& pos);
        void SetCollisionState(const CollisionState& col);

        void SetKilled(bool bKilled);
        inline bool GetKilled() const { return !m_charState.m_bGodmode && m_charState.m_bKilled; }
        inline float GetKillTime() const { return m_charState.m_killTime; }
        inline float GetCurrentKillTime() const { return m_charState.m_currentKillTime; }

        inline float GetSpawnTime() const { return m_charState.m_spawnTime; }
        inline float GetCurrentSpawnTime() const { return m_charState.m_currentSpawnTime; }

        inline void SetDiffuseMtrl(const Vec3& color) { m_diffuseMtrl = color; }
        inline const Vec3& GetDiffuseMtrl() const { return m_diffuseMtrl; }

        inline void SetDiffuseAlpha(float alpha) { m_diffuseAlpha = alpha; }
        inline float GetDiffuseAlpha() const { return m_diffuseAlpha; }

        inline void SetCharMtrl(const Vec3& color) { m_charMtrl = Vec3(color); }
        inline const Vec3& GetCharMtrl() const { return m_charMtrl; }

        inline const CollisionState& GetCollisionState() const { return m_collisionState; }

        inline void ForceShape() { m_bForceShape = true; timeReshape = CHAR_SHAPE_TIME; }

    private:

        friend class boost::serialization::access;

        template< class Archive > void save(Archive& ar, const unsigned int version) const
        {
            ar & boost::serialization::base_object< CEntity >(*this);
            ar & m_pBodyNode;
            ar & m_pChestNode;
            ar & m_pHeadNode;
            ar & m_pLeg01Node;
            ar & m_pLeg02Node;
            ar & m_pArm01Node;
            ar & m_pArm02Node;
            ar & m_startPos;
            ar & m_bCollision;
            ar & m_pParticlesTerrainTrace;
            ar & m_pParticlesSuper;

            if (version > 0)
            {
                CMeshInstance* pHeadMesh = nullptr;
                CMeshInstance* pChestMesh = nullptr;
                CMeshInstance* pBodyMesh = nullptr;
                CMeshInstance* pArm01Mesh = nullptr;
                CMeshInstance* pArm02Mesh = nullptr;
                CMeshInstance* pLeg01Mesh = nullptr;
                CMeshInstance* pLeg02Mesh = nullptr;

                ar & pHeadMesh;
                ar & pChestMesh;
                ar & pBodyMesh;
                ar & pArm01Mesh;
                ar & pArm02Mesh;
                ar & pLeg01Mesh;
                ar & pLeg02Mesh;
            }

            if (version > 1)
            {
                ar & m_diffuseMtrl;
                ar & m_charMtrl;
            }

            if (version > 2)
            {
                ar & m_pParticlesDistortHitField;
            }

            if (version > 3)
            {
                ar & m_pParticlesDistortSuperField;
            }

            if (version > 4)
            {
                ar & m_pParticlesDistortTrace;
            }

            if (version > 5)
            {
                ar & m_pParticlesDistortSpawn;
                ar & m_pParticlesDistortDeath;
            }

            if (version > 6)
                ar & m_meshInstContainer;

            if (version > 7)
                ar & m_pParticlesSuperBlack;
        }

        template< class Archive > void load(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CEntity >(*this);
            ar & m_pBodyNode;
            ar & m_pChestNode;
            ar & m_pHeadNode;
            ar & m_pLeg01Node;
            ar & m_pLeg02Node;
            ar & m_pArm01Node;
            ar & m_pArm02Node;
            ar & m_startPos;
            ar & m_bCollision;
            ar & m_pParticlesTerrainTrace;
            ar & m_pParticlesSuper;

            if (version > 0)
            {
                CMeshInstance* pHeadMesh = nullptr;
                CMeshInstance* pChestMesh = nullptr;
                CMeshInstance* pBodyMesh = nullptr;
                CMeshInstance* pArm01Mesh = nullptr;
                CMeshInstance* pArm02Mesh = nullptr;
                CMeshInstance* pLeg01Mesh = nullptr;
                CMeshInstance* pLeg02Mesh = nullptr;

                // Bugfix: Forgot to serialize mesh instances!
                ar & pHeadMesh;
                ar & pChestMesh;
                ar & pBodyMesh;
                ar & pArm01Mesh;
                ar & pArm02Mesh;
                ar & pLeg01Mesh;
                ar & pLeg02Mesh;

                // Patch: remove mesh instances, replaced by mesh inst container
                gEngine->GetSceneMgr()->RemoveMeshInstance(pHeadMesh);
                gEngine->GetSceneMgr()->RemoveMeshInstance(pChestMesh);
                gEngine->GetSceneMgr()->RemoveMeshInstance(pBodyMesh);
                gEngine->GetSceneMgr()->RemoveMeshInstance(pArm01Mesh);
                gEngine->GetSceneMgr()->RemoveMeshInstance(pArm02Mesh);
                gEngine->GetSceneMgr()->RemoveMeshInstance(pLeg01Mesh);
                gEngine->GetSceneMgr()->RemoveMeshInstance(pLeg02Mesh);
            }

            if (version > 1)
            {
                ar & m_diffuseMtrl;
                ar & m_charMtrl;
            }

            if (version > 2)
            {
                ar & m_pParticlesDistortHitField;
            }

            if (version > 3)
            {
                ar & m_pParticlesDistortSuperField;
            }

            if (version > 4)
            {
                ar & m_pParticlesDistortTrace;
            }

            if (version > 5)
            {
                ar & m_pParticlesDistortSpawn;
                ar & m_pParticlesDistortDeath;
            }

            if (version > 6)
                ar & m_meshInstContainer;

            if (version > 7)
                ar & m_pParticlesSuperBlack;

        }

        BOOST_SERIALIZATION_SPLIT_MEMBER();

    private:

        void UpdateColors(float dt);
        void UpdateParticleSystems(float dt);
        void UpdateSounds(float dt);
        void UpdateCharacterShape(CCharacter3DEntity::TransformState& characterTransform,
                                  CCharacter3DEntity::MovementInputState& m_movementInputState,
                                  CCharacter3DEntity::MovementParams& m_movementParams,
                                  CCharacter3DEntity::MovementState& movementState,
                                  CCharacter3DEntity::AnimationState& animState,
                                  const Vec2& velocity,
                                  const CollisionState& colRes,
                                  float dt,
                                  float acceleratorTime);

        void InitMeshes();

        void InitSprites();
        void InitSprites(const std::string& strCharMaterial,
                         CSprite& spriteHead,
                         CSprite& spriteChest,
                         CSprite& spriteBody,
                         CSprite& spriteArm01,
                         CSprite& spriteArm02,
                         CSprite& spriteLeg01,
                         CSprite& spriteLeg02);

        void InitHitbox(bool bCreate = true);

        void InitParticleSystems();

        void InitSounds();

        void LoadScriptValues();

        void CheckPlatforms();

        bool CheckPlayerOutsideLevel();

        void DumpCollisionInfo();

        template<typename Platform> bool CheckPlatform(Platform* pPlatform);
    private:

        Vec2 m_velocity;
        VECTOR2FIXED m_fixedPositionPrevFrame;

        float m_currentAcceleratorTime;
        float m_acceleratorTime;

        bool m_bAcceleratorBoss;
        float m_bossAcceleratorTime;
        float m_bossAcceleratorDuration;
        CEntity* m_pAcceleratorBoss;


        Vec2 m_acceleratorForce;
        Vec2 m_currentAcceleratorForce;

        CForceFieldEntity* m_pForceField;
        CMagnetEntity* m_pMagnet;

        CollisionState m_collisionState;
        CollisionState m_prevCollisionState;
        MovementState m_prevMovementState;

        CSceneNode* m_pPlatformNode;
        VECTOR3FIXED m_fixedPlatformPrevPos;
        VECTOR3FIXED m_fixedPlatformRevertMove;
        CollisionState m_platformCollision;

        MovementInputState m_movementInputState;
        MovementParams m_movementParams;
        MovementState m_movementState;
        AnimationState m_animState;
        LifeState m_charState;

        int m_width;
        int m_height;

        float m_silhouetteRadius;
        float m_silhouetteOpacity;

        float m_gainStrengthTime;
        float m_gainStrengthDuration;

        float m_gainStrengthTime1;
        float m_gainStrengthDuration1;

        float timeReshape;
        bool m_bForceShape;

        bool m_bFirstStart;
        bool m_bBlackSkin;
        bool m_bTrace;
        bool m_bCells;

        CTerrainMgr* m_pLevelMgr;

        CParticleSystemEntity* m_pParticlesTerrainTrace;
        CParticleSystemEntity* m_pParticlesDistortHitField;
        CParticleSystemEntity* m_pParticlesDistortSuperField;
        CParticleSystemEntity* m_pParticlesDistortTrace;
        CParticleSystemEntity* m_pParticlesSuper;
        CParticleSystemEntity* m_pParticlesSuperBlack;
        CParticleSystemEntity* m_pParticlesDistortSpawn;
        CParticleSystemEntity* m_pParticlesDistortDeath;


        CSceneNode* m_pBodyNode;
        CSceneNode* m_pChestNode;
        CSceneNode* m_pHeadNode;
        CSceneNode* m_pLeg01Node;
        CSceneNode* m_pLeg02Node;
        CSceneNode* m_pArm01Node;
        CSceneNode* m_pArm02Node;

        CSprite m_spriteCharHead;
        CSprite m_spriteCharChest;
        CSprite m_spriteCharBody;
        CSprite m_spriteCharArm01;
        CSprite m_spriteCharArm02;
        CSprite m_spriteCharLeg01;
        CSprite m_spriteCharLeg02;

        CSprite m_spriteCharHeadBlack;
        CSprite m_spriteCharChestBlack;
        CSprite m_spriteCharBodyBlack;
        CSprite m_spriteCharArm01Black;
        CSprite m_spriteCharArm02Black;
        CSprite m_spriteCharLeg01Black;
        CSprite m_spriteCharLeg02Black;

        //         CMeshInstanceContainer m_pHeadMesh;
        //         CMeshInstanceContainer m_pChestMesh;
        //         CMeshInstanceContainer m_pBodyMesh;
        //         CMeshInstanceContainer m_pArm01Mesh;
        //         CMeshInstanceContainer m_pArm02Mesh;
        //         CMeshInstanceContainer m_pLeg01Mesh;
        //         CMeshInstanceContainer m_pLeg02Mesh;

        CMeshInstanceContainer m_meshInstContainer;


        Vec3 m_startPos;
        Vec4 m_characterDoubleJumpColor;
        Vec3 m_diffuseMtrl;
        float m_diffuseAlpha;
        Vec3 m_charMtrl;

        Character3DRenderObject m_charRenderObject;

        Vec3 m_magnetEnterPosition;
        float m_currentMagnetEnterTime;

        bool m_bCollision;

        std::list< CEntity* > m_platforms;

        enum CHARACTER_SOUND
        {
            CHARACTER_SOUND_MOVE = 0,
            CHARACTER_SOUND_JUMP,
            CHARACTER_SOUND_SECONDJUMP,
            CHARACTER_SOUND_HITFLOOR,
            CHARACTER_SOUND_SLIDEWALL,
            CHARACTER_SOUND_START,
            CHARACTER_SOUND_FINISH,
            CHARACTER_SOUND_DIE,
            CHARACTER_SOUND_FLY,
            CHARACTER_SOUND_MAGNET_HOVER,
            CHARACTER_SOUND_MAGNET_LAUNCH,
            CHARACTER_SOUND_END
        };
        std::vector< CSound* > m_characterSounds;

        // CharacterRenderObject m_characterRenderObject;

    };
} // env

#endif // CHARACTER3D_ENTITY_H 