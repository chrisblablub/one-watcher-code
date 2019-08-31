/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/WaypointEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/PathEntity.h>
#include <Engine/Entity/PathWaypointEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Engine.h>
#define UNICODE 




class CWaypointEntityPanel : public CPanel<Ui::WaypointEntityPanel>
{
    Q_OBJECT

public:
    CWaypointEntityPanel(QWidget* pForm)
        : CPanel<Ui::WaypointEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pSpinBoxSpeed, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxAcceleration, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxPause, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxBreaking, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pBtnAttach, SIGNAL(pressed()), this, SLOT(slotAttachEntity()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CPathWaypointEntity*>(pEntity);

        m_ui.pSpinBoxAcceleration->setValue(m_pEntity->GetAcceleration());
        m_ui.pSpinBoxBreaking->setValue(m_pEntity->GetBreakAcceleartion());
        m_ui.pSpinBoxPause->setValue(m_pEntity->GetPause());
        m_ui.pSpinBoxSpeed->setValue(m_pEntity->GetSpeed());

        UpdatePathComboBox();
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxAcceleration->hasFocus())
            m_pEntity->SetAcceleration(m_ui.pSpinBoxAcceleration->value());

        if (m_ui.pSpinBoxBreaking->hasFocus())
            m_pEntity->SetBreakAcceleartion(m_ui.pSpinBoxBreaking->value());

        if (m_ui.pSpinBoxPause->hasFocus())
            m_pEntity->SetPause(m_ui.pSpinBoxPause->value());

        if (m_ui.pSpinBoxSpeed->hasFocus())
            m_pEntity->SetSpeed(m_ui.pSpinBoxSpeed->value());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotAttachEntity()
    {
        const QString& strCurrentEntity = m_ui.pComboBoxPath->currentText();

        if (strCurrentEntity.isEmpty())
            return;

        if (strCurrentEntity == "<No Path>")
            return;

        env::CPathEntity* pEntity = static_cast<env::CPathEntity*>(env::gEngine->GetEntityMgr()->GetEntity(strCurrentEntity.toStdString()));

        if (!pEntity)
            return;

        m_pEntity->SetPath(pEntity);
    }


private:

    //-----------------------------------------------------------------------------------
    void UpdatePathComboBox()
    {
        m_ui.pComboBoxPath->clear();

        std::list< std::string > entities;
        env::gEngine->GetEntityMgr()->GetEntities(entities, env::ENTITY_TYPEID_PATH);

        for (std::list< std::string >::iterator it = entities.begin(); it != entities.end(); ++it)
        {
            std::string strEntityName = *it;

            m_ui.pComboBoxPath->addItem(strEntityName.c_str());
        }

        if (m_pEntity->GetPath())
        {
            m_ui.pComboBoxPath->setCurrentText(m_pEntity->GetPath()->GetName().c_str());
        }
        else
        {
            m_ui.pComboBoxPath->addItem("<No Path>");
            m_ui.pComboBoxPath->setCurrentText("<No Path>");
        }
    }
private:

    env::CPathWaypointEntity* m_pEntity;

};