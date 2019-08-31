/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/


#pragma once

#include <QEditor/UIGen/StreamerGunEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/StreamerGunEntity.h>
#define UNICODE 


class CStreamerGunEntityPanel : public CPanel<Ui::StreamerGunEntityPanel>
{
    Q_OBJECT

public:
    CStreamerGunEntityPanel(QWidget* pForm)
        : CPanel<Ui::StreamerGunEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pSpinBoxSpeed, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxLifetime, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxFireRate, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxDelay, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));

    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CStreamerGunEntity*>(pEntity);

        m_ui.pSpinBoxSpeed->setValue((double)m_pEntity->GetSpeed());
        m_ui.pSpinBoxLifetime->setValue((double)m_pEntity->GetLifeTime());
        m_ui.pSpinBoxFireRate->setValue((double)m_pEntity->GetFireRate());
        m_ui.pSpinBoxDelay->setValue((double)m_pEntity->GetDelay());

    }

    public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxSpeed->hasFocus())
            m_pEntity->SetSpeed((float)m_ui.pSpinBoxSpeed->value());

        if (m_ui.pSpinBoxLifetime->hasFocus())
            m_pEntity->SetLifeTime((float)m_ui.pSpinBoxLifetime->value());

        if (m_ui.pSpinBoxFireRate->hasFocus())
            m_pEntity->SetFireRate((float)m_ui.pSpinBoxFireRate->value());

        if (m_ui.pSpinBoxDelay->hasFocus())
            m_pEntity->SetDelay((float)m_ui.pSpinBoxDelay->value());
    }


private:

    env::CStreamerGunEntity* m_pEntity;

};