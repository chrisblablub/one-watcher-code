/********************************************************************************
** Form generated from reading UI file 'BordersPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BORDERSPANEL_UI_H
#define BORDERSPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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

class Ui_BordersPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *pGroupBoxColor_2;
    QPushButton *pBtnBorderTexture;
    QGroupBox *pGroupBoxColor_3;
    QLabel *label_6;
    QLabel *label_3;
    QLabel *label_2;
    QDoubleSpinBox *pSpinBoxBorderRotation;
    QLabel *label;
    QSpinBox *pSpinBoxMaximumDepth;
    QPushButton *pBtnRotationCenter;
    QPushButton *pBtnBorderColor;
    QLabel *label_4;

    void setupUi(QWidget *BordersPanel)
    {
        if (BordersPanel->objectName().isEmpty())
            BordersPanel->setObjectName(QStringLiteral("BordersPanel"));
        BordersPanel->resize(340, 397);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BordersPanel->sizePolicy().hasHeightForWidth());
        BordersPanel->setSizePolicy(sizePolicy);
        BordersPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(BordersPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(BordersPanel);
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
        pGroupBoxColor_2 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_2->setObjectName(QStringLiteral("pGroupBoxColor_2"));
        pGroupBoxColor_2->setGeometry(QRect(10, 40, 321, 51));
        pGroupBoxColor_2->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pBtnBorderTexture = new QPushButton(pGroupBoxColor_2);
        pBtnBorderTexture->setObjectName(QStringLiteral("pBtnBorderTexture"));
        pBtnBorderTexture->setGeometry(QRect(10, 20, 301, 23));
        pBtnBorderTexture->setStyleSheet(QStringLiteral(""));
        pGroupBoxColor_3 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_3->setObjectName(QStringLiteral("pGroupBoxColor_3"));
        pGroupBoxColor_3->setGeometry(QRect(10, 100, 321, 141));
        pGroupBoxColor_3->setStyleSheet(QLatin1String("QGroupBox\n"
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
        label_6 = new QLabel(pGroupBoxColor_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 20, 64, 16));
        label_3 = new QLabel(pGroupBoxColor_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 110, 71, 16));
        label_2 = new QLabel(pGroupBoxColor_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 80, 121, 21));
        pSpinBoxBorderRotation = new QDoubleSpinBox(pGroupBoxColor_3);
        pSpinBoxBorderRotation->setObjectName(QStringLiteral("pSpinBoxBorderRotation"));
        pSpinBoxBorderRotation->setGeometry(QRect(210, 50, 101, 22));
        pSpinBoxBorderRotation->setMinimum(-360);
        pSpinBoxBorderRotation->setMaximum(360);
        pSpinBoxBorderRotation->setSingleStep(0.01);
        label = new QLabel(pGroupBoxColor_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 50, 111, 16));
        pSpinBoxMaximumDepth = new QSpinBox(pGroupBoxColor_3);
        pSpinBoxMaximumDepth->setObjectName(QStringLiteral("pSpinBoxMaximumDepth"));
        pSpinBoxMaximumDepth->setGeometry(QRect(210, 20, 101, 22));
        pSpinBoxMaximumDepth->setMaximum(10000);
        pSpinBoxMaximumDepth->setSingleStep(10);
        pBtnRotationCenter = new QPushButton(pGroupBoxColor_3);
        pBtnRotationCenter->setObjectName(QStringLiteral("pBtnRotationCenter"));
        pBtnRotationCenter->setGeometry(QRect(210, 80, 101, 23));
        pBtnRotationCenter->setStyleSheet(QStringLiteral(""));
        pBtnBorderColor = new QPushButton(pGroupBoxColor_3);
        pBtnBorderColor->setObjectName(QStringLiteral("pBtnBorderColor"));
        pBtnBorderColor->setGeometry(QRect(210, 110, 101, 23));
        pBtnBorderColor->setStyleSheet(QStringLiteral(""));
        label_4 = new QLabel(pGroupBoxColor_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 81, 16));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(BordersPanel);

        QMetaObject::connectSlotsByName(BordersPanel);
    } // setupUi

    void retranslateUi(QWidget *BordersPanel)
    {
        BordersPanel->setWindowTitle(QApplication::translate("BordersPanel", "BordersPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("BordersPanel", "Borders", 0)));
        pLabelName->setText(QString());
        pGroupBoxColor_2->setTitle(QApplication::translate("BordersPanel", "Texture", 0));
        pBtnBorderTexture->setText(QApplication::translate("BordersPanel", "Choose Texture", 0));
        pGroupBoxColor_3->setTitle(QApplication::translate("BordersPanel", "Properties", 0));
        label_6->setText(QString());
        label_3->setText(QApplication::translate("BordersPanel", "Border Color", 0));
        label_2->setText(QApplication::translate("BordersPanel", "Border Rotation Center", 0));
        label->setText(QApplication::translate("BordersPanel", "Border Rotation (deg)", 0));
        pBtnRotationCenter->setText(QApplication::translate("BordersPanel", "Update", 0));
        pBtnBorderColor->setText(QString());
        label_4->setText(QApplication::translate("BordersPanel", "Maximum Depth", 0));
    } // retranslateUi

};

namespace Ui {
    class BordersPanel: public Ui_BordersPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BORDERSPANEL_UI_H
