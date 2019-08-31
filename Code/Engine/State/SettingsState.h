/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SETTINGS_STATE_H
#define SETTINGS_STATE_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/State.h>
#include <Engine/Scene/Sprite.h>
#include <Engine/Entity/FadeEntity.h>
#include <Engine/Audio/AudioSystem.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    const float SETTINGS_STATE_DURATIONS[] =
    {
        0.2f,
        0.2f,
        0.0f
    };

    const Vec4 SETTINGS_MENUE_ITEM_COLOR(0.3f, 0.3f, 0.3f, 1.0f);
    const Vec4 SETTINGS_MENUE_ITEM_SELECTION_COLOR(1.0f, 1.0f, 1.0f, 1.0f);

    class CSettingsState : public CState
    {
    public:
        enum SETTINGS
        {
            SETTINGS_RESOLUTION = 0,
            SETTINGS_FULLSCREEN,
            SETTINGS_BLOOM,
            SETTINGS_VSYNC,
            SETTINGS_LIGHTRAYS,
            SETTINGS_AUDIO_SFX,
            SETTINGS_AUDIO_MUSIC,
            SETTINGS_APPLY,
            SETTINGS_END
        };

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

        CSettingsState();
        virtual ~CSettingsState();

        virtual bool Load(const std::string& strPreviousState);
        virtual bool Unload();

        virtual void Update(float time, float dt, RenderQueue& renderQueue);

    private:
        void SetState(SETTINGS_STATE state) { m_state = state; m_currentStateTime = SETTINGS_STATE_DURATIONS[(int)state]; }
        float GetCurrentStateDuration() { return SETTINGS_STATE_DURATIONS[(int)m_state]; }

        void UpdateInput();

        void ApplyAudioSettings();
    private:
        std::string m_strPreviousState;

        unsigned int m_selection;

        SETTINGS_STATE m_state;

        FadeBarsRenderObject m_fade;

        float m_currentStateTime;

        bool m_bResolutionChanged;

        CSound* m_pGameSound;

        RenderQualityParams m_videoParams;
        AudioParameters m_audioParams;

        //Text m_textResolution;
        std::vector< TextRenderObject > m_settings;
        std::vector< TextRenderObject > m_settingsValues;

        TextRenderObject m_settingsText;

        CSound* m_pSFXSound;
        CSound* m_pMusic;

    };
} // env
#endif // SETTINGS_STATE_H