/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/


#pragma once

#include <QEditor/UIGen/WeaponEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/WeaponEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Engine.h>
#define UNICODE 

class CWeaponEntityPanel : public CPanel<Ui::WeaponEntityPanel>
{
    Q_OBJECT

public:
    CWeaponEntityPanel(QWidget* pForm)
        : CPanel<Ui::WeaponEntityPanel>(pForm)
        , m_pEntity(nullptr)
    {
        connect(m_ui.pSpinBoxRadius, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxBossDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxChargingDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));

    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CWeaponEntity*>(pEntity);

        m_ui.pSpinBoxRadius->setValue(m_pEntity->GetRadius());
        m_ui.pSpinBoxBossDuration->setValue(m_pEntity->GetBossLaserDuration());
        m_ui.pSpinBoxChargingDuration->setValue(m_pEntity->GetChargingDuration());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSettingsChanged()
    {
        if (m_ui.pSpinBoxRadius->hasFocus())
            m_pEntity->SetRadius(m_ui.pSpinBoxRadius->value());

        if (m_ui.pSpinBoxBossDuration->hasFocus())
            m_pEntity->SetBossLaserDuration(m_ui.pSpinBoxBossDuration->value());

        if (m_ui.pSpinBoxChargingDuration->hasFocus())
            m_pEntity->SetChargingDuration(m_ui.pSpinBoxChargingDuration->value());
    }

private:

    env::CWeaponEntity* m_pEntity;

};