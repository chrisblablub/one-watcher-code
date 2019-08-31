/********************************************************************************
** Form generated from reading UI file 'ForcefieldEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef FORCEFIELDENTITYPANEL_UI_H
#define FORCEFIELDENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_ForcefieldEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxAcceleration;
    QDoubleSpinBox *pSpinBoxTargetVelocity;
    QLabel *label;
    QPushButton *pBtnColor;

    void setupUi(QWidget *ForcefieldEntityPanel)
    {
        if (ForcefieldEntityPanel->objectName().isEmpty())
            ForcefieldEntityPanel->setObjectName(QStringLiteral("ForcefieldEntityPanel"));
        ForcefieldEntityPanel->resize(270, 246);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ForcefieldEntityPanel->sizePolicy().hasHeightForWidth());
        ForcefieldEntityPanel->setSizePolicy(sizePolicy);
        ForcefieldEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(ForcefieldEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(ForcefieldEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 130));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 50, 81, 16));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 70, 81, 16));
        pSpinBoxAcceleration = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxAcceleration->setObjectName(QStringLiteral("pSpinBoxAcceleration"));
        pSpinBoxAcceleration->setGeometry(QRect(150, 50, 101, 18));
        pSpinBoxAcceleration->setMinimum(0);
        pSpinBoxAcceleration->setMaximum(100000);
        pSpinBoxAcceleration->setSingleStep(50);
        pSpinBoxTargetVelocity = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxTargetVelocity->setObjectName(QStringLiteral("pSpinBoxTargetVelocity"));
        pSpinBoxTargetVelocity->setGeometry(QRect(150, 70, 101, 18));
        pSpinBoxTargetVelocity->setMinimum(0);
        pSpinBoxTargetVelocity->setMaximum(100000);
        pSpinBoxTargetVelocity->setSingleStep(50);
        label = new QLabel(qEditorPanelBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 90, 47, 13));
        pBtnColor = new QPushButton(qEditorPanelBox);
        pBtnColor->setObjectName(QStringLiteral("pBtnColor"));
        pBtnColor->setGeometry(QRect(150, 90, 101, 23));
        pBtnColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(ForcefieldEntityPanel);

        QMetaObject::connectSlotsByName(ForcefieldEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *ForcefieldEntityPanel)
    {
        ForcefieldEntityPanel->setWindowTitle(QApplication::translate("ForcefieldEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("ForcefieldEntityPanel", "Forcefield", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("ForcefieldEntityPanel", "Acceleration", 0));
        label_5->setText(QApplication::translate("ForcefieldEntityPanel", "Target Velocity", 0));
        label->setText(QApplication::translate("ForcefieldEntityPanel", "Color", 0));
        pBtnColor->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ForcefieldEntityPanel: public Ui_ForcefieldEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // FORCEFIELDENTITYPANEL_UI_H
