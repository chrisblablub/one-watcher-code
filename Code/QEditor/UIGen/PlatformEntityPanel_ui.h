/********************************************************************************
** Form generated from reading UI file 'PlatformEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PLATFORMENTITYPANEL_UI_H
#define PLATFORMENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_PlatformEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *groupBox;
    QRadioButton *pRadioBtnTop;
    QRadioButton *pRadioBtnBottom;
    QRadioButton *pRadioBtnTopBottom;

    void setupUi(QWidget *PlatformEntityPanel)
    {
        if (PlatformEntityPanel->objectName().isEmpty())
            PlatformEntityPanel->setObjectName(QStringLiteral("PlatformEntityPanel"));
        PlatformEntityPanel->resize(270, 265);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PlatformEntityPanel->sizePolicy().hasHeightForWidth());
        PlatformEntityPanel->setSizePolicy(sizePolicy);
        PlatformEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(PlatformEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(PlatformEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 150));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 40, 251, 91));
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
        pRadioBtnTop = new QRadioButton(groupBox);
        pRadioBtnTop->setObjectName(QStringLiteral("pRadioBtnTop"));
        pRadioBtnTop->setGeometry(QRect(40, 20, 61, 17));
        pRadioBtnBottom = new QRadioButton(groupBox);
        pRadioBtnBottom->setObjectName(QStringLiteral("pRadioBtnBottom"));
        pRadioBtnBottom->setGeometry(QRect(40, 40, 131, 17));
        pRadioBtnTopBottom = new QRadioButton(groupBox);
        pRadioBtnTopBottom->setObjectName(QStringLiteral("pRadioBtnTopBottom"));
        pRadioBtnTopBottom->setGeometry(QRect(40, 60, 82, 17));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(PlatformEntityPanel);

        QMetaObject::connectSlotsByName(PlatformEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *PlatformEntityPanel)
    {
        PlatformEntityPanel->setWindowTitle(QApplication::translate("PlatformEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("PlatformEntityPanel", "Platform", 0)));
        pLabelName->setText(QString());
        groupBox->setTitle(QApplication::translate("PlatformEntityPanel", "Type", 0));
        pRadioBtnTop->setText(QApplication::translate("PlatformEntityPanel", "Top", 0));
        pRadioBtnBottom->setText(QApplication::translate("PlatformEntityPanel", "Bottom", 0));
        pRadioBtnTopBottom->setText(QApplication::translate("PlatformEntityPanel", "Top Bottom", 0));
    } // retranslateUi

};

namespace Ui {
    class PlatformEntityPanel: public Ui_PlatformEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PLATFORMENTITYPANEL_UI_H
