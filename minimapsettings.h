// Copyright (C) 2017, emJay Software Consulting AB
// Copyright (C) 2024, Xavier BESSON
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-2.1-only WITH Qt-GPL-exception-1.0

#pragma once

#include "minimapconstants.h"

#include <QObject>

namespace Utils {
class QtcSettings;
}
namespace ExtensionSystem {
class IPlugin;
}

namespace Minimap::Internal {
class MinimapSettingsPage;
class MinimapSettingsPageWidget;

class MinimapSettings : public QObject
{
    Q_OBJECT

public:
    explicit MinimapSettings();
    ~MinimapSettings();

    bool equals(const MinimapSettings &other) const;

    void toSettings(Utils::QtcSettings *s) const;
    void fromSettings(Utils::QtcSettings *s);

    bool m_enabled = false;
    int m_width = Constants::MINIMAP_WIDTH_DEFAULT;
    int m_lineCountThreshold = Constants::MINIMAP_MAX_LINE_COUNT_DEFAULT;
    int m_alpha = Constants::MINIMAP_ALPHA_DEFAULT;

signals:
    void changed();

public:
    friend bool operator==(const MinimapSettings &p1, const MinimapSettings &p2)
    {
        return p1.equals(p2);
    }
    friend bool operator!=(const MinimapSettings &p1, const MinimapSettings &p2)
    {
        return !p1.equals(p2);
    }
};

MinimapSettings &globalMinimapSettings();

void setupMinimapSettings(ExtensionSystem::IPlugin &plugin);
} // namespace Minimap::Internal
