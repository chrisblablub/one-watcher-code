/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/


#pragma once

#include <QEditor/UIGen/StripesEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>


#undef UNICODE
#include <Engine/Entity/StripesEntity.h>
#define UNICODE 


class CStripesEntityPanel : public CPanel<Ui::StripesEntityPanel>
{
    Q_OBJECT

public:
    CStripesEntityPanel(QWidget* pForm)
        : CPanel<Ui::StripesEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pCheckBoxGrowVertically, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxGrowAlternatingly, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pRadioBtnFadeOut, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pRadioBtnFadeIn, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));

        connect(m_ui.pSpinBoxTimeDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxTimeOffset, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxFadeDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxStartWidth, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxEndWidth, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxSlices, SIGNAL(valueChanged(int)), this, SLOT(slotSpinBoxSettingsChanged()));

        connect(m_ui.pBtnTopStartColor, SIGNAL(pressed()), this, SLOT(slotChooseTopStartColor()));
        connect(m_ui.pBtnTopEndColor, SIGNAL(pressed()), this, SLOT(slotChooseTopEndColor()));
        connect(m_ui.pBtnBotStartColor, SIGNAL(pressed()), this, SLOT(slotChooseBotStartColor()));
        connect(m_ui.pBtnBotEndColor, SIGNAL(pressed()), this, SLOT(slotChooseBotEndColor()));

        connect(m_ui.pBtnSaveTemplate, SIGNAL(pressed()), this, SLOT(SaveToMeshTemplate()));
        connect(m_ui.pBtnLoadTemplate, SIGNAL(pressed()), this, SLOT(LoadFromMeshTemplate()));
        connect(m_ui.pCheckBoxTemplate, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CStripesEntity*>(pEntity);

        UpdateUI();
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        env::StripeParameters params = m_pEntity->GetStripeParameters();

        if (m_ui.pSpinBoxTimeDuration->hasFocus())
            params.m_timeSpawnDuration = (float)m_ui.pSpinBoxTimeDuration->value();

        if (m_ui.pSpinBoxTimeOffset->hasFocus())
            params.m_timeSpawnOffset = (float)m_ui.pSpinBoxTimeOffset->value();

        if (m_ui.pSpinBoxFadeDuration->hasFocus())
            params.m_timeFadeDuration = (float)m_ui.pSpinBoxFadeDuration->value();

        if (m_ui.pSpinBoxStartWidth->hasFocus())
            params.m_startWidth = m_ui.pSpinBoxStartWidth->value();

        if (m_ui.pSpinBoxEndWidth->hasFocus())
            params.m_endWidth = m_ui.pSpinBoxEndWidth->value();

        if (m_ui.pSpinBoxSlices->hasFocus())
            params.m_numSlices = m_ui.pSpinBoxSlices->value();

        m_pEntity->SetStripeParameters(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotCheckBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        env::StripeParameters params = m_pEntity->GetStripeParameters();

        params.m_bGrowVertically = m_ui.pCheckBoxGrowVertically->isChecked();
        params.m_bGrowAlternatingly = m_ui.pCheckBoxGrowAlternatingly->isChecked();
        params.m_bFadeOut = m_ui.pRadioBtnFadeOut->isChecked();

        m_pEntity->SetStripeParameters(params);
        m_pEntity->SetInitFromTemplate(m_ui.pCheckBoxTemplate->isChecked());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseTopStartColor()
    {
        if (!m_pEntity)
            return;

        env::StripeParameters params = m_pEntity->GetStripeParameters();

        env::DoGamma(params.m_startTopColor);

        gEditor->AcquireColorDialog(this, SLOT(slotTopStartColorChanged(const QColor&)), params.m_startTopColor, "Choose Top Start Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotTopStartColorChanged(const QColor& color)
    {
        env::Vec4 c = FromQColorAlpha(color);

        SetBtnColor(m_ui.pBtnTopStartColor, c);

        env::StripeParameters params = m_pEntity->GetStripeParameters();
        params.m_startTopColor = env::UndoGamma(c);
        m_pEntity->SetStripeParameters(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseTopEndColor()
    {
        if (!m_pEntity)
            return;

        env::StripeParameters params = m_pEntity->GetStripeParameters();

        env::DoGamma(params.m_endTopColor);

        gEditor->AcquireColorDialog(this, SLOT(slotTopEndColorChanged(const QColor&)), params.m_endTopColor, "Choose Top End Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotTopEndColorChanged(const QColor& color)
    {
        env::Vec4 c = FromQColorAlpha(color);

        SetBtnColor(m_ui.pBtnTopEndColor, c);

        env::StripeParameters params = m_pEntity->GetStripeParameters();
        params.m_endTopColor = env::UndoGamma(c);
        m_pEntity->SetStripeParameters(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseBotStartColor()
    {
        if (!m_pEntity)
            return;

        env::StripeParameters params = m_pEntity->GetStripeParameters();

        env::DoGamma(params.m_startBotColor);

        gEditor->AcquireColorDialog(this, SLOT(slotBotStartColorChanged(const QColor&)), params.m_startBotColor, "Choose Bottom Start Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotBotStartColorChanged(const QColor& color)
    {
        env::Vec4 c = FromQColorAlpha(color);

        SetBtnColor(m_ui.pBtnBotStartColor, c);

        env::StripeParameters params = m_pEntity->GetStripeParameters();
        params.m_startBotColor = env::UndoGamma(c);
        m_pEntity->SetStripeParameters(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseBotEndColor()
    {
        if (!m_pEntity)
            return;

        env::StripeParameters params = m_pEntity->GetStripeParameters();

        env::DoGamma(params.m_endBotColor);

        gEditor->AcquireColorDialog(this, SLOT(slotBotEndColorChanged(const QColor&)), params.m_endBotColor, "Choose Bottom End Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotBotEndColorChanged(const QColor& color)
    {
        env::Vec4 c = FromQColorAlpha(color);

        SetBtnColor(m_ui.pBtnBotEndColor, c);

        env::StripeParameters params = m_pEntity->GetStripeParameters();
        params.m_endBotColor = env::UndoGamma(c);
        m_pEntity->SetStripeParameters(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void SaveToMeshTemplate()
    {
        QString strFilename = QFileDialog::getSaveFileName(m_ui.qEditorPanelBox, "Save Stripes Template", "../Resources/Stripes", "Stripes Templates (*.stp)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        m_pEntity->SaveTemplate(strFilename.toStdString(), "../Stripes/" + fi.fileName().toStdString());

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void LoadFromMeshTemplate()
    {
        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Save Stripes Template", "../Resources/Stripes", "Stripes Templates (*.stp)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        m_pEntity->LoadTemplate("../Stripes/" + fi.fileName().toStdString());

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    void UpdateUI()
    {
        const env::StripeParameters& params = m_pEntity->GetStripeParameters();

        params.m_bFadeOut ? m_ui.pRadioBtnFadeOut->setChecked(true) : m_ui.pRadioBtnFadeIn->setChecked(true);

        m_ui.pCheckBoxGrowVertically->setChecked(params.m_bGrowVertically);
        m_ui.pCheckBoxGrowAlternatingly->setChecked(params.m_bGrowAlternatingly);
        m_ui.pSpinBoxTimeDuration->setValue(params.m_timeSpawnDuration);
        m_ui.pSpinBoxTimeOffset->setValue(params.m_timeSpawnOffset);
        m_ui.pSpinBoxFadeDuration->setValue(params.m_timeFadeDuration);
        m_ui.pSpinBoxStartWidth->setValue(params.m_startWidth);
        m_ui.pSpinBoxEndWidth->setValue(params.m_endWidth);
        m_ui.pSpinBoxSlices->setValue(params.m_numSlices);

        env::Vec4 topStartColor = params.m_startTopColor;
        env::Vec4 topEndColor = params.m_endTopColor;
        env::Vec4 botStartColor = params.m_startBotColor;
        env::Vec4 botEndColor = params.m_endBotColor;

        env::DoGamma(topStartColor);
        env::DoGamma(topEndColor);
        env::DoGamma(botStartColor);
        env::DoGamma(botEndColor);

        SetBtnColor(m_ui.pBtnTopStartColor, topStartColor);
        SetBtnColor(m_ui.pBtnTopEndColor, topEndColor);
        SetBtnColor(m_ui.pBtnBotStartColor, botStartColor);
        SetBtnColor(m_ui.pBtnBotEndColor, botEndColor);

        if (!m_pEntity->GetTemplate().empty())
            m_ui.pBtnLoadTemplate->setText(m_pEntity->GetTemplate().c_str());
        else
            m_ui.pBtnLoadTemplate->setText("Load template");

        m_ui.pCheckBoxTemplate->setChecked(m_pEntity->GetInitFromTemplate());
    }

private:

    env::CStripesEntity* m_pEntity;
};