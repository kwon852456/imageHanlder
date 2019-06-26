#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imghandler_stt.h"
#include "dbdialog.h"


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
    void onDbItemSelected(int idx);


signals:
    void askCon(const QString _addr, const int _port);
    void askDisCon();
    void askTask(QString _fPath, int _mode, bool compress, QString _option = "" );
    void presetChecked(bool checked);
    void setdbList(QStringList list);
    void saveTempImage(QString _path);

private:
   int imgWidth  = -1;
   int imgHeight = -1;
   QString sizeOption  = "";
   dbDialog* dbList;

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

    void on_RoiRelese();

    void on_btn_Perspectiv_clicked();

    void on_btn_conv_clicked();

    void on_radio_rgb_clicked();

    void on_radio_hsv_clicked();

    void on_cb_preset_clicked(bool checked);

    void on_btn_nomalization_clicked();

    void on_btn_end_in_clicked();

    void on_actionSaveImage_triggered();

    void on_actionLoadImage_triggered();

    void on_actionsave_as_triggered();

private:
    Ui::MainWindow *ui;
    QString current_path;
    QThread socThread;
    QByteArray inImg;
    SocWorker* sw;
    bool is_presetSaving = false;
    bool is_homoing = false;
    bool is_zooming = false;
    bool is_compressing = false;

    QVector<QVector<QPoint>> rois;
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
    void onAskTask(QString _fPath, int _mode, bool compress, QString _option = ""  );
    void connectServer(const QString _addr, const int _port);
    void onPresetChecked(bool checked);
    void disConnectServer();
    void onServerConnected();
    void onRecv();
    void onSaveTempImage(QString _path);

signals:
    void resultReady(const QByteArray result);
    void sendMsg(const QString _text);


private:
    QString hostName;
    QString port;
    QTcpSocket* sendSock;
    QByteArray recvData;
    QByteArray tempData;
    int targetLength = -1;
    bool is_recvStart = false;
    bool is_presetSaving = false;
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
    void roiRelesed();

private:

    QPoint p;
    QVector<QPoint> points;
};



#endif // MAINWINDOW_H
