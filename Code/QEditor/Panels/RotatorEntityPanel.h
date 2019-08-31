/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/RotatorEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/RotatorEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Engine.h>
#define UNICODE 


class CRotatorEntityPanel : public CPanel<Ui::RotatorEntityPanel>
{
    Q_OBJECT

public:
    CRotatorEntityPanel(QWidget* pForm)
        : CPanel<Ui::RotatorEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pSpinBoxSpeed, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxAngle, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxStartAngle, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxStartPause, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEndPause, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxRadius, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pCheckBoxReverse, SIGNAL(clicked(bool)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pBtnAttach, SIGNAL(pressed()), this, SLOT(slotAttachEntity()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CRotatorEntity*>(pEntity);

        m_ui.pSpinBoxSpeed->setValue(m_pEntity->GetSpeed());
        m_ui.pSpinBoxAngle->setValue(env::RadToDeg(m_pEntity->GetAngle()));
        m_ui.pSpinBoxStartAngle->setValue(env::RadToDeg(m_pEntity->GetStartAngle()));
        m_ui.pSpinBoxStartPause->setValue(m_pEntity->GetStartPause());
        m_ui.pSpinBoxEndPause->setValue(m_pEntity->GetEndPause());
        m_ui.pSpinBoxRadius->setValue(m_pEntity->GetRadius());
        m_ui.pCheckBoxReverse->setChecked(m_pEntity->GetReverse());

        UpdatePathComboBox();
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxSpeed->hasFocus())
            m_pEntity->SetSpeed(m_ui.pSpinBoxSpeed->value());

        if (m_ui.pSpinBoxAngle->hasFocus())
            m_pEntity->SetAngle(env::DegToRad(m_ui.pSpinBoxAngle->value()));

        if (m_ui.pSpinBoxStartAngle->hasFocus())
            m_pEntity->SetStartAngle(env::DegToRad(m_ui.pSpinBoxStartAngle->value()));

        if (m_ui.pSpinBoxStartPause->hasFocus())
            m_pEntity->SetStartPause(m_ui.pSpinBoxStartPause->value());

        if (m_ui.pSpinBoxEndPause->hasFocus())
            m_pEntity->SetEndPause(m_ui.pSpinBoxEndPause->value());

        if (m_ui.pSpinBoxRadius->hasFocus())
            m_pEntity->SetRadius(m_ui.pSpinBoxRadius->value());

        m_pEntity->SetReverse(m_ui.pCheckBoxReverse->isChecked());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotAttachEntity()
    {
        const QString& strCurrentEntity = m_ui.pComboBoxEntity->currentText();

        if (strCurrentEntity.isEmpty())
            return;

        if (strCurrentEntity == "<No Entity>")
            return;

        env::CEntity* pEntity = (env::gEngine->GetEntityMgr()->GetEntity(strCurrentEntity.toStdString()));

        if (!pEntity)
            return;

        m_pEntity->AttachNode(pEntity->GetNode());

    }

private:

    //-----------------------------------------------------------------------------------
    void UpdatePathComboBox()
    {
        m_ui.pComboBoxEntity->clear();

        std::list< std::string > entities;
        env::gEngine->GetEntityMgr()->GetEntities(entities);

        for (std::list< std::string >::iterator it = entities.begin(); it != entities.end(); ++it)
        {
            std::string strEntityName = *it;

            if (strEntityName == m_pEntity->GetName())
                continue;

            m_ui.pComboBoxEntity->addItem(strEntityName.c_str());
        }

        if (m_pEntity->GetAttachedNode())
        {
            m_ui.pComboBoxEntity->setCurrentText(m_pEntity->GetName().c_str());
        }
        else
        {
            m_ui.pComboBoxEntity->addItem("<No Entity>");
            m_ui.pComboBoxEntity->setCurrentText("<No Entity>");
        }
    }
private:
    env::CRotatorEntity* m_pEntity;

};