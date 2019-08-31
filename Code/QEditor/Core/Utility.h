/* 
 * ============================================================
 * Copyright (C) Christian Hamm. All Rights Reserved.
 * ============================================================
 */


#pragma once


#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QPushButton>

#undef UNICODE
#include <Engine/Core/Engine.h>
#define UNICODE 


inline env::Vec3 FromQColor(const QColor& color)
{
    return env::Vec3(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f);
}

inline env::Vec4 FromQColorAlpha(const QColor& color)
{
    return env::Vec4(color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f, color.alpha() / 255.0f);
}

inline QColor ToQColor(const env::Vec3& color)
{
    return QColor(color.x * 255.0f, color.y * 255.0f, color.z * 255.0f);
}

inline QColor ToQColor(const env::Vec4& color)
{
    return QColor(color.x * 255.0f, color.y * 255.0f, color.z * 255.0f, color.w * 255.0f);
}

template<class ColorType>
inline void SetBtnColor(QPushButton* pBtn, const ColorType& color)
{
    QString strBackgroundColor = QString::number(color.x * 255) + ", " +
        QString::number(color.y * 255) + ", " +
        QString::number(color.z * 255) + ", 1.0";

    pBtn->setStyleSheet("background-color:rgba(" + strBackgroundColor + ")");
}
