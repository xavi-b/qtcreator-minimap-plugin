// Copyright (C) 2017, emJay Software Consulting AB
// Copyright (C) 2024, Xavier BESSON
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-2.1-only WITH Qt-GPL-exception-1.0

#pragma once

#include <QProxyStyle>

namespace TextEditor {
class BaseTextEditor;
}

namespace Minimap {
namespace Internal {
class MinimapStyleObject;

class MinimapStyle : public QProxyStyle
{
    Q_OBJECT
public:
    MinimapStyle(QStyle *style);

    void drawComplexControl(
        ComplexControl control,
        const QStyleOptionComplex *option,
        QPainter *painter,
        const QWidget *widget = Q_NULLPTR) const override;

    // need this due to QTBUG-24279
    SubControl hitTestComplexControl(
        ComplexControl control,
        const QStyleOptionComplex *option,
        const QPoint &pos,
        const QWidget *widget = Q_NULLPTR) const override;

    int pixelMetric(
        PixelMetric metric,
        const QStyleOption *option = Q_NULLPTR,
        const QWidget *widget = Q_NULLPTR) const override;

    QRect subControlRect(
        ComplexControl cc,
        const QStyleOptionComplex *opt,
        SubControl sc,
        const QWidget *widget) const override;

    static QObject *createMinimapStyleObject(TextEditor::BaseTextEditor *editor);

private:
    bool drawMinimap(
        const QStyleOptionComplex *, QPainter *, const QWidget *, MinimapStyleObject *) const;
};
} // namespace Internal
} // namespace Minimap
