/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/SkyFogPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Entity/MeshEntity.h>
#define UNICODE 

class CSkyFogPanel : public CPanel<Ui::SkyFogPanel>
{
    Q_OBJECT

public:
    CSkyFogPanel(QWidget* pForm)
        : CPanel<Ui::SkyFogPanel>(pForm)
    {
        connect(m_ui.pBtnFogColor, SIGNAL(pressed()), this, SLOT(slotChooseFogColor()));
        connect(m_ui.pBtnSkyColor, SIGNAL(pressed()), this, SLOT(slotChooseSkyColor()));
        connect(m_ui.pSpinBoxFogStartDistance, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxFogRange, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxCloudIntensity, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxEnergyfieldIntensity, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
         env::Fog fog = env::gEngine->GetWorldMgr()->GetScenario()->GetFog();
         env::Vec3 fogColor = env::Vec3(fog.initialFogColor);
         env::Vec3 skyColor = env::Vec3(fog.initialSkyColor);

        SetBtnColor(m_ui.pBtnSkyColor, env::DoGamma(skyColor));
        SetBtnColor(m_ui.pBtnFogColor, env::DoGamma(fogColor));

        m_ui.pSpinBoxCloudIntensity->setValue(env::gEngine->GetWorldMgr()->GetScenario()->GetCloudIntensity());
        m_ui.pSpinBoxEnergyfieldIntensity->setValue(env::gEngine->GetWorldMgr()->GetScenario()->GetEnergyFieldIntensity());
        m_ui.pSpinBoxFogRange->setValue(fog.fogRange);
        m_ui.pSpinBoxFogStartDistance->setValue(fog.fogStart);
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSettingsChanged()
    {
        if (m_ui.pSpinBoxCloudIntensity->hasFocus())
            env::gEngine->GetWorldMgr()->GetScenario()->SetCloudIntensity((float)m_ui.pSpinBoxCloudIntensity->value());

        if (m_ui.pSpinBoxEnergyfieldIntensity->hasFocus())
            env::gEngine->GetWorldMgr()->GetScenario()->SetInitialEnergyFieldIntensity((float)m_ui.pSpinBoxEnergyfieldIntensity->value());

        env::Fog& fog = env::gEngine->GetWorldMgr()->GetScenario()->GetFog();

        if (m_ui.pSpinBoxFogRange->hasFocus())
            fog.fogRange = (float)m_ui.pSpinBoxFogRange->value();

        if (m_ui.pSpinBoxFogStartDistance->hasFocus())
            fog.fogStart = (float)m_ui.pSpinBoxFogStartDistance->value();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotFogColorChanged(const QColor& color)
    {
        env::Fog& fog = env::gEngine->GetWorldMgr()->GetScenario()->GetFog();

        env::Vec3 lightColor = FromQColor(color);

        env::UndoGamma(lightColor);

        fog.initialFogColor = env::Vec4(lightColor, 1.0f);
        fog.fogColor = fog.initialFogColor;

        SetBtnColor(m_ui.pBtnFogColor, lightColor);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseFogColor()
    {
        env::Fog& fog = env::gEngine->GetWorldMgr()->GetScenario()->GetFog();

        env::Vec3 lightColor = env::Vec3(fog.initialFogColor);

        env::DoGamma(lightColor);

        gEditor->AcquireColorDialog(this, SLOT(slotFogColorChanged(const QColor&)), lightColor, "Choose Fog Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotSkyColorChanged(const QColor& color)
    {
        env::Fog& fog = env::gEngine->GetWorldMgr()->GetScenario()->GetFog();

        env::Vec3 lightColor = FromQColor(color);

        env::UndoGamma(lightColor);

        fog.initialSkyColor = lightColor;
        fog.skyColor = fog.initialSkyColor;

        SetBtnColor(m_ui.pBtnSkyColor, lightColor);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseSkyColor()
    {
        env::Fog& fog = env::gEngine->GetWorldMgr()->GetScenario()->GetFog();

        env::Vec3 lightColor = env::Vec3(fog.initialSkyColor);

        env::DoGamma(lightColor);

        gEditor->AcquireColorDialog(this, SLOT(slotSkyColorChanged(const QColor&)), lightColor, "Choose Sky Color");
    }

};