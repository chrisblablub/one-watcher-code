/********************************************************************************
** Form generated from reading UI file 'RotatorEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ROTATORENTITYPANEL_UI_H
#define ROTATORENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_RotatorEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxStartAngle;
    QDoubleSpinBox *pSpinBoxStartPause;
    QLabel *label;
    QLabel *label_2;
    QDoubleSpinBox *pSpinBoxAngle;
    QDoubleSpinBox *pSpinBoxSpeed;
    QLabel *label_3;
    QComboBox *pComboBoxEntity;
    QPushButton *pBtnAttach;
    QLabel *label_6;
    QDoubleSpinBox *pSpinBoxEndPause;
    QLabel *label_7;
    QDoubleSpinBox *pSpinBoxRadius;
    QCheckBox *pCheckBoxReverse;

    void setupUi(QWidget *RotatorEntityPanel)
    {
        if (RotatorEntityPanel->objectName().isEmpty())
            RotatorEntityPanel->setObjectName(QStringLiteral("RotatorEntityPanel"));
        RotatorEntityPanel->resize(270, 405);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RotatorEntityPanel->sizePolicy().hasHeightForWidth());
        RotatorEntityPanel->setSizePolicy(sizePolicy);
        RotatorEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(RotatorEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(RotatorEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 250));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 140, 81, 16));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 160, 71, 16));
        pSpinBoxStartAngle = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxStartAngle->setObjectName(QStringLiteral("pSpinBoxStartAngle"));
        pSpinBoxStartAngle->setGeometry(QRect(150, 140, 101, 18));
        pSpinBoxStartAngle->setMinimum(0);
        pSpinBoxStartAngle->setMaximum(360);
        pSpinBoxStartAngle->setSingleStep(10);
        pSpinBoxStartPause = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxStartPause->setObjectName(QStringLiteral("pSpinBoxStartPause"));
        pSpinBoxStartPause->setGeometry(QRect(150, 160, 101, 18));
        pSpinBoxStartPause->setMinimum(0);
        pSpinBoxStartPause->setMaximum(100000);
        pSpinBoxStartPause->setSingleStep(0.05);
        label = new QLabel(qEditorPanelBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 120, 81, 16));
        label_2 = new QLabel(qEditorPanelBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 100, 71, 16));
        pSpinBoxAngle = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxAngle->setObjectName(QStringLiteral("pSpinBoxAngle"));
        pSpinBoxAngle->setGeometry(QRect(150, 120, 101, 18));
        pSpinBoxAngle->setMaximum(360);
        pSpinBoxAngle->setSingleStep(10);
        pSpinBoxSpeed = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxSpeed->setObjectName(QStringLiteral("pSpinBoxSpeed"));
        pSpinBoxSpeed->setGeometry(QRect(150, 100, 101, 18));
        pSpinBoxSpeed->setMinimum(-100000);
        pSpinBoxSpeed->setMaximum(100000);
        pSpinBoxSpeed->setSingleStep(0.05);
        label_3 = new QLabel(qEditorPanelBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 40, 31, 21));
        pComboBoxEntity = new QComboBox(qEditorPanelBox);
        pComboBoxEntity->setObjectName(QStringLiteral("pComboBoxEntity"));
        pComboBoxEntity->setGeometry(QRect(60, 40, 191, 22));
        pBtnAttach = new QPushButton(qEditorPanelBox);
        pBtnAttach->setObjectName(QStringLiteral("pBtnAttach"));
        pBtnAttach->setGeometry(QRect(150, 70, 101, 23));
        pBtnAttach->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_6 = new QLabel(qEditorPanelBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 180, 61, 16));
        pSpinBoxEndPause = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxEndPause->setObjectName(QStringLiteral("pSpinBoxEndPause"));
        pSpinBoxEndPause->setGeometry(QRect(150, 180, 101, 18));
        pSpinBoxEndPause->setMaximum(100000);
        pSpinBoxEndPause->setSingleStep(0.05);
        label_7 = new QLabel(qEditorPanelBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 200, 47, 13));
        pSpinBoxRadius = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxRadius->setObjectName(QStringLiteral("pSpinBoxRadius"));
        pSpinBoxRadius->setGeometry(QRect(150, 200, 101, 18));
        pSpinBoxRadius->setMaximum(100000);
        pSpinBoxRadius->setSingleStep(10);
        pCheckBoxReverse = new QCheckBox(qEditorPanelBox);
        pCheckBoxReverse->setObjectName(QStringLiteral("pCheckBoxReverse"));
        pCheckBoxReverse->setGeometry(QRect(149, 220, 101, 21));

        gridLayout->addWidget(qEditorPanelBox, 0, 1, 1, 1);


        retranslateUi(RotatorEntityPanel);

        QMetaObject::connectSlotsByName(RotatorEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *RotatorEntityPanel)
    {
        RotatorEntityPanel->setWindowTitle(QApplication::translate("RotatorEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("RotatorEntityPanel", "Rotator", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("RotatorEntityPanel", "Start Angle", 0));
        label_5->setText(QApplication::translate("RotatorEntityPanel", "Start Pause", 0));
        label->setText(QApplication::translate("RotatorEntityPanel", "Angle", 0));
        label_2->setText(QApplication::translate("RotatorEntityPanel", "Speed", 0));
        label_3->setText(QApplication::translate("RotatorEntityPanel", "Entity", 0));
        pBtnAttach->setText(QApplication::translate("RotatorEntityPanel", "Attach", 0));
        label_6->setText(QApplication::translate("RotatorEntityPanel", "End Pause", 0));
        label_7->setText(QApplication::translate("RotatorEntityPanel", "Radius", 0));
        pCheckBoxReverse->setText(QApplication::translate("RotatorEntityPanel", "Reverse", 0));
    } // retranslateUi

};

namespace Ui {
    class RotatorEntityPanel: public Ui_RotatorEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ROTATORENTITYPANEL_UI_H
