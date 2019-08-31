/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CHARACTER_ENTITY_H
#define CHARACTER_ENTITY_H

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
    class CLightEntity;
    class CTerrainMgr;

    struct RecordState
    {
        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & pos;
            ar & bOnFloor;
            ar & bOnRamp;
            ar & bHitCeiling;
            ar & bStickyCeiling;
            ar & bGrabbedWallFromLeft;
            ar & bGrabbedWallFromRight;
            ar & bSwitch;
            ar & bWatchingRight;
            ar & bSecondary;
            ar & vVelocity;
            ar & color;
        }

        VECTOR2FIXED pos;

        bool bOnFloor,
            bOnRamp,
            bHitCeiling,
            bStickyCeiling,
            bGrabbedWallFromLeft,
            bGrabbedWallFromRight;

        bool bSwitch;
        bool bWatchingRight;
        bool bSecondary;

        Vec2 vVelocity;
        Vec4 color;

        void SetCollisionState(const CollisionState& s)
        {
            pos = s.pos;
            bOnFloor = s.bOnFloor;
            bOnRamp = s.bOnRamp;
            bHitCeiling = s.bHitCeiling;
            bStickyCeiling = s.bStickyCeiling;
            bGrabbedWallFromLeft = s.bGrabbedWallFromLeft;
            bGrabbedWallFromRight = s.bGrabbedWallFromRight;
        }

        CollisionState GetCollisionState()
        {
            CollisionState colState;
            colState.pos = pos;
            colState.bOnFloor = bOnFloor;
            colState.bOnRamp = bOnRamp;
            colState.bHitCeiling = bHitCeiling;
            colState.bStickyCeiling = bStickyCeiling;
            colState.bGrabbedWallFromLeft = bGrabbedWallFromLeft;
            colState.bGrabbedWallFromRight = bGrabbedWallFromRight;
            return colState;
        }

        
    };

    /*enum CHARACTER_TYPE
    {
        CHARACTER_TYPE_PLAYER = 0,
        CHARACTER_TYPE_CHASER,
        CHARACTER_TYPE_SHADOW
    };*/

    typedef std::list< RecordState > listRecordStates;

    #define CHARACTER_DEFAULT_POSITION VECTOR3(80.0f, 80.0f, 0.0f)

    const float CHAR_SHAPE_TIME = 0.1f;

    class CCharacter2DEntity : public CEntity
    {
    public:
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
            CHARACTER_EVENT_FLIP_INPUT
        };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CCharacter2DEntity();
        virtual ~CCharacter2DEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual bool HasCollision(CEntity& e);
        virtual bool HasCollisionWithRay(const Vec3& vRayOrigin, const Vec3& vRayDirection, float& distance, CSceneNode** ppNode);
        virtual void OnTriggerEntered(CEntity& e);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        void SetLight(bool bSet);

        //void SetType(CHARACTER_TYPE type) { m_charType = type; }
        //CHARACTER_TYPE GetPlayer() const { return m_charType; }
        void SetPlayer(bool bSet);
        bool GetPlayer() const { return m_bPlayer;}

        void SetSuccessFinisher(bool bSet) { m_bSuccessFinisher = bSet; }
        bool GetSuccessFinisher() const { return m_bSuccessFinisher; }

        void SetFailFinisher(bool bSet) { m_bFailFinisher = bSet; }
        bool GetFailFinisher() const { return m_bFailFinisher; }

        void SetCollision(bool bSet) { m_bCollision = bSet; }
        bool GetCollision() const { return m_bCollision; }


        CLightEntity* GetLight() { return m_pLight; }
        const VECTOR2FIXED& GetPositionFixedPoint() const { return m_pos; }
        const VECTOR2FIXED& GetVelocityFixedPoint() const { return m_velocity; }
        const Vec2& GetVelocityFloatingPoint() const { return m_vVelocity; }

        const int GetWidth() const { return m_width; }
        const int GetHeight() const { return m_height; }

        void SetVelocity(const Vec2& vVelocity) { m_vVelocity = vVelocity; }

        void SetPosition(const Vec3& vPos);
        void SetNewCollisionState(const CollisionState& col);

        void SetKilled(bool bKilled);
        bool GetKilled() const { return m_bKilled; }

        void SetPlayerColor(const Vec4& color) { m_characterPlayerColor = color; }
        const Vec4& GetPlayerColor() const { return m_characterPlayerColor; }

        const CollisionState& GetCollisionState() const { return m_collisionState; }

        void SetVisible(bool bSet) { m_bVisible = bSet; }


        void GetSprites(listSpriteRenderObjects& sprites);

        bool GetSecondJump() const { return m_bSecondJump; }
        float GetSecondJumpAlpha() const { return 1.0f - m_secondJumpAlpha; }


    private:
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CEntity >(*this);
            ar & m_pTraceParticles;
            ar & m_pJumpParticles;
            ar & m_pCharacterBodyNode;
            ar & m_pCharacterChestNode;
            ar & m_pCharacterHeadNode;
            ar & m_pCharacterLeg01Node;
            ar & m_pCharacterLeg02Node;
            ar & m_pCharacterArm01Node;
            ar & m_pCharacterArm02Node;
            ar & m_startPos;

            if(version > 0)
                ar & m_pLight;

            if(version > 1)
                ar & m_pTraceFloorParticles;

            if(version > 2)
            {
                ar & m_record;
                ar & m_bPlayer;
            }

            if(version > 3)
            {
                ar & m_bSuccessFinisher;
                ar & m_bFailFinisher;
                ar & m_bCollision;
            }

            if(version > 4)
            {
                ar & m_pParticlesTrace;
                ar & m_pParticlesDoubleJump;
            }

            if(version > 5)
            {
                ar & m_pParticlesBirth;
                ar & m_pParticlesDeath;
            }

            if(version > 6)
            {
                ar & m_pParticlesEnergyParticles;
            }

            if(version > 7)
                ar & m_pParticlesSuper;
        }
    private:

        void UpdateColors(float dt);
        void UpdateParticleSystems(float dt);
        void UpdateSounds();

        void InitSprites();

        void InitHitbox(bool bCreate = true);

        void InitParticleSystems();

        void InitSounds();

        void LoadScriptValues();

        void CheckPlatforms();

        bool CheckPlayerOutsideLevel();
    
        void DumpCollisionInfo();
    
    private:
        float m_secondJumpAlpha;

        VECTOR2FIXED m_velocity;
        VECTOR2FIXED m_pos;
        Vec2 m_vVelocity;


        float m_currentAcceleratorTime;
        float m_acceleratorTime;
        Vec2 m_acceleratorForce;
        Vec2 m_currentAcceleratorForce;

        bool m_bInForceField;
        CForceFieldEntity* m_pForceField;

        bool m_bOnMagnet;
        CMagnetEntity* m_pMagnet;

        CTimer m_jumpTimer;
        CTimer m_wallStickTimer;
        CTimer m_spacePressedTimer;
        CTimer m_jumpDelayTimer;

        CollisionState m_collisionState;
        CollisionState m_prevCollisionState;


        bool m_bOnPlatform;
        CSceneNode* m_pPlatformNode;
        VECTOR3FIXED m_platformPrevPos;
        VECTOR3FIXED m_vPlatformRevertMove;
        CollisionState m_platformCollision;







        bool m_bJumpedFromWallFromRight;
        bool m_bJumpedFromWallFromLeft;
        bool m_bJumpedFromFloorOrRamp;
        bool m_bSecondJump;
        bool m_bSecondJumpBlocked;
        bool m_bFirstJumpFrame;
        bool m_bJumping;
        bool m_bJumpingUp;
        bool m_bJumpLocked;
        bool m_bPrimaryKeyPressed;
        bool m_bSecondaryKeyPressed;
        bool m_bKeyLeftPressed;
        bool m_bKeyRightPressed;
        bool m_bWatchingRight;
        
        bool m_bMovementFlipInput;
        bool m_bMovementDisabled;
        bool m_bMovementSprintDisabled;
        bool m_bMovementJumpDisabled;
        bool m_bMovementDoubleJumpDisabled;
        bool m_bMovementWallGrabDisabled;
        bool m_bMovementWallJumpDisabled;
        bool m_bFrozen;

        bool m_bArmRightRotation;
        bool m_bLegRightRotation;

        bool m_bKilled;

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

        int m_width;
        int m_height;

        float m_legRotation;
        float m_armRotation;
        float m_chestBreathe;
        float m_timeShape;

        float m_charHeadRoll;
        float m_charChestRoll;
        float m_charLeg01Roll;
        float m_charLeg02Roll;

        float m_silhouetteRadius;
        float m_silhouetteOpacity;


        bool m_bVisible;

        CTerrainMgr* m_pLevel;

        CParticleSystemEntity* m_pTraceParticles;	
        CParticleSystemEntity* m_pTraceFloorParticles;
        CParticleSystemEntity* m_pJumpParticles;

        CParticleSystemEntity* m_pParticlesEnergyParticles;
        CParticleSystemEntity* m_pParticlesTrace;
        CParticleSystemEntity* m_pParticlesDoubleJump;
        CParticleSystemEntity* m_pParticlesDeath;
        CParticleSystemEntity* m_pParticlesBirth;
        CParticleSystemEntity* m_pParticlesSuper;

        CSceneNode* m_pCharacterBodyNode;
        CSceneNode* m_pCharacterChestNode;
        CSceneNode* m_pCharacterHeadNode;
        CSceneNode* m_pCharacterLeg01Node;
        CSceneNode* m_pCharacterLeg02Node;
        CSceneNode* m_pCharacterArm01Node;
        CSceneNode* m_pCharacterArm02Node;
        

        CSprite m_spriteCharHead;
        CSprite m_spriteCharChest;
        CSprite m_spriteCharBody;
        CSprite m_spriteCharArm01;
        CSprite m_spriteCharArm02;
        CSprite m_spriteCharLeg01;
        CSprite m_spriteCharLeg02;


        Vec3 m_startPos;
        Vec4 m_characterDoubleJumpColor;
        Vec4 m_characterPlayerColor;
        Vec4 m_characterShadowColor;

        Vec3 m_magnetEnterPosition;
        float m_currentMagnetEnterTime;

        CLightEntity* m_pLight;


        bool m_bPlayer;
        bool m_bSuccessFinisher;
        bool m_bFailFinisher;
        bool m_bCollision;

        listRecordStates m_record;
        listRecordStates m_currentRecord;

        RecordState m_currentState;

        enum CHARACTER_SOUND
        {
            CHARACTER_SOUND_MOVE = 0,
            CHARACTER_SOUND_JUMP,
            CHARACTER_SOUND_SECONDJUMP,
            CHARACTER_SOUND_HITFLOOR,
            CHARACTER_SOUND_SLIDEWALL,
            CHARACTER_SOUND_START,
            CHARACTER_SOUND_END
        };
        std::vector< CSound* > m_characterSounds;

        CharacterRenderObject m_characterRenderObject;
        
    };
} // env

#endif // CHARACTER_ENTITY_H 