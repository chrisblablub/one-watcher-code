/********************************************************************************
** Form generated from reading UI file 'CheckpointDestinationEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CHECKPOINTDESTINATIONENTITYPANEL_UI_H
#define CHECKPOINTDESTINATIONENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_CheckpointDestinationEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLineEdit *pLineEditNextLevel;
    QCheckBox *pCheckBoxFinishChapter;

    void setupUi(QWidget *CheckpointDestinationEntityPanel)
    {
        if (CheckpointDestinationEntityPanel->objectName().isEmpty())
            CheckpointDestinationEntityPanel->setObjectName(QStringLiteral("CheckpointDestinationEntityPanel"));
        CheckpointDestinationEntityPanel->resize(270, 246);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CheckpointDestinationEntityPanel->sizePolicy().hasHeightForWidth());
        CheckpointDestinationEntityPanel->setSizePolicy(sizePolicy);
        CheckpointDestinationEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(CheckpointDestinationEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(CheckpointDestinationEntityPanel);
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
        label_4->setGeometry(QRect(20, 50, 61, 16));
        pLineEditNextLevel = new QLineEdit(qEditorPanelBox);
        pLineEditNextLevel->setObjectName(QStringLiteral("pLineEditNextLevel"));
        pLineEditNextLevel->setGeometry(QRect(80, 50, 171, 20));
        pCheckBoxFinishChapter = new QCheckBox(qEditorPanelBox);
        pCheckBoxFinishChapter->setObjectName(QStringLiteral("pCheckBoxFinishChapter"));
        pCheckBoxFinishChapter->setGeometry(QRect(150, 80, 101, 20));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(CheckpointDestinationEntityPanel);

        QMetaObject::connectSlotsByName(CheckpointDestinationEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *CheckpointDestinationEntityPanel)
    {
        CheckpointDestinationEntityPanel->setWindowTitle(QApplication::translate("CheckpointDestinationEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("CheckpointDestinationEntityPanel", "Checkpoint Destination", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("CheckpointDestinationEntityPanel", "Next Level", 0));
        pCheckBoxFinishChapter->setText(QApplication::translate("CheckpointDestinationEntityPanel", "Finishes Chapter", 0));
    } // retranslateUi

};

namespace Ui {
    class CheckpointDestinationEntityPanel: public Ui_CheckpointDestinationEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CHECKPOINTDESTINATIONENTITYPANEL_UI_H
