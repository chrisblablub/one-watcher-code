/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/EntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#undef UNICODE
#include <Engine/Entity/Entity.h>
#define UNICODE 


class CEntityPanel : public CPanel<Ui::Form>
{
    Q_OBJECT

public:

    //-----------------------------------------------------------------------------------
    CEntityPanel(QWidget* pForm)
        : CPanel<Ui::Form>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pSpinBoxPositionX, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxPositionY, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxPositionZ, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxYaw, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxPitch, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxRoll, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxScalingX, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxScalingY, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pSpinBoxScalingZ, SIGNAL(valueChanged(double)), this, SLOT(slotSpinBoxSettingsChanged()));
        connect(m_ui.pBtnResetOrientation, SIGNAL(pressed()), this, SLOT(slotResetOrientation()));
        connect(m_ui.pBtnResetScaling, SIGNAL(pressed()), this, SLOT(slotResetScaling()));
        connect(m_ui.pBtnSetName, SIGNAL(pressed()), this, SLOT(slotSetName()));
        connect(m_ui.pLineEditName, SIGNAL(textEdited(const QString&)), this, SLOT(slotNameChanged()));
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = pEntity;
        
        UpdateUI();

        UpdateTitle();
    }

    //-----------------------------------------------------------------------------------
    void UpdateUI()
    {
        m_ui.pBtnSetName->setEnabled(true);
        m_ui.pLineEditName->setText(m_pEntity->GetName().c_str());

        env::CSceneNode* pNode = m_pEntity->GetNode();

        m_ui.pSpinBoxPositionX->setValue((double)pNode->GetPosition().x);
        m_ui.pSpinBoxPositionY->setValue((double)pNode->GetPosition().y);
        m_ui.pSpinBoxPositionZ->setValue((double)pNode->GetPosition().z);

        m_ui.pSpinBoxYaw->setValue((double)pNode->GetOrientation().GetYaw() / PI * 180.0);
        m_ui.pSpinBoxPitch->setValue((double)pNode->GetOrientation().GetPitch() / PI * 180.0);
        m_ui.pSpinBoxRoll->setValue((double)pNode->GetOrientation().GetRoll() / PI * 180.0);

        m_ui.pSpinBoxScalingX->setValue((double)pNode->GetScale().x);
        m_ui.pSpinBoxScalingY->setValue((double)pNode->GetScale().y);
        m_ui.pSpinBoxScalingZ->setValue((double)pNode->GetScale().z);
    }

    //-----------------------------------------------------------------------------------
    void UpdateTitle()
    {
        m_ui.qEditorPanelBox->setTitle(QString("Entity: ") + m_pEntity->GetName().c_str());
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotSpinBoxSettingsChanged()
    {
        if (!m_pEntity)
            return;

        env::CSceneNode* pNode = m_pEntity->GetNode();

        if (m_ui.pSpinBoxPositionX->hasFocus() ||
            m_ui.pSpinBoxPositionY->hasFocus() ||
            m_ui.pSpinBoxPositionZ->hasFocus())
        {
            pNode->SetPosition(env::Vec3((float)m_ui.pSpinBoxPositionX->value(),
                (float)m_ui.pSpinBoxPositionY->value(),
                (float)m_ui.pSpinBoxPositionZ->value()));
        }

        if (m_ui.pSpinBoxYaw->hasFocus() ||
            m_ui.pSpinBoxPitch->hasFocus() ||
            m_ui.pSpinBoxRoll->hasFocus())
        {
            pNode->SetOrientation(env::CQuaternion((float)m_ui.pSpinBoxYaw->value() / 180.0f * PI,
                (float)m_ui.pSpinBoxPitch->value() / 180.0f * PI,
                (float)m_ui.pSpinBoxRoll->value() / 180.0f * PI));
        }

        if (m_ui.pSpinBoxScalingX->hasFocus() ||
            m_ui.pSpinBoxScalingY->hasFocus() ||
            m_ui.pSpinBoxScalingZ->hasFocus())
        {
            if (m_ui.pBtnLockAxis->isChecked())
            {
                env::Vec3 scale = pNode->GetScale();

                float scaling = 0.0f;

                if (m_ui.pSpinBoxScalingX->hasFocus())
                    scaling = (float)m_ui.pSpinBoxScalingX->value() - scale.x;
                else if (m_ui.pSpinBoxScalingY->hasFocus())
                    scaling = (float)m_ui.pSpinBoxScalingY->value() - scale.y;
                else if (m_ui.pSpinBoxScalingZ->hasFocus())
                    scaling = (float)m_ui.pSpinBoxScalingZ->value() - scale.z;

                scale += scaling;

                pNode->SetScale(scale);

                UpdateUI();
            }
            else
            {
                pNode->SetScale(env::Vec3((float)m_ui.pSpinBoxScalingX->value(),
                                          (float)m_ui.pSpinBoxScalingY->value(),
                                          (float)m_ui.pSpinBoxScalingZ->value()));
            }
            
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetOrientation()
    {
        if (!m_pEntity)
            return;

        m_pEntity->GetNode()->SetOrientation(env::CQuaternion());

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotResetScaling()
    {
        if (!m_pEntity)
            return;

        m_pEntity->GetNode()->SetScale(env::Vec3(1.0f));

        UpdateUI();
    }


    //-----------------------------------------------------------------------------------
    virtual void slotNameChanged()
    {
        std::string strName = m_ui.pLineEditName->text().toStdString();

        m_ui.pBtnSetName->setEnabled(env::gEngine->GetEntityMgr()->GetEntity(strName) == nullptr);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotSetName()
    {
        if (!m_pEntity)
            return;

        env::gEngine->GetEntityMgr()->SetEntityName(m_pEntity->GetName(), m_ui.pLineEditName->text().toStdString());

        UpdateTitle();
    }

private:

    env::CEntity* m_pEntity;

};