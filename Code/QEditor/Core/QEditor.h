/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifndef QEDITOR_H
#define QEDITOR_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#define UNICODE 


#include <QEditor/Core/QRenderWidget.h>
#include <QEditor/UIGen/QEditor_ui.h>

class CPanelBase;

class QEditor : public QMainWindow
{
    Q_OBJECT

public:

    typedef std::pair< CPanelBase*, bool > pairPanelInitialVisibility;
    typedef std::map< std::string, pairPanelInitialVisibility > mapEntityPanels;
    typedef std::list< CPanelBase* > listEntityPanels;

    QEditor(QWidget *parent = 0);
    ~QEditor();

    bool InitializeEngine(QSplashScreen& splashScreen);

    void UpdateQuickAccessPanel(const QString& str);

    bool InitializeEntities(QSplashScreen& splashScreen);

    bool InitializeEntityPanels(QSplashScreen& splashScreen);

    bool Update();

    void AcquireColorDialog(QObject* pReceiver, const char* pSlot, const env::Vec3& initialColor, const char* pTitle);
    void AcquireColorDialog(QObject* pReceiver, const char* pSlot, const env::Vec4& initialColor, const char* pTitle);

    void ResetColorDialog();


signals:
    void togglePanel(bool);

public slots:

    virtual void slotEntitySearchEntered(const QString&);
    virtual void slotEntityBtnClicked();
    virtual void slotMenuMainNewTriggered();
    virtual void slotMenuMainSaveLevelTriggered();
    virtual void slotMenuMainSaveLevelAsTriggered();
    virtual void slotMenuMainLoadLevelTriggered();
    virtual void slotMenuMainExitTriggered();
    virtual void slotMenuViewResetCameraTriggered();
    virtual void slotMenuToolsResizeLevelTriggered();
    virtual void slotMenuToolsOffsetLevelTriggered();
    virtual void slotMenuToolsTerrainTriggered();
    virtual void slotMenuToolsResetPainterTriggered();
    virtual void slotMenuToolsEntityBrowserTriggered();
    virtual void slotMenuToolsNodeBrowserTriggered();
    virtual void slotMenuLightingBakeLighting();
    virtual void slotMenuWindowSidePaneToggled(bool bToggeld);
    virtual void slotMenuWindowQuickAccessPaneToggled(bool bToggeld);
    virtual void slotMenuWindowFullscreenToggled(bool bToggeld);
    virtual void slotMenuWindowBigPictureToggled(bool bToggeld);
    virtual void slotToolBarShowGridToggled(bool bToggeld);
    virtual void slotToolBarPickEntityPressed();
    virtual void slotToolBarSelectTerrainPressed();
    virtual void slotToolBarEditTerrainPressed();
    virtual void slotToolBarEditTerrainDiffusePressed();
    virtual void slotToolBarEditTerrainEmissivePressed();
    virtual void slotToolBarPickTerrainPressed();
    virtual void slotToolBarPaintTerrainPressed();
    virtual void slotToolBarPickFilterAllPressed();
    virtual void slotToolBarPickFilterEntitiesPressed();
    virtual void slotToolBarPickFilterMeshesPressed();
    virtual void slotToolBarPickFilterSpritesPressed();

    virtual void slotToolBarSaveTerrainSelection();
    virtual void slotToolBarLoadTerrainSelection();

    virtual void slotMenuLevelEnvironmentAndTerrain();
    virtual void slotMenuLevelPostProcessing();
    virtual void slotLightingModeIndexChanged(int index);
    virtual void applicationStateChanged(Qt::ApplicationState state);
    

protected:

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

    void resizeEvent(QResizeEvent * event);
    void closeEvent(QCloseEvent *event);


private:

    void initSidePanel();
    void initQuickAccessPanel();
    void initCenterWidget(QWidget* pParent);
    void initStatusBar();
    void initMenuBar();
    void initConnections();

    template< class T >
    void registerPanel(QSplashScreen& splatScreen, const std::string& strPanelName, bool bInitialVisibility = true);
    template< class T >
    void registerPanel(QSplashScreen& splatScreen, env::ENTITY_TYPEID entityTypeID, bool bInitialVisibility = true);
    CPanelBase* addEntityPanel(const std::string& strPanelID, bool bInitial = false);
    void syncPanels();
    void clearPanel();

    void UncheckActionsToolbar(QAction* pAction);

private:
    Ui::QtEditorClass ui;

    QWidget*        m_pParentWidget;
    QWidget*        m_pSidePanelWidget;
    QScrollArea*    m_pSidePanelScrollArea;
    QVBoxLayout*    m_pSidePanelLayout;

    QRenderWidget*  m_pRenderWidget;

    QWidget*        m_pQuickAccessWidget;
    QScrollArea*    m_pQuickAccessScrollArea;
    QVBoxLayout*    m_pQuickAccessLayout;

    QSplitter*      m_pMainSplitter;

    QAction*        m_pActionShowGrid;
    QAction*        m_pActionPickEntities;
    QAction*        m_pActionPaintTerrain;
    QAction*        m_pActionEditTerrain;
    QAction*        m_pActionEditTerrainDiffuse;
    QAction*        m_pActionEditTerrainEmissive;
    QAction*        m_pActionPickTerrain;

    QAction*        m_pActionSelectTerrain;
    QAction*        m_pActionSaveTerrainSelection;
    QAction*        m_pActionLoadTerrainSelection;
    QAction*        m_pActionPickFilterAll;
    QAction*        m_pActionPickFilterEntities;
    QAction*        m_pActionPickFilterMeshes;
    QAction*        m_pActionPickFilterSprites;

    QList<QAction*> m_actionsToolBar;

    QComboBox*      m_pComboLighting;

    QLabel*         m_pLabelStatusBarLevel;
    QColorDialog*   m_pColorDialog;
    QObject*        m_pColorDialogReceiver;
    const char*     m_pColorDialogSlot;

    env::CEngine*   m_pEngine;
    env::CEntity*   m_pActiveEntity;


    mapEntityPanels m_entityPanels;

    listEntityPanels m_activeEntityPanels;

    bool            m_bQuit;

};

extern QEditor* gEditor;

#endif // QEDITOR_H
