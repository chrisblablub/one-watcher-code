
#include <Engine/Entity/EntitySlider.h>
#include <Engine/Core/Input.h>
#include <Engine/Scene/SceneMgr.h>
#include <Engine/Scene/TerrainMgr.h>
#include <Engine/Math/Intersection.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#include <Graphic/Core/Mesh.h>

namespace env
{
    //-----------------------------------------------------------------------------------
//     namespace Lambdas
//     {
//         auto setRenderInEditorTrue  = [](const auto& pSubMeshInstance) { pSubMeshInstance->SetRenderInEditor(true); };
//         auto setRenderInEditorFalse = [](const auto& pSubMeshInstance) { pSubMeshInstance->SetRenderInEditor(false); };
//         auto setRenderInGameTrue    = [](const auto& pSubMeshInstance) { pSubMeshInstance->SetRenderInGame(true); };
//         auto setRenderInGameFalse   = [](const auto& pSubMeshInstance) { pSubMeshInstance->SetRenderInGame(false); };
//     }

    //-----------------------------------------------------------------------------------
    CEntitySlider::CEntitySlider()
        : m_selectedSliders(0)
        , m_snapTranslatorStepSize(CELL_SIZE * 0.5f)
        , m_snapRotatorStepSize(5.0f / 360.0f * 2 * PI)
        , m_snappedRotation(0.0f)
        , m_version(0)
        , m_bTranslatorActivated(false)
        , m_bSnap(true)
        , m_bLocalRotatorTransform(true)
        , m_bLocalTranslatorTransform(false)
        , m_pTranslatorAxes(nullptr)
        , m_pTranslatorPlanes(nullptr)
        , m_pRotator(nullptr)
        , m_pAttached(nullptr)
        , m_pRotatorNode(nullptr)
        , m_pTranslatorNode(nullptr)
        , m_pSliderNode(nullptr)
        , m_mode(TransformMode::MOVE)
        , m_selectedRotator(RotationMode::ROTATOR_NONE)
        , m_vLastSliderPos(Vec3(0.0f, 0.0f, 0.0f))
        , m_vLastMousePos(Vec3(0.0f, 0.0f, 0.0f))
        , m_vSnappedPosition(Vec3(0.0f, 0.0f, 0.0f)) {}

    //-----------------------------------------------------------------------------------
    CEntitySlider::~CEntitySlider() {}

    //-----------------------------------------------------------------------------------
    void CEntitySlider::Reset()
    {
        Detach();

        gEngine->GetSceneMgr()->RemoveMeshInstance(m_pTranslatorPlanes);
        gEngine->GetSceneMgr()->RemoveMeshInstance(m_pTranslatorAxes);
        gEngine->GetSceneMgr()->RemoveMeshInstance(m_pRotator);
        gEngine->GetSceneMgr()->RemoveNode(m_pRotatorNode);
        gEngine->GetSceneMgr()->RemoveNode(m_pTranslatorNode);
        gEngine->GetSceneMgr()->RemoveNode(m_pSliderNode);

        m_pTranslatorPlanes = nullptr;
        m_pTranslatorAxes = nullptr;
        m_pRotator = nullptr;
        m_pRotatorNode = nullptr;
        m_pTranslatorNode = nullptr;
        m_pSliderNode = nullptr;

        // This needs to happen to make sure the mesh containers and its mesh instances 
        // are destroyed before the scene mgr is destroyed; otherwise the pointers will
        // be invalid. It's sad.
        m_rotator.Destroy();
        m_translatorPlanes.Destroy();
        m_translatorAxes.Destroy();

        m_pAttached = nullptr;
    }

    //-----------------------------------------------------------------------------------
    bool CEntitySlider::InitializeAtCreation()
    {
        if (!Initialize())
            return false;

        SetSelectedTranslator(SLIDER_X | SLIDER_Y | SLIDER_Z, false);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CEntitySlider::InitializeAtLoad()
    {
        if (!Initialize())
            return false;

        SetSelectedTranslator(SLIDER_X | SLIDER_Y | SLIDER_Z, false);

        return true;
    }

    //-----------------------------------------------------------------------------------
    bool CEntitySlider::Initialize()
    {
        gEngine->GetSceneMgr()->RemoveMeshInstance(m_pTranslatorAxes);
        gEngine->GetSceneMgr()->RemoveMeshInstance(m_pTranslatorPlanes);
        gEngine->GetSceneMgr()->RemoveMeshInstance(m_pRotator);

        m_pTranslatorAxes = nullptr;
        m_pTranslatorPlanes = nullptr;
        m_pRotator = nullptr;

        m_translatorAxes.InitFromMeshContainer("translatorAxes.x");
        m_translatorPlanes.InitFromMeshContainer("translatorPlanes.x");
        m_rotator.InitFromMeshContainer("rotator.x");

        if (m_pSliderNode == nullptr)
            m_pSliderNode = gEngine->GetSceneMgr()->CreateSceneNode("sliderNode", "rootNode");

        if (m_pTranslatorNode == nullptr)
            m_pTranslatorNode = gEngine->GetSceneMgr()->CreateSceneNode("translatorNode", "sliderNode");

        if (m_pRotatorNode == nullptr)
            m_pRotatorNode = gEngine->GetSceneMgr()->CreateSceneNode("rotatorNode", "sliderNode");

        for (const auto& subMesh : m_translatorAxes.GetMeshInstances())
        {
            subMesh->AttachNode(m_pTranslatorNode);
            subMesh->GetMaterial().SetRenderInEditor(false);
            subMesh->GetMaterial().SetRenderInGame(false);
            subMesh->GetMaterial().SetRenderCategory(MESH_RENDER_CATEGORY_FORWARD);
        }

        for (const auto& subMesh : m_translatorPlanes.GetMeshInstances())
        {
            subMesh->AttachNode(m_pTranslatorNode);
            subMesh->GetMaterial().SetRenderInEditor(false);
            subMesh->GetMaterial().SetRenderInGame(false);
            subMesh->GetMaterial().SetRenderCategory(MESH_RENDER_CATEGORY_FORWARD);
        }

        for (const auto& subMesh : m_rotator.GetMeshInstances())
        {
            subMesh->AttachNode(m_pRotatorNode);
            subMesh->GetMaterial().SetRenderInEditor(false);
            subMesh->GetMaterial().SetRenderInGame(false);
            subMesh->GetMaterial().SetRenderCategory(MESH_RENDER_CATEGORY_FORWARD);
        }

        return true;
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::SetSelectedRotator(RotationMode rotator, bool bSelected)
    {
        m_selectedRotator = rotator;

        switch (m_selectedRotator)
        {
        case RotationMode::ROTATOR_X:
        {
            CMaterial* pMaterial = m_rotator.GetMeshInstance("RotatorX")->GetMaterialPtr();
            if (bSelected)
            {
                pMaterial->SetDiffuseColor(Vec3(1.0f, 1.0f, 0.0f));
            }
            else
            {
                pMaterial->SetDiffuseColor(Vec3(1.0f, 0.0f, 0.0f));
                m_snappedRotation = 0.0;
            }
        }
        break;

        case RotationMode::ROTATOR_Y:
        {
            CMaterial* pMaterial = m_rotator.GetMeshInstance("RotatorY")->GetMaterialPtr();
            if (bSelected)
            {
                pMaterial->SetDiffuseColor(Vec3(1.0f, 1.0f, 0.0f));
            }
            else
            {
                pMaterial->SetDiffuseColor(Vec3(0.0f, 1.0f, 0.0f));
                m_snappedRotation = 0.0;
            }
        }
        break;

        case RotationMode::ROTATOR_Z:
        {
            CMaterial* pMaterial = m_rotator.GetMeshInstance("RotatorZ")->GetMaterialPtr();

            if (bSelected)
            {
                pMaterial->SetDiffuseColor(Vec3(1.0f, 1.0f, 0.0f));
            }
            else
            {
                pMaterial->SetDiffuseColor(Vec3(0.0f, 0.0f, 1.0f));
                m_snappedRotation = 0.0;
            }
        }
        break;

        default:
            break;
        }

        if (!bSelected)
            m_selectedRotator = RotationMode::ROTATOR_NONE;

    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::SetSelectedTranslator(unsigned long sliders, bool bSelected)
    {
        if (bSelected)
        {
            m_selectedSliders |= sliders;

            if ((m_selectedSliders & SLIDER_X) || (m_selectedSliders & SLIDER_XY) || (m_selectedSliders & SLIDER_XZ))
            {
                CMaterial* pMaterial = m_translatorAxes.GetMeshInstance("SliderX")->GetMaterialPtr();
                pMaterial->SetDiffuseColor(Vec3(1.0f, 1.0f, 0.0f));
            }

            if ((m_selectedSliders & SLIDER_Y) || (m_selectedSliders & SLIDER_XY) || (m_selectedSliders & SLIDER_YZ))
            {
                CMaterial* pMaterial = m_translatorAxes.GetMeshInstance("SliderY")->GetMaterialPtr();
                pMaterial->SetDiffuseColor(Vec3(1.0f, 1.0f, 0.0f));
            }

            if ((m_selectedSliders & SLIDER_Z) || (m_selectedSliders & SLIDER_XZ) || (m_selectedSliders & SLIDER_YZ))
            {
                CMaterial* pMaterial = m_translatorAxes.GetMeshInstance("SliderZ")->GetMaterialPtr();
                pMaterial->SetDiffuseColor(Vec3(1.0f, 1.0f, 0.0f));
            }
        }
        else
        {
            if ((sliders & SLIDER_X) || (m_selectedSliders & SLIDER_XY) || (m_selectedSliders & SLIDER_XZ))
            {
                CMaterial* pMaterial = m_translatorAxes.GetMeshInstance("SliderX")->GetMaterialPtr();
                pMaterial->SetDiffuseColor(Vec3(1.0f, 0.0f, 0.0f));

                m_selectedSliders &= !SLIDER_X;
                m_selectedSliders &= !SLIDER_XY;
                m_selectedSliders &= !SLIDER_XZ;
            }

            if ((sliders & SLIDER_Y) || (m_selectedSliders & SLIDER_XY) || (m_selectedSliders & SLIDER_YZ))
            {
                CMaterial* pMaterial = m_translatorAxes.GetMeshInstance("SliderY")->GetMaterialPtr();
                pMaterial->SetDiffuseColor(Vec3(0.0f, 1.0f, 0.0f));

                m_selectedSliders &= !SLIDER_Y;
                m_selectedSliders &= !SLIDER_XY;
                m_selectedSliders &= !SLIDER_YZ;
            }

            if ((sliders & SLIDER_Z) || (m_selectedSliders & SLIDER_XZ) || (m_selectedSliders & SLIDER_YZ))
            {
                CMaterial* pMaterial = m_translatorAxes.GetMeshInstance("SliderZ")->GetMaterialPtr();
                pMaterial->SetDiffuseColor(Vec3(0.0f, 0.0f, 1.0f));

                m_selectedSliders &= !SLIDER_Z;
                m_selectedSliders &= !SLIDER_XZ;
                m_selectedSliders &= !SLIDER_YZ;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::SetMode(TransformMode mode)
    {
        m_mode = mode;

        if (!m_pAttached)
            return;

        if (m_mode == TransformMode::ROTATE)
            ShowSlider(false, true);
        else if (m_mode == TransformMode::MOVE)
            ShowSlider(true, false);
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::Update(float dt, const Vec3& vOrigin, const Vec3& vRay, CCamera* pCam)
    {
        if (m_mode == TransformMode::MOVE)
            UpdateTranslator(dt, vOrigin, vRay, pCam);
        else if (m_mode == TransformMode::ROTATE)
            UpdateRotator(dt, vOrigin, vRay, pCam);
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::UpdateRotator(float dt, const Vec3& vOrigin, const Vec3& vRay, CCamera* pCam)
    {
        if (!m_pAttached || !m_pSliderNode)
            return;

        if (gEngine->GetInput()->IsKeyJustDown('L'))
            m_bLocalRotatorTransform = !m_bLocalRotatorTransform;

        m_pSliderNode->SetOrientation(CQuaternion());

        if (m_bLocalRotatorTransform)
        {
            CQuaternion rotatorOrientation(m_pAttached->GetDerivedDirection(),
                                           m_pAttached->GetDerivedRight(),
                                           m_pAttached->GetDerivedUp());

            m_pRotatorNode->SetOrientation(rotatorOrientation);
        }
        else
        {
            m_pRotatorNode->SetOrientation(CQuaternion());
        }

        m_pSliderNode->SetPosition(m_pAttached->GetDerivedPosition());
        float distance = glm::length(pCam->GetAttachedNode()->GetDerivedPosition() - m_pAttached->GetDerivedPosition());
        float scale = std::max(1.0f, distance / 400.0f);
        m_pRotatorNode->SetScale(Vec3(scale, scale, scale));



        CBoundingVolume pVolume(m_rotator.GetMeshInstance("RotatorY")->GetAttachedSubMesh()->GetBoundingVolume());
        Matrix4x4 mtxScaling = glm::scale(Matrix4x4(1.0f), Vec3(scale, scale, scale));

        pVolume.Transform(mtxScaling);

        Vec3 vMin, vMax;
        pVolume.GetMinMax(vMin, vMax);
        float radius = std::abs(vMax.x - vMin.x) / 2.0f;
        float eps = 10.0f;
        Vec3 vRotatorOrigin = m_pAttached->GetDerivedPosition();

        Matrix4x4 mtxViewInv = glm::inverse(pCam->GetViewMatrix());
        Matrix4x4 mtxViewInvTrans = glm::transpose(mtxViewInv);

        Vec3 vMouseRayOrigin, vMouseRayDir;
        ComputeMouseRay(vMouseRayOrigin, vMouseRayDir, pCam->GetProjectionMatrix(), mtxViewInv);


        if (gEngine->GetInput()->IsButtonJustUp(0))
        {
            SetSelectedRotator(m_selectedRotator, false);
        }

        if (!gEngine->GetInput()->IsButtonDown(0))
        {
            SetSelectedRotator(RotationMode::ROTATOR_X, false);
            SetSelectedRotator(RotationMode::ROTATOR_Y, false);
            SetSelectedRotator(RotationMode::ROTATOR_Z, false);

            Vec3 vRotatorNormals[3];
            vRotatorNormals[0] = m_pRotatorNode->GetDerivedRight();
            vRotatorNormals[1] = m_pRotatorNode->GetDerivedUp();
            vRotatorNormals[2] = m_pRotatorNode->GetDerivedDirection();

            float currentDistance = FLT_MAX;

            for (int i = 0; i < 3; ++i)
            {
                Vec3 vIntersection(0.0f, 0.0f, 0.0f);
                float lineParam = 0.0f;
                if (!ComputeIntersectionRayPlane(vMouseRayOrigin, vMouseRayDir, m_pAttached->GetDerivedPosition(), vRotatorNormals[i], lineParam))
                    continue;

                vIntersection = vMouseRayOrigin + glm::normalize(vMouseRayDir) * lineParam;
                float distance = glm::length(vIntersection - vRotatorOrigin);
                if (std::abs(distance - radius) < eps && distance < currentDistance)
                {
                    for (int k = 0; k < i + 1; ++k)
                        SetSelectedRotator((RotationMode)(k), false);

                    SetSelectedRotator((RotationMode)(i + 1), true);

                    Vec3 v1 = vIntersection - vRotatorOrigin;
                    Vec3 vTangent = glm::cross(vRotatorNormals[i], v1);

                    // Project tangent onto image plane
                    Vec3 vProjectedTangent;
                    Vec3 vNormalImgPlane = pCam->GetAttachedNode()->GetDerivedDirection();
                    vProjectedTangent = glm::cross(vNormalImgPlane, vTangent);
                    vProjectedTangent = glm::cross(vNormalImgPlane, vProjectedTangent);
                    vProjectedTangent = glm::normalize(vProjectedTangent);

                    vProjectedTangent = Vec3(Vec4(vProjectedTangent, 0.0f) * mtxViewInvTrans);

                    Vec2 vProjectedTangent2D = glm::normalize(Vec2(vProjectedTangent.x, vProjectedTangent.y));

                    m_vProjTangent = vProjectedTangent2D;

                    currentDistance = distance;
                }
            }
        }

        if (m_selectedRotator != RotationMode::ROTATOR_NONE && gEngine->GetInput()->IsButtonDown(0))
        {
            Vec2 vAxisX = Vec2(1.0f, 0.0f);
            Vec2 vAxisY = Vec2(0.0f, 1.0f);

            float alpha = std::acos(glm::dot(vAxisX, m_vProjTangent));
            float beta = std::acos(glm::dot(vAxisY, m_vProjTangent));

            float angle = -(gEngine->GetInput()->GetMouseDX() * std::cos(alpha)) + (gEngine->GetInput()->GetMouseDY() * std::cos(beta));
            angle *= dt * 2.0f;

            if (angle != 0.0f)
            {
                switch (m_selectedRotator)
                {
                case RotationMode::ROTATOR_X:
                {
                    if (!m_bSnap)
                    {
                        CQuaternion orientation(m_pAttached->GetOrientation());
                        orientation.RotateAroundAxis(angle, Vec3(1.0f, 0.0f, 0.0f), m_bLocalRotatorTransform);
                        m_pAttached->SetOrientation(orientation);
                    }
                    else
                    {
                        m_snappedRotation += angle;
                        if (abs(m_snappedRotation) > m_snapRotatorStepSize)
                        {
                            float pitch = m_snappedRotation;
                            pitch = (int)(pitch / m_snapRotatorStepSize) * m_snapRotatorStepSize;
                            CQuaternion orientation(m_pAttached->GetOrientation());
                            orientation.RotateAroundAxis(pitch, Vec3(1.0f, 0.0f, 0.0f), m_bLocalRotatorTransform);
                            m_pAttached->SetOrientation(orientation);
                            m_snappedRotation = 0.0;
                        }
                    }
                }
                break;
                case RotationMode::ROTATOR_Y:
                {
                    if (!m_bSnap)
                    {
                        CQuaternion orientation(m_pAttached->GetOrientation());
                        orientation.RotateAroundAxis(angle, Vec3(0.0f, 1.0f, 0.0f), m_bLocalRotatorTransform);
                        m_pAttached->SetOrientation(orientation);
                    }
                    else
                    {
                        m_snappedRotation += angle;
                        if (abs(m_snappedRotation) > m_snapRotatorStepSize)
                        {
                            float yaw = m_snappedRotation;
                            yaw = (int)(yaw / m_snapRotatorStepSize) * m_snapRotatorStepSize;
                            CQuaternion orientation(m_pAttached->GetOrientation());
                            orientation.RotateAroundAxis(yaw, Vec3(0.0f, 1.0f, 0.0f), m_bLocalRotatorTransform);
                            m_pAttached->SetOrientation(orientation);
                            m_snappedRotation = 0.0;
                        }
                    }

                }
                break;
                case RotationMode::ROTATOR_Z:
                {
                    if (!m_bSnap)
                    {
                        CQuaternion orientation(m_pAttached->GetOrientation());
                        orientation.RotateAroundAxis(angle, Vec3(0.0f, 0.0f, 1.0f), m_bLocalRotatorTransform);
                        m_pAttached->SetOrientation(orientation);
                    }
                    else
                    {
                        m_snappedRotation += angle;
                        if (abs(m_snappedRotation) > m_snapRotatorStepSize)
                        {
                            float roll = m_snappedRotation;
                            roll = (int)(roll / m_snapRotatorStepSize) * m_snapRotatorStepSize;
                            CQuaternion orientation(m_pAttached->GetOrientation());
                            orientation.RotateAroundAxis(roll, Vec3(0.0f, 0.0f, 1.0f), m_bLocalRotatorTransform);
                            m_pAttached->SetOrientation(orientation);
                            m_snappedRotation = 0.0;
                        }
                    }

                }
                break;
                default:
                    break;
                }

                if (m_bLocalRotatorTransform)
                {
                    CQuaternion rotatorOrientation(m_pAttached->GetDerivedDirection(),
                                                   m_pAttached->GetDerivedRight(),
                                                   m_pAttached->GetDerivedUp());

                    m_pRotatorNode->SetOrientation(rotatorOrientation);
                }

            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::DeselectSliders()
    {
        m_bTranslatorActivated = false;

        SetSelectedTranslator(SLIDER_X | SLIDER_Y | SLIDER_Z | SLIDER_XY | SLIDER_XZ | SLIDER_YZ, false);

        SetSelectedRotator(RotationMode::ROTATOR_X, false);
        SetSelectedRotator(RotationMode::ROTATOR_Y, false);
        SetSelectedRotator(RotationMode::ROTATOR_Z, false);
        m_selectedRotator = RotationMode::ROTATOR_NONE;
    }

    //-----------------------------------------------------------------------------------
    bool CEntitySlider::IsSliderSelected()
    {
        return (m_selectedRotator != RotationMode::ROTATOR_NONE) || (m_selectedSliders != 0);
    }

    //-----------------------------------------------------------------------------------
    bool CEntitySlider::IsActive()
    {
        return m_pAttached != 0;
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::Attach(CSceneNode* pObject)
    {
        if (IsSliderSelected())
            return;

        Detach();
        if (GetMode() != TransformMode::NONE)
        {
            m_vSnappedPosition = Vec3(0.0, 0.0, 0.0);

            m_pAttached = pObject;
            m_pSliderNode->SetPosition(m_pAttached->GetDerivedPosition());
            m_pTranslatorNode->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pRotatorNode->SetPosition(Vec3(0.0f, 0.0f, 0.0f));

//             if (m_mode == MOVE)
//             {
//                 for (const auto& subMesh : m_translatorAxes.GetSubMeshInstances())
//                     subMesh->SetRenderInEditor(true);
//             }
//             if (m_mode == ROTATE)
//             {
//                 for (const auto& subMesh : m_rotator.GetSubMeshInstances())
//                     subMesh->SetRenderInEditor(true);
//             }

            ShowSlider(m_mode == TransformMode::MOVE,
                       m_mode == TransformMode::ROTATE);

            if (m_pAttached)
            {
                CQuaternion rotatorOrientation(m_pAttached->GetDerivedDirection(),
                                               m_pAttached->GetDerivedRight(),
                                               m_pAttached->GetDerivedUp());

                m_pRotatorNode->SetOrientation(rotatorOrientation);
            }

            m_bTranslatorActivated = true;
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::Rotate(CAttachableObject* pObject)
    {

    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::Detach()
    {
        if (!IsActive())
            return;

        m_pAttached = nullptr;

        ShowSlider(false, false);

//         m_rotator.CallForEach(Lambdas::setRenderInEditorFalse);
//         m_rotator.CallForEach(Lambdas::setRenderInGameFalse);
// 
//         m_translatorAxes.CallForEach(Lambdas::setRenderInEditorFalse);
//         m_translatorAxes.CallForEach(Lambdas::setRenderInGameFalse);
// 
//         m_translatorPlanes.CallForEach(Lambdas::setRenderInEditorFalse);
//         m_translatorPlanes.CallForEach(Lambdas::setRenderInGameFalse);

        DeselectSliders();
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::ShowSlider(bool bShowTranslator, bool bShowRotator)
    {
        for (const auto& meshInst : m_rotator.GetMeshInstances())
            meshInst->GetMaterial().SetRenderInEditor(bShowRotator);
        
        for (const auto& meshInst : m_translatorAxes.GetMeshInstances())
            meshInst->GetMaterial().SetRenderInEditor(bShowTranslator);
        
        for (const auto& meshInst : m_translatorPlanes.GetMeshInstances())
            meshInst->GetMaterial().SetRenderInEditor(bShowTranslator);


//         m_rotator.CallForEach(Lambdas::setRenderInEditorFalse);
//         m_rotator.CallForEach(Lambdas::setRenderInGameFalse);
// 
//         m_translatorAxes.CallForEach(Lambdas::setRenderInEditorFalse);
//         m_translatorAxes.CallForEach(Lambdas::setRenderInGameFalse);
// 
//         m_translatorPlanes.CallForEach(Lambdas::setRenderInEditorFalse);
//         m_translatorPlanes.CallForEach(Lambdas::setRenderInGameFalse);
    }

    //-----------------------------------------------------------------------------------
    CEntitySlider::TransformMode CEntitySlider::GetMode()
    {
        return m_mode;
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::EnableSnap(bool bSnap)
    {
        m_bSnap = bSnap;
    }

    //-----------------------------------------------------------------------------------
    bool CEntitySlider::IsSnapEnabled()
    {
        return m_bSnap;
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::ToggleSnapStepSize()
    {
        if (m_snapTranslatorStepSize == CELL_SIZE)
            m_snapTranslatorStepSize = CELL_SIZE * 0.5f;
        else
            m_snapTranslatorStepSize = CELL_SIZE;
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::SetSnapStepSize(float stepSize)
    {
        m_snapTranslatorStepSize = stepSize;
    }

    //-----------------------------------------------------------------------------------
    float CEntitySlider::GetSnapStepSize()
    {
        return m_snapTranslatorStepSize;
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::UpdateTranslator(float dt, const Vec3& vOrigin, const Vec3& vRay, CCamera* pCam)
    {
        if (!m_pAttached || !m_pSliderNode)
            return;

        if (gEngine->GetInput()->IsKeyJustDown('L'))
            m_bLocalTranslatorTransform = !m_bLocalTranslatorTransform;

        m_pSliderNode->SetPosition(m_pAttached->GetDerivedPosition());

        if (m_bLocalTranslatorTransform)
        {
            m_pSliderNode->SetOrientation(m_pAttached->GetDerivedOrientation());
        }
        else
        {
            m_pSliderNode->SetOrientation(CQuaternion());
        }

        float distance = glm::length(pCam->GetAttachedNode()->GetDerivedPosition() - m_pAttached->GetDerivedPosition());
        float scale = std::max(1.0f, distance / 400.0f);
        m_pTranslatorNode->SetScale(Vec3(scale, scale, scale));


        if (m_bTranslatorActivated)
        {
            m_bTranslatorActivated = false;
        }
        else
        {
            Matrix4x4 mtxView = pCam->GetViewMatrix();
            Matrix4x4 mtxVInv;
            Matrix4x4 mtxVInvTrans;
            mtxVInv = glm::inverse(mtxView);

            if (!gEngine->GetInput()->IsButtonDown(0))
            {
                SetSelectedTranslator(SLIDER_X | SLIDER_Y | SLIDER_Z, false);

                float curDistance = FLT_MAX;
                float tmpDistance = FLT_MAX;

                bool bAxesSelected = false;

                CBoundingVolume pBoundingBoxSliderX(m_translatorAxes.GetMeshInstance("SliderX")->GetAttachedSubMesh()->GetBoundingVolume());

                pBoundingBoxSliderX.Transform(m_pTranslatorNode->GetDerivedRST());
                if (intersection::RayIntersectsBB(pBoundingBoxSliderX, vOrigin, vRay, &tmpDistance) && tmpDistance < curDistance)
                {
                    curDistance = tmpDistance;

                    SetSelectedTranslator(SLIDER_X, true);

                    bAxesSelected = true;
                }

                CBoundingVolume pBoundingBoxSliderY(m_translatorAxes.GetMeshInstance("SliderY")->GetAttachedSubMesh()->GetBoundingVolume());
                pBoundingBoxSliderY.Transform(m_pTranslatorNode->GetDerivedRST());
                if (intersection::RayIntersectsBB(pBoundingBoxSliderY, vOrigin, vRay, &tmpDistance) && tmpDistance < curDistance)
                {
                    curDistance = tmpDistance;

                    SetSelectedTranslator(SLIDER_Y, true);

                    bAxesSelected = true;
                }

                CBoundingVolume pBoundingBoxSliderZ(m_translatorAxes.GetMeshInstance("SliderZ")->GetAttachedSubMesh()->GetBoundingVolume());
                pBoundingBoxSliderZ.Transform(m_pTranslatorNode->GetDerivedRST());
                if (intersection::RayIntersectsBB(pBoundingBoxSliderZ, vOrigin, vRay, &tmpDistance) && tmpDistance < curDistance)
                {
                    curDistance = tmpDistance;

                    SetSelectedTranslator(SLIDER_Z, true);

                    bAxesSelected = true;
                }

                if (!bAxesSelected)
                {
                    CBoundingVolume pBoundingBoxSliderXY(m_translatorPlanes.GetMeshInstance("SliderXY")->GetAttachedSubMesh()->GetBoundingVolume());
                    pBoundingBoxSliderXY.Transform(m_pTranslatorNode->GetDerivedRST());
                    if (intersection::RayIntersectsBB(pBoundingBoxSliderXY, vOrigin, vRay, &tmpDistance) && tmpDistance < curDistance)
                    {
                        curDistance = tmpDistance;

                        SetSelectedTranslator(SLIDER_XY, true);
                    }

                    CBoundingVolume pBoundingBoxSliderXZ(m_translatorPlanes.GetMeshInstance("SliderXZ")->GetAttachedSubMesh()->GetBoundingVolume());
                    pBoundingBoxSliderXZ.Transform(m_pTranslatorNode->GetDerivedRST());
                    if (intersection::RayIntersectsBB(pBoundingBoxSliderXZ, vOrigin, vRay, &tmpDistance) && tmpDistance < curDistance)
                    {
                        curDistance = tmpDistance;

                        SetSelectedTranslator(SLIDER_XZ, true);
                    }

                    CBoundingVolume pBoundingBoxSliderYZ(m_translatorPlanes.GetMeshInstance("SliderYZ")->GetAttachedSubMesh()->GetBoundingVolume());
                    pBoundingBoxSliderYZ.Transform(m_pTranslatorNode->GetDerivedRST());
                    if (intersection::RayIntersectsBB(pBoundingBoxSliderYZ, vOrigin, vRay, &tmpDistance) && tmpDistance < curDistance)
                    {
                        curDistance = tmpDistance;

                        SetSelectedTranslator(SLIDER_YZ, true);
                    }
                }
            }


            if (gEngine->GetInput()->IsButtonJustDown(0))
            {
                m_vLastSliderPos = ComputeMousePosition(m_selectedSliders, pCam->GetProjectionMatrix(), mtxVInv, true, pCam);

                if (m_bLocalTranslatorTransform)
                {
                    CQuaternion q = m_pSliderNode->GetDerivedOrientation();
                    Matrix4x4 r = q.GetRotationMatrix();
                    r = glm::inverse(r);
                    m_vLastSliderPos = Vec3(Vec4(m_vLastSliderPos, 1.0f) * r);
                }
            }

            if (gEngine->GetInput()->IsButtonDown(0))
            {

                Vec3 vTranslation = Vec3(0.0f, 0.0f, 0.0f);

                Vec3 vCurrentMousePos = ComputeMousePosition(m_selectedSliders, pCam->GetProjectionMatrix(), mtxVInv, true, pCam);
                if (m_bLocalTranslatorTransform)
                {
                    CQuaternion q = m_pSliderNode->GetDerivedOrientation();
                    Matrix4x4 r = q.GetRotationMatrix();
                    r = glm::inverse(r);

                    vCurrentMousePos = Vec3(Vec4(vCurrentMousePos, 1.0f) * r);
                }

                if ((m_selectedSliders & SLIDER_X) || (m_selectedSliders & SLIDER_XY) || (m_selectedSliders & SLIDER_XZ))
                {
                    vTranslation.x += vCurrentMousePos.x - m_vLastSliderPos.x;
                }

                if ((m_selectedSliders & SLIDER_Y) || (m_selectedSliders & SLIDER_XY) || (m_selectedSliders & SLIDER_YZ))
                {
                    vTranslation.y += vCurrentMousePos.y - m_vLastSliderPos.y;
                }

                if ((m_selectedSliders & SLIDER_Z) || (m_selectedSliders & SLIDER_XZ) || (m_selectedSliders & SLIDER_YZ))
                {
                    vTranslation.z += vCurrentMousePos.z - m_vLastSliderPos.z;
                }

                if (glm::length(vTranslation) <= 0.1f)
                    return;

                if (!m_bSnap)
                {
                    if (m_bLocalTranslatorTransform)
                    {
                        CQuaternion q = m_pSliderNode->GetDerivedOrientation();
                        Matrix4x4 r = q.GetRotationMatrix();
                        vTranslation = Vec3(Vec4(vTranslation, 1.0f) * r);

                        Vec3 sliderPosition = m_pSliderNode->GetDerivedPosition() + vTranslation;
                        m_pSliderNode->SetDerivedPosition(sliderPosition);
                        m_pAttached->SetDerivedPosition(sliderPosition);
                    }
                    else
                    {
                        Vec3 sliderPosition = m_pSliderNode->GetDerivedPosition() + vTranslation;
                        m_pSliderNode->SetDerivedPosition(sliderPosition);
                        m_pAttached->SetDerivedPosition(sliderPosition);
                    }
                }
                else
                {
                    m_vSnappedPosition += vTranslation;

                    Vec3 translation(0.0f, 0.0f, 0.0f);
                    while (abs(m_vSnappedPosition.x) > m_snapTranslatorStepSize)
                    {
                        if (m_vSnappedPosition.x <= 0.0)
                        {
                            translation.x -= m_snapTranslatorStepSize;
                            m_vSnappedPosition.x = std::min(0.0f, m_vSnappedPosition.x + m_snapTranslatorStepSize);
                        }
                        else
                        {
                            translation.x += m_snapTranslatorStepSize;
                            m_vSnappedPosition.x = std::max(0.0f, m_vSnappedPosition.x - m_snapTranslatorStepSize);
                        }

                    }
                    while (abs(m_vSnappedPosition.y) > m_snapTranslatorStepSize)
                    {
                        if (m_vSnappedPosition.y <= 0.0f)
                        {
                            translation.y -= m_snapTranslatorStepSize;
                            m_vSnappedPosition.y = std::min(0.0f, m_vSnappedPosition.y + m_snapTranslatorStepSize);
                        }
                        else
                        {
                            translation.y += m_snapTranslatorStepSize;
                            m_vSnappedPosition.y = std::max(0.0f, m_vSnappedPosition.y - m_snapTranslatorStepSize);
                        }
                    }
                    translation.z = vTranslation.z;

                    Vec3 sliderPosition = m_pSliderNode->GetDerivedPosition() + translation;
                    sliderPosition.x = ((int)(sliderPosition.x / m_snapTranslatorStepSize)) * m_snapTranslatorStepSize;
                    sliderPosition.y = ((int)(sliderPosition.y / m_snapTranslatorStepSize)) * m_snapTranslatorStepSize;

                    m_pSliderNode->SetDerivedPosition(sliderPosition);
                    m_pAttached->SetDerivedPosition(sliderPosition);
                }

                m_vLastSliderPos = m_vLastSliderPos + vTranslation;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void CEntitySlider::ComputeMouseRay(Vec3& vMouseRayOrigin, Vec3& vMouseRayDir,
                                        const Matrix4x4& mtxProj, const Matrix4x4& mtxViewInv)
    {
        float ScreenWidth = (float)gRenderSystem->GetRenderQualityParams().screenWidth;
        float ScreenHeight = (float)gRenderSystem->GetRenderQualityParams().screenHeight;
        float x = (2.0f * (float)gEngine->GetInput()->GetMouseX() / ScreenWidth - 1.0f) / mtxProj[0][0];
        float y = (-2.0f * (float)gEngine->GetInput()->GetMouseY() / ScreenHeight + 1.0f) / mtxProj[1][1];

        vMouseRayDir = Vec3(x, y, 1.0f);

        Matrix4x4 mtxViewInvTrans = glm::transpose(glm::inverse(mtxViewInv));
        vMouseRayDir = glm::normalize(Vec3(Vec4(vMouseRayDir, 0.0f) * mtxViewInvTrans));
        vMouseRayOrigin = (Vec3(Vec4(0.0f, 0.0f, 0.0f, 1.0f) * mtxViewInv));
    }

    //-----------------------------------------------------------------------------------
    Vec3 CEntitySlider::ComputeMousePosition(unsigned long selectedSlider, const Matrix4x4& mtxProj, const Matrix4x4& mtxViewInv, bool bLocal, CCamera* pCam)
    {
        Vec3 vCurrentMousePos;
        Vec3 vCurrentMouseDir;

        Vec3 right(1.0f, 0.0f, 0.0f);
        Vec3 up(0.0f, 1.0f, 0.0f);
        Vec3 forward(0.0f, 0.0f, 1.0f);

        if (bLocal)
        {
            right = m_pSliderNode->GetDerivedRight();
            up = m_pSliderNode->GetDerivedUp();
            forward = m_pSliderNode->GetDerivedDirection();
        }
        ComputeMouseRay(vCurrentMousePos, vCurrentMouseDir, mtxProj, mtxViewInv);

        Vec3 vIntersection = Vec3(0.0f, 0.0f, 0.0f);
        Vec3 v0 = pCam->GetAttachedNode()->GetDerivedPosition() - m_pTranslatorNode->GetDerivedPosition();

        if ((m_selectedSliders & SLIDER_X) && pCam)
        {
            Vec3 v = glm::cross(v0, m_pTranslatorNode->GetDerivedRight());

            up = glm::normalize(glm::cross(v, m_pTranslatorNode->GetDerivedRight()));
        }
        else if ((m_selectedSliders & SLIDER_Y) && pCam)
        {
            Vec3 v = glm::cross(v0, m_pTranslatorNode->GetDerivedUp());

            forward = glm::normalize(glm::cross(v, m_pTranslatorNode->GetDerivedUp()));
        }
        else if ((m_selectedSliders & SLIDER_Z) && pCam)
        {
            Vec3 v = glm::cross(v0, m_pTranslatorNode->GetDerivedDirection());

            up = glm::normalize(glm::cross(v, m_pTranslatorNode->GetDerivedDirection()));
        }

        if ((m_selectedSliders & SLIDER_X) || (m_selectedSliders & SLIDER_XZ) || (m_selectedSliders & SLIDER_Z))
        {
            float distance = 0.0f;
            if (ComputeIntersectionRayPlane(vCurrentMousePos, vCurrentMouseDir, m_pTranslatorNode->GetDerivedPosition(), up, distance))
                vIntersection = vCurrentMousePos + distance * vCurrentMouseDir;
        }
        else if ((m_selectedSliders & SLIDER_Y) || (m_selectedSliders & SLIDER_XY))
        {
            float distance = 0.0f;
            if (ComputeIntersectionRayPlane(vCurrentMousePos, vCurrentMouseDir, m_pTranslatorNode->GetDerivedPosition(), -forward, distance))
                vIntersection = vCurrentMousePos + distance * vCurrentMouseDir;
        }
        else if (m_selectedSliders & SLIDER_YZ)
        {
            float distance = 0.0f;
            if (ComputeIntersectionRayPlane(vCurrentMousePos, vCurrentMouseDir, m_pTranslatorNode->GetDerivedPosition(), right, distance))
                vIntersection = vCurrentMousePos + distance * vCurrentMouseDir;
        }

        return vIntersection;
    }
}