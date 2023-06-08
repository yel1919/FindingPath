#include "map.h"
using find_path::Map;

Map::Map(QWidget* parent)
    :
      QGraphicsView(parent),
      mapGridPen(Qt::black),
      backgroundBrush(Qt::blue),
      disabledNodeBrush(Qt::green),
      roadBrush(Qt::white),
      errorRoadBrush(Qt::red),
      currScale(1)
{
    mapGridGroup = new QGraphicsItemGroup();
    roadGroup = new QGraphicsItemGroup();
    newRoadGroup = new QGraphicsItemGroup();

    scene = new QGraphicsScene(this);
    scene->addItem(mapGridGroup);
    scene->addItem(roadGroup);
    scene->addItem(newRoadGroup);

    setScene(scene);
    setMouseTracking(true);

    worker = new MapWorker();

    connect(worker, &MapWorker::ready, this, &Map::DrawMap);
    connect(worker, &MapWorker::recalcReady, this, &Map::RedrawMap);
    connect(worker, &MapWorker::removed, this, &Map::ClearMap);

    connect(this, &Map::init, worker, &MapWorker::Init);
    connect(this, &Map::remove, worker, &MapWorker::Clear);
    connect(this, &Map::resize, worker, &MapWorker::Recalc);

    connect(this, &Map::clicked, worker, &MapWorker::SetPoint);
    connect(this, &Map::mouseMove, worker, &MapWorker::SetMovePoint);
    connect(worker, &MapWorker::roadChanged, this, &Map::FindResultHandler);
    connect(worker, &MapWorker::newRoadChanged, this, &Map::FindResultMoveHandler);
}

Map::~Map() {
    delete worker;
    worker = nullptr;

    delete newRoadGroup;
    newRoadGroup = nullptr;

    delete roadGroup;
    roadGroup = nullptr;

    delete mapGridGroup;
    mapGridGroup = nullptr;

    delete scene;
    scene = nullptr;
}

void Map::CoordinatesToIndex(int* x, int* y) {
    int modX = currScale > 1 ? horizontalScrollBar()->value() : (scene->sceneRect().width() * currScale - scene->sceneRect().width()) / 2;
    int modY = currScale > 1 ? verticalScrollBar()->value() : (scene->sceneRect().height() * currScale - scene->sceneRect().height()) / 2;

    *x = (*x + modX) / (valueDivisionX * currScale);
    *y = (*y + modY) / (valueDivisionY * currScale);
}

void Map::DeleteItemFromGroup(QGraphicsItemGroup* group) {
    if(group != nullptr && scene != nullptr) {
        for(auto item : scene->items(group->boundingRect())) {
            if(item->group() == group) {
                delete item;
                item = nullptr;
            }
        }
    }
}

void Map::ClearMap() {
    DeleteItemFromGroup(mapGridGroup);
    DeleteItemFromGroup(roadGroup);
    DeleteItemFromGroup(newRoadGroup);
}

void Map::FindResultHandler(FindResult result) {
    if(result.success) {
        DrawPath(result.path, mapGridPen, roadBrush);
    }
    else {
        ErrorBox::Message("FindError", QObject::tr("Path not found"));
    }
}

void Map::FindResultMoveHandler(FindResult result) {
    if(result.success) {
        DrawNewPath(result.path, Qt::NoPen, roadBrush);
    }
    else {
        DrawNewPath(result.path, Qt::NoPen, errorRoadBrush);
    }
}

QGraphicsRectItem* Map::DrawNode(const QPair<int, int>& index, const QPen& pen, const QBrush& brush) {
    return scene->addRect(
                QRectF(
                    index.first * valueDivisionX + scene->sceneRect().left(),
                    index.second * valueDivisionY + scene->sceneRect().top(),
                    valueDivisionX,
                    valueDivisionY
                ),
                pen,
                brush
            );
}

QGraphicsEllipseItem* Map::DrawEllipseNode(const QPair<int, int>& index, const QPen& pen, const QBrush& brush) {
    int radiusX = valueDivisionX / 4;
    int radiusY = valueDivisionY / 4;
    return scene->addEllipse(
                index.first * valueDivisionX + scene->sceneRect().left() + radiusX,
                index.second * valueDivisionY + scene->sceneRect().top() + radiusY,
                radiusX * 2,
                radiusY * 2,
                pen,
                brush
            );
}

void Map::DrawMap(NodeList* disableNodes) {
    QRectF src = scene->sceneRect();

    mapGridGroup->addToGroup(
                scene->addRect(
                    src,
                    mapGridPen,
                    backgroundBrush
                ));

    for(int i = 1; i < width; ++i) {
        mapGridGroup->addToGroup(
                    scene->addLine(
                        i * valueDivisionX + src.left(),
                        src.top(),
                        i * valueDivisionX + src.left(),
                        src.bottom(),
                        mapGridPen
                    ));
    }

    for(int i = 1; i < height; ++i) {
        mapGridGroup->addToGroup(
                    scene->addLine(
                        src.left(),
                        i * valueDivisionY + src.top(),
                        src.right(),
                        i * valueDivisionY + src.top(),
                        mapGridPen
                    ));
    }

    if(disableNodes != nullptr && !disableNodes->empty()) {
        for(auto node : *disableNodes)
            mapGridGroup->addToGroup(DrawNode(node, mapGridPen, disabledNodeBrush));
    }
}

void Map::RedrawMap(NodeList* disableNodes, Road* road, Road* newRoad) {
    ClearMap();
    DrawMap(disableNodes);
    DrawPath(road, mapGridPen, roadBrush);
    DrawNewPath(newRoad, Qt::NoPen, roadBrush);
}

void Map::DrawPath(const Path<QPair<int, int>>* path, const QPen& pen, const QBrush& brush) {
    if(path == nullptr) return;
    DeleteItemFromGroup(newRoadGroup);
    DeleteItemFromGroup(roadGroup);
    for(auto it = path->begin(); it < path->end(); ++it) {
        roadGroup->addToGroup(
                    DrawNode(
                        *it,
                        pen,
                        brush
                    )
                );
    }
}

void Map::DrawNewPath(const Path<QPair<int, int>>* path, const QPen& pen, const QBrush& brush) {
    if(path == nullptr) return;
    DeleteItemFromGroup(newRoadGroup);
    for(auto it = path->begin(); it < path->end(); ++it) {
        newRoadGroup->addToGroup(
                    DrawEllipseNode(
                        *it,
                        pen,
                        brush
                    )
                );
    }
}

void Map::Remove() {
    emit remove();
}

void Map::Init(int w, int h) {
    width = w;
    height = h;

    valueDivisionX = scene->sceneRect().width() / w;
    valueDivisionY = scene->sceneRect().height() / h;

    worker->setValueDivision(valueDivisionX, valueDivisionY);

    emit init(w, h);
}

void Map::Create(int w, int h) {
    if(w == 0 || h == 0) {
        ErrorBox::Message("InputError", QObject::tr("Width or height equal null"));
        return;
    }

    int maxX = scene->sceneRect().width() / minValueDivision;
    int maxY = scene->sceneRect().height() / minValueDivision;

    if(maxX < w || maxY < h) {
        ErrorBox::Message("InputError", QObject::tr("Maximum size: %1 x %2").arg(QString::number(maxX), QString::number(maxY)));
        return;
    }

    Init(w, h);
}

void Map::Clear() {
    Remove();
}

void Map::resizeEvent(QResizeEvent *event) {
    scene->setSceneRect(QRectF(1, 1, event->size().width() - 1, event->size().height() - 1));

    valueDivisionX = scene->sceneRect().width() / width;
    valueDivisionY = scene->sceneRect().height() / height;

    worker->setValueDivision(valueDivisionX, valueDivisionY);
    emit resize();

    QGraphicsView::resizeEvent(event);
}

void find_path::Map::wheelEvent(QWheelEvent* event) {
    if(event->modifiers() == Qt::ControlModifier) {
        if(event->angleDelta().y() > 0) {
            scale(scaleFactor, scaleFactor);
            currScale *= scaleFactor;
        }
        else {
            scale(1 / scaleFactor, 1 / scaleFactor);
            currScale /= scaleFactor; }
    }
    else {
        QGraphicsView::wheelEvent(event);
    }
}

void Map::LButtonClick(QMouseEvent* event) {
    int i = event->pos().x(), j = event->pos().y();
    CoordinatesToIndex(&i, &j);

    emit clicked(i, j);
}

void find_path::Map::RButtonClick(QMouseEvent* event) {

}

void find_path::Map::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::MouseButton::LeftButton) {
        LButtonClick(event);
    }
    if(event->button() == Qt::MouseButton::RightButton) {
        RButtonClick(event);
    }
    QGraphicsView::mousePressEvent(event);
}

void Map::mouseMoveEvent(QMouseEvent* event)  {
    int i = event->pos().x(), j = event->pos().y();
    CoordinatesToIndex(&i, &j);

    emit mouseMove(i, j);
}
