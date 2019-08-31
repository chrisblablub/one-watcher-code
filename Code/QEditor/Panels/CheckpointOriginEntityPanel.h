/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/


#pragma once

#include <QEditor/UIGen/CheckpointOriginEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/CheckpointEntity.h>
#define UNICODE 


class CCheckpointOriginEntityPanel : public CPanel<Ui::CheckpointOriginEntityPanel>
{
    Q_OBJECT

public:
    CCheckpointOriginEntityPanel(QWidget* pForm)
        : CPanel<Ui::CheckpointOriginEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pCheckBoxDisabled, SIGNAL(clicked(bool)), this, SLOT(slotRadioButtonClicked()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CCheckpointOriginEntity*>(pEntity);

        m_ui.pCheckBoxDisabled->setChecked(m_pEntity->GetDisabledInitial());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotRadioButtonClicked()
    {
        m_pEntity->SetDisabledInitial(m_ui.pCheckBoxDisabled->isChecked());
    }


private:

    env::CCheckpointOriginEntity* m_pEntity;

};