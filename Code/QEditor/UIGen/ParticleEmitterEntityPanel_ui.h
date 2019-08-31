/********************************************************************************
** Form generated from reading UI file 'ParticleEmitterEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PARTICLEEMITTERENTITYPANEL_UI_H
#define PARTICLEEMITTERENTITYPANEL_UI_H

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

class Ui_ParticleEmitterEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *groupBox;
    QPushButton *pBtnColorBegin;
    QLabel *label;
    QDoubleSpinBox *pSpinBoxSizeRange;
    QSpinBox *pSpinBoxMaterialSequence;
    QSpinBox *pSpinBoxSizeEnd;
    QSpinBox *pSpinBoxSizeBegin;
    QSpinBox *pSpinBoxParticleCount;
    QDoubleSpinBox *pSpinBoxFadeOut;
    QDoubleSpinBox *pSpinBoxFadeIn;
    QDoubleSpinBox *pSpinBoxAlphaBegin;
    QDoubleSpinBox *pSpinBoxAlphaEnd;
    QPushButton *pBtnColorEnd;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_11;
    QLabel *label_13;
    QDoubleSpinBox *pSpinBoxVelocityAngle;
    QDoubleSpinBox *pSpinBoxVelocityMax;
    QDoubleSpinBox *pSpinBoxVelocityMin;
    QLabel *label_14;
    QLabel *label_15;
    QDoubleSpinBox *pSpinBoxLifetimeMax;
    QDoubleSpinBox *pSpinBoxLifetimeMin;
    QLabel *label_19;
    QDoubleSpinBox *pSpinBoxEmitDuration;
    QDoubleSpinBox *pSpinBoxEmitRate;
    QDoubleSpinBox *pSpinBoxEmitDelay;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_23;
    QDoubleSpinBox *pSpinBoxWarmupTime;
    QCheckBox *pCheckBoxGlobalSpaceParticles;
    QCheckBox *pCheckBoxEmitAllParticlesOnce;
    QCheckBox *pCheckBoxStartDisabled;
    QPushButton *pBtnPickParticleSystem;
    QGroupBox *groupBox_2;
    QRadioButton *pRadioBtnOrientationAlignToVelocity;
    QRadioButton *pRadioBtnOrientationRandom;
    QDoubleSpinBox *pSpinBoxOrientationVelocityMin;
    QDoubleSpinBox *pSpinBoxOrientationVelocityMax;
    QLabel *label_17;
    QGroupBox *groupBox_3;
    QRadioButton *pRadioBtnFacingEmitter;
    QRadioButton *pRadioBtnFacingCamera;
    QRadioButton *pRadioBtnFacingHorizontal;
    QRadioButton *pRadioBtnFacingScreen;
    QGroupBox *pGroupBoxStaticBoundingBox;
    QPushButton *pBtnCaptureBoundingBox;
    QGroupBox *pGroupBoxDistort;
    QDoubleSpinBox *pSpinBoxDistortionMax;
    QDoubleSpinBox *pSpinBoxDistortionMin;
    QLabel *label_18;
    QLabel *label_24;
    QGroupBox *pGroupBoxColor;
    QDoubleSpinBox *pSpinBoxBlendMode;
    QLabel *label_25;

    void setupUi(QWidget *ParticleEmitterEntityPanel)
    {
        if (ParticleEmitterEntityPanel->objectName().isEmpty())
            ParticleEmitterEntityPanel->setObjectName(QStringLiteral("ParticleEmitterEntityPanel"));
        ParticleEmitterEntityPanel->resize(340, 918);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ParticleEmitterEntityPanel->sizePolicy().hasHeightForWidth());
        ParticleEmitterEntityPanel->setSizePolicy(sizePolicy);
        ParticleEmitterEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(ParticleEmitterEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(ParticleEmitterEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 840));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 50, 321, 391));
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
        pBtnColorBegin = new QPushButton(groupBox);
        pBtnColorBegin->setObjectName(QStringLiteral("pBtnColorBegin"));
        pBtnColorBegin->setGeometry(QRect(160, 110, 71, 23));
        pBtnColorBegin->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
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
        label->setGeometry(QRect(10, 30, 101, 16));
        pSpinBoxSizeRange = new QDoubleSpinBox(groupBox);
        pSpinBoxSizeRange->setObjectName(QStringLiteral("pSpinBoxSizeRange"));
        pSpinBoxSizeRange->setGeometry(QRect(240, 140, 71, 18));
        pSpinBoxSizeRange->setMinimum(0);
        pSpinBoxSizeRange->setMaximum(1);
        pSpinBoxSizeRange->setSingleStep(0.05);
        pSpinBoxMaterialSequence = new QSpinBox(groupBox);
        pSpinBoxMaterialSequence->setObjectName(QStringLiteral("pSpinBoxMaterialSequence"));
        pSpinBoxMaterialSequence->setGeometry(QRect(240, 30, 71, 18));
        pSpinBoxSizeEnd = new QSpinBox(groupBox);
        pSpinBoxSizeEnd->setObjectName(QStringLiteral("pSpinBoxSizeEnd"));
        pSpinBoxSizeEnd->setGeometry(QRect(240, 50, 71, 18));
        pSpinBoxSizeEnd->setMaximum(1000);
        pSpinBoxSizeBegin = new QSpinBox(groupBox);
        pSpinBoxSizeBegin->setObjectName(QStringLiteral("pSpinBoxSizeBegin"));
        pSpinBoxSizeBegin->setGeometry(QRect(160, 50, 71, 18));
        pSpinBoxSizeBegin->setMaximum(1000);
        pSpinBoxParticleCount = new QSpinBox(groupBox);
        pSpinBoxParticleCount->setObjectName(QStringLiteral("pSpinBoxParticleCount"));
        pSpinBoxParticleCount->setGeometry(QRect(240, 160, 71, 18));
        pSpinBoxParticleCount->setMaximum(1000);
        pSpinBoxFadeOut = new QDoubleSpinBox(groupBox);
        pSpinBoxFadeOut->setObjectName(QStringLiteral("pSpinBoxFadeOut"));
        pSpinBoxFadeOut->setGeometry(QRect(240, 70, 71, 18));
        pSpinBoxFadeOut->setMinimum(0);
        pSpinBoxFadeOut->setMaximum(100000);
        pSpinBoxFadeOut->setSingleStep(0.1);
        pSpinBoxFadeIn = new QDoubleSpinBox(groupBox);
        pSpinBoxFadeIn->setObjectName(QStringLiteral("pSpinBoxFadeIn"));
        pSpinBoxFadeIn->setGeometry(QRect(160, 70, 71, 18));
        pSpinBoxFadeIn->setMinimum(0);
        pSpinBoxFadeIn->setMaximum(100000);
        pSpinBoxFadeIn->setSingleStep(0.1);
        pSpinBoxAlphaBegin = new QDoubleSpinBox(groupBox);
        pSpinBoxAlphaBegin->setObjectName(QStringLiteral("pSpinBoxAlphaBegin"));
        pSpinBoxAlphaBegin->setGeometry(QRect(160, 90, 71, 18));
        pSpinBoxAlphaBegin->setMinimum(-100000);
        pSpinBoxAlphaBegin->setMaximum(100000);
        pSpinBoxAlphaBegin->setSingleStep(0.1);
        pSpinBoxAlphaEnd = new QDoubleSpinBox(groupBox);
        pSpinBoxAlphaEnd->setObjectName(QStringLiteral("pSpinBoxAlphaEnd"));
        pSpinBoxAlphaEnd->setGeometry(QRect(240, 90, 71, 18));
        pSpinBoxAlphaEnd->setMinimum(-100000);
        pSpinBoxAlphaEnd->setMaximum(100000);
        pSpinBoxAlphaEnd->setSingleStep(0.1);
        pBtnColorEnd = new QPushButton(groupBox);
        pBtnColorEnd->setObjectName(QStringLiteral("pBtnColorEnd"));
        pBtnColorEnd->setGeometry(QRect(240, 110, 71, 23));
        pBtnColorEnd->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 61, 16));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 140, 141, 16));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 160, 101, 16));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 70, 71, 16));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 90, 61, 16));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 111, 61, 20));
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 180, 101, 16));
        pSpinBoxVelocityAngle = new QDoubleSpinBox(groupBox);
        pSpinBoxVelocityAngle->setObjectName(QStringLiteral("pSpinBoxVelocityAngle"));
        pSpinBoxVelocityAngle->setGeometry(QRect(240, 180, 71, 18));
        pSpinBoxVelocityAngle->setMinimum(0);
        pSpinBoxVelocityAngle->setMaximum(360);
        pSpinBoxVelocityAngle->setSingleStep(10);
        pSpinBoxVelocityMax = new QDoubleSpinBox(groupBox);
        pSpinBoxVelocityMax->setObjectName(QStringLiteral("pSpinBoxVelocityMax"));
        pSpinBoxVelocityMax->setGeometry(QRect(240, 200, 71, 18));
        pSpinBoxVelocityMax->setMinimum(0);
        pSpinBoxVelocityMax->setMaximum(100000);
        pSpinBoxVelocityMax->setSingleStep(10);
        pSpinBoxVelocityMin = new QDoubleSpinBox(groupBox);
        pSpinBoxVelocityMin->setObjectName(QStringLiteral("pSpinBoxVelocityMin"));
        pSpinBoxVelocityMin->setGeometry(QRect(160, 200, 71, 18));
        pSpinBoxVelocityMin->setMinimum(0);
        pSpinBoxVelocityMin->setMaximum(100000);
        pSpinBoxVelocityMin->setSingleStep(10);
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(10, 200, 101, 16));
        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(70, 200, 61, 16));
        pSpinBoxLifetimeMax = new QDoubleSpinBox(groupBox);
        pSpinBoxLifetimeMax->setObjectName(QStringLiteral("pSpinBoxLifetimeMax"));
        pSpinBoxLifetimeMax->setGeometry(QRect(240, 220, 71, 18));
        pSpinBoxLifetimeMax->setMinimum(0);
        pSpinBoxLifetimeMax->setMaximum(100000);
        pSpinBoxLifetimeMax->setSingleStep(0.1);
        pSpinBoxLifetimeMin = new QDoubleSpinBox(groupBox);
        pSpinBoxLifetimeMin->setObjectName(QStringLiteral("pSpinBoxLifetimeMin"));
        pSpinBoxLifetimeMin->setGeometry(QRect(160, 220, 71, 18));
        pSpinBoxLifetimeMin->setMinimum(0);
        pSpinBoxLifetimeMin->setMaximum(100000);
        pSpinBoxLifetimeMin->setSingleStep(0.1);
        label_19 = new QLabel(groupBox);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(10, 220, 61, 16));
        pSpinBoxEmitDuration = new QDoubleSpinBox(groupBox);
        pSpinBoxEmitDuration->setObjectName(QStringLiteral("pSpinBoxEmitDuration"));
        pSpinBoxEmitDuration->setGeometry(QRect(240, 260, 71, 18));
        pSpinBoxEmitDuration->setMinimum(0);
        pSpinBoxEmitDuration->setMaximum(100000);
        pSpinBoxEmitDuration->setSingleStep(0.05);
        pSpinBoxEmitRate = new QDoubleSpinBox(groupBox);
        pSpinBoxEmitRate->setObjectName(QStringLiteral("pSpinBoxEmitRate"));
        pSpinBoxEmitRate->setGeometry(QRect(240, 240, 71, 18));
        pSpinBoxEmitRate->setMinimum(0);
        pSpinBoxEmitRate->setMaximum(100000);
        pSpinBoxEmitRate->setSingleStep(0.05);
        pSpinBoxEmitDelay = new QDoubleSpinBox(groupBox);
        pSpinBoxEmitDelay->setObjectName(QStringLiteral("pSpinBoxEmitDelay"));
        pSpinBoxEmitDelay->setGeometry(QRect(240, 280, 71, 18));
        pSpinBoxEmitDelay->setMinimum(0);
        pSpinBoxEmitDelay->setMaximum(100000);
        pSpinBoxEmitDelay->setSingleStep(0.05);
        label_20 = new QLabel(groupBox);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(10, 240, 61, 16));
        label_21 = new QLabel(groupBox);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(10, 260, 71, 16));
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(10, 280, 81, 16));
        label_23 = new QLabel(groupBox);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(10, 300, 81, 16));
        pSpinBoxWarmupTime = new QDoubleSpinBox(groupBox);
        pSpinBoxWarmupTime->setObjectName(QStringLiteral("pSpinBoxWarmupTime"));
        pSpinBoxWarmupTime->setGeometry(QRect(240, 300, 71, 18));
        pSpinBoxWarmupTime->setMinimum(0);
        pSpinBoxWarmupTime->setMaximum(100000);
        pSpinBoxWarmupTime->setSingleStep(0.05);
        pCheckBoxGlobalSpaceParticles = new QCheckBox(groupBox);
        pCheckBoxGlobalSpaceParticles->setObjectName(QStringLiteral("pCheckBoxGlobalSpaceParticles"));
        pCheckBoxGlobalSpaceParticles->setGeometry(QRect(180, 320, 131, 20));
        pCheckBoxEmitAllParticlesOnce = new QCheckBox(groupBox);
        pCheckBoxEmitAllParticlesOnce->setObjectName(QStringLiteral("pCheckBoxEmitAllParticlesOnce"));
        pCheckBoxEmitAllParticlesOnce->setGeometry(QRect(180, 340, 131, 17));
        pCheckBoxStartDisabled = new QCheckBox(groupBox);
        pCheckBoxStartDisabled->setObjectName(QStringLiteral("pCheckBoxStartDisabled"));
        pCheckBoxStartDisabled->setGeometry(QRect(180, 360, 131, 17));
        pBtnPickParticleSystem = new QPushButton(qEditorPanelBox);
        pBtnPickParticleSystem->setObjectName(QStringLiteral("pBtnPickParticleSystem"));
        pBtnPickParticleSystem->setGeometry(QRect(20, 800, 301, 23));
        pBtnPickParticleSystem->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        groupBox_2 = new QGroupBox(qEditorPanelBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 570, 321, 80));
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
        pRadioBtnOrientationAlignToVelocity = new QRadioButton(groupBox_2);
        pRadioBtnOrientationAlignToVelocity->setObjectName(QStringLiteral("pRadioBtnOrientationAlignToVelocity"));
        pRadioBtnOrientationAlignToVelocity->setGeometry(QRect(10, 20, 101, 17));
        pRadioBtnOrientationRandom = new QRadioButton(groupBox_2);
        pRadioBtnOrientationRandom->setObjectName(QStringLiteral("pRadioBtnOrientationRandom"));
        pRadioBtnOrientationRandom->setGeometry(QRect(240, 20, 71, 17));
        pSpinBoxOrientationVelocityMin = new QDoubleSpinBox(groupBox_2);
        pSpinBoxOrientationVelocityMin->setObjectName(QStringLiteral("pSpinBoxOrientationVelocityMin"));
        pSpinBoxOrientationVelocityMin->setGeometry(QRect(160, 50, 71, 18));
        pSpinBoxOrientationVelocityMin->setMinimum(-100000);
        pSpinBoxOrientationVelocityMin->setMaximum(100000);
        pSpinBoxOrientationVelocityMin->setSingleStep(0.1);
        pSpinBoxOrientationVelocityMax = new QDoubleSpinBox(groupBox_2);
        pSpinBoxOrientationVelocityMax->setObjectName(QStringLiteral("pSpinBoxOrientationVelocityMax"));
        pSpinBoxOrientationVelocityMax->setGeometry(QRect(240, 50, 71, 18));
        pSpinBoxOrientationVelocityMax->setMinimum(-100000);
        pSpinBoxOrientationVelocityMax->setMaximum(100000);
        pSpinBoxOrientationVelocityMax->setSingleStep(0.1);
        label_17 = new QLabel(groupBox_2);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(10, 50, 101, 16));
        groupBox_3 = new QGroupBox(qEditorPanelBox);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 660, 321, 71));
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
        pRadioBtnFacingEmitter = new QRadioButton(groupBox_3);
        pRadioBtnFacingEmitter->setObjectName(QStringLiteral("pRadioBtnFacingEmitter"));
        pRadioBtnFacingEmitter->setGeometry(QRect(10, 20, 82, 17));
        pRadioBtnFacingCamera = new QRadioButton(groupBox_3);
        pRadioBtnFacingCamera->setObjectName(QStringLiteral("pRadioBtnFacingCamera"));
        pRadioBtnFacingCamera->setGeometry(QRect(240, 20, 71, 17));
        pRadioBtnFacingHorizontal = new QRadioButton(groupBox_3);
        pRadioBtnFacingHorizontal->setObjectName(QStringLiteral("pRadioBtnFacingHorizontal"));
        pRadioBtnFacingHorizontal->setGeometry(QRect(10, 40, 82, 17));
        pRadioBtnFacingScreen = new QRadioButton(groupBox_3);
        pRadioBtnFacingScreen->setObjectName(QStringLiteral("pRadioBtnFacingScreen"));
        pRadioBtnFacingScreen->setGeometry(QRect(240, 40, 71, 17));
        pGroupBoxStaticBoundingBox = new QGroupBox(qEditorPanelBox);
        pGroupBoxStaticBoundingBox->setObjectName(QStringLiteral("pGroupBoxStaticBoundingBox"));
        pGroupBoxStaticBoundingBox->setGeometry(QRect(10, 740, 321, 51));
        pGroupBoxStaticBoundingBox->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pGroupBoxStaticBoundingBox->setCheckable(true);
        pGroupBoxStaticBoundingBox->setChecked(false);
        pBtnCaptureBoundingBox = new QPushButton(pGroupBoxStaticBoundingBox);
        pBtnCaptureBoundingBox->setObjectName(QStringLiteral("pBtnCaptureBoundingBox"));
        pBtnCaptureBoundingBox->setGeometry(QRect(10, 20, 301, 23));
        pBtnCaptureBoundingBox->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pGroupBoxDistort = new QGroupBox(qEditorPanelBox);
        pGroupBoxDistort->setObjectName(QStringLiteral("pGroupBoxDistort"));
        pGroupBoxDistort->setGeometry(QRect(10, 510, 321, 51));
        pGroupBoxDistort->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pSpinBoxDistortionMax = new QDoubleSpinBox(pGroupBoxDistort);
        pSpinBoxDistortionMax->setObjectName(QStringLiteral("pSpinBoxDistortionMax"));
        pSpinBoxDistortionMax->setGeometry(QRect(240, 20, 71, 21));
        pSpinBoxDistortionMax->setMinimum(0);
        pSpinBoxDistortionMax->setMaximum(100);
        pSpinBoxDistortionMax->setSingleStep(0.1);
        pSpinBoxDistortionMin = new QDoubleSpinBox(pGroupBoxDistort);
        pSpinBoxDistortionMin->setObjectName(QStringLiteral("pSpinBoxDistortionMin"));
        pSpinBoxDistortionMin->setGeometry(QRect(160, 20, 71, 21));
        pSpinBoxDistortionMin->setMinimum(0);
        pSpinBoxDistortionMin->setMaximum(100);
        pSpinBoxDistortionMin->setSingleStep(0.1);
        label_18 = new QLabel(pGroupBoxDistort);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(70, 20, 61, 21));
        label_24 = new QLabel(pGroupBoxDistort);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(10, 20, 51, 21));
        pGroupBoxColor = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor->setObjectName(QStringLiteral("pGroupBoxColor"));
        pGroupBoxColor->setGeometry(QRect(10, 450, 321, 51));
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
        pSpinBoxBlendMode = new QDoubleSpinBox(pGroupBoxColor);
        pSpinBoxBlendMode->setObjectName(QStringLiteral("pSpinBoxBlendMode"));
        pSpinBoxBlendMode->setGeometry(QRect(240, 20, 71, 18));
        pSpinBoxBlendMode->setMinimum(0);
        pSpinBoxBlendMode->setMaximum(1);
        pSpinBoxBlendMode->setSingleStep(0.05);
        label_25 = new QLabel(pGroupBoxColor);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(10, 20, 81, 16));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(ParticleEmitterEntityPanel);

        QMetaObject::connectSlotsByName(ParticleEmitterEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *ParticleEmitterEntityPanel)
    {
        ParticleEmitterEntityPanel->setWindowTitle(QApplication::translate("ParticleEmitterEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("ParticleEmitterEntityPanel", "Particle Emitter", 0)));
        pLabelName->setText(QString());
        groupBox->setTitle(QApplication::translate("ParticleEmitterEntityPanel", "Common", 0));
#ifndef QT_NO_TOOLTIP
        pBtnColorBegin->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "begin", 0));
#endif // QT_NO_TOOLTIP
        pBtnColorBegin->setText(QString());
        label->setText(QApplication::translate("ParticleEmitterEntityPanel", "Material Sequence", 0));
#ifndef QT_NO_TOOLTIP
        pSpinBoxSizeEnd->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "end", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pSpinBoxSizeBegin->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "begin", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pSpinBoxFadeOut->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "out", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pSpinBoxFadeIn->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "in", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pSpinBoxAlphaBegin->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "begin", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pSpinBoxAlphaEnd->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "end", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pBtnColorEnd->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "end", 0));
#endif // QT_NO_TOOLTIP
        pBtnColorEnd->setText(QString());
#ifndef QT_NO_TOOLTIP
        label_2->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "begin/end", 0));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("ParticleEmitterEntityPanel", "Size", 0));
#ifndef QT_NO_TOOLTIP
        label_3->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "[0, 1] Particle size will be adjusted based on a random percentage value of the original size within the specified range ", 0));
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("ParticleEmitterEntityPanel", "Size Range (+/- 0%-100%)", 0));
        label_4->setText(QApplication::translate("ParticleEmitterEntityPanel", "Particle Count", 0));
#ifndef QT_NO_TOOLTIP
        label_5->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "[0, ...) Particle fade in/out time in secs", 0));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("ParticleEmitterEntityPanel", "Fade-time", 0));
#ifndef QT_NO_TOOLTIP
        label_6->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "[0, 1] Begin/end particle alpha multiplier", 0));
#endif // QT_NO_TOOLTIP
        label_6->setText(QApplication::translate("ParticleEmitterEntityPanel", "Alpha", 0));
#ifndef QT_NO_TOOLTIP
        label_11->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "[RGB] Begin/end Particle color multiplier", 0));
#endif // QT_NO_TOOLTIP
        label_11->setText(QApplication::translate("ParticleEmitterEntityPanel", "Color", 0));
#ifndef QT_NO_TOOLTIP
        label_13->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "[0, 360] The angle within particles are emitted", 0));
#endif // QT_NO_TOOLTIP
        label_13->setText(QApplication::translate("ParticleEmitterEntityPanel", "Velocity Angle", 0));
#ifndef QT_NO_TOOLTIP
        pSpinBoxVelocityMax->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "end", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pSpinBoxVelocityMin->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "begin", 0));
#endif // QT_NO_TOOLTIP
        label_14->setText(QApplication::translate("ParticleEmitterEntityPanel", "Velocity", 0));
        label_15->setText(QApplication::translate("ParticleEmitterEntityPanel", "[begin/end]", 0));
#ifndef QT_NO_TOOLTIP
        pSpinBoxLifetimeMax->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "end", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pSpinBoxLifetimeMin->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "begin", 0));
#endif // QT_NO_TOOLTIP
        label_19->setText(QApplication::translate("ParticleEmitterEntityPanel", "Lifetime", 0));
        label_20->setText(QApplication::translate("ParticleEmitterEntityPanel", "Emit Rate", 0));
        label_21->setText(QApplication::translate("ParticleEmitterEntityPanel", "Emit Duration", 0));
        label_22->setText(QApplication::translate("ParticleEmitterEntityPanel", "Emit Delay", 0));
        label_23->setText(QApplication::translate("ParticleEmitterEntityPanel", "Warmup Time", 0));
        pCheckBoxGlobalSpaceParticles->setText(QApplication::translate("ParticleEmitterEntityPanel", "World Space Particles", 0));
#ifndef QT_NO_TOOLTIP
        pCheckBoxEmitAllParticlesOnce->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "Emit the maximum number of particles once and stop", 0));
#endif // QT_NO_TOOLTIP
        pCheckBoxEmitAllParticlesOnce->setText(QApplication::translate("ParticleEmitterEntityPanel", "Emit All Particles Once", 0));
        pCheckBoxStartDisabled->setText(QApplication::translate("ParticleEmitterEntityPanel", "Start Disabled", 0));
        pBtnPickParticleSystem->setText(QApplication::translate("ParticleEmitterEntityPanel", "Pick Particle System", 0));
        groupBox_2->setTitle(QApplication::translate("ParticleEmitterEntityPanel", "Orientation", 0));
        pRadioBtnOrientationAlignToVelocity->setText(QApplication::translate("ParticleEmitterEntityPanel", "Align to Velocity", 0));
        pRadioBtnOrientationRandom->setText(QApplication::translate("ParticleEmitterEntityPanel", "Random", 0));
#ifndef QT_NO_TOOLTIP
        pSpinBoxOrientationVelocityMin->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "min", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pSpinBoxOrientationVelocityMax->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "max", 0));
#endif // QT_NO_TOOLTIP
        label_17->setText(QApplication::translate("ParticleEmitterEntityPanel", "Velocity", 0));
        groupBox_3->setTitle(QApplication::translate("ParticleEmitterEntityPanel", "Facing", 0));
        pRadioBtnFacingEmitter->setText(QApplication::translate("ParticleEmitterEntityPanel", "Emitter", 0));
        pRadioBtnFacingCamera->setText(QApplication::translate("ParticleEmitterEntityPanel", "Camera", 0));
        pRadioBtnFacingHorizontal->setText(QApplication::translate("ParticleEmitterEntityPanel", "Horizontal", 0));
        pRadioBtnFacingScreen->setText(QApplication::translate("ParticleEmitterEntityPanel", "Screen", 0));
        pGroupBoxStaticBoundingBox->setTitle(QApplication::translate("ParticleEmitterEntityPanel", "Static Bounding Box", 0));
        pBtnCaptureBoundingBox->setText(QApplication::translate("ParticleEmitterEntityPanel", "Capture Bounding Box", 0));
        pGroupBoxDistort->setTitle(QApplication::translate("ParticleEmitterEntityPanel", "Distort", 0));
#ifndef QT_NO_TOOLTIP
        pSpinBoxDistortionMax->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "max", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pSpinBoxDistortionMin->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "min", 0));
#endif // QT_NO_TOOLTIP
        label_18->setText(QApplication::translate("ParticleEmitterEntityPanel", "[min/max]", 0));
#ifndef QT_NO_TOOLTIP
        label_24->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "[0, ...) Multiplier for the reflected ray to compute the distorted particle color", 0));
#endif // QT_NO_TOOLTIP
        label_24->setText(QApplication::translate("ParticleEmitterEntityPanel", "Distortion", 0));
        pGroupBoxColor->setTitle(QApplication::translate("ParticleEmitterEntityPanel", "Color", 0));
#ifndef QT_NO_TOOLTIP
        pSpinBoxBlendMode->setToolTip(QApplication::translate("ParticleEmitterEntityPanel", "[0 = solid, 1 = additive] Particles are blended onto the screen based on the specified value", 0));
#endif // QT_NO_TOOLTIP
        label_25->setText(QApplication::translate("ParticleEmitterEntityPanel", "Blend mode", 0));
    } // retranslateUi

};

namespace Ui {
    class ParticleEmitterEntityPanel: public Ui_ParticleEmitterEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PARTICLEEMITTERENTITYPANEL_UI_H
