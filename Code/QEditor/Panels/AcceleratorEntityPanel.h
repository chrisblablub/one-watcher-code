/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/AcceleratorEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/AcceleratorEntity.h>
#define UNICODE 




class CAcceleratorEntityPanel : public CPanel<Ui::AcceleratorEntityPanel>
{
    Q_OBJECT

public:
    CAcceleratorEntityPanel(QWidget* pForm)
        : CPanel<Ui::AcceleratorEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pSpinBoxForce, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CAcceleratorEntity*>(pEntity);

        m_ui.pSpinBoxForce->setValue((double)m_pEntity->GetStrength());
        m_ui.pSpinBoxDuration->setValue((double)m_pEntity->GetDuration());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxForce->hasFocus())
            m_pEntity->SetStrength((float)m_ui.pSpinBoxForce->value());

        if (m_ui.pSpinBoxDuration->hasFocus())
            m_pEntity->SetDuration((float)m_ui.pSpinBoxDuration->value());
    }


private:

    env::CAcceleratorEntity* m_pEntity;

};