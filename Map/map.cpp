#include "map.h"
using find_path::Map;

Map::Map(QWidget* parent)
    :
      QGraphicsView(parent),
      mapGridPen(Qt::black),
      backgroundBrush(Qt::blue),
      disabledNodeBrush(Qt::green),
      currScale(1)
{
    mapGridGroup = new QGraphicsItemGroup();

    scene = new QGraphicsScene(this);
    scene->addItem(mapGridGroup);

    setScene(scene);
    setMouseTracking(true);

    worker = new MapWorker();

    connect(worker, &MapWorker::ready, this, &Map::DrawMap);
    connect(worker, &MapWorker::recalcReady, this, &Map::RedrawMap);
    connect(worker, &MapWorker::removed, this, &Map::ClearMap);

    connect(this, &Map::init, worker, &MapWorker::Init);
    connect(this, &Map::remove, worker, &MapWorker::Clear);
    connect(this, &Map::resize, worker, &MapWorker::Recalc);

    connect(this, &Map::clicked, worker, &MapWorker::SetStart);
}

Map::~Map() {
    delete worker;
    worker = nullptr;

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
}

void Map::DrawNode(const QPair<int, int>& index) {
    mapGridGroup->addToGroup(
                scene->addRect(
                    QRectF(
                        index.first * valueDivisionX + scene->sceneRect().left(),
                        index.second * valueDivisionY + scene->sceneRect().top(),
                        valueDivisionX,
                        valueDivisionY
                    ),
                    mapGridPen,
                    disabledNodeBrush
                ));
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
            DrawNode(node);
    }
}

void Map::RedrawMap(NodeList* disableNodes) {
    ClearMap();
    DrawMap(disableNodes);
}

void Map::Remove() {
    emit remove();
}

void Map::Init(int w, int h) {
    width = w;
    height = h;

    valueDivisionX = scene->sceneRect().width() / w;
    valueDivisionY = scene->sceneRect().height() / h;

    worker->setSize(QSize(width, height));
    worker->setValueDivision(valueDivisionX, valueDivisionY);

    emit init();
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
