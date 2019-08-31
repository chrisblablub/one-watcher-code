/********************************************************************************
** Form generated from reading UI file 'SilhouettesPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SILHOUETTESPANEL_UI_H
#define SILHOUETTESPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_SilhouettesPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QLabel *label_4;
    QLabel *label;
    QCheckBox *pCheckBoxSilhouettes;
    QPushButton *pBtnShadingColor;
    QDoubleSpinBox *pSpinBoxShadingIntensity;

    void setupUi(QWidget *SilhouettesPanel)
    {
        if (SilhouettesPanel->objectName().isEmpty())
            SilhouettesPanel->setObjectName(QStringLiteral("SilhouettesPanel"));
        SilhouettesPanel->resize(270, 252);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SilhouettesPanel->sizePolicy().hasHeightForWidth());
        SilhouettesPanel->setSizePolicy(sizePolicy);
        SilhouettesPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(SilhouettesPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(SilhouettesPanel);
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
        label_4->setGeometry(QRect(20, 50, 101, 16));
        label = new QLabel(qEditorPanelBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 80, 111, 16));
        pCheckBoxSilhouettes = new QCheckBox(qEditorPanelBox);
        pCheckBoxSilhouettes->setObjectName(QStringLiteral("pCheckBoxSilhouettes"));
        pCheckBoxSilhouettes->setEnabled(false);
        pCheckBoxSilhouettes->setGeometry(QRect(150, 110, 91, 17));
        pCheckBoxSilhouettes->setCheckable(true);
        pCheckBoxSilhouettes->setTristate(false);
        pBtnShadingColor = new QPushButton(qEditorPanelBox);
        pBtnShadingColor->setObjectName(QStringLiteral("pBtnShadingColor"));
        pBtnShadingColor->setGeometry(QRect(150, 80, 101, 23));
        pBtnShadingColor->setStyleSheet(QLatin1String("		border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), 															 stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"		height:24px;"));
        pSpinBoxShadingIntensity = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxShadingIntensity->setObjectName(QStringLiteral("pSpinBoxShadingIntensity"));
        pSpinBoxShadingIntensity->setGeometry(QRect(150, 50, 101, 22));
        pSpinBoxShadingIntensity->setMinimum(0);
        pSpinBoxShadingIntensity->setMaximum(5);
        pSpinBoxShadingIntensity->setSingleStep(0.1);

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(SilhouettesPanel);

        QMetaObject::connectSlotsByName(SilhouettesPanel);
    } // setupUi

    void retranslateUi(QWidget *SilhouettesPanel)
    {
        SilhouettesPanel->setWindowTitle(QApplication::translate("SilhouettesPanel", "SilhouettesPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("SilhouettesPanel", "Silhouettes", 0)));
        pLabelName->setText(QString());
        label_4->setText(QApplication::translate("SilhouettesPanel", "Shading Intensity", 0));
        label->setText(QApplication::translate("SilhouettesPanel", "Shading Color", 0));
        pCheckBoxSilhouettes->setText(QApplication::translate("SilhouettesPanel", "Silhouettes", 0));
        pBtnShadingColor->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SilhouettesPanel: public Ui_SilhouettesPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SILHOUETTESPANEL_UI_H
