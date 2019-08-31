/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */
#pragma once

#include <QEditor/UIGen/MeshEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Entity/MeshEntity.h>
#define UNICODE 

class CMeshEntityPanel : public CPanel<Ui::MeshEntityPanel>
{
    Q_OBJECT

public:
    CMeshEntityPanel(QWidget* pForm)
        : CPanel<Ui::MeshEntityPanel>(pForm)
        , m_pEntity(nullptr)
    {
        connect(m_ui.pSpinBoxRotationSpeedX, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxRotationSpeedY, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxRotationSpeedZ, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxDiffuseEnergy, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxEmissiveEnergy, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxEnvReflectivity, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxMotionBlurIntensity, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));

        connect(m_ui.pCheckBoxRenderInGame, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxRenderInEditor, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxDirectionalLighting, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxMotionBlur, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxCategorySolid, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxCategoryTransmissive, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxCategoryOutline, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
//        connect(m_ui.pCheckBoxMaterialTemplate, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxMeshTemplate, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));

        connect(m_ui.pBtnChooseMesh, SIGNAL(pressed()), this, SLOT(slotChooseMesh()));
        connect(m_ui.pBtnReplaceMeshes, SIGNAL(pressed()), this, SLOT(slotReplaceMeshes()));
        connect(m_ui.pBtnDiffuseColor, SIGNAL(pressed()), this, SLOT(slotChooseDiffuseColor()));
        connect(m_ui.pBtnEmissiveColor, SIGNAL(pressed()), this, SLOT(slotChooseEmissiveColor()));
        connect(m_ui.pBtnEnvColor, SIGNAL(pressed()), this, SLOT(slotChooseEnvironmentColor()));
        connect(m_ui.pBtnCopyColors, SIGNAL(pressed()), this, SLOT(CopyEditColors()));
        connect(m_ui.pBtnPasteColors, SIGNAL(pressed()), this, SLOT(PasteEditColors()));
//         connect(m_ui.pBtnSaveTemplate, SIGNAL(pressed()), this, SLOT(SaveToTemplate()));
//         connect(m_ui.pBtnLoadTemplate, SIGNAL(pressed()), this, SLOT(LoadFromTemplate()));
        connect(m_ui.pBtnSaveMeshTemplate, SIGNAL(pressed()), this, SLOT(SaveToMeshTemplate()));
        connect(m_ui.pBtnLoadMeshTemplate, SIGNAL(pressed()), this, SLOT(LoadFromMeshTemplate()));
        connect(m_ui.pBtnApplyTemplateToAll, SIGNAL(pressed()), this, SLOT(ApplyTemplateToAll()));
        

        connect(m_ui.pComboMeshInstance, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotMeshInstanceChanged(const QString &)));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CMeshEntity*>(pEntity);
        
        m_ui.pComboMeshInstance->clear();

        const env::CMeshInstanceContainer& meshInstanceContainer = m_pEntity->GetMeshInstanceContainer();

        for (const auto& meshInstance : meshInstanceContainer.GetMeshInstances())
        {
            m_ui.pComboMeshInstance->addItem(meshInstance->GetAttachedSubMeshName().c_str());
        }

        m_ui.pComboMeshInstance->setCurrentIndex(0);

        UpdateUI();
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        env::CMeshInstance* pMeshInst = GetCurrentInstance();

        if (m_ui.pSpinBoxRotationSpeedX->hasFocus())
            pMeshInst->SetRotationSpeedX((float)m_ui.pSpinBoxRotationSpeedX->value());

        if (m_ui.pSpinBoxRotationSpeedY->hasFocus())
            pMeshInst->SetRotationSpeedY((float)m_ui.pSpinBoxRotationSpeedY->value());

        if (m_ui.pSpinBoxRotationSpeedZ->hasFocus())
            pMeshInst->SetRotationSpeedZ((float)m_ui.pSpinBoxRotationSpeedZ->value());

        if (m_ui.pSpinBoxDiffuseEnergy->hasFocus())
            pMeshInst->GetMaterial().SetDiffuseEnergy((float)m_ui.pSpinBoxDiffuseEnergy->value());

        if (m_ui.pSpinBoxEmissiveEnergy->hasFocus())
            pMeshInst->GetMaterial().SetEmissiveEnergy((float)m_ui.pSpinBoxEmissiveEnergy->value());
        
        if (m_ui.pSpinBoxEnvReflectivity->hasFocus())
            pMeshInst->GetMaterial().SetEnvReflectivity((float)m_ui.pSpinBoxEnvReflectivity->value());

        if (m_ui.pSpinBoxMotionBlurIntensity->hasFocus())
            pMeshInst->GetMaterial().SetMotionBlurScaling((float)m_ui.pSpinBoxMotionBlurIntensity->value());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotMeshInstanceChanged(const QString&)
    {
        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotCheckBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        env::CMeshInstance* pMeshInst = GetCurrentInstance();

        pMeshInst->GetMaterial().SetRenderInGame(m_ui.pCheckBoxRenderInGame->isChecked());
        pMeshInst->GetMaterial().SetRenderInEditor(m_ui.pCheckBoxRenderInEditor->isChecked());
        pMeshInst->GetMaterial().SetDirectionalLighting(m_ui.pCheckBoxDirectionalLighting->isChecked());
        pMeshInst->GetMaterial().SetMotionBlur(m_ui.pCheckBoxMotionBlur->isChecked());
        
        if (m_ui.pCheckBoxCategorySolid->isChecked())
            pMeshInst->GetMaterial().AddRenderCategory(env::MESH_RENDER_CATEGORY_DEFERRED);
        else
            pMeshInst->GetMaterial().RemoveRenderCategory(env::MESH_RENDER_CATEGORY_DEFERRED);

        if (m_ui.pCheckBoxCategoryTransmissive->isChecked())
            pMeshInst->GetMaterial().AddRenderCategory(env::MESH_RENDER_CATEGORY_TRANSMISSIVE);
        else
            pMeshInst->GetMaterial().RemoveRenderCategory(env::MESH_RENDER_CATEGORY_TRANSMISSIVE);

//        pMeshInst->GetMaterial().SetMaterialTemplateInit(m_ui.pCheckBoxMaterialTemplate->isChecked());
        
        m_pEntity->SetInitFromMeshTemplate(m_ui.pCheckBoxMeshTemplate->isChecked());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotReplaceMeshes()
    {
        if (!m_pEntity)
            return;

        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Replaces Meshes", "../Resources/Meshes", "Meshes (*.*)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        std::string strMesh = m_pEntity->GetMeshInstanceContainer().GetMeshContainer();

        std::list<env::CEntity*> meshEntities;
        env::gEngine->GetEntityMgr()->GetEntities<env::CMeshEntity>(meshEntities);

        for (auto& meshEntity : meshEntities)
        {
            env::CMeshEntity* pMeshEntity = dynamic_cast<env::CMeshEntity*>(meshEntity);

            if (pMeshEntity == nullptr ||
                pMeshEntity->GetMeshInstanceContainer().GetMeshContainer() != strMesh)
            {
                continue;
            }

            pMeshEntity->SetMeshFilename(fi.fileName().toStdString());
            pMeshEntity->SetInitFromMeshTemplate(false);
            pMeshEntity->SetMeshTemplate(std::string());
        }

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseMesh()
    {
        if (!m_pEntity)
            return;

        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Load Mesh", "../Resources/Meshes", "Meshes (*.*)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        m_pEntity->SetMeshFilename(fi.fileName().toStdString());
        m_pEntity->SetInitFromMeshTemplate(false);
        m_pEntity->SetMeshTemplate(std::string());

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseDiffuseColor()
    {
        if (!m_pEntity)
            return;

        env::Vec3 diffuseColor = GetCurrentInstance()->GetMaterial().GetDiffuseColor();

        env::DoGamma(diffuseColor);

        gEditor->AcquireColorDialog(this, SLOT(slotDiffuseColorChanged(const QColor&)), diffuseColor, "Choose Diffuse Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotDiffuseColorChanged(const QColor& color)
    {
        env::Vec3 diffuseColor = FromQColor(color);

        GetCurrentInstance()->GetMaterial().SetDiffuseColor(env::UndoGamma(diffuseColor));

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseEmissiveColor()
    {
        if (!m_pEntity)
            return;

        env::Vec3 emissiveColor = GetCurrentInstance()->GetMaterial().GetEmissiveColor();

        env::DoGamma(emissiveColor);

        gEditor->AcquireColorDialog(this, SLOT(slotEmissiveColorChanged(const QColor&)), emissiveColor, "Choose Emissive Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotEmissiveColorChanged(const QColor& color)
    {
        env::Vec3 emissiveColor = FromQColor(color);

        GetCurrentInstance()->GetMaterial().SetEmissiveColor(env::UndoGamma(emissiveColor));

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseEnvironmentColor()
    {
        if (!m_pEntity)
            return;

        env::Vec3 envColor = GetCurrentInstance()->GetMaterial().GetEnvColor();

        env::DoGamma(envColor);

        gEditor->AcquireColorDialog(this, SLOT(slotEnvironmentColorChanged(const QColor&)), envColor, "Choose Environment Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotEnvironmentColorChanged(const QColor& color)
    {
        env::Vec3 envColor = FromQColor(color);

        GetCurrentInstance()->GetMaterial().SetEnvColor(env::UndoGamma(envColor));

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void CopyEditColors()
    {
        env::CMeshInstance* pMeshInst = GetCurrentInstance();

        m_editEmissiveColor  = pMeshInst->GetMaterial().GetEmissiveColor();
        m_editDiffuseColor   = pMeshInst->GetMaterial().GetDiffuseColor();
        m_editEnvColor       = pMeshInst->GetMaterial().GetEnvColor();
        m_editEmissiveEnergy = pMeshInst->GetMaterial().GetEmissiveEnergy();
        m_editDiffuseEnergy  = pMeshInst->GetMaterial().GetDiffuseEnergy();
        m_editEnvReflectivity = pMeshInst->GetMaterial().GetEnvReflectivity();
    }

    //-----------------------------------------------------------------------------------
    virtual void PasteEditColors()
    {
        env::CMeshInstance* pMeshInst = GetCurrentInstance();

        pMeshInst->GetMaterial().SetEmissiveColor(m_editEmissiveColor);
        pMeshInst->GetMaterial().SetEmissiveEnergy(m_editEmissiveEnergy);
        pMeshInst->GetMaterial().SetEnvColor(m_editEnvColor);
        pMeshInst->GetMaterial().SetDiffuseColor(m_editDiffuseColor);
        pMeshInst->GetMaterial().SetDiffuseEnergy(m_editDiffuseEnergy);
        pMeshInst->GetMaterial().SetEnvReflectivity(m_editEnvReflectivity);

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    void UpdateUI()
    {
        if (m_ui.pComboMeshInstance->count() <= 0)
            return;

        env::CMeshInstance* pMeshInst = GetCurrentInstance();

        env::Vec3 emissiveColor = pMeshInst->GetMaterial().GetEmissiveColor();
        env::Vec3 diffuseColor  = pMeshInst->GetMaterial().GetDiffuseColor();
        env::Vec3 envColor      = pMeshInst->GetMaterial().GetEnvColor();

        SetBtnColor(m_ui.pBtnEmissiveColor, env::DoGamma(emissiveColor));
        SetBtnColor(m_ui.pBtnDiffuseColor, env::DoGamma(diffuseColor));
        SetBtnColor(m_ui.pBtnEnvColor, env::DoGamma(envColor));

        m_ui.pSpinBoxRotationSpeedX->setValue((double)pMeshInst->GetRotationSpeedX());
        m_ui.pSpinBoxRotationSpeedY->setValue((double)pMeshInst->GetRotationSpeedY());
        m_ui.pSpinBoxRotationSpeedZ->setValue((double)pMeshInst->GetRotationSpeedZ());
        m_ui.pSpinBoxDiffuseEnergy->setValue((double)pMeshInst->GetMaterial().GetDiffuseEnergy());
        m_ui.pSpinBoxEmissiveEnergy->setValue((double)pMeshInst->GetMaterial().GetEmissiveEnergy());
        m_ui.pSpinBoxEnvReflectivity->setValue(pMeshInst->GetMaterial().GetEnvReflectivity());
        m_ui.pSpinBoxMotionBlurIntensity->setValue((double)pMeshInst->GetMaterial().GetMotionBlurScaling());

        m_ui.pCheckBoxRenderInGame->setChecked(pMeshInst->GetMaterial().GetRenderInGame());
        m_ui.pCheckBoxRenderInEditor->setChecked(pMeshInst->GetMaterial().GetRenderInEditor());
        m_ui.pCheckBoxDirectionalLighting->setChecked(pMeshInst->GetMaterial().GetDirectionalLighting());
        m_ui.pCheckBoxMotionBlur->setChecked(pMeshInst->GetMaterial().GetMotionBlur());

        m_ui.pCheckBoxCategorySolid->setChecked(pMeshInst->GetMaterial().IsRenderCategorySet(env::MESH_RENDER_CATEGORY_DEFERRED));
        m_ui.pCheckBoxCategoryTransmissive->setChecked(pMeshInst->GetMaterial().IsRenderCategorySet(env::MESH_RENDER_CATEGORY_TRANSMISSIVE));
        m_ui.pCheckBoxCategoryOutline->setChecked(pMeshInst->GetMaterial().IsRenderCategorySet(env::MESH_RENDER_CATEGORY_OUTLINE));

        m_ui.pBtnChooseMesh->setText(pMeshInst->GetAttachedMeshContainerName().c_str());

//         if (!pMeshInst->GetMaterial().GetMaterialTemplate().empty())
//             m_ui.pBtnLoadTemplate->setText(pMeshInst->GetMaterial().GetMaterialTemplate().c_str());
//         else
//             m_ui.pBtnLoadTemplate->setText("Load from material template");

        if (!m_pEntity->GetMeshTemplate().empty())
            m_ui.pBtnLoadMeshTemplate->setText(m_pEntity->GetMeshTemplate().c_str());
        else
            m_ui.pBtnLoadMeshTemplate->setText("Load mesh template");

//        m_ui.pCheckBoxMaterialTemplate->setChecked(pMeshInst->GetMaterial().GetMaterialTemplateInit());
        m_ui.pCheckBoxMeshTemplate->setChecked(m_pEntity->GetInitFromMeshTemplate());

        m_pEntity->GetMeshInstanceContainer().CallForEach([](env::CMeshInstance* p) { p->GetMaterial().SetOutlineColor(env::Vec3(0.0f, 1.0f, 0.0f)); });
        
        pMeshInst->GetMaterial().SetOutlineColor(env::Vec3(1.0f, 0.0f, 0.0f));
    }

    //-----------------------------------------------------------------------------------
//     virtual void SaveToTemplate()
//     {
//         env::CMeshInstance* pMeshInst = GetCurrentInstance();
// 
//         QString strFilename = QFileDialog::getSaveFileName(m_ui.qEditorPanelBox, "Save Material Template", "../Resources/Textures/Materials", "Materials (*.mtrl)");
// 
//         if (strFilename.isEmpty())
//             return;
// 
//         QFileInfo fi(strFilename);
// 
//         pMeshInst->GetMaterial().SaveTemplate(strFilename.toStdString(), "Materials/" + fi.fileName().toStdString());
// 
//         UpdateUI();
//     }

    //-----------------------------------------------------------------------------------
//     virtual void LoadFromTemplate()
//     {
//         env::CMeshInstance* pMeshInst = GetCurrentInstance();
// 
//         QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Save Material Template", "../Resources/Textures/Materials", "Materials (*.mtrl)");
// 
//         if (strFilename.isEmpty())
//             return;
// 
//         QFileInfo fi(strFilename);
// 
//         pMeshInst->GetMaterial().LoadTemplate("Materials/" + fi.fileName().toStdString());
// 
//         UpdateUI();
//     }

    //-----------------------------------------------------------------------------------
    virtual void SaveToMeshTemplate()
    {
        env::CMeshInstance* pMeshInst = GetCurrentInstance();

        QString strFilename = QFileDialog::getSaveFileName(m_ui.qEditorPanelBox, "Save Mesh Template", "../Resources/Meshes/Templates", "Mesh Templates (*.mesh)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        m_pEntity->SaveTemplate(strFilename.toStdString(), "../Meshes/Templates/" + fi.fileName().toStdString());

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void ApplyTemplateToAll()
    {
        std::list<env::CEntity*> meshEntities;
        env::gEngine->GetEntityMgr()->GetEntities<env::CMeshEntity>(meshEntities);

        const std::string& strMeshTemplate = m_pEntity->GetMeshTemplate();
        const std::string& strMeshContainer = m_pEntity->GetMeshInstanceContainer().GetMeshContainer();

        for (auto& meshEntity : meshEntities)
        {
            env::CMeshEntity* pMeshEntity = dynamic_cast<env::CMeshEntity*>(meshEntity);

            if (pMeshEntity == nullptr ||
                pMeshEntity == m_pEntity || 
                pMeshEntity->GetMeshTemplate() == strMeshTemplate || 
                pMeshEntity->GetMeshInstanceContainer().GetMeshContainer() != strMeshContainer)
            {
                continue;
            }

            pMeshEntity->LoadTemplate(strMeshTemplate);
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void LoadFromMeshTemplate()
    {
        env::CMeshInstance* pMeshInst = GetCurrentInstance();

        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Save Mesh Template", "../Resources/Meshes/Templates", "Mesh Templates (*.mesh)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        m_pEntity->LoadTemplate("../Meshes/Templates/" + fi.fileName().toStdString());

        UpdateUI();
    }

private:

    //-----------------------------------------------------------------------------------
    env::CMeshInstance* GetCurrentInstance()
    {
        return m_pEntity->GetMeshInstanceContainer().GetMeshInstance(m_ui.pComboMeshInstance->currentIndex());
    }

    static env::Vec3 m_editEmissiveColor;
    static env::Vec3 m_editDiffuseColor;
    static env::Vec3 m_editEnvColor;

    static float m_editEmissiveEnergy;
    static float m_editDiffuseEnergy;
    static float m_editEnvReflectivity;

    env::CMeshEntity* m_pEntity;

};