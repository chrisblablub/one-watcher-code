/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/UIGen/EntityOutputPanel_ui.h>
#include <QEditor/Panels/PanelBase.h>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>

#undef UNICODE
#include <Engine/Entity/Entity.h>
#include <Engine/Entity/EntityMgr.h>
#define UNICODE 


class CEntityOutputPanel : public CPanel<Ui::EntityOutputPanel>
{
    Q_OBJECT

public:
    CEntityOutputPanel(QWidget* pForm)
        : CPanel<Ui::EntityOutputPanel>(pForm)
        , m_pEntity(0)
    {
        connect(m_ui.pTableOutputs, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotOutputSelectionChanged()));
        connect(m_ui.pComboOutputReceiver, SIGNAL(currentIndexChanged(int)), this, SLOT(slotReceiverChanged()));
        connect(m_ui.pBtnEditOutput, SIGNAL(pressed()), this, SLOT(slotBtnEditSelectionClicked()));
        connect(m_ui.pBtnDeleteOutput, SIGNAL(pressed()), this, SLOT(slotBtnDeleteSelectionClicked()));
        connect(m_ui.pBtnAddOutput, SIGNAL(pressed()), this, SLOT(slotBtnAddOutputClicked()));
        connect(m_ui.pComboOutputEvent, SIGNAL(currentIndexChanged(int)), this, SLOT(slotInitEventParameters()));
        connect(m_ui.pBtnOffsetOutputs, SIGNAL(pressed()), this, SLOT(slotBtnOffsetOutputsClicked()));
        connect(m_ui.pSpinBoxDebugOffset, SIGNAL(valueChanged(double)), this, SLOT(slotDebugOffsetChanged()));
        
        
        QWidget* widgetForScrolling = new QWidget(m_ui.pScrollAreaEventParams);

        m_pLayout = new QGridLayout(widgetForScrolling);

        widgetForScrolling->setLayout(m_pLayout);

        m_ui.pScrollAreaEventParams->setWidget(widgetForScrolling);
    }

    //-----------------------------------------------------------------------------------
    virtual void syncEntityEvent(env::CEntity* pEntity)
    {
        if (!pEntity)
            return;

        m_pEntity = pEntity;

        initOutputs();

        initReceivers();

        clearEventParamWidgets();
    }

    //-----------------------------------------------------------------------------------
    void initOutputs()
    {
        env::CEntity::mapOutputs& outputs = m_pEntity->GetOutputs();

        m_keys.clear();

        m_ui.pTableOutputs->reset();
        m_ui.pTableOutputs->clear();
        m_ui.pTableOutputs->setRowCount(0);
        m_ui.pTableOutputs->setColumnCount(4);
        m_ui.pTableOutputs->setHorizontalHeaderItem(0, new QTableWidgetItem("Receiver"));
        m_ui.pTableOutputs->setHorizontalHeaderItem(1, new QTableWidgetItem("Event"));
        m_ui.pTableOutputs->setHorizontalHeaderItem(2, new QTableWidgetItem("Delay"));
        m_ui.pTableOutputs->setHorizontalHeaderItem(3, new QTableWidgetItem("Key"));


        std::list<env::CEntity::EntityOutput> outputsSorted;

        for (env::CEntity::mapOutputs::iterator it = outputs.begin(); it != outputs.end(); ++it)
        {
            env::CEntity::listReceiverOutputs& receiverOutputs = it->second;

            for (env::CEntity::listReceiverOutputs::iterator jt = receiverOutputs.begin(); jt != receiverOutputs.end(); ++jt)
            {
                outputsSorted.push_back((*jt));
            }
        }


        outputsSorted.sort();

        for (env::CEntity::EntityOutput& output : outputsSorted)
        {
            //env::CEntity::EntityOutput& output = *pOutput;

            std::string strEventName;
            output.pReceiver->GetEventName(strEventName, output.eventId);
            
            m_ui.pTableOutputs->insertRow(m_ui.pTableOutputs->rowCount());
            m_ui.pTableOutputs->setItem(m_ui.pTableOutputs->rowCount() - 1, 0, new QTableWidgetItem(output.pReceiver->GetName().c_str()));
            m_ui.pTableOutputs->setItem(m_ui.pTableOutputs->rowCount() - 1, 1, new QTableWidgetItem(strEventName.c_str()));
            m_ui.pTableOutputs->setItem(m_ui.pTableOutputs->rowCount() - 1, 2, new QTableWidgetItem(QString::number(output.delay)));
            m_ui.pTableOutputs->setItem(m_ui.pTableOutputs->rowCount() - 1, 3, new QTableWidgetItem(QString::number(output.key)));

            m_keys.append(output.key);
        }

//         for (env::CEntity::mapOutputs::iterator it = outputs.begin(); it != outputs.end(); ++it)
//         {
//             env::CEntity::listReceiverOutputs& receiverOutputs = it->second;
// 
//             receiverOutputs.sort();
// 
//             for (env::CEntity::listReceiverOutputs::iterator jt = receiverOutputs.begin(); jt != receiverOutputs.end(); ++jt)
//             {
//                 env::CEntity::EntityOutput& output = *jt;
// 
//                 std::string strEventName;
//                 output.pReceiver->GetEventName(strEventName, output.eventId);
// 
//                 m_ui.pTableOutputs->insertRow(m_ui.pTableOutputs->rowCount());
//                 m_ui.pTableOutputs->setItem(m_ui.pTableOutputs->rowCount() - 1, 0, new QTableWidgetItem(it->first.c_str()));
//                 m_ui.pTableOutputs->setItem(m_ui.pTableOutputs->rowCount() - 1, 1, new QTableWidgetItem(strEventName.c_str()));
//                 m_ui.pTableOutputs->setItem(m_ui.pTableOutputs->rowCount() - 1, 2, new QTableWidgetItem(QString::number(output.delay)));
//                 m_ui.pTableOutputs->setItem(m_ui.pTableOutputs->rowCount() - 1, 3, new QTableWidgetItem(QString::number(output.key)));
// 
//                 m_keys.append(output.key);
//             }
//         }

        m_ui.pTableOutputs->resizeColumnsToContents();
    }

    //-----------------------------------------------------------------------------------
    void initReceivers(const QString strSelection = "")
    {
        std::list<env::CEntity*> entities;
        env::gEngine->GetEntityMgr()->GetEntities(entities);

        m_ui.pComboOutputReceiver->clear();

        for (auto it = entities.begin(); it != entities.end(); ++it)
        {
            const std::string& strEntityName = (*it)->GetName();

            if (strEntityName == m_pEntity->GetName() || (*it)->GetPickable() == false)
                continue;

            m_ui.pComboOutputReceiver->addItem(strEntityName.c_str());
        }

        m_ui.pComboOutputReceiver->setCurrentText(strSelection);
    }

    //-----------------------------------------------------------------------------------
    void initEvents(const QString& strReceiver)
    {
        m_ui.pComboOutputEvent->clear();

        env::CEntity* pEntity = env::gEngine->GetEntityMgr()->GetEntity(strReceiver.toStdString());

        if (!pEntity)
            return;

        const env::CEntity::mapEvents& events = pEntity->GetEvents();

        for (env::CEntity::mapEvents::const_iterator it = events.begin(); it != events.end(); ++it)
        {
            m_ui.pComboOutputEvent->addItem(it->second.m_strEventName.c_str());
        }
    }

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    private:

        template<class Widget, class T>
        void initControl(Widget* pWidget, const T& initial, const T& minValue, const T& maxValue)
        {
            pWidget->setValue(initial);
            pWidget->setMinimum(minValue);
            pWidget->setMaximum(maxValue);
        }

    public slots:

    //-----------------------------------------------------------------------------------
    int GetSelectedEventKey()
    {
        if (m_ui.pTableOutputs->currentRow() >= 0 &&
            m_ui.pTableOutputs->currentRow() < m_keys.size())
        {
            return m_keys.at(m_ui.pTableOutputs->currentRow());
        }

        return 0;
    }

    //-----------------------------------------------------------------------------------
    env::CEntity* GetSelectedReceiver()
    {
        QTableWidgetItem* pItem = m_ui.pTableOutputs->item(m_ui.pTableOutputs->currentRow(), 0);
        if (!pItem)
            return nullptr;

        return env::gEngine->GetEntityMgr()->GetEntity(pItem->text().toStdString());
    }

    //-----------------------------------------------------------------------------------
    void slotDebugOffsetChanged()
    {
        m_pEntity->SetFireDebugOffset(m_ui.pSpinBoxDebugOffset->value());
    }

    //-----------------------------------------------------------------------------------
    void slotBtnOffsetOutputsClicked()
    {
        float offsetFrom = m_ui.pSpinBoxOffsetFrom->value();
        float offsetBy = m_ui.pSpinBoxOffsetBy->value();

        env::CEntity::mapOutputs& outputs = m_pEntity->GetOutputs();


        for (env::CEntity::mapOutputs::iterator it = outputs.begin(); it != outputs.end(); ++it)
        {
            env::CEntity::listReceiverOutputs& receiverOutputs = it->second;

            for (env::CEntity::listReceiverOutputs::iterator jt = receiverOutputs.begin(); jt != receiverOutputs.end(); ++jt)
            {
                if (jt->delay >= offsetFrom)
                    jt->delay += offsetBy;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    bool GetSelectedReceiverEventKey(env::CEntity** ppReceiver, int& eventKey)
    {
        *ppReceiver = GetSelectedReceiver();

        eventKey = GetSelectedEventKey();

        if (*ppReceiver == nullptr ||
            eventKey < 0)
        {
            return false;
        }

        return true;
    }

    unsigned int GetSelectedEventID()
    {
        env::CEntity* pReceiver = GetCurrentReceiver();

        QString eventName = m_ui.pComboOutputEvent->currentText();

        unsigned int eventID = 0;
        pReceiver->GetEventID(eventName.toStdString(), eventID);

        return eventID;
    }

    //-----------------------------------------------------------------------------------
    env::CEntity* GetCurrentReceiver()
    {
        QString strReceiver = m_ui.pComboOutputReceiver->currentText();

        if (strReceiver.isEmpty())
            return nullptr;

        return env::gEngine->GetEntityMgr()->GetEntity(strReceiver.toStdString());
    }

    //-----------------------------------------------------------------------------------
    void clearEventParamWidgets()
    {
        while (m_pLayout->count() > 0)
        {
            m_pLayout->removeWidget(m_pLayout->itemAt(0)->widget());
        }

        QObjectList objects = m_ui.pScrollAreaEventParams->widget()->children();

        for (int i = 0; i < objects.size(); ++i)
        {
            QObject* p = objects.at(i);

            if (dynamic_cast<QDoubleSpinBox*>(p) ||
                dynamic_cast<QSpinBox*>(p) ||
                dynamic_cast<QLineEdit*>(p) ||
                dynamic_cast<QCheckBox*>(p) ||
                dynamic_cast<QLabel*>(p))
            {
                m_pLayout->removeWidget((QWidget*)p);
                ENV_DELETE(p);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void slotInitEventParameters()
    {
        clearEventParamWidgets();

        unsigned int eventID = GetSelectedEventID();

        env::CEntity* pSelectedReceiver = GetCurrentReceiver();
        //env::CEntity* pSelectedReceiver = GetSelectedReceiver();

        if (!pSelectedReceiver)
            return;

        env::CEntity::mapEventParameterDescs eventParamDescs;
        pSelectedReceiver->GetEventParameter(eventID, eventParamDescs);

        int i = 0;
        for (env::CEntity::mapEventParameterDescs::iterator it = eventParamDescs.begin(); it != eventParamDescs.end(); ++it, ++i)
        {
            const env::CEntity::EventParamDesc& eventParamDesc = *it;

            const std::string strParamName = eventParamDesc.m_strName;
            const std::string strParamDesc = eventParamDesc.m_strDescription;
            const env::CBaseParam::PARAMETER_TYPE paramType = eventParamDesc.m_paramType;
            const env::CBaseParam& initialValue = eventParamDesc.m_initialValue;
            const env::CBaseParam& minValue = eventParamDesc.m_minValue;
            const env::CBaseParam& maxValue = eventParamDesc.m_maxValue;

            QLabel* pLabel = new QLabel(strParamName.c_str());
            pLabel->setToolTip(strParamDesc.c_str());
            m_pLayout->addWidget(pLabel, i, 0, 1, 1);

            switch (paramType)
            {
            case env::CBaseParam::PARAMETER_TYPE_FLOAT:
            {
                QDoubleSpinBox* pSpinBox = new QDoubleSpinBox();
                pSpinBox->setObjectName(strParamName.c_str());
                pSpinBox->setToolTip(strParamDesc.c_str());
                pSpinBox->setSingleStep(0.1);

                initControl(pSpinBox,
                            initialValue.IsValid() ? (float)initialValue : 0.0f,
                            minValue.IsValid() ? (float)minValue : (float)pSpinBox->minimum(),
                            maxValue.IsValid() ? (float)maxValue : (float)pSpinBox->maximum());

                if (pSelectedReceiver)
                {
                    env::CEntity::EntityOutput output;
                    m_pEntity->GetOutput(output, pSelectedReceiver->GetName(), GetSelectedEventKey());

                    env::CEntity::mapEventParameter::iterator it = output.m_parameter.find(strParamName);
                    if (it != output.m_parameter.end())
                        pSpinBox->setValue(it->second);
                }

                m_pLayout->addWidget(pSpinBox, i, 1, 1, 1);

            } break;

            case env::CBaseParam::PARAMETER_TYPE_VECTOR2:
            {
                QDoubleSpinBox* pSpinBoxX = new QDoubleSpinBox();
                pSpinBoxX->setObjectName(strParamName.c_str());
                pSpinBoxX->setToolTip(strParamDesc.c_str());
                pSpinBoxX->setSingleStep(0.1);

                QDoubleSpinBox* pSpinBoxY = new QDoubleSpinBox();
                pSpinBoxY->setObjectName(strParamName.c_str());
                pSpinBoxY->setToolTip(strParamDesc.c_str());
                pSpinBoxY->setSingleStep(0.1);

                initControl(pSpinBoxX,
                            initialValue.IsValid() ? ((env::Vec2)initialValue).x : 0.0f,
                            minValue.IsValid() ? ((env::Vec2)minValue).x : (float)pSpinBoxX->minimum(),
                            maxValue.IsValid() ? ((env::Vec2)maxValue).y : (float)pSpinBoxX->maximum());

                initControl(pSpinBoxY,
                            initialValue.IsValid() ? ((env::Vec2)initialValue).y : 0.0f,
                            minValue.IsValid() ? ((env::Vec2)minValue).x : (float)pSpinBoxY->minimum(),
                            maxValue.IsValid() ? ((env::Vec2)maxValue).y : (float)pSpinBoxY->maximum());

                if (pSelectedReceiver)
                {
                    env::CEntity::EntityOutput output;
                    m_pEntity->GetOutput(output, pSelectedReceiver->GetName(), GetSelectedEventKey());

                    env::CEntity::mapEventParameter::iterator it = output.m_parameter.find(strParamName);
                    if (it != output.m_parameter.end())
                    {
                        env::Vec2 v = it->second;
                        pSpinBoxX->setValue(v.x);
                        pSpinBoxY->setValue(v.y);
                    }
                }

                m_pLayout->addWidget(pSpinBoxX, i, 1, 1, 1);
                m_pLayout->addWidget(pSpinBoxY, i, 2, 1, 1);

            } break;

            case env::CBaseParam::PARAMETER_TYPE_VECTOR3:
            {
                QDoubleSpinBox* pSpinBoxX = new QDoubleSpinBox();
                pSpinBoxX->setObjectName(strParamName.c_str());
                pSpinBoxX->setToolTip(strParamDesc.c_str());
                pSpinBoxX->setSingleStep(0.1);

                QDoubleSpinBox* pSpinBoxY = new QDoubleSpinBox();
                pSpinBoxY->setObjectName(strParamName.c_str());
                pSpinBoxY->setToolTip(strParamDesc.c_str());
                pSpinBoxY->setSingleStep(0.1);

                QDoubleSpinBox* pSpinBoxZ = new QDoubleSpinBox();
                pSpinBoxZ->setObjectName(strParamName.c_str());
                pSpinBoxZ->setToolTip(strParamDesc.c_str());
                pSpinBoxZ->setSingleStep(0.1);

                initControl(pSpinBoxX,
                            initialValue.IsValid() ? ((env::Vec3)initialValue).x : 0.0f,
                            minValue.IsValid() ? ((env::Vec3)minValue).x : (float)pSpinBoxX->minimum(),
                            maxValue.IsValid() ? ((env::Vec3)maxValue).x : (float)pSpinBoxX->maximum());

                initControl(pSpinBoxY,
                            initialValue.IsValid() ? ((env::Vec3)initialValue).y : 0.0f,
                            minValue.IsValid() ? ((env::Vec3)minValue).y : (float)pSpinBoxY->minimum(),
                            maxValue.IsValid() ? ((env::Vec3)maxValue).y : (float)pSpinBoxY->maximum());

                initControl(pSpinBoxZ,
                            initialValue.IsValid() ? ((env::Vec3)initialValue).z : 0.0f,
                            minValue.IsValid() ? ((env::Vec3)minValue).z : (float)pSpinBoxZ->minimum(),
                            maxValue.IsValid() ? ((env::Vec3)maxValue).z : (float)pSpinBoxZ->maximum());

                if (pSelectedReceiver)
                {
                    env::CEntity::EntityOutput output;
                    m_pEntity->GetOutput(output, pSelectedReceiver->GetName(), GetSelectedEventKey());
                    
                    env::CEntity::mapEventParameter::iterator it = output.m_parameter.find(strParamName);
                    if (it != output.m_parameter.end())
                    {
                        env::Vec3 v = it->second;
                        pSpinBoxX->setValue(v.x);
                        pSpinBoxY->setValue(v.y);
                        pSpinBoxZ->setValue(v.z);
                    }
                }

                m_pLayout->addWidget(pSpinBoxX, i, 1, 1, 1);
                m_pLayout->addWidget(pSpinBoxY, i, 2, 1, 1);
                m_pLayout->addWidget(pSpinBoxZ, i, 3, 1, 1);

            } break;

            case env::CBaseParam::PARAMETER_TYPE_VECTOR4:
            {
                QDoubleSpinBox* pSpinBoxX = new QDoubleSpinBox();
                pSpinBoxX->setObjectName(strParamName.c_str());
                pSpinBoxX->setToolTip(strParamDesc.c_str());
                pSpinBoxX->setSingleStep(0.1);

                QDoubleSpinBox* pSpinBoxY = new QDoubleSpinBox();
                pSpinBoxY->setObjectName(strParamName.c_str());
                pSpinBoxY->setToolTip(strParamDesc.c_str());
                pSpinBoxY->setSingleStep(0.1);

                QDoubleSpinBox* pSpinBoxZ = new QDoubleSpinBox();
                pSpinBoxZ->setObjectName(strParamName.c_str());
                pSpinBoxZ->setToolTip(strParamDesc.c_str());
                pSpinBoxZ->setSingleStep(0.1);

                QDoubleSpinBox* pSpinBoxW = new QDoubleSpinBox();
                pSpinBoxW->setObjectName(strParamName.c_str());
                pSpinBoxW->setToolTip(strParamDesc.c_str());
                pSpinBoxW->setSingleStep(0.1);

                initControl(pSpinBoxX,
                            initialValue.IsValid() ? ((env::Vec4)initialValue).x : 0.0f,
                            minValue.IsValid() ? ((env::Vec4)minValue).x : (float)pSpinBoxX->minimum(),
                            maxValue.IsValid() ? ((env::Vec4)maxValue).x : (float)pSpinBoxX->maximum());

                initControl(pSpinBoxY,
                            initialValue.IsValid() ? ((env::Vec4)initialValue).y : 0.0f,
                            minValue.IsValid() ? ((env::Vec4)minValue).y : (float)pSpinBoxY->minimum(),
                            maxValue.IsValid() ? ((env::Vec4)maxValue).y : (float)pSpinBoxY->maximum());

                initControl(pSpinBoxZ,
                            initialValue.IsValid() ? ((env::Vec4)initialValue).z : 0.0f,
                            minValue.IsValid() ? ((env::Vec4)minValue).z : (float)pSpinBoxZ->minimum(),
                            maxValue.IsValid() ? ((env::Vec4)maxValue).z : (float)pSpinBoxZ->maximum());

                initControl(pSpinBoxW,
                            initialValue.IsValid() ? ((env::Vec4)initialValue).w : 0.0f,
                            minValue.IsValid() ? ((env::Vec4)minValue).w : (float)pSpinBoxW->minimum(),
                            maxValue.IsValid() ? ((env::Vec4)maxValue).w : (float)pSpinBoxW->maximum());

                if (pSelectedReceiver)
                {
                    env::CEntity::EntityOutput output;
                    m_pEntity->GetOutput(output, pSelectedReceiver->GetName(), GetSelectedEventKey());

                    env::CEntity::mapEventParameter::iterator it = output.m_parameter.find(strParamName);
                    if (it != output.m_parameter.end())
                    {
                        env::Vec4 v = it->second;
                        pSpinBoxX->setValue(v.x);
                        pSpinBoxY->setValue(v.y);
                        pSpinBoxZ->setValue(v.z);
                        pSpinBoxW->setValue(v.w);
                    }
                }

                m_pLayout->addWidget(pSpinBoxX, i, 1, 1, 1);
                m_pLayout->addWidget(pSpinBoxY, i, 2, 1, 1);
                m_pLayout->addWidget(pSpinBoxZ, i, 3, 1, 1);
                m_pLayout->addWidget(pSpinBoxW, i, 4, 1, 1);

            } break;

            case env::CBaseParam::PARAMETER_TYPE_INT:
            {
                QSpinBox* pSpinBox = new QSpinBox();
                pSpinBox->setObjectName(strParamName.c_str());
                pSpinBox->setToolTip(strParamDesc.c_str());

                initControl(pSpinBox,
                            initialValue.IsValid() ? (int)initialValue : 0,
                            minValue.IsValid() ? (int)minValue : (int)pSpinBox->minimum(),
                            maxValue.IsValid() ? (int)maxValue : (int)pSpinBox->maximum());

                if (pSelectedReceiver)
                {
                    env::CEntity::EntityOutput output;
                    m_pEntity->GetOutput(output, pSelectedReceiver->GetName(), GetSelectedEventKey());

                    env::CEntity::mapEventParameter::iterator it = output.m_parameter.find(strParamName);
                    if (it != output.m_parameter.end())
                        pSpinBox->setValue(it->second);
                }

                m_pLayout->addWidget(pSpinBox, i, 1, 1, 1);

            } break;

            case env::CBaseParam::PARAMETER_TYPE_STRING:
            {
                QLineEdit* pSpinBox = new QLineEdit();
                pSpinBox->setObjectName(strParamName.c_str());
                pSpinBox->setToolTip(strParamDesc.c_str());

                if (initialValue.IsValid())
                    pSpinBox->setText(((std::string)initialValue).c_str());

                if (pSelectedReceiver)
                {
                    env::CEntity::EntityOutput output;
                    m_pEntity->GetOutput(output, pSelectedReceiver->GetName(), GetSelectedEventKey());

                    env::CEntity::mapEventParameter::iterator it = output.m_parameter.find(strParamName);
                    if (it != output.m_parameter.end())
                        pSpinBox->setText(((std::string)it->second).c_str());
                }

                m_pLayout->addWidget(pSpinBox, i, 1, 1, 1);

            } break;

            case env::CBaseParam::PARAMETER_TYPE_BOOL:
            {
                QCheckBox* pSpinBox = new QCheckBox();
                pSpinBox->setObjectName(strParamName.c_str());
                pSpinBox->setToolTip(strParamDesc.c_str());

                if (initialValue.IsValid())
                    pSpinBox->setChecked((bool)initialValue);

                if (pSelectedReceiver)
                {
                    env::CEntity::EntityOutput output;
                    m_pEntity->GetOutput(output, pSelectedReceiver->GetName(), GetSelectedEventKey());

                    env::CEntity::mapEventParameter::iterator it = output.m_parameter.find(strParamName);
                    if (it != output.m_parameter.end())
                        pSpinBox->setChecked(it->second);
                }

                m_pLayout->addWidget(pSpinBox, i, 1, 1, 1);

            } break;
            }
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void slotOutputSelectionChanged()
    {
        env::CEntity* pReceiver = nullptr;
        int key = 0;

        if (!GetSelectedReceiverEventKey(&pReceiver, key))
            return;

        env::CEntity::EntityOutput output;
        m_pEntity->GetOutput(output, pReceiver->GetName(), key);

        m_ui.pSpinBoxOutputDelay->setValue(output.delay);
        m_ui.pComboOutputReceiver->setCurrentText(pReceiver->GetName().c_str());

        std::string strEventName;
        pReceiver->GetEventName(strEventName, output.eventId);
        m_ui.pComboOutputEvent->setCurrentText(strEventName.c_str());

        //slotInitEventParameters();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotReceiverChanged()
    {
        env::CEntity* pReceiver = GetCurrentReceiver();

        if (pReceiver == nullptr)
            return;

        initEvents(pReceiver->GetName().c_str());
    }

    //-----------------------------------------------------------------------------------
    virtual void slotBtnEditSelectionClicked()
    {
        env::CEntity* pReceiver = nullptr;
        int key = 0;

        if (!GetSelectedReceiverEventKey(&pReceiver, key))
            return;

        env::gEngine->GetEntityMgr()->RemoveEntityOutput(m_pEntity, pReceiver->GetName(), key);

        slotBtnAddOutputClicked();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotBtnDeleteSelectionClicked()
    {
        int key = 0;
        env::CEntity* pReceiver = nullptr;

        if (!GetSelectedReceiverEventKey(&pReceiver, key))
            return;

        env::gEngine->GetEntityMgr()->RemoveEntityOutput(m_pEntity, pReceiver->GetName(), key);

        initOutputs();
    }

    //-----------------------------------------------------------------------------------
    virtual void slotBtnAddOutputClicked()
    {
        env::CEntity* pReceiver = GetCurrentReceiver();

        if (pReceiver == nullptr)
            return;

        unsigned int eventID = 0;
        if (!pReceiver->GetEventID(m_ui.pComboOutputEvent->currentText().toStdString(), eventID))
            return;

        env::CEntity::EntityOutput output(pReceiver, eventID, m_ui.pSpinBoxOutputDelay->value());
        env::CEntity::mapEventParameterDescs paramDescs;
        pReceiver->GetEventParameter(eventID, paramDescs);

        for(int i = 0; i < m_pLayout->rowCount(); ++i)
        {
            if (!m_pLayout->itemAtPosition(i, 1))
                continue;

            QWidget* p = m_pLayout->itemAtPosition(i, 1)->widget();
            std::string strObjectName = p->objectName().toStdString();

            env::CEntity::mapEventParameterDescs::iterator it = paramDescs.begin();
            
            while (it != paramDescs.end())
            {
                if ((*it).m_strName == strObjectName)
                    break;

                ++it;
            }

            if (it == paramDescs.end())
                continue;

            env::CBaseParam::PARAMETER_TYPE paramType = (*it).m_paramType;
            env::CBaseParam baseParam;

            switch (paramType)
            {
            case env::CBaseParam::PARAMETER_TYPE_FLOAT:
            {
                QDoubleSpinBox* pSpinBox = dynamic_cast<QDoubleSpinBox*>(p);
                baseParam = (float)pSpinBox->value();
            } break;

            case env::CBaseParam::PARAMETER_TYPE_INT:
            {
                QSpinBox* pSpinBox = dynamic_cast<QSpinBox*>(p);
                baseParam = (int)pSpinBox->value();
            } break;

            case env::CBaseParam::PARAMETER_TYPE_STRING:
            {
                QLineEdit* pSpinBox = dynamic_cast<QLineEdit*>(p);
                baseParam = pSpinBox->text().toStdString();
            } break;

            case env::CBaseParam::PARAMETER_TYPE_BOOL:
            {
                QCheckBox* pSpinBox = dynamic_cast<QCheckBox*>(p);
                baseParam = pSpinBox->isChecked();
            } break;

            case env::CBaseParam::PARAMETER_TYPE_VECTOR2:
            {
                QDoubleSpinBox* pSpinBoxX = dynamic_cast<QDoubleSpinBox*>(m_pLayout->itemAtPosition(i, 1)->widget());
                QDoubleSpinBox* pSpinBoxY = dynamic_cast<QDoubleSpinBox*>(m_pLayout->itemAtPosition(i, 2)->widget());

                baseParam = env::Vec2(pSpinBoxX->value(), pSpinBoxY->value());
            } break;

            case env::CBaseParam::PARAMETER_TYPE_VECTOR3:
            {
                QDoubleSpinBox* pSpinBoxX = dynamic_cast<QDoubleSpinBox*>(m_pLayout->itemAtPosition(i, 1)->widget());
                QDoubleSpinBox* pSpinBoxY = dynamic_cast<QDoubleSpinBox*>(m_pLayout->itemAtPosition(i, 2)->widget());
                QDoubleSpinBox* pSpinBoxZ = dynamic_cast<QDoubleSpinBox*>(m_pLayout->itemAtPosition(i, 3)->widget());

                baseParam = env::Vec3(pSpinBoxX->value(), pSpinBoxY->value(), pSpinBoxZ->value());
            } break;

            case env::CBaseParam::PARAMETER_TYPE_VECTOR4:
            {
                QDoubleSpinBox* pSpinBoxX = dynamic_cast<QDoubleSpinBox*>(m_pLayout->itemAtPosition(i, 1)->widget());
                QDoubleSpinBox* pSpinBoxY = dynamic_cast<QDoubleSpinBox*>(m_pLayout->itemAtPosition(i, 2)->widget());
                QDoubleSpinBox* pSpinBoxZ = dynamic_cast<QDoubleSpinBox*>(m_pLayout->itemAtPosition(i, 3)->widget());
                QDoubleSpinBox* pSpinBoxW = dynamic_cast<QDoubleSpinBox*>(m_pLayout->itemAtPosition(i, 4)->widget());

                baseParam = env::Vec4(pSpinBoxX->value(), pSpinBoxY->value(), pSpinBoxZ->value(), pSpinBoxW->value());
            } break;
            }

            output.m_parameter[it->m_strName] = baseParam;
        }


        env::gEngine->GetEntityMgr()->AddEntityOutput(m_pEntity, output);

        initOutputs();
    }

private:

    env::CEntity* m_pEntity;

    QList< int > m_keys;
    QGridLayout* m_pLayout;
};