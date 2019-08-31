/********************************************************************************
** Form generated from reading UI file 'NewLevel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef NEWLEVEL_UI_H
#define NEWLEVEL_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_NewLevel
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *pSpinBoxCellsX;
    QDialogButtonBox *buttonBox;
    QSpinBox *pSpinBoxCellsY;

    void setupUi(QDialog *NewLevel)
    {
        if (NewLevel->objectName().isEmpty())
            NewLevel->setObjectName(QStringLiteral("NewLevel"));
        NewLevel->resize(211, 99);
        NewLevel->setStyleSheet(QLatin1String("QDialog\n"
"{\n"
"	background-color:#555a63; \n"
"	border:1px solid #6e788a;\n"
"}\n"
"\n"
"QLabel\n"
"{\n"
"	background-color:#555a63; \n"
"	color: #EEEEEE;\n"
"}\n"
"\n"
"QSpinBox\n"
"{\n"
"	background-color:#EEEEEE; \n"
"	border:1px solid #6e788a;\n"
"}"));
        formLayout = new QFormLayout(NewLevel);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label = new QLabel(NewLevel);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(NewLevel);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        pSpinBoxCellsX = new QSpinBox(NewLevel);
        pSpinBoxCellsX->setObjectName(QStringLiteral("pSpinBoxCellsX"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pSpinBoxCellsX->sizePolicy().hasHeightForWidth());
        pSpinBoxCellsX->setSizePolicy(sizePolicy1);
        pSpinBoxCellsX->setMaximumSize(QSize(16777215, 16777215));
        pSpinBoxCellsX->setMaximum(10000);
        pSpinBoxCellsX->setValue(50);

        formLayout->setWidget(0, QFormLayout::FieldRole, pSpinBoxCellsX);

        buttonBox = new QDialogButtonBox(NewLevel);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy2);
        buttonBox->setStyleSheet(QLatin1String("border: 1px solid #222222;\n"
"		border - bottom: 1px solid black;\n"
"		border - radius:0px;\n"
"		background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));\n"
"		text-align:left;\n"
"		padding-left:5px;\n"
"		padding-right:5px;\n"
"		color:#CCCCCC;\n"
"height:24px;"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        formLayout->setWidget(3, QFormLayout::FieldRole, buttonBox);

        pSpinBoxCellsY = new QSpinBox(NewLevel);
        pSpinBoxCellsY->setObjectName(QStringLiteral("pSpinBoxCellsY"));
        sizePolicy1.setHeightForWidth(pSpinBoxCellsY->sizePolicy().hasHeightForWidth());
        pSpinBoxCellsY->setSizePolicy(sizePolicy1);
        pSpinBoxCellsY->setMaximumSize(QSize(16777215, 16777215));
        pSpinBoxCellsY->setMaximum(10000);
        pSpinBoxCellsY->setValue(50);

        formLayout->setWidget(2, QFormLayout::FieldRole, pSpinBoxCellsY);


        retranslateUi(NewLevel);
        QObject::connect(buttonBox, SIGNAL(accepted()), NewLevel, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), NewLevel, SLOT(reject()));

        QMetaObject::connectSlotsByName(NewLevel);
    } // setupUi

    void retranslateUi(QDialog *NewLevel)
    {
        NewLevel->setWindowTitle(QApplication::translate("NewLevel", "Create New Level", 0));
        label->setText(QApplication::translate("NewLevel", "Cells X", 0));
        label_2->setText(QApplication::translate("NewLevel", "Cells Y", 0));
    } // retranslateUi

};

namespace Ui {
    class NewLevel: public Ui_NewLevel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // NEWLEVEL_UI_H
