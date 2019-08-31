/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/SpriteEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>


#undef UNICODE
#include <Engine/Entity/SpriteEntity.h>
#define UNICODE 


class CSpriteEntityPanel : public CPanel<Ui::SpriteEntityPanel>
{
    Q_OBJECT

public:
    CSpriteEntityPanel(QWidget* pForm)
        : CPanel<Ui::SpriteEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pCheckBoxBillboard, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxAlphaBlending, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxScreenSpace, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxRenderInGame, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pCheckBoxRenderInEditor, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
        connect(m_ui.pSpinBoxPositionX, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxPositionY, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxAlpha, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pBtnColor, SIGNAL(pressed()), this, SLOT(slotChooseSpriteColor()));
        connect(m_ui.pBtnChooseTexture, SIGNAL(pressed()), this, SLOT(slotChooseTexture()));
        connect(m_ui.pRadioBtnBeforePostProcessing, SIGNAL(toggled(bool)), this, SLOT(slotPipelineClicked()));
        connect(m_ui.pRadioBtnAfterPostProcessing, SIGNAL(toggled(bool)), this, SLOT(slotPipelineClicked()));
        connect(m_ui.pRadioBtnAfterFadeScreen, SIGNAL(toggled(bool)), this, SLOT(slotPipelineClicked()));
        connect(m_ui.pRadioBtnAfterFade, SIGNAL(toggled(bool)), this, SLOT(slotPipelineClicked()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CSpriteEntity*>(pEntity);

        m_ui.pSpinBoxPositionX->setValue((double)m_pEntity->GetScreenPosition().x);
        m_ui.pSpinBoxPositionY->setValue((double)m_pEntity->GetScreenPosition().y);
        m_ui.pSpinBoxAlpha->setValue((double)m_pEntity->GetSprite().GetColor().w);

        m_ui.pCheckBoxBillboard->setChecked(m_pEntity->GetSprite().GetBillboard());
        m_ui.pCheckBoxAlphaBlending->setChecked(m_pEntity->GetSprite().GetAlphaBlending());
        m_ui.pCheckBoxScreenSpace->setChecked(m_pEntity->GetScreenSpace());
        m_ui.pCheckBoxRenderInGame->setChecked(m_pEntity->GetRenderGame());
        m_ui.pCheckBoxRenderInEditor->setChecked(m_pEntity->GetRenderEditor());

        if (m_pEntity->GetSprite().GetSpritePipeline() == env::SPRITE_PIPELINE_BEFORE_POSTPROCESSING)
            m_ui.pRadioBtnBeforePostProcessing->setChecked(true);
        else if (m_pEntity->GetSprite().GetSpritePipeline() == env::SPRITE_PIPELINE_AFTER_POSTPROCESSING)
            m_ui.pRadioBtnAfterPostProcessing->setChecked(true);
        else if (m_pEntity->GetSprite().GetSpritePipeline() == env::SPRITE_PIPELINE_AFTER_FADE_SCREEN)
            m_ui.pRadioBtnAfterFadeScreen->setChecked(true);
        else if (m_pEntity->GetSprite().GetSpritePipeline() == env::SPRITE_PIPELINE_AFTER_FADE)
            m_ui.pRadioBtnAfterFade->setChecked(true);

        env::Vec3 color = env::Vec3(m_pEntity->GetSprite().GetColor());

        env::DoGamma(color);

        SetBtnColor(m_ui.pBtnColor, color);
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        env::Vec2 screenPos = m_pEntity->GetScreenPosition();

        if (m_ui.pSpinBoxPositionX->hasFocus())
            m_pEntity->SetScreenPosition(env::Vec2((float)m_ui.pSpinBoxPositionX->value(), screenPos.y));

        if (m_ui.pSpinBoxPositionY->hasFocus())
            m_pEntity->SetScreenPosition(env::Vec2(screenPos.x, (float)m_ui.pSpinBoxPositionY->value()));

        if (m_ui.pSpinBoxAlpha->hasFocus())
            m_pEntity->GetSprite().SetAlpha((float)m_ui.pSpinBoxAlpha->value());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotCheckBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        m_pEntity->SetRenderGame(m_ui.pCheckBoxRenderInGame->isChecked());
        m_pEntity->SetRenderEditor(m_ui.pCheckBoxRenderInEditor->isChecked());
        m_pEntity->GetSprite().SetBillboard(m_ui.pCheckBoxBillboard->isChecked());
        m_pEntity->GetSprite().SetAlphaBlending(m_ui.pCheckBoxAlphaBlending->isChecked());
        m_pEntity->SetScreenSpace(m_ui.pCheckBoxScreenSpace->isChecked());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotPipelineClicked()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pRadioBtnBeforePostProcessing->isChecked())
            m_pEntity->GetSprite().SetSpritePipeline(env::SPRITE_PIPELINE_BEFORE_POSTPROCESSING);
        else if (m_ui.pRadioBtnAfterPostProcessing->isChecked())
            m_pEntity->GetSprite().SetSpritePipeline(env::SPRITE_PIPELINE_AFTER_POSTPROCESSING);
        else if (m_ui.pRadioBtnAfterFadeScreen->isChecked())
            m_pEntity->GetSprite().SetSpritePipeline(env::SPRITE_PIPELINE_AFTER_FADE_SCREEN);
        else if (m_ui.pRadioBtnAfterFade->isChecked())
            m_pEntity->GetSprite().SetSpritePipeline(env::SPRITE_PIPELINE_AFTER_FADE);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseTexture()
    {
        if (!m_pEntity)
            return;

        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Load Texture", "../Resources/Textures/Sprites", "Textures (*.*)");

        QFileInfo fi(strFilename);

        m_pEntity->SetMaterialFilename("Sprites/" + fi.fileName().toStdString());

    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseSpriteColor()
    {
        if (!m_pEntity)
            return;

        env::Vec3 diffuseColor = env::Vec3(m_pEntity->GetSprite().GetColor());

        env::DoGamma(diffuseColor);

        gEditor->AcquireColorDialog(this, SLOT(slotSpriteColorChanged(const QColor&)), diffuseColor, "Choose Diffuse Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotSpriteColorChanged(const QColor& color)
    {
        env::Vec3 diffuseColor = FromQColor(color);

        SetBtnColor(m_ui.pBtnColor, diffuseColor);

        m_pEntity->GetSprite().SetColor(env::UndoGamma(diffuseColor));
    }

private:

    env::CSpriteEntity* m_pEntity;
};