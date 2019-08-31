/********************************************************************************
** Form generated from reading UI file 'MissileGunAimingEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MISSILEGUNAIMINGENTITYPANEL_UI_H
#define MISSILEGUNAIMINGENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_MissileGunAimingEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxRadius;
    QDoubleSpinBox *pSpinBoxTargetingVelocity;

    void setupUi(QWidget *MissileGunAimingEntityPanel)
    {
        if (MissileGunAimingEntityPanel->objectName().isEmpty())
            MissileGunAimingEntityPanel->setObjectName(QStringLiteral("MissileGunAimingEntityPanel"));
        MissileGunAimingEntityPanel->resize(270, 258);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MissileGunAimingEntityPanel->sizePolicy().hasHeightForWidth());
        MissileGunAimingEntityPanel->setSizePolicy(sizePolicy);
        MissileGunAimingEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(MissileGunAimingEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(MissileGunAimingEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 110));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 50, 81, 16));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 70, 111, 16));
        pSpinBoxRadius = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxRadius->setObjectName(QStringLiteral("pSpinBoxRadius"));
        pSpinBoxRadius->setGeometry(QRect(150, 50, 101, 18));
        pSpinBoxRadius->setMinimum(0);
        pSpinBoxRadius->setMaximum(100000);
        pSpinBoxRadius->setSingleStep(50);
        pSpinBoxTargetingVelocity = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxTargetingVelocity->setObjectName(QStringLiteral("pSpinBoxTargetingVelocity"));
        pSpinBoxTargetingVelocity->setGeometry(QRect(150, 70, 101, 18));
        pSpinBoxTargetingVelocity->setMinimum(0);
        pSpinBoxTargetingVelocity->setMaximum(100000);
        pSpinBoxTargetingVelocity->setSingleStep(0.05);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(MissileGunAimingEntityPanel);

        QMetaObject::connectSlotsByName(MissileGunAimingEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *MissileGunAimingEntityPanel)
    {
        MissileGunAimingEntityPanel->setWindowTitle(QApplication::translate("MissileGunAimingEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("MissileGunAimingEntityPanel", "Missile Gun Aiming ", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("MissileGunAimingEntityPanel", "Radius", 0));
        label_5->setText(QApplication::translate("MissileGunAimingEntityPanel", "Targeting Velocity", 0));
    } // retranslateUi

};

namespace Ui {
    class MissileGunAimingEntityPanel: public Ui_MissileGunAimingEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MISSILEGUNAIMINGENTITYPANEL_UI_H
