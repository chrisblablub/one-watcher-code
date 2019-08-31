/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/RobotArmEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/RobotArmEntity.h>
#include <Engine/Core/Engine.h>
#define UNICODE 


class CRobotArmEntityPanel : public CPanel<Ui::RobotArmEntityPanel>
{
    Q_OBJECT

public:
    CRobotArmEntityPanel(QWidget* pForm)
        : CPanel<Ui::RobotArmEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pSpinBoxDelay, SIGNAL(valueChanged(double)()), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxHitTime, SIGNAL(valueChanged(double)()), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxHitPauseTime, SIGNAL(valueChanged(double)()), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxReverseTime, SIGNAL(valueChanged(double)()), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxReversePauseTime, SIGNAL(valueChanged(double)()), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pCheckBoxMirror, SIGNAL(clicked(bool)), this, SLOT(slotSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CRobotArmT3Entity*>(pEntity);

        if (!m_pEntity)
            return;

        m_ui.pSpinBoxDelay->setValue(m_pEntity->GetDelay());
        m_ui.pSpinBoxHitTime->setValue(m_pEntity->GetHitTime());
        m_ui.pSpinBoxHitPauseTime->setValue(m_pEntity->GetHitPauseTime());
        m_ui.pSpinBoxReverseTime->setValue(m_pEntity->GetReverseHitTime());
        m_ui.pSpinBoxReversePauseTime->setValue(m_pEntity->GetReverseHitPauseTime());
        m_ui.pCheckBoxMirror->setChecked(m_pEntity->GetMirror());

    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxDelay->hasFocus())
            m_pEntity->SetDelay(m_ui.pSpinBoxDelay->value());

        if (m_ui.pSpinBoxHitTime->hasFocus())
            m_pEntity->SetHitTime(m_ui.pSpinBoxHitTime->value());

        if (m_ui.pSpinBoxHitPauseTime->hasFocus())
            m_pEntity->SetHitPauseTime(m_ui.pSpinBoxHitPauseTime->value());

        if (m_ui.pSpinBoxReversePauseTime->hasFocus())
            m_pEntity->SetReverseHitPauseTime(m_ui.pSpinBoxReversePauseTime->value());

        if (m_ui.pSpinBoxReverseTime->hasFocus())
            m_pEntity->SetReverseHitTime(m_ui.pSpinBoxReverseTime->value());

        m_pEntity->SetMirror(m_ui.pCheckBoxMirror->isChecked());
    }


private:

    env::CRobotArmT3Entity* m_pEntity;

};