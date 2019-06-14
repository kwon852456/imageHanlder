#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imghandler_stt.h"



/////////////////////////////////////////
///////    Constant Values    ///////////
/////////////////////////////////////////

int (*inImg5)[512] = new int[512][512];
int (*inImg2)[256] = new int[256][256];
int (*inImg1)[128] = new int[128][128];

int (*outImg5)[512] = new int[512][512];
int (*outImg2)[256] = new int[256][256];
int (*outImg1)[128] = new int[128][128];


/////////////////////////////////////////
///////    static functions    //////////
/////////////////////////////////////////




/////////////////////////////////////////
///////         Main          ///////////
/////////////////////////////////////////




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openInImage_triggered()
{
    lab_dial(this,ui->label_img);
}
