/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/MagnetEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/MagnetEntity.h>
#define UNICODE 




class CMagnetEntityPanel : public CPanel<Ui::MagnetEntityPanel>
{
    Q_OBJECT

public:
    CMagnetEntityPanel(QWidget* pForm)
        : CPanel<Ui::MagnetEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pSpinBoxForce, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CMagnetEntity*>(pEntity);

        m_ui.pSpinBoxForce->setValue((double)m_pEntity->GetSpeed());
        m_ui.pSpinBoxDuration->setValue((double)m_pEntity->GetTime());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxForce->hasFocus())
            m_pEntity->SetSpeed((float)m_ui.pSpinBoxForce->value());

        if (m_ui.pSpinBoxDuration->hasFocus())
            m_pEntity->SetTime((float)m_ui.pSpinBoxDuration->value());
    }


private:

    env::CMagnetEntity* m_pEntity;

};