#include "connectwidgets.h"
#include "commands/addrelation.h"
#include "commands/changerelation.h"
#include "commands/splitrelation.h"
#include "scene.h"
#include "widgets/abstractwidget.h"
#include "handles/connectionhandle.h"
#include "widgets/crossconnection.h"
#include "widgets/relation.h"

#include <QGraphicsSceneEvent>

namespace GraphView
{

bool HilightRectWidget::isObscuredBy(const QGraphicsItem *item) const
{
    Q_UNUSED(item)
    return false;
}

ConnectWidgetsTool::ConnectWidgetsTool(Scene *scene)
    : AbstractTool{scene}
    , _crossConnection{new CrossConnection}
{
//    _relationRect = new HilightRectWidget;
//    _relationRect->setPen(QPen{Qt::darkBlue, 3});
//    _relationRect->hide();
//    _relationRect->setZValue(9999);
//    _scene->addItem(_relationRect);

    _crossConnection->hide();
    _scene->addItem(_crossConnection);

    _relationPreview = new RelationWidget;
    //    _relationPreview->hide();
    _scene->addItem(_relationPreview);
}

bool ConnectWidgetsTool::accept(QGraphicsItem *item, QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton) {
        auto handle = dynamic_cast<ConnectionHandleWidget *>(item);
        if (!handle)
            return false;
        _connectMode = ConnectMode::FromHandle;

        qDebug() << "HANDLE selected" << (QObject *)handle << (handle->relation() ? "had relation" : "had not relation");
        _relation = handle->relation();
        if (_relation) {
            _moveType = MoveType::ExistingRelation;
            _source = _relation->from().handle();
        } else {
            _source = handle;
            _source->setIsSelected(true);
            _relation = _relationPreview;
            _relation->setFrom(handle);
            _moveType = MoveType::NewRalation;
        }

    } else if (mouseEvent->button() == Qt::RightButton) {
        _sceneRelations = _scene->relations();
        auto i = std::find_if(_sceneRelations.begin(), _sceneRelations.end(), [&mouseEvent, this](RelationWidget *r) {
            return r->intersect(mouseEvent->scenePos(), &_sourcePoint);
        });

        if (i == _sceneRelations.end()) {
            qDebug() << "Not in line";
            _crossConnection->hide();
            return false;
        } else {
            qDebug() << "In line";
            _crossConnection->setCenterPos(_sourcePoint);
            _crossConnection->show();
            _sourceRelation = *i;
            _connectMode = ConnectMode::FromLine;

            _relation = _relationPreview;
            _relation->setFrom(_sourcePoint);
            _relation->setTo(_sourcePoint);
            _relation->show();
        }
    }
    _relation->setTo(mouseEvent->scenePos());
    _relation->update();
    _destination = nullptr;
    return true;
}

void ConnectWidgetsTool::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
    _sceneRelations = _scene->relations();
    AbstractWidget *w;
    ConnectionHandleWidget *ch;
    if (hilightHandleUnderCursor(mouseEvent, w, ch)) {
    } else {
        qDebug() << "No source";
    }
    _relation->show();

}

void ConnectWidgetsTool::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
//    if ((mouseEvent->buttons() & Qt::RightButton) == Qt::RightButton) {
//        _relation->setTo(mouseEvent->scenePos());
//    }
//    if (!_connectSourceWidget)
//        return;

    AbstractWidget *w{};
    ConnectionHandleWidget *ch{};
//    _connectMode = ConnectMode::None;
    if (hilightHandleUnderCursor(mouseEvent, w, ch)) {
        if (ch != _source && ch != _destination) {
            if (_destination)
                _destination->setIsSelected(false);

            _destination = ch;
            _destination->setIsSelected(true);

            _relation->setTo(ch);
        }
    } else {
        if (_destination){
            _destination->setIsSelected(false);
        }
        _relation->setTo(mouseEvent->scenePos());
    }
}

void ConnectWidgetsTool::nullAllSelectedItems()
{
//    _relationPreview->reset();// setFrom(nullptr);
//    _relationPreview->setTo(nullptr);
    _destination = nullptr;
    _source = nullptr;
    _relation = nullptr;
    _sourceRelation = nullptr;
    _connectSourceWidget = nullptr;
}

void ConnectWidgetsTool::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
    _relationPreview->reset();
    _crossConnection->hide();
    _relationPreview->hide();
    AbstractWidget *w{};
    hilightHandleUnderCursor(mouseEvent, w, _destination);
    if (_source)
        _source->setIsSelected(false);
    if (_destination)
        _destination->setIsSelected(false);
    else {
        nullAllSelectedItems();
        Q_EMIT AbstractTool::finished();
        return;
    }

    if (_moveType == MoveType::ExistingRelation) {
        auto cmd = new ChangeRelationCommand{_scene, _relation, _source, _destination};
        _scene->pushCommand(cmd);
    } else {
        switch (_connectMode) {
        case ConnectMode::None:
            break;
        case ConnectMode::FromHandle: {
            auto cmd = new AddRelationCommand{_scene, _source, _destination};
            _scene->pushCommand(cmd);
            break;
        }
        case ConnectMode::FromLine:
            if (_destination)
                qDebug() << "No dest";
            auto cmd = new SplitRelationCommand{_scene, _sourceRelation, _destination, _sourcePoint};
            _scene->pushCommand(cmd);
            break;
        }
    }
    nullAllSelectedItems();

    Q_EMIT AbstractTool::finished();
    // return;
    // auto item = _scene->itemAt(mouseEvent->scenePos(), QTransform());
    // auto ch = dynamic_cast<ConnectionHandleWidget *>(item);
    // if (!ch) {
    //     _relationPreview->hide();
    //     Q_EMIT AbstractTool::finished();
    //     return;
    // }

    // _destination = ch;
    // w = dynamic_cast<AbstractWidget *>(ch->parentItem());
    // qDebug() << "Released at" << mouseEvent->scenePos() << (int *)_connectSourceWidget << item << (int *)w;

    // if (_connectSourceWidget && w) {
    //     if (_moveType == MoveType::NewRalation) {
    //         Q_EMIT _scene->widgetsConnectionRequested(_source, ch);
    //     }
    //     _source->setIsSelected(false);
    //     _destination->setIsSelected(false);
    // } else {
    //     qDebug() << "Unable to connect";
    // }
    // _connectSourceWidget = nullptr;

    // Q_EMIT AbstractTool::finished();
}

QString ConnectWidgetsTool::text() const
{
    return "Connect widgets";
}

void ConnectWidgetsTool::deactivate()
{
    _relationPreview->hide();
}

AbstractTool::ToolType ConnectWidgetsTool::toolType() const
{
    return AbstractTool::ToolType::RequireActivation;
}

bool ConnectWidgetsTool::hilightHandleUnderCursor(QGraphicsSceneMouseEvent *mouseEvent, AbstractWidget *&w, ConnectionHandleWidget *&ch)
{
    auto item = _scene->itemAt(mouseEvent->scenePos(), QTransform());
    //    qDebug() << "item" << item;
    ch = dynamic_cast<ConnectionHandleWidget *>(item);
    if (!ch || ch->relation()) {
        //        qDebug() << "no handle";
        return false;
    }

    w = dynamic_cast<AbstractWidget *>(item->parentItem());
    if (!w) {
        //        qDebug() << "no widget";
        return false;
    }

    //    constexpr double p{3};
    //    QRectF rc{ch->scenePos(), ch->size()};
    //    _relationRect->setRect(QRectF{rc.left() - p, rc.top() - p, rc.width() + p * 2, rc.height() + p * 2});
    //    _relationRect->show();

    ch->setIsSelected(true);

    return true;
}
}
