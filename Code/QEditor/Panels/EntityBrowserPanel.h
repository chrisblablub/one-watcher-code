/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/EntityBrowserPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Entity/EntityMgr.h>
#define UNICODE 


class CEntityBrowserPanel : public CPanel<Ui::EntityBrowserPanel>
{
    Q_OBJECT

public:

    //-----------------------------------------------------------------------------------
    CEntityBrowserPanel(QWidget* pForm)
        : CPanel<Ui::EntityBrowserPanel>(pForm)
        , m_bBlockPaneUpdate(false)
        , m_bNeedPaneUpdate(false)
    {
        connect(m_ui.pBtnDeleteSelection, SIGNAL(pressed()), this, SLOT(slotDeleteSelection()));
        connect(m_ui.pTreeEntities, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotItemDoubleClicked(QTreeWidgetItem *, int)));
        connect(m_ui.pTreeEntities, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slotItemClicked(QTreeWidgetItem *, int)));

        InitTreeWidget();
    }

    //-----------------------------------------------------------------------------------
    virtual ~CEntityBrowserPanel()
    {
        ClearTreeWidget();
    }

    void SetBlockPaneUpdate(bool b) { m_bBlockPaneUpdate = b; }
    bool GetBlockPaneUpdate() const { return m_bBlockPaneUpdate; }

    void SetNeedsPaneUpdate(bool b) { m_bNeedPaneUpdate = b; }
    bool GetNeedsPaneUpdate() const { return m_bNeedPaneUpdate; }

    //-----------------------------------------------------------------------------------
    void UpdateUI()
    {
        ClearTreeWidget();

        InitTreeWidget();

        if (m_pEntity)
        {
            mapTopLevelItems::iterator it = m_topLevelItems.find(m_pEntity->GetMetaTypeId());

            if (it != m_topLevelItems.end())
            {
                m_ui.pTreeEntities->setCurrentItem(it->second);
            }
        }

        m_ui.qEditorPanelBox->setTitle("Entity Browser [" + QString::number(env::gEngine->GetEntityMgr()->GetEntities().size()) + "]");
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        m_pEntity = pEntity;

        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    void ClearTreeWidget()
    {
        for (mapTopLevelItems::iterator it = m_topLevelItems.begin(); it != m_topLevelItems.end(); ++it)
        {
            delete it->second;
        }

        m_topLevelItems.clear();

        m_ui.pTreeEntities->clear();
    }

    //-----------------------------------------------------------------------------------
    void InitTreeWidget()
    {
        m_ui.pTreeEntities->setHeaderLabel("Type");
        m_ui.pTreeEntities->header()->setSortIndicator(0, Qt::AscendingOrder);

        std::list< env::ENTITY_TYPEID > metaTypenames;
        env::gEngine->GetEntityMgr()->GetEntityMetaTypeIds(metaTypenames);

        for (std::list< env::ENTITY_TYPEID >::iterator it = metaTypenames.begin(); it != metaTypenames.end(); ++it)
        {
            env::ENTITY_TYPEID typeID = *it;
            std::string typeName = env::gEngine->GetEntityMgr()->GetEntityMetaTypename(typeID);

            QTreeWidgetItem* pTypeItem = new QTreeWidgetItem();
            pTypeItem->setText(0, "[" + QString(typeName.c_str()) + "]");
            pTypeItem->setFlags(Qt::ItemIsEnabled);

            m_topLevelItems[typeID] = pTypeItem;

            m_ui.pTreeEntities->addTopLevelItem(pTypeItem);

            std::list< env::CEntity* > entities;
            env::gEngine->GetEntityMgr()->GetEntities(entities, typeID);

            for (std::list< env::CEntity* >::iterator jt = entities.begin(); jt != entities.end(); ++jt)
            {
                env::CEntity* pEntity = *jt;

                QTreeWidgetItem* pEntityItem = new QTreeWidgetItem();
                pEntityItem->setText(0, pEntity->GetName().c_str());
                pEntityItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);


                QString strPickable = "Pickable: " + QString(pEntity->GetPickable() ? "Yes" : "No");
                QString strAutoDelete = "Auto Delete: " + QString(pEntity->GetAutoDelete() ? "Yes" : "No");
                QString strGhost = "Ghost: " + QString(pEntity->GetGhost() ? "Yes" : "No");
                QString strInputs = "Inputs: " + QString::number(pEntity->GetInputs().size());
                QString strOutputs = "Outputs: " + QString::number(pEntity->GetOutputs().size());
                QString strNode = "Node: " + QString(pEntity->GetNode()->GetName().c_str());

                QTreeWidgetItem* pEntityItemPickable = new QTreeWidgetItem();
                pEntityItemPickable->setText(0, strPickable);
                pEntityItemPickable->setFlags(Qt::NoItemFlags);

                QTreeWidgetItem* pEntityItemAutoDelete = new QTreeWidgetItem();
                pEntityItemAutoDelete->setText(0, strAutoDelete);
                pEntityItemAutoDelete->setFlags(Qt::NoItemFlags);

                QTreeWidgetItem* pEntityItemGhost = new QTreeWidgetItem();
                pEntityItemGhost->setText(0, strGhost);
                pEntityItemGhost->setFlags(Qt::NoItemFlags);

                QTreeWidgetItem* pEntityItemInputs = new QTreeWidgetItem();
                pEntityItemInputs->setText(0, strInputs);
                pEntityItemInputs->setFlags(Qt::NoItemFlags);

                QTreeWidgetItem* pEntityItemOutputs = new QTreeWidgetItem();
                pEntityItemOutputs->setText(0, strOutputs);
                pEntityItemOutputs->setFlags(Qt::NoItemFlags);

                QTreeWidgetItem* pEntityItemNode = new QTreeWidgetItem();
                pEntityItemNode->setText(0, strNode);
                pEntityItemNode->setFlags(Qt::NoItemFlags);

                pTypeItem->addChild(pEntityItem);
                pEntityItem->addChild(pEntityItemPickable);
                pEntityItem->addChild(pEntityItemAutoDelete);
                pEntityItem->addChild(pEntityItemGhost);
                pEntityItem->addChild(pEntityItemInputs);
                pEntityItem->addChild(pEntityItemOutputs);
                pEntityItem->addChild(pEntityItemNode);


            }
        }
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotItemClicked(QTreeWidgetItem* pItem, int column)
    {
        env::CEntity* pEntity = env::gEngine->GetEntityMgr()->GetEntity(pItem->text(column).toStdString());

        if (pEntity)
        {
            env::gEngine->GetWorldMgr()->GetEntitySlider().DeselectSliders();
            env::gEngine->GetWorldMgr()->GetEntitySlider().Attach(pEntity->GetNode());
            env::gEngine->GetWorldMgr()->GetDefaultCamera()->GetAttachedNode()->SetPosition(pEntity->GetNode()->GetDerivedPosition() + env::Vec3(0.0f, 0.0f, -1200.0f));

            env::gEngine->GetEntityMgr()->SetActiveEntity(pEntity);

            SetBlockPaneUpdate(true);
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void slotItemDoubleClicked(QTreeWidgetItem* pItem, int column)
    {
        env::CEntity* pEntity = env::gEngine->GetEntityMgr()->GetEntity(pItem->text(column).toStdString());

        if (pEntity)
        {
            env::gEngine->GetWorldMgr()->GetEntitySlider().DeselectSliders();
            env::gEngine->GetWorldMgr()->GetEntitySlider().Attach(pEntity->GetNode());
            env::gEngine->GetWorldMgr()->GetDefaultCamera()->GetAttachedNode()->SetPosition(pEntity->GetNode()->GetDerivedPosition() + env::Vec3(0.0f, 0.0f, -1200.0f));

            env::gEngine->GetEntityMgr()->SetActiveEntity(pEntity);

            SetNeedsPaneUpdate(true);
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void slotDeleteSelection()
    {
        QList< QTreeWidgetItem* > selectedItems = m_ui.pTreeEntities->selectedItems();

        for (int i = 0; i < selectedItems.size(); ++i)
        {
            QTreeWidgetItem* pTreeItem = selectedItems.at(i);

            const QString& strName = pTreeItem->text(0);

            if (env::gEngine->GetEntityMgr()->RemoveEntity(strName.toStdString()))
            {
                m_ui.pTreeEntities->removeItemWidget(pTreeItem, 0);
            }
        }

        m_ui.pTreeEntities->update();

        UpdateUI();
    }

private:

    typedef std::map< env::ENTITY_TYPEID, QTreeWidgetItem* > mapTopLevelItems;
    mapTopLevelItems m_topLevelItems;

    env::CEntity* m_pEntity;

    bool m_bBlockPaneUpdate;
    bool m_bNeedPaneUpdate;

};