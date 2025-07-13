#include "mygraphicsview.h"
#include "document.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsItem>

MyGraphicsView::MyGraphicsView(QWidget *parent): QGraphicsView(parent) {
    tempRect=nullptr;
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    startP=mapToScene(event->pos());
    is_dragging=true;
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    is_dragging=false;
    QPointF p=mapToScene(event->pos());
    if (p==startP) {
        // click
        if(scene()->itemAt(p, transform())){
            QGraphicsItem* it=scene()->itemAt(p, transform());
            doc->removeRect(it);
            update();
        }
        return;
    }
    QRectF r(startP, p);
    scene()->removeItem(tempRect);
    delete tempRect;
    tempRect=nullptr;
    doc->addRect(r);
    update();
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (!is_dragging) return;
    if (!tempRect) {
        tempRect=new QGraphicsRectItem(QRectF(startP,QSize(0,0)));
        tempRect->setPen(QPen(Qt::GlobalColor::green,2.0/transform().m11() ,Qt::PenStyle::DashLine));
        scene()->addItem(tempRect);
        update();
    }
    QPointF p=mapToScene(event->pos());
    QRectF r(startP, p);
    tempRect->setRect(r);
    update();

}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y()>0) {
        double times=event->angleDelta().y()/120.0;
        doc->setZoom_factor(doc->getZoom_factor()*(times*0.5+1));
    } else {
        double times=-event->angleDelta().y()/120.0;
        doc->setZoom_factor(doc->getZoom_factor()/(times*0.5+1));
    }
}

Document *MyGraphicsView::getDoc() const
{
    return doc;
}

void MyGraphicsView::setDoc(Document *newDoc)
{
    doc = newDoc;
}
