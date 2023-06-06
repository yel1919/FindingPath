#include "map.h"
using find_path::Map;

Map::Map(QWidget* parent)
    :
      QGraphicsView(parent),
      mapGridPen(Qt::black),
      backgroundBrush(Qt::blue),
      disabledNodeBrush(Qt::green)
{
    //setGeometry(10, 10, 255, 255);
    scene = new QGraphicsScene(this);
    worker = new MapWorker();


    setScene(scene);
    setMouseTracking(true);


}

Map::~Map() {
    delete scene;
    delete worker;
}

void Map::DrawMap() {
    scene->setBackgroundBrush(backgroundBrush);

    QRectF src = scene->sceneRect();
    scene->addLine(src.left()+10, src.top()+10, src.right()-10, src.top()+10, mapGridPen);
    scene->addLine(src.left()+10, src.top()+10, src.left()+10, src.bottom()-10, mapGridPen);
    scene->addLine(src.left()+10, src.bottom(), src.right()-10, src.bottom()-10, mapGridPen);
    scene->addLine(src.right()-10, src.top()+10, src.right()-10, src.bottom()-10, mapGridPen);
}

void Map::RedrawMap() {
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
    Remove();
}

void Map::resizeEvent(QResizeEvent *event) {
    scene->setSceneRect(QRectF(0, 0, event->size().width(), event->size().height()));

    valueDivisionX = scene->sceneRect().width() / width;
    valueDivisionY = scene->sceneRect().height() / height;

    worker->setValueDivision(valueDivisionX, valueDivisionY);
    worker->Recalc();

    RedrawMap();
    QGraphicsView::resizeEvent(event);
}
