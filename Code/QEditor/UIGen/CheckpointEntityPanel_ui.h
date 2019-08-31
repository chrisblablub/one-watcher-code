/********************************************************************************
** Form generated from reading UI file 'CheckpointEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CHECKPOINTENTITYPANEL_UI_H
#define CHECKPOINTENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_CheckpointEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QGroupBox *groupBox;
    QRadioButton *pRadioBtnStart;
    QRadioButton *pRadioBtnFinal;
    QLineEdit *pLineEditNextLevel;
    QCheckBox *pCheckBoxFinishChapter;

    void setupUi(QWidget *CheckpointEntityPanel)
    {
        if (CheckpointEntityPanel->objectName().isEmpty())
            CheckpointEntityPanel->setObjectName(QStringLiteral("CheckpointEntityPanel"));
        CheckpointEntityPanel->resize(270, 246);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CheckpointEntityPanel->sizePolicy().hasHeightForWidth());
        CheckpointEntityPanel->setSizePolicy(sizePolicy);
        CheckpointEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(CheckpointEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(CheckpointEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 170));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 100, 61, 16));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 40, 251, 51));
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
        pRadioBtnStart = new QRadioButton(groupBox);
        pRadioBtnStart->setObjectName(QStringLiteral("pRadioBtnStart"));
        pRadioBtnStart->setGeometry(QRect(40, 20, 82, 17));
        pRadioBtnFinal = new QRadioButton(groupBox);
        pRadioBtnFinal->setObjectName(QStringLiteral("pRadioBtnFinal"));
        pRadioBtnFinal->setGeometry(QRect(160, 20, 82, 17));
        pLineEditNextLevel = new QLineEdit(qEditorPanelBox);
        pLineEditNextLevel->setObjectName(QStringLiteral("pLineEditNextLevel"));
        pLineEditNextLevel->setGeometry(QRect(80, 100, 171, 20));
        pCheckBoxFinishChapter = new QCheckBox(qEditorPanelBox);
        pCheckBoxFinishChapter->setObjectName(QStringLiteral("pCheckBoxFinishChapter"));
        pCheckBoxFinishChapter->setGeometry(QRect(150, 130, 101, 20));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(CheckpointEntityPanel);

        QMetaObject::connectSlotsByName(CheckpointEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *CheckpointEntityPanel)
    {
        CheckpointEntityPanel->setWindowTitle(QApplication::translate("CheckpointEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("CheckpointEntityPanel", "Checkpoint", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("CheckpointEntityPanel", "Next Level", 0));
        groupBox->setTitle(QApplication::translate("CheckpointEntityPanel", "Type", 0));
        pRadioBtnStart->setText(QApplication::translate("CheckpointEntityPanel", "Start", 0));
        pRadioBtnFinal->setText(QApplication::translate("CheckpointEntityPanel", "Final", 0));
        pCheckBoxFinishChapter->setText(QApplication::translate("CheckpointEntityPanel", "Finishes Chapter", 0));
    } // retranslateUi

};

namespace Ui {
    class CheckpointEntityPanel: public Ui_CheckpointEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CHECKPOINTENTITYPANEL_UI_H
