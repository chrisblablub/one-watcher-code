/********************************************************************************
** Form generated from reading UI file 'LightEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LIGHTENTITYPANEL_UI_H
#define LIGHTENTITYPANEL_UI_H

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

class Ui_LightEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *groupBox;
    QRadioButton *pRadioBtnPoint;
    QRadioButton *pRadioBtnDirectional;
    QLabel *label_2;
    QPushButton *pBtnDiffuseColor;
    QLabel *label;
    QPushButton *pBtnSpecularColor;
    QLabel *label_4;
    QDoubleSpinBox *pSpinBoxRadius;
    QDoubleSpinBox *pSpinBoxEnergy;
    QLabel *label_5;
    QCheckBox *pCheckBoxSpecular;
    QCheckBox *pCheckBoxStatic;
    QCheckBox *pCheckBoxAffectsTerrain;
    QCheckBox *pCheckBoxAffectsMeshes;
    QGroupBox *pGroupBoxLightRays;
    QDoubleSpinBox *pSpinBoxIntensity;
    QDoubleSpinBox *pSpinBoxFalloff;
    QLabel *label_6;
    QLabel *label_7;
    QCheckBox *pCheckBoxWorldPosition;
    QCheckBox *pCheckBoxOcclusionMask;
    QGroupBox *pGroupBoxFlicker;
    QDoubleSpinBox *pSpinBoxFlickerTime;
    QLabel *label_8;
    QDoubleSpinBox *pSpinBoxFlickerPauseTime;
    QDoubleSpinBox *pSpinBoxFlickerIntensity;
    QDoubleSpinBox *pSpinBoxFlickerRandomness;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QGroupBox *pGroupBoxColor_7;
    QCheckBox *pCheckBoxTemplate;
    QPushButton *pBtnLoadTemplate;
    QPushButton *pBtnSaveTemplate;

    void setupUi(QWidget *LightEntityPanel)
    {
        if (LightEntityPanel->objectName().isEmpty())
            LightEntityPanel->setObjectName(QStringLiteral("LightEntityPanel"));
        LightEntityPanel->resize(340, 698);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LightEntityPanel->sizePolicy().hasHeightForWidth());
        LightEntityPanel->setSizePolicy(sizePolicy);
        LightEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(LightEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(LightEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 630));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 50, 321, 201));
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
        pRadioBtnPoint = new QRadioButton(groupBox);
        pRadioBtnPoint->setObjectName(QStringLiteral("pRadioBtnPoint"));
        pRadioBtnPoint->setGeometry(QRect(40, 20, 82, 17));
        pRadioBtnDirectional = new QRadioButton(groupBox);
        pRadioBtnDirectional->setObjectName(QStringLiteral("pRadioBtnDirectional"));
        pRadioBtnDirectional->setGeometry(QRect(190, 20, 82, 17));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 71, 16));
        pBtnDiffuseColor = new QPushButton(groupBox);
        pBtnDiffuseColor->setObjectName(QStringLiteral("pBtnDiffuseColor"));
        pBtnDiffuseColor->setGeometry(QRect(210, 50, 101, 23));
        pBtnDiffuseColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 80, 81, 16));
        pBtnSpecularColor = new QPushButton(groupBox);
        pBtnSpecularColor->setObjectName(QStringLiteral("pBtnSpecularColor"));
        pBtnSpecularColor->setGeometry(QRect(210, 80, 101, 23));
        pBtnSpecularColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 150, 81, 16));
        pSpinBoxRadius = new QDoubleSpinBox(groupBox);
        pSpinBoxRadius->setObjectName(QStringLiteral("pSpinBoxRadius"));
        pSpinBoxRadius->setGeometry(QRect(210, 150, 101, 18));
        pSpinBoxRadius->setMaximum(100000);
        pSpinBoxRadius->setSingleStep(10);
        pSpinBoxEnergy = new QDoubleSpinBox(groupBox);
        pSpinBoxEnergy->setObjectName(QStringLiteral("pSpinBoxEnergy"));
        pSpinBoxEnergy->setGeometry(QRect(210, 170, 101, 18));
        pSpinBoxEnergy->setSingleStep(0.5);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 170, 71, 16));
        pCheckBoxSpecular = new QCheckBox(groupBox);
        pCheckBoxSpecular->setObjectName(QStringLiteral("pCheckBoxSpecular"));
        pCheckBoxSpecular->setGeometry(QRect(210, 110, 91, 17));
        pCheckBoxStatic = new QCheckBox(groupBox);
        pCheckBoxStatic->setObjectName(QStringLiteral("pCheckBoxStatic"));
        pCheckBoxStatic->setGeometry(QRect(210, 130, 91, 17));
        pCheckBoxAffectsTerrain = new QCheckBox(groupBox);
        pCheckBoxAffectsTerrain->setObjectName(QStringLiteral("pCheckBoxAffectsTerrain"));
        pCheckBoxAffectsTerrain->setEnabled(false);
        pCheckBoxAffectsTerrain->setGeometry(QRect(20, 110, 101, 17));
        pCheckBoxAffectsMeshes = new QCheckBox(groupBox);
        pCheckBoxAffectsMeshes->setObjectName(QStringLiteral("pCheckBoxAffectsMeshes"));
        pCheckBoxAffectsMeshes->setEnabled(true);
        pCheckBoxAffectsMeshes->setGeometry(QRect(20, 130, 101, 17));
        pCheckBoxAffectsMeshes->setChecked(true);
        pGroupBoxLightRays = new QGroupBox(qEditorPanelBox);
        pGroupBoxLightRays->setObjectName(QStringLiteral("pGroupBoxLightRays"));
        pGroupBoxLightRays->setGeometry(QRect(10, 260, 321, 111));
        pGroupBoxLightRays->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pGroupBoxLightRays->setCheckable(true);
        pGroupBoxLightRays->setChecked(false);
        pSpinBoxIntensity = new QDoubleSpinBox(pGroupBoxLightRays);
        pSpinBoxIntensity->setObjectName(QStringLiteral("pSpinBoxIntensity"));
        pSpinBoxIntensity->setGeometry(QRect(210, 20, 101, 18));
        pSpinBoxIntensity->setMinimum(0);
        pSpinBoxIntensity->setMaximum(100000);
        pSpinBoxIntensity->setSingleStep(0.01);
        pSpinBoxFalloff = new QDoubleSpinBox(pGroupBoxLightRays);
        pSpinBoxFalloff->setObjectName(QStringLiteral("pSpinBoxFalloff"));
        pSpinBoxFalloff->setGeometry(QRect(210, 40, 101, 18));
        pSpinBoxFalloff->setMinimum(0);
        pSpinBoxFalloff->setMaximum(100000);
        pSpinBoxFalloff->setSingleStep(0.01);
        label_6 = new QLabel(pGroupBoxLightRays);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 20, 111, 16));
        label_7 = new QLabel(pGroupBoxLightRays);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 40, 111, 16));
        pCheckBoxWorldPosition = new QCheckBox(pGroupBoxLightRays);
        pCheckBoxWorldPosition->setObjectName(QStringLiteral("pCheckBoxWorldPosition"));
        pCheckBoxWorldPosition->setGeometry(QRect(190, 80, 111, 17));
        pCheckBoxOcclusionMask = new QCheckBox(pGroupBoxLightRays);
        pCheckBoxOcclusionMask->setObjectName(QStringLiteral("pCheckBoxOcclusionMask"));
        pCheckBoxOcclusionMask->setGeometry(QRect(190, 60, 121, 17));
        pGroupBoxFlicker = new QGroupBox(qEditorPanelBox);
        pGroupBoxFlicker->setObjectName(QStringLiteral("pGroupBoxFlicker"));
        pGroupBoxFlicker->setGeometry(QRect(10, 380, 321, 111));
        pGroupBoxFlicker->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pGroupBoxFlicker->setCheckable(true);
        pGroupBoxFlicker->setChecked(false);
        pSpinBoxFlickerTime = new QDoubleSpinBox(pGroupBoxFlicker);
        pSpinBoxFlickerTime->setObjectName(QStringLiteral("pSpinBoxFlickerTime"));
        pSpinBoxFlickerTime->setGeometry(QRect(210, 20, 101, 18));
        pSpinBoxFlickerTime->setMaximum(100000);
        pSpinBoxFlickerTime->setSingleStep(0.05);
        label_8 = new QLabel(pGroupBoxFlicker);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 20, 111, 16));
        pSpinBoxFlickerPauseTime = new QDoubleSpinBox(pGroupBoxFlicker);
        pSpinBoxFlickerPauseTime->setObjectName(QStringLiteral("pSpinBoxFlickerPauseTime"));
        pSpinBoxFlickerPauseTime->setGeometry(QRect(210, 40, 101, 18));
        pSpinBoxFlickerPauseTime->setMaximum(100000);
        pSpinBoxFlickerPauseTime->setSingleStep(0.05);
        pSpinBoxFlickerIntensity = new QDoubleSpinBox(pGroupBoxFlicker);
        pSpinBoxFlickerIntensity->setObjectName(QStringLiteral("pSpinBoxFlickerIntensity"));
        pSpinBoxFlickerIntensity->setGeometry(QRect(210, 60, 101, 18));
        pSpinBoxFlickerIntensity->setMaximum(100000);
        pSpinBoxFlickerIntensity->setSingleStep(0.05);
        pSpinBoxFlickerRandomness = new QDoubleSpinBox(pGroupBoxFlicker);
        pSpinBoxFlickerRandomness->setObjectName(QStringLiteral("pSpinBoxFlickerRandomness"));
        pSpinBoxFlickerRandomness->setGeometry(QRect(210, 80, 101, 18));
        pSpinBoxFlickerRandomness->setMaximum(1);
        pSpinBoxFlickerRandomness->setSingleStep(0.05);
        label_9 = new QLabel(pGroupBoxFlicker);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 40, 111, 16));
        label_10 = new QLabel(pGroupBoxFlicker);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 60, 111, 16));
        label_11 = new QLabel(pGroupBoxFlicker);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 80, 111, 16));
        pGroupBoxColor_7 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_7->setObjectName(QStringLiteral("pGroupBoxColor_7"));
        pGroupBoxColor_7->setGeometry(QRect(10, 500, 321, 111));
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
        pBtnLoadTemplate->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pBtnSaveTemplate = new QPushButton(pGroupBoxColor_7);
        pBtnSaveTemplate->setObjectName(QStringLiteral("pBtnSaveTemplate"));
        pBtnSaveTemplate->setGeometry(QRect(10, 20, 301, 23));
        pBtnSaveTemplate->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(LightEntityPanel);

        QMetaObject::connectSlotsByName(LightEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *LightEntityPanel)
    {
        LightEntityPanel->setWindowTitle(QApplication::translate("LightEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("LightEntityPanel", "Light", 0)));
        pLabelName->setText(QString());
        groupBox->setTitle(QApplication::translate("LightEntityPanel", "General", 0));
        pRadioBtnPoint->setText(QApplication::translate("LightEntityPanel", "Point", 0));
        pRadioBtnDirectional->setText(QApplication::translate("LightEntityPanel", "Directional", 0));
        label_2->setText(QApplication::translate("LightEntityPanel", "Diffuse Color", 0));
        pBtnDiffuseColor->setText(QString());
        label->setText(QApplication::translate("LightEntityPanel", "Specular Color", 0));
        pBtnSpecularColor->setText(QString());
        label_4->setText(QApplication::translate("LightEntityPanel", "Radius", 0));
        label_5->setText(QApplication::translate("LightEntityPanel", "Energy", 0));
        pCheckBoxSpecular->setText(QApplication::translate("LightEntityPanel", "Is Specular", 0));
        pCheckBoxStatic->setText(QApplication::translate("LightEntityPanel", "Is Static", 0));
        pCheckBoxAffectsTerrain->setText(QApplication::translate("LightEntityPanel", "Affects Terrain", 0));
        pCheckBoxAffectsMeshes->setText(QApplication::translate("LightEntityPanel", "Affects Meshes", 0));
        pGroupBoxLightRays->setTitle(QApplication::translate("LightEntityPanel", "Light Rays", 0));
        label_6->setText(QApplication::translate("LightEntityPanel", "Intensity", 0));
        label_7->setText(QApplication::translate("LightEntityPanel", "Falloff", 0));
#ifndef QT_NO_TOOLTIP
        pCheckBoxWorldPosition->setToolTip(QApplication::translate("LightEntityPanel", "If unchecked, the origin of the light rays will always be the center of the screen.", 0));
#endif // QT_NO_TOOLTIP
        pCheckBoxWorldPosition->setText(QApplication::translate("LightEntityPanel", "Use World Position", 0));
        pCheckBoxOcclusionMask->setText(QApplication::translate("LightEntityPanel", "Use Occlusion Mask", 0));
        pGroupBoxFlicker->setTitle(QApplication::translate("LightEntityPanel", "Flicker", 0));
        label_8->setText(QApplication::translate("LightEntityPanel", "Time", 0));
        label_9->setText(QApplication::translate("LightEntityPanel", "Pause Time", 0));
        label_10->setText(QApplication::translate("LightEntityPanel", "Intensity", 0));
        label_11->setText(QApplication::translate("LightEntityPanel", "Randomness", 0));
        pGroupBoxColor_7->setTitle(QApplication::translate("LightEntityPanel", "Light Template", 0));
        pCheckBoxTemplate->setText(QApplication::translate("LightEntityPanel", "Init from template after loading", 0));
        pBtnLoadTemplate->setText(QApplication::translate("LightEntityPanel", "Load from light template", 0));
        pBtnSaveTemplate->setText(QApplication::translate("LightEntityPanel", "Save light template", 0));
    } // retranslateUi

};

namespace Ui {
    class LightEntityPanel: public Ui_LightEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LIGHTENTITYPANEL_UI_H
