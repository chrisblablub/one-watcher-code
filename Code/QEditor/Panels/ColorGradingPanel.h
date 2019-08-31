/*
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/ColorGradingPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Entity/MeshEntity.h>
#define UNICODE 


class CColorGradingPanel : public CPanel<Ui::ColorGradingPanel>
{
    Q_OBJECT

public:
    CColorGradingPanel(QWidget* pForm)
        : CPanel<Ui::ColorGradingPanel>(pForm)
    {
        connect(m_ui.pSliderColorBalanceR, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSliderColorBalanceG, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSliderColorBalanceB, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSliderColorBrightnessR, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSliderColorBrightnessG, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSliderColorBrightnessB, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSliderHue, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSliderSaturation, SIGNAL(valueChanged(int)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxBloomThreshold, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxBloomScaling, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));

        connect(m_ui.pBtnResetBalanceR, SIGNAL(pressed()), this, SLOT(slotResetColorBalanceR()));
        connect(m_ui.pBtnResetBalanceG, SIGNAL(pressed()), this, SLOT(slotResetColorBalanceG()));
        connect(m_ui.pBtnResetBalanceB, SIGNAL(pressed()), this, SLOT(slotResetColorBalanceB()));
        connect(m_ui.pBtnResetBrightnessR, SIGNAL(pressed()), this, SLOT(slotResetColorBrightnessR()));
        connect(m_ui.pBtnResetBrightnessG, SIGNAL(pressed()), this, SLOT(slotResetColorBrightnessG()));
        connect(m_ui.pBtnResetBrightnessB, SIGNAL(pressed()), this, SLOT(slotResetColorBrightnessB()));
        connect(m_ui.pBtnResetHue, SIGNAL(pressed()), this, SLOT(slotResetHue()));
        connect(m_ui.pBtnResetSaturation, SIGNAL(pressed()), this, SLOT(slotResetSaturation()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    void UpdateUI()
    {
        m_ui.pSliderHue->setValue((int)(env::gEngine->GetWorldMgr()->GetScenario()->GetHueRotation()));
        m_ui.pSliderSaturation->setValue((int)(env::gEngine->GetWorldMgr()->GetScenario()->GetSaturation() * 99.0f));

        const env::Vec3& colorBalanace = env::gEngine->GetWorldMgr()->GetScenario()->GetColorBalance();

        m_ui.pSliderColorBalanceR->setValue((int)(colorBalanace.x * 99.0f));
        m_ui.pSliderColorBalanceG->setValue((int)(colorBalanace.y * 99.0f));
        m_ui.pSliderColorBalanceB->setValue((int)(colorBalanace.z * 99.0f));

        const env::Vec3& colorBrightness = env::gEngine->GetWorldMgr()->GetScenario()->GetColorBrightness();

        m_ui.pSliderColorBrightnessR->setValue((int)(colorBrightness.x * 99.0f));
        m_ui.pSliderColorBrightnessG->setValue((int)(colorBrightness.y * 99.0f));
        m_ui.pSliderColorBrightnessB->setValue((int)(colorBrightness.z * 99.0f));

        m_ui.pSpinBoxBloomThreshold->setValue(env::gEngine->GetWorldMgr()->GetScenario()->GetBloomBrightpassThreshold());
        m_ui.pSpinBoxBloomScaling->setValue(env::gEngine->GetWorldMgr()->GetScenario()->GetBloomScale());
    }

    public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSettingsChanged()
    {
        env::Vec3 colorBalance(m_ui.pSliderColorBalanceR->value() / 99.0f,
                               m_ui.pSliderColorBalanceG->value() / 99.0f,
                               m_ui.pSliderColorBalanceB->value() / 99.0f);

        env::Vec3 colorBrightness(m_ui.pSliderColorBrightnessR->value() / 99.0f,
                                  m_ui.pSliderColorBrightnessG->value() / 99.0f,
                                  m_ui.pSliderColorBrightnessB->value() / 99.0f);

        if (m_ui.pSliderHue->hasFocus())
            env::gEngine->GetWorldMgr()->GetScenario()->SetHueRotation(m_ui.pSliderHue->value());

        if (m_ui.pSliderSaturation->hasFocus())
            env::gEngine->GetWorldMgr()->GetScenario()->SetSaturation(m_ui.pSliderSaturation->value() / 99.0f);

        if (m_ui.pSliderColorBalanceR->hasFocus() || m_ui.pSliderColorBalanceG->hasFocus() || m_ui.pSliderColorBalanceB->hasFocus())
            env::gEngine->GetWorldMgr()->GetScenario()->SetColorBalance(colorBalance);

        if (m_ui.pSliderColorBrightnessR->hasFocus() || m_ui.pSliderColorBrightnessG->hasFocus() || m_ui.pSliderColorBrightnessB->hasFocus())
            env::gEngine->GetWorldMgr()->GetScenario()->SetColorBrightness(colorBrightness);

        if (m_ui.pSpinBoxBloomThreshold->hasFocus())
            env::gEngine->GetWorldMgr()->GetScenario()->SetBloomBrightpassThreshold(m_ui.pSpinBoxBloomThreshold->value());

        if (m_ui.pSpinBoxBloomScaling->hasFocus())
            env::gEngine->GetWorldMgr()->GetScenario()->SetBloomScale(m_ui.pSpinBoxBloomScaling->value());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetColorBalanceR()
    {
        env::Vec3 colorBalance = env::gEngine->GetWorldMgr()->GetScenario()->GetColorBalance();

        colorBalance.r = 1.0f;

        env::gEngine->GetWorldMgr()->GetScenario()->SetColorBalance(colorBalance);

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetColorBalanceG()
    {
        env::Vec3 colorBalance = env::gEngine->GetWorldMgr()->GetScenario()->GetColorBalance();

        colorBalance.g = 1.0f;

        env::gEngine->GetWorldMgr()->GetScenario()->SetColorBalance(colorBalance);

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetColorBalanceB()
    {
        env::Vec3 colorBalance = env::gEngine->GetWorldMgr()->GetScenario()->GetColorBalance();

        colorBalance.b = 1.0f;

        env::gEngine->GetWorldMgr()->GetScenario()->SetColorBalance(colorBalance);

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetColorBrightnessR()
    {
        env::Vec3 colorBrightness = env::gEngine->GetWorldMgr()->GetScenario()->GetColorBrightness();

        colorBrightness.r = 0.0f;

        env::gEngine->GetWorldMgr()->GetScenario()->SetColorBrightness(colorBrightness);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetColorBrightnessG()
    {
        env::Vec3 colorBrightness = env::gEngine->GetWorldMgr()->GetScenario()->GetColorBrightness();

        colorBrightness.g = 0.0f;

        env::gEngine->GetWorldMgr()->GetScenario()->SetColorBrightness(colorBrightness);

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetColorBrightnessB()
    {
        env::Vec3 colorBrightness = env::gEngine->GetWorldMgr()->GetScenario()->GetColorBrightness();

        colorBrightness.b = 0.0f;

        env::gEngine->GetWorldMgr()->GetScenario()->SetColorBrightness(colorBrightness);

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetHue()
    {
        env::gEngine->GetWorldMgr()->GetScenario()->SetHueRotation(0.0f);

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetSaturation()
    {
        env::gEngine->GetWorldMgr()->GetScenario()->SetSaturation(1.0f);

        UpdateUI();
    }

};