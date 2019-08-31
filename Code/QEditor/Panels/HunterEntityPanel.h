/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/HunterEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/HunterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#define UNICODE 


class CHunterEntityPanel : public CPanel<Ui::HunterEntityPanel>
{
    Q_OBJECT

public:
    CHunterEntityPanel(QWidget* pForm)
        : CPanel<Ui::HunterEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pBtnSetTarget, SIGNAL(pressed()), this, SLOT(slotSetTarget()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CHuntingHunterEntity*>(pEntity);

        UpdatePathComboBox();
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSetTarget()
    {
        if (!m_pEntity)
            return;

        const QString& strCurrentEntity = m_ui.pComboBoxSpawnTarget->currentText();

        if (strCurrentEntity.isEmpty())
            return;

        if (strCurrentEntity == "<No Target>")
        {
            m_pEntity->SetTarget(0);

            return;
        }

        env::CEntity* pEntity = (env::gEngine->GetEntityMgr()->GetEntity(strCurrentEntity.toStdString()));

        if (!pEntity)
            return;

        m_pEntity->SetTarget(pEntity);
    }

private:

    void UpdatePathComboBox()
    {
        m_ui.pComboBoxSpawnTarget->clear();

        std::list< std::string > entities;
        env::gEngine->GetEntityMgr()->GetEntities(entities);

        for (std::list< std::string >::iterator it = entities.begin(); it != entities.end(); ++it)
        {
            std::string strEntityName = *it;

            if (strEntityName == m_pEntity->GetName())
                continue;

            m_ui.pComboBoxSpawnTarget->addItem(strEntityName.c_str());
        }

        if (m_pEntity->GetTarget())
        {
            m_ui.pComboBoxSpawnTarget->setCurrentText(m_pEntity->GetName().c_str());
        }
        else
        {
            m_ui.pComboBoxSpawnTarget->addItem("<No Target>");
            m_ui.pComboBoxSpawnTarget->setCurrentText("<No Target>");
        }
    }

private:

    env::CHuntingHunterEntity* m_pEntity;

};