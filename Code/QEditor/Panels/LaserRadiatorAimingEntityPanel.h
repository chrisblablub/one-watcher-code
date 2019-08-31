/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/


#pragma once

#include <QEditor/UIGen/LaserRadiatorAimingEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/LaserRadiatorEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Engine.h>
#define UNICODE 

class CLaserRadiatorAimingEntityPanel : public CPanel<Ui::LaserRadiatorAimingEntityPanel>
{
    Q_OBJECT

public:
    CLaserRadiatorAimingEntityPanel(QWidget* pForm)
        : CPanel<Ui::LaserRadiatorAimingEntityPanel>(pForm)
        , m_pEntity(nullptr)
    {
        connect(m_ui.pBtnAttachEntity, SIGNAL(pressed()), this, SLOT(slotAttachEntity()));
        connect(m_ui.pBtnDetachEntity, SIGNAL(pressed()), this, SLOT(slotDetachEntity()));
        connect(m_ui.pComboEditAttachee, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(slotEditAttachee(const QString &)));
        connect(m_ui.pSpinBoxRadius, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxOffset, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxGrowDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxShrinkDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));

    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CLaserRadiatorAimingEntity*>(pEntity);

        m_ui.pSpinBoxRadius->setValue(m_pEntity->GetRadius());
        m_ui.pSpinBoxOffset->setValue(m_pEntity->GetOffset());
        m_ui.pSpinBoxGrowDuration->setValue(m_pEntity->GetGrowDuration());
        m_ui.pSpinBoxShrinkDuration->setValue(m_pEntity->GetShrinkDuration());

        UpdateEntitiesComboBox();

        UpdateAttacheeComboBox();
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:
    
    virtual void slotSettingsChanged()
    {
        if (m_ui.pSpinBoxRadius->hasFocus())
            m_pEntity->SetRadius(m_ui.pSpinBoxRadius->value());

        if (m_ui.pSpinBoxOffset->hasFocus())
            m_pEntity->SetOffset(m_ui.pSpinBoxOffset->value());

        if (m_ui.pSpinBoxGrowDuration->hasFocus())
            m_pEntity->SetGrowDuration(m_ui.pSpinBoxGrowDuration->value());

        if (m_ui.pSpinBoxShrinkDuration->hasFocus())
            m_pEntity->SetShrinkDuration(m_ui.pSpinBoxShrinkDuration->value());
    }

    virtual void slotAttachEntity()
    {
        const QString& strCurrentEntity = m_ui.pComboAttachEntity->currentText();

        if (strCurrentEntity.isEmpty())
            return;

        env::CEntity* pEntity = env::gEngine->GetEntityMgr()->GetEntity(strCurrentEntity.toStdString());

        if (!pEntity)
            return;

        m_pEntity->AddTargetEntity(pEntity);

        UpdateEntitiesComboBox();

        UpdateAttacheeComboBox();
    }

    virtual void slotDetachEntity()
    {
        const QString& strCurrentEntity = m_ui.pComboEditAttachee->currentText();

        if (strCurrentEntity.isEmpty())
            return;

        m_pEntity->RemoveTargetEntity(env::gEngine->GetEntityMgr()->GetEntity(strCurrentEntity.toStdString()));

        UpdateEntitiesComboBox();

        UpdateAttacheeComboBox();
    }

private:

    void UpdateAttacheeComboBox()
    {
        m_ui.pComboEditAttachee->clear();

        const std::set<env::CEntity*>& attachees = m_pEntity->GetTargetEntities();

        for (std::set<env::CEntity*>::const_iterator it = attachees.begin(); it != attachees.end(); ++it)
        {
            m_ui.pComboEditAttachee->addItem((*it)->GetName().c_str());
        }

    }

    void UpdateEntitiesComboBox()
    {
        m_ui.pComboAttachEntity->clear();

        std::list< env::CEntity* > entities;
        env::gEngine->GetEntityMgr()->GetEntities(entities);
        const std::set<env::CEntity*>& targetEntities = m_pEntity->GetTargetEntities();

        for (std::list< env::CEntity* >::iterator it = entities.begin(); it != entities.end(); ++it)
        {

            if (*it == m_pEntity || targetEntities.find(*it) != targetEntities.end())
                continue;

            m_ui.pComboAttachEntity->addItem((*it)->GetName().c_str());
        }
    }
private:

    env::CLaserRadiatorAimingEntity* m_pEntity;

};