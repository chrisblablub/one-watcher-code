/********************************************************************************
** Form generated from reading UI file 'MissileGunEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MISSILEGUNENTITYPANEL_UI_H
#define MISSILEGUNENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_MissileGunEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label;
    QDoubleSpinBox *pSpinBoxFireDelayMax;
    QDoubleSpinBox *pSpinBoxFireDelayMin;
    QDoubleSpinBox *pSpinBoxLifetimeMax;
    QDoubleSpinBox *pSpinBoxLifetimeMin;
    QDoubleSpinBox *pSpinBoxVelocityMin;
    QDoubleSpinBox *pSpinBoxVelocityMax;
    QDoubleSpinBox *pSpinBoxTargetingVelocity;
    QDoubleSpinBox *pSpinBoxRadius;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QCheckBox *pCheckBoxVisibilityChecking;
    QCheckBox *pCheckBoxMissileTargeting;

    void setupUi(QWidget *MissileGunEntityPanel)
    {
        if (MissileGunEntityPanel->objectName().isEmpty())
            MissileGunEntityPanel->setObjectName(QStringLiteral("MissileGunEntityPanel"));
        MissileGunEntityPanel->resize(270, 497);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MissileGunEntityPanel->sizePolicy().hasHeightForWidth());
        MissileGunEntityPanel->setSizePolicy(sizePolicy);
        MissileGunEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(MissileGunEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(MissileGunEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 210));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label = new QLabel(qEditorPanelBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 50, 81, 16));
        pSpinBoxFireDelayMax = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxFireDelayMax->setObjectName(QStringLiteral("pSpinBoxFireDelayMax"));
        pSpinBoxFireDelayMax->setGeometry(QRect(190, 50, 61, 18));
        pSpinBoxFireDelayMax->setMinimum(0);
        pSpinBoxFireDelayMax->setMaximum(100000);
        pSpinBoxFireDelayMax->setSingleStep(0.05);
        pSpinBoxFireDelayMin = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxFireDelayMin->setObjectName(QStringLiteral("pSpinBoxFireDelayMin"));
        pSpinBoxFireDelayMin->setGeometry(QRect(120, 50, 61, 18));
        pSpinBoxFireDelayMin->setMinimum(0);
        pSpinBoxFireDelayMin->setSingleStep(0.05);
        pSpinBoxFireDelayMin->setValue(1);
        pSpinBoxLifetimeMax = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxLifetimeMax->setObjectName(QStringLiteral("pSpinBoxLifetimeMax"));
        pSpinBoxLifetimeMax->setGeometry(QRect(190, 70, 61, 18));
        pSpinBoxLifetimeMax->setMinimum(0);
        pSpinBoxLifetimeMax->setSingleStep(0.05);
        pSpinBoxLifetimeMax->setValue(1);
        pSpinBoxLifetimeMin = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxLifetimeMin->setObjectName(QStringLiteral("pSpinBoxLifetimeMin"));
        pSpinBoxLifetimeMin->setGeometry(QRect(120, 70, 61, 18));
        pSpinBoxLifetimeMin->setMinimum(0);
        pSpinBoxLifetimeMin->setSingleStep(0.05);
        pSpinBoxLifetimeMin->setValue(1);
        pSpinBoxVelocityMin = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxVelocityMin->setObjectName(QStringLiteral("pSpinBoxVelocityMin"));
        pSpinBoxVelocityMin->setGeometry(QRect(120, 90, 61, 18));
        pSpinBoxVelocityMin->setMinimum(0.01);
        pSpinBoxVelocityMin->setMaximum(100000);
        pSpinBoxVelocityMin->setSingleStep(25);
        pSpinBoxVelocityMin->setValue(1);
        pSpinBoxVelocityMax = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxVelocityMax->setObjectName(QStringLiteral("pSpinBoxVelocityMax"));
        pSpinBoxVelocityMax->setGeometry(QRect(190, 90, 61, 18));
        pSpinBoxVelocityMax->setMinimum(0);
        pSpinBoxVelocityMax->setMaximum(100000);
        pSpinBoxVelocityMax->setSingleStep(25);
        pSpinBoxVelocityMax->setValue(1);
        pSpinBoxTargetingVelocity = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxTargetingVelocity->setObjectName(QStringLiteral("pSpinBoxTargetingVelocity"));
        pSpinBoxTargetingVelocity->setGeometry(QRect(190, 130, 61, 18));
        pSpinBoxTargetingVelocity->setMinimum(0);
        pSpinBoxTargetingVelocity->setMaximum(100000);
        pSpinBoxTargetingVelocity->setSingleStep(0.05);
        pSpinBoxTargetingVelocity->setValue(1);
        pSpinBoxRadius = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxRadius->setObjectName(QStringLiteral("pSpinBoxRadius"));
        pSpinBoxRadius->setGeometry(QRect(190, 110, 61, 18));
        pSpinBoxRadius->setMinimum(0);
        pSpinBoxRadius->setMaximum(100000);
        pSpinBoxRadius->setSingleStep(25);
        pSpinBoxRadius->setValue(1);
        label_2 = new QLabel(qEditorPanelBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 70, 47, 13));
        label_3 = new QLabel(qEditorPanelBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 90, 47, 13));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 110, 47, 13));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 130, 141, 16));
        pCheckBoxVisibilityChecking = new QCheckBox(qEditorPanelBox);
        pCheckBoxVisibilityChecking->setObjectName(QStringLiteral("pCheckBoxVisibilityChecking"));
        pCheckBoxVisibilityChecking->setGeometry(QRect(129, 150, 121, 20));
        pCheckBoxMissileTargeting = new QCheckBox(qEditorPanelBox);
        pCheckBoxMissileTargeting->setObjectName(QStringLiteral("pCheckBoxMissileTargeting"));
        pCheckBoxMissileTargeting->setGeometry(QRect(129, 170, 121, 20));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(MissileGunEntityPanel);

        QMetaObject::connectSlotsByName(MissileGunEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *MissileGunEntityPanel)
    {
        MissileGunEntityPanel->setWindowTitle(QApplication::translate("MissileGunEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("MissileGunEntityPanel", "Missile Gun", 0)));
        pLabelName->setText(QString());
        label->setText(QApplication::translate("MissileGunEntityPanel", "Fire Delay", 0));
        label_2->setText(QApplication::translate("MissileGunEntityPanel", "Lifetime", 0));
        label_3->setText(QApplication::translate("MissileGunEntityPanel", "Velocity", 0));
        label_4->setText(QApplication::translate("MissileGunEntityPanel", "Radius", 0));
        label_5->setText(QApplication::translate("MissileGunEntityPanel", "Missile Targeting Velocity", 0));
        pCheckBoxVisibilityChecking->setText(QApplication::translate("MissileGunEntityPanel", "Visibility Checking", 0));
        pCheckBoxMissileTargeting->setText(QApplication::translate("MissileGunEntityPanel", "Missile Targeting", 0));
    } // retranslateUi

};

namespace Ui {
    class MissileGunEntityPanel: public Ui_MissileGunEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MISSILEGUNENTITYPANEL_UI_H
