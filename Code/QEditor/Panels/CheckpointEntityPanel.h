/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/CheckpointEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/CheckpointEntity.h>
#define UNICODE 


class CCheckpointEntityPanel : public CPanel<Ui::CheckpointEntityPanel>
{
    Q_OBJECT

public:
    CCheckpointEntityPanel(QWidget* pForm)
        : CPanel<Ui::CheckpointEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pLineEditNextLevel, SIGNAL(textChanged(const QString&)), this, SLOT(slotRadioButtonClicked()));
        connect(m_ui.pCheckBoxFinishChapter, SIGNAL(clicked(bool)), this, SLOT(slotRadioButtonClicked()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CCheckpointDestinatonEntity*>(pEntity);

        m_ui.pLineEditNextLevel->setText(m_pEntity->GetNextLevel().c_str());
        m_ui.pCheckBoxFinishChapter->setChecked(m_pEntity->GetFinishChapter());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotRadioButtonClicked()
    {
        m_pEntity->SetFinishChapter(m_ui.pCheckBoxFinishChapter->isChecked());
        m_pEntity->SetNextLevel(m_ui.pLineEditNextLevel->text().toStdString());
    }


private:

    env::CCheckpointDestinatonEntity* m_pEntity;

};