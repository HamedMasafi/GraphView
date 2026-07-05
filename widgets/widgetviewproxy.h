#pragma once

#include <QGraphicsProxyWidget>

namespace GraphView {

class WidgetViewProxy : public QGraphicsProxyWidget
{
    Q_OBJECT

public:
    WidgetViewProxy();
    bool sceneEvent(QEvent* event) override;
};

}
