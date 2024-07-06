// Copyright (C) 2017, emJay Software Consulting AB
// Copyright (C) 2024, Xavier BESSON
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-2.1-only WITH Qt-GPL-exception-1.0

#pragma once

#include <qglobal.h>

#if defined(MINIMAP_LIBRARY)
#  define MINIMAP_EXPORT Q_DECL_EXPORT
#elif defined(MINIMAP_STATIC_LIBRARY)
#  define MINIMAP_EXPORT
#else
#  define MINIMAP_EXPORT Q_DECL_IMPORT
#endif