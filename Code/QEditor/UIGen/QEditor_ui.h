/********************************************************************************
** Form generated from reading UI file 'QEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef QEDITOR_UI_H
#define QEDITOR_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtEditorClass
{
public:
    QAction *pActionMainNewLevel;
    QAction *pActionMainSaveLevel;
    QAction *pActionMainSaveLevelAs;
    QAction *pActionMainLoadLevel;
    QAction *pActionMainExit;
    QAction *m_pActionViewResetCamera;
    QAction *pActionToolsResizeLevel;
    QAction *pActionToolsOffsetLevel;
    QAction *pActionToolsEntityBrowser;
    QAction *actionGeneral_Sky_and_Fog;
    QAction *actionBorders_and_Layers;
    QAction *actionPost_Processing;
    QAction *actionWeather;
    QAction *actionEntities;
    QAction *pActionWindowBigPicture;
    QAction *pActionWindowSidePane;
    QAction *pActionWindowQuickAccessPane;
    QAction *pActionWindowFullscreen;
    QAction *pActionLevelSkyAndFog;
    QAction *actionTerrain;
    QAction *pActionToolsTerrain;
    QAction *pActionLevelEnvironmentAndTerrain;
    QAction *pActionLevelPostProcessing;
    QAction *pActionLightingBakeLighting;
    QAction *pActionToolsResetPainter;
    QAction *pActionToolsNodeBrowser;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuMain;
    QMenu *menuView;
    QMenu *menuTools;
    QMenu *menuWindow;
    QMenu *menuLEVEL;
    QMenu *menuLIGHTING;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *QtEditorClass)
    {
        if (QtEditorClass->objectName().isEmpty())
            QtEditorClass->setObjectName(QStringLiteral("QtEditorClass"));
        QtEditorClass->resize(929, 560);
        QtEditorClass->setWindowTitle(QStringLiteral("QEditor"));
        QtEditorClass->setAutoFillBackground(false);
        QtEditorClass->setStyleSheet(QStringLiteral(""));
        QtEditorClass->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        pActionMainNewLevel = new QAction(QtEditorClass);
        pActionMainNewLevel->setObjectName(QStringLiteral("pActionMainNewLevel"));
        pActionMainNewLevel->setEnabled(true);
        pActionMainNewLevel->setPriority(QAction::NormalPriority);
        pActionMainSaveLevel = new QAction(QtEditorClass);
        pActionMainSaveLevel->setObjectName(QStringLiteral("pActionMainSaveLevel"));
        pActionMainSaveLevelAs = new QAction(QtEditorClass);
        pActionMainSaveLevelAs->setObjectName(QStringLiteral("pActionMainSaveLevelAs"));
        pActionMainLoadLevel = new QAction(QtEditorClass);
        pActionMainLoadLevel->setObjectName(QStringLiteral("pActionMainLoadLevel"));
        pActionMainExit = new QAction(QtEditorClass);
        pActionMainExit->setObjectName(QStringLiteral("pActionMainExit"));
        m_pActionViewResetCamera = new QAction(QtEditorClass);
        m_pActionViewResetCamera->setObjectName(QStringLiteral("m_pActionViewResetCamera"));
        pActionToolsResizeLevel = new QAction(QtEditorClass);
        pActionToolsResizeLevel->setObjectName(QStringLiteral("pActionToolsResizeLevel"));
        pActionToolsOffsetLevel = new QAction(QtEditorClass);
        pActionToolsOffsetLevel->setObjectName(QStringLiteral("pActionToolsOffsetLevel"));
        pActionToolsEntityBrowser = new QAction(QtEditorClass);
        pActionToolsEntityBrowser->setObjectName(QStringLiteral("pActionToolsEntityBrowser"));
        actionGeneral_Sky_and_Fog = new QAction(QtEditorClass);
        actionGeneral_Sky_and_Fog->setObjectName(QStringLiteral("actionGeneral_Sky_and_Fog"));
        actionBorders_and_Layers = new QAction(QtEditorClass);
        actionBorders_and_Layers->setObjectName(QStringLiteral("actionBorders_and_Layers"));
        actionPost_Processing = new QAction(QtEditorClass);
        actionPost_Processing->setObjectName(QStringLiteral("actionPost_Processing"));
        actionWeather = new QAction(QtEditorClass);
        actionWeather->setObjectName(QStringLiteral("actionWeather"));
        actionEntities = new QAction(QtEditorClass);
        actionEntities->setObjectName(QStringLiteral("actionEntities"));
        pActionWindowBigPicture = new QAction(QtEditorClass);
        pActionWindowBigPicture->setObjectName(QStringLiteral("pActionWindowBigPicture"));
        pActionWindowBigPicture->setCheckable(true);
        pActionWindowSidePane = new QAction(QtEditorClass);
        pActionWindowSidePane->setObjectName(QStringLiteral("pActionWindowSidePane"));
        pActionWindowSidePane->setCheckable(true);
        pActionWindowSidePane->setChecked(true);
        pActionWindowQuickAccessPane = new QAction(QtEditorClass);
        pActionWindowQuickAccessPane->setObjectName(QStringLiteral("pActionWindowQuickAccessPane"));
        pActionWindowQuickAccessPane->setCheckable(true);
        pActionWindowQuickAccessPane->setChecked(true);
        pActionWindowFullscreen = new QAction(QtEditorClass);
        pActionWindowFullscreen->setObjectName(QStringLiteral("pActionWindowFullscreen"));
        pActionWindowFullscreen->setCheckable(true);
        pActionLevelSkyAndFog = new QAction(QtEditorClass);
        pActionLevelSkyAndFog->setObjectName(QStringLiteral("pActionLevelSkyAndFog"));
        actionTerrain = new QAction(QtEditorClass);
        actionTerrain->setObjectName(QStringLiteral("actionTerrain"));
        pActionToolsTerrain = new QAction(QtEditorClass);
        pActionToolsTerrain->setObjectName(QStringLiteral("pActionToolsTerrain"));
        pActionLevelEnvironmentAndTerrain = new QAction(QtEditorClass);
        pActionLevelEnvironmentAndTerrain->setObjectName(QStringLiteral("pActionLevelEnvironmentAndTerrain"));
        pActionLevelPostProcessing = new QAction(QtEditorClass);
        pActionLevelPostProcessing->setObjectName(QStringLiteral("pActionLevelPostProcessing"));
        pActionLightingBakeLighting = new QAction(QtEditorClass);
        pActionLightingBakeLighting->setObjectName(QStringLiteral("pActionLightingBakeLighting"));
        pActionToolsResetPainter = new QAction(QtEditorClass);
        pActionToolsResetPainter->setObjectName(QStringLiteral("pActionToolsResetPainter"));
        pActionToolsNodeBrowser = new QAction(QtEditorClass);
        pActionToolsNodeBrowser->setObjectName(QStringLiteral("pActionToolsNodeBrowser"));
        centralWidget = new QWidget(QtEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QtEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 929, 29));
        QFont font;
        font.setFamily(QStringLiteral("Calibri"));
        font.setPointSize(11);
        font.setBold(false);
        font.setWeight(50);
        menuBar->setFont(font);
        menuBar->setAutoFillBackground(false);
        menuBar->setStyleSheet(QLatin1String("QMenuBar {\n"
"    background-color: #555a63;\n"
"	border: 0px;\n"
"	color: #EEEEEE;\n"
"	padding:5px;\n"
"\n"
"}\n"
"QMenuBar::item {\n"
"    border-radius: 4px;\n"
"	color: #EEEEEE;\n"
"    padding-left:10px;\n"
"    padding-right:10px;\n"
"}\n"
"\n"
"QMenuBar::item:pressed {\n"
"    border-radius: 4px;\n"
"	color: #555a63;\n"
"   background-color: #EEEEEE;\n"
"}\n"
"\n"
"QMenuBar::item:selected {\n"
"    border-radius: 0px;\n"
"    background-color: #6e788a;\n"
"	color: #EEEEEE;\n"
"}\n"
""));
        menuBar->setNativeMenuBar(false);
        menuMain = new QMenu(menuBar);
        menuMain->setObjectName(QStringLiteral("menuMain"));
        menuMain->setAutoFillBackground(false);
        menuMain->setStyleSheet(QLatin1String("QMenu\n"
"{\n"
"    background-color: #EEEEEE;\n"
"	border: 0px solid #555555;\n"
"	color: #333333;\n"
"	padding:2px;\n"
"	padding-left:5px;\n"
"}\n"
"\n"
"QMenu::item:selected\n"
"{\n"
"    padding:0px;\n"
"    padding-left:2px;\n"
"    background-color: #6e788a;\n"
"	color: #EEEEEE;\n"
"}"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuView->setStyleSheet(QLatin1String("QMenu\n"
"{\n"
"    background-color: #EEEEEE;\n"
"	border: 0px solid #555555;\n"
"	color: #333333;\n"
"	padding:2px;\n"
"	padding-left:5px;\n"
"}\n"
"\n"
"QMenu::item:selected\n"
"{\n"
"    padding:0px;\n"
"    padding-left:2px;\n"
"    background-color: #6e788a;\n"
"	color: #EEEEEE;\n"
"}"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        menuTools->setStyleSheet(QLatin1String("QMenu\n"
"{\n"
"    background-color: #EEEEEE;\n"
"	border: 0px solid #555555;\n"
"	color: #333333;\n"
"	padding:2px;\n"
"	padding-left:5px;\n"
"}\n"
"\n"
"QMenu::item:selected\n"
"{\n"
"    padding:0px;\n"
"    padding-left:2px;\n"
"    background-color: #6e788a;\n"
"	color: #EEEEEE;\n"
"}"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QStringLiteral("menuWindow"));
        menuWindow->setStyleSheet(QLatin1String("QMenu\n"
"{\n"
"    background-color: #EEEEEE;\n"
"	border: 0px solid #555555;\n"
"	color: #333333;\n"
"	padding:2px;\n"
"	padding-left:5px;\n"
"}\n"
"\n"
"QMenu::item:selected\n"
"{\n"
"    padding:0px;\n"
"    padding-left:2px;\n"
"    background-color: #6e788a;\n"
"	color: #EEEEEE;\n"
"}"));
        menuLEVEL = new QMenu(menuBar);
        menuLEVEL->setObjectName(QStringLiteral("menuLEVEL"));
        menuLEVEL->setStyleSheet(QLatin1String("QMenu\n"
"{\n"
"    background-color: #EEEEEE;\n"
"	border: 0px solid #555555;\n"
"	color: #333333;\n"
"	padding:2px;\n"
"	padding-left:5px;\n"
"}\n"
"\n"
"QMenu::item:selected\n"
"{\n"
"    padding:0px;\n"
"    padding-left:2px;\n"
"    background-color: #6e788a;\n"
"	color: #EEEEEE;\n"
"}"));
        menuLIGHTING = new QMenu(menuBar);
        menuLIGHTING->setObjectName(QStringLiteral("menuLIGHTING"));
        QtEditorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(QtEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtEditorClass->setStatusBar(statusBar);
        mainToolBar = new QToolBar(QtEditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QFont font1;
        font1.setFamily(QStringLiteral("Calibri"));
        mainToolBar->setFont(font1);
        mainToolBar->setAutoFillBackground(false);
        mainToolBar->setStyleSheet(QLatin1String("\n"
"\n"
"background: #555a63;\n"
"border:1px solid #6e788a;\n"
"margin:1px;\n"
"color:white;\n"
"\n"
"font-size:13px;\n"
"font-family:Calibri;\n"
"\n"
"padding:3px;\n"
"color:white;\n"
"\n"
"border:1px solid #6e788a;\n"
"\n"
"QToolButton\n"
"{\n"
"	border: 2px solid red;\n"
"}\n"
""));
        mainToolBar->setMovable(false);
        mainToolBar->setFloatable(false);
        QtEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuMain->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuLEVEL->menuAction());
        menuBar->addAction(menuLIGHTING->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuMain->addAction(pActionMainNewLevel);
        menuMain->addAction(pActionMainSaveLevel);
        menuMain->addAction(pActionMainSaveLevelAs);
        menuMain->addAction(pActionMainLoadLevel);
        menuMain->addAction(pActionMainExit);
        menuView->addAction(m_pActionViewResetCamera);
        menuView->addSeparator();
        menuTools->addAction(pActionToolsResizeLevel);
        menuTools->addAction(pActionToolsOffsetLevel);
        menuTools->addSeparator();
        menuTools->addAction(pActionToolsEntityBrowser);
        menuTools->addAction(pActionToolsNodeBrowser);
        menuTools->addSeparator();
        menuTools->addAction(pActionToolsTerrain);
        menuTools->addSeparator();
        menuTools->addAction(pActionToolsResetPainter);
        menuWindow->addAction(pActionWindowFullscreen);
        menuWindow->addAction(pActionWindowBigPicture);
        menuWindow->addAction(pActionWindowSidePane);
        menuWindow->addAction(pActionWindowQuickAccessPane);
        menuLEVEL->addAction(pActionLevelEnvironmentAndTerrain);
        menuLEVEL->addAction(pActionLevelPostProcessing);
        menuLIGHTING->addAction(pActionLightingBakeLighting);

        retranslateUi(QtEditorClass);

        QMetaObject::connectSlotsByName(QtEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtEditorClass)
    {
        pActionMainNewLevel->setText(QApplication::translate("QtEditorClass", "New Level", 0));
        pActionMainSaveLevel->setText(QApplication::translate("QtEditorClass", "Save Level", 0));
        pActionMainSaveLevelAs->setText(QApplication::translate("QtEditorClass", "Save Level As", 0));
        pActionMainLoadLevel->setText(QApplication::translate("QtEditorClass", "Load Level", 0));
        pActionMainExit->setText(QApplication::translate("QtEditorClass", "Exit", 0));
        m_pActionViewResetCamera->setText(QApplication::translate("QtEditorClass", "Reset Camera", 0));
        pActionToolsResizeLevel->setText(QApplication::translate("QtEditorClass", "Resize Level", 0));
        pActionToolsOffsetLevel->setText(QApplication::translate("QtEditorClass", "Offset Level", 0));
        pActionToolsEntityBrowser->setText(QApplication::translate("QtEditorClass", "Entity Browser", 0));
        actionGeneral_Sky_and_Fog->setText(QApplication::translate("QtEditorClass", "General, Sky and Fog", 0));
        actionBorders_and_Layers->setText(QApplication::translate("QtEditorClass", "Borders and Layers", 0));
        actionPost_Processing->setText(QApplication::translate("QtEditorClass", "Post Processing", 0));
        actionWeather->setText(QApplication::translate("QtEditorClass", "Weather", 0));
        actionEntities->setText(QApplication::translate("QtEditorClass", "Entities", 0));
        pActionWindowBigPicture->setText(QApplication::translate("QtEditorClass", "     Big Picture", 0));
        pActionWindowSidePane->setText(QApplication::translate("QtEditorClass", "     Side Pane", 0));
        pActionWindowQuickAccessPane->setText(QApplication::translate("QtEditorClass", "     Quick Access Pane", 0));
        pActionWindowFullscreen->setText(QApplication::translate("QtEditorClass", "     Fullscreen", 0));
        pActionLevelSkyAndFog->setText(QApplication::translate("QtEditorClass", "Sky and Fog", 0));
        actionTerrain->setText(QApplication::translate("QtEditorClass", "Terrain", 0));
        pActionToolsTerrain->setText(QApplication::translate("QtEditorClass", "Terrain", 0));
        pActionLevelEnvironmentAndTerrain->setText(QApplication::translate("QtEditorClass", "Environment and Terrain", 0));
        pActionLevelPostProcessing->setText(QApplication::translate("QtEditorClass", "Post Processing", 0));
        pActionLightingBakeLighting->setText(QApplication::translate("QtEditorClass", "Bake Lighting", 0));
        pActionToolsResetPainter->setText(QApplication::translate("QtEditorClass", "Reset Painter", 0));
        pActionToolsNodeBrowser->setText(QApplication::translate("QtEditorClass", "Node Browser", 0));
        menuMain->setTitle(QApplication::translate("QtEditorClass", "MAIN", 0));
        menuView->setTitle(QApplication::translate("QtEditorClass", "VIEW", 0));
        menuTools->setTitle(QApplication::translate("QtEditorClass", "TOOLS", 0));
        menuWindow->setTitle(QApplication::translate("QtEditorClass", "WINDOW", 0));
        menuLEVEL->setTitle(QApplication::translate("QtEditorClass", "LEVEL", 0));
        menuLIGHTING->setTitle(QApplication::translate("QtEditorClass", "LIGHTING", 0));
        Q_UNUSED(QtEditorClass);
    } // retranslateUi

};

namespace Ui {
    class QtEditorClass: public Ui_QtEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // QEDITOR_UI_H
