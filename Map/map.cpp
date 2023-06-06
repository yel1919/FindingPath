#include "map.h"
using find_path::Map;

Map::Map(QWidget* parent)
    :
      QGraphicsView(parent),
      mapGridPen(Qt::black),
      backgroundBrush(Qt::blue),
      disabledNodeBrush(Qt::green)
{
    mapGridGroup = new QGraphicsItemGroup();

    scene = new QGraphicsScene(this);
    scene->addItem(mapGridGroup);

    setScene(scene);
    setMouseTracking(true);

    worker = new MapWorker();
}

Map::~Map() {
    delete mapGridGroup;
    delete scene;

    delete worker;
}

void Map::DeleteItemFromGroup(QGraphicsItemGroup* group) {
    for(auto item : scene->items(group->boundingRect())) {
        if(item->group() == group) {
            delete item;
            item = nullptr;
        }
    }
}

void Map::ClearMap() {
    DeleteItemFromGroup(mapGridGroup);
}

void Map::DrawMap() {
    scene->setBackgroundBrush(backgroundBrush);

    QRectF src = scene->sceneRect();
    mapGridGroup->addToGroup(scene->addLine(src.left(), src.top(), src.right(), src.top(), mapGridPen));
    mapGridGroup->addToGroup(scene->addLine(src.left(), src.top(), src.left(), src.bottom(), mapGridPen));
    mapGridGroup->addToGroup(scene->addLine(src.left(), src.bottom(), src.right(), src.bottom(), mapGridPen));
    mapGridGroup->addToGroup(scene->addLine(src.right(), src.top(), src.right(), src.bottom(), mapGridPen));
}

void Map::RedrawMap() {
    ClearMap();
    DrawMap();
}

void Map::Remove() {
    worker->Clear();
}

void Map::Init(int w, int h) {
    width = w;
    height = h;

    valueDivisionX = scene->sceneRect().width() / w;
    valueDivisionY = scene->sceneRect().height() / h;

    worker->setSize(QSize(width, height));
    worker->setValueDivision(valueDivisionX, valueDivisionY);

    worker->Init();
}

void Map::Create(int w, int h) {
    if(w == 0 || h == 0)
        throw Exception("Width or height equal null");

    int maxX = scene->sceneRect().width() / minValueDivision;
    int maxY = scene->sceneRect().height() / minValueDivision;

    if(maxX < w || maxY < h)
        throw Exception(QObject::tr("Maximum size: %1 x %2").arg(QString::number(maxX), QString::number(maxY)));

    Init(w, h);
}

void Map::Clear() {
    ClearMap();
    Remove();
}

void Map::resizeEvent(QResizeEvent *event) {
    scene->setSceneRect(QRectF(10, 10, event->size().width() - 10, event->size().height() - 10));

    valueDivisionX = scene->sceneRect().width() / width;
    valueDivisionY = scene->sceneRect().height() / height;

    worker->setValueDivision(valueDivisionX, valueDivisionY);
    worker->Recalc();

    RedrawMap();

    QGraphicsView::resizeEvent(event);
}
