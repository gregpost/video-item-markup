#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class Document;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget* parent=nullptr);

    // QWidget interface
    Document *getDoc() const;
    void setDoc(Document *newDoc);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QPointF startP;
    bool is_dragging;
    QGraphicsRectItem *tempRect;
    Document* doc;

};

#endif // MYGRAPHICSVIEW_H
