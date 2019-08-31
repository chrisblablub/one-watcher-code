/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once
#ifndef SCENENODEANIMATION_H
#define SCENENODEANIMATION_H

#include <Engine/EngineCommon.h>
#include <Engine/Scene/SceneNode.h>

namespace env
{
    class CSceneNodeAnimation
    {
    public:
        CSceneNodeAnimation(CSceneNode* target, Vec3 translationStart, Vec3 translationEnd, Vec3 scaleStart, Vec3 scaleEnd, CQuaternion rotationStart, CQuaternion rotationEnd, float duration);
        ~CSceneNodeAnimation();

        bool Update(float dt);

    private:
        CSceneNode* m_pTarget;

        Vec3 m_TranslationStart;
        Vec3 m_TranslationEnd;

        Vec3 m_ScaleStart;
        Vec3 m_ScaleEnd;

        CQuaternion m_RotationStart;
        CQuaternion m_RotationEnd;

        float m_CurrentTime;
        float m_Duration;
    };
} // env

#endif // SCENENODEANIMATION_H

