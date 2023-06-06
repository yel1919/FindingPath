#ifndef MAP_H
#define MAP_H

#include <QPen>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QResizeEvent>

#include "../MapWorker/mapworker.h"

namespace find_path {
    class Map : public QGraphicsView {
        Q_DISABLE_COPY(Map)
    private:
        MapWorker* worker;
        QGraphicsScene* scene;

        QPen mapGridPen;
        QBrush backgroundBrush;
        QBrush disabledNodeBrush;

        qreal valueDivisionX;
        qreal valueDivisionY;

        const int minValueDivision = 3; //минимальный размер узла

        int width;
        int height;

        const qreal scaleFactor = 1.5; //коэффициент масштабирование
        qreal currScale;

        void Init(int width, int height);
        void Remove();
    protected:
        virtual void DrawMap();
        virtual void RedrawMap();

        void resizeEvent(QResizeEvent *event) override;
    public:
        Map(QWidget* parent = nullptr);
        virtual ~Map();

    public slots:
        virtual void Create(int width, int height);
        virtual void Clear();
    };
}

#endif // MAP_H
