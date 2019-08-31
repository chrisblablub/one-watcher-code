/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/MissileGunAimingEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/MissileEntity.h>
#define UNICODE 


class CMissileGunAimingEntityPanel : public CPanel<Ui::MissileGunAimingEntityPanel>
{
    Q_OBJECT

public:
    CMissileGunAimingEntityPanel(QWidget* pForm)
        : CPanel<Ui::MissileGunAimingEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pSpinBoxRadius, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxTargetingVelocity, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CMissileGunAimingEntity*>(pEntity);

        m_ui.pSpinBoxRadius->setValue((double)m_pEntity->GetOffsetRadius());
        m_ui.pSpinBoxTargetingVelocity->setValue((double)m_pEntity->GetTargetingSpeed());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxTargetingVelocity->hasFocus())
            m_pEntity->SetTargetingSpeed((float)m_ui.pSpinBoxTargetingVelocity->value());

        if (m_ui.pSpinBoxRadius->hasFocus())
            m_pEntity->SetOffsetRadius((float)m_ui.pSpinBoxRadius->value());
    }

private:
    env::CMissileGunAimingEntity* m_pEntity;

};