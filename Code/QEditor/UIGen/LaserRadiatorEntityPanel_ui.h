/********************************************************************************
** Form generated from reading UI file 'LaserRadiatorEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LASERRADIATORENTITYPANEL_UI_H
#define LASERRADIATORENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_LaserRadiatorEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxLoadingTime;
    QDoubleSpinBox *pSpinBoxDelay;
    QLabel *label;
    QLabel *label_2;
    QDoubleSpinBox *pSpinBoxPausingTime;
    QDoubleSpinBox *pSpinBoxActiveTime;
    QLabel *label_3;
    QDoubleSpinBox *pSpinBoxMaxDistance;
    QDoubleSpinBox *pSpinBoxOffsetRadius;
    QSpinBox *pSpinBoxLasers;
    QLabel *label_6;
    QLabel *label_7;
    QCheckBox *pCheckBoxTerrainCollisions;

    void setupUi(QWidget *LaserRadiatorEntityPanel)
    {
        if (LaserRadiatorEntityPanel->objectName().isEmpty())
            LaserRadiatorEntityPanel->setObjectName(QStringLiteral("LaserRadiatorEntityPanel"));
        LaserRadiatorEntityPanel->resize(270, 507);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LaserRadiatorEntityPanel->sizePolicy().hasHeightForWidth());
        LaserRadiatorEntityPanel->setSizePolicy(sizePolicy);
        LaserRadiatorEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(LaserRadiatorEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(LaserRadiatorEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 230));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 110, 81, 16));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 130, 71, 16));
        pSpinBoxLoadingTime = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxLoadingTime->setObjectName(QStringLiteral("pSpinBoxLoadingTime"));
        pSpinBoxLoadingTime->setGeometry(QRect(150, 130, 101, 18));
        pSpinBoxLoadingTime->setMinimum(0);
        pSpinBoxLoadingTime->setMaximum(10000);
        pSpinBoxLoadingTime->setSingleStep(0.05);
        pSpinBoxDelay = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxDelay->setObjectName(QStringLiteral("pSpinBoxDelay"));
        pSpinBoxDelay->setGeometry(QRect(150, 150, 101, 18));
        pSpinBoxDelay->setMinimum(0);
        pSpinBoxDelay->setMaximum(100000);
        pSpinBoxDelay->setSingleStep(0.05);
        pSpinBoxDelay->setValue(0);
        label = new QLabel(qEditorPanelBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 90, 81, 16));
        label_2 = new QLabel(qEditorPanelBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 70, 101, 16));
        pSpinBoxPausingTime = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxPausingTime->setObjectName(QStringLiteral("pSpinBoxPausingTime"));
        pSpinBoxPausingTime->setGeometry(QRect(150, 110, 101, 18));
        pSpinBoxPausingTime->setMaximum(10000);
        pSpinBoxPausingTime->setSingleStep(0.05);
        pSpinBoxActiveTime = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxActiveTime->setObjectName(QStringLiteral("pSpinBoxActiveTime"));
        pSpinBoxActiveTime->setGeometry(QRect(150, 90, 101, 18));
        pSpinBoxActiveTime->setMaximum(100000);
        pSpinBoxActiveTime->setSingleStep(0.05);
        label_3 = new QLabel(qEditorPanelBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 50, 61, 21));
        pSpinBoxMaxDistance = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxMaxDistance->setObjectName(QStringLiteral("pSpinBoxMaxDistance"));
        pSpinBoxMaxDistance->setGeometry(QRect(150, 70, 101, 18));
        pSpinBoxMaxDistance->setMaximum(10000);
        pSpinBoxMaxDistance->setSingleStep(10);
        pSpinBoxOffsetRadius = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxOffsetRadius->setObjectName(QStringLiteral("pSpinBoxOffsetRadius"));
        pSpinBoxOffsetRadius->setGeometry(QRect(150, 170, 101, 18));
        pSpinBoxOffsetRadius->setMaximum(100000);
        pSpinBoxOffsetRadius->setSingleStep(10);
        pSpinBoxLasers = new QSpinBox(qEditorPanelBox);
        pSpinBoxLasers->setObjectName(QStringLiteral("pSpinBoxLasers"));
        pSpinBoxLasers->setGeometry(QRect(150, 50, 101, 18));
        label_6 = new QLabel(qEditorPanelBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 150, 47, 16));
        label_7 = new QLabel(qEditorPanelBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 170, 71, 16));
        pCheckBoxTerrainCollisions = new QCheckBox(qEditorPanelBox);
        pCheckBoxTerrainCollisions->setObjectName(QStringLiteral("pCheckBoxTerrainCollisions"));
        pCheckBoxTerrainCollisions->setGeometry(QRect(150, 190, 101, 17));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(LaserRadiatorEntityPanel);

        QMetaObject::connectSlotsByName(LaserRadiatorEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *LaserRadiatorEntityPanel)
    {
        LaserRadiatorEntityPanel->setWindowTitle(QApplication::translate("LaserRadiatorEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("LaserRadiatorEntityPanel", "Laser Radiator", 0)));
        label_4->setText(QApplication::translate("LaserRadiatorEntityPanel", "Pausing Time", 0));
        label_5->setText(QApplication::translate("LaserRadiatorEntityPanel", "Loading Time", 0));
        label->setText(QApplication::translate("LaserRadiatorEntityPanel", "Active Time", 0));
        label_2->setText(QApplication::translate("LaserRadiatorEntityPanel", "Maximum Distance", 0));
        label_3->setText(QApplication::translate("LaserRadiatorEntityPanel", "Lasers", 0));
        label_6->setText(QApplication::translate("LaserRadiatorEntityPanel", "Delay", 0));
        label_7->setText(QApplication::translate("LaserRadiatorEntityPanel", "Offset Radius", 0));
        pCheckBoxTerrainCollisions->setText(QApplication::translate("LaserRadiatorEntityPanel", "Terrain Collisions", 0));
    } // retranslateUi

};

namespace Ui {
    class LaserRadiatorEntityPanel: public Ui_LaserRadiatorEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LASERRADIATORENTITYPANEL_UI_H
