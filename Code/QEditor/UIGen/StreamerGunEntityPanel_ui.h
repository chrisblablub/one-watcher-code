/********************************************************************************
** Form generated from reading UI file 'StreamerGunEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef STREAMERGUNENTITYPANEL_UI_H
#define STREAMERGUNENTITYPANEL_UI_H

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

class Ui_StreamerGunEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QDoubleSpinBox *pSpinBoxSpeed;
    QLabel *label_6;
    QLabel *label_7;
    QDoubleSpinBox *pSpinBoxLifetime;
    QLabel *label_8;
    QDoubleSpinBox *pSpinBoxFireRate;
    QDoubleSpinBox *pSpinBoxDelay;
    QLabel *label_9;

    void setupUi(QWidget *StreamerGunEntityPanel)
    {
        if (StreamerGunEntityPanel->objectName().isEmpty())
            StreamerGunEntityPanel->setObjectName(QStringLiteral("StreamerGunEntityPanel"));
        StreamerGunEntityPanel->resize(270, 508);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(StreamerGunEntityPanel->sizePolicy().hasHeightForWidth());
        StreamerGunEntityPanel->setSizePolicy(sizePolicy);
        StreamerGunEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(StreamerGunEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(StreamerGunEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 200));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pSpinBoxSpeed = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxSpeed->setObjectName(QStringLiteral("pSpinBoxSpeed"));
        pSpinBoxSpeed->setGeometry(QRect(141, 50, 111, 22));
        pSpinBoxSpeed->setMaximum(1e+6);
        pSpinBoxSpeed->setSingleStep(10);
        label_6 = new QLabel(qEditorPanelBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 50, 71, 16));
        label_7 = new QLabel(qEditorPanelBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(19, 80, 71, 16));
        pSpinBoxLifetime = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxLifetime->setObjectName(QStringLiteral("pSpinBoxLifetime"));
        pSpinBoxLifetime->setGeometry(QRect(140, 80, 111, 22));
        pSpinBoxLifetime->setMaximum(1e+6);
        pSpinBoxLifetime->setSingleStep(0.1);
        label_8 = new QLabel(qEditorPanelBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(19, 110, 71, 16));
        pSpinBoxFireRate = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxFireRate->setObjectName(QStringLiteral("pSpinBoxFireRate"));
        pSpinBoxFireRate->setGeometry(QRect(140, 110, 111, 22));
        pSpinBoxFireRate->setMaximum(1e+6);
        pSpinBoxFireRate->setSingleStep(0.1);
        pSpinBoxDelay = new QDoubleSpinBox(qEditorPanelBox);
        pSpinBoxDelay->setObjectName(QStringLiteral("pSpinBoxDelay"));
        pSpinBoxDelay->setGeometry(QRect(141, 140, 111, 22));
        pSpinBoxDelay->setMaximum(1e+6);
        pSpinBoxDelay->setSingleStep(0.1);
        label_9 = new QLabel(qEditorPanelBox);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 140, 71, 16));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(StreamerGunEntityPanel);

        QMetaObject::connectSlotsByName(StreamerGunEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *StreamerGunEntityPanel)
    {
        StreamerGunEntityPanel->setWindowTitle(QApplication::translate("StreamerGunEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("StreamerGunEntityPanel", "Exploding Hunter Emitter", 0)));
        label_6->setText(QApplication::translate("StreamerGunEntityPanel", "Speed", 0));
        label_7->setText(QApplication::translate("StreamerGunEntityPanel", "Lifetime", 0));
        label_8->setText(QApplication::translate("StreamerGunEntityPanel", "Fire rate", 0));
        label_9->setText(QApplication::translate("StreamerGunEntityPanel", "Delay", 0));
    } // retranslateUi

};

namespace Ui {
    class StreamerGunEntityPanel: public Ui_StreamerGunEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // STREAMERGUNENTITYPANEL_UI_H
