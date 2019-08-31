/********************************************************************************
** Form generated from reading UI file 'EnvMapEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ENVMAPENTITYPANEL_UI_H
#define ENVMAPENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_EnvMapEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QCheckBox *pCheckBoxEnabled;
    QPushButton *pBtnChooseEnvMap;
    QLabel *label_7;

    void setupUi(QWidget *EnvMapEntityPanel)
    {
        if (EnvMapEntityPanel->objectName().isEmpty())
            EnvMapEntityPanel->setObjectName(QStringLiteral("EnvMapEntityPanel"));
        EnvMapEntityPanel->resize(324, 405);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EnvMapEntityPanel->sizePolicy().hasHeightForWidth());
        EnvMapEntityPanel->setSizePolicy(sizePolicy);
        EnvMapEntityPanel->setMinimumSize(QSize(0, 220));
        gridLayout = new QGridLayout(EnvMapEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(EnvMapEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 130));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pCheckBoxEnabled = new QCheckBox(qEditorPanelBox);
        pCheckBoxEnabled->setObjectName(QStringLiteral("pCheckBoxEnabled"));
        pCheckBoxEnabled->setGeometry(QRect(10, 100, 91, 17));
        pBtnChooseEnvMap = new QPushButton(qEditorPanelBox);
        pBtnChooseEnvMap->setObjectName(QStringLiteral("pBtnChooseEnvMap"));
        pBtnChooseEnvMap->setGeometry(QRect(10, 65, 251, 23));
        pBtnChooseEnvMap->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_7 = new QLabel(qEditorPanelBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 45, 101, 16));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(EnvMapEntityPanel);

        QMetaObject::connectSlotsByName(EnvMapEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *EnvMapEntityPanel)
    {
        EnvMapEntityPanel->setWindowTitle(QApplication::translate("EnvMapEntityPanel", "EnvMapEntityPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("EnvMapEntityPanel", "Sprite", 0)));
        pCheckBoxEnabled->setText(QApplication::translate("EnvMapEntityPanel", "Enabled", 0));
        pBtnChooseEnvMap->setText(QApplication::translate("EnvMapEntityPanel", "Choose Texture", 0));
        label_7->setText(QApplication::translate("EnvMapEntityPanel", "Environment Map", 0));
    } // retranslateUi

};

namespace Ui {
    class EnvMapEntityPanel: public Ui_EnvMapEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ENVMAPENTITYPANEL_UI_H
