/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/ParticleSystemEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/ParticleSystemEntity.h>
#include <Engine/Entity/ParticleEmitterEntity.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/ParticleSystemMgr.h>
#define UNICODE 


class CParticleSystemEntityPanel : public CPanel<Ui::ParticleSystemEntityPanel>
{
    Q_OBJECT

public:
    CParticleSystemEntityPanel(QWidget* pForm)
        : CPanel<Ui::ParticleSystemEntityPanel>(pForm),
        m_pEntity(0)
    {
        connect(m_ui.pBtnSaveTemplate, SIGNAL(pressed()), this, SLOT(slotSaveTemplate()));
        connect(m_ui.pBtnLoadTemplate, SIGNAL(pressed()), this, SLOT(slotLoadTemplate()));
        connect(m_ui.pBtnTexture, SIGNAL(pressed()), this, SLOT(slotChooseTexture()));
        connect(m_ui.pBtnAttachEmitter, SIGNAL(pressed()), this, SLOT(slotAttachEmitter()));
        connect(m_ui.pBtnDetachEmitter, SIGNAL(pressed()), this, SLOT(slotDetachEmitter()));
        connect(m_ui.pBtnPickEmitter, SIGNAL(pressed()), this, SLOT(slotPickEmitter()));
        connect(m_ui.pRadioBtnRenderPassColor, SIGNAL(clicked(bool)), this, SLOT(slotRenderPassClicked()));
        connect(m_ui.pRadioBtnRenderPassDistort, SIGNAL(clicked(bool)), this, SLOT(slotRenderPassClicked()));
        connect(m_ui.pBtnAlignmentTop, SIGNAL(toggled(bool)), this, SLOT(slotBtnAlignmnetTopClicked()));
        connect(m_ui.pBtnAlignmentBottom, SIGNAL(toggled(bool)), this, SLOT(slotBtnAlignmnetBottomClicked()));
        connect(m_ui.pBtnAlignmentCenter, SIGNAL(toggled(bool)), this, SLOT(slotBtnAlignmnetCenterClicked()));
        connect(m_ui.pBtnAlignmentLeft, SIGNAL(toggled(bool)), this, SLOT(slotBtnAlignmnetLeftClicked()));
        connect(m_ui.pBtnAlignmentRight, SIGNAL(toggled(bool)), this, SLOT(slotBtnAlignmnetRightClicked()));
        connect(m_ui.pCheckBoxLoadFromTemplate, SIGNAL(clicked(bool)), this, SLOT(slotCheckBoxSettingsChanged()));
    }


    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CParticleSystemEntity*>(pEntity);

        
        UpdateEmitterRadioButtons();
        UpdateMaterialName();
        UpdateTemplateComboBox();
        UpdateEmittersComboBoxes();

        m_ui.pComboBoxTemplate->setCurrentText(m_pEntity->GetParticleSystemTemplate().c_str());
        m_ui.pLabelTemplateLoaded->setText(m_pEntity->GetParticleSystemTemplate().c_str());

        env::PARTICLESYSTEM_ALIGNMENT alignment = m_pEntity->GetAlignment();

        m_ui.pBtnAlignmentTop->setChecked(false);
        m_ui.pBtnAlignmentLeft->setChecked(false);
        m_ui.pBtnAlignmentRight->setChecked(false);
        m_ui.pBtnAlignmentCenter->setChecked(false);
        m_ui.pBtnAlignmentBottom->setChecked(false);

        m_ui.pCheckBoxLoadFromTemplate->setChecked(m_pEntity->GetLoadFromTemplate());

        switch (alignment)
        {
        case env::PARTICLESYSTEM_ALIGNMENT_BOTTOM:
            m_ui.pBtnAlignmentBottom->setChecked(true);
            break;

        case env::PARTICLESYSTEM_ALIGNMENT_LEFT:
            m_ui.pBtnAlignmentLeft->setChecked(true);
            break;

        case env::PARTICLESYSTEM_ALIGNMENT_TOP:
            m_ui.pBtnAlignmentTop->setChecked(true);
            break;

        case env::PARTICLESYSTEM_ALIGNMENT_RIGHT:
            m_ui.pBtnAlignmentRight->setChecked(true);
            break;

        case env::PARTICLESYSTEM_ALIGNMENT_CENTER:
            m_ui.pBtnAlignmentCenter->setChecked(true);
            break;
        }
    }

    void UpdateEmitterRadioButtons()
    {
        if (m_pEntity->GetRenderer() == "Distort")
            m_ui.pRadioBtnRenderPassDistort->setChecked(true);
        else
            m_ui.pRadioBtnRenderPassColor->setChecked(true);
    }

    void UpdateMaterialName()
    {
        QString materialName = m_pEntity->GetParticleMaterialFilename().c_str();
        
        m_ui.pBtnTexture->setText(materialName.right(30));
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

 public slots:

     //-----------------------------------------------------------------------------------
     virtual void slotCheckBoxSettingsChanged()
     {
         if (!m_pEntity)
             return;

         m_pEntity->SetLoadFromTemplate(m_ui.pCheckBoxLoadFromTemplate->isChecked());
     }

     virtual void slotBtnAlignmnetTopClicked()
     {
         bool checked = m_ui.pBtnAlignmentTop->isChecked();

         m_ui.pBtnAlignmentLeft->setChecked(false);
         m_ui.pBtnAlignmentRight->setChecked(false);
         m_ui.pBtnAlignmentCenter->setChecked(false);
         m_ui.pBtnAlignmentBottom->setChecked(false);

         if (checked)
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_TOP);
         else
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_NONE);

         m_ui.pBtnAlignmentTop->setChecked(checked);
     }

     virtual void slotBtnAlignmnetBottomClicked()
     {
         bool checked = m_ui.pBtnAlignmentBottom->isChecked();

         m_ui.pBtnAlignmentLeft->setChecked(false);
         m_ui.pBtnAlignmentRight->setChecked(false);
         m_ui.pBtnAlignmentCenter->setChecked(false);
         m_ui.pBtnAlignmentTop->setChecked(false);

         if (checked)
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_BOTTOM);
         else
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_NONE);

         m_ui.pBtnAlignmentBottom->setChecked(checked);
     }

     virtual void slotBtnAlignmnetLeftClicked()
     {
         bool checked = m_ui.pBtnAlignmentLeft->isChecked();

         m_ui.pBtnAlignmentBottom->setChecked(false);
         m_ui.pBtnAlignmentRight->setChecked(false);
         m_ui.pBtnAlignmentCenter->setChecked(false);
         m_ui.pBtnAlignmentTop->setChecked(false);

         if (checked)
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_LEFT);
         else
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_NONE);
         
         m_ui.pBtnAlignmentLeft->setChecked(checked);
     }

     virtual void slotBtnAlignmnetRightClicked()
     {
         bool checked = m_ui.pBtnAlignmentRight->isChecked();

         m_ui.pBtnAlignmentLeft->setChecked(false);
         m_ui.pBtnAlignmentBottom->setChecked(false);
         m_ui.pBtnAlignmentCenter->setChecked(false);
         m_ui.pBtnAlignmentTop->setChecked(false);

         if (checked)
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_RIGHT);
         else
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_NONE);

         m_ui.pBtnAlignmentRight->setChecked(checked);
     }

     virtual void slotBtnAlignmnetCenterClicked()
     {
         bool checked = m_ui.pBtnAlignmentCenter->isChecked();

         m_ui.pBtnAlignmentLeft->setChecked(false);
         m_ui.pBtnAlignmentRight->setChecked(false);
         m_ui.pBtnAlignmentBottom->setChecked(false);
         m_ui.pBtnAlignmentTop->setChecked(false);

         if (checked)
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_CENTER);
         else
             m_pEntity->SetAlignment(env::PARTICLESYSTEM_ALIGNMENT_NONE);

         m_ui.pBtnAlignmentCenter->setChecked(checked);
     }

    virtual void slotRenderPassClicked()
    {
        if (m_ui.pRadioBtnRenderPassDistort->isChecked())
        {
            m_pEntity->SetRenderer("Distort");
        }
        else
        {
            m_pEntity->SetRenderer("Default");
        }
    }

    virtual void slotChooseTexture()
    {
        if (!m_pEntity)
            return;

        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Load Texture", "../Resources/Textures/Sprites", "Textures (*.*)");

        QFileInfo fi(strFilename);

        m_pEntity->SetParticleMaterialFilename("Sprites/" + fi.fileName().toStdString());

        UpdateMaterialName();
    }

    virtual void slotSaveTemplate()
    {
        const QString& strTemplate = m_ui.pComboBoxTemplate->currentText();

        if (strTemplate == "<No Template>" || strTemplate.isEmpty())
            return;

        m_pEntity->SaveAsTemplate(strTemplate.toStdString());

        m_ui.pLabelTemplateLoaded->setText(m_pEntity->GetParticleSystemTemplate().c_str());
    }

    virtual void slotLoadTemplate()
    {
        const QString& strTemplate = m_ui.pComboBoxTemplate->currentText();

        if (strTemplate == "<No Template>")
            return;

        m_pEntity->LoadFromTemplate(strTemplate.toStdString());

        m_ui.pLabelTemplateLoaded->setText(strTemplate);

        UpdateEmitterRadioButtons();
        UpdateMaterialName();
    }

    virtual void slotAttachEmitter()
    {
        if (!m_pEntity)
            return;

        m_pEntity->AddEmitter(m_ui.pComboBoxEmittersAvailable->currentText().toStdString());

        UpdateEmittersComboBoxes();
    }

    virtual void slotDetachEmitter()
    {
        if (!m_pEntity)
            return;

        m_pEntity->RemoveEmitter(m_ui.pComboBoxEmittersAttached->currentText().toStdString());

        UpdateEmittersComboBoxes();
    }

    virtual void slotPickEmitter()
    {

    }

private:

    void UpdateTemplateComboBox()
    {
        m_ui.pComboBoxTemplate->clear();

        std::set< std::string > templates;
        env::gEngine->GetParticleSystemMgr()->GetParticleSystemTemplates(templates);


        m_ui.pComboBoxTemplate->addItem("<No Template>");

        for (std::set< std::string >::iterator it = templates.begin(); it != templates.end(); ++it)
        {
            std::string strTemplateName = *it;

            m_ui.pComboBoxTemplate->addItem(strTemplateName.c_str());
        }
    }

    void UpdateEmittersComboBoxes()
    {
        m_ui.pComboBoxEmittersAvailable->clear();
        m_ui.pComboBoxEmittersAttached->clear();

        std::set< std::string > emittersAttached;
        for (env::CParticleSystemEntity::mapEmitters::const_iterator it = m_pEntity->GetEmitters().begin(); it != m_pEntity->GetEmitters().end(); ++it)
        {
            emittersAttached.insert(it->first);
        }

        std::list< std::string > emittersAvailable;
        const env::CEntityMgr::mapEntities& entities = env::gEngine->GetEntityMgr()->GetEntities();
        for (env::CEntityMgr::mapEntities::const_iterator it = entities.begin(); it != entities.end(); ++it)
        {
            env::CParticleEmitterEntity* p = dynamic_cast<env::CParticleEmitterEntity*>(it->second);
            if (p != nullptr &&
                p->GetParticleSystem() == nullptr)
            {
                emittersAvailable.push_back(it->first);
            }

//             std::set< std::string >::const_iterator jt = emittersAttached.find(it->first);
//             if (jt != emittersAttached.end())
//                 continue;
        }


        for (std::list< std::string >::iterator it = emittersAvailable.begin(); it != emittersAvailable.end(); ++it)
        {
            std::string strEntityName = *it;

            m_ui.pComboBoxEmittersAvailable->addItem(strEntityName.c_str());
        }

        m_ui.pComboBoxEmittersAttached->addItem("<No Emitters>");

        for (std::set< std::string >::iterator it = emittersAttached.begin(); it != emittersAttached.end(); ++it)
        {
            std::string strEntityName = *it;

            m_ui.pComboBoxEmittersAttached->addItem(strEntityName.c_str());
        }
    }

private:

    env::CParticleSystemEntity* m_pEntity;

};