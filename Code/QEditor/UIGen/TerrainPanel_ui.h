/********************************************************************************
** Form generated from reading UI file 'TerrainPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TERRAINPANEL_UI_H
#define TERRAINPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_TerrainPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *groupBox;
    QPushButton *pBtnChooseMaterial;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *pLineEditTexture;
    QLineEdit *pLineEditNormalMap;
    QLabel *label_10;
    QLabel *label_11;
    QLineEdit *pLineEditEmissiveMap;
    QLabel *label_12;
    QLineEdit *pLineEditSpecularityMap;
    QGroupBox *groupBox_2;
    QLabel *label_3;
    QDoubleSpinBox *pSpinBoxEmissiveEnergy;
    QLabel *label_4;
    QPushButton *pBtnEmissiveColor;
    QLabel *label_5;
    QLabel *label_6;
    QDoubleSpinBox *pSpinBoxEnvReflectivity;
    QPushButton *pBtnEnvColor;

    void setupUi(QWidget *TerrainPanel)
    {
        if (TerrainPanel->objectName().isEmpty())
            TerrainPanel->setObjectName(QStringLiteral("TerrainPanel"));
        TerrainPanel->resize(340, 926);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TerrainPanel->sizePolicy().hasHeightForWidth());
        TerrainPanel->setSizePolicy(sizePolicy);
        TerrainPanel->setMinimumSize(QSize(0, 600));
        gridLayout = new QGridLayout(TerrainPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(TerrainPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 380));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 200, 321, 171));
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
        pBtnChooseMaterial = new QPushButton(groupBox);
        pBtnChooseMaterial->setObjectName(QStringLiteral("pBtnChooseMaterial"));
        pBtnChooseMaterial->setGeometry(QRect(70, 20, 241, 23));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 51, 21));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 41, 21));
        pLineEditTexture = new QLineEdit(groupBox);
        pLineEditTexture->setObjectName(QStringLiteral("pLineEditTexture"));
        pLineEditTexture->setEnabled(true);
        pLineEditTexture->setGeometry(QRect(70, 50, 241, 20));
        pLineEditTexture->setReadOnly(true);
        pLineEditNormalMap = new QLineEdit(groupBox);
        pLineEditNormalMap->setObjectName(QStringLiteral("pLineEditNormalMap"));
        pLineEditNormalMap->setEnabled(true);
        pLineEditNormalMap->setGeometry(QRect(70, 80, 241, 20));
        pLineEditNormalMap->setReadOnly(true);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 80, 51, 21));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 110, 51, 21));
        pLineEditEmissiveMap = new QLineEdit(groupBox);
        pLineEditEmissiveMap->setObjectName(QStringLiteral("pLineEditEmissiveMap"));
        pLineEditEmissiveMap->setEnabled(true);
        pLineEditEmissiveMap->setGeometry(QRect(70, 110, 241, 20));
        pLineEditEmissiveMap->setReadOnly(true);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 140, 61, 21));
        pLineEditSpecularityMap = new QLineEdit(groupBox);
        pLineEditSpecularityMap->setObjectName(QStringLiteral("pLineEditSpecularityMap"));
        pLineEditSpecularityMap->setEnabled(true);
        pLineEditSpecularityMap->setGeometry(QRect(70, 140, 241, 20));
        pLineEditSpecularityMap->setReadOnly(true);
        groupBox_2 = new QGroupBox(qEditorPanelBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 40, 321, 141));
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
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 50, 91, 16));
        pSpinBoxEmissiveEnergy = new QDoubleSpinBox(groupBox_2);
        pSpinBoxEmissiveEnergy->setObjectName(QStringLiteral("pSpinBoxEmissiveEnergy"));
        pSpinBoxEmissiveEnergy->setGeometry(QRect(210, 50, 101, 18));
        pSpinBoxEmissiveEnergy->setSingleStep(0.01);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 91, 21));
        pBtnEmissiveColor = new QPushButton(groupBox_2);
        pBtnEmissiveColor->setObjectName(QStringLiteral("pBtnEmissiveColor"));
        pBtnEmissiveColor->setGeometry(QRect(210, 20, 101, 23));
        pBtnEmissiveColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 110, 131, 16));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 80, 91, 21));
        pSpinBoxEnvReflectivity = new QDoubleSpinBox(groupBox_2);
        pSpinBoxEnvReflectivity->setObjectName(QStringLiteral("pSpinBoxEnvReflectivity"));
        pSpinBoxEnvReflectivity->setGeometry(QRect(210, 110, 101, 18));
        pSpinBoxEnvReflectivity->setSingleStep(0.01);
        pBtnEnvColor = new QPushButton(groupBox_2);
        pBtnEnvColor->setObjectName(QStringLiteral("pBtnEnvColor"));
        pBtnEnvColor->setGeometry(QRect(210, 80, 101, 23));
        pBtnEnvColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(TerrainPanel);

        QMetaObject::connectSlotsByName(TerrainPanel);
    } // setupUi

    void retranslateUi(QWidget *TerrainPanel)
    {
        TerrainPanel->setWindowTitle(QApplication::translate("TerrainPanel", "TerrainPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("TerrainPanel", "Terrain", 0)));
        pLabelName->setText(QString());
        groupBox->setTitle(QApplication::translate("TerrainPanel", "Material", 0));
        pBtnChooseMaterial->setText(QApplication::translate("TerrainPanel", "Choose Material (*.mtrl)", 0));
        label->setText(QApplication::translate("TerrainPanel", "Material", 0));
        label_2->setText(QApplication::translate("TerrainPanel", "Albedo", 0));
        label_10->setText(QApplication::translate("TerrainPanel", "Normals", 0));
        label_11->setText(QApplication::translate("TerrainPanel", "Emissive", 0));
        label_12->setText(QApplication::translate("TerrainPanel", "Specular", 0));
        groupBox_2->setTitle(QApplication::translate("TerrainPanel", "Material Properties", 0));
        label_3->setText(QApplication::translate("TerrainPanel", "Emissive Energy", 0));
        label_4->setText(QApplication::translate("TerrainPanel", "Emissive Color", 0));
        pBtnEmissiveColor->setText(QString());
        label_5->setText(QApplication::translate("TerrainPanel", "Environment Reflectivity", 0));
        label_6->setText(QApplication::translate("TerrainPanel", "Environment Color", 0));
        pBtnEnvColor->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TerrainPanel: public Ui_TerrainPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TERRAINPANEL_UI_H
