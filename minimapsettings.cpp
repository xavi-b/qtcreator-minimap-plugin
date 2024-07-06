// Copyright (C) 2017, emJay Software Consulting AB
// Copyright (C) 2024, Xavier BESSON
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-2.1-only WITH Qt-GPL-exception-1.0

#include "minimapsettings.h"
#include "minimaptr.h"

#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/displaysettings.h>
#include <texteditor/texteditorsettings.h>
#include <utils/qtcassert.h>

#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPointer>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include <limits>

namespace Minimap {
namespace Internal {
namespace {
const char minimapPostFix[] = "Minimap";
const char enabledKey[] = "Enabled";
const char widthKey[] = "Width";
const char lineCountThresholdKey[] = "LineCountThresHold";
const char alphaKey[] = "Alpha";
} // namespace

MinimapSettings &settings()
{
    static MinimapSettings theMinimapSettings;
    return theMinimapSettings;
}

class MinimapSettingsPageWidget : public Core::IOptionsPageWidget
{
public:
    MinimapSettingsPageWidget()
    {
        QVBoxLayout *layout = new QVBoxLayout;
        QGroupBox *groupBox = new QGroupBox(this);
        groupBox->setTitle(Tr::tr("Minimap"));
        layout->addWidget(groupBox);
        QFormLayout *form = new QFormLayout;
        m_enabled = new QCheckBox(groupBox);
        m_enabled->setToolTip(Tr::tr("Check to enable Minimap scrollbar"));
        form->addRow(Tr::tr("Enabled:"), m_enabled);
        m_width = new QSpinBox;
        m_width->setMinimum(1);
        m_width->setMaximum(std::numeric_limits<int>::max());
        m_width->setToolTip(Tr::tr("The width of the Minimap"));
        form->addRow(Tr::tr("Width:"), m_width);
        m_lineCountThresHold = new QSpinBox;
        m_lineCountThresHold->setMinimum(1);
        m_lineCountThresHold->setMaximum(std::numeric_limits<int>::max());
        m_lineCountThresHold->setToolTip(
            Tr::tr("Line count threshold where no Minimap scrollbar is to be used"));
        form->addRow(Tr::tr("Line Count Threshold:"), m_lineCountThresHold);
        m_alpha = new QSpinBox;
        m_alpha->setMinimum(0);
        m_alpha->setMaximum(255);
        m_alpha->setToolTip(Tr::tr("The alpha value of the scrollbar slider"));
        form->addRow(Tr::tr("Scrollbar slider alpha value:"), m_alpha);
        groupBox->setLayout(form);
        setLayout(layout);
        setEnabled(!m_textWrapping);
        setToolTip(
            m_textWrapping ? Tr::tr("Disable text wrapping to enable Minimap scrollbar")
                           : QString());

        connect(
            TextEditor::TextEditorSettings::instance(),
            &TextEditor::TextEditorSettings::displaySettingsChanged,
            this,
            &MinimapSettingsPageWidget::displaySettingsChanged);
        connect(&settings(), &MinimapSettings::changed,
                this, &MinimapSettingsPageWidget::updateUi);
        m_textWrapping = TextEditor::TextEditorSettings::displaySettings().m_textWrapping;
    }

    void apply()
    {
        settings().m_enabled = m_enabled->isChecked();
        settings().m_width = m_width->value();
        settings().m_lineCountThreshold = m_lineCountThresHold->value();
        settings().m_alpha = m_alpha->value();
        settings().toSettings(Core::ICore::settings());
    }

private:
    void displaySettingsChanged(const TextEditor::DisplaySettings &settings)
    {
        m_textWrapping = settings.m_textWrapping;
        setEnabled(!m_textWrapping);
        setToolTip(
            m_textWrapping ? Tr::tr("Disable text wrapping to enable Minimap scrollbar")
                           : QString());
    }

    void updateUi()
    {
        m_enabled->setChecked(settings().m_enabled);
        m_width->setValue(settings().m_width);
        m_lineCountThresHold->setValue(settings().m_lineCountThreshold);
        m_alpha->setValue(settings().m_alpha);
    }

    QCheckBox *m_enabled;
    QSpinBox *m_width;
    QSpinBox *m_lineCountThresHold;
    QSpinBox *m_alpha;
    bool m_textWrapping;
};

class MinimapSettingsPage final : public Core::IOptionsPage
{
public:
    MinimapSettingsPage()
        : Core::IOptionsPage()
    {
        setId(Constants::MINIMAP_SETTINGS);
        setDisplayName(Tr::tr("Minimap"));
        setWidgetCreator([] { return new MinimapSettingsPageWidget(); });
    }

    ~MinimapSettingsPage() {}
};

MinimapSettings::MinimapSettings()
    : QObject()
{}

MinimapSettings::~MinimapSettings() {}

bool MinimapSettings::equals(const MinimapSettings &other) const
{
    return m_enabled == other.m_enabled && m_width == other.m_width
           && m_lineCountThreshold == other.m_lineCountThreshold && m_alpha == other.m_alpha;
}

void MinimapSettings::toSettings(Utils::QtcSettings *s) const
{
    s->beginGroup(minimapPostFix);
    s->setValue(enabledKey, m_enabled);
    s->setValue(widthKey, m_width);
    s->setValue(lineCountThresholdKey, m_lineCountThreshold);
    s->setValue(alphaKey, m_alpha);
    s->endGroup();
}

void MinimapSettings::fromSettings(Utils::QtcSettings *s)
{
    const MinimapSettings def;
    s->beginGroup(minimapPostFix);
    m_enabled = s->value(enabledKey, def.m_enabled).toBool();
    m_width = s->value(widthKey, def.m_width).toInt();
    m_lineCountThreshold = s->value(lineCountThresholdKey, def.m_lineCountThreshold).toInt();
    m_alpha = s->value(alphaKey, def.m_alpha).toInt();
    s->endGroup();
}

void setupMinimapSettings(ExtensionSystem::IPlugin &plugin)
{
    static MinimapSettingsPage theMinimapSettingsPage;

    settings().fromSettings(Core::ICore::settings());
}

} // namespace Internal
} // namespace Minimap
