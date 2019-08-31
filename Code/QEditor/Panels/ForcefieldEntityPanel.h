/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/ForcefieldEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>


#undef UNICODE
#include <Engine/Entity/ForceFieldEntity.h>
#define UNICODE 




class CForcefieldEntityPanel : public CPanel<Ui::ForcefieldEntityPanel>
{
    Q_OBJECT

public:
    CForcefieldEntityPanel(QWidget* pForm)
        : CPanel<Ui::ForcefieldEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pSpinBoxAcceleration, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxTargetVelocity, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pBtnColor, SIGNAL(pressed()), this, SLOT(slotChooseColor()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CForceFieldEntity*>(pEntity);

        m_ui.pSpinBoxAcceleration->setValue((double)m_pEntity->GetAcceleration());
        m_ui.pSpinBoxTargetVelocity->setValue((double)m_pEntity->GetTargetVelocity());

        SetBtnColor(m_ui.pBtnColor, m_pEntity->GetColor());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        if (m_ui.pSpinBoxAcceleration->hasFocus())
            m_pEntity->SetAcceleration((float)m_ui.pSpinBoxAcceleration->value());

        if (m_ui.pSpinBoxTargetVelocity->hasFocus())
            m_pEntity->SetTargetVelocity((float)m_ui.pSpinBoxTargetVelocity->value());
    }

    virtual void slotColorChanged(const QColor& color)
    {
        env::Vec3 spriteColor = FromQColor(color);

        env::UndoGamma(spriteColor);

        m_pEntity->SetColor(spriteColor);

        SetBtnColor(m_ui.pBtnColor, spriteColor);
    }

    virtual void slotChooseColor()
    {
        if (!m_pEntity)
            return;

        env::Vec3 diffuseColor = m_pEntity->GetColor();

        env::DoGamma(diffuseColor);

        gEditor->AcquireColorDialog(this, SLOT(slotColorChanged(const QColor&)), diffuseColor, "Choose Color");
    }

private:
    env::CForceFieldEntity* m_pEntity;

};