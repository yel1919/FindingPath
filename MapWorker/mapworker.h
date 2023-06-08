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
        qreal valueDivisionX;
        qreal valueDivisionY;

        bool inited;
    protected:
        void Initialization(int width, int height);
        QRect CalcNodeRect(int index, int jndex);
        void Remove();

        void DeleteRoad(Road*& path);

        virtual void Bind(Node& from, Node& to);
        virtual Road* BFS(Node* from, Node* to);
    public:
        MapWorker(QObject* parent = nullptr);
        virtual ~MapWorker();

        const qreal getValueDivisionX() const;
        const qreal getValueDivisionY() const;
        void setValueDivisionX(qreal x);
        void setValueDivisionY(qreal y);
        void setValueDivision(qreal x, qreal y);

        void SetPoint(int index, int jndex);
        void SetMovePoint(int index, int jndex);

        virtual void Init(int width, int height);
        virtual void Recalc();
        virtual void Clear();
        virtual Road* FindPath(int index, int jndex);
    public: signals:
        void removed();
        void ready(NodeList*);
        void recalcReady(NodeList*);
        void roadChanged(FindResult);
        void newRoadChanged(FindResult);
    };
}

#endif // MAPWORKER_H
