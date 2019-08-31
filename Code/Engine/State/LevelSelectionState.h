/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef LEVEL_SELECTION_STATE_H
#define LEVEL_SELECTION_STATE_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/State.h>
#include <Engine/Scene/Sprite.h>
#include <Engine/Entity/FadeEntity.h>
#include <Engine/Audio/AudioSystem.h>
#include <Common/Core/StateMachine.h>

namespace env
{


    class CLevelSelectionState : public CState
    {
    public:
        enum STATE
        {
            STATE_LOAD = 0,
            STATE_MENUE,
            STATE_UNLOAD,
            STATE_LOADLEVEL,
            STATE_CHAPTER_COMPLETE_CAM_TRANS,
            STATE_CHAPTER_COMPLETE_CAM_SHAKE,
            STATE_CHAPTER_COMPLETE_CHANGE,
            STATE_LOADCHAPTER,
            STATE_UNLOADCHAPTER
        };

        static const std::string STATE_NAME;

        CLevelSelectionState();
        virtual ~CLevelSelectionState();

        virtual bool Load(const std::string& strPreviousState);
        virtual bool Unload();

        virtual void Update(float time, float dt, RenderQueue& renderQueue);
        virtual void OnResetDevice(int screenWidth, int screenHeight);

        void SetPlayChapterComplete(bool bSet) { m_bPlayChapterFinish = bSet; }

    private:
        void UpdateLabels(float dt);
        void ProcessInput(STATE& nextState);
        void FadeMenuePage(float alpha, FADE_BARS_TYPE fadeType, RenderQueue& queue);

        typedef CStateMachineRender<CLevelSelectionState> StateMachine;

        void LoadMenuUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void UnloadMenuUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void UnloadMenuLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);
        void LoadMenuLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);
        void MenuUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);

        void UnloadChapterUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void LoadChapterUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void LoadChapterLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);

        void LoadLevelUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void LoadLevelLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);

        void ChapterCompleteCamTransitionUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void ChapterCompleteCamShakeUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);

        void ChapterCompleteChangeUpdate(const StateMachine::FrameParamsType& frameParams, const StateUpdateInfo& stateInfo);
        void ChapterCompleteChangeLeave(const StateMachine::FrameParamsType& frameParams, const StateEnterLeaveInfo& stateInfo);

        typedef std::pair< CSprite, CSceneNode > pairSpriteNode;
        std::vector< pairSpriteNode > m_part1LevelSelections;
        CManagedMaterial* m_pPart1LevelSelectionMtrl;

        CSprite* m_pTitleSprite;
        CSceneNode* m_pTitleNode;

        CSprite* m_pChapterSprite;
        CSceneNode* m_pChapterNode;

        CSprite* m_pCompleteSprite;
        CSceneNode* m_pCompleteNode;

        unsigned int m_currentSelection;
        unsigned int m_currentChapter;

        STATE m_state;

        FadeBarsRenderObject m_fade;

        float m_loadTime;
        float m_unloadTime;
        float m_currentStateTime;
        float m_currentSelectionTime;

        bool m_bChangeChapterLeft;
        bool m_bLoadFromChapter;
        bool m_bPlayChapterFinish;

        unsigned int m_firstLevelIndex;
        unsigned int m_chapter;

        Vec3 m_camStartPos;
        Vec3 m_camEndPos;

        CSound* m_pMenueSelectionSound;
        CSound* m_pMenueTransitionInSound;
        CSound* m_pMenueTransitionOutSound;
        CSound* m_pMenueConfirmationSound;

		CSound* m_pMenueUnlockLevelSound;
		CSound* m_pMenueUnlockChapterSound;

        CSprite* m_pTimeBackground;
        CSprite* m_pTimeBackgroundGold;
        CSprite* m_pTimeBackgroundSilver;
        CSprite* m_pTimeBackgroundBronze;
        CSprite* m_pKillsBackground;

        CSceneNode* m_pTimeBackgroundNode;
        CSceneNode* m_pKillsBackgroundNode;

        

        StateMachine m_stateMachine;

    };
} // env
#endif // LEVEL_SELECTION_STATE_H