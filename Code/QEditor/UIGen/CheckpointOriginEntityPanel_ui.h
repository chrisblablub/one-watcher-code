/********************************************************************************
** Form generated from reading UI file 'CheckpointOriginEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CHECKPOINTORIGINENTITYPANEL_UI_H
#define CHECKPOINTORIGINENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_CheckpointOriginEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QCheckBox *pCheckBoxDisabled;

    void setupUi(QWidget *CheckpointOriginEntityPanel)
    {
        if (CheckpointOriginEntityPanel->objectName().isEmpty())
            CheckpointOriginEntityPanel->setObjectName(QStringLiteral("CheckpointOriginEntityPanel"));
        CheckpointOriginEntityPanel->resize(270, 248);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CheckpointOriginEntityPanel->sizePolicy().hasHeightForWidth());
        CheckpointOriginEntityPanel->setSizePolicy(sizePolicy);
        CheckpointOriginEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(CheckpointOriginEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(CheckpointOriginEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 100));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        pCheckBoxDisabled = new QCheckBox(qEditorPanelBox);
        pCheckBoxDisabled->setObjectName(QStringLiteral("pCheckBoxDisabled"));
        pCheckBoxDisabled->setGeometry(QRect(150, 50, 101, 20));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(CheckpointOriginEntityPanel);

        QMetaObject::connectSlotsByName(CheckpointOriginEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *CheckpointOriginEntityPanel)
    {
        CheckpointOriginEntityPanel->setWindowTitle(QApplication::translate("CheckpointOriginEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("CheckpointOriginEntityPanel", "Checkpoint Destination", 0)));
        pLabelName->setText(QString());
        pCheckBoxDisabled->setText(QApplication::translate("CheckpointOriginEntityPanel", "Initially disabled", 0));
    } // retranslateUi

};

namespace Ui {
    class CheckpointOriginEntityPanel: public Ui_CheckpointOriginEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CHECKPOINTORIGINENTITYPANEL_UI_H
