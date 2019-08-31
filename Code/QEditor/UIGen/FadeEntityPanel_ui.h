/********************************************************************************
** Form generated from reading UI file 'FadeEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef FADEENTITYPANEL_UI_H
#define FADEENTITYPANEL_UI_H

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
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_FadeEntityPanel
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
    QDoubleSpinBox *pSpinBoxTopBegin;
    QLabel *label_5;
    QLabel *label_10;
    QLabel *pLabelName;
    QLabel *label_11;
    QDoubleSpinBox *pSpinBoxTopEnd;
    QDoubleSpinBox *pSpinBoxLeftBegin;
    QDoubleSpinBox *pSpinBoxLeftEnd;
    QDoubleSpinBox *pSpinBoxBottomEnd;
    QDoubleSpinBox *pSpinBoxBottomBegin;
    QDoubleSpinBox *pSpinBoxRightBegin;
    QDoubleSpinBox *pSpinBoxRightEnd;
    QDoubleSpinBox *pSpinBoxDuration;
    QLabel *label_6;
    QLabel *label_7;
    QDoubleSpinBox *pSpinBoxDelay;
    QGroupBox *pGroupBoxColor_7;
    QCheckBox *pCheckBoxTemplate;
    QPushButton *pBtnLoadTemplate;
    QPushButton *pBtnSaveTemplate;

    void setupUi(QWidget *FadeEntityPanel)
    {
        if (FadeEntityPanel->objectName().isEmpty())
            FadeEntityPanel->setObjectName(QStringLiteral("FadeEntityPanel"));
        FadeEntityPanel->resize(340, 741);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FadeEntityPanel->sizePolicy().hasHeightForWidth());
        FadeEntityPanel->setSizePolicy(sizePolicy);
        FadeEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(FadeEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(FadeEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 420));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pGroupBoxColor = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor->setObjectName(QStringLiteral("pGroupBoxColor"));
        pGroupBoxColor->setGeometry(QRect(10, 200, 321, 91));
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
        pGroupBoxColor_3->setGeometry(QRect(10, 40, 321, 151));
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
        label_4->setGeometry(QRect(10, 60, 81, 16));
        pSpinBoxTopBegin = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxTopBegin->setObjectName(QStringLiteral("pSpinBoxTopBegin"));
        pSpinBoxTopBegin->setGeometry(QRect(160, 60, 71, 18));
        pSpinBoxTopBegin->setDecimals(4);
        pSpinBoxTopBegin->setMinimum(0);
        pSpinBoxTopBegin->setMaximum(1);
        pSpinBoxTopBegin->setSingleStep(0.05);
        label_5 = new QLabel(pGroupBoxColor_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 80, 81, 16));
        label_10 = new QLabel(pGroupBoxColor_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 100, 81, 16));
        pLabelName = new QLabel(pGroupBoxColor_3);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(90, 0, 151, 20));
        label_11 = new QLabel(pGroupBoxColor_3);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 120, 81, 16));
        pSpinBoxTopEnd = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxTopEnd->setObjectName(QStringLiteral("pSpinBoxTopEnd"));
        pSpinBoxTopEnd->setGeometry(QRect(240, 60, 71, 18));
        pSpinBoxTopEnd->setDecimals(4);
        pSpinBoxTopEnd->setMinimum(0);
        pSpinBoxTopEnd->setMaximum(1);
        pSpinBoxTopEnd->setSingleStep(0.05);
        pSpinBoxLeftBegin = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxLeftBegin->setObjectName(QStringLiteral("pSpinBoxLeftBegin"));
        pSpinBoxLeftBegin->setGeometry(QRect(160, 80, 71, 18));
        pSpinBoxLeftBegin->setDecimals(4);
        pSpinBoxLeftBegin->setMinimum(0);
        pSpinBoxLeftBegin->setMaximum(1);
        pSpinBoxLeftBegin->setSingleStep(0.05);
        pSpinBoxLeftEnd = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxLeftEnd->setObjectName(QStringLiteral("pSpinBoxLeftEnd"));
        pSpinBoxLeftEnd->setGeometry(QRect(240, 80, 71, 18));
        pSpinBoxLeftEnd->setDecimals(4);
        pSpinBoxLeftEnd->setMinimum(0);
        pSpinBoxLeftEnd->setMaximum(1);
        pSpinBoxLeftEnd->setSingleStep(0.05);
        pSpinBoxBottomEnd = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxBottomEnd->setObjectName(QStringLiteral("pSpinBoxBottomEnd"));
        pSpinBoxBottomEnd->setGeometry(QRect(240, 100, 71, 18));
        pSpinBoxBottomEnd->setDecimals(4);
        pSpinBoxBottomEnd->setMinimum(0);
        pSpinBoxBottomEnd->setMaximum(1);
        pSpinBoxBottomEnd->setSingleStep(0.05);
        pSpinBoxBottomBegin = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxBottomBegin->setObjectName(QStringLiteral("pSpinBoxBottomBegin"));
        pSpinBoxBottomBegin->setGeometry(QRect(160, 100, 71, 18));
        pSpinBoxBottomBegin->setDecimals(4);
        pSpinBoxBottomBegin->setMinimum(0);
        pSpinBoxBottomBegin->setMaximum(1);
        pSpinBoxBottomBegin->setSingleStep(0.05);
        pSpinBoxRightBegin = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxRightBegin->setObjectName(QStringLiteral("pSpinBoxRightBegin"));
        pSpinBoxRightBegin->setGeometry(QRect(160, 120, 71, 18));
        pSpinBoxRightBegin->setDecimals(4);
        pSpinBoxRightBegin->setMinimum(0);
        pSpinBoxRightBegin->setMaximum(1);
        pSpinBoxRightBegin->setSingleStep(0.05);
        pSpinBoxRightEnd = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxRightEnd->setObjectName(QStringLiteral("pSpinBoxRightEnd"));
        pSpinBoxRightEnd->setGeometry(QRect(240, 120, 71, 18));
        pSpinBoxRightEnd->setDecimals(4);
        pSpinBoxRightEnd->setMinimum(0);
        pSpinBoxRightEnd->setMaximum(1);
        pSpinBoxRightEnd->setSingleStep(0.05);
        pSpinBoxDuration = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxDuration->setObjectName(QStringLiteral("pSpinBoxDuration"));
        pSpinBoxDuration->setGeometry(QRect(240, 20, 71, 18));
        pSpinBoxDuration->setDecimals(4);
        pSpinBoxDuration->setMinimum(0);
        pSpinBoxDuration->setMaximum(1);
        pSpinBoxDuration->setSingleStep(0.05);
        label_6 = new QLabel(pGroupBoxColor_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 20, 81, 16));
        label_7 = new QLabel(pGroupBoxColor_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 40, 81, 16));
        pSpinBoxDelay = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxDelay->setObjectName(QStringLiteral("pSpinBoxDelay"));
        pSpinBoxDelay->setGeometry(QRect(240, 40, 71, 18));
        pSpinBoxDelay->setDecimals(4);
        pSpinBoxDelay->setMinimum(0);
        pSpinBoxDelay->setMaximum(1);
        pSpinBoxDelay->setSingleStep(0.05);
        pGroupBoxColor_7 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_7->setObjectName(QStringLiteral("pGroupBoxColor_7"));
        pGroupBoxColor_7->setGeometry(QRect(10, 300, 321, 111));
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


        retranslateUi(FadeEntityPanel);

        QMetaObject::connectSlotsByName(FadeEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *FadeEntityPanel)
    {
        FadeEntityPanel->setWindowTitle(QApplication::translate("FadeEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("FadeEntityPanel", "Stripes", 0)));
        pGroupBoxColor->setTitle(QApplication::translate("FadeEntityPanel", "Colors", 0));
        label_8->setText(QApplication::translate("FadeEntityPanel", "Top Color", 0));
        pBtnTopStartColor->setText(QString());
        pBtnTopEndColor->setText(QString());
        label_9->setText(QApplication::translate("FadeEntityPanel", "Bottom Color", 0));
        pBtnBotStartColor->setText(QString());
        pBtnBotEndColor->setText(QString());
        pGroupBoxColor_3->setTitle(QApplication::translate("FadeEntityPanel", "Time", 0));
#ifndef QT_NO_TOOLTIP
        label_4->setToolTip(QApplication::translate("FadeEntityPanel", "Time it takes to fully fade in a single stripe", 0));
#endif // QT_NO_TOOLTIP
        label_4->setText(QApplication::translate("FadeEntityPanel", "Top", 0));
#ifndef QT_NO_TOOLTIP
        label_5->setToolTip(QApplication::translate("FadeEntityPanel", "Offset between generation of each stripe", 0));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("FadeEntityPanel", "Left", 0));
#ifndef QT_NO_TOOLTIP
        label_10->setToolTip(QApplication::translate("FadeEntityPanel", "Offset between generation of each stripe", 0));
#endif // QT_NO_TOOLTIP
        label_10->setText(QApplication::translate("FadeEntityPanel", "Bottom", 0));
        pLabelName->setText(QString());
#ifndef QT_NO_TOOLTIP
        label_11->setToolTip(QApplication::translate("FadeEntityPanel", "Offset between generation of each stripe", 0));
#endif // QT_NO_TOOLTIP
        label_11->setText(QApplication::translate("FadeEntityPanel", "Right", 0));
#ifndef QT_NO_TOOLTIP
        label_6->setToolTip(QApplication::translate("FadeEntityPanel", "Time it takes to fully fade in a single stripe", 0));
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("FadeEntityPanel", "Duration", 0));
#ifndef QT_NO_TOOLTIP
        label_7->setToolTip(QApplication::translate("FadeEntityPanel", "Time it takes to fully fade in a single stripe", 0));
#endif // QT_NO_TOOLTIP
        label_7->setText(QApplication::translate("FadeEntityPanel", "Delay", 0));
        pGroupBoxColor_7->setTitle(QApplication::translate("FadeEntityPanel", "Template", 0));
        pCheckBoxTemplate->setText(QApplication::translate("FadeEntityPanel", "Init from template after loading", 0));
        pBtnLoadTemplate->setText(QApplication::translate("FadeEntityPanel", "Load template", 0));
        pBtnSaveTemplate->setText(QApplication::translate("FadeEntityPanel", "Save template", 0));
    } // retranslateUi

};

namespace Ui {
    class FadeEntityPanel: public Ui_FadeEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // FADEENTITYPANEL_UI_H
