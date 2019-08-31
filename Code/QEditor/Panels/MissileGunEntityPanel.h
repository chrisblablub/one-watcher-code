/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/MissileGunEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/MissileEntity.h>
#define UNICODE 


class CMissileGunEntityPanel : public CPanel<Ui::MissileGunEntityPanel>
{
    Q_OBJECT

public:
    CMissileGunEntityPanel(QWidget* pForm)
        : CPanel<Ui::MissileGunEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pSpinBoxFireDelayMin, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxFireDelayMax, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxLifetimeMin, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxLifetimeMax, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxVelocityMin, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxVelocityMax, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxRadius, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxTargetingVelocity, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pCheckBoxMissileTargeting, SIGNAL(clicked(bool)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pCheckBoxVisibilityChecking, SIGNAL(clicked(bool)), this, SLOT(slotSpinBoxSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CMissileGunEntity*>(pEntity);

        m_ui.pSpinBoxFireDelayMin->setValue((double)m_pEntity->GetFireDelayMin());
        m_ui.pSpinBoxFireDelayMax->setValue((double)m_pEntity->GetFireDelayMax());
        m_ui.pSpinBoxLifetimeMin->setValue((double)m_pEntity->GetLifetimeMin());
        m_ui.pSpinBoxLifetimeMax->setValue((double)m_pEntity->GetLifetimeMax());
        m_ui.pSpinBoxVelocityMin->setValue((double)m_pEntity->GetVelocityMin());
        m_ui.pSpinBoxVelocityMax->setValue((double)m_pEntity->GetVelocityMax());
        m_ui.pSpinBoxRadius->setValue((double)m_pEntity->GetVisibilityRadius());
        m_ui.pSpinBoxTargetingVelocity->setValue((double)m_pEntity->GetMissileTargetingSpeed());
        m_ui.pCheckBoxMissileTargeting->setChecked(m_pEntity->GetMissilesTargeting());
        m_ui.pCheckBoxVisibilityChecking->setChecked(m_pEntity->GetVisibility());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxFireDelayMin->hasFocus())
            m_pEntity->SetFireDelayMin((float)m_ui.pSpinBoxFireDelayMin->value());

        if (m_ui.pSpinBoxFireDelayMax->hasFocus())
            m_pEntity->SetFireDelayMax((float)m_ui.pSpinBoxFireDelayMax->value());

        if (m_ui.pSpinBoxLifetimeMin->hasFocus())
            m_pEntity->SetLifetimeMin((float)m_ui.pSpinBoxLifetimeMin->value());

        if (m_ui.pSpinBoxLifetimeMax->hasFocus())
            m_pEntity->SetLifetimeMax((float)m_ui.pSpinBoxLifetimeMax->value());

        if (m_ui.pSpinBoxVelocityMin->hasFocus())
            m_pEntity->SetVelocityMin((float)m_ui.pSpinBoxVelocityMin->value());

        if (m_ui.pSpinBoxVelocityMax->hasFocus())
            m_pEntity->SetVelocityMax((float)m_ui.pSpinBoxVelocityMax->value());

        if (m_ui.pSpinBoxRadius->hasFocus())
            m_pEntity->SetVisibilityRadius((float)m_ui.pSpinBoxRadius->value());

        if (m_ui.pSpinBoxTargetingVelocity->hasFocus())
            m_pEntity->SetMissileTargetingSpeed((float)m_ui.pSpinBoxTargetingVelocity->value());

        m_pEntity->SetMissilesTargeting(m_ui.pCheckBoxMissileTargeting->isChecked());
        m_pEntity->SetVisibility(m_ui.pCheckBoxVisibilityChecking->isChecked());
    }


private:
    env::CMissileGunEntity* m_pEntity;

};