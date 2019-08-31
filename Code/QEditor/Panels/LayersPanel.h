/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/LayersPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Entity/MeshEntity.h>
#define UNICODE 


class CLayersPanel : public CPanel<Ui::LayersPanel>
{
    Q_OBJECT

public:
    CLayersPanel(QWidget* pForm)
        : CPanel<Ui::LayersPanel>(pForm)
    {
        connect(m_ui.pSpinBoxLayer,          SIGNAL(valueChanged(int)),      this, SLOT(slotLayerChanged(int)));
        connect(m_ui.pSpinBoxLayerDepth,     SIGNAL(valueChanged(double)),   this, SLOT(slotLayerDepthChanged(double)));
        connect(m_ui.pBtnLayerColor,         SIGNAL(pressed()),              this, SLOT(slotChooseLayerColor()));
        connect(m_ui.pBtnEmissiveColor,      SIGNAL(pressed()),              this, SLOT(slotChooseEmissiveColor()));
        connect(m_ui.pBtnLayerApplyAll,      SIGNAL(pressed()),              this, SLOT(slotLayerApplyAll()));
        connect(m_ui.pBtnEmissiveApplyAll,   SIGNAL(pressed()),              this, SLOT(slotEmissiveApplyAll()));
        connect(m_ui.pSpinBoxEmissiveEnergy, SIGNAL(valueChanged(double)),   this, SLOT(slotEmissiveEnergyChanged(double)));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        m_ui.pSpinBoxLayer->setMinimum(1);
        m_ui.pSpinBoxLayer->setMaximum(env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetNumLayers());
        m_ui.pSpinBoxLayer->setValue(1);

        slotLayerChanged(1);
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

private:

    //-----------------------------------------------------------------------------------
    void UpdateEditParameters()
    {
        env::Vec3 diffuseColor = env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainDiffuseColor(m_ui.pSpinBoxLayer->value() - 1);
        env::Vec4 emissiveColorEnergy = env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(m_ui.pSpinBoxLayer->value() - 1);

        env::LevelCellEditParameters& editParams = env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetCellEditParameters();
        if (editParams.currentEditLayer == (m_ui.pSpinBoxLayer->value() - 1))
        {
            editParams.currentEditColor = diffuseColor;
            editParams.currentEditEmissiveColor = emissiveColorEnergy;
        }
    }

public slots:
    
    //-----------------------------------------------------------------------------------
    virtual void slotLayerChanged(int value)
    {
        value -= 1;

        env::Vec3 layerColor    = env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainDiffuseColor(value);
        env::Vec4 emissiveColorEnergy = env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(value);
        env::Vec3 emissiveColor(emissiveColorEnergy);

        SetBtnColor(m_ui.pBtnLayerColor, env::DoGamma(layerColor));
        SetBtnColor(m_ui.pBtnEmissiveColor, env::DoGamma(emissiveColor));

        m_ui.pSpinBoxLayerDepth->setValue(env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetLayerDepth(value));
        m_ui.pSpinBoxEmissiveEnergy->setValue(emissiveColorEnergy.a);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotLayerDepthChanged(double value)
    {
        if (m_ui.pSpinBoxLayerDepth->hasFocus())
            env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().SetLayerDepth(m_ui.pSpinBoxLayer->value() - 1, value);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotEmissiveEnergyChanged(double value)
    {
        if (m_ui.pSpinBoxEmissiveEnergy->hasFocus())
        {
            env::Vec4 emissiveColor = env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(m_ui.pSpinBoxLayer->value() - 1);
            emissiveColor.a = m_ui.pSpinBoxEmissiveEnergy->value();

            env::gEngine->GetWorldMgr()->GetScenario()->SetTerrainEmissiveColor(m_ui.pSpinBoxLayer->value() - 1, emissiveColor);

            UpdateEditParameters();
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void slotEmissiveApplyAll()
    {
        env::Vec4 emissiveColor = env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(m_ui.pSpinBoxLayer->value() - 1);

        env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().ApplyEmissiveColor(m_ui.pSpinBoxLayer->value() - 1, emissiveColor);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotLayerApplyAll()
    {
        env::Vec3 layerColor = env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainDiffuseColor(m_ui.pSpinBoxLayer->value() - 1);

        env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().ApplyDiffuseColor(m_ui.pSpinBoxLayer->value() - 1, layerColor);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotLayerColorChanged(const QColor& color)
    {
        env::Vec3 layerColor = FromQColor(color);

        SetBtnColor(m_ui.pBtnLayerColor, layerColor);

        env::UndoGamma(layerColor);

        env::gEngine->GetWorldMgr()->GetScenario()->SetTerrainDiffuseColor(m_ui.pSpinBoxLayer->value() - 1, layerColor);

        UpdateEditParameters();
    }
    
    //-----------------------------------------------------------------------------------
    virtual void slotChooseLayerColor()
    {
        env::Vec3 layerColor = env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainDiffuseColor(m_ui.pSpinBoxLayer->value() - 1);

        env::DoGamma(layerColor);

        gEditor->AcquireColorDialog(this, SLOT(slotLayerColorChanged(const QColor&)), layerColor, "Choose Diffuse Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotEmissiveColorChanged(const QColor& color)
    {
        env::Vec3 emissiveColor = FromQColor(color);

        SetBtnColor(m_ui.pBtnEmissiveColor, emissiveColor);

        env::UndoGamma(emissiveColor);

        env::Vec4 emissiveColorEnergy = env::Vec4(emissiveColor, env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(m_ui.pSpinBoxLayer->value() - 1).a);

        env::gEngine->GetWorldMgr()->GetScenario()->SetTerrainEmissiveColor(m_ui.pSpinBoxLayer->value() - 1, emissiveColorEnergy);
        
        UpdateEditParameters();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseEmissiveColor()
    {
        env::Vec3 layerColor = env::Vec3(env::gEngine->GetWorldMgr()->GetScenario()->GetTerrainEmissiveColor(m_ui.pSpinBoxLayer->value() - 1));

        env::DoGamma(layerColor);

        gEditor->AcquireColorDialog(this, SLOT(slotEmissiveColorChanged(const QColor&)), layerColor, "Choose Emissive Color");
    }

};