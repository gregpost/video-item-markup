#include "mainwindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QVideoSink>
#include <QMediaMetaData>
#include <QAudioOutput>
#include "ui_mainwindow.h"
#include <cmath>

QGraphicsScene *MainWindow::getScene() const
{
    return scene;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , doc(this), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoItem = new QGraphicsVideoItem;
    videoItem->setSize(ui->graphicsView->size());
    scene = new QGraphicsScene(this);
    scene->addItem(videoItem);
    ui->graphicsView->setScene(scene);
    audioOutput=new QAudioOutput;
    audioOutput->setVolume(50);

    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setVideoOutput(videoItem);
    mediaPlayer->setAudioOutput(audioOutput);

    ui->graphicsView->setDoc(&doc);

    connect(ui->horizontalSlider, &QAbstractSlider::sliderMoved, this, [=](int pos){
        qint64 newpos=std::round(pos*doc.getFrame_lapse() * 1000.0);
        mediaPlayer->setPosition(newpos);
    });

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [=](qint64 pos){
        int sl_pos=std::round(pos/1000.0/doc.getFrame_lapse());
        ui->horizontalSlider->setSliderPosition(sl_pos);
    });

    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::onDurationChanged);
    connect(ui->actionPlay, &QAction::triggered, mediaPlayer, &QMediaPlayer::play);
    connect(ui->actionStop, &QAction::triggered, mediaPlayer, &QMediaPlayer::stop);
    connect(ui->actionPause, &QAction::triggered, mediaPlayer, &QMediaPlayer::pause);


    connect(ui->actionNext_Frame,&QAction::triggered, this, [=](bool ok){
        Q_UNUSED(ok);
        if (mediaPlayer->position()<mediaPlayer->duration()-1)
            mediaPlayer->setPosition(std::round(mediaPlayer->position()+doc.getFrame_lapse()));
    });

    connect(ui->actionPrev_Frame,&QAction::triggered, this, [=](bool ok){
        Q_UNUSED(ok);
        if (mediaPlayer->position()>0)
            mediaPlayer->setPosition(std::round(mediaPlayer->position()-doc.getFrame_lapse()));
    });

    connect(ui->actionOpen,&QAction::triggered, this, &MainWindow::onOpen);

    connect(ui->actionZoom_In,&QAction::triggered, this, [=](bool ok){
        Q_UNUSED(ok);
        doc.setZoom_factor(doc.getZoom_factor()*1.5);
    });

    connect(ui->actionZoom_Out,&QAction::triggered, this, [=](bool ok){
        Q_UNUSED(ok);
        doc.setZoom_factor(doc.getZoom_factor()/1.5);
    });

    connect(ui->actionClear_marks,  &QAction::triggered, this, [=](bool ok){
        Q_UNUSED(ok);
        doc.clearRects();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpen(bool checked)
{
    Q_UNUSED(checked);
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation)
                                .value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted) {
        doc.clearRects();
        mediaPlayer->setSource(fileDialog.selectedUrls().constFirst());
        ui->actionPlay->setEnabled(true);
        mediaPlayer->pause();
    }
}


void MainWindow::onDurationChanged(qint64 dur)
{
    double framerate=mediaPlayer->metaData().value(QMediaMetaData::VideoFrameRate).toDouble();
    doc.setFrame_lapse(1.0/framerate);
    int nframes=floor(dur/1000.0*framerate);
    ui->horizontalSlider->setMaximum(nframes);
    QSize sz=mediaPlayer->videoSink()->videoSize();
    videoItem->setSize(sz);
    ui->graphicsView->fitInView(videoItem,Qt::AspectRatioMode::KeepAspectRatio);
    doc.setZoom_factor(ui->graphicsView->transform().m11()); // suppopsed to be a scaling factor
    qDebug()<<"framerate:"<<framerate<<" dur:"<<dur<<" nframes:"<<nframes<<" lapse:"<<doc.getFrame_lapse()<<" sz:"<<sz;
    ui->graphicsView->update();
}

void MainWindow::applyScale()
{
    QTransform t = QTransform::fromScale(doc.getZoom_factor(), doc.getZoom_factor());
    ui->graphicsView->setTransform(t);
    ui->graphicsView->update();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->graphicsView->fitInView(videoItem,Qt::AspectRatioMode::KeepAspectRatio);
    doc.setZoom_factor(ui->graphicsView->transform().m11()); // suppopsed to be a scaling factor
    ui->graphicsView->update();
}
