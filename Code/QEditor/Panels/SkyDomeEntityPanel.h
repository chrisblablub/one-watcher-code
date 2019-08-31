/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#pragma once

#include <QEditor/UIGen/SkyDomeEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/SkyDomeEntity.h>
#define UNICODE 

class CSkyDomeEntityPanel : public CPanel<Ui::SkyDomeEntityPanel>
{
    Q_OBJECT

public:
    CSkyDomeEntityPanel(QWidget* pForm)
        : CPanel<Ui::SkyDomeEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pBtnChooseMesh, SIGNAL(pressed()), this, SLOT(slotChooseMesh()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CSkyDomeEntity*>(pEntity);

        m_ui.pBtnChooseMesh->setText(m_pEntity->GetMeshInstanceContainer().GetMeshInstance(0)->GetAttachedMeshContainerName().c_str());
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotChooseMesh()
    {
        if (!m_pEntity)
            return;

        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Load Texture", "../Resources/Textures/Skies", "Textures (*.*)");

        QFileInfo fi(strFilename);

        m_pEntity->SetSkyTextureName((fi.fileName()).toStdString());
    }

private:

    env::CSkyDomeEntity* m_pEntity;

};