#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imghandler_stt.h"


namespace Ui {
class MainWindow;
}

class SocWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void makeConnections();
    void createThreads  ();

public slots:
    void on_openInImage_triggered();
    void handleResults(const QByteArray result);
    void putStatus    (QString _status);


signals:
    void askCon(const QString _addr, const int _port);
    void askDisCon();
    void askTask(QString _fPath, int _mode, bool compress, QString _option = "" );

private:
   int imgWidth  = -1;
   int imgHeight = -1;
   QString sizeOption  = "";

private slots:
    void on_btn_bright_clicked();

    void on_btn_connect_clicked();

    void on_btn_reverse_clicked();

    void on_btn_bin_clicked();

    void on_btn_para_clicked();

    void on_btn_rotate_clicked();

    void on_RoiSelect(QVector<QPoint> _points);

    void on_check_compress_clicked(bool checked);

    void on_btn_zoomOut_clicked();

    void on_btn_zoomIn_clicked();

    void on_btn_hist_clicked();

    void on_btn_homography_clicked();

    void on_check_Log_clicked(bool checked);

    void on_btn_emboss_clicked();

    void on_btn_disconnect_clicked();

private:
    Ui::MainWindow *ui;
    QThread socThread;
    QByteArray inImg;
    SocWorker* sw;
    QString current_path;

    bool is_compressing = false;
};




/////////////////////////////////////////
///////  Socket Worker Class  ///////////
/////////////////////////////////////////
class SocWorker : public QObject{
    Q_OBJECT

public:
    SocWorker();
   ~SocWorker();


public slots:
    void connectServer(const QString _addr, const int _port);
    void disConnectServer();
    void onServerConnected();
    void onAskTask(QString _fPath, int _mode, bool compress, QString _option = ""  );
    void onRecv();

signals:
    void resultReady(const QByteArray result);
    void sendMsg(const QString _text);


private:
    QString hostName;
    QString port;
    QTcpSocket* sendSock;
    QByteArray recvData;
    int targetLength = -1;
    bool is_recvStart = false;
};


/////////////////////////////////////////
//////////  ImageLabel Class  ///////////
/////////////////////////////////////////


class PaintLabel : public QLabel
{
    Q_OBJECT
public:
    PaintLabel(QWidget *parent = nullptr);
    void mousePressEvent ( QMouseEvent * ev );
    void paintEvent      ( QPaintEvent * e  );

signals:
    void roiSelect(QVector<QPoint> _points);

private:

    QPoint p;
    QVector<QPoint> points;
};



#endif // MAINWINDOW_H
