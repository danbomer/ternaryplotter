#include "ternarycanvas.h"
#include "math.h"
#include <QPainter>
#include <QMouseEvent>
#include <QInputDialog>

TernaryCanvas::TernaryCanvas(QWidget *parent) : QWidget(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);

    setPalette(pal);
    setAutoFillBackground(true);
    setMouseTracking(true);

    border_ = 50;
    majorGrid_ = 0.2;
    minorGrid_ = 0.1;
    drawGrid_ = true;
    nextDataSetNumber_ = 1;
    mouseIsDragging = false;
    rubberBand_ = new QRubberBand(QRubberBand::Rectangle, this);
    selectionBoxPath_ = new QPainterPath();

    aVertexLabel_ = "A";
    bVertexLabel_ = "B";
    cVertexLabel_ = "C";


    ternaryRectF_.setWidth(100.0);
    ternaryRectF_.setHeight(100 * sqrt(3)/2.0);
}

void TernaryCanvas::addDataSet(TernaryDataSet *dataSet)
{
    dataSets_.append(dataSet);
    update();
}

void TernaryCanvas::resizeEvent(QResizeEvent * event)
{
    QSize maxSize = rect().adjusted(border_,border_,-border_,-border_).size();
    QSizeF rectSize = ternaryRectF_.size();
    rectSize.scale(maxSize,Qt::KeepAspectRatio);

    ternaryRectF_.setWidth(rectSize.width());
    ternaryRectF_.setHeight(rectSize.height());
    ternaryRectF_.setLeft(border_);
    ternaryRectF_.setTop(border_);
}

void TernaryCanvas::paintEvent(QPaintEvent *event)
{
    TernaryDataSet *d;
    TernaryDataPoint *p;
    QPainter painter(this);
    QPen *pen = new QPen(Qt::black);

    pen->setWidth(2);
    painter.setPen(*pen);

    painter.drawLine(ternaryRectF_.left(),ternaryRectF_.bottom(),ternaryRectF_.left()+ternaryRectF_.width()/2,ternaryRectF_.top());
    painter.drawLine(ternaryRectF_.left()+ternaryRectF_.width()/2,ternaryRectF_.top(),ternaryRectF_.right(),ternaryRectF_.bottom());
    painter.drawLine(ternaryRectF_.right(),ternaryRectF_.bottom(),ternaryRectF_.left(),ternaryRectF_.bottom());

    //label corners
    painter.drawText(ternaryRectF_.left()+ternaryRectF_.width()/2 - 5 * aVertexLabel_.length(),ternaryRectF_.top() - 25, 10 * aVertexLabel_.length(), 30, Qt::AlignCenter, aVertexLabel_);
    painter.drawText(ternaryRectF_.left() - 12 * (bVertexLabel_.length() + 1),ternaryRectF_.bottom() - 5, 30 * bVertexLabel_.length(), 30, Qt::AlignCenter, bVertexLabel_);
    painter.drawText(ternaryRectF_.right() - 2 * cVertexLabel_.length(),ternaryRectF_.bottom() - 5, 10 * cVertexLabel_.length(), 30, Qt::AlignCenter, cVertexLabel_);

    pen->setWidth(1);
    painter.setPen(*pen);

    if(drawGrid_) drawGrid(painter);

    pen->setColor(Qt::red);
    pen->setWidth(2);
    painter.setPen(*pen);

    //painter.drawEllipse(p1, 3.0, 3.0);
    foreach(d,dataSets_)
    {
        int i;
        for(i = 0; i < d->pointCount(); i++)
        {
            p = d->dataPoint(i);
            if(p->isVisible())
            {
                QPointF p1 = coordToCanvas(p->a(),p->b());
                if(p->isSelected())
                {
                    painter.drawEllipse(p1, 4.0, 4.0);
                }
                else
                {
                    painter.drawEllipse(p1, 2.0, 2.0);
                }
            }
        }
    }

    pen->setColor(Qt::black);
    painter.setPen(*pen);
    painter.drawText(QPoint(20, 20), coordinateText_);


    if (mouseIsDragging) {
        pen->setWidth(1);
        pen->setColor(QColor(118, 207, 252,255));
        painter.setBrush(QColor(118, 207, 252,100));
        painter.setPen(*pen);

        painter.drawPath(*selectionBoxPath_);
    }

}

void TernaryCanvas::mousePressEvent(QMouseEvent *event)
{
    mouseIsDragging = true;
    rubberBand_->setGeometry(QRect(event->pos(),QSize()));
    mouseDownPos_ = event->pos();
    QRectF hitAperture = QRectF(event->pos().x()-2, event->pos().y()-2,4,4);

    // did we hit a point?
    // if we did, it is selected
    // all others deselected
    TernaryDataSet *d;
    TernaryDataPoint *p;
    foreach(d,dataSets_)
    {
        int i;
        for(i = 0; i < d->pointCount(); i++)
        {
            p = d->dataPoint(i);
            if(p->isVisible())
            {
                QPointF p1 = coordToCanvas(p->a(),p->b());
                if(hitAperture.contains(p1))
                {
                    p->setSelection(true);
                }
                else
                {
                    p->setSelection(false);
                }
            }
        }
    }
    update();
}

void TernaryCanvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    QRectF clickAperture = QRectF(event->pos().x()-15, event->pos().y()-15,30,30);

    QPointF aLabelPos = QPointF(ternaryRectF_.left()+ternaryRectF_.width()/2 - 5,ternaryRectF_.top() - 20);
    QPointF bLabelPos = QPointF(ternaryRectF_.left() - 15,ternaryRectF_.bottom());
    QPointF cLabelPos = QPointF(ternaryRectF_.right() + 5,ternaryRectF_.bottom());

    if (clickAperture.contains(aLabelPos))
    {
        //qDebug("%f a edit", event->x());
        renameVertex('A');
    } else if (clickAperture.contains(bLabelPos))
    {
        qDebug("%f b edit", event->x());
        renameVertex('B');
    } else if (clickAperture.contains(cLabelPos))
    {
        qDebug("%f c edit", event->x());
        renameVertex('C');
    }
}

void TernaryCanvas::renameVertex(char vertex)
{
    float num = 1;
    if (vertex == 'A')
    {
        bool okA;
        QString textA = QInputDialog::getText(0, "Rename Vertex", "", QLineEdit::Normal,aVertexLabel_, &okA);
        if (okA && !textA.isEmpty()) {
            aVertexLabel_ = textA;
        }
    }
    else if (vertex == 'B')
    {
        bool okB;
        QString textB = QInputDialog::getText(0, "Rename Vertex", "", QLineEdit::Normal,bVertexLabel_, &okB);
        if (okB && !textB.isEmpty()) {
            bVertexLabel_ = textB;
        }
    }
    else if (vertex == 'C')
    {
        bool okC;
        QString textC = QInputDialog::getText(0, "Rename Vertex", "", QLineEdit::Normal,cVertexLabel_, &okC);
        if (okC && !textC.isEmpty()) {
            cVertexLabel_ = textC;
        }
    }
}


void TernaryCanvas::mouseMoveEvent(QMouseEvent *event)
{
    TernaryDataSet *d;
    TernaryDataPoint *p;
    double a,b,c;

    if (canvasToCoordinate(event->x(),event->y(), a, b, c))
    {
        coordinateText_ = "A: " + QString::number(a * 100,'f',1) + "%  B: " + QString::number(b * 100,'f',1) + "%  C: " + QString::number(c * 100,'f',1) + "%";
    }
    else
    {
        coordinateText_ = "";
    }

    if (mouseIsDragging)
    {
        rubberBand_->setGeometry(QRect(mouseDownPos_,event->pos()).normalized());
        rubberBand_->show();
        foreach(d,dataSets_)
        {
            int i;
            for(i = 0; i < d->pointCount(); i++)
            {
                p = d->dataPoint(i);
                if(p->isVisible())
                {
                    QPointF p1 = coordToCanvas(p->a(),p->b());
                    QRectF dragRectF = QRectF(rubberBand_->x(),rubberBand_->y(),rubberBand_->width(),rubberBand_->height());
                    if(dragRectF.contains(p1))
                    {
                        p->setSelection(true);
                    }
                    else
                    {
                        p->setSelection(false);
                    }
                }
            }
        }
    }
    update();
}


void TernaryCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    mouseIsDragging = false;
    rubberBand_->hide();
    update();
}


QPointF TernaryCanvas::coordToCanvas(double a, double b)
{
    double y = (1-a) * ternaryRectF_.height() + ternaryRectF_.top();
    double x = ((1-a-b) + a/2) * ternaryRectF_.width() + ternaryRectF_.left();

    QPointF p;
    p.setX(x);
    p.setY(y);
    return p;
}


bool TernaryCanvas::canvasToCoordinate(double mouseX, double mouseY, double &a, double &b, double &c)
{
    QPointF p;

    a = 1 - ((mouseY - ternaryRectF_.top()) / ternaryRectF_.height());
    b = 1 - (a / 2) - ((mouseX - ternaryRectF_.left()) / ternaryRectF_.width());
    c = 1 - (a + b);
    //qDebug("a: %f, b: %f, c: %f", a, b, c);

    if (a >= 0 && b >= 0 && c >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void TernaryCanvas::drawGrid(QPainter& painter)
{
    double a,b,c;
    QPointF p1, p2;

    //draw minor grid lines
    for(a = minorGrid_; a < 1; a += majorGrid_)
    {
        p1 = coordToCanvas(a,0);
        p2 = coordToCanvas(a,1-a);

        painter.setPen(Qt::lightGray);
        painter.drawLine(p1,p2);

        painter.setPen(Qt::black);
    }
    for(b = minorGrid_; b < 1; b += majorGrid_)
    {
        p1 = coordToCanvas(0,b);
        p2 = coordToCanvas(1-b,b);

        painter.setPen(Qt::lightGray);
        painter.drawLine(p1,p2);

        painter.setPen(Qt::black);

    }
    for(c = minorGrid_; c < 1; c += majorGrid_)
    {
        p1 = coordToCanvas(0,1-c);
        p2 = coordToCanvas(1-c,0);

        painter.setPen(Qt::lightGray);
        painter.drawLine(p1,p2);

        painter.setPen(Qt::black);
    }

    //draw major grid lines
    for(a = 0; a < 1; a += majorGrid_)
    {
        p1 = coordToCanvas(a,0);
        p2 = coordToCanvas(a,1-a);

        painter.drawLine(p1,p2);
    }
    for(b = 0; b < 1; b += majorGrid_)
    {
        p1 = coordToCanvas(0,b);
        p2 = coordToCanvas(1-b,b);

        painter.drawLine(p1,p2);
    }
    for(c = 0; c < 1; c += majorGrid_)
    {
        p1 = coordToCanvas(0,1-c);
        p2 = coordToCanvas(1-c,0);

        painter.drawLine(p1,p2);
    }
}

void TernaryCanvas::toggleGrid(bool drawGrid)
{
    drawGrid_ = drawGrid;
    update();
}

