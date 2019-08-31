/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CHECKPOINT_ENTITY_H
#define CHECKPOINT_ENTITY_H


#include <Engine/EngineCommon.h>
#include <Engine/Entity/SpriteEntity.h>
#include <Engine/Entity/FadeEntity.h>
#include <Engine/Audio/AudioSystem.h>
#include <Common/Core/StateMachine.h>

namespace env
{
    class CCharacter3DEntity;

    class CCheckpointEntity : public CSpriteEntity
    {
    public:
        enum CHECKPOINT_STATE
        {
            CHECKPOINT_STATE_UNDEFINED = 0,
            CHECKPOINT_STATE_OPENING_16_9,
            CHECKPOINT_STATE_OPENED_16_9,
            CHECKPOINT_STATE_OPENING,
            CHECKPOINT_STATE_OPENED,
            CHECKPOINT_STATE_CLOSING_16_9,
            CHECKPOINT_STATE_CLOSED_16_9,
            CHECKPOINT_STATE_CLOSING,
            CHECKPOINT_STATE_CLOSED,
            CHECKPOINT_STATE_STARTER,
            CHECKPOINT_STATE_FINISHER
        };

        enum TRIGGERABLE_EVENTS
        {
            CHECKPOINT_EVENT_CHANGELEVEL = SPRITE_EVENT_END
        };

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        CCheckpointEntity();
        virtual ~CCheckpointEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnTriggerEntered(CEntity& e);
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        void SetStart(bool bStart);
        bool GetStart();

        void SetFinal(bool bFinal);
        bool GetFinal();

        void SetYaw(float yaw) { m_yaw = yaw; }
        float GetYaw() const { return m_yaw; }

        void SetPitch(float pitch) { m_pitch = pitch; }
        float GetPitch() const { return m_pitch; }

        void SetRoll(float roll) { m_roll = roll; }
        float GetRoll() const { return m_roll; }

        void SetZoomOpeningFactor(float zoom) { m_openingZoomFactor = zoom; }
        float GetZoomOpeningFactor() const { return m_openingZoomFactor; }

        void SetZoomClosingFactor(float zoom) { m_closingZoomFactor = zoom; }
        float GetZoomClosingFactor() const { return m_closingZoomFactor; }

        void SetBronzeTime(float t) { m_levelBronzeTime = t; }
        float GetBronzeTime() const { return m_levelBronzeTime; }

        void SetSilverTime(float t) { m_levelSilverTime = t; }
        float GetSilverTime() const { return m_levelSilverTime; }

        void SetGoldTime(float t) { m_levelGoldTime = t; }
        float GetGoldTime() const { return m_levelGoldTime; }

        void SetNextLevel(const std::string& strNextLevel) { m_strNextLevel = strNextLevel; }
        const std::string& GetNextLevel() const { return m_strNextLevel; }

        void SetFinishChapter(bool bFinish) { m_bFinishChapter = bFinish; }
        bool GetFinishChapter() const { return m_bFinishChapter; }

        

    private:
        void LoadLevel();
        void FireCheckpoint();
        void InitSprites(bool bCreate);

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CSpriteEntity >(*this);

            if (version > 0)
            {
                ar & m_strNextLevel;
                ar & m_bStart;
                ar & m_bFinal;
                ar & m_openingTime;
                ar & m_openingZoomFactor;
                ar & m_yaw;
                ar & m_pitch;
                ar & m_roll;
            }

            if (version > 1)
            {
                ar & m_closingZoomFactor;
                ar & m_closingTime;
                ar & m_closingPauseTime;
                ar & m_openingPauseTime;
                ar & m_pFade;
            }

            if (version > 2)
                ar & m_bFinishChapter;

            if (version > 3)
            {
                ar & m_levelBronzeTime;
                ar & m_levelSilverTime;
                ar & m_levelGoldTime;
            }
        }

    private:
        std::string m_strNextLevel;

        bool m_bCheckpointFired;

        bool m_bStart;
        bool m_bFinal;
        bool m_bFinishChapter;

        enum CHECKPOINT_ACTION
        {
            CHECKPOINT_ACTION_WAITFORINPUT = 0,
            CHECKPOINT_ACTION_LOADLEVEL,
            CHECKPOINT_ACTION_REPLAYLEVEL,
            CHECKPOINT_ACTION_LOADMENUE
        };

        bool m_bTriggerCheckpointAction;

        CHECKPOINT_ACTION m_checkpointAction;

        float m_closingTime;
        float m_closingPauseTime;
        float m_openingTime;
        float m_openingPauseTime;
        float m_currentStateTime;
        float m_openingZoomFactor;
        float m_closingZoomFactor;
        float m_yaw;
        float m_pitch;
        float m_roll;

        CFadeEntity* m_pFade;

        FadeBarsRenderObject m_fade;

        CHECKPOINT_STATE m_state;

        Vec3 m_camStartPos;
        Vec3 m_camFinalPos;

        float m_levelFinishTime;
        float m_levelBronzeTime;
        float m_levelSilverTime;
        float m_levelGoldTime;

        CSprite m_logo;
        CSceneNode m_logoNode;

        CSound* m_pMenueTransitionInSound;
        CSound* m_pMenueTransitionOutSound;
        CSound* m_pCharacterFinishSound;

        CSprite* m_pTimeBackground;
        CSprite* m_pTimeBackgroundGold;
        CSprite* m_pTimeBackgroundSilver;
        CSprite* m_pTimeBackgroundBronze;
        CSprite* m_pKillsBackground;
        CSprite* m_pReward;
        CSprite* m_pRewardGold;
        CSprite* m_pRewardSilver;
        CSprite* m_pRewardBronze;
        CSprite* m_pArrowLeft;
        CSprite* m_pArrowRight;
        CSprite* m_pEscape;

        CSceneNode* m_pTimeBackgroundNode;
        CSceneNode* m_pTimeBackgroundGoldNode;
        CSceneNode* m_pTimeBackgroundSilverNode;
        CSceneNode* m_pTimeBackgroundBronzeNode;
        CSceneNode* m_pKillsBackgroundNode;
        CSceneNode* m_pRewardNode;
        CSceneNode* m_pArrowLeftNode;
        CSceneNode* m_pArrowRightNode;
        CSceneNode* m_pEscapeNode;

        CCharacter3DEntity* m_pCharacter;
    };


    class CCheckpointOriginEntity : public CTriggerEntity
    {
    public:

        static ENTITY_TYPEID ENTITY_META_TYPEID;

        enum TRIGGERABLE_EVENTS
        {
            CHECKPOINT_EVENT_SET_DISABLED = SPRITE_EVENT_END
        };

        typedef CStateMachineRender<CCheckpointOriginEntity> StateMachineOrigin;

        CCheckpointOriginEntity();
        virtual ~CCheckpointOriginEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnTriggerEntered(CEntity& e);
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);

        inline bool GetDisabledInitial() const { return m_bDisabledInitial; }
        inline void SetDisabledInitial(bool bDisabledInitial) { m_bDisabledInitial = bDisabledInitial; }

    private:
      
        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CTriggerEntity >(*this);

            if (version > 0)
                ar & m_bDisabledInitial;
        }

        void InitStateMachine();
        void FadeInUpdate(const StateMachineOrigin::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        
    private:

        FadeBarsRenderObject m_fade;

        CCharacter3DEntity* m_pCharacter;

        bool m_bCheckpointFired;
        bool m_bDisabled;
        bool m_bDisabledInitial;

        StateMachineOrigin m_stateMachineBars;
    };


    class CPlayerCameraEntity;

    class CCheckpointDestinatonEntity : public CTriggerEntity
    {
    public:

        enum TRIGGERABLE_EVENTS
        {
            CHECKPOINT_EVENT_CHANGELEVEL = SPRITE_EVENT_END
        };

        enum STATES
        {
            STATE_FADE_BORDERS_AND_POSITION_CAM = 0,
            STATE_FADE_CHARACTER,
            STATE_FADE_BARS_AND_SCREEN
        };

        static const FontInfo fontMenue;
        static ENTITY_TYPEID ENTITY_META_TYPEID;

        typedef CStateMachineRender<CCheckpointDestinatonEntity> StateMachine;

        CCheckpointDestinatonEntity();
        virtual ~CCheckpointDestinatonEntity();

        virtual void InitializeAtCreation();
        virtual void InitializeAtLoad();
        virtual void OnTriggerEntered(CEntity& e);
        virtual void Update(float dt);
        virtual void AddToRenderQueue(float dt, RenderQueue& queue);
        virtual void OnGameModeChanged(bool bGameMode);
        virtual void OnInput(unsigned int eventId, const EventParameter& eventParameter);
        
        void LoadLevel();

        void SetNextLevel(const std::string& strNextLevel) { m_strNextLevel = strNextLevel; }
        const std::string& GetNextLevel() const { return m_strNextLevel; }

        void SetFinishChapter(bool bFinish) { m_bFinishChapter = bFinish; }
        bool GetFinishChapter() const { return m_bFinishChapter; }


    private:

        friend class boost::serialization::access;

        template< class Archive > void serialize(Archive& ar, const unsigned int version)
        {
            ar & boost::serialization::base_object< CTriggerEntity >(*this);
            ar & m_pLogoBackground;

            if (version > 0)
                ar & m_strNextLevel;

            if (version > 1)
                ar & m_bFinishChapter;
        }

        void InitSprites();

        void InitStateMachine();
        void FadeBordersAndPositionCamUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void FadeBordersAndPositionCamLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);
        void FadeCharacterAndLogoEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);
        void FadeCharacterAndLogoUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void FadeBarsAndScreenUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void FadeBarsAndScreenLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);

        void LevelCompleteEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);
        void LevelCompleteUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);

        void LevelUnlockedEnter(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);
        void LevelUnlockedUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);

    private:
        std::string m_strNextLevel;

        CSpriteEntity* m_pLogoBackground;

        CCharacter3DEntity* m_pCharacter;

        Vec3 m_camStartPos;
        Vec3 m_camTargetPos;
        Vec3 m_charPos;

        bool m_bCheckpointFired;
        bool m_bFinishChapter;

        float m_startRotationRoll;
        float m_targetRotationRoll;
        float m_rotationSpeed;

        CPlayerCameraEntity* m_pPlayerCamera;
        StateMachine m_stateMachineBars;

        CSprite m_spriteLevelUnlocked;
        CSprite m_spriteLevelUnlockedName;
        CSceneNode m_nodeLevelUnlocked;
        CSceneNode m_nodeLevelUnlockedName;

        CSprite m_spriteComplete;
        CSceneNode m_nodeComplete;

        TextRenderObject m_levelIndexText;

        CSound* m_pLevelFinishedSound;

        std::queue<unsigned int> m_unlockedLevels;
        CManagedMaterial* m_pLevelNamesMtrl;
        CManagedMaterial* m_pBossUnlocked;
        CManagedMaterial* m_pLevelUnlocked;
    };
} // env

#endif // CHECKPOINT_ENTITY_H 