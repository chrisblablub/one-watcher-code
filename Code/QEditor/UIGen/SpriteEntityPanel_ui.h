/********************************************************************************
** Form generated from reading UI file 'SpriteEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SPRITEENTITYPANEL_UI_H
#define SPRITEENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_SpriteEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QGroupBox *pCheckBoxScreenSpace;
    QLabel *label_8;
    QDoubleSpinBox *pSpinBoxPositionY;
    QLabel *label_9;
    QLabel *label_10;
    QDoubleSpinBox *pSpinBoxPositionX;
    QGroupBox *pGroupBoxColor;
    QCheckBox *pCheckBoxRenderInEditor;
    QCheckBox *pCheckBoxRenderInGame;
    QCheckBox *pCheckBoxBillboard;
    QCheckBox *pCheckBoxAlphaBlending;
    QLabel *label_4;
    QLabel *label;
    QRadioButton *pRadioBtnBeforePostProcessing;
    QRadioButton *pRadioBtnAfterPostProcessing;
    QRadioButton *pRadioBtnAfterFadeScreen;
    QRadioButton *pRadioBtnAfterFade;
    QGroupBox *pGroupBoxColor_2;
    QLabel *label_6;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxAlpha;
    QPushButton *pBtnColor;
    QGroupBox *pGroupBoxColor_3;
    QPushButton *pBtnChooseTexture;

    void setupUi(QWidget *SpriteEntityPanel)
    {
        if (SpriteEntityPanel->objectName().isEmpty())
            SpriteEntityPanel->setObjectName(QStringLiteral("SpriteEntityPanel"));
        SpriteEntityPanel->resize(340, 503);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SpriteEntityPanel->sizePolicy().hasHeightForWidth());
        SpriteEntityPanel->setSizePolicy(sizePolicy);
        SpriteEntityPanel->setMinimumSize(QSize(0, 220));
        gridLayout = new QGridLayout(SpriteEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(SpriteEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 420));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pCheckBoxScreenSpace = new QGroupBox(qEditorPanelBox);
        pCheckBoxScreenSpace->setObjectName(QStringLiteral("pCheckBoxScreenSpace"));
        pCheckBoxScreenSpace->setGeometry(QRect(10, 260, 321, 71));
        pCheckBoxScreenSpace->setStyleSheet(QLatin1String("QGroupBox\n"
"{\n"
"	border: 1px solid #6e788a;\n"
"	border-radius:0px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}"));
        pCheckBoxScreenSpace->setCheckable(true);
        label_8 = new QLabel(pCheckBoxScreenSpace);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 20, 81, 16));
        pSpinBoxPositionY = new QDoubleSpinBox(pCheckBoxScreenSpace);
        pSpinBoxPositionY->setObjectName(QStringLiteral("pSpinBoxPositionY"));
        pSpinBoxPositionY->setGeometry(QRect(210, 40, 101, 18));
        pSpinBoxPositionY->setMinimum(-1);
        pSpinBoxPositionY->setMaximum(1);
        pSpinBoxPositionY->setSingleStep(0.01);
        label_9 = new QLabel(pCheckBoxScreenSpace);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(190, 40, 16, 16));
        label_10 = new QLabel(pCheckBoxScreenSpace);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(190, 20, 16, 16));
        pSpinBoxPositionX = new QDoubleSpinBox(pCheckBoxScreenSpace);
        pSpinBoxPositionX->setObjectName(QStringLiteral("pSpinBoxPositionX"));
        pSpinBoxPositionX->setGeometry(QRect(210, 20, 101, 18));
        pSpinBoxPositionX->setMinimum(-1);
        pSpinBoxPositionX->setMaximum(1);
        pSpinBoxPositionX->setSingleStep(0.01);
        pSpinBoxPositionX->setValue(0);
        pGroupBoxColor = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor->setObjectName(QStringLiteral("pGroupBoxColor"));
        pGroupBoxColor->setGeometry(QRect(10, 100, 321, 151));
        pGroupBoxColor->setStyleSheet(QLatin1String("QGroupBox\n"
"{\n"
"	border: 1px solid #6e788a;\n"
"	border-radius:0px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}"));
        pCheckBoxRenderInEditor = new QCheckBox(pGroupBoxColor);
        pCheckBoxRenderInEditor->setObjectName(QStringLiteral("pCheckBoxRenderInEditor"));
        pCheckBoxRenderInEditor->setGeometry(QRect(210, 120, 101, 17));
        pCheckBoxRenderInGame = new QCheckBox(pGroupBoxColor);
        pCheckBoxRenderInGame->setObjectName(QStringLiteral("pCheckBoxRenderInGame"));
        pCheckBoxRenderInGame->setGeometry(QRect(210, 100, 101, 17));
        pCheckBoxBillboard = new QCheckBox(pGroupBoxColor);
        pCheckBoxBillboard->setObjectName(QStringLiteral("pCheckBoxBillboard"));
        pCheckBoxBillboard->setGeometry(QRect(210, 20, 91, 17));
        pCheckBoxAlphaBlending = new QCheckBox(pGroupBoxColor);
        pCheckBoxAlphaBlending->setObjectName(QStringLiteral("pCheckBoxAlphaBlending"));
        pCheckBoxAlphaBlending->setGeometry(QRect(210, 40, 91, 17));
        label_4 = new QLabel(pGroupBoxColor);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 100, 64, 16));
        label = new QLabel(pGroupBoxColor);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 91, 16));
        pRadioBtnBeforePostProcessing = new QRadioButton(pGroupBoxColor);
        pRadioBtnBeforePostProcessing->setObjectName(QStringLiteral("pRadioBtnBeforePostProcessing"));
        pRadioBtnBeforePostProcessing->setGeometry(QRect(80, 20, 131, 21));
        pRadioBtnAfterPostProcessing = new QRadioButton(pGroupBoxColor);
        pRadioBtnAfterPostProcessing->setObjectName(QStringLiteral("pRadioBtnAfterPostProcessing"));
        pRadioBtnAfterPostProcessing->setGeometry(QRect(80, 40, 131, 21));
        pRadioBtnAfterFadeScreen = new QRadioButton(pGroupBoxColor);
        pRadioBtnAfterFadeScreen->setObjectName(QStringLiteral("pRadioBtnAfterFadeScreen"));
        pRadioBtnAfterFadeScreen->setGeometry(QRect(80, 60, 131, 21));
        pRadioBtnAfterFade = new QRadioButton(pGroupBoxColor);
        pRadioBtnAfterFade->setObjectName(QStringLiteral("pRadioBtnAfterFade"));
        pRadioBtnAfterFade->setGeometry(QRect(80, 80, 131, 21));
        pGroupBoxColor_2 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_2->setObjectName(QStringLiteral("pGroupBoxColor_2"));
        pGroupBoxColor_2->setGeometry(QRect(10, 340, 321, 71));
        pGroupBoxColor_2->setStyleSheet(QLatin1String("QGroupBox\n"
"{\n"
"	border: 1px solid #6e788a;\n"
"	border-radius:0px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}"));
        label_6 = new QLabel(pGroupBoxColor_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 20, 47, 13));
        label_5 = new QLabel(pGroupBoxColor_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 40, 47, 21));
        pSpinBoxAlpha = new QDoubleSpinBox(pGroupBoxColor_2);
        pSpinBoxAlpha->setObjectName(QStringLiteral("pSpinBoxAlpha"));
        pSpinBoxAlpha->setGeometry(QRect(210, 20, 101, 18));
        pSpinBoxAlpha->setMinimum(0);
        pSpinBoxAlpha->setMaximum(5);
        pSpinBoxAlpha->setSingleStep(0.1);
        pBtnColor = new QPushButton(pGroupBoxColor_2);
        pBtnColor->setObjectName(QStringLiteral("pBtnColor"));
        pBtnColor->setGeometry(QRect(210, 40, 101, 23));
        pBtnColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pGroupBoxColor_3 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_3->setObjectName(QStringLiteral("pGroupBoxColor_3"));
        pGroupBoxColor_3->setGeometry(QRect(10, 40, 321, 51));
        pGroupBoxColor_3->setStyleSheet(QLatin1String("QGroupBox\n"
"{\n"
"	border: 1px solid #6e788a;\n"
"	border-radius:0px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}"));
        pBtnChooseTexture = new QPushButton(pGroupBoxColor_3);
        pBtnChooseTexture->setObjectName(QStringLiteral("pBtnChooseTexture"));
        pBtnChooseTexture->setGeometry(QRect(10, 20, 301, 23));
        pBtnChooseTexture->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(SpriteEntityPanel);

        QMetaObject::connectSlotsByName(SpriteEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *SpriteEntityPanel)
    {
        SpriteEntityPanel->setWindowTitle(QApplication::translate("SpriteEntityPanel", "SpriteEntityPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("SpriteEntityPanel", "Sprite", 0)));
        pCheckBoxScreenSpace->setTitle(QApplication::translate("SpriteEntityPanel", "Screen Space", 0));
        label_8->setText(QApplication::translate("SpriteEntityPanel", "Logical Position", 0));
        label_9->setText(QApplication::translate("SpriteEntityPanel", "Y", 0));
        label_10->setText(QApplication::translate("SpriteEntityPanel", "X", 0));
        pGroupBoxColor->setTitle(QApplication::translate("SpriteEntityPanel", "Flags", 0));
        pCheckBoxRenderInEditor->setText(QApplication::translate("SpriteEntityPanel", "Render in Editor", 0));
        pCheckBoxRenderInGame->setText(QApplication::translate("SpriteEntityPanel", "Render in Game", 0));
        pCheckBoxBillboard->setText(QApplication::translate("SpriteEntityPanel", "Billboard", 0));
        pCheckBoxAlphaBlending->setText(QApplication::translate("SpriteEntityPanel", "Alpha Blending", 0));
        label_4->setText(QApplication::translate("SpriteEntityPanel", "Render Mode", 0));
        label->setText(QApplication::translate("SpriteEntityPanel", "Sprite Mode", 0));
        pRadioBtnBeforePostProcessing->setText(QApplication::translate("SpriteEntityPanel", "Before Postprocessing", 0));
        pRadioBtnAfterPostProcessing->setText(QApplication::translate("SpriteEntityPanel", "After Postprocessing", 0));
        pRadioBtnAfterFadeScreen->setText(QApplication::translate("SpriteEntityPanel", "After Fade Screen", 0));
        pRadioBtnAfterFade->setText(QApplication::translate("SpriteEntityPanel", "After Fade", 0));
        pGroupBoxColor_2->setTitle(QApplication::translate("SpriteEntityPanel", "Properties", 0));
        label_6->setText(QApplication::translate("SpriteEntityPanel", "Alpha", 0));
        label_5->setText(QApplication::translate("SpriteEntityPanel", "Color", 0));
        pBtnColor->setText(QString());
        pGroupBoxColor_3->setTitle(QApplication::translate("SpriteEntityPanel", "Texture", 0));
        pBtnChooseTexture->setText(QApplication::translate("SpriteEntityPanel", "Choose Texture", 0));
    } // retranslateUi

};

namespace Ui {
    class SpriteEntityPanel: public Ui_SpriteEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SPRITEENTITYPANEL_UI_H
