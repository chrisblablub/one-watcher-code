/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/LaserRadiatorEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/LaserRadiatorEntity.h>
#include <Engine/Core/Engine.h>
#define UNICODE 




class CLaserRadiatorEntityPanel : public CPanel<Ui::LaserRadiatorEntityPanel>
{
    Q_OBJECT

public:
    CLaserRadiatorEntityPanel(QWidget* pForm)
        : CPanel<Ui::LaserRadiatorEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pSpinBoxLasers, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxMaxDistance, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxActiveTime, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxPausingTime, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxLoadingTime, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxDelay, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxOffsetRadius, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pCheckBoxTerrainCollisions, SIGNAL(clicked(bool)), this, SLOT(slotCheckboxChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CLaserRadiatorEntity*>(pEntity);

        m_ui.pSpinBoxLasers->setValue(m_pEntity->GetLaserCount());
        m_ui.pSpinBoxMaxDistance->setValue(m_pEntity->GetLaserMaxDistance());
        m_ui.pSpinBoxActiveTime->setValue(m_pEntity->GetLaserActiveTime());
        m_ui.pSpinBoxPausingTime->setValue(m_pEntity->GetLaserPausingTime());
        m_ui.pSpinBoxLoadingTime->setValue(m_pEntity->GetLaserLoadingTime());
        m_ui.pSpinBoxDelay->setValue(m_pEntity->GetLaserDelay());
        m_ui.pSpinBoxOffsetRadius->setValue(m_pEntity->GetLaserOffsetRadius());
        m_ui.pCheckBoxTerrainCollisions->setChecked(m_pEntity->GetLaserTerrainCollisions());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxLasers->hasFocus())
            m_pEntity->SetLaserCount(m_ui.pSpinBoxLasers->value());

        if (m_ui.pSpinBoxMaxDistance->hasFocus())
            m_pEntity->SetLaserMaxDistance(m_ui.pSpinBoxMaxDistance->value());

        if (m_ui.pSpinBoxActiveTime->hasFocus())
            m_pEntity->SetLaserActiveTime(m_ui.pSpinBoxActiveTime->value());

        if (m_ui.pSpinBoxPausingTime->hasFocus())
            m_pEntity->SetLaserPausingTime(m_ui.pSpinBoxPausingTime->value());

        if (m_ui.pSpinBoxLoadingTime->hasFocus())
            m_pEntity->SetLaserLoadingTime(m_ui.pSpinBoxLoadingTime->value());

        if (m_ui.pSpinBoxDelay->hasFocus())
            m_pEntity->SetLaserDelay(m_ui.pSpinBoxDelay->value());

        if (m_ui.pSpinBoxOffsetRadius->hasFocus())
            m_pEntity->SetLaserOffsetRadius(m_ui.pSpinBoxOffsetRadius->value());

        
    }

    virtual void slotCheckBoxChanged()
    {
        if (!m_pEntity)
            return;

        m_pEntity->SetLaserTerrainCollisions(m_ui.pCheckBoxTerrainCollisions->isChecked());
    }


private:
    //Ui::LaserRadiatorEntityPanel m_ui;
    env::CLaserRadiatorEntity* m_pEntity;

};