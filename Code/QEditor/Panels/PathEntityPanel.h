/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/PathEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/PathEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Engine.h>
#define UNICODE 

class CPathEntityPanel : public CPanel<Ui::PathEntityPanel>
{
    Q_OBJECT

public:
    CPathEntityPanel(QWidget* pForm)
        : CPanel<Ui::PathEntityPanel>(pForm)
        , m_pEntity(nullptr)
    {
        connect(m_ui.pCheckBoxLoop, SIGNAL(clicked(bool)), this, SLOT(slotPathSettingsChanged()));
        connect(m_ui.pCheckBoxReverse, SIGNAL(clicked(bool)), this, SLOT(slotPathSettingsChanged()));
        connect(m_ui.pBtnAttachEntity, SIGNAL(pressed()), this, SLOT(slotAttachEntity()));
        connect(m_ui.pBtnDetachEntity, SIGNAL(pressed()), this, SLOT(slotDetachEntity()));
        connect(m_ui.pComboEditAttachee, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotEditAttachee(const QString &)));
        connect(m_ui.pSpinBoxStartingWaypoint, SIGNAL(valueChanged(int)), this, SLOT(slotAttacheeSettingsChanged()));
        connect(m_ui.pSpinBoxTimeOffset, SIGNAL(valueChanged(double)), this, SLOT(slotAttacheeSettingsChanged()));
        connect(m_ui.pCheckBoxFollowPlayer, SIGNAL(clicked(bool)), this, SLOT(slotAttacheeSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CPathEntity*>(pEntity);

        m_ui.pCheckBoxLoop->setChecked(m_pEntity->GetLoop());
        m_ui.pCheckBoxReverse->setChecked(m_pEntity->GetReverse());

        UpdateEntitiesComboBox();

        UpdateAttacheeComboBox();

    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotPathSettingsChanged()
    {
        if (!m_pEntity)
            return;

        m_pEntity->SetLoop(m_ui.pCheckBoxLoop->isChecked());
        m_pEntity->SetReverse(m_ui.pCheckBoxReverse->isChecked());
    }

    virtual void slotAttacheeSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pComboEditAttachee->count() == 0)
            return;

        const QString& strCurrentAttachee = m_ui.pComboEditAttachee->currentText();

        if (strCurrentAttachee.isEmpty())
            return;

        env::Train* pAttachee = m_pEntity->GetTrain(strCurrentAttachee.toStdString());

        if (!pAttachee)
            return;

        if (m_ui.pSpinBoxStartingWaypoint->hasFocus())
        {
            pAttachee->startCornerInitial = m_ui.pSpinBoxStartingWaypoint->value();
            pAttachee->startCorner = pAttachee->startCornerInitial;
        }

        if (m_ui.pSpinBoxTimeOffset->hasFocus())
            pAttachee->offset = (float)m_ui.pSpinBoxTimeOffset->value();

        pAttachee->bFollowPlayer = m_ui.pCheckBoxFollowPlayer->isChecked();
    }


    virtual void slotEditAttachee(const QString& entityName)
    {
        if (entityName.isEmpty())
        {
            m_ui.pCheckBoxFollowPlayer->setEnabled(false);
            m_ui.pSpinBoxStartingWaypoint->setEnabled(false);
            m_ui.pSpinBoxTimeOffset->setEnabled(false);

            return;
        }
        else
        {
            m_ui.pCheckBoxFollowPlayer->setEnabled(true);
            m_ui.pSpinBoxStartingWaypoint->setEnabled(true);
            m_ui.pSpinBoxTimeOffset->setEnabled(true);
        }

        env::Train* pAttachee = m_pEntity->GetTrain(entityName.toStdString());

        if (!pAttachee)
            return;

        m_ui.pCheckBoxFollowPlayer->setChecked(pAttachee->bFollowPlayer);
        m_ui.pSpinBoxStartingWaypoint->setValue(pAttachee->startCornerInitial);
        m_ui.pSpinBoxTimeOffset->setValue(pAttachee->offset);
    }

    virtual void slotAttachEntity()
    {
        const QString& strCurrentEntity = m_ui.pComboAttachEntity->currentText();

        if (strCurrentEntity.isEmpty())
            return;

        env::CEntity* pEntity = env::gEngine->GetEntityMgr()->GetEntity(strCurrentEntity.toStdString());

        if (!pEntity)
            return;

        m_pEntity->AttachNode(pEntity->GetNode(), pEntity->GetName());

        UpdateEntitiesComboBox();

        UpdateAttacheeComboBox();
    }

    virtual void slotDetachEntity()
    {
        const QString& strCurrentEntity = m_ui.pComboEditAttachee->currentText();

        if (strCurrentEntity.isEmpty())
            return;

        env::Vec3 derivedPosition(0.0f, 0.0f, 0.0f);
        env::CQuaternion derivedOrientation;
        env::CSceneNode* pEntityNode = 0;

        if (m_pEntity->GetTrain(strCurrentEntity.toStdString()))
        {
            pEntityNode = m_pEntity->GetTrain(strCurrentEntity.toStdString())->pAttachedNode;

            derivedPosition = m_pEntity->GetTrain(strCurrentEntity.toStdString())->pAttachedNode->GetDerivedPosition();
            derivedOrientation = m_pEntity->GetTrain(strCurrentEntity.toStdString())->pAttachedNode->GetDerivedOrientation();
        }

        m_pEntity->DetachNode(strCurrentEntity.toStdString());

        pEntityNode->SetPosition(derivedPosition);
        pEntityNode->SetOrientation(derivedOrientation);

        UpdateEntitiesComboBox();

        UpdateAttacheeComboBox();
    }

private:

    void UpdateAttacheeComboBox()
    {
        m_ui.pComboEditAttachee->clear();

        const std::list< env::Train >& attachees = m_pEntity->GetTrains();

        for (std::list< env::Train >::const_iterator it = attachees.begin(); it != attachees.end(); ++it)
        {
            m_ui.pComboEditAttachee->addItem((*it).strEntityName.c_str());
        }

    }

    void UpdateEntitiesComboBox()
    {
        m_ui.pComboAttachEntity->clear();

        std::list< std::string > entities;
        env::gEngine->GetEntityMgr()->GetEntities(entities);

        for (std::list< std::string >::iterator it = entities.begin(); it != entities.end(); ++it)
        {
            std::string strEntityName = *it;

            if (strEntityName == m_pEntity->GetName() || m_pEntity->GetTrain(strEntityName))
                continue;

            m_ui.pComboAttachEntity->addItem(strEntityName.c_str());
        }
    }
private:

    env::CPathEntity* m_pEntity;

};