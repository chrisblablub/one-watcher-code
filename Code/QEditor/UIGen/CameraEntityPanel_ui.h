/********************************************************************************
** Form generated from reading UI file 'CameraEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CAMERAENTITYPANEL_UI_H
#define CAMERAENTITYPANEL_UI_H

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

class Ui_CameraEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxRotationSpeed;
    QDoubleSpinBox *pSpinBoxSmoothEnableDuration;
    QPushButton *pBtnEnableCam;
    QLabel *label;
    QComboBox *pComboTarget;
    QPushButton *pBtnDetachTarget;
    QPushButton *pBtnAttachTarget;
    QCheckBox *pCheckBoxRollToTarget;

    void setupUi(QWidget *CameraEntityPanel)
    {
        if (CameraEntityPanel->objectName().isEmpty())
            CameraEntityPanel->setObjectName(QStringLiteral("CameraEntityPanel"));
        CameraEntityPanel->resize(270, 330);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CameraEntityPanel->sizePolicy().hasHeightForWidth());
        CameraEntityPanel->setSizePolicy(sizePolicy);
        CameraEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(CameraEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(CameraEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 260));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 80, 51, 16));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 200, 81, 16));
        pSpinBoxRotationSpeed = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxRotationSpeed->setObjectName(QStringLiteral("pSpinBoxRotationSpeed"));
        pSpinBoxRotationSpeed->setGeometry(QRect(150, 200, 101, 18));
        pSpinBoxRotationSpeed->setMinimum(0);
        pSpinBoxRotationSpeed->setMaximum(100000);
        pSpinBoxRotationSpeed->setSingleStep(0.05);
        pSpinBoxSmoothEnableDuration = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxSmoothEnableDuration->setObjectName(QStringLiteral("pSpinBoxSmoothEnableDuration"));
        pSpinBoxSmoothEnableDuration->setGeometry(QRect(150, 220, 101, 18));
        pSpinBoxSmoothEnableDuration->setMinimum(0);
        pSpinBoxSmoothEnableDuration->setMaximum(100000);
        pSpinBoxSmoothEnableDuration->setSingleStep(0.05);
        pBtnEnableCam = new QPushButton(qEditorPanelBox);
        pBtnEnableCam->setObjectName(QStringLiteral("pBtnEnableCam"));
        pBtnEnableCam->setGeometry(QRect(10, 50, 251, 23));
        pBtnEnableCam->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:center;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label = new QLabel(qEditorPanelBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 220, 121, 16));
        pComboTarget = new QComboBox(qEditorPanelBox);
        pComboTarget->setObjectName(QStringLiteral("pComboTarget"));
        pComboTarget->setGeometry(QRect(70, 80, 181, 22));
        pBtnDetachTarget = new QPushButton(qEditorPanelBox);
        pBtnDetachTarget->setObjectName(QStringLiteral("pBtnDetachTarget"));
        pBtnDetachTarget->setGeometry(QRect(70, 140, 181, 23));
        pBtnDetachTarget->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:center;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pBtnAttachTarget = new QPushButton(qEditorPanelBox);
        pBtnAttachTarget->setObjectName(QStringLiteral("pBtnAttachTarget"));
        pBtnAttachTarget->setGeometry(QRect(70, 110, 181, 23));
        pBtnAttachTarget->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:center;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pCheckBoxRollToTarget = new QCheckBox(qEditorPanelBox);
        pCheckBoxRollToTarget->setObjectName(QStringLiteral("pCheckBoxRollToTarget"));
        pCheckBoxRollToTarget->setGeometry(QRect(150, 170, 101, 20));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(CameraEntityPanel);

        QMetaObject::connectSlotsByName(CameraEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *CameraEntityPanel)
    {
        CameraEntityPanel->setWindowTitle(QApplication::translate("CameraEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("CameraEntityPanel", "Camera", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("CameraEntityPanel", "Target", 0));
        label_5->setText(QApplication::translate("CameraEntityPanel", "Rotation Speed", 0));
        pBtnEnableCam->setText(QApplication::translate("CameraEntityPanel", "Enable Cam", 0));
        label->setText(QApplication::translate("CameraEntityPanel", "Smooth Enable Duration", 0));
        pBtnDetachTarget->setText(QApplication::translate("CameraEntityPanel", "Detach Target", 0));
        pBtnAttachTarget->setText(QApplication::translate("CameraEntityPanel", "Attach Target", 0));
        pCheckBoxRollToTarget->setText(QApplication::translate("CameraEntityPanel", "Roll To Target", 0));
    } // retranslateUi

};

namespace Ui {
    class CameraEntityPanel: public Ui_CameraEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CAMERAENTITYPANEL_UI_H
