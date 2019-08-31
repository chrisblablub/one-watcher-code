/********************************************************************************
** Form generated from reading UI file 'ColorGradingPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef COLORGRADINGPANEL_UI_H
#define COLORGRADINGPANEL_UI_H

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
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_ColorGradingPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *groupBox;
    QSlider *pSliderColorBalanceR;
    QLabel *label;
    QSlider *pSliderColorBalanceG;
    QLabel *label_2;
    QLabel *label_3;
    QSlider *pSliderColorBalanceB;
    QPushButton *pBtnResetBalanceR;
    QPushButton *pBtnResetBalanceB;
    QPushButton *pBtnResetBalanceG;
    QGroupBox *groupBox_2;
    QLabel *label_4;
    QSlider *pSliderColorBrightnessR;
    QSlider *pSliderColorBrightnessG;
    QLabel *label_5;
    QLabel *label_6;
    QSlider *pSliderColorBrightnessB;
    QPushButton *pBtnResetBrightnessR;
    QPushButton *pBtnResetBrightnessB;
    QPushButton *pBtnResetBrightnessG;
    QGroupBox *groupBox_3;
    QLabel *label_7;
    QLabel *label_8;
    QDoubleSpinBox *pSpinBoxBloomThreshold;
    QDoubleSpinBox *pSpinBoxBloomScaling;
    QGroupBox *groupBox_4;
    QSlider *pSliderHue;
    QLabel *label_9;
    QSlider *pSliderSaturation;
    QLabel *label_10;
    QPushButton *pBtnResetHue;
    QPushButton *pBtnResetSaturation;

    void setupUi(QWidget *ColorGradingPanel)
    {
        if (ColorGradingPanel->objectName().isEmpty())
            ColorGradingPanel->setObjectName(QStringLiteral("ColorGradingPanel"));
        ColorGradingPanel->resize(330, 617);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ColorGradingPanel->sizePolicy().hasHeightForWidth());
        ColorGradingPanel->setSizePolicy(sizePolicy);
        ColorGradingPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(ColorGradingPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(ColorGradingPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 480));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 150, 311, 121));
        groupBox->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pSliderColorBalanceR = new QSlider(groupBox);
        pSliderColorBalanceR->setObjectName(QStringLiteral("pSliderColorBalanceR"));
        pSliderColorBalanceR->setGeometry(QRect(39, 30, 201, 21));
        pSliderColorBalanceR->setMaximum(199);
        pSliderColorBalanceR->setOrientation(Qt::Horizontal);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 30, 21, 16));
        pSliderColorBalanceG = new QSlider(groupBox);
        pSliderColorBalanceG->setObjectName(QStringLiteral("pSliderColorBalanceG"));
        pSliderColorBalanceG->setGeometry(QRect(39, 55, 201, 21));
        pSliderColorBalanceG->setMaximum(199);
        pSliderColorBalanceG->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 55, 16, 16));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 80, 16, 16));
        pSliderColorBalanceB = new QSlider(groupBox);
        pSliderColorBalanceB->setObjectName(QStringLiteral("pSliderColorBalanceB"));
        pSliderColorBalanceB->setGeometry(QRect(39, 80, 201, 21));
        pSliderColorBalanceB->setMaximum(199);
        pSliderColorBalanceB->setOrientation(Qt::Horizontal);
        pBtnResetBalanceR = new QPushButton(groupBox);
        pBtnResetBalanceR->setObjectName(QStringLiteral("pBtnResetBalanceR"));
        pBtnResetBalanceR->setGeometry(QRect(250, 30, 51, 21));
        pBtnResetBalanceB = new QPushButton(groupBox);
        pBtnResetBalanceB->setObjectName(QStringLiteral("pBtnResetBalanceB"));
        pBtnResetBalanceB->setGeometry(QRect(250, 80, 51, 21));
        pBtnResetBalanceG = new QPushButton(groupBox);
        pBtnResetBalanceG->setObjectName(QStringLiteral("pBtnResetBalanceG"));
        pBtnResetBalanceG->setGeometry(QRect(250, 55, 51, 21));
        groupBox_2 = new QGroupBox(qEditorPanelBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 280, 311, 111));
        groupBox_2->setStyleSheet(QLatin1String("QGroupBox\n"
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
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 30, 21, 16));
        pSliderColorBrightnessR = new QSlider(groupBox_2);
        pSliderColorBrightnessR->setObjectName(QStringLiteral("pSliderColorBrightnessR"));
        pSliderColorBrightnessR->setGeometry(QRect(39, 30, 201, 20));
        pSliderColorBrightnessR->setOrientation(Qt::Horizontal);
        pSliderColorBrightnessG = new QSlider(groupBox_2);
        pSliderColorBrightnessG->setObjectName(QStringLiteral("pSliderColorBrightnessG"));
        pSliderColorBrightnessG->setGeometry(QRect(39, 55, 201, 20));
        pSliderColorBrightnessG->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 55, 21, 16));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 80, 21, 16));
        pSliderColorBrightnessB = new QSlider(groupBox_2);
        pSliderColorBrightnessB->setObjectName(QStringLiteral("pSliderColorBrightnessB"));
        pSliderColorBrightnessB->setGeometry(QRect(39, 80, 201, 20));
        pSliderColorBrightnessB->setOrientation(Qt::Horizontal);
        pBtnResetBrightnessR = new QPushButton(groupBox_2);
        pBtnResetBrightnessR->setObjectName(QStringLiteral("pBtnResetBrightnessR"));
        pBtnResetBrightnessR->setGeometry(QRect(250, 30, 51, 21));
        pBtnResetBrightnessB = new QPushButton(groupBox_2);
        pBtnResetBrightnessB->setObjectName(QStringLiteral("pBtnResetBrightnessB"));
        pBtnResetBrightnessB->setGeometry(QRect(250, 80, 51, 21));
        pBtnResetBrightnessG = new QPushButton(groupBox_2);
        pBtnResetBrightnessG->setObjectName(QStringLiteral("pBtnResetBrightnessG"));
        pBtnResetBrightnessG->setGeometry(QRect(250, 55, 51, 21));
        groupBox_3 = new QGroupBox(qEditorPanelBox);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 400, 311, 71));
        groupBox_3->setStyleSheet(QLatin1String("QGroupBox\n"
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
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 20, 47, 16));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 40, 47, 16));
        pSpinBoxBloomThreshold = new QDoubleSpinBox(groupBox_3);
        pSpinBoxBloomThreshold->setObjectName(QStringLiteral("pSpinBoxBloomThreshold"));
        pSpinBoxBloomThreshold->setGeometry(QRect(220, 20, 81, 18));
        pSpinBoxBloomThreshold->setSingleStep(0.05);
        pSpinBoxBloomScaling = new QDoubleSpinBox(groupBox_3);
        pSpinBoxBloomScaling->setObjectName(QStringLiteral("pSpinBoxBloomScaling"));
        pSpinBoxBloomScaling->setGeometry(QRect(220, 40, 81, 18));
        pSpinBoxBloomScaling->setSingleStep(0.05);
        groupBox_4 = new QGroupBox(qEditorPanelBox);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 50, 311, 91));
        groupBox_4->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pSliderHue = new QSlider(groupBox_4);
        pSliderHue->setObjectName(QStringLiteral("pSliderHue"));
        pSliderHue->setGeometry(QRect(79, 30, 161, 21));
        pSliderHue->setMaximum(360);
        pSliderHue->setOrientation(Qt::Horizontal);
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 30, 21, 16));
        pSliderSaturation = new QSlider(groupBox_4);
        pSliderSaturation->setObjectName(QStringLiteral("pSliderSaturation"));
        pSliderSaturation->setGeometry(QRect(79, 55, 161, 21));
        pSliderSaturation->setMaximum(99);
        pSliderSaturation->setOrientation(Qt::Horizontal);
        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(20, 55, 51, 16));
        pBtnResetHue = new QPushButton(groupBox_4);
        pBtnResetHue->setObjectName(QStringLiteral("pBtnResetHue"));
        pBtnResetHue->setGeometry(QRect(250, 30, 51, 21));
        pBtnResetSaturation = new QPushButton(groupBox_4);
        pBtnResetSaturation->setObjectName(QStringLiteral("pBtnResetSaturation"));
        pBtnResetSaturation->setGeometry(QRect(250, 55, 51, 21));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(ColorGradingPanel);

        QMetaObject::connectSlotsByName(ColorGradingPanel);
    } // setupUi

    void retranslateUi(QWidget *ColorGradingPanel)
    {
        ColorGradingPanel->setWindowTitle(QApplication::translate("ColorGradingPanel", "ColorGradingPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("ColorGradingPanel", "Color Grading", 0)));
        pLabelName->setText(QString());
        groupBox->setTitle(QApplication::translate("ColorGradingPanel", "Color Balance", 0));
        label->setText(QApplication::translate("ColorGradingPanel", "R", 0));
        label_2->setText(QApplication::translate("ColorGradingPanel", "G", 0));
        label_3->setText(QApplication::translate("ColorGradingPanel", "B", 0));
        pBtnResetBalanceR->setText(QApplication::translate("ColorGradingPanel", "Reset", 0));
        pBtnResetBalanceB->setText(QApplication::translate("ColorGradingPanel", "Reset", 0));
        pBtnResetBalanceG->setText(QApplication::translate("ColorGradingPanel", "Reset", 0));
        groupBox_2->setTitle(QApplication::translate("ColorGradingPanel", "Color Brightness", 0));
        label_4->setText(QApplication::translate("ColorGradingPanel", "R", 0));
        label_5->setText(QApplication::translate("ColorGradingPanel", "G", 0));
        label_6->setText(QApplication::translate("ColorGradingPanel", "B", 0));
        pBtnResetBrightnessR->setText(QApplication::translate("ColorGradingPanel", "Reset", 0));
        pBtnResetBrightnessB->setText(QApplication::translate("ColorGradingPanel", "Reset", 0));
        pBtnResetBrightnessG->setText(QApplication::translate("ColorGradingPanel", "Reset", 0));
        groupBox_3->setTitle(QApplication::translate("ColorGradingPanel", "Bloom", 0));
        label_7->setText(QApplication::translate("ColorGradingPanel", "Threshold", 0));
        label_8->setText(QApplication::translate("ColorGradingPanel", "Scaling", 0));
        groupBox_4->setTitle(QApplication::translate("ColorGradingPanel", "Colors", 0));
        label_9->setText(QApplication::translate("ColorGradingPanel", "Hue", 0));
        label_10->setText(QApplication::translate("ColorGradingPanel", "Saturation", 0));
        pBtnResetHue->setText(QApplication::translate("ColorGradingPanel", "Reset", 0));
        pBtnResetSaturation->setText(QApplication::translate("ColorGradingPanel", "Reset", 0));
    } // retranslateUi

};

namespace Ui {
    class ColorGradingPanel: public Ui_ColorGradingPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // COLORGRADINGPANEL_UI_H
