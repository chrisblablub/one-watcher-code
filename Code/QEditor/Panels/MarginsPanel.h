/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/MarginsPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Entity/MeshEntity.h>
#define UNICODE 


class CMarginsPanel : public CPanel<Ui::MarginsPanel>
{
    Q_OBJECT

public:
    CMarginsPanel(QWidget* pForm)
        : CPanel<Ui::MarginsPanel>(pForm)
    {
        connect(m_ui.pSpinBoxLeftCell, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxRightCell, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxBottomCell, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxTopCell, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        env::LevelBorderOffsets& borderMargins = env::gEngine->GetWorldMgr()->GetScenario()->GetLevelBorderOffsets();

        m_ui.pSpinBoxLeftCell->setValue(borderMargins.left);
        m_ui.pSpinBoxRightCell->setValue(borderMargins.right);
        m_ui.pSpinBoxBottomCell->setValue(borderMargins.bottom);
        m_ui.pSpinBoxTopCell->setValue(borderMargins.top);
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSettingsChanged()
    {
        env::LevelBorderOffsets& borderMargins = env::gEngine->GetWorldMgr()->GetScenario()->GetLevelBorderOffsets();

        if (m_ui.pSpinBoxLeftCell->hasFocus())
            borderMargins.left = m_ui.pSpinBoxLeftCell->value();

        if (m_ui.pSpinBoxRightCell->hasFocus())
            borderMargins.right = m_ui.pSpinBoxRightCell->value();

        if (m_ui.pSpinBoxBottomCell->hasFocus())
            borderMargins.bottom = m_ui.pSpinBoxBottomCell->value();

        if (m_ui.pSpinBoxTopCell->hasFocus())
            borderMargins.top = m_ui.pSpinBoxTopCell->value();
    }

};