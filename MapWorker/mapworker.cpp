#include "mapworker.h"

using find_path::MapWorker;
using find_path::Node;
using find_path::Path;
using find_path::Road;

const int INF = 1e9;

MapWorker::MapWorker(QObject* parent)
    :
      QObject(parent),
      map(new MapMatrix({})),
      disableNodes(new NodeList({})),
      start(nullptr),
      road(nullptr),
      newRoad(nullptr),
      width(0),
      height(0),
      inited(false)
{}

MapWorker::~MapWorker() {
    Remove();

    delete disableNodes;
    disableNodes = nullptr;

    delete map;
    map = nullptr;
}

void MapWorker::DeleteRoad(Road*& path) {
    if(path != nullptr) {
        delete path;
        path = nullptr;
    }
}

void MapWorker::Bind(Node& from, Node& to) {
    if(from.IsEnabled() && to.IsEnabled()) {
        from.AddEdge(to);
    }
}

void MapWorker::Initialization(int w, int h) {
    width = w;
    height = h;
    map->resize(width, QVector<Node*>(height, nullptr));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3);

    for(auto itW = map->begin(); itW < map->end(); ++itW) {
        for(auto itH = itW->begin(); itH < itW->end(); ++itH) {
            int modX = std::distance(map->begin(), itW);
            int modY = std::distance(itW->begin(), itH);

            Node* node = new Node(dist(gen));

            *itH = node;

            if(!node->IsEnabled()) {
                disableNodes->push_back(QPair<int, int>(modX, modY));
            }

            if(itW > map->begin())
                Bind(*node, **((itW - 1)->begin() + modY));
            if(itH > itW->begin())
                Bind(*node, **(itH - 1));
        }
    }

    inited = true;

    emit ready(disableNodes);
}

void MapWorker::Recalc() {
    if(inited) {
        emit recalcReady(disableNodes, road, newRoad);
    }
}

void MapWorker::Remove() {
    if(inited) {
        disableNodes->clear();

        DeleteRoad(road);
        DeleteRoad(newRoad);

        start = nullptr;

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

void MapWorker::Init(int w, int h) {
    if(inited)
        Remove();
    Initialization(w, h);
}

void MapWorker::Clear() {
    Remove();
}

Road* MapWorker::BFS(Node* from, Node* to) {
    QHash<Node*, int> dist;
    QHash<Node*, QPair<Node*, int>> p;
    QQueue<Node*> q;

    for(auto itW = map->begin(); itW < map->end(); ++itW) {
        for(auto itH = itW->begin(); itH < itW->end(); ++itH) {
            dist[*itH] = INF;
            p[*itH].first = nullptr;
            p[*itH].second = (itW - map->begin()) * height + (itH - itW->begin());
        }
    }

    dist[from] = 0;
    q.enqueue(from);

    while(!q.isEmpty()) {
        Node* node = q.front();
        q.dequeue();

        for(Node* rel : node->RelatedEdges()) {
            if(dist[rel] > dist[node] + 1) {
                p[rel].first = node;
                dist[rel] = dist[node] + 1;
                q.enqueue(rel);
            }
        }
    }

    if(dist[to] == INF) {
        return nullptr;
    }

    Road* _newRoad = new Road();
    while(to != nullptr) {
        _newRoad->push_front(
                    {
                        (p[to].second) / height,
                        p[to].second - ((p[to].second) / height) * height
                    }
                );
        to = p[to].first;
    }
    return _newRoad;
}

void MapWorker::SetPoint(int index, int jndex) {
    if(inited) {
        if(
                index >= 0 && index < map->size() &&
                jndex >= 0 && jndex < (*map)[index].size()
        ) {
            if(start == nullptr && (*map)[index][jndex]->IsEnabled()) {
                start = (*map)[index][jndex];
            }
            else if(start != nullptr) {
                Road* _road = FindPath(index, jndex);

                FindResult result;
                if(_road != nullptr) {
                    result.path = _road;
                    result.success = true;

                    DeleteRoad(road);
                    DeleteRoad(newRoad);

                    road = _road;
                    start = nullptr;
                }
                else {
                    result.path = road;
                }

                emit roadChanged(result);
            }
        }
    }
}

void MapWorker::SetMovePoint(int index, int jndex) {
    if(inited) {
        if(
                index >= 0 && index < map->size() &&
                jndex >= 0 && jndex < (*map)[index].size() &&
                start != nullptr
        ) {
            Road* _road = FindPath(index, jndex);

            FindResult result;
            if(_road != nullptr) {
                result.path = _road;
                result.success = true;

                DeleteRoad(newRoad);
                newRoad = _road;
            }
            else {
                result.path = newRoad;
            }

            emit newRoadChanged(result);
        }
    }
}

void MapWorker::CancelFind() {
    start = nullptr;
    DeleteRoad(newRoad);
    emit canceledFind();
}

Road* MapWorker::FindPath(int index, int jndex) {
    if(
            index >= 0 && index < map->size() &&
            jndex >= 0 && jndex < (*map)[index].size()
    ) {
        return BFS(start, (*map)[index][jndex]);
    }
    return {};
}
