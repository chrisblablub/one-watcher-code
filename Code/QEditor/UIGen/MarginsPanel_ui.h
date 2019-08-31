/********************************************************************************
** Form generated from reading UI file 'MarginsPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MARGINSPANEL_UI_H
#define MARGINSPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_MarginsPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *pGroupBoxColor_2;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_2;
    QSpinBox *pSpinBoxBottomCell;
    QSpinBox *pSpinBoxLeftCell;
    QSpinBox *pSpinBoxTopCell;
    QLabel *label_3;
    QSpinBox *pSpinBoxRightCell;

    void setupUi(QWidget *MarginsPanel)
    {
        if (MarginsPanel->objectName().isEmpty())
            MarginsPanel->setObjectName(QStringLiteral("MarginsPanel"));
        MarginsPanel->resize(340, 252);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MarginsPanel->sizePolicy().hasHeightForWidth());
        MarginsPanel->setSizePolicy(sizePolicy);
        MarginsPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(MarginsPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(MarginsPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 190));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        pGroupBoxColor_2 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_2->setObjectName(QStringLiteral("pGroupBoxColor_2"));
        pGroupBoxColor_2->setGeometry(QRect(10, 40, 321, 141));
        pGroupBoxColor_2->setStyleSheet(QLatin1String("QGroupBox\n"
"{\n"
"	border: 1px solid #6e788a;\n"
"	border-radius:0px;\n"
"	margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}"));
        label = new QLabel(pGroupBoxColor_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 50, 111, 16));
        label_4 = new QLabel(pGroupBoxColor_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 81, 16));
        label_2 = new QLabel(pGroupBoxColor_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 80, 71, 16));
        pSpinBoxBottomCell = new QSpinBox(pGroupBoxColor_2);
        pSpinBoxBottomCell->setObjectName(QStringLiteral("pSpinBoxBottomCell"));
        pSpinBoxBottomCell->setGeometry(QRect(210, 80, 101, 22));
        pSpinBoxBottomCell->setMaximum(1000);
        pSpinBoxLeftCell = new QSpinBox(pGroupBoxColor_2);
        pSpinBoxLeftCell->setObjectName(QStringLiteral("pSpinBoxLeftCell"));
        pSpinBoxLeftCell->setGeometry(QRect(209, 20, 101, 22));
        pSpinBoxLeftCell->setMaximum(1000);
        pSpinBoxTopCell = new QSpinBox(pGroupBoxColor_2);
        pSpinBoxTopCell->setObjectName(QStringLiteral("pSpinBoxTopCell"));
        pSpinBoxTopCell->setGeometry(QRect(210, 110, 101, 22));
        pSpinBoxTopCell->setMaximum(1000);
        label_3 = new QLabel(pGroupBoxColor_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 110, 47, 13));
        pSpinBoxRightCell = new QSpinBox(pGroupBoxColor_2);
        pSpinBoxRightCell->setObjectName(QStringLiteral("pSpinBoxRightCell"));
        pSpinBoxRightCell->setGeometry(QRect(209, 50, 101, 22));
        pSpinBoxRightCell->setMaximum(1000);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(MarginsPanel);

        QMetaObject::connectSlotsByName(MarginsPanel);
    } // setupUi

    void retranslateUi(QWidget *MarginsPanel)
    {
        MarginsPanel->setWindowTitle(QApplication::translate("MarginsPanel", "MarginsPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("MarginsPanel", "Margins", 0)));
        pLabelName->setText(QString());
        pGroupBoxColor_2->setTitle(QApplication::translate("MarginsPanel", "Properties", 0));
        label->setText(QApplication::translate("MarginsPanel", "Right Cell", 0));
        label_4->setText(QApplication::translate("MarginsPanel", "Left Cell", 0));
        label_2->setText(QApplication::translate("MarginsPanel", "Bottom Cell", 0));
        label_3->setText(QApplication::translate("MarginsPanel", "Top Cell", 0));
    } // retranslateUi

};

namespace Ui {
    class MarginsPanel: public Ui_MarginsPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MARGINSPANEL_UI_H
