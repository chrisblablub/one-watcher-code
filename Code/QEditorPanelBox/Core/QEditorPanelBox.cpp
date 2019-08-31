#include "QEditorPanelBox.h"

QEditorPanelBox::QEditorPanelBox(QWidget *parent)
    : QWidget(parent)
{
    QString strGroupBoxStyle =
        "QGroupBox{"
        "border: 1px solid #444444;"
        "border - radius: 0px;"
        "border - top:1px solid #222222;"
        "border - bottom:4px solid #222222;"
        "background:rgba(45, 45, 45, 255);"
        "}";

    QString strTitleBtnStyle =
        "border: 1px solid #202328;"
        "border-left: 4px solid #202328;"
        "border-right: 0px;"
        "border - bottom: 1px solid black;"
        "border - radius:1px;"
        "background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));"
        "text-align: left;"
        "padding-left: 10px;"
        "color:#CCCCCC;";

    QString strVisibilityBtnStyle =
        "border: 1px solid #202328;"
        "border-left: 1px solid #202328;"
        "border-right: 4px solid #202328;"
        "border - bottom: 1px solid black;"
        "border - radius:1px;"
        "background:qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(85, 90, 99, 255), stop:1 rgba(98, 106, 119, 255));"
        "color:#CCCCCC;";

    QString strContentStyle =
        "border: 4px solid #202328;"
        "border - bottom: 8px solid #202328;"
        "border - top: 0px;"
        "border - radius:1px;"
        "background: qlineargradient(spread : pad, x1 : 0, y1 : 1, x2 : 0, y2 : 0.306, stop : 0 rgba(196, 196, 196, 255), stop : 1 rgba(232, 232, 232, 255));"
        "color:#111111;";

    setAutoFillBackground(true);
    setStyleSheet(strGroupBoxStyle);

    m_pGridLayout = new QGridLayout(this);

    QFont titleFont("MS Shell Dlg 2", 10, QFont::Bold);

    m_pTitleBtn = new QPushButton("Title", this);
    m_pTitleBtn->setStyleSheet(strTitleBtnStyle);
    m_pTitleBtn->setMinimumHeight(32);
    m_pTitleBtn->setFont(titleFont);

    m_pVisibilityBtn = new QPushButton("-", this);
    m_pVisibilityBtn->setStyleSheet(strVisibilityBtnStyle);
    m_pVisibilityBtn->setFixedSize(32, 32);
    m_pVisibilityBtn->setCheckable(true);
    m_pVisibilityBtn->setFont(titleFont);

    m_pContent = new QWidget(this);
    m_pContent->setStyleSheet(strContentStyle);

    m_pGridLayout->addWidget(m_pTitleBtn, 0, 0);
    m_pGridLayout->addWidget(m_pVisibilityBtn, 0, 1);
    m_pGridLayout->addWidget(m_pContent, 1, 0, 1, -1);
    m_pGridLayout->setVerticalSpacing(0);
    m_pGridLayout->setHorizontalSpacing(0);
    m_pGridLayout->setMargin(0);

    setContentsMargins(0, 0, 0, 0);
    m_pGridLayout->setContentsMargins(0, 0, 0, 0);

    connect(m_pVisibilityBtn, SIGNAL(toggled(bool)), this, SLOT(slotToggleVisibility(bool)));
}

QEditorPanelBox::~QEditorPanelBox()
{

}

void QEditorPanelBox::slotToggleVisibility(bool bHide)
{
    if (bHide)
    {
        m_contentHeight = m_pContent->height();

        m_pContent->hide();

        m_pVisibilityBtn->setText("+");

        setFixedHeight(m_pTitleBtn->y() + m_pTitleBtn->height());
    }
    else
    {
        m_pContent->show();

        m_pVisibilityBtn->setText("-");

        setFixedHeight(m_pTitleBtn->y() + m_pTitleBtn->height() + m_contentHeight);
    }
}
