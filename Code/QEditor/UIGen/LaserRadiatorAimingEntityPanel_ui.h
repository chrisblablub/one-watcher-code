/********************************************************************************
** Form generated from reading UI file 'LaserRadiatorAimingEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LASERRADIATORAIMINGENTITYPANEL_UI_H
#define LASERRADIATORAIMINGENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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

class Ui_LaserRadiatorAimingEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *label_4;
    QComboBox *pComboAttachEntity;
    QGroupBox *groupBox;
    QLabel *label_5;
    QComboBox *pComboEditAttachee;
    QPushButton *pBtnDetachEntity;
    QPushButton *pBtnAttachEntity;
    QDoubleSpinBox *pSpinBoxRadius;
    QLabel *label_6;
    QDoubleSpinBox *pSpinBoxGrowDuration;
    QDoubleSpinBox *pSpinBoxShrinkDuration;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QDoubleSpinBox *pSpinBoxOffset;

    void setupUi(QWidget *LaserRadiatorAimingEntityPanel)
    {
        if (LaserRadiatorAimingEntityPanel->objectName().isEmpty())
            LaserRadiatorAimingEntityPanel->setObjectName(QStringLiteral("LaserRadiatorAimingEntityPanel"));
        LaserRadiatorAimingEntityPanel->resize(270, 508);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LaserRadiatorAimingEntityPanel->sizePolicy().hasHeightForWidth());
        LaserRadiatorAimingEntityPanel->setSizePolicy(sizePolicy);
        LaserRadiatorAimingEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(LaserRadiatorAimingEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(LaserRadiatorAimingEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 340));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 170, 71, 16));
        pComboAttachEntity = new QComboBox(qEditorPanelBox);
        pComboAttachEntity->setObjectName(QStringLiteral("pComboAttachEntity"));
        pComboAttachEntity->setGeometry(QRect(90, 170, 161, 22));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 230, 251, 91));
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
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 20, 41, 16));
        pComboEditAttachee = new QComboBox(groupBox);
        pComboEditAttachee->setObjectName(QStringLiteral("pComboEditAttachee"));
        pComboEditAttachee->setGeometry(QRect(80, 20, 161, 22));
        pBtnDetachEntity = new QPushButton(groupBox);
        pBtnDetachEntity->setObjectName(QStringLiteral("pBtnDetachEntity"));
        pBtnDetachEntity->setGeometry(QRect(80, 50, 161, 23));
        pBtnDetachEntity->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:center;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pBtnAttachEntity = new QPushButton(qEditorPanelBox);
        pBtnAttachEntity->setObjectName(QStringLiteral("pBtnAttachEntity"));
        pBtnAttachEntity->setGeometry(QRect(90, 200, 161, 23));
        pBtnAttachEntity->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:center;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pSpinBoxRadius = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxRadius->setObjectName(QStringLiteral("pSpinBoxRadius"));
        pSpinBoxRadius->setGeometry(QRect(141, 50, 111, 22));
        pSpinBoxRadius->setMaximum(1e+6);
        pSpinBoxRadius->setSingleStep(50);
        label_6 = new QLabel(qEditorPanelBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 50, 71, 16));
        pSpinBoxGrowDuration = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxGrowDuration->setObjectName(QStringLiteral("pSpinBoxGrowDuration"));
        pSpinBoxGrowDuration->setGeometry(QRect(140, 110, 111, 22));
        pSpinBoxGrowDuration->setMaximum(1e+6);
        pSpinBoxGrowDuration->setSingleStep(50);
        pSpinBoxShrinkDuration = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxShrinkDuration->setObjectName(QStringLiteral("pSpinBoxShrinkDuration"));
        pSpinBoxShrinkDuration->setGeometry(QRect(140, 140, 111, 22));
        pSpinBoxShrinkDuration->setMaximum(1e+6);
        pSpinBoxShrinkDuration->setSingleStep(50);
        label_7 = new QLabel(qEditorPanelBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 110, 91, 16));
        label_8 = new QLabel(qEditorPanelBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 140, 91, 16));
        label_9 = new QLabel(qEditorPanelBox);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(19, 80, 71, 16));
        pSpinBoxOffset = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxOffset->setObjectName(QStringLiteral("pSpinBoxOffset"));
        pSpinBoxOffset->setGeometry(QRect(140, 80, 111, 22));
        pSpinBoxOffset->setMaximum(1e+6);
        pSpinBoxOffset->setSingleStep(50);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(LaserRadiatorAimingEntityPanel);

        QMetaObject::connectSlotsByName(LaserRadiatorAimingEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *LaserRadiatorAimingEntityPanel)
    {
        LaserRadiatorAimingEntityPanel->setWindowTitle(QApplication::translate("LaserRadiatorAimingEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("LaserRadiatorAimingEntityPanel", "Laser Radiator Aiming", 0)));
        label_4->setText(QApplication::translate("LaserRadiatorAimingEntityPanel", "Attach Entity", 0));
        groupBox->setTitle(QApplication::translate("LaserRadiatorAimingEntityPanel", "Edit Attachee", 0));
        label_5->setText(QApplication::translate("LaserRadiatorAimingEntityPanel", "Entity", 0));
        pBtnDetachEntity->setText(QApplication::translate("LaserRadiatorAimingEntityPanel", "Detach Entity", 0));
        pBtnAttachEntity->setText(QApplication::translate("LaserRadiatorAimingEntityPanel", "Attach Entity", 0));
        label_6->setText(QApplication::translate("LaserRadiatorAimingEntityPanel", "Radius", 0));
        label_7->setText(QApplication::translate("LaserRadiatorAimingEntityPanel", "Grow Duration", 0));
        label_8->setText(QApplication::translate("LaserRadiatorAimingEntityPanel", "Shrink Duration", 0));
        label_9->setText(QApplication::translate("LaserRadiatorAimingEntityPanel", "Offset", 0));
    } // retranslateUi

};

namespace Ui {
    class LaserRadiatorAimingEntityPanel: public Ui_LaserRadiatorAimingEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LASERRADIATORAIMINGENTITYPANEL_UI_H
