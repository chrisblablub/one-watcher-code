/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/LightEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Entity/LightSpriteEntity.h>
#include <Engine/Scene/Light.h>
#define UNICODE 




class CLightEntityPanel : public CPanel<Ui::LightEntityPanel>
{
    Q_OBJECT

public:
    CLightEntityPanel(QWidget* pForm)
        : CPanel<Ui::LightEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pRadioBtnPoint, SIGNAL(clicked(bool)), this, SLOT(slotGeneralSettingsChanged()));
        connect(m_ui.pRadioBtnDirectional, SIGNAL(clicked(bool)), this, SLOT(slotGeneralSettingsChanged()));
        connect(m_ui.pCheckBoxSpecular, SIGNAL(clicked(bool)), this, SLOT(slotGeneralSettingsChanged()));
        connect(m_ui.pCheckBoxStatic, SIGNAL(clicked(bool)), this, SLOT(slotGeneralSettingsChanged()));
        connect(m_ui.pCheckBoxAffectsTerrain, SIGNAL(clicked(bool)), this, SLOT(slotGeneralSettingsChanged()));
        connect(m_ui.pCheckBoxAffectsMeshes, SIGNAL(clicked(bool)), this, SLOT(slotGeneralSettingsChanged()));
        connect(m_ui.pGroupBoxLightRays, SIGNAL(clicked(bool)), this, SLOT(slotLightRaysSettingsChanged()));
        connect(m_ui.pGroupBoxFlicker, SIGNAL(clicked(bool)), this, SLOT(slotFlickerSettingsChanged()));
        connect(m_ui.pCheckBoxWorldPosition, SIGNAL(clicked(bool)), this, SLOT(slotLightRaysSettingsChanged()));
        connect(m_ui.pSpinBoxIntensity, SIGNAL(valueChanged(double)), this, SLOT(slotLightRaysSettingsChanged()));
        connect(m_ui.pSpinBoxFalloff, SIGNAL(valueChanged(double)), this, SLOT(slotLightRaysSettingsChanged()));
        connect(m_ui.pSpinBoxRadius, SIGNAL(valueChanged(double)), this, SLOT(slotGeneralSettingsChanged()));
        connect(m_ui.pSpinBoxEnergy, SIGNAL(valueChanged(double)), this, SLOT(slotGeneralSettingsChanged()));
        connect(m_ui.pSpinBoxFlickerTime, SIGNAL(valueChanged(double)), this, SLOT(slotFlickerSettingsChanged()));
        connect(m_ui.pSpinBoxFlickerIntensity, SIGNAL(valueChanged(double)), this, SLOT(slotFlickerSettingsChanged()));
        connect(m_ui.pSpinBoxFlickerPauseTime, SIGNAL(valueChanged(double)), this, SLOT(slotFlickerSettingsChanged()));
        connect(m_ui.pSpinBoxFlickerRandomness, SIGNAL(valueChanged(double)), this, SLOT(slotFlickerSettingsChanged()));
        connect(m_ui.pBtnDiffuseColor, SIGNAL(pressed()), this, SLOT(slotDiffuseColorBtnClicked()));
        connect(m_ui.pBtnSpecularColor, SIGNAL(pressed()), this, SLOT(slotSpecularColorBtnClicked()));
        connect(m_ui.pBtnSaveTemplate, SIGNAL(pressed()), this, SLOT(SaveToTemplate()));
        connect(m_ui.pBtnLoadTemplate, SIGNAL(pressed()), this, SLOT(LoadFromTemplate()));
        connect(m_ui.pCheckBoxTemplate, SIGNAL(clicked(bool)), this, SLOT(slotGeneralSettingsChanged()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CLightSpriteEntity*>(pEntity);

        UpdateUI();
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }


    //-----------------------------------------------------------------------------------
    void UpdateUI()
    {
        if (m_pEntity->GetLight()->GetType() == env::LT_POINT)
            m_ui.pRadioBtnPoint->setChecked(true);
        else
            m_ui.pRadioBtnDirectional->setChecked(true);

        env::Vec3 diffuseColor = env::Vec3(m_pEntity->GetLight()->GetInitialDiffuseColor());
        env::Vec3 specularColor = env::Vec3(m_pEntity->GetLight()->GetInitialSpecularColor());

        SetBtnColor(m_ui.pBtnDiffuseColor, env::DoGamma(diffuseColor));
        SetBtnColor(m_ui.pBtnSpecularColor, env::DoGamma(specularColor));

        m_ui.pSpinBoxEnergy->setValue((double)m_pEntity->GetLight()->GetInitialEnergy());
        m_ui.pSpinBoxRadius->setValue((double)m_pEntity->GetLight()->GetRadius());
        m_ui.pCheckBoxSpecular->setChecked(m_pEntity->GetLight()->GetSpecular());
        m_ui.pCheckBoxStatic->setChecked(m_pEntity->GetLight()->GetStatic());
        m_ui.pCheckBoxAffectsTerrain->setChecked(m_pEntity->GetLight()->GetAffectsTerrain());
        m_ui.pCheckBoxAffectsMeshes->setChecked(m_pEntity->GetLight()->GetAffectsMeshes());

        m_ui.pGroupBoxLightRays->setChecked(m_pEntity->GetLight()->GetLightRays());
        m_ui.pCheckBoxOcclusionMask->setChecked(m_pEntity->GetLight()->GetLightRaysUseOcclusionMask());
        m_ui.pCheckBoxWorldPosition->setChecked(m_pEntity->GetLight()->GetLightRaysUseWorldOrigin());
        m_ui.pSpinBoxFalloff->setValue(m_pEntity->GetLight()->GetLightRaysDecay());
        m_ui.pSpinBoxIntensity->setValue(m_pEntity->GetLight()->GetLightRaysIllumDecay());

        m_ui.pGroupBoxFlicker->setChecked(m_pEntity->GetFlicker());
        m_ui.pSpinBoxFlickerIntensity->setValue(m_pEntity->GetFlickerAmount());
        m_ui.pSpinBoxFlickerPauseTime->setValue(m_pEntity->GetFlickerPauseTime());
        m_ui.pSpinBoxFlickerRandomness->setValue(m_pEntity->GetFlickerRandomness());
        m_ui.pSpinBoxFlickerTime->setValue(m_pEntity->GetFlickerTime());

        if (!m_pEntity->GetTemplate().empty())
            m_ui.pBtnLoadTemplate->setText(m_pEntity->GetTemplate().c_str());
        else
            m_ui.pBtnLoadTemplate->setText("Load light template");

        m_ui.pCheckBoxTemplate->setChecked(m_pEntity->GetInitFromTemplate());
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotDiffuseColorChanged(const QColor& color)
    {
        env::Vec3 lightColor = FromQColor(color);

        env::UndoGamma(lightColor);

        m_pEntity->GetLight()->SetInitialDiffuseColor(env::Vec4(lightColor, 1.0f));
        m_pEntity->GetSprite().SetColor(env::Vec4(lightColor, 1.0f));

        SetBtnColor(m_ui.pBtnDiffuseColor, lightColor);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotDiffuseColorBtnClicked()
    {
        if (!m_pEntity)
            return;

        env::Vec3 lightColor = env::Vec3(m_pEntity->GetLight()->GetInitialDiffuseColor());

        env::DoGamma(lightColor);

        gEditor->AcquireColorDialog(this, SLOT(slotDiffuseColorChanged(const QColor&)), lightColor, "Choose Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotSpecularColorChanged(const QColor& color)
    {
        env::Vec3 lightColor = FromQColor(color);

        env::UndoGamma(lightColor);

        m_pEntity->GetLight()->SetInitialSpecularColor(env::Vec4(lightColor, 1.0f));

        SetBtnColor(m_ui.pBtnSpecularColor, lightColor);
    }
    
    //-----------------------------------------------------------------------------------
    virtual void slotSpecularColorBtnClicked()
    {
        if (!m_pEntity)
            return;

        env::Vec3 lightColor = env::Vec3(m_pEntity->GetLight()->GetInitialSpecularColor());

        env::DoGamma(lightColor);

        gEditor->AcquireColorDialog(this, SLOT(slotSpecularColorChanged(const QColor&)), lightColor, "Choose Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotGeneralSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pRadioBtnDirectional->isChecked())
            m_pEntity->GetLight()->SetType(env::LT_DIRECTIONAL);
        else
            m_pEntity->GetLight()->SetType(env::LT_POINT);

        if (m_ui.pCheckBoxSpecular->hasFocus())
            m_pEntity->GetLight()->SetSpecular(m_ui.pCheckBoxSpecular->isChecked());

        if (m_ui.pCheckBoxStatic->hasFocus())
            m_pEntity->GetLight()->SetStatic(m_ui.pCheckBoxStatic->isChecked());

        if (m_ui.pCheckBoxTemplate->hasFocus())
            m_pEntity->SetInitFromTemplate(m_ui.pCheckBoxTemplate->isChecked());

        if (m_ui.pCheckBoxAffectsMeshes->hasFocus())
            m_pEntity->GetLight()->SetAffectsMeshes(m_ui.pCheckBoxAffectsMeshes->isChecked());

        if (m_ui.pSpinBoxRadius->hasFocus())
            m_pEntity->GetLight()->SetRadius(m_ui.pSpinBoxRadius->value());

        if (m_ui.pSpinBoxEnergy->hasFocus())
            m_pEntity->GetLight()->SetInitialEnergy(m_ui.pSpinBoxEnergy->value());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotLightRaysSettingsChanged()
    {
        if (!m_pEntity)
            return;

        m_pEntity->GetLight()->SetLightRays(m_ui.pGroupBoxLightRays->isChecked());
        m_pEntity->GetLight()->SetLightRaysUseOcclusionMask(m_ui.pCheckBoxOcclusionMask->isChecked());
        m_pEntity->GetLight()->SetLightRaysUseWorldOrigin(m_ui.pCheckBoxWorldPosition->isChecked());

        if (m_ui.pSpinBoxIntensity->hasFocus())
            m_pEntity->GetLight()->SetLightRaysIllumDecay(m_ui.pSpinBoxIntensity->value());

        if (m_ui.pSpinBoxFalloff->hasFocus())
            m_pEntity->GetLight()->SetLightRaysDecay(m_ui.pSpinBoxFalloff->value());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotFlickerSettingsChanged()
    {
        if (!m_pEntity)
            return;

        m_pEntity->SetFlicker(m_ui.pGroupBoxFlicker->isChecked());

        if (m_ui.pSpinBoxFlickerTime->hasFocus())
            m_pEntity->SetFlickerTime(m_ui.pSpinBoxFlickerTime->value());

        if (m_ui.pSpinBoxFlickerPauseTime->hasFocus())
            m_pEntity->SetFlickerPauseTime(m_ui.pSpinBoxFlickerPauseTime->value());

        if (m_ui.pSpinBoxFlickerIntensity->hasFocus())
            m_pEntity->SetFlickerAmount(m_ui.pSpinBoxFlickerIntensity->value());

        if (m_ui.pSpinBoxFlickerRandomness->hasFocus())
            m_pEntity->SetFlickerRandomness(m_ui.pSpinBoxFlickerRandomness->value());
    }


    //-----------------------------------------------------------------------------------
    virtual void SaveToTemplate()
    {
        QString strFilename = QFileDialog::getSaveFileName(m_ui.qEditorPanelBox, "Save Light Template", "../Resources/Lights", "Light Templates (*.light)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        m_pEntity->SaveTemplate(strFilename.toStdString(), "../Lights/" + fi.fileName().toStdString());

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void LoadFromTemplate()
    {
        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Save Light Template", "../Resources/Lights", "Light Templates (*.light)");

        if (strFilename.isEmpty())
            return;

        QFileInfo fi(strFilename);

        m_pEntity->LoadTemplate("../Lights/" + fi.fileName().toStdString());

        UpdateUI();
    }

private:

    env::CLightSpriteEntity* m_pEntity;

};