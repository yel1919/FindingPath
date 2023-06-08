#ifndef MAPWORKER_H
#define MAPWORKER_H

#include <random>
#include <QQueue>
#include <QHash>
#include "../FindResult/findresult.h"

namespace find_path {
    typedef QVector<QVector<Node*>> MapMatrix;
    typedef QList<QPair<int, int>> NodeList;
    typedef Path<QPair<int, int>> Road;

    class MapWorker : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(MapWorker)
    private:
        MapMatrix* map;
        NodeList* disableNodes;

        Node* start;
        Road* road;
        Road* newRoad;

        int width;
        int height;

        bool inited;
    protected:
        void Initialization(int width, int height);
        void Remove();

        void DeleteRoad(Road*& path);

        virtual void Bind(Node& from, Node& to);
        virtual Road* BFS(Node* from, Node* to);
    public:
        MapWorker(QObject* parent = nullptr);
        virtual ~MapWorker();

        void SetPoint(int index, int jndex);
        void SetMovePoint(int index, int jndex);
        void CancelFind();

        virtual void Init(int width, int height);
        virtual void Recalc();
        virtual void Clear();
        virtual Road* FindPath(int index, int jndex);
    public: signals:
        void removed();
        void ready(NodeList*);
        void recalcReady(NodeList*, Road*, Road*);
        void roadChanged(FindResult);
        void newRoadChanged(FindResult);
        void canceledFind();
    };
}

#endif // MAPWORKER_H
