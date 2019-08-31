/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

#include <QEditor/UIGen/TerrainPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Entity/MeshEntity.h>
#define UNICODE 

class CTerrainPanel : public CPanel<Ui::TerrainPanel>
{
    Q_OBJECT

public:
    CTerrainPanel(QWidget* pForm)
        : CPanel<Ui::TerrainPanel>(pForm)
    {
        connect(m_ui.pBtnEmissiveColor, SIGNAL(pressed()), this, SLOT(slotChooseEmissiveColor()));
        connect(m_ui.pSpinBoxEmissiveEnergy, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pBtnEnvColor, SIGNAL(pressed()), this, SLOT(slotChooseEnvColor()));
        connect(m_ui.pSpinBoxEnvReflectivity, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pBtnChooseMaterial, SIGNAL(pressed()), this, SLOT(slotChooseMaterial()));

    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        env::CTerrainMgrCPU& levelMgrCPU = env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid();

        env::Vec4 emissiveMtrl = levelMgrCPU.GetMaterial().GetEmissiveMtrl();
        env::Vec4 envMaterial = levelMgrCPU.GetMaterial().GetEnvMtrl();

        SetBtnColor(m_ui.pBtnEmissiveColor, env::DoGamma(env::Vec3(emissiveMtrl)));
        SetBtnColor(m_ui.pBtnEnvColor, env::DoGamma(env::Vec3(envMaterial)));

        m_ui.pSpinBoxEmissiveEnergy->setValue(emissiveMtrl.a);
        m_ui.pSpinBoxEnvReflectivity->setValue(envMaterial.a);

        UpdateUI();
    }

public slots:

    //-----------------------------------------------------------------------------------
    void UpdateUI()
    {
        env::CTerrainMgrCPU&   levelMgrCPU = env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid();
        env::CManagedMaterial* pMtrl = levelMgrCPU.GetMaterial().GetMaterialPtr();

        if (!pMtrl)
            return;

        m_ui.pBtnChooseMaterial->setText(pMtrl->GetName().c_str());
        m_ui.pLineEditTexture->setText(pMtrl->GetTexture() ? pMtrl->GetTexture()->GetName().c_str() : "<None>");
        m_ui.pLineEditNormalMap->setText(pMtrl->GetNormalMap() ? pMtrl->GetNormalMap()->GetName().c_str() : "<None>");
        m_ui.pLineEditEmissiveMap->setText(pMtrl->GetEmissiveMap() ? pMtrl->GetEmissiveMap()->GetName().c_str() : "<None>");
        m_ui.pLineEditSpecularityMap->setText(pMtrl->GetSpecularMap() ? pMtrl->GetSpecularMap()->GetName().c_str() : "<None>");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseMaterial()
    {
        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Choose Material", "../Resources/Textures/Terrain", "Material (*.mtrl)");

        QFileInfo fi(strFilename);

        env::CTerrainMgrCPU& levelMgrCPU = env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid();

        levelMgrCPU.SetMaterial("Terrain/" + fi.fileName().toStdString());
        levelMgrCPU.GetMaterial().UpdateMaterial();

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotSettingsChanged()
    {
        env::CTerrainMgrCPU& levelMgrCPU = env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid();

        if (m_ui.pSpinBoxEmissiveEnergy->hasFocus())
        {
            env::Vec4 emissive = levelMgrCPU.GetMaterial().GetEmissiveMtrl();
            emissive.a = (float)m_ui.pSpinBoxEmissiveEnergy->value();
            levelMgrCPU.GetMaterial().SetEmissiveMtrl(emissive);
            levelMgrCPU.GetMaterial().UpdateMaterial();
        }

        if (m_ui.pSpinBoxEnvReflectivity->hasFocus())
        {
            env::Vec4 envMaterial = levelMgrCPU.GetMaterial().GetEnvMtrl();
            envMaterial.a = (float)m_ui.pSpinBoxEnvReflectivity->value();
            levelMgrCPU.GetMaterial().SetEnvMtrl(envMaterial);
            levelMgrCPU.GetMaterial().UpdateMaterial();
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void slotEmissiveColorChanged(const QColor& color)
    {
        env::CTerrainMgrCPU& levelMgrCPU = env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid();

        float emissiveEnergy = levelMgrCPU.GetMaterial().GetEmissiveMtrl().a;

        env::Vec3 emissive = FromQColor(color);

        env::UndoGamma(emissive);

        levelMgrCPU.GetMaterial().SetEmissiveMtrl(env::Vec4(emissive, emissiveEnergy));
        levelMgrCPU.GetMaterial().UpdateMaterial();

        SetBtnColor(m_ui.pBtnEmissiveColor, emissive);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseEmissiveColor()
    {
        env::CTerrainMgrCPU& levelMgrCPU = env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid();

        env::Vec3 emissiveColor = env::Vec3(levelMgrCPU.GetMaterial().GetEmissiveMtrl());

        env::DoGamma(emissiveColor);

        gEditor->AcquireColorDialog(this, SLOT(slotEmissiveColorChanged(const QColor&)), emissiveColor, "Choose Emissive Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotEnvColorChanged(const QColor& color)
    {
        env::CTerrainMgrCPU& levelMgrCPU = env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid();

        float envReflectivity = levelMgrCPU.GetMaterial().GetEnvMtrl().a;

        env::Vec3 envColor = FromQColor(color);

        env::UndoGamma(envColor);

        levelMgrCPU.GetMaterial().SetEnvMtrl(env::Vec4(envColor, envReflectivity));
        levelMgrCPU.GetMaterial().UpdateMaterial();

        SetBtnColor(m_ui.pBtnEnvColor, envColor);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseEnvColor()
    {
        env::CTerrainMgrCPU& levelMgrCPU = env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid();

        env::Vec3 envColor = env::Vec3(levelMgrCPU.GetMaterial().GetEnvMtrl());

        env::DoGamma(envColor);

        gEditor->AcquireColorDialog(this, SLOT(slotEnvColorChanged(const QColor&)), envColor, "Choose Environment Color");
    }
};