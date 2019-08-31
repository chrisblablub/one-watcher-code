/********************************************************************************
** Form generated from reading UI file 'SoundSpriteEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SoundSpriteEntityPANEL_UI_H
#define SoundSpriteEntityPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_SoundSpriteEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pVolumeLabel;
    QDoubleSpinBox *pSpinBoxVolume;
    QPushButton *pBtnLoadSound;
    QLabel *pPeakVolumeDistanceLabel;
    QLabel *pAudibleDistanceLabel;
    QDoubleSpinBox *pSpinBoxPeakVolumeDistance;
    QDoubleSpinBox *pSpinBoxAudibleDistance;
    QCheckBox *pCheckBoxIsMuted;
    QCheckBox *pCheckBoxIsAutoplay;

    void setupUi(QWidget *SoundSpriteEntityPanel)
    {
        if (SoundSpriteEntityPanel->objectName().isEmpty())
            SoundSpriteEntityPanel->setObjectName(QStringLiteral("SoundSpriteEntityPanel"));
        SoundSpriteEntityPanel->resize(289, 362);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SoundSpriteEntityPanel->sizePolicy().hasHeightForWidth());
        SoundSpriteEntityPanel->setSizePolicy(sizePolicy);
        SoundSpriteEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(SoundSpriteEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(SoundSpriteEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 180));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pVolumeLabel = new QLabel(qEditorPanelBox);
        pVolumeLabel->setObjectName(QStringLiteral("pVolumeLabel"));
        pVolumeLabel->setGeometry(QRect(20, 90, 61, 16));
        pSpinBoxVolume = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxVolume->setObjectName(QStringLiteral("pSpinBoxVolume"));
        pSpinBoxVolume->setGeometry(QRect(150, 90, 101, 18));
        pSpinBoxVolume->setMinimum(0);
        pSpinBoxVolume->setMaximum(100000);
        pSpinBoxVolume->setSingleStep(0.05);
        pBtnLoadSound = new QPushButton(qEditorPanelBox);
        pBtnLoadSound->setObjectName(QStringLiteral("pBtnLoadSound"));
        pBtnLoadSound->setEnabled(true);
        pBtnLoadSound->setGeometry(QRect(20, 60, 231, 26));
        pBtnLoadSound->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pPeakVolumeDistanceLabel = new QLabel(qEditorPanelBox);
        pPeakVolumeDistanceLabel->setObjectName(QStringLiteral("pPeakVolumeDistanceLabel"));
        pPeakVolumeDistanceLabel->setGeometry(QRect(20, 110, 111, 16));
        pAudibleDistanceLabel = new QLabel(qEditorPanelBox);
        pAudibleDistanceLabel->setObjectName(QStringLiteral("pAudibleDistanceLabel"));
        pAudibleDistanceLabel->setGeometry(QRect(20, 130, 111, 16));
        pSpinBoxPeakVolumeDistance = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxPeakVolumeDistance->setObjectName(QStringLiteral("pSpinBoxPeakVolumeDistance"));
        pSpinBoxPeakVolumeDistance->setGeometry(QRect(150, 110, 101, 18));
        pSpinBoxPeakVolumeDistance->setMinimum(0);
        pSpinBoxPeakVolumeDistance->setMaximum(100000);
        pSpinBoxPeakVolumeDistance->setSingleStep(0.05);
        pSpinBoxAudibleDistance = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxAudibleDistance->setObjectName(QStringLiteral("pSpinBoxAudibleDistance"));
        pSpinBoxAudibleDistance->setGeometry(QRect(150, 130, 101, 18));
        pSpinBoxAudibleDistance->setMinimum(0);
        pSpinBoxAudibleDistance->setMaximum(100000);
        pSpinBoxAudibleDistance->setSingleStep(0.05);
        pCheckBoxIsMuted = new QCheckBox(qEditorPanelBox);
        pCheckBoxIsMuted->setObjectName(QStringLiteral("pCheckBoxIsMuted"));
        pCheckBoxIsMuted->setGeometry(QRect(20, 150, 91, 17));
        pCheckBoxIsAutoplay = new QCheckBox(qEditorPanelBox);
        pCheckBoxIsAutoplay->setObjectName(QStringLiteral("pCheckBoxIsAutoplay"));
        pCheckBoxIsAutoplay->setGeometry(QRect(150, 150, 91, 17));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(SoundSpriteEntityPanel);

        QMetaObject::connectSlotsByName(SoundSpriteEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *SoundSpriteEntityPanel)
    {
        SoundSpriteEntityPanel->setWindowTitle(QApplication::translate("SoundSpriteEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("SoundSpriteEntityPanel", "Sound", 0)));
        pVolumeLabel->setText(QApplication::translate("SoundSpriteEntityPanel", "Volume", 0));
        pBtnLoadSound->setText(QApplication::translate("SoundSpriteEntityPanel", "Load Sound...", 0));
        pPeakVolumeDistanceLabel->setText(QApplication::translate("SoundSpriteEntityPanel", "Peak Volume Distance", 0));
        pAudibleDistanceLabel->setText(QApplication::translate("SoundSpriteEntityPanel", "Audible Distance", 0));
        pCheckBoxIsMuted->setText(QApplication::translate("SoundSpriteEntityPanel", "Is Muted", 0));
        pCheckBoxIsAutoplay->setText(QApplication::translate("SoundSpriteEntityPanel", "Is Autoplay", 0));
    } // retranslateUi

};

namespace Ui {
    class SoundSpriteEntityPanel: public Ui_SoundSpriteEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SoundSpriteEntityPANEL_UI_H
