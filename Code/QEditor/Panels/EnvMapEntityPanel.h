/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/


#pragma once

#include <QEditor/UIGen/EnvMapEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>


#undef UNICODE
#include <Engine/Entity/EnvMapEntity.h>
#define UNICODE 


class CEnvMapEntityPanel : public CPanel<Ui::EnvMapEntityPanel>
{
    Q_OBJECT

public:
    CEnvMapEntityPanel(QWidget* pForm)
        : CPanel<Ui::EnvMapEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pCheckBoxEnabled, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pBtnChooseEnvMap, SIGNAL(pressed()), this, SLOT(slotChooseTexture()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CEnvMapEntity*>(pEntity);

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

public slots:

    void UpdateUI()
    {
        if (!m_pEntity->GetEnvironmentMap())
            return;

        std::string str;
        env::GetNameFromFilename(m_pEntity->GetEnvironmentMap()->GetFilename(), str);
        m_ui.pCheckBoxEnabled->setChecked(m_pEntity->GetEnabled());
        m_ui.pBtnChooseEnvMap->setText(str.c_str());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotCheckBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        m_pEntity->SetEnabled(m_ui.pCheckBoxEnabled->isChecked());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseTexture()
    {
        if (!m_pEntity)
            return;

        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Load Environment Map", "../Resources/Textures/Skies", "Textures (*.*)");

        QFileInfo fi(strFilename);

        m_pEntity->SetEnvironmentMap("Skies/" + fi.fileName().toStdString());

        UpdateUI();
    }

private:

    env::CEnvMapEntity* m_pEntity;
};