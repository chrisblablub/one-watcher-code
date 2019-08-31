/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/SilhouettesPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Entity/MeshEntity.h>
#define UNICODE 


class CSilhouettesPanel : public CPanel<Ui::SilhouettesPanel>
{
    Q_OBJECT

public:
    CSilhouettesPanel(QWidget* pForm)
        : CPanel<Ui::SilhouettesPanel>(pForm)
    {
        connect(m_ui.pSpinBoxShadingIntensity, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pCheckBoxSilhouettes, SIGNAL(clicked(bool)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pBtnShadingColor, SIGNAL(pressed()), this, SLOT(slotChooseColor()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        m_ui.pSpinBoxShadingIntensity->setValue(env::gEngine->GetWorldMgr()->GetScenario()->GetSilhouetteIntensity());
        m_ui.pCheckBoxSilhouettes->setChecked(env::gEngine->GetWorldMgr()->GetScenario()->GetSilhouettes());

        const env::Vec4& color = env::gEngine->GetWorldMgr()->GetScenario()->GetSilhouetteColor();

        QString strBackgroundColor = QString::number(color.x * 255) + ", " +
            QString::number(color.y * 255) + ", " +
            QString::number(color.z * 255) + ", 1.0";

        m_ui.pBtnShadingColor->setStyleSheet("background-color:rgba(" + strBackgroundColor + ")");
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSettingsChanged()
    {
        if (m_ui.pSpinBoxShadingIntensity->hasFocus())
            env::gEngine->GetWorldMgr()->GetScenario()->SetSilhouetteIntensity(m_ui.pSpinBoxShadingIntensity->value());

        env::gEngine->GetWorldMgr()->GetScenario()->SetSilhouettes(m_ui.pCheckBoxSilhouettes->isChecked());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseColor()
    {

        env::Vec4 spriteColor = env::gEngine->GetWorldMgr()->GetScenario()->GetSilhouetteColor();

        QColor color = QColorDialog::getColor(QColor(spriteColor.x * 255.0f, spriteColor.y * 255.0f, spriteColor.z * 255.0f), GetWidget(), "Choose Color");

        spriteColor = env::Vec4(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f, spriteColor.w);

        env::gEngine->GetWorldMgr()->GetScenario()->SetSilhouetteColor(spriteColor);

        QString strBackgroundColor = QString::number(spriteColor.x * 255) + ", " +
            QString::number(spriteColor.y * 255) + ", " +
            QString::number(spriteColor.z * 255) + ", 1.0";

        m_ui.pBtnShadingColor->setStyleSheet("background-color:rgba(" + strBackgroundColor + ")");
    }
};