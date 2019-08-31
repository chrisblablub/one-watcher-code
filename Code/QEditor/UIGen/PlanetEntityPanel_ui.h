/********************************************************************************
** Form generated from reading UI file 'PlanetEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PLANETENTITYPANEL_UI_H
#define PLANETENTITYPANEL_UI_H

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

class Ui_PlanetEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QPushButton *pBtnColor;
    QLabel *label_5;

    void setupUi(QWidget *PlanetEntityPanel)
    {
        if (PlanetEntityPanel->objectName().isEmpty())
            PlanetEntityPanel->setObjectName(QStringLiteral("PlanetEntityPanel"));
        PlanetEntityPanel->resize(270, 405);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PlanetEntityPanel->sizePolicy().hasHeightForWidth());
        PlanetEntityPanel->setSizePolicy(sizePolicy);
        PlanetEntityPanel->setMinimumSize(QSize(0, 220));
        gridLayout = new QGridLayout(PlanetEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(PlanetEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 90));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pBtnColor = new QPushButton(qEditorPanelBox);
        pBtnColor->setObjectName(QStringLiteral("pBtnColor"));
        pBtnColor->setGeometry(QRect(150, 50, 101, 23));
        pBtnColor->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 50, 81, 21));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(PlanetEntityPanel);

        QMetaObject::connectSlotsByName(PlanetEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *PlanetEntityPanel)
    {
        PlanetEntityPanel->setWindowTitle(QApplication::translate("PlanetEntityPanel", "SpriteEntityPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("PlanetEntityPanel", "Planet", 0)));
        pBtnColor->setText(QString());
        label_5->setText(QApplication::translate("PlanetEntityPanel", "Planet Color", 0));
    } // retranslateUi

};

namespace Ui {
    class PlanetEntityPanel: public Ui_PlanetEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PLANETENTITYPANEL_UI_H
