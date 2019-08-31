/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef MENUE_STATE_H
#define MENUE_STATE_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/State.h>
#include <Engine/Scene/Sprite.h>
#include <Engine/Entity/FadeEntity.h>
#include <Engine/Audio/AudioSystem.h>
#include <Graphic/Core/StripeGenerator.h>

namespace env
{

    float MENUE_STATE_DURATIONS[] =
    {
        2.0f,
        0.5f,
        0.6f,
        1.0f,
        0.5f,
        0.6f,
        0.2f,
        0.6f,
        0.6f,
        0.2f,
        0.2f
    };

    class CMenueState : public CState
    {
    public:
        enum MENUE_STATE
        {
            MENUE_STATE_PRESSSTART = 0,
            MENUE_STATE_FADEOUT,
            MENUE_STATE_FADEIN,
            MENUE_STATE_CAMERAMOVE,
            MENUE_STATE_MENUESELECTION,
            MENUE_STATE_LOADFROMLEVELSELECTION,
            MENUE_STATE_LOADFROMSETTINGS,
            MENUE_STATE_UNLOAD,
            MENUE_STATE_UNLOADQUIT,
            MENUE_STATE_UNLOADSETTINGS,
            MENUE_STATE_UNLOADCREDITS
        };

        enum MENUE_ITEM
        {
            MENUE_ITEM_START = 0,
            MENUE_ITEM_OPTIONS,
            MENUE_ITEM_CREDITS,
            MENUE_ITEM_QUIT
        };

        static const std::string STATE_NAME;

        CMenueState();
        virtual ~CMenueState();

        virtual bool Load(const std::string& strPreviousState);
        virtual bool Unload();

        virtual void Update(float time, float dt, RenderQueue& renderQueue);
        virtual void OnResetDevice(int screenWidth, int screenHeight);

    private:
        void FadeMenuePage(float alpha, RenderQueue& queue);

        void SetState(MENUE_STATE state) { m_state = state; m_currentStateTime = MENUE_STATE_DURATIONS[(int)state]; }
        float GetCurrentStateDuration() { return MENUE_STATE_DURATIONS[(int)m_state]; }

        CSprite* m_pIntroLogo;

        CSceneNode* m_pIntroLogoNode;

        unsigned int m_mainSelection;

        typedef std::pair< CSprite, CSceneNode > pairSpriteNode;
        std::vector< pairSpriteNode > m_menueItems;

        MENUE_STATE m_state;

        FadeBarsRenderObject m_fade;

        float m_currentStateTime;

        float m_currentSelectionTime;

        CSound* m_pMenueTransitionInSound;
        CSound* m_pMenueTransitionOutSound;
        CSound* m_pMenueSelectionSound;
        CSound* m_pMenueConfirmationSound;

        CStripeManager m_stripeManager;

        float m_stripeDuration;
        float m_stripeTime;

        TextRenderObject m_textPressStart;
    };
} // env
#endif // MENUE_STATE_H