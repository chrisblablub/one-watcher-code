/********************************************************************************
** Form generated from reading UI file 'SkyFogPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SKYFOGPANEL_UI_H
#define SKYFOGPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_SkyFogPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QGroupBox *pGroupBoxColor_2;
    QPushButton *pBtnFogColor;
    QLabel *label_4;
    QLabel *label;
    QPushButton *pBtnSkyColor;
    QGroupBox *pGroupBoxColor_3;
    QDoubleSpinBox *pSpinBoxEnergyfieldIntensity;
    QLabel *label_8;
    QDoubleSpinBox *pSpinBoxCloudIntensity;
    QLabel *label_9;
    QDoubleSpinBox *pSpinBoxFogRange;
    QLabel *label_3;
    QLabel *label_2;
    QDoubleSpinBox *pSpinBoxFogStartDistance;

    void setupUi(QWidget *SkyFogPanel)
    {
        if (SkyFogPanel->objectName().isEmpty())
            SkyFogPanel->setObjectName(QStringLiteral("SkyFogPanel"));
        SkyFogPanel->resize(340, 360);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SkyFogPanel->sizePolicy().hasHeightForWidth());
        SkyFogPanel->setSizePolicy(sizePolicy);
        SkyFogPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(SkyFogPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(SkyFogPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 250));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pGroupBoxColor_2 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_2->setObjectName(QStringLiteral("pGroupBoxColor_2"));
        pGroupBoxColor_2->setGeometry(QRect(10, 40, 321, 81));
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
        pBtnFogColor = new QPushButton(pGroupBoxColor_2);
        pBtnFogColor->setObjectName(QStringLiteral("pBtnFogColor"));
        pBtnFogColor->setGeometry(QRect(210, 50, 101, 23));
        pBtnFogColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_4 = new QLabel(pGroupBoxColor_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 71, 16));
        label = new QLabel(pGroupBoxColor_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 50, 81, 16));
        pBtnSkyColor = new QPushButton(pGroupBoxColor_2);
        pBtnSkyColor->setObjectName(QStringLiteral("pBtnSkyColor"));
        pBtnSkyColor->setGeometry(QRect(210, 20, 101, 23));
        pBtnSkyColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
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
        pGroupBoxColor_3->setGeometry(QRect(10, 130, 321, 111));
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
        pSpinBoxEnergyfieldIntensity = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxEnergyfieldIntensity->setObjectName(QStringLiteral("pSpinBoxEnergyfieldIntensity"));
        pSpinBoxEnergyfieldIntensity->setGeometry(QRect(210, 80, 101, 22));
        pSpinBoxEnergyfieldIntensity->setMaximum(100000);
        pSpinBoxEnergyfieldIntensity->setSingleStep(0.05);
        label_8 = new QLabel(pGroupBoxColor_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 60, 91, 16));
        pSpinBoxCloudIntensity = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxCloudIntensity->setObjectName(QStringLiteral("pSpinBoxCloudIntensity"));
        pSpinBoxCloudIntensity->setGeometry(QRect(210, 60, 101, 18));
        pSpinBoxCloudIntensity->setMinimum(0);
        pSpinBoxCloudIntensity->setMaximum(100000);
        pSpinBoxCloudIntensity->setSingleStep(0.05);
        label_9 = new QLabel(pGroupBoxColor_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 80, 111, 16));
        pSpinBoxFogRange = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxFogRange->setObjectName(QStringLiteral("pSpinBoxFogRange"));
        pSpinBoxFogRange->setGeometry(QRect(210, 40, 101, 18));
        pSpinBoxFogRange->setMinimum(0);
        pSpinBoxFogRange->setMaximum(100000);
        pSpinBoxFogRange->setSingleStep(50);
        label_3 = new QLabel(pGroupBoxColor_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 40, 71, 16));
        label_2 = new QLabel(pGroupBoxColor_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 20, 91, 16));
        pSpinBoxFogStartDistance = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxFogStartDistance->setObjectName(QStringLiteral("pSpinBoxFogStartDistance"));
        pSpinBoxFogStartDistance->setGeometry(QRect(210, 20, 101, 18));
        pSpinBoxFogStartDistance->setMinimum(0);
        pSpinBoxFogStartDistance->setMaximum(100000);
        pSpinBoxFogStartDistance->setSingleStep(25);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(SkyFogPanel);

        QMetaObject::connectSlotsByName(SkyFogPanel);
    } // setupUi

    void retranslateUi(QWidget *SkyFogPanel)
    {
        SkyFogPanel->setWindowTitle(QApplication::translate("SkyFogPanel", "SkyFogPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("SkyFogPanel", "Sky and Fog", 0)));
        pGroupBoxColor_2->setTitle(QApplication::translate("SkyFogPanel", "Colors", 0));
        pBtnFogColor->setText(QString());
        label_4->setText(QApplication::translate("SkyFogPanel", "Sky Color", 0));
        label->setText(QApplication::translate("SkyFogPanel", "Fog Color", 0));
        pBtnSkyColor->setText(QString());
        pGroupBoxColor_3->setTitle(QApplication::translate("SkyFogPanel", "Properties", 0));
        label_8->setText(QApplication::translate("SkyFogPanel", "Cloud Intensity", 0));
        label_9->setText(QApplication::translate("SkyFogPanel", "Energyfield Intensity", 0));
        label_3->setText(QApplication::translate("SkyFogPanel", "Fog Range", 0));
        label_2->setText(QApplication::translate("SkyFogPanel", "Fog Start Distance", 0));
    } // retranslateUi

};

namespace Ui {
    class SkyFogPanel: public Ui_SkyFogPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SKYFOGPANEL_UI_H
