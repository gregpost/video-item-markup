#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "document.h"
#include "qmediaplayer.h"
#include <QMainWindow>
#include <QGraphicsVideoItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Document doc;
    QMediaPlayer *mediaPlayer = nullptr;
    QGraphicsVideoItem *videoItem = nullptr;
    QAudioOutput *audioOutput;
    QGraphicsScene* scene;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QGraphicsScene *getScene() const;
    void applyScale();

protected slots:
    void onOpen(bool checked);
    void onDurationChanged(qint64 dur);

private:
    Ui::MainWindow *ui;
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
