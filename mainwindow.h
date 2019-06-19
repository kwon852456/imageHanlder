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
    void askTask(QString _fPath, int _mode, QString = "");


private slots:
    void on_btn_bright_clicked();
    void on_btn_connect_clicked();

    void on_btn_reverse_clicked();

    void on_btn_bin_clicked();

    void on_btn_para_clicked();

    void on_btn_rotate_clicked();

private:
    Ui::MainWindow *ui;
    QThread socThread;
    QByteArray inImg;
    SocWorker* sw;
    QString current_path;
};




/////////////////////////////////////////
///////  Socket Worker Class  ///////////
/////////////////////////////////////////
class SocWorker : public QObject{
    Q_OBJECT

public:
    SocWorker();


public slots:
    void connectServer(const QString _addr, const int _port);
    void onServerConnected();
    void onAskTask(QString _fPath, int _mode, QString _option = "");

signals:
    void resultReady(const QByteArray result);
    void sendMsg(const QString _text);


private:
    QString hostName;
    QString port;
    QTcpSocket* sendSock;

};



#endif // MAINWINDOW_H
