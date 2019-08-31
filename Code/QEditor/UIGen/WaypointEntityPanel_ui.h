/********************************************************************************
** Form generated from reading UI file 'WaypointEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WAYPOINTENTITYPANEL_UI_H
#define WAYPOINTENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_WaypointEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxPause;
    QDoubleSpinBox *pSpinBoxBreaking;
    QLabel *label;
    QLabel *label_2;
    QDoubleSpinBox *pSpinBoxAcceleration;
    QDoubleSpinBox *pSpinBoxSpeed;
    QLabel *label_3;
    QComboBox *pComboBoxPath;
    QPushButton *pBtnAttach;

    void setupUi(QWidget *WaypointEntityPanel)
    {
        if (WaypointEntityPanel->objectName().isEmpty())
            WaypointEntityPanel->setObjectName(QStringLiteral("WaypointEntityPanel"));
        WaypointEntityPanel->resize(270, 246);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(WaypointEntityPanel->sizePolicy().hasHeightForWidth());
        WaypointEntityPanel->setSizePolicy(sizePolicy);
        WaypointEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(WaypointEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(WaypointEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 190));
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
        pSpinBoxPause = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxPause->setObjectName(QStringLiteral("pSpinBoxPause"));
        pSpinBoxPause->setGeometry(QRect(150, 140, 101, 18));
        pSpinBoxPause->setMinimum(0);
        pSpinBoxPause->setMaximum(100000);
        pSpinBoxPause->setSingleStep(0.05);
        pSpinBoxBreaking = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxBreaking->setObjectName(QStringLiteral("pSpinBoxBreaking"));
        pSpinBoxBreaking->setGeometry(QRect(150, 160, 101, 18));
        pSpinBoxBreaking->setMinimum(0);
        pSpinBoxBreaking->setMaximum(100000);
        pSpinBoxBreaking->setSingleStep(25);
        label = new QLabel(qEditorPanelBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 120, 81, 16));
        label_2 = new QLabel(qEditorPanelBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 100, 71, 16));
        pSpinBoxAcceleration = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxAcceleration->setObjectName(QStringLiteral("pSpinBoxAcceleration"));
        pSpinBoxAcceleration->setGeometry(QRect(150, 120, 101, 18));
        pSpinBoxAcceleration->setMaximum(100000);
        pSpinBoxAcceleration->setSingleStep(25);
        pSpinBoxSpeed = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxSpeed->setObjectName(QStringLiteral("pSpinBoxSpeed"));
        pSpinBoxSpeed->setGeometry(QRect(150, 100, 101, 18));
        pSpinBoxSpeed->setMaximum(100000);
        pSpinBoxSpeed->setSingleStep(25);
        label_3 = new QLabel(qEditorPanelBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 40, 21, 21));
        pComboBoxPath = new QComboBox(qEditorPanelBox);
        pComboBoxPath->setObjectName(QStringLiteral("pComboBoxPath"));
        pComboBoxPath->setGeometry(QRect(50, 40, 201, 22));
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

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(WaypointEntityPanel);

        QMetaObject::connectSlotsByName(WaypointEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *WaypointEntityPanel)
    {
        WaypointEntityPanel->setWindowTitle(QApplication::translate("WaypointEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("WaypointEntityPanel", "Waypoint", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("WaypointEntityPanel", "Pause", 0));
        label_5->setText(QApplication::translate("WaypointEntityPanel", "Breaking", 0));
        label->setText(QApplication::translate("WaypointEntityPanel", "Acceleration", 0));
        label_2->setText(QApplication::translate("WaypointEntityPanel", "Speed", 0));
        label_3->setText(QApplication::translate("WaypointEntityPanel", "Path", 0));
        pBtnAttach->setText(QApplication::translate("WaypointEntityPanel", "Attach", 0));
    } // retranslateUi

};

namespace Ui {
    class WaypointEntityPanel: public Ui_WaypointEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // WAYPOINTENTITYPANEL_UI_H
