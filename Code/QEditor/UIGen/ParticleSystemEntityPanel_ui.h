/********************************************************************************
** Form generated from reading UI file 'ParticleSystemEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PARTICLESYSTEMENTITYPANEL_UI_H
#define PARTICLESYSTEMENTITYPANEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_ParticleSystemEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QComboBox *pComboBoxTemplate;
    QPushButton *pBtnLoadTemplate;
    QPushButton *pBtnTexture;
    QPushButton *pBtnSaveTemplate;
    QGroupBox *groupBox;
    QComboBox *pComboBoxEmittersAvailable;
    QPushButton *pBtnAttachEmitter;
    QPushButton *pBtnDetachEmitter;
    QPushButton *pBtnPickEmitter;
    QLabel *label;
    QComboBox *pComboBoxEmittersAttached;
    QLabel *label_2;
    QGroupBox *groupBox_2;
    QRadioButton *pRadioBtnRenderPassColor;
    QRadioButton *pRadioBtnRenderPassDistort;
    QLabel *label_3;
    QLabel *pLabelTemplateLoaded;
    QGroupBox *groupBox_3;
    QPushButton *pBtnAlignmentCenter;
    QPushButton *pBtnAlignmentTop;
    QPushButton *pBtnAlignmentBottom;
    QPushButton *pBtnAlignmentLeft;
    QPushButton *pBtnAlignmentRight;
    QCheckBox *pCheckBoxLoadFromTemplate;

    void setupUi(QWidget *ParticleSystemEntityPanel)
    {
        if (ParticleSystemEntityPanel->objectName().isEmpty())
            ParticleSystemEntityPanel->setObjectName(QStringLiteral("ParticleSystemEntityPanel"));
        ParticleSystemEntityPanel->resize(340, 772);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ParticleSystemEntityPanel->sizePolicy().hasHeightForWidth());
        ParticleSystemEntityPanel->setSizePolicy(sizePolicy);
        ParticleSystemEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(ParticleSystemEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(ParticleSystemEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 590));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        pComboBoxTemplate = new QComboBox(qEditorPanelBox);
        pComboBoxTemplate->setObjectName(QStringLiteral("pComboBoxTemplate"));
        pComboBoxTemplate->setGeometry(QRect(10, 40, 321, 22));
        pComboBoxTemplate->setEditable(true);
        pBtnLoadTemplate = new QPushButton(qEditorPanelBox);
        pBtnLoadTemplate->setObjectName(QStringLiteral("pBtnLoadTemplate"));
        pBtnLoadTemplate->setGeometry(QRect(180, 70, 151, 23));
        pBtnLoadTemplate->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pBtnTexture = new QPushButton(qEditorPanelBox);
        pBtnTexture->setObjectName(QStringLiteral("pBtnTexture"));
        pBtnTexture->setGeometry(QRect(10, 150, 321, 23));
        pBtnTexture->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pBtnSaveTemplate = new QPushButton(qEditorPanelBox);
        pBtnSaveTemplate->setObjectName(QStringLiteral("pBtnSaveTemplate"));
        pBtnSaveTemplate->setGeometry(QRect(10, 70, 161, 23));
        pBtnSaveTemplate->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 240, 321, 151));
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
        pComboBoxEmittersAvailable = new QComboBox(groupBox);
        pComboBoxEmittersAvailable->setObjectName(QStringLiteral("pComboBoxEmittersAvailable"));
        pComboBoxEmittersAvailable->setGeometry(QRect(60, 20, 251, 22));
        pBtnAttachEmitter = new QPushButton(groupBox);
        pBtnAttachEmitter->setObjectName(QStringLiteral("pBtnAttachEmitter"));
        pBtnAttachEmitter->setGeometry(QRect(60, 50, 251, 23));
        pBtnAttachEmitter->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pBtnDetachEmitter = new QPushButton(groupBox);
        pBtnDetachEmitter->setObjectName(QStringLiteral("pBtnDetachEmitter"));
        pBtnDetachEmitter->setGeometry(QRect(60, 110, 121, 23));
        pBtnDetachEmitter->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pBtnPickEmitter = new QPushButton(groupBox);
        pBtnPickEmitter->setObjectName(QStringLiteral("pBtnPickEmitter"));
        pBtnPickEmitter->setGeometry(QRect(190, 110, 121, 23));
        pBtnPickEmitter->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 47, 16));
        pComboBoxEmittersAttached = new QComboBox(groupBox);
        pComboBoxEmittersAttached->setObjectName(QStringLiteral("pComboBoxEmittersAttached"));
        pComboBoxEmittersAttached->setGeometry(QRect(58, 80, 251, 22));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 80, 47, 21));
        groupBox_2 = new QGroupBox(qEditorPanelBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 180, 321, 51));
        groupBox_2->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pRadioBtnRenderPassColor = new QRadioButton(groupBox_2);
        pRadioBtnRenderPassColor->setObjectName(QStringLiteral("pRadioBtnRenderPassColor"));
        pRadioBtnRenderPassColor->setGeometry(QRect(20, 20, 101, 17));
        pRadioBtnRenderPassDistort = new QRadioButton(groupBox_2);
        pRadioBtnRenderPassDistort->setObjectName(QStringLiteral("pRadioBtnRenderPassDistort"));
        pRadioBtnRenderPassDistort->setGeometry(QRect(250, 20, 61, 17));
        label_3 = new QLabel(qEditorPanelBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 100, 41, 21));
        pLabelTemplateLoaded = new QLabel(qEditorPanelBox);
        pLabelTemplateLoaded->setObjectName(QStringLiteral("pLabelTemplateLoaded"));
        pLabelTemplateLoaded->setGeometry(QRect(60, 100, 271, 21));
        groupBox_3 = new QGroupBox(qEditorPanelBox);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 400, 321, 171));
        groupBox_3->setStyleSheet(QLatin1String("QGroupBox\n"
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
        pBtnAlignmentCenter = new QPushButton(groupBox_3);
        pBtnAlignmentCenter->setObjectName(QStringLiteral("pBtnAlignmentCenter"));
        pBtnAlignmentCenter->setGeometry(QRect(90, 60, 131, 61));
        pBtnAlignmentCenter->setStyleSheet(QStringLiteral(""));
        pBtnAlignmentCenter->setCheckable(true);
        pBtnAlignmentTop = new QPushButton(groupBox_3);
        pBtnAlignmentTop->setObjectName(QStringLiteral("pBtnAlignmentTop"));
        pBtnAlignmentTop->setGeometry(QRect(90, 20, 131, 31));
        pBtnAlignmentTop->setStyleSheet(QStringLiteral(""));
        pBtnAlignmentTop->setCheckable(true);
        pBtnAlignmentBottom = new QPushButton(groupBox_3);
        pBtnAlignmentBottom->setObjectName(QStringLiteral("pBtnAlignmentBottom"));
        pBtnAlignmentBottom->setGeometry(QRect(90, 130, 131, 31));
        pBtnAlignmentBottom->setStyleSheet(QStringLiteral(""));
        pBtnAlignmentBottom->setCheckable(true);
        pBtnAlignmentLeft = new QPushButton(groupBox_3);
        pBtnAlignmentLeft->setObjectName(QStringLiteral("pBtnAlignmentLeft"));
        pBtnAlignmentLeft->setGeometry(QRect(50, 60, 31, 61));
        pBtnAlignmentLeft->setStyleSheet(QStringLiteral(""));
        pBtnAlignmentLeft->setCheckable(true);
        pBtnAlignmentRight = new QPushButton(groupBox_3);
        pBtnAlignmentRight->setObjectName(QStringLiteral("pBtnAlignmentRight"));
        pBtnAlignmentRight->setGeometry(QRect(230, 60, 31, 61));
        pBtnAlignmentRight->setStyleSheet(QStringLiteral(""));
        pBtnAlignmentRight->setCheckable(true);
        pCheckBoxLoadFromTemplate = new QCheckBox(qEditorPanelBox);
        pCheckBoxLoadFromTemplate->setObjectName(QStringLiteral("pCheckBoxLoadFromTemplate"));
        pCheckBoxLoadFromTemplate->setGeometry(QRect(20, 126, 121, 16));

        gridLayout->addWidget(qEditorPanelBox, 0, 1, 1, 1);


        retranslateUi(ParticleSystemEntityPanel);

        QMetaObject::connectSlotsByName(ParticleSystemEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *ParticleSystemEntityPanel)
    {
        ParticleSystemEntityPanel->setWindowTitle(QApplication::translate("ParticleSystemEntityPanel", "Form", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("ParticleSystemEntityPanel", "Particle System", 0)));
        pLabelName->setText(QString());
        pBtnLoadTemplate->setText(QApplication::translate("ParticleSystemEntityPanel", "Load Template", 0));
        pBtnTexture->setText(QApplication::translate("ParticleSystemEntityPanel", "Choose Texture", 0));
        pBtnSaveTemplate->setText(QApplication::translate("ParticleSystemEntityPanel", "Save Template", 0));
        groupBox->setTitle(QApplication::translate("ParticleSystemEntityPanel", "Emitters", 0));
        pBtnAttachEmitter->setText(QApplication::translate("ParticleSystemEntityPanel", "Attach", 0));
        pBtnDetachEmitter->setText(QApplication::translate("ParticleSystemEntityPanel", "Detach", 0));
        pBtnPickEmitter->setText(QApplication::translate("ParticleSystemEntityPanel", "Pick", 0));
        label->setText(QApplication::translate("ParticleSystemEntityPanel", "Available", 0));
        label_2->setText(QApplication::translate("ParticleSystemEntityPanel", "Attached", 0));
        groupBox_2->setTitle(QApplication::translate("ParticleSystemEntityPanel", "Renderpass", 0));
        pRadioBtnRenderPassColor->setText(QApplication::translate("ParticleSystemEntityPanel", "Color", 0));
        pRadioBtnRenderPassDistort->setText(QApplication::translate("ParticleSystemEntityPanel", "Distort", 0));
        label_3->setText(QApplication::translate("ParticleSystemEntityPanel", "Loaded:", 0));
        pLabelTemplateLoaded->setText(QApplication::translate("ParticleSystemEntityPanel", "<None Loaded>", 0));
        groupBox_3->setTitle(QApplication::translate("ParticleSystemEntityPanel", "Alignment", 0));
        pBtnAlignmentCenter->setText(QString());
        pBtnAlignmentTop->setText(QString());
        pBtnAlignmentBottom->setText(QString());
        pBtnAlignmentLeft->setText(QString());
        pBtnAlignmentRight->setText(QString());
        pCheckBoxLoadFromTemplate->setText(QApplication::translate("ParticleSystemEntityPanel", "Load From Template", 0));
    } // retranslateUi

};

namespace Ui {
    class ParticleSystemEntityPanel: public Ui_ParticleSystemEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PARTICLESYSTEMENTITYPANEL_UI_H
