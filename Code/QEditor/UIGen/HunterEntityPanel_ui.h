/********************************************************************************
** Form generated from reading UI file 'HunterEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef HUNTERENTITYPANEL_UI_H
#define HUNTERENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_HunterEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QComboBox *pComboBoxSpawnTarget;
    QPushButton *pBtnSetTarget;

    void setupUi(QWidget *HunterEntityPanel)
    {
        if (HunterEntityPanel->objectName().isEmpty())
            HunterEntityPanel->setObjectName(QStringLiteral("HunterEntityPanel"));
        HunterEntityPanel->resize(270, 227);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(HunterEntityPanel->sizePolicy().hasHeightForWidth());
        HunterEntityPanel->setSizePolicy(sizePolicy);
        HunterEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(HunterEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(HunterEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 120));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 50, 71, 21));
        pComboBoxSpawnTarget = new QComboBox(qEditorPanelBox);
        pComboBoxSpawnTarget->setObjectName(QStringLiteral("pComboBoxSpawnTarget"));
        pComboBoxSpawnTarget->setGeometry(QRect(100, 50, 151, 22));
        pBtnSetTarget = new QPushButton(qEditorPanelBox);
        pBtnSetTarget->setObjectName(QStringLiteral("pBtnSetTarget"));
        pBtnSetTarget->setGeometry(QRect(150, 80, 101, 23));
        pBtnSetTarget->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(HunterEntityPanel);

        QMetaObject::connectSlotsByName(HunterEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *HunterEntityPanel)
    {
        HunterEntityPanel->setWindowTitle(QApplication::translate("HunterEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("HunterEntityPanel", "Hunter", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("HunterEntityPanel", "Spawn Target", 0));
        pBtnSetTarget->setText(QApplication::translate("HunterEntityPanel", "Set Target", 0));
    } // retranslateUi

};

namespace Ui {
    class HunterEntityPanel: public Ui_HunterEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // HUNTERENTITYPANEL_UI_H
