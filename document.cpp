#include "document.h"
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QPen>

Document::Document(MainWindow *parent)
    : QObject{(QObject*)parent}, mainwindow(parent),zoom_factor(1),frame_lapse(0)
{}

double Document::getZoom_factor() const
{
    return zoom_factor;
}

void Document::setZoom_factor(double newZoom_factor)
{
    zoom_factor = newZoom_factor;
    fixZoom();

    // move to controller? or to MainWindow slot?
    mainwindow->applyScale();
}

double Document::getFrame_lapse() const
{
    return frame_lapse;
}

void Document::setFrame_lapse(double newFrame_lapse)
{
    frame_lapse = newFrame_lapse;
}

void Document::addRect(QRectF r)
{
    // into QGraphicsItemGroup ???
    double top=r.top();
    double bottom=r.bottom();
    double left=r.left();
    double right=r.right();
    if (left>right) std::swap(left,right);
    if (top>bottom) std::swap(top, bottom);
    r=QRectF(left,top, right-left, bottom-top);
    QGraphicsRectItem *rItem=new QGraphicsRectItem(r,nullptr);
    //rItem->setPen(QPen(Qt::GlobalColor::red,1.0,Qt::PenStyle::SolidLine));
    rects.append(rItem);
    mainwindow->getScene()->addItem(rItem);
    count++;

    QGraphicsTextItem *tItem=new QGraphicsTextItem(rItem);
    QString t=QString("<span style=\"background: #FF0000; color: #FFFFFF; font-weight: bold;\">%1</span>").arg(count);
    QPointF p=r.topLeft();
    // p+=QPointF(-4.0,-4.0); // kostyl
    // tItem->setPos(p);
    tItem->setHtml(t);

    fixZoom();
}

void Document::removeRect(QGraphicsItem *it)
{
    for (int i=0; i<rects.size(); i++) {
        if (rects[i]==it){
            mainwindow->getScene()->removeItem(it);
        }
    }
    rects.removeAll(it);
}

void Document::clearRects()
{
    for (QGraphicsItem* it : rects) {
        mainwindow->getScene()->removeItem(it);
        delete it;
    }
    rects.clear();

}

void Document::fixZoom()
{
    // возможно с точки зрения MVC это следовало бы засунуть ближе к графике.
    for (QGraphicsRectItem* it : rects) {
        // чтобы на экране толщина линии была постоянной == 2px
        it->setPen(QPen(Qt::GlobalColor::red,1.0/zoom_factor,Qt::PenStyle::SolidLine));

        // так делать не надо. лучше все метки явным образом хранить и запрашивать в Document
        QGraphicsTextItem* t=static_cast<QGraphicsTextItem*>(it->childItems().at(0));
        QFont f=t->font();
        // чтобы при любом масштабе размер текста в пикселях был одинаковый
        f.setPointSizeF(12.0/zoom_factor);
        t->setFont(f);
        // прижать текст внутрь левого-верхнего угла рамки вплотную
        QPointF p=it->rect().topLeft()+QPointF(-4.0,-4.0); // kostyl
        t->setPos(p);
    }

}
