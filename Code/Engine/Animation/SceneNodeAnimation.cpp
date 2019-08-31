#include <Engine/Animation/SceneNodeAnimation.h>

namespace env
{
    CSceneNodeAnimation::CSceneNodeAnimation(CSceneNode* target, Vec3 translationStart, Vec3 translationEnd, Vec3 scaleStart, Vec3 scaleEnd, CQuaternion rotationStart, CQuaternion rotationEnd, float duration)
        : m_pTarget(target)
        , m_TranslationStart(translationStart)
        , m_TranslationEnd(translationEnd)
        , m_ScaleStart(scaleStart)
        , m_ScaleEnd(scaleEnd)
        , m_RotationStart(rotationStart)
        , m_RotationEnd(rotationEnd)
        , m_CurrentTime(0.0f)
        , m_Duration(duration)
    {

    }

    CSceneNodeAnimation::~CSceneNodeAnimation()
    {

    }

    bool CSceneNodeAnimation::Update(float dt)
    {
        if (m_pTarget == nullptr)
        {
            return true;
        }

        m_CurrentTime += dt;
        m_CurrentTime = Clamp(m_CurrentTime, 0.0f, m_Duration);

        float blendFactor = 0;

        if (m_Duration != 0.0f)
        {
            blendFactor = Clamp(m_CurrentTime / m_Duration, 0.0f, 1.0f);
            blendFactor = 1.0f - blendFactor;
            blendFactor *= blendFactor;
            blendFactor = 1.0f - blendFactor;
        }

        Vec3 currentTranslation = (1 - blendFactor) * m_TranslationStart + blendFactor * m_TranslationEnd;
        Vec3 currentScale = (1 - blendFactor) * m_ScaleStart + blendFactor * m_ScaleEnd;
        CQuaternion currentRotation = m_RotationEnd;
        currentRotation.Slerp(m_RotationStart, blendFactor);

        m_pTarget->SetPosition(currentTranslation);
        m_pTarget->SetScale(currentScale);
        m_pTarget->SetOrientation(currentRotation);


        bool isDone = Equals(m_CurrentTime, m_Duration);
        return isDone;
    }
} // env