/********************************************************************************
** Form generated from reading UI file 'BoxParticleEmitterEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BOXPARTICLEEMITTERENTITYPANEL_UI_H
#define BOXPARTICLEEMITTERENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_BoxParticleEmitterEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *pGroupBoxDistort;
    QLabel *label_18;
    QLabel *label_24;
    QLabel *label_25;
    QLabel *label_19;
    QSpinBox *pSpinBoxEmitterMinX;
    QSpinBox *pSpinBoxEmitterMinY;
    QSpinBox *pSpinBoxEmitterMinZ;
    QSpinBox *pSpinBoxEmitterMaxX;
    QSpinBox *pSpinBoxEmitterMaxY;
    QSpinBox *pSpinBoxEmitterMaxZ;
    QLabel *label_26;
    QCheckBox *pCheckBoxAlignScreenX;
    QCheckBox *pCheckBoxAlignScreenY;

    void setupUi(QWidget *BoxParticleEmitterEntityPanel)
    {
        if (BoxParticleEmitterEntityPanel->objectName().isEmpty())
            BoxParticleEmitterEntityPanel->setObjectName(QStringLiteral("BoxParticleEmitterEntityPanel"));
        BoxParticleEmitterEntityPanel->resize(270, 319);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BoxParticleEmitterEntityPanel->sizePolicy().hasHeightForWidth());
        BoxParticleEmitterEntityPanel->setSizePolicy(sizePolicy);
        BoxParticleEmitterEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(BoxParticleEmitterEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(BoxParticleEmitterEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 200));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        pGroupBoxDistort = new QGroupBox(qEditorPanelBox);
        pGroupBoxDistort->setObjectName(QStringLiteral("pGroupBoxDistort"));
        pGroupBoxDistort->setGeometry(QRect(10, 50, 251, 131));
        pGroupBoxDistort->setStyleSheet(QLatin1String("QGroupBox\n"
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
        label_18 = new QLabel(pGroupBoxDistort);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(30, 20, 31, 21));
        label_24 = new QLabel(pGroupBoxDistort);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(10, 20, 21, 21));
        label_25 = new QLabel(pGroupBoxDistort);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(10, 50, 21, 21));
        label_19 = new QLabel(pGroupBoxDistort);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(30, 50, 31, 21));
        pSpinBoxEmitterMinX = new QSpinBox(pGroupBoxDistort);
        pSpinBoxEmitterMinX->setObjectName(QStringLiteral("pSpinBoxEmitterMinX"));
        pSpinBoxEmitterMinX->setGeometry(QRect(70, 20, 51, 22));
        pSpinBoxEmitterMinX->setMinimum(-10000);
        pSpinBoxEmitterMinX->setMaximum(10000);
        pSpinBoxEmitterMinX->setSingleStep(10);
        pSpinBoxEmitterMinY = new QSpinBox(pGroupBoxDistort);
        pSpinBoxEmitterMinY->setObjectName(QStringLiteral("pSpinBoxEmitterMinY"));
        pSpinBoxEmitterMinY->setGeometry(QRect(130, 20, 51, 22));
        pSpinBoxEmitterMinY->setMinimum(-10000);
        pSpinBoxEmitterMinY->setMaximum(10000);
        pSpinBoxEmitterMinY->setSingleStep(10);
        pSpinBoxEmitterMinZ = new QSpinBox(pGroupBoxDistort);
        pSpinBoxEmitterMinZ->setObjectName(QStringLiteral("pSpinBoxEmitterMinZ"));
        pSpinBoxEmitterMinZ->setGeometry(QRect(190, 20, 51, 22));
        pSpinBoxEmitterMinZ->setMinimum(-10000);
        pSpinBoxEmitterMinZ->setMaximum(10000);
        pSpinBoxEmitterMinZ->setSingleStep(10);
        pSpinBoxEmitterMaxX = new QSpinBox(pGroupBoxDistort);
        pSpinBoxEmitterMaxX->setObjectName(QStringLiteral("pSpinBoxEmitterMaxX"));
        pSpinBoxEmitterMaxX->setGeometry(QRect(70, 50, 51, 22));
        pSpinBoxEmitterMaxX->setMinimum(-10000);
        pSpinBoxEmitterMaxX->setMaximum(10000);
        pSpinBoxEmitterMaxX->setSingleStep(10);
        pSpinBoxEmitterMaxY = new QSpinBox(pGroupBoxDistort);
        pSpinBoxEmitterMaxY->setObjectName(QStringLiteral("pSpinBoxEmitterMaxY"));
        pSpinBoxEmitterMaxY->setGeometry(QRect(130, 50, 51, 22));
        pSpinBoxEmitterMaxY->setMinimum(-10000);
        pSpinBoxEmitterMaxY->setMaximum(10000);
        pSpinBoxEmitterMaxY->setSingleStep(10);
        pSpinBoxEmitterMaxZ = new QSpinBox(pGroupBoxDistort);
        pSpinBoxEmitterMaxZ->setObjectName(QStringLiteral("pSpinBoxEmitterMaxZ"));
        pSpinBoxEmitterMaxZ->setGeometry(QRect(190, 50, 51, 22));
        pSpinBoxEmitterMaxZ->setMinimum(-10000);
        pSpinBoxEmitterMaxZ->setMaximum(10000);
        pSpinBoxEmitterMaxZ->setSingleStep(10);
        label_26 = new QLabel(pGroupBoxDistort);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(10, 80, 81, 21));
        pCheckBoxAlignScreenX = new QCheckBox(pGroupBoxDistort);
        pCheckBoxAlignScreenX->setObjectName(QStringLiteral("pCheckBoxAlignScreenX"));
        pCheckBoxAlignScreenX->setGeometry(QRect(90, 80, 91, 17));
        pCheckBoxAlignScreenY = new QCheckBox(pGroupBoxDistort);
        pCheckBoxAlignScreenY->setObjectName(QStringLiteral("pCheckBoxAlignScreenY"));
        pCheckBoxAlignScreenY->setGeometry(QRect(90, 100, 71, 16));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(BoxParticleEmitterEntityPanel);

        QMetaObject::connectSlotsByName(BoxParticleEmitterEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *BoxParticleEmitterEntityPanel)
    {
        BoxParticleEmitterEntityPanel->setWindowTitle(QApplication::translate("BoxParticleEmitterEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("BoxParticleEmitterEntityPanel", "Box Particle Emitter", 0)));
        pLabelName->setText(QString());
        pGroupBoxDistort->setTitle(QApplication::translate("BoxParticleEmitterEntityPanel", "Dimensions", 0));
        label_18->setText(QApplication::translate("BoxParticleEmitterEntityPanel", "[x y z]", 0));
#ifndef QT_NO_TOOLTIP
        label_24->setToolTip(QApplication::translate("BoxParticleEmitterEntityPanel", "[0, ...) Multiplier for the reflected ray to compute the distorted particle color", 0));
#endif // QT_NO_TOOLTIP
        label_24->setText(QApplication::translate("BoxParticleEmitterEntityPanel", "Min", 0));
#ifndef QT_NO_TOOLTIP
        label_25->setToolTip(QApplication::translate("BoxParticleEmitterEntityPanel", "[0, ...) Multiplier for the reflected ray to compute the distorted particle color", 0));
#endif // QT_NO_TOOLTIP
        label_25->setText(QApplication::translate("BoxParticleEmitterEntityPanel", "Max", 0));
        label_19->setText(QApplication::translate("BoxParticleEmitterEntityPanel", "[x y z]", 0));
#ifndef QT_NO_TOOLTIP
        label_26->setToolTip(QApplication::translate("BoxParticleEmitterEntityPanel", "[0, ...) Multiplier for the reflected ray to compute the distorted particle color", 0));
#endif // QT_NO_TOOLTIP
        label_26->setText(QApplication::translate("BoxParticleEmitterEntityPanel", "Align to screen", 0));
        pCheckBoxAlignScreenX->setText(QApplication::translate("BoxParticleEmitterEntityPanel", "Horizontal", 0));
        pCheckBoxAlignScreenY->setText(QApplication::translate("BoxParticleEmitterEntityPanel", "Vertical", 0));
    } // retranslateUi

};

namespace Ui {
    class BoxParticleEmitterEntityPanel: public Ui_BoxParticleEmitterEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BOXPARTICLEEMITTERENTITYPANEL_UI_H
