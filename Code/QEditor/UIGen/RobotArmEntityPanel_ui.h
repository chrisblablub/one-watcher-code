/********************************************************************************
** Form generated from reading UI file 'RobotArmEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ROBOTARMENTITYPANEL_UI_H
#define ROBOTARMENTITYPANEL_UI_H

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

class Ui_RobotArmEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxHitPauseTime;
    QDoubleSpinBox *pSpinBoxReverseTime;
    QLabel *label;
    QLabel *label_2;
    QDoubleSpinBox *pSpinBoxHitTime;
    QDoubleSpinBox *pSpinBoxDelay;
    QLabel *label_6;
    QDoubleSpinBox *pSpinBoxReversePauseTime;
    QCheckBox *pCheckBoxMirror;

    void setupUi(QWidget *RobotArmEntityPanel)
    {
        if (RobotArmEntityPanel->objectName().isEmpty())
            RobotArmEntityPanel->setObjectName(QStringLiteral("RobotArmEntityPanel"));
        RobotArmEntityPanel->resize(270, 405);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RobotArmEntityPanel->sizePolicy().hasHeightForWidth());
        RobotArmEntityPanel->setSizePolicy(sizePolicy);
        RobotArmEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(RobotArmEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(RobotArmEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 180));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 90, 81, 16));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 110, 71, 16));
        pSpinBoxHitPauseTime = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxHitPauseTime->setObjectName(QStringLiteral("pSpinBoxHitPauseTime"));
        pSpinBoxHitPauseTime->setGeometry(QRect(150, 90, 101, 18));
        pSpinBoxHitPauseTime->setMinimum(0);
        pSpinBoxHitPauseTime->setMaximum(100000);
        pSpinBoxHitPauseTime->setSingleStep(0.05);
        pSpinBoxReverseTime = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxReverseTime->setObjectName(QStringLiteral("pSpinBoxReverseTime"));
        pSpinBoxReverseTime->setGeometry(QRect(150, 110, 101, 18));
        pSpinBoxReverseTime->setMinimum(0);
        pSpinBoxReverseTime->setMaximum(100000);
        pSpinBoxReverseTime->setSingleStep(0.05);
        label = new QLabel(qEditorPanelBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 70, 81, 16));
        label_2 = new QLabel(qEditorPanelBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 50, 71, 16));
        pSpinBoxHitTime = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxHitTime->setObjectName(QStringLiteral("pSpinBoxHitTime"));
        pSpinBoxHitTime->setGeometry(QRect(150, 70, 101, 18));
        pSpinBoxHitTime->setMaximum(100000);
        pSpinBoxHitTime->setSingleStep(0.05);
        pSpinBoxDelay = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxDelay->setObjectName(QStringLiteral("pSpinBoxDelay"));
        pSpinBoxDelay->setGeometry(QRect(150, 50, 101, 18));
        pSpinBoxDelay->setMaximum(100000);
        pSpinBoxDelay->setSingleStep(0.05);
        label_6 = new QLabel(qEditorPanelBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 130, 111, 16));
        pSpinBoxReversePauseTime = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxReversePauseTime->setObjectName(QStringLiteral("pSpinBoxReversePauseTime"));
        pSpinBoxReversePauseTime->setGeometry(QRect(150, 130, 101, 18));
        pSpinBoxReversePauseTime->setMaximum(100000);
        pSpinBoxReversePauseTime->setSingleStep(0.05);
        pCheckBoxMirror = new QCheckBox(qEditorPanelBox);
        pCheckBoxMirror->setObjectName(QStringLiteral("pCheckBoxMirror"));
        pCheckBoxMirror->setGeometry(QRect(149, 150, 101, 21));

        gridLayout->addWidget(qEditorPanelBox, 0, 1, 1, 1);


        retranslateUi(RobotArmEntityPanel);

        QMetaObject::connectSlotsByName(RobotArmEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *RobotArmEntityPanel)
    {
        RobotArmEntityPanel->setWindowTitle(QApplication::translate("RobotArmEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("RobotArmEntityPanel", "Robot Arm", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("RobotArmEntityPanel", "Hit Pause Time", 0));
        label_5->setText(QApplication::translate("RobotArmEntityPanel", "Reverse Time", 0));
        label->setText(QApplication::translate("RobotArmEntityPanel", "Hit Time", 0));
        label_2->setText(QApplication::translate("RobotArmEntityPanel", "Delay", 0));
        label_6->setText(QApplication::translate("RobotArmEntityPanel", "Reverse Pause Time", 0));
        pCheckBoxMirror->setText(QApplication::translate("RobotArmEntityPanel", "Mirror", 0));
    } // retranslateUi

};

namespace Ui {
    class RobotArmEntityPanel: public Ui_RobotArmEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ROBOTARMENTITYPANEL_UI_H
