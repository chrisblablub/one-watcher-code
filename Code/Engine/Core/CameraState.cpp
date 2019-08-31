#include <Engine/Core/CameraState.h>
#include <Engine/Core/Camera.h>

namespace env
{
    void CameraState::setFromCamera(CCamera* pCam, int screenWidth, int screenHeight)
    {
        m_mtxProjection = pCam->GetProjectionMatrix();
        m_mtxView = pCam->GetViewMatrix();
        m_mtxViewProjection = m_mtxView * m_mtxProjection;

        m_mtxInverseProjection = glm::inverse(m_mtxProjection);
        m_mtxInverseView = glm::inverse(m_mtxView);

        m_position = pCam->GetAttachedNode()->GetDerivedPosition();
        m_up = pCam->GetAttachedNode()->GetDerivedUp();
        m_forward = pCam->GetAttachedNode()->GetDerivedDirection();
        m_right = pCam->GetAttachedNode()->GetDerivedRight();

        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;

        m_frustum = pCam->GetFrustum();
    }
} // env