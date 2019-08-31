/********************************************************************************
** Form generated from reading UI file 'MeshEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MESHENTITYPANEL_UI_H
#define MESHENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_MeshEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *pGroupBoxMaterial;
    QLabel *label_2;
    QDoubleSpinBox *pSpinBoxEmissiveEnergy;
    QDoubleSpinBox *pSpinBoxDiffuseEnergy;
    QLabel *label_3;
    QLabel *label_8;
    QLabel *label_10;
    QDoubleSpinBox *pSpinBoxEnvReflectivity;
    QLabel *label_14;
    QLabel *label_15;
    QCheckBox *pCheckBoxCategorySolid;
    QCheckBox *pCheckBoxRenderInEditor;
    QCheckBox *pCheckBoxRenderInGame;
    QCheckBox *pCheckBoxDirectionalLighting;
    QLabel *label;
    QLabel *label_9;
    QLabel *label_13;
    QCheckBox *pCheckBoxCategoryOutline;
    QCheckBox *pCheckBoxCategoryTransmissive;
    QGroupBox *pCheckBoxMotionBlur;
    QLabel *label_12;
    QDoubleSpinBox *pSpinBoxMotionBlurIntensity;
    QPushButton *pBtnDiffuseColor;
    QPushButton *pBtnEmissiveColor;
    QPushButton *pBtnEnvColor;
    QPushButton *pBtnCopyColors;
    QPushButton *pBtnPasteColors;
    QGroupBox *pGroupBoxColor_3;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_4;
    QDoubleSpinBox *pSpinBoxRotationSpeedZ;
    QLabel *label_6;
    QDoubleSpinBox *pSpinBoxRotationSpeedX;
    QDoubleSpinBox *pSpinBoxRotationSpeedY;
    QGroupBox *pGroupBoxColor_4;
    QGroupBox *pGroupBoxColor_7;
    QCheckBox *pCheckBoxMeshTemplate;
    QPushButton *pBtnLoadMeshTemplate;
    QPushButton *pBtnSaveMeshTemplate;
    QPushButton *pBtnApplyTemplateToAll;
    QPushButton *pBtnChooseMesh;
    QPushButton *pBtnReplaceMeshes;
    QGroupBox *pGroupBoxColor_5;
    QComboBox *pComboMeshInstance;

    void setupUi(QWidget *MeshEntityPanel)
    {
        if (MeshEntityPanel->objectName().isEmpty())
            MeshEntityPanel->setObjectName(QStringLiteral("MeshEntityPanel"));
        MeshEntityPanel->resize(340, 917);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MeshEntityPanel->sizePolicy().hasHeightForWidth());
        MeshEntityPanel->setSizePolicy(sizePolicy);
        MeshEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(MeshEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(MeshEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 870));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        pGroupBoxMaterial = new QGroupBox(qEditorPanelBox);
        pGroupBoxMaterial->setObjectName(QStringLiteral("pGroupBoxMaterial"));
        pGroupBoxMaterial->setGeometry(QRect(20, 390, 301, 441));
        pGroupBoxMaterial->setStyleSheet(QLatin1String("QGroupBox\n"
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
        label_2 = new QLabel(pGroupBoxMaterial);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 250, 91, 20));
        pSpinBoxEmissiveEnergy = new QDoubleSpinBox(pGroupBoxMaterial);
        pSpinBoxEmissiveEnergy->setObjectName(QStringLiteral("pSpinBoxEmissiveEnergy"));
        pSpinBoxEmissiveEnergy->setGeometry(QRect(190, 309, 101, 21));
        pSpinBoxEmissiveEnergy->setMinimum(0);
        pSpinBoxEmissiveEnergy->setMaximum(1000);
        pSpinBoxEmissiveEnergy->setSingleStep(0.1);
        pSpinBoxDiffuseEnergy = new QDoubleSpinBox(pGroupBoxMaterial);
        pSpinBoxDiffuseEnergy->setObjectName(QStringLiteral("pSpinBoxDiffuseEnergy"));
        pSpinBoxDiffuseEnergy->setGeometry(QRect(190, 249, 101, 21));
        pSpinBoxDiffuseEnergy->setMinimum(0);
        pSpinBoxDiffuseEnergy->setMaximum(10000);
        pSpinBoxDiffuseEnergy->setSingleStep(0.1);
        label_3 = new QLabel(pGroupBoxMaterial);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 280, 81, 20));
        label_8 = new QLabel(pGroupBoxMaterial);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 220, 71, 20));
        label_10 = new QLabel(pGroupBoxMaterial);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 309, 91, 16));
        pSpinBoxEnvReflectivity = new QDoubleSpinBox(pGroupBoxMaterial);
        pSpinBoxEnvReflectivity->setObjectName(QStringLiteral("pSpinBoxEnvReflectivity"));
        pSpinBoxEnvReflectivity->setGeometry(QRect(190, 370, 101, 21));
        pSpinBoxEnvReflectivity->setMinimum(0);
        pSpinBoxEnvReflectivity->setMaximum(1000);
        pSpinBoxEnvReflectivity->setSingleStep(0.1);
        label_14 = new QLabel(pGroupBoxMaterial);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(10, 341, 111, 20));
        label_15 = new QLabel(pGroupBoxMaterial);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(10, 370, 121, 16));
        pCheckBoxCategorySolid = new QCheckBox(pGroupBoxMaterial);
        pCheckBoxCategorySolid->setObjectName(QStringLiteral("pCheckBoxCategorySolid"));
        pCheckBoxCategorySolid->setGeometry(QRect(180, 70, 101, 17));
        pCheckBoxRenderInEditor = new QCheckBox(pGroupBoxMaterial);
        pCheckBoxRenderInEditor->setObjectName(QStringLiteral("pCheckBoxRenderInEditor"));
        pCheckBoxRenderInEditor->setGeometry(QRect(180, 40, 131, 16));
        pCheckBoxRenderInGame = new QCheckBox(pGroupBoxMaterial);
        pCheckBoxRenderInGame->setObjectName(QStringLiteral("pCheckBoxRenderInGame"));
        pCheckBoxRenderInGame->setGeometry(QRect(180, 20, 131, 16));
        pCheckBoxDirectionalLighting = new QCheckBox(pGroupBoxMaterial);
        pCheckBoxDirectionalLighting->setObjectName(QStringLiteral("pCheckBoxDirectionalLighting"));
        pCheckBoxDirectionalLighting->setGeometry(QRect(180, 140, 131, 16));
        label = new QLabel(pGroupBoxMaterial);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 81, 16));
        label_9 = new QLabel(pGroupBoxMaterial);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 140, 81, 16));
        label_13 = new QLabel(pGroupBoxMaterial);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 70, 91, 16));
        pCheckBoxCategoryOutline = new QCheckBox(pGroupBoxMaterial);
        pCheckBoxCategoryOutline->setObjectName(QStringLiteral("pCheckBoxCategoryOutline"));
        pCheckBoxCategoryOutline->setEnabled(false);
        pCheckBoxCategoryOutline->setGeometry(QRect(180, 110, 70, 17));
        pCheckBoxCategoryTransmissive = new QCheckBox(pGroupBoxMaterial);
        pCheckBoxCategoryTransmissive->setObjectName(QStringLiteral("pCheckBoxCategoryTransmissive"));
        pCheckBoxCategoryTransmissive->setGeometry(QRect(180, 90, 101, 17));
        pCheckBoxMotionBlur = new QGroupBox(pGroupBoxMaterial);
        pCheckBoxMotionBlur->setObjectName(QStringLiteral("pCheckBoxMotionBlur"));
        pCheckBoxMotionBlur->setGeometry(QRect(0, 160, 301, 51));
        pCheckBoxMotionBlur->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pCheckBoxMotionBlur->setCheckable(true);
        pCheckBoxMotionBlur->setChecked(false);
        label_12 = new QLabel(pCheckBoxMotionBlur);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 20, 47, 16));
        pSpinBoxMotionBlurIntensity = new QDoubleSpinBox(pCheckBoxMotionBlur);
        pSpinBoxMotionBlurIntensity->setObjectName(QStringLiteral("pSpinBoxMotionBlurIntensity"));
        pSpinBoxMotionBlurIntensity->setGeometry(QRect(190, 20, 101, 18));
        pSpinBoxMotionBlurIntensity->setMinimum(0);
        pSpinBoxMotionBlurIntensity->setMaximum(100);
        pSpinBoxMotionBlurIntensity->setSingleStep(0.01);
        pSpinBoxMotionBlurIntensity->setValue(1);
        pBtnDiffuseColor = new QPushButton(pGroupBoxMaterial);
        pBtnDiffuseColor->setObjectName(QStringLiteral("pBtnDiffuseColor"));
        pBtnDiffuseColor->setGeometry(QRect(190, 220, 101, 23));
        pBtnEmissiveColor = new QPushButton(pGroupBoxMaterial);
        pBtnEmissiveColor->setObjectName(QStringLiteral("pBtnEmissiveColor"));
        pBtnEmissiveColor->setGeometry(QRect(190, 280, 101, 23));
        pBtnEnvColor = new QPushButton(pGroupBoxMaterial);
        pBtnEnvColor->setObjectName(QStringLiteral("pBtnEnvColor"));
        pBtnEnvColor->setGeometry(QRect(190, 340, 101, 23));
        pBtnCopyColors = new QPushButton(pGroupBoxMaterial);
        pBtnCopyColors->setObjectName(QStringLiteral("pBtnCopyColors"));
        pBtnCopyColors->setGeometry(QRect(10, 400, 131, 23));
        pBtnPasteColors = new QPushButton(pGroupBoxMaterial);
        pBtnPasteColors->setObjectName(QStringLiteral("pBtnPasteColors"));
        pBtnPasteColors->setGeometry(QRect(160, 400, 131, 23));
        pGroupBoxColor_3 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_3->setObjectName(QStringLiteral("pGroupBoxColor_3"));
        pGroupBoxColor_3->setGeometry(QRect(20, 290, 301, 91));
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
        label_5 = new QLabel(pGroupBoxColor_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(170, 20, 16, 16));
        label_7 = new QLabel(pGroupBoxColor_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(170, 60, 16, 16));
        label_4 = new QLabel(pGroupBoxColor_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 81, 16));
        pSpinBoxRotationSpeedZ = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxRotationSpeedZ->setObjectName(QStringLiteral("pSpinBoxRotationSpeedZ"));
        pSpinBoxRotationSpeedZ->setGeometry(QRect(190, 60, 101, 18));
        pSpinBoxRotationSpeedZ->setMinimum(-100000);
        pSpinBoxRotationSpeedZ->setMaximum(100000);
        pSpinBoxRotationSpeedZ->setSingleStep(0.05);
        label_6 = new QLabel(pGroupBoxColor_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(170, 40, 16, 16));
        pSpinBoxRotationSpeedX = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxRotationSpeedX->setObjectName(QStringLiteral("pSpinBoxRotationSpeedX"));
        pSpinBoxRotationSpeedX->setGeometry(QRect(190, 20, 101, 18));
        pSpinBoxRotationSpeedX->setMinimum(-100000);
        pSpinBoxRotationSpeedX->setMaximum(100000);
        pSpinBoxRotationSpeedX->setSingleStep(0.05);
        pSpinBoxRotationSpeedY = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxRotationSpeedY->setObjectName(QStringLiteral("pSpinBoxRotationSpeedY"));
        pSpinBoxRotationSpeedY->setGeometry(QRect(190, 40, 101, 18));
        pSpinBoxRotationSpeedY->setMinimum(-100000);
        pSpinBoxRotationSpeedY->setMaximum(100000);
        pSpinBoxRotationSpeedY->setSingleStep(0.05);
        pGroupBoxColor_4 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_4->setObjectName(QStringLiteral("pGroupBoxColor_4"));
        pGroupBoxColor_4->setGeometry(QRect(10, 40, 321, 191));
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
        pGroupBoxColor_7 = new QGroupBox(pGroupBoxColor_4);
        pGroupBoxColor_7->setObjectName(QStringLiteral("pGroupBoxColor_7"));
        pGroupBoxColor_7->setGeometry(QRect(0, 80, 321, 111));
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
        pCheckBoxMeshTemplate = new QCheckBox(pGroupBoxColor_7);
        pCheckBoxMeshTemplate->setObjectName(QStringLiteral("pCheckBoxMeshTemplate"));
        pCheckBoxMeshTemplate->setGeometry(QRect(140, 80, 171, 20));
        pBtnLoadMeshTemplate = new QPushButton(pGroupBoxColor_7);
        pBtnLoadMeshTemplate->setObjectName(QStringLiteral("pBtnLoadMeshTemplate"));
        pBtnLoadMeshTemplate->setGeometry(QRect(10, 50, 301, 23));
        pBtnSaveMeshTemplate = new QPushButton(pGroupBoxColor_7);
        pBtnSaveMeshTemplate->setObjectName(QStringLiteral("pBtnSaveMeshTemplate"));
        pBtnSaveMeshTemplate->setGeometry(QRect(10, 20, 301, 23));
        pBtnApplyTemplateToAll = new QPushButton(pGroupBoxColor_7);
        pBtnApplyTemplateToAll->setObjectName(QStringLiteral("pBtnApplyTemplateToAll"));
        pBtnApplyTemplateToAll->setGeometry(QRect(10, 80, 121, 23));
        pBtnChooseMesh = new QPushButton(pGroupBoxColor_4);
        pBtnChooseMesh->setObjectName(QStringLiteral("pBtnChooseMesh"));
        pBtnChooseMesh->setGeometry(QRect(10, 20, 301, 23));
        pBtnReplaceMeshes = new QPushButton(pGroupBoxColor_4);
        pBtnReplaceMeshes->setObjectName(QStringLiteral("pBtnReplaceMeshes"));
        pBtnReplaceMeshes->setGeometry(QRect(10, 50, 301, 23));
        pGroupBoxColor_5 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_5->setObjectName(QStringLiteral("pGroupBoxColor_5"));
        pGroupBoxColor_5->setGeometry(QRect(10, 240, 321, 611));
        pGroupBoxColor_5->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pComboMeshInstance = new QComboBox(pGroupBoxColor_5);
        pComboMeshInstance->setObjectName(QStringLiteral("pComboMeshInstance"));
        pComboMeshInstance->setGeometry(QRect(10, 20, 301, 22));
        pGroupBoxColor_5->raise();
        pGroupBoxMaterial->raise();
        pLabelName->raise();
        pGroupBoxColor_3->raise();
        pGroupBoxColor_4->raise();

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(MeshEntityPanel);

        QMetaObject::connectSlotsByName(MeshEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *MeshEntityPanel)
    {
        MeshEntityPanel->setWindowTitle(QApplication::translate("MeshEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("MeshEntityPanel", "Mesh", 0)));
        pLabelName->setText(QString());
        pGroupBoxMaterial->setTitle(QApplication::translate("MeshEntityPanel", "Material", 0));
        label_2->setText(QApplication::translate("MeshEntityPanel", "Diffuse Energy", 0));
        label_3->setText(QApplication::translate("MeshEntityPanel", "Emissive Color", 0));
        label_8->setText(QApplication::translate("MeshEntityPanel", "Diffuse Color", 0));
        label_10->setText(QApplication::translate("MeshEntityPanel", "Emissive Energy", 0));
        label_14->setText(QApplication::translate("MeshEntityPanel", "Environment Color", 0));
        label_15->setText(QApplication::translate("MeshEntityPanel", "Environment Reflectivity", 0));
        pCheckBoxCategorySolid->setText(QApplication::translate("MeshEntityPanel", "Solid", 0));
        pCheckBoxRenderInEditor->setText(QApplication::translate("MeshEntityPanel", "Render in Editor", 0));
        pCheckBoxRenderInGame->setText(QApplication::translate("MeshEntityPanel", "Render in Game", 0));
        pCheckBoxDirectionalLighting->setText(QApplication::translate("MeshEntityPanel", "Directional Lighting", 0));
        label->setText(QApplication::translate("MeshEntityPanel", "Visibility", 0));
        label_9->setText(QApplication::translate("MeshEntityPanel", "Lighting", 0));
        label_13->setText(QApplication::translate("MeshEntityPanel", "Category", 0));
        pCheckBoxCategoryOutline->setText(QApplication::translate("MeshEntityPanel", "Outline", 0));
        pCheckBoxCategoryTransmissive->setText(QApplication::translate("MeshEntityPanel", "Transmissive", 0));
        pCheckBoxMotionBlur->setTitle(QApplication::translate("MeshEntityPanel", "Motion blur", 0));
        label_12->setText(QApplication::translate("MeshEntityPanel", "Scaling", 0));
        pBtnDiffuseColor->setText(QString());
        pBtnEmissiveColor->setText(QString());
        pBtnEnvColor->setText(QString());
        pBtnCopyColors->setText(QApplication::translate("MeshEntityPanel", "Copy material", 0));
        pBtnPasteColors->setText(QApplication::translate("MeshEntityPanel", "Paste material", 0));
        pGroupBoxColor_3->setTitle(QApplication::translate("MeshEntityPanel", "Transform", 0));
        label_5->setText(QApplication::translate("MeshEntityPanel", "X", 0));
        label_7->setText(QApplication::translate("MeshEntityPanel", "Z", 0));
        label_4->setText(QApplication::translate("MeshEntityPanel", "Rotation Speed", 0));
        label_6->setText(QApplication::translate("MeshEntityPanel", "Y", 0));
        pGroupBoxColor_4->setTitle(QApplication::translate("MeshEntityPanel", "Mesh Container", 0));
        pGroupBoxColor_7->setTitle(QApplication::translate("MeshEntityPanel", "Mesh Container Template", 0));
        pCheckBoxMeshTemplate->setText(QApplication::translate("MeshEntityPanel", "Init from template after loading", 0));
        pBtnLoadMeshTemplate->setText(QApplication::translate("MeshEntityPanel", "Load mesh template", 0));
        pBtnSaveMeshTemplate->setText(QApplication::translate("MeshEntityPanel", "Save mesh template", 0));
        pBtnApplyTemplateToAll->setText(QApplication::translate("MeshEntityPanel", "Apply to all", 0));
        pBtnChooseMesh->setText(QApplication::translate("MeshEntityPanel", "Choose mesh", 0));
        pBtnReplaceMeshes->setText(QApplication::translate("MeshEntityPanel", "Replace meshes in all entities", 0));
        pGroupBoxColor_5->setTitle(QApplication::translate("MeshEntityPanel", "Mesh instance", 0));
    } // retranslateUi

};

namespace Ui {
    class MeshEntityPanel: public Ui_MeshEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MESHENTITYPANEL_UI_H
