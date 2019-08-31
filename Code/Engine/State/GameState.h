/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Engine/EngineCommon.h>
#include <Engine/Core/State.h>
#include <Engine/Core/Input.h>
#include <Engine/Entity/FadeEntity.h>
#include <Engine/Audio/AudioSystem.h>

namespace env
{
    class CCharacter3DEntity;

    class CGameState : public CState, 
                       public CMouseListener
    {
    public:
        static const std::string STATE_NAME;

        static const FontInfo fontMenue;

        CGameState();
        virtual ~CGameState();

        virtual bool Load(const std::string& strPreviousState);
        virtual bool Unload();

        virtual void Update(float time, float dt, RenderQueue& renderQueue);

        void ProcessInput(float dt, RenderQueue& renderQueue);

    private:
        CCharacter3DEntity* m_pCharacter;

        FadeBarsRenderObject m_deathFade;

        bool m_bNewGameMode;
        bool m_bIsBossLevel;
        bool m_bIsCutScene;

        TextRenderObject m_deathsPerLevel;
        TextRenderObject m_deathsTotal;

        bool m_bKilled;
//        TextRenderObject m_deathsSplitter;

//         float m_currentDistortionTime;
//         float m_distortionTime;
//         float m_distortionAmount;
//         float m_distortionOffset;
//         Vec2 m_distortionScaling;
//         Vec2 m_distortionDirection;
    };
} // env
#endif // GAME_STATE_H