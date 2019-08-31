/********************************************************************************
** Form generated from reading UI file 'ExplodingHunterEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef EXPLODINGHUNTERENTITYPANEL_UI_H
#define EXPLODINGHUNTERENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_ExplodingHunterEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QDoubleSpinBox *pSpinBoxSpawnRate;
    QLabel *label_6;

    void setupUi(QWidget *ExplodingHunterEntityPanel)
    {
        if (ExplodingHunterEntityPanel->objectName().isEmpty())
            ExplodingHunterEntityPanel->setObjectName(QStringLiteral("ExplodingHunterEntityPanel"));
        ExplodingHunterEntityPanel->resize(270, 508);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ExplodingHunterEntityPanel->sizePolicy().hasHeightForWidth());
        ExplodingHunterEntityPanel->setSizePolicy(sizePolicy);
        ExplodingHunterEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(ExplodingHunterEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(ExplodingHunterEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 100));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pSpinBoxSpawnRate = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxSpawnRate->setObjectName(QStringLiteral("pSpinBoxSpawnRate"));
        pSpinBoxSpawnRate->setGeometry(QRect(141, 50, 111, 22));
        pSpinBoxSpawnRate->setMaximum(1e+6);
        pSpinBoxSpawnRate->setSingleStep(50);
        label_6 = new QLabel(qEditorPanelBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 50, 71, 16));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(ExplodingHunterEntityPanel);

        QMetaObject::connectSlotsByName(ExplodingHunterEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *ExplodingHunterEntityPanel)
    {
        ExplodingHunterEntityPanel->setWindowTitle(QApplication::translate("ExplodingHunterEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("ExplodingHunterEntityPanel", "Exploding Hunter Emitter", 0)));
        label_6->setText(QApplication::translate("ExplodingHunterEntityPanel", "Spawn Rate", 0));
    } // retranslateUi

};

namespace Ui {
    class ExplodingHunterEntityPanel: public Ui_ExplodingHunterEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // EXPLODINGHUNTERENTITYPANEL_UI_H
