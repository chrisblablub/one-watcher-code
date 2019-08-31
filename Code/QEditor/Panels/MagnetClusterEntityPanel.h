/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/MagnetClusterEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>


#undef UNICODE
#include <Engine/Entity/MagnetEntity.h>
#define UNICODE 




class CMagnetClusterEntityPanel : public CPanel<Ui::MagnetClusterEntityPanel>
{
    Q_OBJECT

public:
    CMagnetClusterEntityPanel(QWidget* pForm)
        : CPanel<Ui::MagnetClusterEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pSpinBoxRadius, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CMagnetClusterEntity*>(pEntity);

        m_ui.pSpinBoxRadius->setValue((double)m_pEntity->GetOffsetRadius());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxRadius->hasFocus())
            m_pEntity->SetOffsetRadius((float)m_ui.pSpinBoxRadius->value());
    }



private:

    env::CMagnetClusterEntity* m_pEntity;

};