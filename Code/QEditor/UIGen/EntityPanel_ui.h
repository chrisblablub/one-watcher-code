/********************************************************************************
** Form generated from reading UI file 'EntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ENTITYPANEL_UI_H
#define ENTITYPANEL_UI_H

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

class Ui_Form
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QGroupBox *groupBox_2;
    QLabel *label_4;
    QDoubleSpinBox *pSpinBoxPositionX;
    QDoubleSpinBox *pSpinBoxPositionY;
    QDoubleSpinBox *pSpinBoxPositionZ;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QDoubleSpinBox *pSpinBoxYaw;
    QDoubleSpinBox *pSpinBoxPitch;
    QDoubleSpinBox *pSpinBoxRoll;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QPushButton *pBtnResetOrientation;
    QLabel *label_12;
    QDoubleSpinBox *pSpinBoxScalingX;
    QDoubleSpinBox *pSpinBoxScalingZ;
    QDoubleSpinBox *pSpinBoxScalingY;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QPushButton *pBtnLockAxis;
    QPushButton *pBtnResetScaling;
    QGroupBox *groupBox;
    QPushButton *pBtnSetName;
    QLineEdit *pLineEditName;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(340, 544);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Form->sizePolicy().hasHeightForWidth());
        Form->setSizePolicy(sizePolicy);
        Form->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(Form);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(Form);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 390));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        groupBox_2 = new QGroupBox(qEditorPanelBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 100, 321, 281));
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
        label_4->setGeometry(QRect(10, 20, 64, 16));
        pSpinBoxPositionX = new QDoubleSpinBox(groupBox_2);
        pSpinBoxPositionX->setObjectName(QStringLiteral("pSpinBoxPositionX"));
        pSpinBoxPositionX->setGeometry(QRect(210, 20, 101, 18));
        pSpinBoxPositionX->setMinimum(-100000);
        pSpinBoxPositionX->setMaximum(100000);
        pSpinBoxPositionX->setSingleStep(10);
        pSpinBoxPositionY = new QDoubleSpinBox(groupBox_2);
        pSpinBoxPositionY->setObjectName(QStringLiteral("pSpinBoxPositionY"));
        pSpinBoxPositionY->setGeometry(QRect(210, 40, 101, 18));
        pSpinBoxPositionY->setMinimum(-100000);
        pSpinBoxPositionY->setMaximum(100000);
        pSpinBoxPositionY->setSingleStep(10);
        pSpinBoxPositionZ = new QDoubleSpinBox(groupBox_2);
        pSpinBoxPositionZ->setObjectName(QStringLiteral("pSpinBoxPositionZ"));
        pSpinBoxPositionZ->setGeometry(QRect(210, 60, 101, 18));
        pSpinBoxPositionZ->setMinimum(-100000);
        pSpinBoxPositionZ->setMaximum(100000);
        pSpinBoxPositionZ->setSingleStep(10);
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(190, 20, 16, 16));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(190, 40, 16, 16));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(190, 60, 16, 16));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 90, 91, 16));
        pSpinBoxYaw = new QDoubleSpinBox(groupBox_2);
        pSpinBoxYaw->setObjectName(QStringLiteral("pSpinBoxYaw"));
        pSpinBoxYaw->setGeometry(QRect(210, 90, 101, 18));
        pSpinBoxYaw->setMinimum(-360);
        pSpinBoxYaw->setMaximum(360);
        pSpinBoxYaw->setSingleStep(10);
        pSpinBoxPitch = new QDoubleSpinBox(groupBox_2);
        pSpinBoxPitch->setObjectName(QStringLiteral("pSpinBoxPitch"));
        pSpinBoxPitch->setGeometry(QRect(210, 110, 101, 18));
        pSpinBoxPitch->setMinimum(-360);
        pSpinBoxPitch->setMaximum(360);
        pSpinBoxPitch->setSingleStep(10);
        pSpinBoxRoll = new QDoubleSpinBox(groupBox_2);
        pSpinBoxRoll->setObjectName(QStringLiteral("pSpinBoxRoll"));
        pSpinBoxRoll->setGeometry(QRect(210, 130, 101, 18));
        pSpinBoxRoll->setMinimum(-360);
        pSpinBoxRoll->setMaximum(360);
        pSpinBoxRoll->setSingleStep(10);
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(180, 90, 21, 16));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(180, 110, 31, 16));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(180, 130, 21, 16));
        pBtnResetOrientation = new QPushButton(groupBox_2);
        pBtnResetOrientation->setObjectName(QStringLiteral("pBtnResetOrientation"));
        pBtnResetOrientation->setGeometry(QRect(210, 150, 101, 23));
        pBtnResetOrientation->setStyleSheet(QStringLiteral(""));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 190, 91, 16));
        pSpinBoxScalingX = new QDoubleSpinBox(groupBox_2);
        pSpinBoxScalingX->setObjectName(QStringLiteral("pSpinBoxScalingX"));
        pSpinBoxScalingX->setGeometry(QRect(210, 190, 101, 18));
        pSpinBoxScalingX->setMinimum(0.01);
        pSpinBoxScalingX->setSingleStep(0.05);
        pSpinBoxScalingX->setValue(1);
        pSpinBoxScalingZ = new QDoubleSpinBox(groupBox_2);
        pSpinBoxScalingZ->setObjectName(QStringLiteral("pSpinBoxScalingZ"));
        pSpinBoxScalingZ->setGeometry(QRect(210, 230, 101, 18));
        pSpinBoxScalingZ->setMinimum(0.01);
        pSpinBoxScalingZ->setSingleStep(0.05);
        pSpinBoxScalingZ->setValue(1);
        pSpinBoxScalingY = new QDoubleSpinBox(groupBox_2);
        pSpinBoxScalingY->setObjectName(QStringLiteral("pSpinBoxScalingY"));
        pSpinBoxScalingY->setGeometry(QRect(210, 210, 101, 18));
        pSpinBoxScalingY->setMinimum(0.01);
        pSpinBoxScalingY->setSingleStep(0.05);
        pSpinBoxScalingY->setValue(1);
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(150, 190, 16, 16));
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(150, 210, 16, 16));
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(150, 230, 16, 16));
        pBtnLockAxis = new QPushButton(groupBox_2);
        pBtnLockAxis->setObjectName(QStringLiteral("pBtnLockAxis"));
        pBtnLockAxis->setGeometry(QRect(170, 190, 31, 61));
        pBtnLockAxis->setCheckable(true);
        pBtnLockAxis->setChecked(true);
        pBtnResetScaling = new QPushButton(groupBox_2);
        pBtnResetScaling->setObjectName(QStringLiteral("pBtnResetScaling"));
        pBtnResetScaling->setGeometry(QRect(210, 250, 101, 23));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 40, 321, 51));
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
        pBtnSetName = new QPushButton(groupBox);
        pBtnSetName->setObjectName(QStringLiteral("pBtnSetName"));
        pBtnSetName->setGeometry(QRect(240, 20, 71, 21));
        pLineEditName = new QLineEdit(groupBox);
        pLineEditName->setObjectName(QStringLiteral("pLineEditName"));
        pLineEditName->setGeometry(QRect(10, 20, 221, 20));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("Form", "Entity", 0)));
        groupBox_2->setTitle(QApplication::translate("Form", "Node Properties", 0));
        label_4->setText(QApplication::translate("Form", "Position", 0));
        label_5->setText(QApplication::translate("Form", "X", 0));
        label_6->setText(QApplication::translate("Form", "Y", 0));
        label_7->setText(QApplication::translate("Form", "Z", 0));
        label_8->setText(QApplication::translate("Form", "Orientation (deg)", 0));
        label_9->setText(QApplication::translate("Form", "Yaw", 0));
        label_10->setText(QApplication::translate("Form", "Pitch", 0));
        label_11->setText(QApplication::translate("Form", "Roll", 0));
        pBtnResetOrientation->setText(QApplication::translate("Form", "Reset Orientation", 0));
        label_12->setText(QApplication::translate("Form", "Scaling", 0));
        label_13->setText(QApplication::translate("Form", "X", 0));
        label_14->setText(QApplication::translate("Form", "Y", 0));
        label_15->setText(QApplication::translate("Form", "Z", 0));
        pBtnLockAxis->setText(QApplication::translate("Form", "Lock\n"
"Axis", 0));
        pBtnResetScaling->setText(QApplication::translate("Form", "Reset Scaling", 0));
        groupBox->setTitle(QApplication::translate("Form", "Name", 0));
        pBtnSetName->setText(QApplication::translate("Form", "Set Name", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ENTITYPANEL_UI_H
