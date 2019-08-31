/********************************************************************************
** Form generated from reading UI file 'PathEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PATHENTITYPANEL_UI_H
#define PATHENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_PathEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QComboBox *pComboAttachEntity;
    QCheckBox *pCheckBoxReverse;
    QCheckBox *pCheckBoxLoop;
    QGroupBox *groupBox;
    QLabel *label_5;
    QComboBox *pComboEditAttachee;
    QLabel *label;
    QDoubleSpinBox *pSpinBoxTimeOffset;
    QLabel *label_2;
    QSpinBox *pSpinBoxStartingWaypoint;
    QCheckBox *pCheckBoxFollowPlayer;
    QPushButton *pBtnDetachEntity;
    QLabel *label_3;
    QPushButton *pBtnAttachEntity;

    void setupUi(QWidget *PathEntityPanel)
    {
        if (PathEntityPanel->objectName().isEmpty())
            PathEntityPanel->setObjectName(QStringLiteral("PathEntityPanel"));
        PathEntityPanel->resize(270, 508);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PathEntityPanel->sizePolicy().hasHeightForWidth());
        PathEntityPanel->setSizePolicy(sizePolicy);
        PathEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(PathEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(PathEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 330));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 80, 71, 16));
        pComboAttachEntity = new QComboBox(qEditorPanelBox);
        pComboAttachEntity->setObjectName(QStringLiteral("pComboAttachEntity"));
        pComboAttachEntity->setGeometry(QRect(90, 80, 161, 22));
        pCheckBoxReverse = new QCheckBox(qEditorPanelBox);
        pCheckBoxReverse->setObjectName(QStringLiteral("pCheckBoxReverse"));
        pCheckBoxReverse->setGeometry(QRect(170, 50, 71, 20));
        pCheckBoxLoop = new QCheckBox(qEditorPanelBox);
        pCheckBoxLoop->setObjectName(QStringLiteral("pCheckBoxLoop"));
        pCheckBoxLoop->setGeometry(QRect(90, 50, 70, 17));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 140, 251, 171));
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
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 110, 71, 16));
        pSpinBoxTimeOffset = new QDoubleSpinBox(groupBox);
        pSpinBoxTimeOffset->setObjectName(QStringLiteral("pSpinBoxTimeOffset"));
        pSpinBoxTimeOffset->setGeometry(QRect(140, 110, 101, 21));
        pSpinBoxTimeOffset->setMinimum(0);
        pSpinBoxTimeOffset->setMaximum(100000);
        pSpinBoxTimeOffset->setSingleStep(0.01);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 80, 111, 16));
        pSpinBoxStartingWaypoint = new QSpinBox(groupBox);
        pSpinBoxStartingWaypoint->setObjectName(QStringLiteral("pSpinBoxStartingWaypoint"));
        pSpinBoxStartingWaypoint->setGeometry(QRect(140, 80, 101, 22));
        pCheckBoxFollowPlayer = new QCheckBox(groupBox);
        pCheckBoxFollowPlayer->setObjectName(QStringLiteral("pCheckBoxFollowPlayer"));
        pCheckBoxFollowPlayer->setGeometry(QRect(140, 140, 101, 17));
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
        label_3 = new QLabel(qEditorPanelBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 50, 31, 16));
        pBtnAttachEntity = new QPushButton(qEditorPanelBox);
        pBtnAttachEntity->setObjectName(QStringLiteral("pBtnAttachEntity"));
        pBtnAttachEntity->setGeometry(QRect(90, 110, 161, 23));
        pBtnAttachEntity->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:center;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(PathEntityPanel);

        QMetaObject::connectSlotsByName(PathEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *PathEntityPanel)
    {
        PathEntityPanel->setWindowTitle(QApplication::translate("PathEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("PathEntityPanel", "Path", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("PathEntityPanel", "Attach Entity", 0));
        pCheckBoxReverse->setText(QApplication::translate("PathEntityPanel", "Reverse", 0));
        pCheckBoxLoop->setText(QApplication::translate("PathEntityPanel", "Loop", 0));
        groupBox->setTitle(QApplication::translate("PathEntityPanel", "Edit Attachee", 0));
        label_5->setText(QApplication::translate("PathEntityPanel", "Entity", 0));
        label->setText(QApplication::translate("PathEntityPanel", "Time Offset", 0));
        label_2->setText(QApplication::translate("PathEntityPanel", "Starting Waypoint", 0));
        pCheckBoxFollowPlayer->setText(QApplication::translate("PathEntityPanel", "Follow Player", 0));
        pBtnDetachEntity->setText(QApplication::translate("PathEntityPanel", "Detach Entity", 0));
        label_3->setText(QApplication::translate("PathEntityPanel", "Path", 0));
        pBtnAttachEntity->setText(QApplication::translate("PathEntityPanel", "Attach Entity", 0));
    } // retranslateUi

};

namespace Ui {
    class PathEntityPanel: public Ui_PathEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PATHENTITYPANEL_UI_H
