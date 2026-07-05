#pragma once

#include <QFlags>

namespace GraphView {

enum class WidgetResizeMode { None, Horizontal, Vertical, All };

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
using SizeType = qsizetype;
#else
using SizeType = int;
#endif
} // namespace GraphView
