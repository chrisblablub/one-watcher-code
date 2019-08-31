/********************************************************************************
** Form generated from reading UI file 'Character3DEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CHARACTER3DENTITYPANEL_UI_H
#define CHARACTER3DENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_Character3DEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QPushButton *pBtnDiffMtrl;
    QLabel *label_5;
    QPushButton *pBtnCharMtrl;
    QLabel *label_6;
    QDoubleSpinBox *pSpinBoxDiffuseAlpha;

    void setupUi(QWidget *Character3DEntityPanel)
    {
        if (Character3DEntityPanel->objectName().isEmpty())
            Character3DEntityPanel->setObjectName(QStringLiteral("Character3DEntityPanel"));
        Character3DEntityPanel->resize(270, 405);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Character3DEntityPanel->sizePolicy().hasHeightForWidth());
        Character3DEntityPanel->setSizePolicy(sizePolicy);
        Character3DEntityPanel->setMinimumSize(QSize(0, 220));
        gridLayout = new QGridLayout(Character3DEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(Character3DEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 150));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pBtnDiffMtrl = new QPushButton(qEditorPanelBox);
        pBtnDiffMtrl->setObjectName(QStringLiteral("pBtnDiffMtrl"));
        pBtnDiffMtrl->setGeometry(QRect(150, 50, 101, 23));
        pBtnDiffMtrl->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
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
        label_5->setGeometry(QRect(20, 50, 91, 21));
        pBtnCharMtrl = new QPushButton(qEditorPanelBox);
        pBtnCharMtrl->setObjectName(QStringLiteral("pBtnCharMtrl"));
        pBtnCharMtrl->setGeometry(QRect(150, 110, 101, 23));
        pBtnCharMtrl->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_6 = new QLabel(qEditorPanelBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 110, 101, 21));
        pSpinBoxDiffuseAlpha = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxDiffuseAlpha->setObjectName(QStringLiteral("pSpinBoxDiffuseAlpha"));
        pSpinBoxDiffuseAlpha->setGeometry(QRect(150, 80, 101, 22));
        pSpinBoxDiffuseAlpha->setMaximum(10);
        pSpinBoxDiffuseAlpha->setSingleStep(0.1);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(Character3DEntityPanel);

        QMetaObject::connectSlotsByName(Character3DEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *Character3DEntityPanel)
    {
        Character3DEntityPanel->setWindowTitle(QApplication::translate("Character3DEntityPanel", "Character3DEntityPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("Character3DEntityPanel", "Character3D", 0)));
        pBtnDiffMtrl->setText(QString());
        label_5->setText(QApplication::translate("Character3DEntityPanel", "Diffuse Material", 0));
        pBtnCharMtrl->setText(QString());
        label_6->setText(QApplication::translate("Character3DEntityPanel", "Character Material", 0));
    } // retranslateUi

};

namespace Ui {
    class Character3DEntityPanel: public Ui_Character3DEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CHARACTER3DENTITYPANEL_UI_H
