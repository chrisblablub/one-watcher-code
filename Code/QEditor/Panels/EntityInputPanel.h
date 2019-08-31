/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/EntityInputPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>

#undef UNICODE
#include <Engine/Entity/Entity.h>
#include <Engine/Entity/EntityMgr.h>
#define UNICODE 


class CEntityInputPanel : public CPanel<Ui::EntityInputPanel>
{
    Q_OBJECT

public:
    CEntityInputPanel(QWidget* pForm)
        : CPanel<Ui::EntityInputPanel>(pForm)
        , m_pEntity(0)
    {
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = pEntity;

        initInputs();
    }


    //-----------------------------------------------------------------------------------
    void initInputs()
    {
        const env::CEntity::mapInputs& inputs = m_pEntity->GetInputs();

        m_ui.pTableInputs->reset();
        m_ui.pTableInputs->clear();
        m_ui.pTableInputs->setRowCount(0);
        m_ui.pTableInputs->setColumnCount(4);
        m_ui.pTableInputs->setHorizontalHeaderItem(0, new QTableWidgetItem("Sender"));
        m_ui.pTableInputs->setHorizontalHeaderItem(1, new QTableWidgetItem("Event"));
        m_ui.pTableInputs->setHorizontalHeaderItem(2, new QTableWidgetItem("Delay"));
        m_ui.pTableInputs->setHorizontalHeaderItem(3, new QTableWidgetItem("Key"));

        for (env::CEntity::mapInputs::const_iterator it = inputs.begin(); it != inputs.end(); ++it)
        {
            const  env::CEntity::EntityInput& input = it->second;

            env::CEntity* pSender = env::gEngine->GetEntityMgr()->GetEntity(input.strSender);

            env::CEntity::EntityOutput output;
            pSender->GetOutput(output, m_pEntity->GetName(), it->first);

            std::string strEventName;
            m_pEntity->GetEventName(strEventName, output.eventId);

            m_ui.pTableInputs->insertRow(m_ui.pTableInputs->rowCount());
            m_ui.pTableInputs->setItem(m_ui.pTableInputs->rowCount() - 1, 0, new QTableWidgetItem(input.strSender.c_str()));
            m_ui.pTableInputs->setItem(m_ui.pTableInputs->rowCount() - 1, 1, new QTableWidgetItem(strEventName.c_str()));
            m_ui.pTableInputs->setItem(m_ui.pTableInputs->rowCount() - 1, 2, new QTableWidgetItem(QString::number(output.delay)));
            m_ui.pTableInputs->setItem(m_ui.pTableInputs->rowCount() - 1, 3, new QTableWidgetItem(QString::number(output.key)));
        }

        m_ui.pTableInputs->resizeColumnsToContents();
    }


    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }


private:

    env::CEntity* m_pEntity;

};