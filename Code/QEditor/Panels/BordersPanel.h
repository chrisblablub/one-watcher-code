/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

#include <QEditor/UIGen/BordersPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Scene/TerrainMgrCPU.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Entity/MeshEntity.h>
#define UNICODE 


class CBordersPanel : public CPanel<Ui::BordersPanel>
{
    Q_OBJECT

public:
    CBordersPanel(QWidget* pForm)
        : CPanel<Ui::BordersPanel>(pForm)
    {
        connect(m_ui.pSpinBoxMaximumDepth, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxBorderRotation, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pBtnBorderColor, SIGNAL(pressed()), this, SLOT(slotChooseBorderColor()));
        connect(m_ui.pBtnBorderTexture, SIGNAL(pressed()), this, SLOT(slotChooseBorderTexture()));
        connect(m_ui.pBtnRotationCenter, SIGNAL(pressed()), this, SLOT(slotUpdateBorderRotationCenter()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        m_ui.pSpinBoxMaximumDepth->setValue(env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetLayerMaxDepth());
        m_ui.pSpinBoxBorderRotation->setValue(env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetBorderRotationAngle());
        m_ui.pBtnBorderTexture->setText(env::gEngine->GetWorldMgr()->GetScenario()->GetBorderTexture().c_str());

        m_ui.pBtnBorderTexture->setText(env::gEngine->GetWorldMgr()->GetScenario()->GetBorderTexture().c_str());

        env::Vec3 borderColor = env::gEngine->GetWorldMgr()->GetScenario()->GetBorderColor();

        SetBtnColor(m_ui.pBtnBorderColor, env::DoGamma(borderColor));
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

public slots:

    virtual void slotSettingsChanged()
    {
        if (m_ui.pSpinBoxMaximumDepth->hasFocus())
            env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().SetLayerMaxDepth((float)m_ui.pSpinBoxMaximumDepth->value());

        if (m_ui.pSpinBoxBorderRotation->hasFocus())
            env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().SetBorderRotationAngle((float)m_ui.pSpinBoxBorderRotation->value());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseBorderColor()
    {
        env::Vec3 borderColor = env::gEngine->GetWorldMgr()->GetScenario()->GetBorderColor();

        env::DoGamma(borderColor);

        gEditor->AcquireColorDialog(this, SLOT(slotBorderColorChanged(const QColor&)), borderColor, "Choose Border Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotBorderColorChanged(const QColor& color)
    {
        env::Vec3 borderColor = FromQColor(color);

        env::gEngine->GetWorldMgr()->GetScenario()->SetBorderColor(env::UndoGamma(borderColor));

        SetBtnColor(m_ui.pBtnBorderColor, env::DoGamma(borderColor));
    }

    //-----------------------------------------------------------------------------------
    virtual void slotUpdateBorderRotationCenter()
    {
        unsigned int numCellsX = 0, numCellsY = 0;
        float cellWidth = 0.0f;
        env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetSize(numCellsX, numCellsY, cellWidth);
        env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().SetBorderRotationCenter(env::Vec3(numCellsX * cellWidth * 0.5f, numCellsY * cellWidth * 0.5f, 0.0f));
    }

    virtual void slotChooseBorderTexture()
    {
        std::string strCurrentBorderTexture = env::gEngine->GetWorldMgr()->GetScenario()->GetBorderTexture();

        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Load Texture", QString("../Resources/Textures/Terrain/") + strCurrentBorderTexture.c_str(), "Textures (*.*)");

        QFileInfo fi(strFilename);

        env::gEngine->GetWorldMgr()->GetScenario()->SetBorderTexture(("Terrain/" + fi.fileName()).toStdString());

        m_ui.pBtnBorderTexture->setText(env::gEngine->GetWorldMgr()->GetScenario()->GetBorderTexture().c_str());
    }


private:

};