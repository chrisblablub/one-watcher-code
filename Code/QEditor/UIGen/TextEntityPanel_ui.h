/********************************************************************************
** Form generated from reading UI file 'TextEntityPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef TEXTENTITYPANEL_UI_H
#define TEXTENTITYPANEL_UI_H

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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "QEditorPanelBox.h"

QT_BEGIN_NAMESPACE

class Ui_TextEntityPanel
{
public:
    QGridLayout *gridLayout;
    QEditorPanelBox *qEditorPanelBox;
    QLabel *pLabelName;
    QGroupBox *groupBox;
    QLabel *label_3;
    QRadioButton *pRadioBtnAlignLeft;
    QRadioButton *pRadioBtnAlignCenter;
    QRadioButton *pRadioBtnAlignRight;
    QGroupBox *groupBox_2;
    QCheckBox *pCheckBoxBold;
    QCheckBox *pCheckBoxItalic;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QPushButton *pBtnUpdate;
    QSpinBox *pSpinBoxSize;
    QComboBox *pComboFontFamilies;
    QGroupBox *groupBox_3;
    QLineEdit *pEditText;

    void setupUi(QWidget *TextEntityPanel)
    {
        if (TextEntityPanel->objectName().isEmpty())
            TextEntityPanel->setObjectName(QStringLiteral("TextEntityPanel"));
        TextEntityPanel->resize(270, 548);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TextEntityPanel->sizePolicy().hasHeightForWidth());
        TextEntityPanel->setSizePolicy(sizePolicy);
        TextEntityPanel->setMinimumSize(QSize(0, 200));
        gridLayout = new QGridLayout(TextEntityPanel);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        qEditorPanelBox = new QEditorPanelBox(TextEntityPanel);
        qEditorPanelBox->setObjectName(QStringLiteral("qEditorPanelBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(qEditorPanelBox->sizePolicy().hasHeightForWidth());
        qEditorPanelBox->setSizePolicy(sizePolicy1);
        qEditorPanelBox->setMinimumSize(QSize(0, 330));
        qEditorPanelBox->setStyleSheet(QStringLiteral(""));
        pLabelName = new QLabel(qEditorPanelBox);
        pLabelName->setObjectName(QStringLiteral("pLabelName"));
        pLabelName->setGeometry(QRect(100, 50, 151, 20));
        groupBox = new QGroupBox(qEditorPanelBox);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 50, 251, 51));
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
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 230, 64, 16));
        pRadioBtnAlignLeft = new QRadioButton(groupBox);
        pRadioBtnAlignLeft->setObjectName(QStringLiteral("pRadioBtnAlignLeft"));
        pRadioBtnAlignLeft->setGeometry(QRect(20, 20, 51, 17));
        pRadioBtnAlignCenter = new QRadioButton(groupBox);
        pRadioBtnAlignCenter->setObjectName(QStringLiteral("pRadioBtnAlignCenter"));
        pRadioBtnAlignCenter->setGeometry(QRect(90, 20, 61, 17));
        pRadioBtnAlignRight = new QRadioButton(groupBox);
        pRadioBtnAlignRight->setObjectName(QStringLiteral("pRadioBtnAlignRight"));
        pRadioBtnAlignRight->setGeometry(QRect(180, 20, 51, 17));
        groupBox_2 = new QGroupBox(qEditorPanelBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 110, 251, 131));
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
        pCheckBoxBold = new QCheckBox(groupBox_2);
        pCheckBoxBold->setObjectName(QStringLiteral("pCheckBoxBold"));
        pCheckBoxBold->setGeometry(QRect(200, 50, 41, 17));
        pCheckBoxItalic = new QCheckBox(groupBox_2);
        pCheckBoxItalic->setObjectName(QStringLiteral("pCheckBoxItalic"));
        pCheckBoxItalic->setGeometry(QRect(140, 50, 61, 17));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 50, 64, 16));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 21, 31, 20));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 70, 64, 16));
        pBtnUpdate = new QPushButton(groupBox_2);
        pBtnUpdate->setObjectName(QStringLiteral("pBtnUpdate"));
        pBtnUpdate->setGeometry(QRect(10, 100, 231, 23));
        pBtnUpdate->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:center;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        pSpinBoxSize = new QSpinBox(groupBox_2);
        pSpinBoxSize->setObjectName(QStringLiteral("pSpinBoxSize"));
        pSpinBoxSize->setGeometry(QRect(140, 70, 101, 22));
        pSpinBoxSize->setValue(32);
        pComboFontFamilies = new QComboBox(groupBox_2);
        pComboFontFamilies->setObjectName(QStringLiteral("pComboFontFamilies"));
        pComboFontFamilies->setEnabled(true);
        pComboFontFamilies->setGeometry(QRect(50, 20, 191, 22));
        pComboFontFamilies->setEditable(false);
        groupBox_3 = new QGroupBox(qEditorPanelBox);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 250, 251, 51));
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
        pEditText = new QLineEdit(groupBox_3);
        pEditText->setObjectName(QStringLiteral("pEditText"));
        pEditText->setGeometry(QRect(10, 20, 231, 21));

        gridLayout->addWidget(qEditorPanelBox, 0, 0, 1, 1);


        retranslateUi(TextEntityPanel);

        QMetaObject::connectSlotsByName(TextEntityPanel);
    } // setupUi

    void retranslateUi(QWidget *TextEntityPanel)
    {
        TextEntityPanel->setWindowTitle(QApplication::translate("TextEntityPanel", "TextEntityPanel", 0));
        qEditorPanelBox->setProperty("m_strTitle", QVariant(QApplication::translate("TextEntityPanel", "Text", 0)));
        pLabelName->setText(QString());
        groupBox->setTitle(QApplication::translate("TextEntityPanel", "Alignment", 0));
        label_3->setText(QApplication::translate("TextEntityPanel", "Trigger Delay", 0));
        pRadioBtnAlignLeft->setText(QApplication::translate("TextEntityPanel", "Left", 0));
        pRadioBtnAlignCenter->setText(QApplication::translate("TextEntityPanel", "Center", 0));
        pRadioBtnAlignRight->setText(QApplication::translate("TextEntityPanel", "Right", 0));
        groupBox_2->setTitle(QApplication::translate("TextEntityPanel", "Formatting", 0));
        pCheckBoxBold->setText(QApplication::translate("TextEntityPanel", "Bold", 0));
        pCheckBoxItalic->setText(QApplication::translate("TextEntityPanel", "Italic", 0));
        label_6->setText(QApplication::translate("TextEntityPanel", "Decoration", 0));
        label_7->setText(QApplication::translate("TextEntityPanel", "Font", 0));
        label_8->setText(QApplication::translate("TextEntityPanel", "Size", 0));
        pBtnUpdate->setText(QApplication::translate("TextEntityPanel", "Create Font Bitmap", 0));
        groupBox_3->setTitle(QApplication::translate("TextEntityPanel", "Text", 0));
    } // retranslateUi

};

namespace Ui {
    class TextEntityPanel: public Ui_TextEntityPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TEXTENTITYPANEL_UI_H
