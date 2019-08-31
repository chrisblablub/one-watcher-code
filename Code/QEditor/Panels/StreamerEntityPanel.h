/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/StreamerEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/StreamerEntity.h>
#define UNICODE 




class CStreamerEntityPanel : public CPanel<Ui::StreamerEntityPanel>
{
    Q_OBJECT

public:
    CStreamerEntityPanel(QWidget* pForm)
        : CPanel<Ui::StreamerEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pSpinBoxPause, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pRadioBtnSmall, SIGNAL(clicked(bool)), this, SLOT(slotRadioButtonClicked()));
        connect(m_ui.pRadioBtnBig, SIGNAL(clicked(bool)), this, SLOT(slotRadioButtonClicked()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CStreamerEntity*>(pEntity);

        m_ui.pSpinBoxPause->setValue((double)m_pEntity->GetBurnPause());
        m_ui.pSpinBoxDuration->setValue((double)m_pEntity->GetBurnDuration());

        if (m_pEntity->GetType() == env::STREAMER_TYPE_SMALL)
            m_ui.pRadioBtnSmall->setChecked(true);
        else
            m_ui.pRadioBtnBig->setChecked(true);
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxPause->hasFocus())
            m_pEntity->SetBurnPause((float)m_ui.pSpinBoxPause->value());

        if (m_ui.pSpinBoxDuration->hasFocus())
            m_pEntity->SetBurnDuration((float)m_ui.pSpinBoxDuration->value());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotRadioButtonClicked()
    {
        if (m_ui.pRadioBtnSmall->isChecked())
            m_pEntity->SetType(env::STREAMER_TYPE_SMALL);
        else
            m_pEntity->SetType(env::STREAMER_TYPE_BIG);
    }


private:

    env::CStreamerEntity* m_pEntity;

};