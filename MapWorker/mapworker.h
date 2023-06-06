#ifndef MAPWORKER_H
#define MAPWORKER_H

#include <random>
#include "../Node/node.h"

namespace find_path {
    typedef QVector<QVector<Node*>> MapMatrix;

    class MapWorker : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(MapWorker)
    private:
        MapMatrix* map;

        int width;
        int height;
        qreal valueDivisionX;
        qreal valueDivisionY;

        bool inited;
    protected:
        void Initialization();
        QRect CalcNodeRect(int index, int jndex);
        void Remove();

        virtual void Bind(Node* from, Node* to);
    public:
        MapWorker(QObject* parent = nullptr);
        virtual ~MapWorker();

        const QSize getSize() const;
        void setSize(const QSize& size);

        const qreal getValueDivisionX() const;
        const qreal getValueDivisionY() const;
        void setValueDivisionX(qreal x);
        void setValueDivisionY(qreal y);
        void setValueDivision(qreal x, qreal y);

        virtual void Init();
        virtual void Recalc();
        virtual void Clear();
    public: signals:
        void removed();
        void ready();
    };
}

#endif // MAPWORKER_H
