/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/TextEntityPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtCore/QProcess>
#include <QtGui/QFontDatabase>

#undef UNICODE
#include <Engine/Entity/TextEntity.h>
#define UNICODE 

class CTextEntityPanel : public CPanel<Ui::TextEntityPanel>
{
    Q_OBJECT

public:
    CTextEntityPanel(QWidget* pForm)
        : CPanel<Ui::TextEntityPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pRadioBtnAlignLeft, SIGNAL(toggled(bool)), this, SLOT(slotAlignmentChanged()));
        connect(m_ui.pRadioBtnAlignCenter, SIGNAL(toggled(bool)), this, SLOT(slotAlignmentChanged()));
        connect(m_ui.pRadioBtnAlignRight, SIGNAL(toggled(bool)), this, SLOT(slotAlignmentChanged()));
        connect(m_ui.pCheckBoxBold, SIGNAL(stateChanged(int)), this, SLOT(slotFormattingChanged()));
        connect(m_ui.pCheckBoxItalic, SIGNAL(stateChanged(int)), this, SLOT(slotFormattingChanged()));
        connect(m_ui.pSpinBoxSize, SIGNAL(valueChanged(int)), this, SLOT(slotFormattingChanged()));
        connect(m_ui.pComboFontFamilies, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(slotFontFamilyChanged(const QString&)));
        connect(m_ui.pBtnUpdate, SIGNAL(clicked()), this, SLOT(slotCreateFontBitmap()));
        connect(m_ui.pEditText, SIGNAL(textChanged(const QString&)), this, SLOT(slotFormattingChanged()));

    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = static_cast<env::CTextEntity*>(pEntity);

        if (m_pEntity->GetAlignment() == env::TEXT_ALIGNMENT_LEFT)
            m_ui.pRadioBtnAlignLeft->setChecked(true);
        else if (m_pEntity->GetAlignment() == env::TEXT_ALIGNMENT_CENTER)
            m_ui.pRadioBtnAlignCenter->setChecked(true);
        else if (m_pEntity->GetAlignment() == env::TEXT_ALIGNMENT_RIGHT)
            m_ui.pRadioBtnAlignRight->setChecked(true);

        m_ui.pCheckBoxItalic->setChecked(m_pEntity->GetItalic());
        m_ui.pCheckBoxBold->setChecked(m_pEntity->GetBold());

        QFontDatabase db;
        QStringList systemFontFamilies = db.families();

        m_ui.pComboFontFamilies->clear();
        m_ui.pComboFontFamilies->addItem(QString());

        for (int i = 0; i < systemFontFamilies.size(); ++i)
        {
            m_ui.pComboFontFamilies->addItem(systemFontFamilies.at(i));
        }

        QString strCurrentFont = m_pEntity->GetFont().c_str();

        m_ui.pComboFontFamilies->setCurrentText(strCurrentFont);
        m_ui.pEditText->setText(m_pEntity->GetText().c_str());
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotAlignmentChanged()
    {
        if (m_ui.pRadioBtnAlignLeft->isChecked())
            m_pEntity->SetAlignment(env::TEXT_ALIGNMENT_LEFT);
        else if (m_ui.pRadioBtnAlignCenter->isChecked())
            m_pEntity->SetAlignment(env::TEXT_ALIGNMENT_CENTER);
        else if (m_ui.pRadioBtnAlignRight->isChecked())
            m_pEntity->SetAlignment(env::TEXT_ALIGNMENT_RIGHT);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotFormattingChanged()
    {
        if (m_ui.pCheckBoxItalic->hasFocus())
            m_pEntity->SetItalic(m_ui.pCheckBoxItalic->isChecked());

        if (m_ui.pCheckBoxBold->hasFocus())
            m_pEntity->SetBold(m_ui.pCheckBoxBold->isChecked());

        if (m_ui.pSpinBoxSize->hasFocus())
            m_pEntity->SetFontSize(m_ui.pSpinBoxSize->value());

        if (m_ui.pEditText->hasFocus())
            m_pEntity->SetText(m_ui.pEditText->text().toStdString());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotCreateFontBitmap()
    {
        QString workingDir = QDir::currentPath();
        QString fontFamily = m_ui.pComboFontFamilies->currentText();
        QString fontSize("/FontSize:");
        QString fontStyleItalic("/FontStyle:Italic");
        QString fontStyleBold("/FontStyle:Bold");

        if (fontFamily.isEmpty())
            return;

        env::FontInfo fontInfo(m_ui.pComboFontFamilies->currentText().toStdString(), 
                               m_ui.pSpinBoxSize->value(), 
                               m_ui.pCheckBoxItalic->isChecked(), 
                               m_ui.pCheckBoxBold->isChecked());
        
        
        fontSize += QString::number(fontInfo.GetFontSize());

        QString commandLine = " \"" + fontFamily + "\" ../Resources/Fonts/" + fontInfo.GetKey().c_str() + " " + fontSize;

        if (fontInfo.GetItalic())
            commandLine += " " + fontStyleItalic;

        if (fontInfo.GetBold())
            commandLine += " " + fontStyleBold;

        QProcess* pProcess = new QProcess();
        pProcess->setWorkingDirectory("");
        pProcess->start(workingDir + "/MakeSpriteFont.exe" + commandLine);
        while (!pProcess->waitForFinished());
        ENV_DELETE(pProcess);
    }

    //-----------------------------------------------------------------------------------
    virtual void slotFontFamilyChanged(const QString & text)
    {
        if (m_ui.pComboFontFamilies->hasFocus())
            m_pEntity->SetFont(m_ui.pComboFontFamilies->currentText().toStdString());
    }

private:

    env::CTextEntity* m_pEntity;

};