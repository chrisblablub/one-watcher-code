#include <Engine/Core/Camera.h>
#include <Graphic/Core/RenderQueue.h>

namespace env
{
    const std::string CCamera::ENTITY_META_TYPENAME = "Camera";

    //-----------------------------------------------------------------------------------
    CCamera::CCamera(float aspectRatio, float fov, float zNear, float zFar)
        : m_frustum(zNear, zFar, aspectRatio, fov),
        m_vDefaultPosition(Vec3(0.0f, 0.0f, -800.0f)),
        m_projection(PT_PERSPECTIVE)
    {
        BuildProjectionMatrix();
    }

    //-----------------------------------------------------------------------------------
    CCamera::CCamera(const CFrustum& frustum)
        : m_vDefaultPosition(Vec3(0.0f, 0.0f, -800.0f)),
        m_frustum(frustum),
        m_projection(PT_PERSPECTIVE)
    {
        BuildProjectionMatrix();
    }

    //-----------------------------------------------------------------------------------
    CCamera::~CCamera()
    {
    }

    //-----------------------------------------------------------------------------------
    void CCamera::Update()
    {
        BuildViewMatrix();
    }

    //-----------------------------------------------------------------------------------
    void CCamera::AddToRenderQueue(float dt, RenderQueue& renderQueue)
    {
        m_cameraRenderObject.m_mtxProjection = GetProjectionMatrix();
        m_cameraRenderObject.m_mtxView = GetViewMatrix();
        m_cameraRenderObject.m_mtxViewProjection = m_mtxViewProj;

        m_cameraRenderObject.m_mtxInverseProjection = glm::inverse(m_cameraRenderObject.m_mtxProjection);
        m_cameraRenderObject.m_mtxInverseView = glm::inverse(m_mtxView);

        m_cameraRenderObject.m_position = GetAttachedNode()->GetDerivedPosition();
        m_cameraRenderObject.m_up = GetAttachedNode()->GetDerivedUp();
        m_cameraRenderObject.m_forward = GetAttachedNode()->GetDerivedDirection();
        m_cameraRenderObject.m_right = GetAttachedNode()->GetDerivedRight();

        m_cameraRenderObject.m_screenWidth = static_cast<int>(m_frustum.GetWidth());
        m_cameraRenderObject.m_screenHeight = static_cast<int>(m_frustum.GetHeight());

        m_cameraRenderObject.m_near = m_frustum.GetNearClipping();
        m_cameraRenderObject.m_far = m_frustum.GetFarClipping();
        m_cameraRenderObject.m_t = std::tan(m_frustum.GetFieldOfView() / 2.0f);
        m_cameraRenderObject.m_b = -m_cameraRenderObject.m_t;
        m_cameraRenderObject.m_r = m_cameraRenderObject.m_t * m_frustum.GetAspectRatio();
        m_cameraRenderObject.m_l = -m_cameraRenderObject.m_r;


        m_cameraRenderObject.m_frustum = GetFrustum();

        renderQueue.m_pCameraRenderObject = &m_cameraRenderObject;
    }

    //-----------------------------------------------------------------------------------
    void CCamera::BuildProjectionMatrix()
    {
        if (m_projection == PT_PERSPECTIVE)
        {
            float t = std::tan(m_frustum.GetFieldOfView() / 2.0f);
            float b = -t;
            float r = t * m_frustum.GetAspectRatio();
            float l = -r;

            ComputeProjectionMatrix(m_mtxProj, l, r, t, b, m_frustum.GetNearClipping(), m_frustum.GetFarClipping());

            /*float xScale = 2.0f * m_frustum.GetNearClipping() / (r - l);
            float yScale = 2.0f * m_frustum.GetNearClipping() / (t - b);

            m_mtxProj[0] = VECTOR4(xScale, 0, (r + l) / (r - l), 0);
            m_mtxProj[1] = VECTOR4(0, yScale, (t + b) / (t - b), 0);
            m_mtxProj[2] = VECTOR4(0, 0,
            (m_frustum.GetFarClipping()) / (m_frustum.GetFarClipping() - m_frustum.GetNearClipping()),
            -m_frustum.GetNearClipping() * m_frustum.GetFarClipping() / (m_frustum.GetFarClipping() - m_frustum.GetNearClipping()));
            m_mtxProj[3] = VECTOR4(0, 0, 1, 0);*/

        }
        else
        {
            assert(false);
        }

        m_mtxViewProj = m_mtxView * m_mtxProj;

        m_frustum.SetViewProj(m_mtxViewProj);
    }

    //-----------------------------------------------------------------------------------
    void CCamera::SetProjectionType(PROJECTION_TYPE projection)
    {
        m_projection = projection;
    }

    //-----------------------------------------------------------------------------------
    void CCamera::BuildViewMatrix()
    {
        CSceneNode* pNode = GetAttachedNode();
        ENV_ASSERT_DBG(pNode);

        const Vec3& vPosition = pNode->GetDerivedPosition();
        const Vec3& vRight = pNode->GetDerivedRight();
        const Vec3& vUp = pNode->GetDerivedUp();
        const Vec3& vDirection = pNode->GetDerivedDirection();

        float x = -glm::dot(vPosition, vRight);
        float y = -glm::dot(vPosition, vUp);
        float z = -glm::dot(vPosition, vDirection);

        Matrix4x4 mtxTransform(1.0f);
        mtxTransform[0][3] = x;
        mtxTransform[1][3] = y;
        mtxTransform[2][3] = z;
        mtxTransform[3][3] = 1.0f;

        mtxTransform[0][0] = vRight.x;
        mtxTransform[0][1] = vRight.y;
        mtxTransform[0][2] = vRight.z;

        mtxTransform[1][0] = vUp.x;
        mtxTransform[1][1] = vUp.y;
        mtxTransform[1][2] = vUp.z;

        mtxTransform[2][0] = vDirection.x;
        mtxTransform[2][1] = vDirection.y;
        mtxTransform[2][2] = vDirection.z;

        mtxTransform[3][0] = 0.0f;
        mtxTransform[3][1] = 0.0f;
        mtxTransform[3][2] = 0.0f;

        m_mtxView = mtxTransform;

        m_mtxViewProj = m_mtxView * m_mtxProj;

        m_frustum.SetViewProj(m_mtxViewProj);
    }

    //-----------------------------------------------------------------------------------
    void CCamera::Reset()
    {
        CSceneNode* pNode = GetAttachedNode();
        pNode->SetPosition(m_vDefaultPosition);
        pNode->SetOrientation(CQuaternion());
    }

    //-----------------------------------------------------------------------------------
    CFrustum& CCamera::GetFrustum()
    {
        m_frustum.SetViewProj(m_mtxViewProj);

        return m_frustum;
    }

    //-----------------------------------------------------------------------------------
    void CCamera::SetFrustum(const CFrustum& frustum)
    {
        m_frustum = frustum;
        BuildProjectionMatrix();
    }

    //-----------------------------------------------------------------------------------
    float CCamera::GetFrustumHalfWidth(float z)
    {
        float hFovY = m_frustum.GetFieldOfView() * 0.5f;
        float halfWidth = abs(tan(atan(tan(hFovY) * m_frustum.GetNearClipping() * m_frustum.GetAspectRatio() /
            m_frustum.GetNearClipping())) * -z);

        return halfWidth;
    }

    //-----------------------------------------------------------------------------------
    float CCamera::GetFrustumHalfHeight(float z)
    {
        float hFovY = m_frustum.GetFieldOfView() * 0.5f;
        float halfHeight = abs(tan(hFovY) * -z);
        return halfHeight;
    }

    //-----------------------------------------------------------------------------------
    const Matrix4x4& CCamera::GetViewMatrix()
    {
        return m_mtxView;
    }

    //-----------------------------------------------------------------------------------
    const Matrix4x4& CCamera::GetProjectionMatrix()
    {
        return m_mtxProj;
    }

    //-----------------------------------------------------------------------------------
    Matrix4x4 CCamera::GetViewProjectionMatrix() const
    {
        return m_mtxView * m_mtxProj;
    }

    //-----------------------------------------------------------------------------------
    const Vec3& CCamera::GetDefaultPosition()
    {
        return m_vDefaultPosition;
    }
} // env