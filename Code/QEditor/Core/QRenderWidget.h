/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplitter>
#include <QtGui/QMouseEvent>

#undef UNICODE
#include <Engine/Core/Engine.h>
#include <Engine/Entity/EntityMgr.h>
#include <Engine/Core/Input.h>
#include <Graphic/RenderSystems/RenderSystem.h>
#define UNICODE 

class QRenderWidget : public QWidget
{
    Q_OBJECT

public:

    QRenderWidget(QWidget *parent = 0)
        : QWidget(parent)
        , m_pEngine(0)
    {
        setAttribute(Qt::WA_OpaquePaintEvent, true);
        setFocusPolicy(Qt::StrongFocus);
    }

    ~QRenderWidget() {}

    void setEngine(env::CEngine* pEngine, QSplitter* pMainSplitter)
    {
        m_pEngine = pEngine;

        m_pMainSplitter = pMainSplitter;
    }

    void Update()
    {
    }

protected:

    //-----------------------------------------------------------------------------------
    virtual void keyPressEvent(QKeyEvent* event)
    {
        if (m_pEngine &&
            m_pEngine->GetInput())
        {
            m_pEngine->GetInput()->KeyDownEvent(event->nativeVirtualKey());
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void keyReleaseEvent(QKeyEvent* event)
    {
        if (m_pEngine &&
            m_pEngine->GetInput())
        {
            m_pEngine->GetInput()->KeyUpEvent(event->nativeVirtualKey());
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void mousePressEvent(QMouseEvent * event)
    {
        if (!m_pEngine ||
            !m_pEngine->GetInput())
        {
            return;
        }

        if (event->buttons() & Qt::MidButton)
            m_pEngine->GetInput()->MouseDownEvent(2);

        if (m_bFocus)
        {
            if (event->buttons() & Qt::LeftButton)
                m_pEngine->GetInput()->MouseDownEvent(0);
            else if (event->buttons() & Qt::RightButton)
                m_pEngine->GetInput()->MouseDownEvent(1);
        }

        m_bFocus = true;
    }

    //-----------------------------------------------------------------------------------
    virtual void mouseReleaseEvent(QMouseEvent * event)
    {
        if (!m_pEngine ||
            !m_pEngine->GetInput())
        {
            return;
        }

        if (event->button() == Qt::LeftButton)
            m_pEngine->GetInput()->MouseUpEvent(0);
        else if (event->button() == Qt::RightButton)
            m_pEngine->GetInput()->MouseUpEvent(1);
        else if (event->button() == Qt::MidButton)
            m_pEngine->GetInput()->MouseUpEvent(2);
    }

    //-----------------------------------------------------------------------------------
    virtual void wheelEvent(QWheelEvent * event)
    {
        if (m_pEngine &&
            m_pEngine->GetInput())
        {
            m_pEngine->GetInput()->ScrollEvent(event->delta() / 40);
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void resizeEvent(QResizeEvent * event)
    {
        if (m_pEngine)
        {
            env::gRenderSystem->GetRenderQualityParams().screenWidth = event->size().width();
            env::gRenderSystem->GetRenderQualityParams().screenHeight = event->size().height();
            m_pEngine->ResetDevice(env::gRenderSystem->GetRenderQualityParams());
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void focusInEvent(QFocusEvent * event)
    {
        //m_pMainSplitter->setStyleSheet("QSplitter::handle {background-color:#915e38; border:1px solid #6e788a; margin-left:1px;margin-right:1px; }");

        m_bFocus = false;
    }

    //-----------------------------------------------------------------------------------
    virtual void focusOutEvent(QFocusEvent * event)
    {
        //m_pMainSplitter->setStyleSheet("QSplitter::handle {background-color:#555a63; border:1px solid #6e788a; margin-left:1px;margin-right:1px; }");

        if (m_pEngine &&
            m_pEngine->GetInput())
        {
            m_pEngine->GetInput()->Reset();
        }
    }

    //-----------------------------------------------------------------------------------
    virtual void paintEvent(QPaintEvent * event) 
    {
        event->ignore(); 
    }

    QPaintEngine * paintEngine() const { return 0; }

private:

    env::CEngine* m_pEngine;

    QSplitter* m_pMainSplitter;

    bool m_bFocus;

};