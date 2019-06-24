#ifndef IMGHANDLER_STT_H
#define IMGHANDLER_STT_H
#include "QFileDialog"
#include "QMainWindow"
#include "QDebug"
#include "QImage"
#include "QLabel"
#include <QPixmap>
#include <math.h>
#include "QThread"
#include <QTcpSocket>
#include <QWaitCondition>
#include <QTcpServer>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QMouseEvent>
#include <QPainter>
#include <QVector>
#include <KH_01.hpp>
#include <QTime>
#include <QMatrix>
#include <QGenericMatrix>
#include <QTimer>


/////////////////////////////////////////
///////    static functions    //////////
/////////////////////////////////////////


using namespace std;

inline void co_byte(QByteArray _arr){
    qDebug() << __func__;


    QString str  = "";
    QString temp = "";

    for(int i(0) ; i < _arr.length() ; ++i){
        temp.clear();

        temp.sprintf("0x%02x ", _arr.at(i) );

        str += temp;
        qDebug() << temp;
    }

    qDebug() << str;

}



inline QString path_dial(QMainWindow* _this){

    QString strFilter = "bitmap file (*.png) ;; All files (*.*)";
    QString strFileName = QFileDialog::getOpenFileName(_this, "Open a file", QDir::homePath() , strFilter);

    return strFileName;
}

inline QFile* file_path(QString _path ){

    QFile* file = new QFile(_path);

    return file;
}


inline QByteArray ba_file(QFile* _file){

    if( !_file->open(QIODevice::ReadOnly) ){

        qDebug() << "!_file.open(QIODevice::ReadOnly)" << __func__;

        return nullptr;

    }

    QByteArray inImg = _file->readAll();

    delete _file;
    return inImg;
}



inline QImage* img_ba(QByteArray _inImg){
    int size = sqrt( _inImg.length() );
    QImage* image = new QImage( reinterpret_cast<unsigned char*> ( _inImg.data() ), size , size, QImage::Format_Grayscale8);

    return image;

}

inline QImage* colImg_ba(QByteArray* _inImg, int width = 512, int height = 512){
    QImage* image = new QImage( reinterpret_cast<unsigned char*> ( _inImg -> data() ), width , height, QImage::Format_RGB888);

    return image;

}


inline QImage* img_path(QString _imgPath){

    QImage tepImage(_imgPath);
    QImage* image = new QImage(tepImage.convertToFormat(QImage::Format_RGB888)) ;

    qDebug() << "image byteSize : " << image->sizeInBytes();

    return image;

}

inline QPixmap* pix_img(QImage* _img){

    QPixmap *pixmap_ = new QPixmap();

    *pixmap_ = QPixmap::fromImage(*_img);
    *pixmap_ = pixmap_->scaled(_img->width(),_img->height());

    delete _img;

    return pixmap_;

}

inline void lab_pix(QPixmap* _pix, QLabel* _label){
    _label  ->  setPixmap    (  *_pix                         );

    delete _pix;
}


inline QPixmap* pix_dial(QMainWindow* _this){
    QPixmap* buffer = pix_img( img_ba(ba_file((file_path( path_dial(_this) ))))) ;
}


inline void lab_dial(QMainWindow* _this, QLabel* _label ){

    QPixmap* buffer = pix_img(  img_ba(ba_file(file_path( path_dial(_this))))  );
    _label  ->  resize       (  buffer->width(),buffer->height()               );
    _label  ->  setPixmap    (  *buffer                                        );

}

inline QByteArray ba_qs(QString _qstring, int size = 0){

    QByteArray b_out;
    b_out.clear();
    b_out.append(_qstring.toUtf8());
    b_out.resize(size);

    return b_out;
}

inline QByteArray head_enc(QString _fPath, int _mode, QString _option, int imgSize){
    qDebug() << __func__;

    #ifdef Q_OS_LINUX
    QByteArray metaData = ba_qs( "C:" +_fPath + ":" + QString::number(_mode) + ":" + _option + ":" , 200);
    #elif
    QByteArray metaData = ba_qs( _fPath + ":" + QString::number(_mode) + ":" + _option + ":" , 200);
    #endif

    QByteArray header;

    QDataStream headerOut(&header, QIODevice::WriteOnly);
    headerOut.setVersion(QDataStream::Qt_5_12);

    int totalLength = sizeof (int) + 200 + imgSize;

    headerOut << totalLength;

    header.append(metaData);
    QStringList info = _option.split("|");
    qDebug() << "Option Value    : " << info                    ;

    qDebug() << "size of image           : " << imgSize             ;
    qDebug() << "size of image Width     : " << info[1]             ;
    qDebug() << "size of image height    : " << info[2]             ;
    qDebug() << "size of totalLength     : " << totalLength         ;
    qDebug() << "size of metaData        : " << metaData.length   ();
    qDebug() << "size of header          : " << header.length     ();



    return header;
}




inline void con_16bytes(QByteArray* arr) {

    QString msg;
    QString temp;

    for(z::t i(0) ; i < 16 ; ++i) {

        temp.sprintf("0x%02x ", *(arr->constData() + i));
        msg += temp;

    }

    qDebug() << msg;
}
inline void con_16bytes(unsigned char* arr) {

    QString msg;
    QString temp;

    for(z::t i(0) ; i < 16 ; ++i) {

        temp.sprintf("0x%02x ", *(arr + i));
        msg += temp;

    }

    qDebug() << msg;
}

inline void con_16bytes(char* arr) {

    QString msg;
    QString temp;

    for(z::t i(0) ; i < 16 ; ++i) {

        temp.sprintf("0x%02x ", *(arr + i));
        msg += temp;

    }

    qDebug() << msg;
}

inline void con_8bytes(QByteArray* arr) {

    QString msg;
    QString temp;

    for(z::t i(0) ; i < 8 ; ++i) {

        temp.sprintf("0x%02x ", *(arr->constData() + i));
        msg += temp;

    }

    qDebug() << msg;
}
inline void con_8bytes(unsigned char* arr) {

    QString msg;
    QString temp;

    for(z::t i(0) ; i < 8 ; ++i) {

        temp.sprintf("0x%02x ", *(arr + i));
        msg += temp;

    }

    qDebug() << msg;
}

inline void con_8bytes(char* arr) {

    QString msg;
    QString temp;

    for(z::t i(0) ; i < 8 ; ++i) {

        temp.sprintf("0x%02x ", *(arr + i));
        msg += temp;

    }

    qDebug() << msg;
}

inline void con_byte(QByteArray* arr , int offset = 0) {

    QString msg;
    msg.sprintf("0x%02x ", *(arr->constData() + offset ));

    qDebug() << msg;
}

inline void con_byte(unsigned char* arr, int offset = 0) {

    QString msg;
    msg.sprintf("0x%02x ", *(arr + offset ));

    qDebug() << msg;
}


inline void co_arrMat(QByteArray* arr, int row = 512 , int col= 25, int cellDataSize = 8){
        Eigen::Matrix<double,Dynamic, Dynamic> A;
        A.resize(row,col);

        QByteArray temp;
        for (int i = 0 ; i < row ; ++i){
            for(int j = 0 ; j < col ; ++j){
                temp.clear();
                for(int x = 0 ; x < cellDataSize ; ++x){
                    temp.append(*(arr->constData() + (i * (col * cellDataSize) ) + (j * cellDataSize) + x  ));
                }
                A(i , j) = (*(double*)temp.constData());
            }
        }

        cout << A;
}

inline QByteArray* decode_512(QByteArray* _img, int rank = 25){

    const int SIZE = 512;

    Eigen::Matrix<float,Dynamic, Dynamic> A;
    Eigen::Matrix<float,Dynamic, Dynamic> B;
    Eigen::Matrix<float,Dynamic, Dynamic> C;
    Eigen::Matrix<unsigned char,Dynamic, Dynamic> D;

    A.resize(SIZE,rank);
    B.resize(rank,SIZE);

    C.resize(SIZE, SIZE);
    D.resize(SIZE, SIZE);

    const int TYPE_SIZE = sizeof(float);

    QByteArray temp;
    for (int i = 0 ; i < SIZE ; ++i){
        for(int j = 0 ; j < rank ; ++j){
            temp.clear();
            for(int x = 0 ; x < TYPE_SIZE ; ++x){
                temp.append(*(_img->data() + (i * (rank * TYPE_SIZE) ) + (j * TYPE_SIZE) + x  ));
            }
            A(i , j) = (*(float*)temp.constData());
        }
    }


    for (int i = 0 ; i < rank ; ++i){
        for(int j = 0 ; j < SIZE ; ++j){

            temp.clear();
            for(int x = 0 ; x < TYPE_SIZE ; ++x){
                temp.append(*(_img->data() + (SIZE * rank * TYPE_SIZE) + (i * (SIZE * TYPE_SIZE) ) + (j * TYPE_SIZE) + x  ));
            }

            B(i , j) = (*(float*)temp.constData()) ;
        }
    }


    C = (A * B);




    for (int i = 0 ; i < SIZE ; ++i){
        for (int j = 0 ; j < SIZE ; ++j){
            if (C(i, j) > 255){
                C(i,j) = 255;
            }else if(C(i, j) < 0){
                C(i,j) = 0;
            }
        }
    }

    D = C.cast<unsigned char>();


    QByteArray* encoded_mat = new QByteArray( reinterpret_cast<char*>(D.data()), SIZE * SIZE );


    return encoded_mat;
}

inline QByteArray* ba_decode(QByteArray* _data, int rank = 25){
    const int SIZE = 512;

    QByteArray* R = new QByteArray(_data->data()                                   ,SIZE * rank * 2 * sizeof(float));
    QByteArray* G = new QByteArray(_data->data() + SIZE * rank * 2 * sizeof(float) ,SIZE * rank * 2 * sizeof(float));
    QByteArray* B = new QByteArray(_data->data() + SIZE * rank * 4 * sizeof(float) ,SIZE * rank * 2 * sizeof(float));

    QByteArray* decoded_R = new QByteArray(*decode_512(R,rank)) ;
    QByteArray* decoded_G = new QByteArray(*decode_512(G,rank));
    QByteArray* decoded_B = new QByteArray(*decode_512(B,rank));

    QByteArray* sum = new QByteArray();

    for(int i = 0 ; i < SIZE * SIZE ; ++i){
        sum -> push_back(*(decoded_R->data() + i));
        sum -> push_back(*(decoded_G->data() + i));
        sum -> push_back(*(decoded_B->data() + i));
    }

    delete decoded_R;
    delete decoded_G;
    delete decoded_B;

    delete R;
    delete G;
    delete B;

    delete _data;

    return sum;

}


inline QByteArray* encode_512(Eigen::Matrix<float,Dynamic,Dynamic> _mat, int rank = 25){

    const int SIZE = 512;

    Eigen::Matrix<float,Dynamic, Dynamic,RowMajor> A;
    Eigen::Matrix<float,Dynamic, Dynamic,RowMajor> B;

    A.resize(SIZE,rank);
    B.resize(rank,SIZE);

    for(int i = 0 ; i < SIZE ; ++i){
        for(int k = 0 ; k < rank ; ++k){
            A( i , k ) = _mat(i , k * round(SIZE / rank) );
        }
    }

    B = (((A.transpose() * A).inverse()) * A.transpose()) * _mat;

    QByteArray* arr_A = new QByteArray(reinterpret_cast<char*>(A.data()), SIZE * rank * sizeof(float));
    QByteArray* arr_B = new QByteArray(reinterpret_cast<char*>(B.data()), SIZE * rank * sizeof(float));


    QByteArray* encoded_mat = new QByteArray();
    encoded_mat->push_back(*arr_A);
    encoded_mat->push_back(*arr_B);

    delete arr_A;
    delete arr_B;

    return encoded_mat;

}

inline QByteArray encode_path(QString _path, int rank = 25){
    QImage* img     = img_path(_path);

    Eigen::Matrix<float, Dynamic,Dynamic,RowMajor> R;
    Eigen::Matrix<float, Dynamic,Dynamic,RowMajor> G;
    Eigen::Matrix<float, Dynamic,Dynamic,RowMajor> B;

    R.resize(512,512);
    G.resize(512,512);
    B.resize(512,512);


    for ( int row = 0; row < img->height() ; ++row )
        for ( int col = 0; col < img->width(); ++col )
        {

            QColor clrCurrent( img->pixel( row, col ) );
            R(row,col) = clrCurrent.red();
            G(row,col) = clrCurrent.green();
            B(row,col) = clrCurrent.blue();

        }

    QByteArray* arr_R = encode_512(R, rank);
    QByteArray* arr_G = encode_512(G, rank);
    QByteArray* arr_B = encode_512(B, rank);

    QByteArray sum;
    sum.push_back(*arr_R);
    sum.push_back(*arr_G);
    sum.push_back(*arr_B);

    delete arr_R;
    delete arr_G;
    delete arr_B;

    return sum;
}

inline QByteArray createProc(QString _fPath, int _mode,bool compress, QString _option = "None"){

    QByteArray b_Img;
    QByteArray b_header;
    qDebug() << "compress : " << compress;
    if(compress){

        b_Img    = encode_path(_fPath, 50);
        b_header = head_enc( _fPath, _mode, _option, b_Img.length() );

    }else{
        QImage* tempImg = img_path(_fPath);
        b_Img    = QByteArray( reinterpret_cast<char*>(tempImg->bits()), tempImg->sizeInBytes() );
        b_header = head_enc( _fPath, _mode, _option, b_Img.length() );

    }

    return b_header.append(b_Img);

}


inline QString qs_rois(QVector<QVector<QPoint>> _rois){
    QString str  = "|";
    QString temp = "";

    for(QVector<QPoint> roi : _rois){
        for(QPoint p : roi){
            temp.sprintf("%d-", p.x());
            str += temp;
        }
    }

    for(QVector<QPoint> roi : _rois){
        for(QPoint p : roi){
            temp.sprintf("%d-", p.y());
            str += temp;
        }
    }

    str + "|";

    return str;

}


#endif // IMGHANDLER_STT_H
