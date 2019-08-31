/********************************************************************************
** Form generated from reading UI file 'SkyDomeEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SKYDOMEENTITYPANEL_UI_H
#define SKYDOMEENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_SkyDomeEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QPushButton *pBtnChooseMesh;
    QLabel *label;

    void setupUi(QWidget *SkyDomeEntityPanel)
    {
        if (SkyDomeEntityPanel->objectName().isEmpty())
            SkyDomeEntityPanel->setObjectName(QStringLiteral("SkyDomeEntityPanel"));
        SkyDomeEntityPanel->resize(270, 278);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SkyDomeEntityPanel->sizePolicy().hasHeightForWidth());
        SkyDomeEntityPanel->setSizePolicy(sizePolicy);
        SkyDomeEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(SkyDomeEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(SkyDomeEntityPanel);
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
        pBtnChooseMesh = new QPushButton(qEditorPanelBox);
        pBtnChooseMesh->setObjectName(QStringLiteral("pBtnChooseMesh"));
        pBtnChooseMesh->setGeometry(QRect(10, 70, 251, 23));
        pBtnChooseMesh->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label = new QLabel(qEditorPanelBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 50, 81, 16));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(SkyDomeEntityPanel);

        QMetaObject::connectSlotsByName(SkyDomeEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *SkyDomeEntityPanel)
    {
        SkyDomeEntityPanel->setWindowTitle(QApplication::translate("SkyDomeEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("SkyDomeEntityPanel", "Sky Dome", 0)));
        pLabelName->setText(QString());
        pBtnChooseMesh->setText(QApplication::translate("SkyDomeEntityPanel", "Choose Texture", 0));
        label->setText(QApplication::translate("SkyDomeEntityPanel", "Sky Texture", 0));
    } // retranslateUi

};

namespace Ui {
    class SkyDomeEntityPanel: public Ui_SkyDomeEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SKYDOMEENTITYPANEL_UI_H
