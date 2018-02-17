#ifndef TERNARYCANVAS_H
#define TERNARYCANVAS_H

#include <QWidget>
#include <QRubberBand>
#include <QLineEdit>
#include "ternarydataset.h"

class TernaryCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit TernaryCanvas(QWidget *parent = nullptr);
    QPointF coordToCanvas(double a, double b);
    bool canvasToCoordinate(double mouseX, double mouseY, double &a, double &b, double &c);
    void calcCoordinateText(double mouseX, double mouseY);
    void addDataSet(TernaryDataSet* dataSet);

signals:

public slots:
    void toggleGrid(bool drawGrid);

protected:
    QRectF ternaryRectF_;
    int border_;
    bool drawGrid_;
    double majorGrid_;
    double minorGrid_;
    int nextDataSetNumber_;
    QList<TernaryDataSet *> dataSets_;
    QString coordinateText_;
    bool mouseIsDragging;
    QPoint mouseDownPos_;
    QRubberBand *rubberBand_;
    QLineEdit *aLabel_;
    QPainterPath *selectionBoxPath_;

    QString aVertexLabel_;
    QString bVertexLabel_;
    QString cVertexLabel_;


    void resizeEvent (QResizeEvent * event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void drawGrid(QPainter& painter);
    void updateDragBox(double anchorX_, double anchorY_, double mouseX, double mouseY);

    void renameVertex(char vertex);
};

#endif // TERNARYCANVAS_H
