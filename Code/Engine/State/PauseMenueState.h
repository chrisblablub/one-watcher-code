/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

 /*#pragma once
 #ifndef PAUSE_MENUE_STATE_H
 #define PAUSE_MENUE_STATE_H

 #include "../Core/State.h"
 #include "../Scene/Sprite.h"
 #include "../Entity/FadeEntity.h"
 #include "../Audio/AudioSystem.h"

 namespace env
 {
     const float PAUSEMENUE_STATE_DURATIONS[] =
     {
         0.2f,
         0.2f,
         0.0f,
         0.35f
     };

     class CPauseMenueState : public CState
     {
     public:
         enum PAUSEMENUE_STATE
         {
             PAUSEMENUE_STATE_LOAD,
             PAUSEMENUE_STATE_UNLOADCONTINUE,
             PAUSEMENUE_STATE_MENUE,
             PAUSEMENUE_STATE_UNLOADMENUE
         };

         static const std::string STATE_NAME;

         CPauseMenueState();
         virtual ~CPauseMenueState();

         virtual bool Load(const std::string& strPreviousState);
         virtual bool Unload();

         virtual void Update(float time, float dt, RenderQueue& renderQueue);

     private:
         void SetState(PAUSEMENUE_STATE state) { m_state = state; m_currentStateTime = PAUSEMENUE_STATE_DURATIONS[(int)state]; }
         float GetCurrentStateDuration() { return PAUSEMENUE_STATE_DURATIONS[(int)m_state]; }

     private:
         CSprite* m_pContinueBtn;
         CSprite* m_pMenueBtn;
         CSprite* m_pOptionsBtn;
         CSprite* m_pBackground;

         CSceneNode* m_pContinueBtnNode;
         CSceneNode* m_pMenueBtnNode;
         CSceneNode* m_pOptionsBtnNode;
         CSceneNode* m_pBackgroundNode;

         unsigned int m_selection;

         PAUSEMENUE_STATE m_state;

         Fade m_fade;

         float m_startTime;
         float m_introTime;
         float m_loadTime;
         float m_unloadTime;
         float m_currentStateTime;

         CSound* m_pGameSound;


     };
 } // env
 #endif // PAUSE_MENUE_STATE_H*/


#pragma once
#ifndef PAUSE_MENUE_STATE_H
#define PAUSE_MENUE_STATE_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/State.h>
#include <Engine/Scene/Sprite.h>
#include <Engine/Entity/FadeEntity.h>
#include <Engine/Audio/AudioSystem.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    const float PAUSEMENUE_STATE_DURATIONS[] =
    {
        0.2f,
        0.2f,
        0.2f,
        0.5f,
        0.5f,
        0.0f
    };

    const Vec4 PAUSEMENUE_MENUE_ITEM_COLOR(0.3f, 0.3f, 0.3f, 1.0f);
    const Vec4 PAUSEMENUE_MENUE_ITEM_SELECTION_COLOR(1.0f, 1.0f, 1.0f, 1.0f);

    class CPauseMenueState : public CState
    {
    public:

        enum PAUSEMENUE
        {
            PAUSEMENUE_SKIP_CUTSCENE = 0,
            PAUSEMENUE_SETTINGS,
            PAUSEMENUE_MENUE,
            PAUSEMENUE_QUIT,
            PAUSEMENUE_END
        };

        enum PAUSEMENUE_STATE
        {
            PAUSEMENUE_STATE_LOAD,
            PAUSEMENUE_STATE_UNLOADCONTINUE,
            PAUSEMENUE_STATE_UNLOADSETTINGS,
            PAUSEMENUE_STATE_UNLOADMENUE,
            PAUSEMENUE_STATE_UNLOADQUIT,
            PAUSEMENUE_STATE_MENUE
        };

        static const float FADE_IN_AMOUNT;
        static const float FADE_SCREEN_AMOUNT;
        static const Vec4 FADE_FRAME_AMOUNT;
        static const std::string STATE_NAME;
        static const FontInfo fontMenueSelection;
        static const FontInfo fontMenueTitle;
        static const FontInfo fontMenue;


        CPauseMenueState();
        virtual ~CPauseMenueState();

        virtual bool Load(const std::string& strPreviousState);
        virtual bool Unload();

        virtual void Update(float time, float dt, RenderQueue& renderQueue);

    private:
        void SetState(PAUSEMENUE_STATE state) { m_state = state; m_currentStateTime = PAUSEMENUE_STATE_DURATIONS[(int)state]; }
        float GetCurrentStateDuration() { return PAUSEMENUE_STATE_DURATIONS[(int)m_state]; }

        void UpdateInput();

    private:
        std::string m_strPreviousState;

        unsigned int m_selection;

        PAUSEMENUE_STATE m_state;

        FadeBarsRenderObject m_fade;

        float m_currentStateTime;

        CSound* m_pGameSound;

        CSprite* m_pMenueLogo;
        CSceneNode* m_pMenueLogoNode;

        std::vector< TextRenderObject > m_settings;
        std::vector< TextRenderObject > m_settingsValues;
        TextRenderObject m_menueText;
        TextRenderObject m_levelText;

        CSprite m_spriteLevelName;
        CSceneNode m_nodeLevelName;

        bool m_bCutScene;

        CManagedMaterial* m_pLevelNamesMtrl;
        unsigned int m_minIndex;

    };
} // env
#endif // PAUSE_MENUE_STATE_H