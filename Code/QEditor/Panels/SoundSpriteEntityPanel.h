/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/SoundSpriteEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>

#undef UNICODE
#include <Engine/Entity/SoundSpriteEntity.h>
#define UNICODE 




class CSoundSpriteEntityPanel : public CPanel<Ui::SoundSpriteEntityPanel>
{
    Q_OBJECT

public:
    CSoundSpriteEntityPanel(QWidget* pForm)
        : CPanel<Ui::SoundSpriteEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pBtnLoadSound, SIGNAL(pressed()), this, SLOT(LoadSound()));
        connect(m_ui.pSpinBoxVolume, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxPeakVolumeDistance, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pSpinBoxAudibleDistance, SIGNAL(valueChanged(double)), this, SLOT(slotSettingsChanged()));

        connect(m_ui.pCheckBoxIsMuted, SIGNAL(clicked(bool)), this, SLOT(slotSettingsChanged()));
        connect(m_ui.pCheckBoxIsAutoplay, SIGNAL(clicked(bool)), this, SLOT(slotSettingsChanged()));
    }

public slots:
    virtual void slotSettingsChanged()
    {
        if (m_pEntity == nullptr)
        {
            return;
        }

        if (m_ui.pSpinBoxVolume->hasFocus())
        {
            m_pEntity->SetVolume((float) m_ui.pSpinBoxVolume->value());
        }

        if (m_ui.pSpinBoxPeakVolumeDistance->hasFocus())
        {
            m_pEntity->SetPeakVolumeDistance((float)m_ui.pSpinBoxPeakVolumeDistance->value());
        }

        if (m_ui.pSpinBoxAudibleDistance->hasFocus())
        {
            m_pEntity->SetAudibleDistance((float)m_ui.pSpinBoxAudibleDistance->value());
        }

        if (m_ui.pCheckBoxIsMuted->hasFocus())
        {
            m_pEntity->SetIsMuted((bool)m_ui.pCheckBoxIsMuted->isChecked());
        }

        if (m_ui.pCheckBoxIsAutoplay->hasFocus())
        {
            m_pEntity->SetIsAutoplay((bool)m_ui.pCheckBoxIsAutoplay->isChecked());
        }
    }

    virtual void LoadSound()
    {
        QString strFilename = QFileDialog::getOpenFileName(m_ui.qEditorPanelBox, "Load Sound", "../Resources/Audio", "Sounds (*.Wav)");

        if (strFilename.isEmpty())
        {
            return;
        }
        QFileInfo fi(strFilename);
        std::string filePath = fi.absoluteFilePath().toStdString();
        std::size_t audioIndex = filePath.find("Audio");

        if (audioIndex == std::string::npos)
        {
            return;
        }

        filePath = filePath.substr(audioIndex + 6);

        m_pEntity->LoadSound(filePath);
        UpdateUI();
    }

    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (pEntity == nullptr)
        {
            return;
        }

        m_pEntity = static_cast<env::CSoundSpriteEntity*>(pEntity);
        UpdateUI();
    }

    void UpdateUI()
    {
        std::string loadSoundLabel = m_pEntity->GetFileName().empty() ? "Load Sound..." : m_pEntity->GetFileName();
        m_ui.pBtnLoadSound->setText(loadSoundLabel.c_str());

        m_ui.pSpinBoxVolume->setValue((float)m_pEntity->GetVolume());
        m_ui.pSpinBoxPeakVolumeDistance->setValue((float)m_pEntity->GetPeakVolumeDistance());
        m_ui.pSpinBoxAudibleDistance->setValue((float)m_pEntity->GetAudibleDistance());

        m_ui.pCheckBoxIsMuted->setChecked(m_pEntity->GetIsMuted());
        m_ui.pCheckBoxIsAutoplay->setChecked(m_pEntity->GetIsAutoplay());
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

private:

    env::CSoundSpriteEntity* m_pEntity;

};