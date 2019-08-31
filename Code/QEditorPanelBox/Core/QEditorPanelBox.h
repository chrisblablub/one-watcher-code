/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */

#ifndef QEDITORPANELBOX_H
#define QEDITORPANELBOX_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtGui/QFont>
#include "DLLExportImport.h"


class DLL_EXPORT QEditorPanelBox : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(QString m_strTitle READ title WRITE setTitle)

public:
    QEditorPanelBox(QWidget *parent = 0);
    ~QEditorPanelBox();

    QString title() const { return m_strTitle; }
    void setTitle(const QString& strTitle) { m_strTitle = strTitle; m_pTitleBtn->setText(m_strTitle); }

    void showPanel(bool bToggle) { m_pVisibilityBtn->setChecked(!bToggle); }

    QWidget* GetContentWidget() const { return m_pContent; }
    QGridLayout* GetGridLayout() const { return m_pGridLayout; }

public slots:
    virtual void slotToggleVisibility(bool bHide);

private:

    QPushButton* m_pTitleBtn;
    QPushButton* m_pVisibilityBtn;
    QWidget*     m_pContent;
    QGridLayout* m_pGridLayout;

    int m_contentHeight;

    QString m_strTitle;
};

#endif // QEDITORPANELBOX_H
