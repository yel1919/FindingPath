#include "mapworker.h"

using find_path::MapWorker;

MapWorker::MapWorker(QObject* parent)
    :
      QObject(parent),
      map(new MapMatrix({})),
      disableNodes(new NodeList({})),
      start(nullptr),
      finish(nullptr),
      width(0),
      height(0),
      valueDivisionX(0),
      valueDivisionY(0),
      inited(false)
{}

MapWorker::~MapWorker() {
    Remove();

    delete disableNodes;
    disableNodes = nullptr;

    delete map;
    map = nullptr;
}

const QSize MapWorker::getSize() const {
    return QSize(width, height);
}
void MapWorker::setSize(const QSize& size) {
    width = size.width();
    height = size.height();
}

const qreal MapWorker::getValueDivisionX() const {
    return valueDivisionX;
}

const qreal MapWorker::getValueDivisionY() const {
    return valueDivisionY;
}

void MapWorker::setValueDivisionX(qreal x) {
    valueDivisionX = x;
}

void MapWorker::setValueDivisionY(qreal y) {
    valueDivisionY = y;
}

void MapWorker::setValueDivision(qreal x, qreal y) {
    valueDivisionX = x;
    valueDivisionY = y;
}

void MapWorker::Bind(Node* from, Node* to) {
    if(from->IsEnabled() && to->IsEnabled()) {
        from->AddEdge(to);
    }
}

QRect MapWorker::CalcNodeRect(int index, int jndex) {
    return QRect(
                index * valueDivisionX,
                jndex * valueDivisionY,
                valueDivisionX,
                valueDivisionY
    );
}

void MapWorker::Initialization() {
    map->resize(width, QVector<Node*>(height, nullptr));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3);

    for(auto itW = map->begin(); itW < map->end(); ++itW) {
        for(auto itH = itW->begin(); itH < itW->end(); ++itH) {
            int modX = std::distance(map->begin(), itW);
            int modY = std::distance(itW->begin(), itH);

            Node* node = new Node(
                        CalcNodeRect(
                            modX,
                            modY
                        ),
                        dist(gen)
            );

            *itH = node;

            if(!node->IsEnabled()) {
                disableNodes->push_back(QPair<int, int>(modX, modY));
            }

            if(itW > map->begin())
                Bind(node, *((itW - 1)->begin() + modY));
            if(itH > itW->begin())
                Bind(node, *(itH - 1));
        }
    }

    inited = true;

    emit ready(disableNodes);
}

void MapWorker::Recalc() {
    if(inited) {
        emit recalcReady(disableNodes);
    }
}

void MapWorker::Remove() {
    if(inited) {
        disableNodes->clear();

        for(auto itW = map->begin(); itW < map->end(); ++itW) {
            for(auto itH = itW->begin(); itH < itW->end(); ++itH) {
                delete *itH;
                *itH = nullptr;
            }
        }

        map->clear();
        inited = false;

        emit removed();
    }
}

void MapWorker::Init() {
    if(inited)
        Remove();
    Initialization();
}

void MapWorker::Clear() {
    Remove();
}


void MapWorker::SetStart(int index, int jndex) {
    if(inited) {
        if(
                index >= 0 && index < map->size() &&
                jndex >= 0 && jndex < (*map)[index].size()
        ) {
            start = (*map)[index][jndex];
        }
    }
}

void MapWorker::SetFinish(int index, int jndex) {
    if(inited) {
        if(
                index >= 0 && index < map->size() &&
                jndex >= 0 && jndex < (*map)[index].size()
        ) {
            finish = (*map)[index][jndex];
        }
    }
}
