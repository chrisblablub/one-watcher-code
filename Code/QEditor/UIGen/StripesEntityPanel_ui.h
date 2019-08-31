/********************************************************************************
** Form generated from reading UI file 'StripesEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef STRIPESENTITYPANEL_UI_H
#define STRIPESENTITYPANEL_UI_H

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
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_StripesEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QGroupBox *pGroupBoxColor;
    QLabel *label_8;
    QPushButton *pBtnTopStartColor;
    QPushButton *pBtnTopEndColor;
    QLabel *label_9;
    QPushButton *pBtnBotStartColor;
    QPushButton *pBtnBotEndColor;
    QGroupBox *pGroupBoxColor_3;
    QLabel *label_4;
    QDoubleSpinBox *pSpinBoxTimeDuration;
    QDoubleSpinBox *pSpinBoxTimeOffset;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxFadeDuration;
    QLabel *label_10;
    QLabel *pLabelName;
    QGroupBox *pGroupBoxColor_4;
    QLabel *label_6;
    QDoubleSpinBox *pSpinBoxStartWidth;
    QDoubleSpinBox *pSpinBoxEndWidth;
    QCheckBox *pCheckBoxGrowVertically;
    QCheckBox *pCheckBoxGrowAlternatingly;
    QLabel *label_7;
    QSpinBox *pSpinBoxSlices;
    QGroupBox *pGroupBoxColor_2;
    QRadioButton *pRadioBtnFadeOut;
    QRadioButton *pRadioBtnFadeIn;
    QGroupBox *pGroupBoxColor_7;
    QCheckBox *pCheckBoxTemplate;
    QPushButton *pBtnLoadTemplate;
    QPushButton *pBtnSaveTemplate;

    void setupUi(QWidget *StripesEntityPanel)
    {
        if (StripesEntityPanel->objectName().isEmpty())
            StripesEntityPanel->setObjectName(QStringLiteral("StripesEntityPanel"));
        StripesEntityPanel->resize(340, 741);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(StripesEntityPanel->sizePolicy().hasHeightForWidth());
        StripesEntityPanel->setSizePolicy(sizePolicy);
        StripesEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(StripesEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(StripesEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 550));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pGroupBoxColor = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor->setObjectName(QStringLiteral("pGroupBoxColor"));
        pGroupBoxColor->setGeometry(QRect(10, 330, 321, 91));
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
        label_8 = new QLabel(pGroupBoxColor);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 20, 71, 20));
        pBtnTopStartColor = new QPushButton(pGroupBoxColor);
        pBtnTopStartColor->setObjectName(QStringLiteral("pBtnTopStartColor"));
        pBtnTopStartColor->setGeometry(QRect(160, 20, 71, 23));
        pBtnTopStartColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pBtnTopEndColor = new QPushButton(pGroupBoxColor);
        pBtnTopEndColor->setObjectName(QStringLiteral("pBtnTopEndColor"));
        pBtnTopEndColor->setGeometry(QRect(240, 20, 71, 23));
        pBtnTopEndColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_9 = new QLabel(pGroupBoxColor);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 50, 71, 20));
        pBtnBotStartColor = new QPushButton(pGroupBoxColor);
        pBtnBotStartColor->setObjectName(QStringLiteral("pBtnBotStartColor"));
        pBtnBotStartColor->setGeometry(QRect(160, 50, 71, 23));
        pBtnBotStartColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pBtnBotEndColor = new QPushButton(pGroupBoxColor);
        pBtnBotEndColor->setObjectName(QStringLiteral("pBtnBotEndColor"));
        pBtnBotEndColor->setGeometry(QRect(240, 50, 71, 23));
        pBtnBotEndColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
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
        pGroupBoxColor_3->setGeometry(QRect(10, 100, 321, 91));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pGroupBoxColor_3->sizePolicy().hasHeightForWidth());
        pGroupBoxColor_3->setSizePolicy(sizePolicy2);
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
        label_4 = new QLabel(pGroupBoxColor_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 81, 16));
        pSpinBoxTimeDuration = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxTimeDuration->setObjectName(QStringLiteral("pSpinBoxTimeDuration"));
        pSpinBoxTimeDuration->setGeometry(QRect(240, 20, 71, 18));
        pSpinBoxTimeDuration->setDecimals(4);
        pSpinBoxTimeDuration->setMinimum(0);
        pSpinBoxTimeDuration->setMaximum(100000);
        pSpinBoxTimeDuration->setSingleStep(0.05);
        pSpinBoxTimeOffset = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxTimeOffset->setObjectName(QStringLiteral("pSpinBoxTimeOffset"));
        pSpinBoxTimeOffset->setGeometry(QRect(240, 40, 71, 18));
        pSpinBoxTimeOffset->setDecimals(4);
        pSpinBoxTimeOffset->setMinimum(0);
        pSpinBoxTimeOffset->setMaximum(100000);
        pSpinBoxTimeOffset->setSingleStep(0.05);
        label_5 = new QLabel(pGroupBoxColor_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 40, 81, 16));
        pSpinBoxFadeDuration = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxFadeDuration->setObjectName(QStringLiteral("pSpinBoxFadeDuration"));
        pSpinBoxFadeDuration->setGeometry(QRect(240, 60, 71, 18));
        pSpinBoxFadeDuration->setDecimals(4);
        pSpinBoxFadeDuration->setMinimum(0);
        pSpinBoxFadeDuration->setMaximum(100000);
        pSpinBoxFadeDuration->setSingleStep(0.05);
        label_10 = new QLabel(pGroupBoxColor_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 60, 81, 16));
        pLabelName = new QLabel(pGroupBoxColor_3);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(90, 0, 151, 20));
        pGroupBoxColor_4 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_4->setObjectName(QStringLiteral("pGroupBoxColor_4"));
        pGroupBoxColor_4->setGeometry(QRect(10, 200, 321, 121));
        sizePolicy2.setHeightForWidth(pGroupBoxColor_4->sizePolicy().hasHeightForWidth());
        pGroupBoxColor_4->setSizePolicy(sizePolicy2);
        pGroupBoxColor_4->setStyleSheet(QLatin1String("QGroupBox\n"
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
        label_6 = new QLabel(pGroupBoxColor_4);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 50, 81, 16));
        pSpinBoxStartWidth = new QDoubleSpinBox(pGroupBoxColor_4);
        pSpinBoxStartWidth->setObjectName(QStringLiteral("pSpinBoxStartWidth"));
        pSpinBoxStartWidth->setGeometry(QRect(160, 50, 71, 18));
        pSpinBoxStartWidth->setDecimals(4);
        pSpinBoxStartWidth->setMinimum(0);
        pSpinBoxStartWidth->setMaximum(1);
        pSpinBoxStartWidth->setSingleStep(0.1);
        pSpinBoxEndWidth = new QDoubleSpinBox(pGroupBoxColor_4);
        pSpinBoxEndWidth->setObjectName(QStringLiteral("pSpinBoxEndWidth"));
        pSpinBoxEndWidth->setGeometry(QRect(240, 50, 71, 18));
        pSpinBoxEndWidth->setDecimals(4);
        pSpinBoxEndWidth->setMinimum(0);
        pSpinBoxEndWidth->setMaximum(1);
        pSpinBoxEndWidth->setSingleStep(0.1);
        pCheckBoxGrowVertically = new QCheckBox(pGroupBoxColor_4);
        pCheckBoxGrowVertically->setObjectName(QStringLiteral("pCheckBoxGrowVertically"));
        pCheckBoxGrowVertically->setGeometry(QRect(160, 70, 111, 20));
        pCheckBoxGrowAlternatingly = new QCheckBox(pGroupBoxColor_4);
        pCheckBoxGrowAlternatingly->setObjectName(QStringLiteral("pCheckBoxGrowAlternatingly"));
        pCheckBoxGrowAlternatingly->setGeometry(QRect(160, 90, 121, 17));
        label_7 = new QLabel(pGroupBoxColor_4);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 21, 71, 20));
        pSpinBoxSlices = new QSpinBox(pGroupBoxColor_4);
        pSpinBoxSlices->setObjectName(QStringLiteral("pSpinBoxSlices"));
        pSpinBoxSlices->setGeometry(QRect(240, 22, 71, 20));
        pGroupBoxColor_2 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_2->setObjectName(QStringLiteral("pGroupBoxColor_2"));
        pGroupBoxColor_2->setGeometry(QRect(10, 40, 321, 51));
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
        pRadioBtnFadeOut = new QRadioButton(pGroupBoxColor_2);
        pRadioBtnFadeOut->setObjectName(QStringLiteral("pRadioBtnFadeOut"));
        pRadioBtnFadeOut->setGeometry(QRect(241, 20, 71, 21));
        pRadioBtnFadeIn = new QRadioButton(pGroupBoxColor_2);
        pRadioBtnFadeIn->setObjectName(QStringLiteral("pRadioBtnFadeIn"));
        pRadioBtnFadeIn->setGeometry(QRect(10, 20, 82, 21));
        pGroupBoxColor_7 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_7->setObjectName(QStringLiteral("pGroupBoxColor_7"));
        pGroupBoxColor_7->setGeometry(QRect(10, 430, 321, 111));
        pGroupBoxColor_7->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pGroupBoxColor_7->setCheckable(false);
        pGroupBoxColor_7->setChecked(false);
        pCheckBoxTemplate = new QCheckBox(pGroupBoxColor_7);
        pCheckBoxTemplate->setObjectName(QStringLiteral("pCheckBoxTemplate"));
        pCheckBoxTemplate->setGeometry(QRect(140, 80, 171, 20));
        pBtnLoadTemplate = new QPushButton(pGroupBoxColor_7);
        pBtnLoadTemplate->setObjectName(QStringLiteral("pBtnLoadTemplate"));
        pBtnLoadTemplate->setGeometry(QRect(10, 50, 301, 23));
        pBtnSaveTemplate = new QPushButton(pGroupBoxColor_7);
        pBtnSaveTemplate->setObjectName(QStringLiteral("pBtnSaveTemplate"));
        pBtnSaveTemplate->setGeometry(QRect(10, 20, 301, 23));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(StripesEntityPanel);

        QMetaObject::connectSlotsByName(StripesEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *StripesEntityPanel)
    {
        StripesEntityPanel->setWindowTitle(QApplication::translate("StripesEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("StripesEntityPanel", "Stripes", 0)));
        pGroupBoxColor->setTitle(QApplication::translate("StripesEntityPanel", "Colors", 0));
        label_8->setText(QApplication::translate("StripesEntityPanel", "Top Color", 0));
        pBtnTopStartColor->setText(QString());
        pBtnTopEndColor->setText(QString());
        label_9->setText(QApplication::translate("StripesEntityPanel", "Bottom Color", 0));
        pBtnBotStartColor->setText(QString());
        pBtnBotEndColor->setText(QString());
        pGroupBoxColor_3->setTitle(QApplication::translate("StripesEntityPanel", "Time", 0));
#ifndef QT_NO_TOOLTIP
        label_4->setToolTip(QApplication::translate("StripesEntityPanel", "Time it takes to fully fade in a single stripe", 0));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("StripesEntityPanel", "Spawn Duration", 0));
#ifndef QT_NO_TOOLTIP
        label_5->setToolTip(QApplication::translate("StripesEntityPanel", "Offset between generation of each stripe", 0));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("StripesEntityPanel", "Spawn Offset", 0));
#ifndef QT_NO_TOOLTIP
        label_10->setToolTip(QApplication::translate("StripesEntityPanel", "Offset between generation of each stripe", 0));
#endif // QT_NO_TOOLTIP
        label_10->setText(QApplication::translate("StripesEntityPanel", "Fade Duration", 0));
        pLabelName->setText(QString());
        pGroupBoxColor_4->setTitle(QApplication::translate("StripesEntityPanel", "Shape", 0));
        label_6->setText(QApplication::translate("StripesEntityPanel", "Width", 0));
#ifndef QT_NO_TOOLTIP
        pCheckBoxGrowVertically->setToolTip(QApplication::translate("StripesEntityPanel", "Grow shapes vertically either from top to bottom or vice versa.", 0));
#endif // QT_NO_TOOLTIP
        pCheckBoxGrowVertically->setText(QApplication::translate("StripesEntityPanel", "Grow Vertically", 0));
        pCheckBoxGrowAlternatingly->setText(QApplication::translate("StripesEntityPanel", "Grow Alternatingly", 0));
        label_7->setText(QApplication::translate("StripesEntityPanel", "Slices", 0));
        pGroupBoxColor_2->setTitle(QApplication::translate("StripesEntityPanel", "Mode", 0));
        pRadioBtnFadeOut->setText(QApplication::translate("StripesEntityPanel", "Fade out", 0));
        pRadioBtnFadeIn->setText(QApplication::translate("StripesEntityPanel", "Fade in", 0));
        pGroupBoxColor_7->setTitle(QApplication::translate("StripesEntityPanel", "Template", 0));
        pCheckBoxTemplate->setText(QApplication::translate("StripesEntityPanel", "Init from template after loading", 0));
        pBtnLoadTemplate->setText(QApplication::translate("StripesEntityPanel", "Load template", 0));
        pBtnSaveTemplate->setText(QApplication::translate("StripesEntityPanel", "Save template", 0));
    } // retranslateUi

};

namespace Ui {
    class StripesEntityPanel: public Ui_StripesEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // STRIPESENTITYPANEL_UI_H
