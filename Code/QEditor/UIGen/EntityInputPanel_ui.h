/********************************************************************************
** Form generated from reading UI file 'EntityInputPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ENTITYINPUTPANEL_UI_H
#define ENTITYINPUTPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_EntityInputPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *pGroupBoxStaticBoundingBox_2;
    QTableWidget *pTableInputs;

    void setupUi(QWidget *EntityInputPanel)
    {
        if (EntityInputPanel->objectName().isEmpty())
            EntityInputPanel->setObjectName(QStringLiteral("EntityInputPanel"));
        EntityInputPanel->resize(340, 420);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EntityInputPanel->sizePolicy().hasHeightForWidth());
        EntityInputPanel->setSizePolicy(sizePolicy);
        EntityInputPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(EntityInputPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(EntityInputPanel);
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
        pGroupBoxStaticBoundingBox_2 = new QGroupBox(qEditorPanelBox);
        pGroupBoxStaticBoundingBox_2->setObjectName(QStringLiteral("pGroupBoxStaticBoundingBox_2"));
        pGroupBoxStaticBoundingBox_2->setGeometry(QRect(10, 40, 321, 141));
        pGroupBoxStaticBoundingBox_2->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pGroupBoxStaticBoundingBox_2->setCheckable(false);
        pGroupBoxStaticBoundingBox_2->setChecked(false);
        pTableInputs = new QTableWidget(pGroupBoxStaticBoundingBox_2);
        if (pTableInputs->columnCount() < 4)
            pTableInputs->setColumnCount(4);
        pTableInputs->setObjectName(QStringLiteral("pTableInputs"));
        pTableInputs->setGeometry(QRect(0, 20, 321, 121));
        pTableInputs->setSelectionMode(QAbstractItemView::SingleSelection);
        pTableInputs->setColumnCount(4);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(EntityInputPanel);

        QMetaObject::connectSlotsByName(EntityInputPanel);
    } // setupUi

    void retranslateUi(QWidget *EntityInputPanel)
    {
        EntityInputPanel->setWindowTitle(QApplication::translate("EntityInputPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("EntityInputPanel", "EventSys: Inputs", 0)));
        pLabelName->setText(QString());
        pGroupBoxStaticBoundingBox_2->setTitle(QApplication::translate("EntityInputPanel", "Inputs", 0));
    } // retranslateUi

};

namespace Ui {
    class EntityInputPanel: public Ui_EntityInputPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ENTITYINPUTPANEL_UI_H
