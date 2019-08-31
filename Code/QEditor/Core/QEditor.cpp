#include "QEditor.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QIcon>
#include <QtWidgets/QMenu>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollBar>

#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Core/Input.h>
#include <Engine/Core/GameLoader.h>
#include <Engine/State/EditorState.h>

#include <QEditor/UIGen/NewLevel_ui.h>

#include <QEditor/Panels/EntityPanel.h>
#include <QEditor/Panels/EntityTriggerPanel.h>
#include <QEditor/Panels/EntityOutputPanel.h>
#include <QEditor/Panels/EntityInputPanel.h>
#include <QEditor/Panels/MeshEntityPanel.h>
#include <QEditor/Panels/SkyFogPanel.h>
#include <QEditor/Panels/TerrainPanel.h>
#include <QEditor/Panels/LayersPanel.h>
#include <QEditor/Panels/BordersPanel.h>
#include <QEditor/Panels/MarginsPanel.h>
#include <QEditor/Panels/SilhouettesPanel.h>
#include <QEditor/Panels/ColorGradingPanel.h>
#include <QEditor/Panels/SpriteEntityPanel.h>
#include <QEditor/Panels/CameraEntityPanel.h>
#include <QEditor/Panels/EntityBrowserPanel.h>
#include <QEditor/Panels/NodeBrowserPanel.h>
#include <QEditor/Panels/PathEntityPanel.h>
#include <QEditor/Panels/WaypointEntityPanel.h>
#include <QEditor/Panels/AcceleratorEntityPanel.h>
#include <QEditor/Panels/MagnetEntityPanel.h>
#include <QEditor/Panels/StreamerEntityPanel.h>
#include <QEditor/Panels/StreamerGunEntityPanel.h>
#include <QEditor/Panels/CheckpointEntityPanel.h>
#include <QEditor/Panels/CheckpointDestinationEntityPanel.h>
#include <QEditor/Panels/CheckpointOriginEntityPanel.h>
#include <QEditor/Panels/RotatorEntityPanel.h>
#include <QEditor/Panels/PlatformEntityPanel.h>
#include <QEditor/Panels/MagnetClusterEntityPanel.h>
#include <QEditor/Panels/SkyDomeEntityPanel.h>
#include <QEditor/Panels/PlanetEntityPanel.h>
#include <QEditor/Panels/LightEntityPanel.h>
#include <QEditor/Panels/MissileGunEntityPanel.h>
#include <QEditor/Panels/MissileGunAimingEntityPanel.h>
#include <QEditor/Panels/HunterEntityPanel.h>
#include <QEditor/Panels/ParticleSystemEntityPanel.h>
#include <QEditor/Panels/ParticleEmitterEntityPanel.h>
#include <QEditor/Panels/LaserRadiatorEntityPanel.h>
#include <QEditor/Panels/LaserRadiatorAimingEntityPanel.h>
#include <QEditor/Panels/TextEntityPanel.h>
#include <QEditor/Panels/Character3DEntityPanel.h>
#include <QEditor/Panels/EnvMapEntityPanel.h>
#include <QEditor/Panels/StripesEntityPanel.h>
#include <QEditor/Panels/FadeEntityPanel.h>
#include <QEditor/Panels/ForcefieldEntityPanel.h>
#include <QEditor/Panels/SoundSpriteEntityPanel.h>
#include <QEditor/Panels/WeaponEntityPanel.h>
#include <QEditor/Panels/ExplodingHunterEntityPanel.h>

QEditor* gEditor = nullptr;

//-----------------------------------------------------------------------------------
QEditor::QEditor(QWidget *parent)
    : QMainWindow(parent)
    , m_pParentWidget(parent)
    , m_pActiveEntity(0)
    , m_bQuit(false)
    , m_pColorDialog(nullptr)
    , m_pColorDialogReceiver(nullptr)
    , m_pColorDialogSlot(nullptr)
{
    ui.setupUi(this);

    initSidePanel();

    initQuickAccessPanel();

    initCenterWidget(parent);

    initStatusBar();

    initMenuBar();

    initConnections();

    m_pColorDialog = new QColorDialog(m_pRenderWidget);
    m_pColorDialog->setOption(QColorDialog::NoButtons);
    m_pColorDialog->setAutoFillBackground(false);
    m_pColorDialog->setWindowFlags(Qt::Tool);

    gEditor = this;
}

// -----------------------------------------------------------------------------------
QEditor::~QEditor()
{
    for (mapEntityPanels::iterator it = m_entityPanels.begin(); it != m_entityPanels.end(); ++it)
    {
        ENV_DELETE(it->second.first);
    }

    ENV_DELETE(m_pEngine);
    ENV_DELETE(m_pComboLighting);
    ENV_DELETE(m_pLabelStatusBarLevel);
    ENV_DELETE(m_pMainSplitter);
}

//-----------------------------------------------------------------------------------
void QEditor::initSidePanel()
{
    m_pSidePanelLayout = new QVBoxLayout();
    m_pSidePanelLayout->setContentsMargins(0, 0, 0, 0);
    m_pSidePanelLayout->setSpacing(0);
    m_pSidePanelLayout->setAlignment(Qt::AlignTop);
    m_pSidePanelLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    m_pSidePanelWidget = new QWidget();
    m_pSidePanelWidget->setObjectName("PanelWidget");
    m_pSidePanelWidget->setAutoFillBackground(true);
    m_pSidePanelWidget->setContentsMargins(5, 5, 5, 5);
    m_pSidePanelWidget->setLayout(m_pSidePanelLayout);
    m_pSidePanelWidget->setFixedWidth(370);
    m_pSidePanelWidget->setStyleSheet("QWidget#" + m_pSidePanelWidget->objectName() + "{background: #30343b; border: 1px solid #202328;} ");

    m_pSidePanelScrollArea = new QScrollArea();
    m_pSidePanelScrollArea->setObjectName("entityScroll");
    m_pSidePanelScrollArea->setAutoFillBackground(true);
    m_pSidePanelScrollArea->setWidget(m_pSidePanelWidget);
    m_pSidePanelScrollArea->setContentsMargins(5, 5, 5, 5);
    m_pSidePanelScrollArea->setFixedWidth(370);
    m_pSidePanelScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pSidePanelScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_pSidePanelScrollArea->setStyleSheet("QWidget#" + m_pSidePanelScrollArea->objectName() + "{background: #30343b; border: 1px solid #202328;} ");
}

//-----------------------------------------------------------------------------------
void QEditor::initQuickAccessPanel()
{
    m_pQuickAccessLayout = new QVBoxLayout();
    m_pQuickAccessLayout->setContentsMargins(5, 5, 5, 5);
    m_pQuickAccessLayout->setSpacing(0);
    m_pQuickAccessLayout->setSpacing(1);
    m_pQuickAccessLayout->setAlignment(Qt::AlignTop);
    m_pQuickAccessLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    m_pQuickAccessWidget = new QWidget();
    m_pQuickAccessWidget->setObjectName("RightSplitter");
    m_pQuickAccessWidget->setAutoFillBackground(true);
    m_pQuickAccessWidget->setLayout(m_pQuickAccessLayout);
    m_pQuickAccessWidget->setStyleSheet("QWidget#" + m_pQuickAccessWidget->objectName() + "{background: #30343b; border: 0px;} ");

    m_pQuickAccessScrollArea = new QScrollArea();
    m_pQuickAccessScrollArea->setObjectName("scroll");
    m_pQuickAccessScrollArea->setWidget(m_pQuickAccessWidget);
    m_pQuickAccessScrollArea->setFixedWidth(205);
    m_pQuickAccessScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pQuickAccessScrollArea->setStyleSheet("QWidget#" + m_pQuickAccessScrollArea->objectName() + "{background: #30343b; border: 1px solid #202328;} ");

    QFont buttonFont("MS Shell Dlg 2", 10, QFont::Normal);

    QString strTitleBtnStyle =
        "border: 1px solid #777777;"
        "border - bottom: 1px solid black;"
        "border - radius:0px;"
        "background: rgba(78, 86, 99, 255);"
        "text-align:left;"
        "padding-left:5px;"
        "padding-right:5px;"
        "color:#CCCCCC;";

    QLineEdit* pSearchBar = new QLineEdit();
    pSearchBar->setStyleSheet(strTitleBtnStyle);
    pSearchBar->setMinimumHeight(30);
    pSearchBar->setFont(buttonFont);

    connect(pSearchBar, SIGNAL(textChanged(const QString&)), this, SLOT(slotEntitySearchEntered(const QString&)));

    m_pQuickAccessLayout->addWidget(pSearchBar);
}

//-----------------------------------------------------------------------------------
void QEditor::initCenterWidget(QWidget* pParent)
{
    m_pRenderWidget = new QRenderWidget();
    m_pRenderWidget->setAutoFillBackground(false);
    m_pRenderWidget->setMinimumSize(256, 256);
    m_pRenderWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::BypassWindowManagerHint | Qt::SubWindow);

    m_pMainSplitter = new QSplitter(Qt::Horizontal, pParent);
    m_pMainSplitter->setAutoFillBackground(false);
    m_pMainSplitter->setHandleWidth(10);
    m_pMainSplitter->addWidget(m_pSidePanelScrollArea);
    m_pMainSplitter->addWidget(m_pRenderWidget);
    m_pMainSplitter->addWidget(m_pQuickAccessScrollArea);
    m_pMainSplitter->setContentsMargins(0, 0, 0, 0);
    m_pMainSplitter->setStyleSheet("QSplitter::handle {background-color:#555a63; border:1px solid #6e788a; margin-left:1px;margin-right:1px; }");

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(30, 30, 30));

    setBackgroundRole(QPalette::Background);
    setCentralWidget(m_pMainSplitter);
    setContentsMargins(0, 0, 0, 0);
    setMinimumSize(800, 600);
    setPalette(pal);
}

//-----------------------------------------------------------------------------------
void QEditor::initStatusBar()
{
    m_pLabelStatusBarLevel = new QLabel();
    m_pLabelStatusBarLevel->setStyleSheet("color: #EEEEEE; padding-left:10px; padding-right:10px;");

    QStatusBar* pStatusBar = statusBar();
    pStatusBar->setStyleSheet("QStatusBar {background:#555a63; border:1px solid #6e788a; margin:1px; color: #EEEEEE}; QLabel { color: #EEEEEE;}");
    pStatusBar->addWidget(m_pLabelStatusBarLevel);
}

//-----------------------------------------------------------------------------------
void QEditor::initMenuBar()
{
    m_pActionPickFilterAll = ui.mainToolBar->addAction("Pick All");
    m_pActionPickFilterAll->setCheckable(true);
    m_pActionPickFilterAll->setChecked(true);

    m_pActionPickFilterEntities = ui.mainToolBar->addAction("Pick Entities");
    m_pActionPickFilterEntities->setCheckable(true);

    m_pActionPickFilterMeshes = ui.mainToolBar->addAction("Pick Meshes");
    m_pActionPickFilterMeshes->setCheckable(true);

    m_pActionPickFilterSprites = ui.mainToolBar->addAction("Pick Sprites");
    m_pActionPickFilterSprites->setCheckable(true);

    ui.mainToolBar->addSeparator();

    QIcon iconShowGrid("EditorShowGrid.png");
    m_pActionShowGrid = ui.mainToolBar->addAction(iconShowGrid, "");
    m_pActionShowGrid->setCheckable(true);

    ui.mainToolBar->addSeparator();

    QIcon iconPickEntity("EditorPickIcon.png");
    m_pActionPickEntities = ui.mainToolBar->addAction(iconPickEntity, "");
    m_pActionPickEntities->setCheckable(true);
    m_pActionPickEntities->setChecked(true);
    m_pActionPickEntities->setToolTip("Pick Entities");

    ui.mainToolBar->addSeparator();

    QIcon iconEditTerrain("EditorEditTerrain.png");
    m_pActionEditTerrain = ui.mainToolBar->addAction(iconEditTerrain, "");
    m_pActionEditTerrain->setCheckable(true);
    m_pActionEditTerrain->setToolTip("Edit Terrain Tile");

    QIcon iconEditTerrainDiffuse("EditorEditTerrainDiffuse.png");
    m_pActionEditTerrainDiffuse = ui.mainToolBar->addAction(iconEditTerrainDiffuse, "");
    m_pActionEditTerrainDiffuse->setCheckable(true);
    m_pActionEditTerrainDiffuse->setToolTip("Edit Terrain Diffuse");

    QIcon iconEditTerrainEmissive("EditorEditTerrainEmissive.png");
    m_pActionEditTerrainEmissive = ui.mainToolBar->addAction(iconEditTerrainEmissive, "");
    m_pActionEditTerrainEmissive->setCheckable(true);
    m_pActionEditTerrainEmissive->setToolTip("Edit Terrain Emissive");

    QIcon iconPickTerrain("EditorEditTerrain.png");
    m_pActionPickTerrain = ui.mainToolBar->addAction(iconPickTerrain, "");
    m_pActionPickTerrain->setCheckable(true);
    m_pActionPickTerrain->setToolTip("Pick Terrain Tile");
    
    ui.mainToolBar->addSeparator();

    QIcon iconSelectTerrain("EditorPickTerrain.png");
    m_pActionSelectTerrain = ui.mainToolBar->addAction(iconSelectTerrain, "");
    m_pActionSelectTerrain->setToolTip("Select Terrain");
    m_pActionSelectTerrain->setCheckable(true);

    QIcon iconSaveTerrain("EditorSaveTerrain.png");
    m_pActionSaveTerrainSelection = ui.mainToolBar->addAction(iconSaveTerrain, "");
    m_pActionSaveTerrainSelection->setToolTip("Save Terrain Selection");

    QIcon iconLoadTerrain("EditorLoadTerrain.png");
    m_pActionLoadTerrainSelection = ui.mainToolBar->addAction(iconLoadTerrain, "");
    m_pActionLoadTerrainSelection->setToolTip("Load Terrain Selection");

    m_actionsToolBar.push_back(m_pActionPickEntities);
    m_actionsToolBar.push_back(m_pActionEditTerrain);
    m_actionsToolBar.push_back(m_pActionEditTerrainDiffuse);
    m_actionsToolBar.push_back(m_pActionEditTerrainEmissive);
    m_actionsToolBar.push_back(m_pActionPickTerrain);
    m_actionsToolBar.push_back(m_pActionSelectTerrain);

    ui.mainToolBar->addSeparator();

    QLabel* pLabelLighting = new QLabel("Lighting");
    pLabelLighting->setStyleSheet("color: #EEEEEE; padding-left:10px; padding-right:10px;");
    ui.mainToolBar->addWidget(pLabelLighting);

    m_pComboLighting = new QComboBox();
    m_pComboLighting->addItem("Dynamic");
    m_pComboLighting->addItem("Final");
    m_pComboLighting->addItem("Precomputed Only");
    m_pComboLighting->addItem("Dynamic Only");
    m_pComboLighting->addItem("Off");

    ui.mainToolBar->addWidget(m_pComboLighting);

    ui.mainToolBar->setStyleSheet("QToolBar{ background: #555a63; border:1px solid #6e788a; margin:1px; padding-left:5px; padding-right:5px;} QToolButton{ background: #555a63; color: #EEEEEE; font-size:12px;font - family:Calibri;}");
    ui.mainToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
}

//-----------------------------------------------------------------------------------
void QEditor::applicationStateChanged(Qt::ApplicationState state)
{
    if (state == Qt::ApplicationInactive)
    {
        if (m_pEngine &&
            m_pEngine->GetInput())
        {
            m_pEngine->GetInput()->Reset();
        }
    }
}

//-----------------------------------------------------------------------------------
void QEditor::initConnections()
{
    connect(QApplication::instance(), SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(applicationStateChanged(Qt::ApplicationState)));
    connect(ui.pActionMainNewLevel, SIGNAL(triggered()), this, SLOT(slotMenuMainNewTriggered()));
    connect(ui.pActionMainSaveLevel, SIGNAL(triggered()), this, SLOT(slotMenuMainSaveLevelTriggered()));
    connect(ui.pActionMainSaveLevelAs, SIGNAL(triggered()), this, SLOT(slotMenuMainSaveLevelAsTriggered()));
    connect(ui.pActionMainLoadLevel, SIGNAL(triggered()), this, SLOT(slotMenuMainLoadLevelTriggered()));
    connect(ui.pActionMainExit, SIGNAL(triggered()), this, SLOT(slotMenuMainExitTriggered()));
    connect(ui.m_pActionViewResetCamera, SIGNAL(triggered()), this, SLOT(slotMenuViewResetCameraTriggered()));
    connect(ui.pActionToolsResizeLevel, SIGNAL(triggered()), this, SLOT(slotMenuToolsResizeLevelTriggered()));
    connect(ui.pActionToolsOffsetLevel, SIGNAL(triggered()), this, SLOT(slotMenuToolsOffsetLevelTriggered()));
    connect(ui.pActionToolsTerrain, SIGNAL(triggered()), this, SLOT(slotMenuToolsTerrainTriggered()));
    connect(ui.pActionToolsResetPainter, SIGNAL(triggered()), this, SLOT(slotMenuToolsResetPainterTriggered()));
    connect(ui.pActionToolsEntityBrowser, SIGNAL(triggered()), this, SLOT(slotMenuToolsEntityBrowserTriggered()));
    connect(ui.pActionToolsNodeBrowser, SIGNAL(triggered()), this, SLOT(slotMenuToolsNodeBrowserTriggered()));
    connect(ui.pActionWindowFullscreen, SIGNAL(toggled(bool)), this, SLOT(slotMenuWindowFullscreenToggled(bool)));
    connect(ui.pActionWindowBigPicture, SIGNAL(toggled(bool)), this, SLOT(slotMenuWindowBigPictureToggled(bool)));
    connect(ui.pActionWindowSidePane, SIGNAL(toggled(bool)), this, SLOT(slotMenuWindowSidePaneToggled(bool)));
    connect(ui.pActionWindowQuickAccessPane, SIGNAL(toggled(bool)), this, SLOT(slotMenuWindowQuickAccessPaneToggled(bool)));
    connect(ui.pActionLevelEnvironmentAndTerrain, SIGNAL(triggered()), this, SLOT(slotMenuLevelEnvironmentAndTerrain()));
    connect(ui.pActionLevelPostProcessing, SIGNAL(triggered()), this, SLOT(slotMenuLevelPostProcessing()));
    connect(ui.pActionLightingBakeLighting, SIGNAL(triggered()), this, SLOT(slotMenuLightingBakeLighting()));
    connect(m_pActionShowGrid, SIGNAL(toggled(bool)), this, SLOT(slotToolBarShowGridToggled(bool)));
    connect(m_pActionPickEntities, SIGNAL(triggered()), this, SLOT(slotToolBarPickEntityPressed()));
    connect(m_pActionSelectTerrain, SIGNAL(triggered()), this, SLOT(slotToolBarSelectTerrainPressed()));
    connect(m_pActionEditTerrain, SIGNAL(triggered()), this, SLOT(slotToolBarEditTerrainPressed()));
    connect(m_pActionEditTerrainDiffuse, SIGNAL(triggered()), this, SLOT(slotToolBarEditTerrainDiffusePressed()));
    connect(m_pActionEditTerrainEmissive, SIGNAL(triggered()), this, SLOT(slotToolBarEditTerrainEmissivePressed()));
    connect(m_pActionPickTerrain, SIGNAL(triggered()), this, SLOT(slotToolBarPickTerrainPressed()));
    connect(m_pActionSaveTerrainSelection, SIGNAL(triggered()), this, SLOT(slotToolBarSaveTerrainSelection()));
    connect(m_pActionLoadTerrainSelection, SIGNAL(triggered()), this, SLOT(slotToolBarLoadTerrainSelection()));
    connect(m_pActionPickFilterAll, SIGNAL(triggered()), this, SLOT(slotToolBarPickFilterAllPressed()));
    connect(m_pActionPickFilterEntities, SIGNAL(triggered()), this, SLOT(slotToolBarPickFilterEntitiesPressed()));
    connect(m_pActionPickFilterMeshes, SIGNAL(triggered()), this, SLOT(slotToolBarPickFilterMeshesPressed()));
    connect(m_pActionPickFilterSprites, SIGNAL(triggered()), this, SLOT(slotToolBarPickFilterSpritesPressed()));
    connect(m_pComboLighting, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLightingModeIndexChanged(int)));
}

//-----------------------------------------------------------------------------------
bool QEditor::InitializeEngine(QSplashScreen& splashScreen)
{
    splashScreen.showMessage("Initializing Engine...", Qt::AlignBottom, Qt::white);

    env::EngineSetup engineSetup;
    engineSetup.hInstance = (HINSTANCE)::GetModuleHandle(0);
    engineSetup.hWnd = (HWND)m_pRenderWidget->winId();
    engineSetup.bEditor = true;

    m_pEngine = new env::CEngine();
    m_pEngine->Init(engineSetup);

    m_pRenderWidget->setEngine(m_pEngine, m_pMainSplitter);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));

    m_pComboLighting->setCurrentIndex((int)pEditor->GetLightingMode());

    return true;
}

//-----------------------------------------------------------------------------------
void QEditor::UpdateQuickAccessPanel(const QString& str)
{
    for (int i = 1; i < m_pQuickAccessLayout->count(); ++i)
    {
        QPushButton* pWidget = static_cast<QPushButton*>(m_pQuickAccessLayout->itemAt(i)->widget());

        QString strName = pWidget->text();

        if (!str.isEmpty() && !strName.contains(str, Qt::CaseInsensitive))
            pWidget->hide();
        else
            pWidget->show();
    }
}

//-----------------------------------------------------------------------------------
bool QEditor::InitializeEntities(QSplashScreen& splashScreen)
{
    splashScreen.showMessage("Initializing Quick Access Panel...", Qt::AlignBottom, Qt::white);

    QFont buttonFont("MS Shell Dlg 2", 10, QFont::Normal);

    QString strTitleBtnStyle =
        "border: 1px solid #222222;"
        "border - bottom: 1px solid black;"
        "border - radius:0px;"
        "background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));"
        "text-align:left;"
        "padding-left:5px;"
        "padding-right:5px;"
        "color:#CCCCCC;";

    std::list< std::string > entityMetaTypenames;
    m_pEngine->GetEntityMgr()->GetEntityMetaTypeNames(entityMetaTypenames);

    entityMetaTypenames.sort();

    for (std::list< std::string >::iterator it = entityMetaTypenames.begin(); it != entityMetaTypenames.end(); ++it)
    {
        if (!m_pEngine->GetEntityMgr()->GetShowInEditor(*it))
            continue;

        QPushButton* pBtnAddEntity = new QPushButton();

        pBtnAddEntity->setText(QString(it->c_str()));
        pBtnAddEntity->setStyleSheet(strTitleBtnStyle);
        pBtnAddEntity->setMinimumHeight(26);
        pBtnAddEntity->setFont(buttonFont);

        connect(pBtnAddEntity, SIGNAL(pressed()), this, SLOT(slotEntityBtnClicked()));

        m_pQuickAccessLayout->addWidget(pBtnAddEntity);
    }

    return true;
}

//-----------------------------------------------------------------------------------
bool QEditor::InitializeEntityPanels(QSplashScreen& splashScreen)
{
    splashScreen.showMessage("Initializing Level Panels...", Qt::AlignBottom, Qt::white);

    registerPanel<CSkyFogPanel>(splashScreen, "SkyAndFog");
    registerPanel<CTerrainPanel>(splashScreen, "Terrain");
    registerPanel<CLayersPanel>(splashScreen, "Layers");
    registerPanel<CBordersPanel>(splashScreen, "Borders");
    registerPanel<CMarginsPanel>(splashScreen, "Margins");
    registerPanel<CSilhouettesPanel>(splashScreen, "Silhouettes");
    registerPanel<CColorGradingPanel>(splashScreen, "ColorGrading");
    registerPanel<CEntityBrowserPanel>(splashScreen, "EntityBrowser");
    registerPanel<CNodeBrowserPanel>(splashScreen, "NodeBrowser");

    splashScreen.showMessage("Initializing Entity Panels...", Qt::AlignBottom, Qt::white);
    
    registerPanel<CEntityPanel>(splashScreen, "Entity");
    registerPanel<CEntityTriggerPanel>(splashScreen, "EntityTrigger", false);
    registerPanel<CEntityOutputPanel>(splashScreen, "EntityOutput", false);
    registerPanel<CEntityInputPanel>(splashScreen, "EntityInput", false);
    registerPanel<CMeshEntityPanel>(splashScreen, env::ENTITY_TYPEID_MESH);
    registerPanel<CSpriteEntityPanel>(splashScreen, env::ENTITY_TYPEID_SPRITE);
    registerPanel<CameraEntityPanel>(splashScreen, env::ENTITY_TYPEID_CAMERA);
    registerPanel<CPathEntityPanel>(splashScreen, env::ENTITY_TYPEID_PATH);
    registerPanel<CAcceleratorEntityPanel>(splashScreen, env::ENTITY_TYPEID_ACCELERATOR);
    registerPanel<CMagnetEntityPanel>(splashScreen, env::ENTITY_TYPEID_MAGNET);
    registerPanel<CStreamerEntityPanel>(splashScreen, env::ENTITY_TYPEID_BURNER);
    registerPanel<CStreamerGunEntityPanel>(splashScreen, env::ENTITY_TYPEID_STREAMERGUN);
    registerPanel<CCheckpointEntityPanel>(splashScreen, env::ENTITY_TYPEID_CHECKPOINT);
    registerPanel<CCheckpointDestinationEntityPanel>(splashScreen, env::ENTITY_TYPEID_CHECKPOINT_DESTINATION);
    registerPanel<CCheckpointOriginEntityPanel>(splashScreen, env::ENTITY_TYPEID_CHECKPOINT_ORIGIN);
    registerPanel<CWaypointEntityPanel>(splashScreen, env::ENTITY_TYPEID_PATHWAYPOINT);
    registerPanel<CRotatorEntityPanel>(splashScreen, env::ENTITY_TYPEID_ROTATOR);
    registerPanel<CPlatformEntityPanel>(splashScreen, env::ENTITY_TYPEID_PLATFORM);
    registerPanel<CMagnetClusterEntityPanel>(splashScreen, env::ENTITY_TYPEID_MAGNETCLUSTER);
    registerPanel<CSkyDomeEntityPanel>(splashScreen, env::ENTITY_TYPEID_SKYDOME);
    registerPanel<CPlanetEntityPanel>(splashScreen, env::ENTITY_TYPEID_PLANET);
    registerPanel<CLightEntityPanel>(splashScreen, env::ENTITY_TYPEID_LIGHTSPRITE);
    registerPanel<CMissileGunEntityPanel>(splashScreen, env::ENTITY_TYPEID_MISSILEGUN);
    registerPanel<CMissileGunAimingEntityPanel>(splashScreen, env::ENTITY_TYPEID_MISSILEGUNAIMING);
    registerPanel<CHunterEntityPanel>(splashScreen, env::ENTITY_TYPEID_HUNTINGHUNTER);
    registerPanel<CParticleSystemEntityPanel>(splashScreen, env::ENTITY_TYPEID_PARTICLESYSTEM);
    registerPanel<CParticleEmitterEntityPanel>(splashScreen, env::ENTITY_TYPEID_PLANARPOINTEMITTER);
    registerPanel<CParticleEmitterEntityPanel>(splashScreen, env::ENTITY_TYPEID_SPHEREEMITTER);
    registerPanel<CBoxParticleEmitterEntityPanel>(splashScreen, env::ENTITY_TYPEID_BOXEMITTER);
    registerPanel<CParticleEmitterEntityPanel>(splashScreen, env::ENTITY_TYPEID_EMITTER);
    registerPanel<CLaserRadiatorEntityPanel>(splashScreen, env::ENTITY_TYPEID_LASERRADIATOR);
    registerPanel<CLaserRadiatorAimingEntityPanel>(splashScreen, env::ENTITY_TYPEID_LASERRADIATOR_AIMING);
    registerPanel<CTextEntityPanel>(splashScreen, env::ENTITY_TYPEID_TEXT);
    registerPanel<CCharacter3DEntityPanel>(splashScreen, env::ENTITY_TYPEID_CHARACTER3D);
    registerPanel<CEnvMapEntityPanel>(splashScreen, env::ENTITY_TYPEID_ENVMAP);
    registerPanel<CStripesEntityPanel>(splashScreen, env::ENTITY_TYPEID_STRIPES);
    registerPanel<CFadeEntityPanel>(splashScreen, env::ENTITY_TYPEID_FADE);
    registerPanel<CForcefieldEntityPanel>(splashScreen, env::ENTITY_TYPEID_FORCEFIELD);
    registerPanel<CSoundSpriteEntityPanel>(splashScreen, env::ENTITY_TYPEID_SOUNDSPRITE);
    registerPanel<CWeaponEntityPanel>(splashScreen, env::ENTITY_TYPEID_WEAPON);
    registerPanel<CExplodingHunterEntityPanel>(splashScreen, env::ENTITY_TYPEID_EXPLODINGHUNTEREMITTER);

    return true;
}

//-----------------------------------------------------------------------------------
template< class T >
void QEditor::registerPanel(QSplashScreen& splashScreen, const std::string& strPanelName, bool bInitialVisibility)
{
    T* pPanel = new T(this);

    splashScreen.showMessage("Initializing '" + pPanel->GetTitle() + "' Panel...", Qt::AlignBottom, Qt::white);

    m_entityPanels[strPanelName] = pairPanelInitialVisibility(pPanel, bInitialVisibility);
    m_entityPanels[strPanelName].first->GetWidget()->hide();
}

//-----------------------------------------------------------------------------------
template< class T >
void QEditor::registerPanel(QSplashScreen& splashScreen, env::ENTITY_TYPEID entityTypeID, bool bInitialVisibility)
{
    registerPanel<T>(splashScreen, m_pEngine->GetEntityMgr()->GetEntityMetaTypename(entityTypeID), bInitialVisibility);
}

//-----------------------------------------------------------------------------------
void QEditor::slotEntitySearchEntered(const QString& str)
{
    UpdateQuickAccessPanel(str);
}

//-----------------------------------------------------------------------------------
void QEditor::slotEntityBtnClicked()
{
    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());
    if (pBtn)
    {
        QString strEntity = pBtn->text();

        const env::Vec3& camPosition = m_pEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode()->GetDerivedPosition();

        env::ENTITY_TYPEID typeID = m_pEngine->GetEntityMgr()->GetEntityMetaTypeId(strEntity.toStdString());

        env::CEntity* pEntity = m_pEngine->GetEntityMgr()->CreateEntity(typeID);

        pEntity->GetNode()->SetPosition(env::Vec3(camPosition.x, camPosition.y, 0.0f));

        if (pEntity->GetPickable())
        {
            m_pEngine->GetEntityMgr()->SetActiveEntity(pEntity);

            m_pEngine->GetWorldMgr()->GetEntitySlider().DeselectSliders();
            m_pEngine->GetWorldMgr()->GetEntitySlider().Attach(pEntity->GetNode());
        }
    }
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuMainNewTriggered()
{
    QDialog newLevelDialog(this);

    Ui::NewLevel uiNewLevel;
    uiNewLevel.setupUi(&newLevelDialog);

    if (QDialog::Accepted == newLevelDialog.exec())
    {
        m_pEngine->GetLevelLoader().LoadLevel(uiNewLevel.pSpinBoxCellsX->value(),
                                              uiNewLevel.pSpinBoxCellsY->value());

        m_pEngine->SetNewState("EditorState");

        m_pLabelStatusBarLevel->setText("<Unsaved>");

        syncPanels();
    }
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuMainSaveLevelTriggered()
{
    if (m_pEngine->GetLevelLoader().GetLevelLoaded())
    {
        const std::string& strLevelFilename = m_pEngine->GetLevelLoader().GetLevelFilename();

        if (strLevelFilename.empty())
        {
            slotMenuMainSaveLevelAsTriggered();
        }
        else
        {
            m_pEngine->GetWorldMgr()->SaveMap(m_pEngine->GetLevelLoader().GetLevelFilename());
        }
    }
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuMainSaveLevelAsTriggered()
{
    if (m_pEngine->GetLevelLoader().GetLevelLoaded())
    {
        const std::string& strLevelFilename = m_pEngine->GetLevelLoader().GetLevelFilename();

        if (strLevelFilename.empty())
        {
            QString strSaveFilename = QFileDialog::getSaveFileName(this, "Save Level", "../Resources/Levels", "Levels (*.env)");

            m_pEngine->GetWorldMgr()->SaveMap(strSaveFilename.toStdString());
            m_pEngine->GetLevelLoader().SetLevelFilename(strSaveFilename.toStdString());

            m_pLabelStatusBarLevel->setText(strSaveFilename);
        }
        else
        {
            QString strSaveFilename = QFileDialog::getSaveFileName(this, "Save Level", strLevelFilename.c_str(), "Levels (*.env)");

            m_pEngine->GetWorldMgr()->SaveMap(strSaveFilename.toStdString());
            m_pEngine->GetLevelLoader().SetLevelFilename(strSaveFilename.toStdString());

            m_pLabelStatusBarLevel->setText(strSaveFilename);
        }
    }
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarShowGridToggled(bool bToggeld)
{
    m_pEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().SetWireframe(bToggeld);
}

env::CEntity* pActiveEntity = nullptr;

//-----------------------------------------------------------------------------------
void QEditor::UncheckActionsToolbar(QAction* pAction)
{
    if (!pAction->isChecked())
    {
        pAction->setChecked(true);
        return;
    }

    for (auto& action : m_actionsToolBar)
    {
        if (action != pAction)
            action->setChecked(false);
    }
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarPickEntityPressed()
{
    UncheckActionsToolbar(m_pActionPickEntities);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetClickHandler(env::CEditorState::CLICK_HANDLER_PICK);

    if (env::gEngine->GetEntityMgr()->IsValid(pActiveEntity))
    {
        env::gEngine->GetWorldMgr()->GetEntitySlider().Attach(pActiveEntity->GetNode());
        env::gEngine->GetEntityMgr()->SetActiveEntity(pActiveEntity);
    }
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarSelectTerrainPressed()
{
    UncheckActionsToolbar(m_pActionSelectTerrain);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetClickHandler(env::CEditorState::CLICK_HANDLER_SELECTTERRAIN);

    pActiveEntity = env::gEngine->GetEntityMgr()->GetActiveEntity();

    env::gEngine->GetWorldMgr()->GetEntitySlider().Detach();
    env::gEngine->GetEntityMgr()->SetActiveEntity(nullptr);
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarEditTerrainDiffusePressed()
{
    UncheckActionsToolbar(m_pActionEditTerrainDiffuse);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetClickHandler(env::CEditorState::CLICK_HANDLER_EDITTERRAIN_DIFFUSE);

    pActiveEntity = env::gEngine->GetEntityMgr()->GetActiveEntity();

    env::gEngine->GetWorldMgr()->GetEntitySlider().Detach();
    env::gEngine->GetEntityMgr()->SetActiveEntity(nullptr);
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarEditTerrainEmissivePressed()
{
    UncheckActionsToolbar(m_pActionEditTerrainEmissive);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetClickHandler(env::CEditorState::CLICK_HANDLER_EDITTERRAIN_EMISSIVE);

    pActiveEntity = env::gEngine->GetEntityMgr()->GetActiveEntity();

    env::gEngine->GetWorldMgr()->GetEntitySlider().Detach();
    env::gEngine->GetEntityMgr()->SetActiveEntity(nullptr);
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarPickTerrainPressed()
{
    UncheckActionsToolbar(m_pActionEditTerrain);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetClickHandler(env::CEditorState::CLICK_HANDLER_PAINTTERRAIN);

    pActiveEntity = env::gEngine->GetEntityMgr()->GetActiveEntity();

    env::gEngine->GetWorldMgr()->GetEntitySlider().Detach();
    env::gEngine->GetEntityMgr()->SetActiveEntity(nullptr);
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarEditTerrainPressed()
{
    UncheckActionsToolbar(m_pActionEditTerrain);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetClickHandler(env::CEditorState::CLICK_HANDLER_EDITTERRAIN);

    pActiveEntity = env::gEngine->GetEntityMgr()->GetActiveEntity();

    env::gEngine->GetWorldMgr()->GetEntitySlider().Detach();
    env::gEngine->GetEntityMgr()->SetActiveEntity(nullptr);
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarSaveTerrainSelection()
{
    QString strFilename = QFileDialog::getSaveFileName(this, "Save Terrain Template", "../Resources/Terrain", "Terrain Templates (*.terrain)");

    if (strFilename.isEmpty())
        return;

    QFileInfo fi(strFilename);

    env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelSelection().SaveTemplate(strFilename.toStdString());
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarLoadTerrainSelection()
{
    QString strFilename = QFileDialog::getOpenFileName(this, "Load Terrain Template", "../Resources/Terrain", "Terrain Templates (*.terrain)");

    if (strFilename.isEmpty())
        return;

    QFileInfo fi(strFilename);

    m_pActionSelectTerrain->setChecked(true);

    slotToolBarSelectTerrainPressed();

    env::CEditorState* pEditorState = static_cast<env::CEditorState*>(env::gEngine->GetState("EditorState"));

    pEditorState->LoadTerrainTemplate("../Terrain/" + fi.fileName().toStdString());
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarPaintTerrainPressed()
{
//     if (!m_pActionPaintTerrain->isChecked())
//     {
//         m_pActionPaintTerrain->setChecked(true);
//         return;
//     }
// 
//     m_pActionPickEntities->setChecked(false);
//     m_pActionPickTerrain->setChecked(false);
//     m_pActionEditTerrain->setChecked(false);
// 
//     env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
//     pEditor->SetClickHandler(env::CEditorState::CLICK_HANDLER_PAINTTERRAIN);
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarPickFilterAllPressed()
{
    if (!m_pActionPickFilterAll->isChecked())
    {
        m_pActionPickFilterAll->setChecked(true);
        return;
    }

    m_pActionPickFilterEntities->setChecked(false);
    m_pActionPickFilterSprites->setChecked(false);
    m_pActionPickFilterMeshes->setChecked(false);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetPickType(env::CEditorState::ENTITY_PICK_TYPE_ALL);
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarPickFilterEntitiesPressed()
{
    if (!m_pActionPickFilterEntities->isChecked())
    {
        m_pActionPickFilterEntities->setChecked(true);
        return;
    }

    m_pActionPickFilterAll->setChecked(false);
    m_pActionPickFilterSprites->setChecked(false);
    m_pActionPickFilterMeshes->setChecked(false);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetPickType(env::CEditorState::ENTITY_PICK_TYPE_ENTITIES);
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarPickFilterMeshesPressed()
{
    if (!m_pActionPickFilterMeshes->isChecked())
    {
        m_pActionPickFilterMeshes->setChecked(true);
        return;
    }

    m_pActionPickFilterAll->setChecked(false);
    m_pActionPickFilterEntities->setChecked(false);
    m_pActionPickFilterSprites->setChecked(false);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetPickType(env::CEditorState::ENTITY_PICK_TYPE_MESHES);
}

//-----------------------------------------------------------------------------------
void QEditor::slotToolBarPickFilterSpritesPressed()
{
    if (!m_pActionPickFilterSprites->isChecked())
    {
        m_pActionPickFilterSprites->setChecked(true);
        return;
    }

    m_pActionPickFilterAll->setChecked(false);
    m_pActionPickFilterEntities->setChecked(false);
    m_pActionPickFilterMeshes->setChecked(false);

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetPickType(env::CEditorState::ENTITY_PICK_TYPE_SPRITES);
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuLevelEnvironmentAndTerrain()
{
    clearPanel();

    addEntityPanel("SkyAndFog");
    addEntityPanel("Layers");
    addEntityPanel("Borders");
    addEntityPanel("Margins");
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuLevelPostProcessing()
{
    clearPanel();

    addEntityPanel("Silhouettes");
    addEntityPanel("ColorGrading");
}

//-----------------------------------------------------------------------------------
void QEditor::slotLightingModeIndexChanged(int index)
{
    env::RenderQueue::LIGHTING_MODE lightingMode = (env::RenderQueue::LIGHTING_MODE)m_pComboLighting->currentIndex();

    env::CEditorState* pEditor = static_cast<env::CEditorState*>(m_pEngine->GetState("EditorState"));
    pEditor->SetLightingMode(lightingMode);
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuToolsResizeLevelTriggered()
{
    if (!m_pEngine->GetLevelLoader().GetLevelLoaded())
        return;

    QDialog newLevelDialog(this);

    Ui::NewLevel uiNewLevel;
    uiNewLevel.setupUi(&newLevelDialog);
    uiNewLevel.pSpinBoxCellsX->setValue(m_pEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetNumCellsX());
    uiNewLevel.pSpinBoxCellsY->setValue(m_pEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().GetNumCellsY());

    newLevelDialog.setWindowTitle("Resize Level");

    if (QDialog::Accepted == newLevelDialog.exec())
    {
        m_pEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().SetLevelSize(uiNewLevel.pSpinBoxCellsX->value(),
                                                                             uiNewLevel.pSpinBoxCellsY->value());
    }
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuToolsOffsetLevelTriggered()
{
    if (!m_pEngine->GetLevelLoader().GetLevelLoaded())
        return;

    QDialog newLevelDialog(this);

    Ui::NewLevel uiNewLevel;
    uiNewLevel.setupUi(&newLevelDialog);
    uiNewLevel.pSpinBoxCellsX->setValue(0);
    uiNewLevel.pSpinBoxCellsY->setValue(0);

    newLevelDialog.setWindowTitle("Offset Level");

    if (QDialog::Accepted == newLevelDialog.exec())
    {
        env::Vec3 offset(uiNewLevel.pSpinBoxCellsX->value() * env::CELL_SIZE, uiNewLevel.pSpinBoxCellsY->value() * env::CELL_SIZE, 0.0f);

        m_pEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().OffsetLevel(uiNewLevel.pSpinBoxCellsX->value(),
                                                                            uiNewLevel.pSpinBoxCellsY->value());

        m_pEngine->GetSceneMgr()->Offset(offset.x, offset.y);

//         env::CSceneNode* pRootNode = m_pEngine->GetSceneMgr()->GetRootNode();
// 
//         const auto& entities = pRootNode->GetChildNodes();
// 
//         for (auto it = entities.begin(); it != entities.end(); ++it)
//         {
//             it->second->Translate(offset);
//         }
    }
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuToolsTerrainTriggered()
{
    clearPanel();

    addEntityPanel("Terrain");
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuToolsResetPainterTriggered()
{
    env::gEngine->GetWorldMgr()->GetLevelMgr()->GetLevelGrid().ResetPainterCells();
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuToolsEntityBrowserTriggered()
{
    clearPanel();

    addEntityPanel("EntityBrowser");
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuToolsNodeBrowserTriggered()
{
    clearPanel();

    addEntityPanel("NodeBrowser");
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuLightingBakeLighting()
{
    std::list< env::CLight* > staticLights;

    const env::CSceneMgr::mapLights& lights = env::gEngine->GetSceneMgr()->GetLights();
    for (env::CSceneMgr::mapLights::const_iterator it = lights.begin(); it != lights.end(); ++it)
    {
        if (it->second->GetType() != env::LT_DIRECTIONAL && it->second->GetStatic() && !it->second->GetSpecular())
            staticLights.push_back(it->second);
    }

    env::gEngine->GetWorldMgr()->GetLevelMgr()->BakeLighting(staticLights);
}

//-----------------------------------------------------------------------------------
void QEditor::clearPanel()
{
    for (listEntityPanels::iterator it = m_activeEntityPanels.begin(); it != m_activeEntityPanels.end(); ++it)
    {
        m_pSidePanelLayout->removeWidget((*it)->GetWidget());
        (*it)->GetWidget()->hide();
    }

    m_activeEntityPanels.clear();
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuWindowSidePaneToggled(bool bToggeld)
{
    if (bToggeld)
    {
        m_pSidePanelScrollArea->setFixedWidth(370);
    }
    else
        m_pSidePanelScrollArea->setFixedWidth(0);

    if (ui.pActionWindowQuickAccessPane->isChecked() || ui.pActionWindowSidePane->isChecked())
        ui.pActionWindowBigPicture->setChecked(false);
    else
        ui.pActionWindowBigPicture->setChecked(true);
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuWindowQuickAccessPaneToggled(bool bToggeld)
{
    if (bToggeld)
    {
        m_pQuickAccessScrollArea->setFixedWidth(205);
        //ui.pActionWindowBigPicture->setChecked(false);
    }
    else
        m_pQuickAccessScrollArea->setFixedWidth(0);

    if (ui.pActionWindowQuickAccessPane->isChecked() || ui.pActionWindowSidePane->isChecked())
        ui.pActionWindowBigPicture->setChecked(false);
    else
        ui.pActionWindowBigPicture->setChecked(true);
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuWindowBigPictureToggled(bool bToggeld)
{
    ui.pActionWindowSidePane->setChecked(!bToggeld);
    ui.pActionWindowQuickAccessPane->setChecked(!bToggeld);
    ui.pActionWindowBigPicture->setChecked(bToggeld);
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuWindowFullscreenToggled(bool bToggeld)
{
    if (bToggeld)
        showFullScreen();
    else
        showMaximized();
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuMainLoadLevelTriggered()
{
    QString strFilename = QFileDialog::getOpenFileName(this, "Load Level", "../Resources/Levels", "Levels (*.env)");

    if (!strFilename.isEmpty())
    {
        m_pEngine->GetLevelLoader().LoadLevel(strFilename.toStdString(), "EditorState");

        m_pLabelStatusBarLevel->setText(strFilename);

        // 		syncPanels();
    }
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuMainExitTriggered()
{
    ENV_DELETE(m_pEngine);

    m_pRenderWidget->setEngine(nullptr, m_pMainSplitter);

    m_bQuit = true;
}

//-----------------------------------------------------------------------------------
void QEditor::slotMenuViewResetCameraTriggered()
{
    if (m_pEngine->GetWorldMgr()->GetActiveCamera())
        return;

    env::CSceneNode* pNode = m_pEngine->GetWorldMgr()->GetActiveCamera()->GetAttachedNode();
    const env::Vec3& camDefaultPos = m_pEngine->GetWorldMgr()->GetActiveCamera()->GetDefaultPosition();

    pNode->SetOrientation(env::CQuaternion());
    pNode->SetPosition(camDefaultPos);
}

//-----------------------------------------------------------------------------------
bool QEditor::Update()
{
    if (m_bQuit)
        return false;

    m_pRenderWidget->Update();

    if (!m_pEngine)
        return false;

    if (!m_pEngine->GameLoop())
        return false;



    env::CEntity* pEntity = m_pEngine->GetEntityMgr()->GetActiveEntity();

    CEntityBrowserPanel* pBrowserPanel = static_cast<CEntityBrowserPanel*>(m_entityPanels["EntityBrowser"].first);

    // Prevents the left pane from beeing updated because the currently selected entity has changed
    if (pBrowserPanel->GetBlockPaneUpdate())
        m_pActiveEntity = pEntity;

    // Forces the left pane to be updated depending on the currently selected entity
    if (pBrowserPanel->GetNeedsPaneUpdate())
        m_pActiveEntity = nullptr;

    pBrowserPanel->SetBlockPaneUpdate(false);
    pBrowserPanel->SetNeedsPaneUpdate(false);

    if (m_pActiveEntity != pEntity)
    {
        bool bKeepColorDialog = false;

        if (m_pActiveEntity && pEntity &&
            m_pActiveEntity->GetMetaTypeId() == pEntity->GetMetaTypeId() && m_pColorDialog->isVisible())
        {
            bKeepColorDialog = true;
        }
        else
        {
            ResetColorDialog();
        }

        m_pActiveEntity = pEntity;

        clearPanel();

        if (m_pActiveEntity)
        {
            // Adjust side pane, load corresponding panel(s) (multiple panels if necessary)

            static bool bInitial = true;

            CPanelBase* pNewPanel = nullptr;
            if (dynamic_cast<env::CEntity*>(m_pActiveEntity))
            {
                addEntityPanel("Entity", bInitial);
                addEntityPanel("EntityTrigger", bInitial);
                addEntityPanel("EntityOutput", bInitial);
                addEntityPanel("EntityInput", bInitial);
            }

            if (dynamic_cast<env::CSpriteEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CSpriteEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CMeshEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CMeshEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CCameraEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CCameraEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CPathEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CPathEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CPathWaypointEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CPathWaypointEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CRotatorEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CRotatorEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CMagnetEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CMagnetEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CAcceleratorEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CAcceleratorEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CStreamerEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CStreamerEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CStreamerGunEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CStreamerGunEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CCheckpointEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CCheckpointEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CCheckpointDestinatonEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CCheckpointDestinatonEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CCheckpointOriginEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CCheckpointOriginEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CPlatformEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CPlatformEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CMagnetClusterEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CMagnetClusterEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CSkyDomeEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CSkyDomeEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CPlanetEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CPlanetEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CLightSpriteEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CLightSpriteEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CMissileGunEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CMissileGunEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CMissileGunAimingEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CMissileGunAimingEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CHuntingHunterEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CHuntingHunterEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CParticleSystemEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CParticleSystemEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CParticleEmitterEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_PLANARPOINTEMITTER), bInitial);
            }

            if (dynamic_cast<env::CBoxParticleEmitterEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_BOXEMITTER), bInitial);
            }

            if (dynamic_cast<env::CLaserRadiatorEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_LASERRADIATOR), bInitial);
            }

            if (dynamic_cast<env::CLaserRadiatorAimingEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_LASERRADIATOR_AIMING), bInitial);
            }

            if (dynamic_cast<env::CTextEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_TEXT), bInitial);
            }

            if (dynamic_cast<env::CCharacter3DEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_CHARACTER3D), bInitial);
            }

            if (dynamic_cast<env::CEnvMapEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_ENVMAP), bInitial);
            }

            if (dynamic_cast<env::CStripesEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_STRIPES), bInitial);
            }

            if (dynamic_cast<env::CFadeEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_FADE), bInitial);
            }

            if (dynamic_cast<env::CForceFieldEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CForceFieldEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CWeaponEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::ENTITY_TYPEID_WEAPON), bInitial);
            }

            if (dynamic_cast<env::CSoundSpriteEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CSoundSpriteEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (dynamic_cast<env::CExplodingHunterEmitterEntity*>(m_pActiveEntity))
            {
                pNewPanel = addEntityPanel(m_pEngine->GetEntityMgr()->GetEntityMetaTypename(env::CExplodingHunterEmitterEntity::ENTITY_META_TYPEID), bInitial);
            }

            if (pNewPanel && bKeepColorDialog)
            {
                ENV_ASSERT_DBG(dynamic_cast<QObject*>(pNewPanel));

                AcquireColorDialog(static_cast<QObject*>(pNewPanel), m_pColorDialogSlot, FromQColor(m_pColorDialog->currentColor()), m_pColorDialog->windowTitle().toStdString().c_str());
            }

            bInitial = false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------------
void QEditor::AcquireColorDialog(QObject* pReceiver, const char* pSlot, const env::Vec3& initialColor, const char* pTitle)
{
    disconnect(m_pColorDialog, SIGNAL(currentColorChanged(const QColor&)), m_pColorDialogReceiver, m_pColorDialogSlot);

    m_pColorDialogReceiver = pReceiver;
    m_pColorDialogSlot = pSlot;

    connect(m_pColorDialog, SIGNAL(currentColorChanged(const QColor&)), m_pColorDialogReceiver, pSlot);

    m_pColorDialog->setOption(QColorDialog::NoButtons);
    m_pColorDialog->setWindowTitle(pTitle);
    m_pColorDialog->setCurrentColor(ToQColor(initialColor));
    m_pColorDialog->show();

    QPoint colorDialogPosition = m_pRenderWidget->mapToGlobal(QPoint(m_pRenderWidget->width() - m_pColorDialog->width() - 17, m_pRenderWidget->height() - m_pColorDialog->height() - 40));
    // bottom right
    m_pColorDialog->move(colorDialogPosition);
}

//-----------------------------------------------------------------------------------
void QEditor::AcquireColorDialog(QObject* pReceiver, const char* pSlot, const env::Vec4& initialColor, const char* pTitle)
{
    disconnect(m_pColorDialog, SIGNAL(currentColorChanged(const QColor&)), m_pColorDialogReceiver, m_pColorDialogSlot);

    m_pColorDialogReceiver = pReceiver;
    m_pColorDialogSlot = pSlot;

    connect(m_pColorDialog, SIGNAL(currentColorChanged(const QColor&)), m_pColorDialogReceiver, pSlot);

    m_pColorDialog->setOptions(QColorDialog::NoButtons | QColorDialog::ShowAlphaChannel);
    m_pColorDialog->setWindowTitle(pTitle);
    m_pColorDialog->setCurrentColor(ToQColor(initialColor));
    m_pColorDialog->show();

    QPoint colorDialogPosition = m_pRenderWidget->mapToGlobal(QPoint(m_pRenderWidget->width() - m_pColorDialog->width() - 17, m_pRenderWidget->height() - m_pColorDialog->height() - 40));
    // bottom right
    m_pColorDialog->move(colorDialogPosition);
}

//-----------------------------------------------------------------------------------
void QEditor::ResetColorDialog()
{
    disconnect(m_pColorDialog, SIGNAL(currentColorChanged(const QColor&)), m_pColorDialogReceiver, m_pColorDialogSlot);

    m_pColorDialogReceiver = nullptr;
    m_pColorDialogSlot = nullptr;

    m_pColorDialog->hide();
}

//-----------------------------------------------------------------------------------
CPanelBase* QEditor::addEntityPanel(const std::string& strPanelID, bool bInitial)
{
    mapEntityPanels::iterator it = m_entityPanels.find(strPanelID);

    if (it != m_entityPanels.end())
    {
        CPanelBase* pPanel = it->second.first;

        pPanel->GetWidget()->setMaximumWidth(340);
        
        m_pSidePanelLayout->addWidget(pPanel->GetWidget());

        pPanel->syncEntityEvent(m_pActiveEntity);
        pPanel->GetWidget()->show();

        if (bInitial && !it->second.second)
        {
            QEditorPanelBox* pPanelWidget = dynamic_cast<QEditorPanelBox*>(pPanel->GetWidget());

            if (pPanelWidget)
            {
                pPanelWidget->showPanel(false);
            }
        }

        m_activeEntityPanels.push_back(pPanel);

        return pPanel;
    }

    return nullptr;
}

//-----------------------------------------------------------------------------------
void QEditor::syncPanels()
{
    for (mapEntityPanels::iterator it = m_entityPanels.begin(); it != m_entityPanels.end(); ++it)
    {
        it->second.first->syncEntityEvent(m_pActiveEntity);
    }
}

//-----------------------------------------------------------------------------------
void QEditor::mousePressEvent(QMouseEvent * event)
{

}

//-----------------------------------------------------------------------------------
void QEditor::mouseReleaseEvent(QMouseEvent * event)
{

}

//-----------------------------------------------------------------------------------
void QEditor::keyPressEvent(QKeyEvent* event)
{
    event->accept();
}

//-----------------------------------------------------------------------------------
void QEditor::keyReleaseEvent(QKeyEvent* event)
{

}

//-----------------------------------------------------------------------------------
void QEditor::resizeEvent(QResizeEvent * event)
{
    //m_pEngine->ResetDevice();
}

void QEditor::closeEvent(QCloseEvent *event)
{
    ENV_DELETE(m_pEngine);

    m_pRenderWidget->setEngine(nullptr, m_pMainSplitter);

    m_bQuit = true;
}