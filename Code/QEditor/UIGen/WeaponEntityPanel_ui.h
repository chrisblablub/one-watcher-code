/********************************************************************************
** Form generated from reading UI file 'WeaponEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WEAPONENTITYPANEL_UI_H
#define WEAPONENTITYPANEL_UI_H

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

class Ui_WeaponEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QDoubleSpinBox *pSpinBoxRadius;
    QLabel *label_6;
    QDoubleSpinBox *pSpinBoxBossDuration;
    QLabel *label_7;
    QLabel *label_8;
    QDoubleSpinBox *pSpinBoxChargingDuration;

    void setupUi(QWidget *WeaponEntityPanel)
    {
        if (WeaponEntityPanel->objectName().isEmpty())
            WeaponEntityPanel->setObjectName(QStringLiteral("WeaponEntityPanel"));
        WeaponEntityPanel->resize(270, 508);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(WeaponEntityPanel->sizePolicy().hasHeightForWidth());
        WeaponEntityPanel->setSizePolicy(sizePolicy);
        WeaponEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(WeaponEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(WeaponEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 160));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pSpinBoxRadius = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxRadius->setObjectName(QStringLiteral("pSpinBoxRadius"));
        pSpinBoxRadius->setGeometry(QRect(141, 50, 111, 22));
        pSpinBoxRadius->setMaximum(1e+6);
        pSpinBoxRadius->setSingleStep(50);
        label_6 = new QLabel(qEditorPanelBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 50, 71, 16));
        pSpinBoxBossDuration = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxBossDuration->setObjectName(QStringLiteral("pSpinBoxBossDuration"));
        pSpinBoxBossDuration->setGeometry(QRect(140, 80, 111, 22));
        pSpinBoxBossDuration->setMaximum(1e+6);
        pSpinBoxBossDuration->setSingleStep(50);
        label_7 = new QLabel(qEditorPanelBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 80, 111, 16));
        label_8 = new QLabel(qEditorPanelBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 110, 111, 16));
        pSpinBoxChargingDuration = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxChargingDuration->setObjectName(QStringLiteral("pSpinBoxChargingDuration"));
        pSpinBoxChargingDuration->setGeometry(QRect(140, 110, 111, 22));
        pSpinBoxChargingDuration->setMaximum(1e+6);
        pSpinBoxChargingDuration->setSingleStep(50);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(WeaponEntityPanel);

        QMetaObject::connectSlotsByName(WeaponEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *WeaponEntityPanel)
    {
        WeaponEntityPanel->setWindowTitle(QApplication::translate("WeaponEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("WeaponEntityPanel", "Weapon", 0)));
        label_6->setText(QApplication::translate("WeaponEntityPanel", "Radius", 0));
        label_7->setText(QApplication::translate("WeaponEntityPanel", "Boss Laser Duration", 0));
        label_8->setText(QApplication::translate("WeaponEntityPanel", "Charging Duration", 0));
    } // retranslateUi

};

namespace Ui {
    class WeaponEntityPanel: public Ui_WeaponEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // WEAPONENTITYPANEL_UI_H
