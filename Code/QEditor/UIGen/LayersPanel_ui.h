/********************************************************************************
** Form generated from reading UI file 'LayersPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LAYERSPANEL_UI_H
#define LAYERSPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_LayersPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *pGroupBoxColor_2;
    QLabel *label_2;
    QDoubleSpinBox *pSpinBoxEmissiveEnergy;
    QLabel *label_5;
    QPushButton *pBtnEmissiveColor;
    QSpinBox *pSpinBoxLayer;
    QDoubleSpinBox *pSpinBoxLayerDepth;
    QLabel *label_3;
    QPushButton *pBtnLayerColor;
    QLabel *label;
    QLabel *label_4;
    QPushButton *pBtnLayerApplyAll;
    QPushButton *pBtnEmissiveApplyAll;

    void setupUi(QWidget *LayersPanel)
    {
        if (LayersPanel->objectName().isEmpty())
            LayersPanel->setObjectName(QStringLiteral("LayersPanel"));
        LayersPanel->resize(340, 290);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LayersPanel->sizePolicy().hasHeightForWidth());
        LayersPanel->setSizePolicy(sizePolicy);
        LayersPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(LayersPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(LayersPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 220));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        pGroupBoxColor_2 = new QGroupBox(qEditorPanelBox);
        pGroupBoxColor_2->setObjectName(QStringLiteral("pGroupBoxColor_2"));
        pGroupBoxColor_2->setGeometry(QRect(10, 40, 321, 171));
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
        label_2 = new QLabel(pGroupBoxColor_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 91, 16));
        pSpinBoxEmissiveEnergy = new QDoubleSpinBox(pGroupBoxColor_2);
        pSpinBoxEmissiveEnergy->setObjectName(QStringLiteral("pSpinBoxEmissiveEnergy"));
        pSpinBoxEmissiveEnergy->setGeometry(QRect(210, 140, 101, 20));
        pSpinBoxEmissiveEnergy->setMinimum(0);
        pSpinBoxEmissiveEnergy->setMaximum(100000);
        pSpinBoxEmissiveEnergy->setSingleStep(10);
        label_5 = new QLabel(pGroupBoxColor_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 140, 81, 16));
        pBtnEmissiveColor = new QPushButton(pGroupBoxColor_2);
        pBtnEmissiveColor->setObjectName(QStringLiteral("pBtnEmissiveColor"));
        pBtnEmissiveColor->setGeometry(QRect(210, 110, 101, 23));
        pBtnEmissiveColor->setStyleSheet(QStringLiteral(""));
        pSpinBoxLayer = new QSpinBox(pGroupBoxColor_2);
        pSpinBoxLayer->setObjectName(QStringLiteral("pSpinBoxLayer"));
        pSpinBoxLayer->setGeometry(QRect(210, 20, 101, 21));
        pSpinBoxLayer->setMinimum(0);
        pSpinBoxLayer->setValue(0);
        pSpinBoxLayerDepth = new QDoubleSpinBox(pGroupBoxColor_2);
        pSpinBoxLayerDepth->setObjectName(QStringLiteral("pSpinBoxLayerDepth"));
        pSpinBoxLayerDepth->setGeometry(QRect(210, 50, 101, 21));
        pSpinBoxLayerDepth->setMinimum(0);
        pSpinBoxLayerDepth->setMaximum(100000);
        pSpinBoxLayerDepth->setSingleStep(10);
        label_3 = new QLabel(pGroupBoxColor_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 110, 71, 16));
        pBtnLayerColor = new QPushButton(pGroupBoxColor_2);
        pBtnLayerColor->setObjectName(QStringLiteral("pBtnLayerColor"));
        pBtnLayerColor->setGeometry(QRect(210, 80, 101, 23));
        pBtnLayerColor->setStyleSheet(QStringLiteral(""));
        label = new QLabel(pGroupBoxColor_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 80, 81, 16));
        label_4 = new QLabel(pGroupBoxColor_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 71, 16));
        pBtnLayerApplyAll = new QPushButton(pGroupBoxColor_2);
        pBtnLayerApplyAll->setObjectName(QStringLiteral("pBtnLayerApplyAll"));
        pBtnLayerApplyAll->setGeometry(QRect(140, 80, 61, 23));
        pBtnEmissiveApplyAll = new QPushButton(pGroupBoxColor_2);
        pBtnEmissiveApplyAll->setObjectName(QStringLiteral("pBtnEmissiveApplyAll"));
        pBtnEmissiveApplyAll->setGeometry(QRect(140, 110, 61, 51));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(LayersPanel);

        QMetaObject::connectSlotsByName(LayersPanel);
    } // setupUi

    void retranslateUi(QWidget *LayersPanel)
    {
        LayersPanel->setWindowTitle(QApplication::translate("LayersPanel", "LayersPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("LayersPanel", "Layers", 0)));
        pLabelName->setText(QString());
        pGroupBoxColor_2->setTitle(QApplication::translate("LayersPanel", "Properties", 0));
        label_2->setText(QApplication::translate("LayersPanel", "Layer Depth", 0));
        label_5->setText(QApplication::translate("LayersPanel", "Emissive Energy", 0));
        pBtnEmissiveColor->setText(QString());
        label_3->setText(QApplication::translate("LayersPanel", "Emissive Color", 0));
        pBtnLayerColor->setText(QString());
        label->setText(QApplication::translate("LayersPanel", "Diffuse Color", 0));
        label_4->setText(QApplication::translate("LayersPanel", "Layer", 0));
        pBtnLayerApplyAll->setText(QApplication::translate("LayersPanel", "Apply All", 0));
        pBtnEmissiveApplyAll->setText(QApplication::translate("LayersPanel", "Apply All", 0));
    } // retranslateUi

};

namespace Ui {
    class LayersPanel: public Ui_LayersPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LAYERSPANEL_UI_H
