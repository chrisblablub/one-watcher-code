/*
* ============================================================
* Copyright (C) Christian Hamm. All Rights Reserved.
* ============================================================
*/


#pragma once

#include <QEditor/UIGen/FadeEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>


#undef UNICODE
#include <Engine/Entity/FadeEntity.h>
#define UNICODE 


class CFadeEntityPanel : public CPanel<Ui::FadeEntityPanel>
{
    Q_OBJECT

public:
    CFadeEntityPanel(QWidget* pForm)
        : CPanel<Ui::FadeEntityPanel>(pForm)
        , m_pEntity(nullptr)
    {
        connect(m_ui.pCheckBoxTemplate, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));

        connect(m_ui.pSpinBoxDuration, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxDelay, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxTopBegin, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxTopEnd, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxLeftBegin, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxLeftEnd, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxBottomBegin, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxBottomEnd, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxRightBegin, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxRightEnd, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));

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

        m_pEntity = static_cast<env::CFadeEntity*>(pEntity);

        UpdateUI();
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        env::CFadeEntity::FadeFrameParams params = m_pEntity->GetDefaultFadeFrameParams();

        if (m_ui.pSpinBoxDuration->hasFocus())
            params.m_fadeFrameDuration = (float)m_ui.pSpinBoxDuration->value();

        if (m_ui.pSpinBoxDelay->hasFocus())
            params.m_fadeFrameDelay = (float)m_ui.pSpinBoxDelay->value();

        if (m_ui.pSpinBoxTopBegin->hasFocus())
            params.m_fadeFrameParamsBegin.z = (float)m_ui.pSpinBoxTopBegin->value();

        if (m_ui.pSpinBoxTopEnd->hasFocus())
            params.m_fadeFrameParamsEnd.z = (float)m_ui.pSpinBoxTopEnd->value();

        if (m_ui.pSpinBoxLeftBegin->hasFocus())
            params.m_fadeFrameParamsBegin.x = (float)m_ui.pSpinBoxLeftBegin->value();

        if (m_ui.pSpinBoxLeftEnd->hasFocus())
            params.m_fadeFrameParamsEnd.x = m_ui.pSpinBoxLeftEnd->value();

        if (m_ui.pSpinBoxBottomBegin->hasFocus())
            params.m_fadeFrameParamsBegin.w = m_ui.pSpinBoxBottomBegin->value();

        if (m_ui.pSpinBoxBottomEnd->hasFocus())
            params.m_fadeFrameParamsEnd.w = m_ui.pSpinBoxBottomEnd->value();

        if (m_ui.pSpinBoxRightBegin->hasFocus())
            params.m_fadeFrameParamsBegin.y = m_ui.pSpinBoxRightBegin->value();

        if (m_ui.pSpinBoxRightEnd->hasFocus())
            params.m_fadeFrameParamsEnd.y = m_ui.pSpinBoxRightEnd->value();

        m_pEntity->SetDefaultFadeFrameParams(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotCheckBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        m_pEntity->SetInitFromTemplate(m_ui.pCheckBoxTemplate->isChecked());
    }

    //-----------------------------------------------------------------------------------
//     virtual void slotCheckBoxSettingsChanged()
//     {
//         if (!m_pEntity)
//             return;
// 
//         env::StripeParameters params = m_pEntity->GetStripeParameters();
// 
//         params.m_bGrowVertically = m_ui.pCheckBoxGrowVertically->isChecked();
//         params.m_bGrowAlternatingly = m_ui.pCheckBoxGrowAlternatingly->isChecked();
//         params.m_bFadeOut = m_ui.pRadioBtnFadeOut->isChecked();
// 
//         m_pEntity->SetStripeParameters(params);
//         m_pEntity->SetInitFromTemplate(m_ui.pCheckBoxTemplate->isChecked());
//     }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseTopStartColor()
    {
        if (!m_pEntity)
            return;

        env::CFadeEntity::FadeFrameParams params = m_pEntity->GetDefaultFadeFrameParams();

        env::DoGamma(params.m_fadeFrameBeginColorTop);

        gEditor->AcquireColorDialog(this, SLOT(slotTopStartColorChanged(const QColor&)), params.m_fadeFrameBeginColorTop, "Choose Top Start Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotTopStartColorChanged(const QColor& color)
    {
        env::Vec4 c = FromQColorAlpha(color);

        SetBtnColor(m_ui.pBtnTopStartColor, c);

        env::CFadeEntity::FadeFrameParams params = m_pEntity->GetDefaultFadeFrameParams();
        params.m_fadeFrameBeginColorTop = env::UndoGamma(c);
        m_pEntity->SetDefaultFadeFrameParams(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseTopEndColor()
    {
        if (!m_pEntity)
            return;

        env::CFadeEntity::FadeFrameParams params = m_pEntity->GetDefaultFadeFrameParams();

        env::DoGamma(params.m_fadeFrameEndColorTop);

        gEditor->AcquireColorDialog(this, SLOT(slotTopEndColorChanged(const QColor&)), params.m_fadeFrameEndColorTop, "Choose Top End Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotTopEndColorChanged(const QColor& color)
    {
        env::Vec4 c = FromQColorAlpha(color);

        SetBtnColor(m_ui.pBtnTopEndColor, c);

        env::CFadeEntity::FadeFrameParams params = m_pEntity->GetDefaultFadeFrameParams();
        params.m_fadeFrameEndColorTop = env::UndoGamma(c);
        m_pEntity->SetDefaultFadeFrameParams(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseBotStartColor()
    {
        if (!m_pEntity)
            return;

        env::CFadeEntity::FadeFrameParams params = m_pEntity->GetDefaultFadeFrameParams();

        env::DoGamma(params.m_fadeFrameBeginColorBot);

        gEditor->AcquireColorDialog(this, SLOT(slotBotStartColorChanged(const QColor&)), params.m_fadeFrameBeginColorBot, "Choose Bottom Start Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotBotStartColorChanged(const QColor& color)
    {
        env::Vec4 c = FromQColorAlpha(color);

        SetBtnColor(m_ui.pBtnBotStartColor, c);

        env::CFadeEntity::FadeFrameParams params = m_pEntity->GetDefaultFadeFrameParams();
        params.m_fadeFrameBeginColorBot = env::UndoGamma(c);
        m_pEntity->SetDefaultFadeFrameParams(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseBotEndColor()
    {
        if (!m_pEntity)
            return;

        env::CFadeEntity::FadeFrameParams params = m_pEntity->GetDefaultFadeFrameParams();

        env::DoGamma(params.m_fadeFrameEndColorBot);

        gEditor->AcquireColorDialog(this, SLOT(slotBotEndColorChanged(const QColor&)), params.m_fadeFrameEndColorBot, "Choose Bottom End Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotBotEndColorChanged(const QColor& color)
    {
        env::Vec4 c = FromQColorAlpha(color);

        SetBtnColor(m_ui.pBtnBotEndColor, c);

        env::CFadeEntity::FadeFrameParams params = m_pEntity->GetDefaultFadeFrameParams();
        params.m_fadeFrameEndColorBot = env::UndoGamma(c);
        m_pEntity->SetDefaultFadeFrameParams(params);
    }

    //-----------------------------------------------------------------------------------
    virtual void SaveToMeshTemplate()
    {
        QString strFilename = QFileDialog::getSaveFileName(m_ui.qEditorPanelBox, "Save Fades Template", "../Resources/Fades", "Fades Templates (*.fds)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        m_pEntity->SaveTemplate(strFilename.toStdString(), "../Fades/" + fi.fileName().toStdString());

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void LoadFromMeshTemplate()
    {
        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Save Fades Template", "../Resources/Fades", "Fades Templates (*.fds)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        m_pEntity->LoadTemplate("../Fades/" + fi.fileName().toStdString());

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    void UpdateUI()
    {
        const env::CFadeEntity::FadeFrameParams& params = m_pEntity->GetDefaultFadeFrameParams();

        m_ui.pCheckBoxTemplate->setChecked(m_pEntity->GetInitFromTemplate());

        m_ui.pSpinBoxDuration->setValue(params.m_fadeFrameDuration);
        m_ui.pSpinBoxDelay->setValue(params.m_fadeFrameDelay);
        m_ui.pSpinBoxTopBegin->setValue(params.m_fadeFrameParamsBegin.z);
        m_ui.pSpinBoxTopEnd->setValue(params.m_fadeFrameParamsEnd.z);
        m_ui.pSpinBoxLeftBegin->setValue(params.m_fadeFrameParamsBegin.x);
        m_ui.pSpinBoxLeftEnd->setValue(params.m_fadeFrameParamsEnd.x);
        m_ui.pSpinBoxBottomBegin->setValue(params.m_fadeFrameParamsBegin.w);
        m_ui.pSpinBoxBottomEnd->setValue(params.m_fadeFrameParamsEnd.w);
        m_ui.pSpinBoxRightBegin->setValue(params.m_fadeFrameParamsBegin.y);
        m_ui.pSpinBoxRightEnd->setValue(params.m_fadeFrameParamsEnd.y);

        env::Vec4 topStartColor = params.m_fadeFrameBeginColorTop;
        env::Vec4 topEndColor = params.m_fadeFrameEndColorTop;
        env::Vec4 botStartColor = params.m_fadeFrameBeginColorBot;
        env::Vec4 botEndColor = params.m_fadeFrameEndColorBot;

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

    env::CFadeEntity* m_pEntity;
};