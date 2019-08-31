/********************************************************************************
** Form generated from reading UI file 'EntityOutputPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ENTITYOUTPUTPANEL_UI_H
#define ENTITYOUTPUTPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_EntityOutputPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *pGroupBoxStaticBoundingBox_3;
    QDoubleSpinBox *pSpinBoxOutputDelay;
    QPushButton *pBtnEditOutput;
    QComboBox *pComboOutputEvent;
    QPushButton *pBtnAddOutput;
    QLabel *label;
    QComboBox *pComboOutputReceiver;
    QLabel *label_3;
    QPushButton *pBtnDeleteOutput;
    QLabel *label_2;
    QTableWidget *pTableOutputs;
    QScrollArea *pScrollAreaEventParams;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QDoubleSpinBox *pSpinBoxOffsetFrom;
    QDoubleSpinBox *pSpinBoxOffsetBy;
    QPushButton *pBtnOffsetOutputs;
    QLabel *label_8;
    QDoubleSpinBox *pSpinBoxDebugOffset;

    void setupUi(QWidget *EntityOutputPanel)
    {
        if (EntityOutputPanel->objectName().isEmpty())
            EntityOutputPanel->setObjectName(QStringLiteral("EntityOutputPanel"));
        EntityOutputPanel->resize(340, 877);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EntityOutputPanel->sizePolicy().hasHeightForWidth());
        EntityOutputPanel->setSizePolicy(sizePolicy);
        EntityOutputPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(EntityOutputPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(EntityOutputPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 600));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        pGroupBoxStaticBoundingBox_3 = new QGroupBox(qEditorPanelBox);
        pGroupBoxStaticBoundingBox_3->setObjectName(QStringLiteral("pGroupBoxStaticBoundingBox_3"));
        pGroupBoxStaticBoundingBox_3->setGeometry(QRect(10, 100, 321, 491));
        pGroupBoxStaticBoundingBox_3->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pGroupBoxStaticBoundingBox_3->setCheckable(false);
        pGroupBoxStaticBoundingBox_3->setChecked(false);
        pSpinBoxOutputDelay = new QDoubleSpinBox(pGroupBoxStaticBoundingBox_3);
        pSpinBoxOutputDelay->setObjectName(QStringLiteral("pSpinBoxOutputDelay"));
        pSpinBoxOutputDelay->setGeometry(QRect(240, 430, 71, 22));
        pSpinBoxOutputDelay->setMaximum(100000);
        pSpinBoxOutputDelay->setSingleStep(0.1);
        pBtnEditOutput = new QPushButton(pGroupBoxStaticBoundingBox_3);
        pBtnEditOutput->setObjectName(QStringLiteral("pBtnEditOutput"));
        pBtnEditOutput->setGeometry(QRect(9, 460, 141, 23));
        pBtnEditOutput->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pComboOutputEvent = new QComboBox(pGroupBoxStaticBoundingBox_3);
        pComboOutputEvent->setObjectName(QStringLiteral("pComboOutputEvent"));
        pComboOutputEvent->setGeometry(QRect(69, 290, 241, 22));
        pBtnAddOutput = new QPushButton(pGroupBoxStaticBoundingBox_3);
        pBtnAddOutput->setObjectName(QStringLiteral("pBtnAddOutput"));
        pBtnAddOutput->setGeometry(QRect(160, 460, 151, 23));
        pBtnAddOutput->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label = new QLabel(pGroupBoxStaticBoundingBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(9, 261, 47, 16));
        pComboOutputReceiver = new QComboBox(pGroupBoxStaticBoundingBox_3);
        pComboOutputReceiver->setObjectName(QStringLiteral("pComboOutputReceiver"));
        pComboOutputReceiver->setGeometry(QRect(69, 260, 241, 22));
        label_3 = new QLabel(pGroupBoxStaticBoundingBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(9, 430, 47, 16));
        pBtnDeleteOutput = new QPushButton(pGroupBoxStaticBoundingBox_3);
        pBtnDeleteOutput->setObjectName(QStringLiteral("pBtnDeleteOutput"));
        pBtnDeleteOutput->setGeometry(QRect(10, 230, 301, 23));
        pBtnDeleteOutput->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_2 = new QLabel(pGroupBoxStaticBoundingBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(9, 290, 47, 16));
        pTableOutputs = new QTableWidget(pGroupBoxStaticBoundingBox_3);
        if (pTableOutputs->columnCount() < 4)
            pTableOutputs->setColumnCount(4);
        pTableOutputs->setObjectName(QStringLiteral("pTableOutputs"));
        pTableOutputs->setGeometry(QRect(0, 20, 321, 201));
        pTableOutputs->setAlternatingRowColors(true);
        pTableOutputs->setSelectionMode(QAbstractItemView::SingleSelection);
        pTableOutputs->setSelectionBehavior(QAbstractItemView::SelectRows);
        pTableOutputs->setColumnCount(4);
        pScrollAreaEventParams = new QScrollArea(pGroupBoxStaticBoundingBox_3);
        pScrollAreaEventParams->setObjectName(QStringLiteral("pScrollAreaEventParams"));
        pScrollAreaEventParams->setGeometry(QRect(0, 330, 321, 91));
        pScrollAreaEventParams->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 319, 89));
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pScrollAreaEventParams->setWidget(scrollAreaWidgetContents);
        label_4 = new QLabel(pGroupBoxStaticBoundingBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 310, 81, 21));
        label_5 = new QLabel(qEditorPanelBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 40, 81, 16));
        label_6 = new QLabel(qEditorPanelBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 60, 31, 16));
        label_7 = new QLabel(qEditorPanelBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(140, 60, 31, 16));
        pSpinBoxOffsetFrom = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxOffsetFrom->setObjectName(QStringLiteral("pSpinBoxOffsetFrom"));
        pSpinBoxOffsetFrom->setGeometry(QRect(50, 60, 71, 18));
        pSpinBoxOffsetFrom->setMaximum(100000);
        pSpinBoxOffsetFrom->setSingleStep(0.1);
        pSpinBoxOffsetBy = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxOffsetBy->setObjectName(QStringLiteral("pSpinBoxOffsetBy"));
        pSpinBoxOffsetBy->setGeometry(QRect(160, 60, 71, 18));
        pSpinBoxOffsetBy->setMinimum(-100000);
        pSpinBoxOffsetBy->setMaximum(100000);
        pSpinBoxOffsetBy->setSingleStep(0.1);
        pBtnOffsetOutputs = new QPushButton(qEditorPanelBox);
        pBtnOffsetOutputs->setObjectName(QStringLiteral("pBtnOffsetOutputs"));
        pBtnOffsetOutputs->setGeometry(QRect(240, 60, 81, 21));
        pBtnOffsetOutputs->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label_8 = new QLabel(qEditorPanelBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 80, 81, 16));
        pSpinBoxDebugOffset = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxDebugOffset->setObjectName(QStringLiteral("pSpinBoxDebugOffset"));
        pSpinBoxDebugOffset->setGeometry(QRect(160, 80, 71, 18));
        pSpinBoxDebugOffset->setMaximum(100000);
        pSpinBoxDebugOffset->setSingleStep(0.1);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(EntityOutputPanel);

        QMetaObject::connectSlotsByName(EntityOutputPanel);
    } // setupUi

    void retranslateUi(QWidget *EntityOutputPanel)
    {
        EntityOutputPanel->setWindowTitle(QApplication::translate("EntityOutputPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("EntityOutputPanel", "EventSys: Outputs", 0)));
        pLabelName->setText(QString());
        pGroupBoxStaticBoundingBox_3->setTitle(QApplication::translate("EntityOutputPanel", "Outputs", 0));
        pBtnEditOutput->setText(QApplication::translate("EntityOutputPanel", "Edit Selection", 0));
        pBtnAddOutput->setText(QApplication::translate("EntityOutputPanel", "Add Output", 0));
        label->setText(QApplication::translate("EntityOutputPanel", "Receiver", 0));
        label_3->setText(QApplication::translate("EntityOutputPanel", "Delay", 0));
        pBtnDeleteOutput->setText(QApplication::translate("EntityOutputPanel", "Delete Selection", 0));
        label_2->setText(QApplication::translate("EntityOutputPanel", "Event", 0));
        label_4->setText(QApplication::translate("EntityOutputPanel", "Parameter:", 0));
        label_5->setText(QApplication::translate("EntityOutputPanel", "Offset Outputs", 0));
        label_6->setText(QApplication::translate("EntityOutputPanel", "From", 0));
        label_7->setText(QApplication::translate("EntityOutputPanel", "By", 0));
        pBtnOffsetOutputs->setText(QApplication::translate("EntityOutputPanel", "Edit Selection", 0));
        label_8->setText(QApplication::translate("EntityOutputPanel", "Debug Offset", 0));
    } // retranslateUi

};

namespace Ui {
    class EntityOutputPanel: public Ui_EntityOutputPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ENTITYOUTPUTPANEL_UI_H
