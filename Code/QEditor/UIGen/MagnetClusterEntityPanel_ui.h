/********************************************************************************
** Form generated from reading UI file 'MagnetClusterEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAGNETCLUSTERENTITYPANEL_UI_H
#define MAGNETCLUSTERENTITYPANEL_UI_H

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

class Ui_MagnetClusterEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QDoubleSpinBox *pSpinBoxRadius;

    void setupUi(QWidget *MagnetClusterEntityPanel)
    {
        if (MagnetClusterEntityPanel->objectName().isEmpty())
            MagnetClusterEntityPanel->setObjectName(QStringLiteral("MagnetClusterEntityPanel"));
        MagnetClusterEntityPanel->resize(270, 246);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MagnetClusterEntityPanel->sizePolicy().hasHeightForWidth());
        MagnetClusterEntityPanel->setSizePolicy(sizePolicy);
        MagnetClusterEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(MagnetClusterEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(MagnetClusterEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 90));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 50, 81, 16));
        pSpinBoxRadius = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxRadius->setObjectName(QStringLiteral("pSpinBoxRadius"));
        pSpinBoxRadius->setGeometry(QRect(150, 50, 101, 18));
        pSpinBoxRadius->setMinimum(0);
        pSpinBoxRadius->setMaximum(100000);
        pSpinBoxRadius->setSingleStep(50);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(MagnetClusterEntityPanel);

        QMetaObject::connectSlotsByName(MagnetClusterEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *MagnetClusterEntityPanel)
    {
        MagnetClusterEntityPanel->setWindowTitle(QApplication::translate("MagnetClusterEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("MagnetClusterEntityPanel", "Magnet Cluster", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("MagnetClusterEntityPanel", "Radius", 0));
    } // retranslateUi

};

namespace Ui {
    class MagnetClusterEntityPanel: public Ui_MagnetClusterEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAGNETCLUSTERENTITYPANEL_UI_H
