/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef CREDITS_STATE_H
#define CREDITS_STATE_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/State.h>
#include <Engine/Scene/Sprite.h>
#include <Engine/Entity/FadeEntity.h>
#include <Engine/Audio/AudioSystem.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    class CCreditsState : public CState
    {
    public:

        enum SETTINGS_STATE
        {
            SETTINGS_STATE_LOAD,
            SETTINGS_STATE_UNLOAD,
            SETTINGS_STATE_MENUE
        };

        static const float FADE_IN_AMOUNT;
        static const float FADE_SCREEN_AMOUNT;
        static const Vec4 FADE_FRAME_AMOUNT;
        static const std::string STATE_NAME;
        static const FontInfo fontMenue;
        static const FontInfo fontMenueTitle;

        CCreditsState();
        virtual ~CCreditsState();

        virtual bool Load(const std::string& strPreviousState);
        virtual bool Unload();

        virtual void Update(float time, float dt, RenderQueue& renderQueue);

    private:
        void SetState(SETTINGS_STATE state) { m_state = state; m_currentStateTime = SETTINGS_STATE_DURATIONS[(int)state]; }
        float GetCurrentStateDuration() { return SETTINGS_STATE_DURATIONS[(int)m_state]; }

        void UpdateInput();

    private:
        std::string m_strPreviousState;

        unsigned int m_selection;

        SETTINGS_STATE m_state;

        FadeBarsRenderObject m_fade;

        float m_currentStateTime;

        CSound* m_pGameSound;

        CSprite m_spriteFMod;
        CSceneNode m_nodeFMod;

        std::vector< TextRenderObject > m_credits;

        TextRenderObject m_title;

    };
} // env
#endif // CREDITS_STATE_H