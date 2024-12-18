#include <Engine/Entity/RackWheelEntity.h>

namespace env
{
    //-----------------------------------------------------------------------------------
    ENTITY_TYPEID CRackWheelEntity::ENTITY_META_TYPEID = ENTITY_TYPEID_RACKWHEEL;

    //-----------------------------------------------------------------------------------
    CRackWheelEntity::CRackWheelEntity()
        : m_pMeshInner(nullptr)
        , m_pMeshFan(nullptr)
        , m_pMeshShield(nullptr)
    {
        SetMetaTypeID(ENTITY_META_TYPEID);

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    CRackWheelEntity::~CRackWheelEntity()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pMeshShield);
        gEngine->GetEntityMgr()->RemoveEntity(m_pMeshFan);
        gEngine->GetEntityMgr()->RemoveEntity(m_pMeshInner);
    }

    //-----------------------------------------------------------------------------------
    void CRackWheelEntity::InitializeAtCreation()
    {
        CMeshEntity::InitializeAtCreation();

        InitMeshes();

        RemovePolygons();
        RemoveCircles();

        AddCircle(CCircle(130.0f));

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CRackWheelEntity::SetupSounds()
    {
        if (m_pSound == nullptr)
        {
            m_pSound = static_cast<CSoundSpriteEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_SOUNDSPRITE));
            m_pSound->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_pSound->SetDeletable(false);
            m_pSound->SetAutoDelete(false);
            m_pNode->AttachChildNode(m_pSound->GetNode());
        }

        float scale = m_pNode->GetDerivedScale().x;

        m_pSound->SetVolume(0.31f);
        m_pSound->LoadSound("Entities/RackWheel.wav");
        m_pSound->SetAudibleDistance(300 * scale);
        m_pSound->SetPeakVolumeDistance(200 * scale);
    }

    //-----------------------------------------------------------------------------------
    void CRackWheelEntity::InitializeAtLoad()
    {
        CMeshEntity::InitializeAtLoad();

        InitMeshes();

        m_bGhost = false;
    }

    //-----------------------------------------------------------------------------------
    void CRackWheelEntity::OnEntityCloned(CEntity& entity)
    {
        CMeshEntity::OnEntityCloned(entity);

//         CRackWheelEntity& other = static_cast<CRackWheelEntity&>(entity);
// 
//         if (m_pMeshInner)
//             m_pMeshInner->OnEntityCloned(*other.m_pMeshInner);
// 
//         if (m_pMeshFan)
//             m_pMeshFan->OnEntityCloned(*other.m_pMeshFan);
// 
//         if (m_pMeshShield)
//             m_pMeshShield->OnEntityCloned(*other.m_pMeshShield);
    }

    //-----------------------------------------------------------------------------------
//     void CRackWheelEntity::SetRotationSpeedZ(float rot)
//     {
//         CMeshEntity::SetRotationSpeedZ(rot);
// 
//         if (m_pMeshInner)
//             m_pMeshInner->SetRotationSpeedZ(GetRotationSpeedZ());
// 
//         if (m_pMeshFan)
//             m_pMeshFan->SetRotationSpeedZ(GetRotationSpeedZ() * 2.5f);
// 
//         if (m_pMeshShield)
//             m_pMeshShield->SetRotationSpeedZ(GetRotationSpeedZ());
//     }

    //-----------------------------------------------------------------------------------
    void CRackWheelEntity::InitMeshes()
    {
        gEngine->GetEntityMgr()->RemoveEntity(m_pMeshInner);
        gEngine->GetEntityMgr()->RemoveEntity(m_pMeshFan);
        gEngine->GetEntityMgr()->RemoveEntity(m_pMeshShield);

        m_pMeshInner = nullptr;
        m_pMeshFan = nullptr;
        m_pMeshShield = nullptr;

//         if (m_pMeshInner == nullptr)
//         {
//             m_pMeshInner = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
//             m_pMeshInner->SetAutoDelete(false);
//             m_pMeshInner->SetDeletable(false);
//             m_pMeshInner->SetSlidable(false);
//             m_pMeshInner->SetMeshFilename("RackWheelInner.x");
//             m_pMeshInner->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetDiffuseColor(UndoGamma(RGBByte2Float(219, 223, 255))); });
//             m_pMeshInner->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEmissiveColor(UndoGamma(RGBByte2Float(3, 209, 255))); });
//             m_pMeshInner->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEnvironmentColor(UndoGamma(RGBByte2Float(15, 151, 255))); });
//             m_pMeshInner->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetDiffuseEnergy(1.0f); });
//             m_pMeshInner->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEmissiveEnergy(5.5f); });
//             m_pMeshInner->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEnvironmentReflectivity(0.3f); });
//             m_pMeshInner->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -15.0f));
//             m_pMeshInner->GetNode()->SetScale(Vec3(0.85f, 0.85f, 0.85f));
//             m_pMeshInner->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, DegToRad(20.0f)));
//             m_pNode->AttachChildNode(m_pMeshInner->GetNode());
//         }
// 
//         if (m_pMeshFan == nullptr)
//         {
//             m_pMeshFan = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
//             m_pMeshFan->SetAutoDelete(false);
//             m_pMeshFan->SetDeletable(false);
//             m_pMeshFan->SetSlidable(false);
//             m_pMeshFan->SetMeshFilename("fan01.x");
//             m_pMeshFan->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetDiffuseColor(UndoGamma(RGBByte2Float(42, 132, 15))); });
//             m_pMeshFan->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEmissiveColor(UndoGamma(RGBByte2Float(15, 151, 255))); });
//             m_pMeshFan->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEnvironmentColor(UndoGamma(RGBByte2Float(15, 151, 255))); });
//             m_pMeshFan->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetDiffuseEnergy(3.0f); });
//             m_pMeshFan->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEmissiveEnergy(1.0f); });
//             m_pMeshFan->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEnvironmentReflectivity(0.0f); });
//             m_pMeshFan->GetNode()->SetPosition(Vec3(0.0f, 0.0f, 16.0f));
//             m_pNode->AttachChildNode(m_pMeshFan->GetNode());
//         }
// 
//         if (m_pMeshShield == nullptr)
//         {
//             m_pMeshShield = static_cast<CMeshEntity*>(gEngine->GetEntityMgr()->CreateEntity(ENTITY_TYPEID_MESH));
//             m_pMeshShield->SetAutoDelete(false);
//             m_pMeshShield->SetDeletable(false);
//             m_pMeshShield->SetSlidable(false);
//             m_pMeshShield->SetMeshFilename("RackWheelShield.x");
//             m_pMeshShield->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetDiffuseColor(UndoGamma(RGBByte2Float(34, 255, 19))); });
//             m_pMeshShield->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetDiffuseEnergy(1.0f); });
//             m_pMeshShield->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEmissiveEnergy(0.0f); });
//             m_pMeshShield->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetEnvironmentReflectivity(0.0f); });
//             m_pMeshShield->GetMeshInstanceContainer().CallForEach([](const auto& subMeshInst) { subMeshInst->SetRenderCategory(MESH_RENDER_CATEGORY_TRANSMISSIVE); });
//             m_pMeshShield->GetNode()->SetPosition(Vec3(0.0f, 0.0f, -11.0f));
//             m_pMeshShield->GetNode()->SetScale(Vec3(0.85f, 0.85f, 0.85f));
//             m_pMeshShield->GetNode()->SetOrientation(CQuaternion(0.0f, 0.0f, DegToRad(20.0f)));
//             m_pNode->AttachChildNode(m_pMeshShield->GetNode());
//         }

    }

    //-----------------------------------------------------------------------------------
    void CRackWheelEntity::SetMeshFilename(const std::string& strName)
    {
        RemoveCircles();
        RemovePolygons();

        if (strName == "RackWheelBig.x")
        {
            Vec3 p1(-14.0f, 158.0f, 0.0f);
            Vec3 p2(-36.0f, 386.0f, 0.0f);
            Vec3 p3(32.0f, 324.0f, 0.0f);
            Vec3 p4(15.0f, 158.0f, 0.0f);

            for (int i = 0; i < 8; ++i)
            {
                Matrix4x4 mtxScaling = glm::scale(Matrix4x4(1.0f), Vec3(0.7f, 0.96f, 1.0f));

                Matrix4x4 mtxRot = glm::rotate(Matrix4x4(1.0f), i * (2 * PI / 8.0f), Vec3(0.0f, 0.0f, 1.0f));

                Vec3 p1Transformed(0.0f, 0.0f, 0.0f), p2Transformed(0.0f, 0.0f, 0.0f), p3Transformed(0.0f, 0.0f, 0.0f), p4Transformed(0.0f, 0.0f, 0.0f);
                p1Transformed = Vec3(Vec4(p1, 1.0f) * (mtxScaling * mtxRot));
                p2Transformed = Vec3(Vec4(p2, 1.0f) * (mtxScaling * mtxRot));
                p3Transformed = Vec3(Vec4(p3, 1.0f) * (mtxScaling * mtxRot));
                p4Transformed = Vec3(Vec4(p4, 1.0f) * (mtxScaling * mtxRot));

                CPolygon p;
                p.SetNumPoints(4);
                p.AddPoint(p1Transformed);
                p.AddPoint(p2Transformed);
                p.AddPoint(p3Transformed);
                p.AddPoint(p4Transformed);
                AddPolygon(p);
            }

            CCircle c;
            c.SetRadius(170.0f);
            AddCircle(c);
        }
        else
        {
            CCircle c;
            c.SetRadius(130.0f);
            AddCircle(c);
        }

        CMeshEntity::SetMeshFilename(strName);
    }

    //-----------------------------------------------------------------------------------
    void CRackWheelEntity::Update(float dt)
    {
        CMeshEntity::Update(dt);
        auto meshInstances = GetMeshInstanceContainer().GetMeshInstances();
        GetMeshInstanceContainer().CallForEach([](CMeshInstance* pSubMeshInst) { 
            if (pSubMeshInst->GetMaterial().GetRenderCategoryMask() & RenderCategory2Bitmask(MESH_RENDER_CATEGORY_DEFERRED))
            {
                pSubMeshInst->GetMaterial().RemoveRenderCategory(MESH_RENDER_CATEGORY_DEFERRED);
                pSubMeshInst->GetMaterial().AddRenderCategory(MESH_RENDER_CATEGORY_DEFERRED_LAYER0);
            }
        });

        ApplyTransform(GetMeshInstanceNode(0)->GetDerivedRST(), m_pNode->GetDerivedScale().x);
    }

} // env