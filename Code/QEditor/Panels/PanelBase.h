/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QEditor/Core/QEditor.h>
#include <QEditor/Core/Utility.h>

class QWidget;

namespace env
{
    class CEntity;
} // env

class CPanelBase : public QObject
{
    Q_OBJECT

public:
    CPanelBase() : QObject() {}

    virtual ~CPanelBase() {}

    virtual void syncEntityEvent(env::CEntity* pEntity) = 0;

    virtual QWidget* GetWidget() const = 0;

    virtual QString GetTitle() const {
        return QString();
    };
};

template< class T >
class CPanel : public CPanelBase
{
public:
    CPanel(QWidget* pForm)
    {
        m_ui.setupUi(pForm);
    }

    //virtual ~CPanel() {}

    virtual QWidget* GetWidget() const { return m_ui.qEditorPanelBox; }

    virtual QString GetTitle() const { return m_ui.qEditorPanelBox->title(); }

protected:
    T m_ui;
};
