/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/CameraEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/CameraEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Engine.h>
#include <Engine/Core/WorldMgr.h>
#define UNICODE 


class CameraEntityPanel : public CPanel<Ui::CameraEntityPanel>
{
    Q_OBJECT

public:

    //-----------------------------------------------------------------------------------
    CameraEntityPanel(QWidget* pForm)
        : CPanel<Ui::CameraEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pBtnEnableCam, SIGNAL(pressed()), this, SLOT(slotEnableCamera()));
        connect(m_ui.pBtnAttachTarget, SIGNAL(pressed()), this, SLOT(slotAttachTarget()));
        connect(m_ui.pBtnDetachTarget, SIGNAL(pressed()), this, SLOT(slotDetachTarget()));
        connect(m_ui.pSpinBoxRotationSpeed, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxSmoothEnableDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pCheckBoxRollToTarget, SIGNAL(clicked(bool)), this, SLOT(slotSpinBoxSettingsChanged()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CCameraEntity*>(pEntity);

        m_ui.pComboTarget->clear();

        if (!m_pEntity->GetTarget())
            m_ui.pComboTarget->addItem("<No Target>");

        std::list< std::string > listEntities;
        env::gEngine->GetEntityMgr()->GetEntities(listEntities);

        for (std::list< std::string >::iterator it = listEntities.begin(); it != listEntities.end(); ++it)
        {
            const std::string& strName = *it;

            if (strName != m_pEntity->GetName())
                m_ui.pComboTarget->addItem((*it).c_str());
        }

        m_ui.pSpinBoxRotationSpeed->setValue((double)m_pEntity->GetRotationSpeed());
        m_ui.pSpinBoxSmoothEnableDuration->setValue((double)m_pEntity->GetSmoothEnableTime());
        m_ui.pCheckBoxRollToTarget->setChecked(m_pEntity->GetRollToTarget());

        if (m_pEntity->GetTarget())
            m_ui.pComboTarget->setCurrentText(m_pEntity->GetName().c_str());
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxRotationSpeed->hasFocus())
            m_pEntity->SetRotationSpeed((float)m_ui.pSpinBoxRotationSpeed->value());

        if (m_ui.pSpinBoxSmoothEnableDuration->hasFocus())
            m_pEntity->SetSmoothEnableTime((float)m_ui.pSpinBoxSmoothEnableDuration->value());

        m_pEntity->SetRollToTarget(m_ui.pCheckBoxRollToTarget->isChecked());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotEnableCamera()
    {
        if (!m_pEntity)
            return;

        m_pEntity->SetTarget(0);
        
        env::gEngine->GetWorldMgr()->SetActiveCamera(m_pEntity->GetCamera());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotAttachTarget()
    {
        QString strTarget = m_ui.pComboTarget->currentText();

        if (strTarget == "<No Target>")
            return;

        env::CEntity* pEntity = env::gEngine->GetEntityMgr()->GetEntity(strTarget.toStdString());

        if (!pEntity)
            return;

        m_pEntity->SetTarget(pEntity);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotDetachTarget()
    {
        m_pEntity->SetTarget(0);
    }

private:
    env::CCameraEntity* m_pEntity;

};