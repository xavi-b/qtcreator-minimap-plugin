// Copyright (C) 2017, emJay Software Consulting AB
// Copyright (C) 2024, Xavier BESSON
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-2.1-only WITH Qt-GPL-exception-1.0

#include <extensionsystem/iplugin.h>

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/icore.h>
#include <texteditor/texteditor.h>

#include "minimapsettings.h"
#include "minimapstyle.h"

namespace Minimap::Internal {

class MinimapPlugin final : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Minimap.json")

    void initialize() final
    {
        connect(
            Core::EditorManager::instance(),
            &Core::EditorManager::editorCreated,
            this,
            [=](Core::IEditor *editor, const Utils::FilePath &filePath) {
                Q_UNUSED(filePath);
                TextEditor::BaseTextEditor *baseEditor = qobject_cast<TextEditor::BaseTextEditor *>(
                    editor);
                if (baseEditor) {
                    MinimapStyle::createMinimapStyleObject(baseEditor);
                }
            });
    }

    void extensionsInitialized() { setupMinimapSettings(*this); }
};

} // namespace Minimap::Internal

#include "minimapplugin.moc"
