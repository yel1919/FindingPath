#ifndef MAP_H
#define MAP_H

#include <QPen>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>

#include "../MapWorker/mapworker.h"

namespace find_path {
    class Map : public QGraphicsView {
        Q_OBJECT
        Q_DISABLE_COPY(Map)
    private:
        MapWorker* worker;
        QGraphicsScene* scene;

        QGraphicsItemGroup* mapGridGroup;
        QGraphicsItemGroup* roadGroup;
        QGraphicsItemGroup* newRoadGroup;

        QPen mapGridPen;
        QBrush backgroundBrush;
        QBrush disabledNodeBrush;
        QBrush roadBrush;
        QBrush errorRoadBrush;

        qreal valueDivisionX;
        qreal valueDivisionY;

        const int minValueDivision = 3; //минимальный размер узла

        int width;
        int height;

        const qreal scaleFactor = 1.5; //коэффициент масштабирование
        qreal currScale;

        void Init(int width, int height);
        void Remove();
        void DeleteItemFromGroup(QGraphicsItemGroup* group);
    protected: signals:
        void resize();
        void init(int width, int height);
        void remove();
    protected:
        void FindResultHandler(FindResult result);
        void FindResultMoveHandler(FindResult result);
        virtual void CoordinatesToIndex(int* x, int* y);
        virtual void ClearMap();

        virtual QGraphicsRectItem* DrawNode(const QPair<int, int>& index, const QPen& pen, const QBrush& brush);
        virtual QGraphicsEllipseItem* DrawEllipseNode(const QPair<int, int>& index, const QPen& pen, const QBrush& brush);
        virtual void DrawMap(NodeList* disableRects);
        virtual void RedrawMap(NodeList* disableRects);
        virtual void DrawPath(const Path<QPair<int, int>>* path, const QPen& pen, const QBrush& brush);
        virtual void DrawNewPath(const Path<QPair<int, int>>* path, const QPen& pen, const QBrush& brush);

        virtual void LButtonClick(QMouseEvent* event);
        virtual void RButtonClick(QMouseEvent* event);

        void resizeEvent(QResizeEvent *event) override;
        void wheelEvent(QWheelEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
    public:
        Map(QWidget* parent = nullptr);
        virtual ~Map();
    public: signals:
        void clicked(int x, int y);
        void mouseMove(int x, int y);
    public slots:
        virtual void Create(int width, int height);
        virtual void Clear();
    };
}

#endif // MAP_H
