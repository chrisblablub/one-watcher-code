/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/EntityTriggerPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#undef UNICODE
#include <Engine/Entity/Entity.h>
#define UNICODE 


class CEntityTriggerPanel : public CPanel<Ui::EntityTriggerPanel>
{
    Q_OBJECT

public:
    CEntityTriggerPanel(QWidget* pForm)
        : CPanel<Ui::EntityTriggerPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pCheckBoxFireOnStart, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxResetOnStart, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxFireOnce, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxFireOncePerHit, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pSpinBoxTriggerDelay, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = pEntity;

        m_ui.pCheckBoxFireOnStart->setChecked(m_pEntity->GetFireOnStart());
        m_ui.pCheckBoxResetOnStart->setChecked(m_pEntity->GetResetTrigger());
        m_ui.pCheckBoxFireOnce->setChecked(m_pEntity->GetFireOnce());
        m_ui.pCheckBoxFireOncePerHit->setChecked(m_pEntity->GetFireOncePerHit());

        m_ui.pSpinBoxTriggerDelay->setValue((double)m_pEntity->GetFireDelay());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;


        if (m_ui.pSpinBoxTriggerDelay->hasFocus())
            m_pEntity->SetFireDelay((float)m_ui.pSpinBoxTriggerDelay->value());
    }

    virtual void slotCheckBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        m_pEntity->SetFireOnStart(m_ui.pCheckBoxFireOnStart->isChecked());
        m_pEntity->SetResetTrigger(m_ui.pCheckBoxResetOnStart->isChecked());
        m_pEntity->SetFireOnce(m_ui.pCheckBoxFireOnce->isChecked());
        m_pEntity->SetFireOncePerHit(m_ui.pCheckBoxFireOncePerHit->isChecked());
    }


private:

    env::CEntity* m_pEntity;

};