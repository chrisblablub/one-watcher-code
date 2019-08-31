/********************************************************************************
** Form generated from reading UI file 'EntityTriggerPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ENTITYTRIGGERPANEL_UI_H
#define ENTITYTRIGGERPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_EntityTriggerPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *pGroupBoxColor;
    QCheckBox *pCheckBoxFireOnStart;
    QCheckBox *pCheckBoxResetOnStart;
    QCheckBox *pCheckBoxFireOnce;
    QCheckBox *pCheckBoxFireOncePerHit;
    QGroupBox *pGroupBoxColor_2;
    QLabel *label_3;
    QDoubleSpinBox *pSpinBoxTriggerDelay;

    void setupUi(QWidget *EntityTriggerPanel)
    {
        if (EntityTriggerPanel->objectName().isEmpty())
            EntityTriggerPanel->setObjectName(QStringLiteral("EntityTriggerPanel"));
        EntityTriggerPanel->resize(340, 420);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EntityTriggerPanel->sizePolicy().hasHeightForWidth());
        EntityTriggerPanel->setSizePolicy(sizePolicy);
        EntityTriggerPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(EntityTriggerPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(EntityTriggerPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 180));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        pGroupBoxColor = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor->setObjectName(QStringLiteral("pGroupBoxColor"));
        pGroupBoxColor->setGeometry(QRect(10, 40, 321, 71));
        pGroupBoxColor->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pCheckBoxFireOnStart = new QCheckBox(pGroupBoxColor);
        pCheckBoxFireOnStart->setObjectName(QStringLiteral("pCheckBoxFireOnStart"));
        pCheckBoxFireOnStart->setGeometry(QRect(10, 20, 91, 17));
        pCheckBoxResetOnStart = new QCheckBox(pGroupBoxColor);
        pCheckBoxResetOnStart->setObjectName(QStringLiteral("pCheckBoxResetOnStart"));
        pCheckBoxResetOnStart->setGeometry(QRect(210, 20, 91, 17));
        pCheckBoxFireOnce = new QCheckBox(pGroupBoxColor);
        pCheckBoxFireOnce->setObjectName(QStringLiteral("pCheckBoxFireOnce"));
        pCheckBoxFireOnce->setGeometry(QRect(10, 40, 70, 17));
        pCheckBoxFireOncePerHit = new QCheckBox(pGroupBoxColor);
        pCheckBoxFireOncePerHit->setObjectName(QStringLiteral("pCheckBoxFireOncePerHit"));
        pCheckBoxFireOncePerHit->setGeometry(QRect(210, 40, 101, 17));
        pGroupBoxColor_2 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_2->setObjectName(QStringLiteral("pGroupBoxColor_2"));
        pGroupBoxColor_2->setGeometry(QRect(10, 120, 321, 51));
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
        label_3 = new QLabel(pGroupBoxColor_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 20, 64, 16));
        pSpinBoxTriggerDelay = new QDoubleSpinBox(pGroupBoxColor_2);
        pSpinBoxTriggerDelay->setObjectName(QStringLiteral("pSpinBoxTriggerDelay"));
        pSpinBoxTriggerDelay->setGeometry(QRect(210, 20, 101, 18));
        pSpinBoxTriggerDelay->setSingleStep(0.01);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(EntityTriggerPanel);

        QMetaObject::connectSlotsByName(EntityTriggerPanel);
    } // setupUi

    void retranslateUi(QWidget *EntityTriggerPanel)
    {
        EntityTriggerPanel->setWindowTitle(QApplication::translate("EntityTriggerPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("EntityTriggerPanel", "EventSys: Trigger ", 0)));
        pLabelName->setText(QString());
        pGroupBoxColor->setTitle(QApplication::translate("EntityTriggerPanel", "Flags", 0));
        pCheckBoxFireOnStart->setText(QApplication::translate("EntityTriggerPanel", "Fire On Start", 0));
        pCheckBoxResetOnStart->setText(QApplication::translate("EntityTriggerPanel", "Reset On Start", 0));
        pCheckBoxFireOnce->setText(QApplication::translate("EntityTriggerPanel", "Fire Once", 0));
        pCheckBoxFireOncePerHit->setText(QApplication::translate("EntityTriggerPanel", "Fire Once Per Hit", 0));
        pGroupBoxColor_2->setTitle(QApplication::translate("EntityTriggerPanel", "Properties", 0));
        label_3->setText(QApplication::translate("EntityTriggerPanel", "Trigger Delay", 0));
    } // retranslateUi

};

namespace Ui {
    class EntityTriggerPanel: public Ui_EntityTriggerPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ENTITYTRIGGERPANEL_UI_H
