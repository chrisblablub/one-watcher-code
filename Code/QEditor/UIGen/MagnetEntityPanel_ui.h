/********************************************************************************
** Form generated from reading UI file 'MagnetEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAGNETENTITYPANEL_UI_H
#define MAGNETENTITYPANEL_UI_H

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

class Ui_MagnetEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxForce;
    QDoubleSpinBox *pSpinBoxDuration;

    void setupUi(QWidget *MagnetEntityPanel)
    {
        if (MagnetEntityPanel->objectName().isEmpty())
            MagnetEntityPanel->setObjectName(QStringLiteral("MagnetEntityPanel"));
        MagnetEntityPanel->resize(270, 246);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MagnetEntityPanel->sizePolicy().hasHeightForWidth());
        MagnetEntityPanel->setSizePolicy(sizePolicy);
        MagnetEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(MagnetEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(MagnetEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 110));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 50, 81, 16));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 70, 61, 16));
        pSpinBoxForce = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxForce->setObjectName(QStringLiteral("pSpinBoxForce"));
        pSpinBoxForce->setGeometry(QRect(150, 50, 101, 18));
        pSpinBoxForce->setMinimum(0);
        pSpinBoxForce->setMaximum(100000);
        pSpinBoxForce->setSingleStep(50);
        pSpinBoxDuration = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxDuration->setObjectName(QStringLiteral("pSpinBoxDuration"));
        pSpinBoxDuration->setGeometry(QRect(150, 70, 101, 18));
        pSpinBoxDuration->setMinimum(0);
        pSpinBoxDuration->setMaximum(100000);
        pSpinBoxDuration->setSingleStep(50);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(MagnetEntityPanel);

        QMetaObject::connectSlotsByName(MagnetEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *MagnetEntityPanel)
    {
        MagnetEntityPanel->setWindowTitle(QApplication::translate("MagnetEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("MagnetEntityPanel", "Magnet", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("MagnetEntityPanel", "Force", 0));
        label_5->setText(QApplication::translate("MagnetEntityPanel", "Duration", 0));
    } // retranslateUi

};

namespace Ui {
    class MagnetEntityPanel: public Ui_MagnetEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAGNETENTITYPANEL_UI_H
