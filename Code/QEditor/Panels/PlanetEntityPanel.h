/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/PlanetEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Entity/PlanetEntity.h>
#define UNICODE 




class CPlanetEntityPanel : public CPanel<Ui::PlanetEntityPanel>
{
    Q_OBJECT

public:
    CPlanetEntityPanel(QWidget* pForm)
        : CPanel<Ui::PlanetEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pBtnColor, SIGNAL(pressed()), this, SLOT(slotChooseSpriteColor()));
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CPlanetEntity*>(pEntity);

        env::Vec3 color = env::Vec3(m_pEntity->GetEnergyColor());

        env::DoGamma(color);

        SetBtnColor(m_ui.pBtnColor, color);
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    public slots:

    virtual void slotChooseColor()
    {
        if (!m_pEntity)
            return;

        env::Vec3 spriteColor = m_pEntity->GetEnergyColor();

        QColor color = QColorDialog::getColor(QColor(spriteColor.x * 255.0f, spriteColor.y * 255.0f, spriteColor.z * 255.0f), GetWidget(), "Choose Color");

        spriteColor = env::Vec3(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f);

        m_pEntity->SetEnergyColor(spriteColor);

        QString strBackgroundColor = QString::number(spriteColor.x * 255) + ", " +
            QString::number(spriteColor.y * 255) + ", " +
            QString::number(spriteColor.z * 255) + ", 1.0";

        m_ui.pBtnColor->setStyleSheet("background-color:rgba(" + strBackgroundColor + ")");

    }


    //-----------------------------------------------------------------------------------
    virtual void slotChooseSpriteColor()
    {
        if (!m_pEntity)
            return;

        env::Vec3 diffuseColor = env::Vec3(m_pEntity->GetEnergyColor());

        env::DoGamma(diffuseColor);

        gEditor->AcquireColorDialog(this, SLOT(slotSpriteColorChanged(const QColor&)), diffuseColor, "Choose Diffuse Color");
    }

    //-----------------------------------------------------------------------------------
    virtual void slotSpriteColorChanged(const QColor& color)
    {
        env::Vec3 diffuseColor = FromQColor(color);

        SetBtnColor(m_ui.pBtnColor, diffuseColor);

        m_pEntity->SetEnergyColor(env::UndoGamma(diffuseColor));
    }


private:

    env::CPlanetEntity* m_pEntity;

};