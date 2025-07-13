#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QGraphicsItem>
#include <QObject>

class MainWindow;

class Document : public QObject
{
    Q_OBJECT
public:
    explicit Document(MainWindow *parent = nullptr);

    double getZoom_factor() const;
    void setZoom_factor(double newZoom_factor);

    double getFrame_lapse() const;
    void setFrame_lapse(double newFrame_lapse);

    void addRect(QRectF r);
    void removeRect(QGraphicsItem* it);

    void clearRects();

    void fixZoom();

signals:

private:
    int count=0;
    MainWindow *mainwindow;

    // current zoom factor
    double zoom_factor;

    // all rect marks
    QVector<QGraphicsRectItem*> rects;

    // duration of one frame, msec
    double frame_lapse;

};

#endif // DOCUMENT_H
