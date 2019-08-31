/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/PlatformEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/PlatformEntity.h>
#define UNICODE 




class CPlatformEntityPanel : public CPanel<Ui::PlatformEntityPanel>
{
    Q_OBJECT

public:
    CPlatformEntityPanel(QWidget* pForm)
        : CPanel<Ui::PlatformEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pRadioBtnTop, SIGNAL(clicked(bool)), this, SLOT(slotRadioBtnChecked()));
        connect(m_ui.pRadioBtnBottom, SIGNAL(clicked(bool)), this, SLOT(slotRadioBtnChecked()));
        connect(m_ui.pRadioBtnTopBottom, SIGNAL(clicked(bool)), this, SLOT(slotRadioBtnChecked()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CPlatformEntity*>(pEntity);

        if (m_pEntity->GetType() == env::PLATFORM_TYPE_RACKWHEEL_TOP)
            m_ui.pRadioBtnTop->setChecked(true);
        else if (m_pEntity->GetType() == env::PLATFORM_TYPE_RACKWHEEL_BOTTOM)
            m_ui.pRadioBtnBottom->setChecked(true);
        else if (m_pEntity->GetType() == env::PLATFORM_TYPE_RACKWHEEL_TOPBOTTOM)
            m_ui.pRadioBtnTopBottom->setChecked(true);
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotRadioBtnChecked()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pRadioBtnTop->isChecked())
            m_pEntity->SetType(env::PLATFORM_TYPE_RACKWHEEL_TOP);
        else if (m_ui.pRadioBtnBottom->isChecked())
            m_pEntity->SetType(env::PLATFORM_TYPE_RACKWHEEL_BOTTOM);
        else if (m_ui.pRadioBtnTopBottom->isChecked())
            m_pEntity->SetType(env::PLATFORM_TYPE_RACKWHEEL_TOPBOTTOM);
    }


private:

    env::CPlatformEntity* m_pEntity;

};