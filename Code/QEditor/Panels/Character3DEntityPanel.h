/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/Character3DEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>


#undef UNICODE
#include <Engine/Entity/Character3DEntity.h>
#define UNICODE 


class CCharacter3DEntityPanel : public CPanel<Ui::Character3DEntityPanel>
{
    Q_OBJECT

public:
    CCharacter3DEntityPanel(QWidget* pForm)
        : CPanel<Ui::Character3DEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pBtnDiffMtrl, SIGNAL(pressed()), this, SLOT(slotChooseDiffuseMtrl()));
        connect(m_ui.pBtnCharMtrl, SIGNAL(pressed()), this, SLOT(slotChooseCharMtrl()));
        connect(m_ui.pSpinBoxDiffuseAlpha, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CCharacter3DEntity*>(pEntity);

        env::Vec3 diffuseMaterial = env::Vec3(m_pEntity->GetDiffuseMtrl());
        env::Vec3 charNaterial = env::Vec3(m_pEntity->GetCharMtrl());

        env::DoGamma(diffuseMaterial);
        env::DoGamma(charNaterial);

        SetBtnColor(m_ui.pBtnDiffMtrl, diffuseMaterial);
        SetBtnColor(m_ui.pBtnCharMtrl, charNaterial);

        m_ui.pSpinBoxDiffuseAlpha->setValue(m_pEntity->GetDiffuseAlpha());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

 public slots:

     virtual void slotSettingsChanged()
     {
         if (!m_pEntity)
             return;

         if (m_ui.pSpinBoxDiffuseAlpha->hasFocus())
             m_pEntity->SetDiffuseAlpha(m_ui.pSpinBoxDiffuseAlpha->value());
     }


    //-----------------------------------------------------------------------------------
    virtual void slotChooseDiffuseMtrl()
    {
        if (!m_pEntity)
            return;

        env::Vec3 diffuseColor = env::Vec3(m_pEntity->GetDiffuseMtrl());

        env::DoGamma(diffuseColor);

        gEditor->AcquireColorDialog(this, SLOT(slotDiffuseMtrlChanged(const QColor&)), diffuseColor, "Choose Diffuse Material");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotChooseCharMtrl()
    {
        if (!m_pEntity)
            return;

        env::Vec3 charMaterial = env::Vec3(m_pEntity->GetCharMtrl());

        env::DoGamma(charMaterial);

        gEditor->AcquireColorDialog(this, SLOT(slotCharMtrlChanged(const QColor&)), charMaterial, "Choose Character Material");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotDiffuseMtrlChanged(const QColor& color)
    {
        env::Vec3 diffuseColor = FromQColor(color);

        SetBtnColor(m_ui.pBtnDiffMtrl, diffuseColor);

        m_pEntity->SetDiffuseMtrl(env::UndoGamma(diffuseColor));
    }

    //-----------------------------------------------------------------------------------
    virtual void slotCharMtrlChanged(const QColor& color)
    {
        env::Vec3 diffuseColor = FromQColor(color);

        SetBtnColor(m_ui.pBtnCharMtrl, diffuseColor);

        m_pEntity->SetCharMtrl(env::UndoGamma(diffuseColor));
    }

private:

    env::CCharacter3DEntity* m_pEntity;
};