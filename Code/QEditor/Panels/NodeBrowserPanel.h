/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/NodeBrowserPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QColorDialog>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Core/Scenario.h>
#include <Engine/Core/WorldMgr.h>
#include <Engine/Scene/SceneMgr.h>
#define UNICODE 


class CNodeBrowserPanel : public CPanel<Ui::NodeBrowserPanel>
{
    Q_OBJECT

public:

    //-----------------------------------------------------------------------------------
    CNodeBrowserPanel(QWidget* pForm)
        : CPanel<Ui::NodeBrowserPanel>(pForm)
        , m_bBlockPaneUpdate(false)
        , m_bNeedPaneUpdate(false)
    {
        connect(m_ui.pBtnDeleteSelection, SIGNAL(pressed()), this, SLOT(slotDeleteSelection()));
        connect(m_ui.pTreeNodes, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotItemDoubleClicked(QTreeWidgetItem *, int)));
        connect(m_ui.pTreeNodes, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slotItemClicked(QTreeWidgetItem *, int)));

        InitTreeWidget();
    }

    //-----------------------------------------------------------------------------------
    virtual ~CNodeBrowserPanel()
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
        
        m_ui.qEditorPanelBox->setTitle("Node Browser [" + QString::number(env::gEngine->GetSceneMgr()->GetSceneNodes().size()) + "]");
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        UpdateUI();
    }

    //-----------------------------------------------------------------------------------
    void ClearTreeWidget()
    {
        for (listTopLevelItems::iterator it = m_topLevelItems.begin(); it != m_topLevelItems.end(); ++it)
        {
            delete *it;
        }
       
        m_topLevelItems.clear();

        m_ui.pTreeNodes->clear();
    }

    //-----------------------------------------------------------------------------------
    void AddNodesRecursive(env::CSceneNode* pNode, QTreeWidgetItem* pNodeItem)
    {
        for (const auto& node : pNode->GetChildNodes())
        {
            QTreeWidgetItem* pNewNodeItem = new QTreeWidgetItem();

            if (node.second->GetDebugName().empty())
                pNewNodeItem->setText(0, node.second->GetName().c_str());
            else
                pNewNodeItem->setText(0, node.second->GetDebugName().c_str());
            pNewNodeItem->setData(0, Qt::UserRole, node.second->GetName().c_str());

            pNewNodeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            pNodeItem->addChild(pNewNodeItem);

            AddNodesRecursive(node.second, pNewNodeItem);
        }
    }

    //-----------------------------------------------------------------------------------
    void InitTreeWidget()
    {
        m_ui.pTreeNodes->setHeaderLabel("Type");
        m_ui.pTreeNodes->header()->setSortIndicator(0, Qt::AscendingOrder);

        std::list< env::ENTITY_TYPEID > metaTypenames;
        env::CSceneNode* pRootNode = env::gEngine->GetSceneMgr()->GetRootNode();

        QTreeWidgetItem* pRootNodeItem = new QTreeWidgetItem();
        pRootNodeItem->setText(0, pRootNode->GetName().c_str());
        pRootNodeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        m_ui.pTreeNodes->addTopLevelItem(pRootNodeItem);

        m_topLevelItems.push_back(pRootNodeItem);

        AddNodesRecursive(pRootNode, pRootNodeItem);

        for (const auto& node : env::gEngine->GetSceneMgr()->GetSceneNodes())
        {
            if (node.second->GetParentNode() ||
                node.second == env::gEngine->GetSceneMgr()->GetRootNode())
            {
                continue;
            }

            QTreeWidgetItem* pRootNodeItem = new QTreeWidgetItem();
            if (node.second->GetDebugName().empty())
                pRootNodeItem->setText(0, node.second->GetName().c_str());
            else
                pRootNodeItem->setText(0, node.second->GetDebugName().c_str());
            pRootNodeItem->setData(0, Qt::UserRole, node.second->GetName().c_str());
            pRootNodeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

            m_ui.pTreeNodes->addTopLevelItem(pRootNodeItem);

            m_topLevelItems.push_back(pRootNodeItem);
        }
    }

public slots:

    //-----------------------------------------------------------------------------------
    virtual void slotItemClicked(QTreeWidgetItem* pItem, int column)
    {
    }

    //-----------------------------------------------------------------------------------
    virtual void slotItemDoubleClicked(QTreeWidgetItem* pItem, int column)
    {
    }

    //-----------------------------------------------------------------------------------
    virtual void slotDeleteSelection()
    {
        QList< QTreeWidgetItem* > selectedItems = m_ui.pTreeNodes->selectedItems();

        for (int i = 0; i < selectedItems.size(); ++i)
        {
            QTreeWidgetItem* pTreeItem = selectedItems.at(i);

            const QString& strName = pTreeItem->data(0, Qt::UserRole).toString();

            if (env::gEngine->GetSceneMgr()->RemoveNode(strName.toStdString()))
            {
                m_ui.pTreeNodes->removeItemWidget(pTreeItem, 0);
            }
        }

        m_ui.pTreeNodes->update();

        UpdateUI();
    }

private:

    typedef std::list< QTreeWidgetItem* > listTopLevelItems;
    listTopLevelItems m_topLevelItems;

    bool m_bBlockPaneUpdate;
    bool m_bNeedPaneUpdate;

};