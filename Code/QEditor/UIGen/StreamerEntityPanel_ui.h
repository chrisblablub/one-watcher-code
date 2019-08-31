/********************************************************************************
** Form generated from reading UI file 'StreamerEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef STREAMERENTITYPANEL_UI_H
#define STREAMERENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_StreamerEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *pSpinBoxPause;
    QDoubleSpinBox *pSpinBoxDuration;
    QGroupBox *groupBox;
    QRadioButton *pRadioBtnSmall;
    QRadioButton *pRadioBtnBig;

    void setupUi(QWidget *StreamerEntityPanel)
    {
        if (StreamerEntityPanel->objectName().isEmpty())
            StreamerEntityPanel->setObjectName(QStringLiteral("StreamerEntityPanel"));
        StreamerEntityPanel->resize(270, 246);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(StreamerEntityPanel->sizePolicy().hasHeightForWidth());
        StreamerEntityPanel->setSizePolicy(sizePolicy);
        StreamerEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(StreamerEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(StreamerEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 150));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        label_4 = new QLabel(qEditorPanelBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 100, 81, 16));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 120, 61, 16));
        pSpinBoxPause = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxPause->setObjectName(QStringLiteral("pSpinBoxPause"));
        pSpinBoxPause->setGeometry(QRect(150, 100, 101, 18));
        pSpinBoxPause->setMinimum(0);
        pSpinBoxPause->setMaximum(100000);
        pSpinBoxPause->setSingleStep(0.05);
        pSpinBoxDuration = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxDuration->setObjectName(QStringLiteral("pSpinBoxDuration"));
        pSpinBoxDuration->setGeometry(QRect(150, 120, 101, 18));
        pSpinBoxDuration->setMinimum(0);
        pSpinBoxDuration->setMaximum(100000);
        pSpinBoxDuration->setSingleStep(0.05);
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 40, 251, 51));
        groupBox->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pRadioBtnSmall = new QRadioButton(groupBox);
        pRadioBtnSmall->setObjectName(QStringLiteral("pRadioBtnSmall"));
        pRadioBtnSmall->setGeometry(QRect(40, 20, 82, 17));
        pRadioBtnBig = new QRadioButton(groupBox);
        pRadioBtnBig->setObjectName(QStringLiteral("pRadioBtnBig"));
        pRadioBtnBig->setGeometry(QRect(160, 20, 82, 17));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(StreamerEntityPanel);

        QMetaObject::connectSlotsByName(StreamerEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *StreamerEntityPanel)
    {
        StreamerEntityPanel->setWindowTitle(QApplication::translate("StreamerEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("StreamerEntityPanel", "Streamer", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("StreamerEntityPanel", "Pause", 0));
        label_5->setText(QApplication::translate("StreamerEntityPanel", "Duration", 0));
        groupBox->setTitle(QApplication::translate("StreamerEntityPanel", "Type", 0));
        pRadioBtnSmall->setText(QApplication::translate("StreamerEntityPanel", "Small", 0));
        pRadioBtnBig->setText(QApplication::translate("StreamerEntityPanel", "Big", 0));
    } // retranslateUi

};

namespace Ui {
    class StreamerEntityPanel: public Ui_StreamerEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // STREAMERENTITYPANEL_UI_H
