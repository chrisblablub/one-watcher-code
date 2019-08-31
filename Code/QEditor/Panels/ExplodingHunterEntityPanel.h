/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/


#pragma once

#include <QEditor/UIGen/ExplodingHunterEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/HunterEntity.h>
#include <Engine/Core/Engine.h>
#define UNICODE 




class CExplodingHunterEntityPanel : public CPanel<Ui::ExplodingHunterEntityPanel>
{
    Q_OBJECT

public:
    CExplodingHunterEntityPanel(QWidget* pForm)
        : CPanel<Ui::ExplodingHunterEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pSpinBoxSpawnRate, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CExplodingHunterEmitterEntity*>(pEntity);

        m_ui.pSpinBoxSpawnRate->setValue(m_pEntity->GetEmitRate());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxSpawnRate->hasFocus())
            m_pEntity->SetEmitRate(m_ui.pSpinBoxSpawnRate->value());
    }


private:
    env::CExplodingHunterEmitterEntity* m_pEntity;

};