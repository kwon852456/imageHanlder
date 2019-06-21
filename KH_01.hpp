#ifndef KH_01_HPP
#define KH_01_HPP
#include <functional>
#include <algorithm>
#include <array>
#include <math.h>
#include <iostream>
#include <./Eigen/Dense>

#define EOA INT_MAX
#define PI M_PI


using namespace Eigen;
using namespace std;

namespace  z { typedef  std::size_t t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP);    T Z(sizeof(t)); T T0(0); }/// 4byte(4,294,967,295)[Win32], 8byte[x64], unsigned long (int) std::vector.size(), sizeof(), ... not unsigned int
/// C-types
namespace  c { typedef          char        t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); z::T Z(sizeof(t)); T T0('\0'); }/// 1byte  8bit(-128~) c::t cp1[]="str"; ///no warning  T T0(0x00); T0('\0'); T0(NUL) T0(NULL);
namespace  w { typedef          wchar_t     t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); z::T Z(sizeof(t)); T T0(0x0000); }/// 2byte  16bit(0~65,535) w::t wp1[]=L"str";  //no warning
namespace  y { typedef unsigned char        t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); z::T Z(sizeof(t)); T T0(0x00); }/// 1byte header
namespace  h { typedef unsigned short       t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); z::T Z(sizeof(t)); T T0(0); }/// 2byte size u_short
namespace  i { typedef          int         t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); z::T Z(sizeof(t)); T T0(0); }/// 4byte 32bit(-2,147,483,648~) (signed) int, (signed long) pixel index 1920x1080x3=6,220,800  return
namespace  nn{ typedef unsigned int         t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); z::T Z(sizeof(t)); T T0(0); }/// 4byte 32bit( 4,294,967,256~) (unsigned) int, (unsigned long) pixel index 1920x1080x3=6,220,800  return
namespace  n { typedef unsigned long long   t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); z::T Z(sizeof(t)); T T0(0); }/// 8byte 64bit header time
namespace  f { typedef          float       t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); z::T Z(sizeof(t)); T T0(0.f); }/// 4byte  7digits(-3.4 e-38  ~ 3.4 e+38 )
namespace  d { typedef          double      t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); z::T Z(sizeof(t)); T T0(0.0); }/// 8byte 15digits(-1.79e-308 ~ 1.79e+308)
namespace  b {///
    typedef
    #ifdef __cplusplus
        bool
    #else
        int
    #endif
        t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); z::T Z(sizeof(t)); T T0(
        #ifdef __cplusplus
            false
        #else
            0
        #endif
        ), T1(
        #ifdef __cplusplus
            true
        #else
            1
        #endif
        );
    typedef t a2[2]; typedef const a2 A2; typedef a2* p2; typedef const p2 P2; z::T Z2(sizeof(a2)); A2 A2_0 = { b::T0,b::T0                         };
    typedef t a3[3]; typedef const a3 A3; typedef a3* p3; typedef const p3 P3; z::T Z3(sizeof(a3)); A3 A3_0 = { b::T0,b::T0,b::T0                   };
    typedef t a4[4]; typedef const a4 A4; typedef a4* p4; typedef const p4 P4; z::T Z4(sizeof(a4)); A4 A4_0 = { b::T0,b::T0,b::T0,b::T0             };
    typedef t a6[6]; typedef const a6 A6; typedef a6* p6; typedef const p6 P6; z::T Z6(sizeof(a6)); A6 A6_0 = { b::T0,b::T0,b::T0,b::T0,b::T0,b::T0 };
}///


namespace  mxd  { typedef  MatrixXd   t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP);   typedef Block<t> b; }/// 4byte(4,294,967,295)[Win32], 8byte[x64], unsigned long (int) std::vector.size(), sizeof(), ... not unsigned int
namespace  vxd  { typedef  VectorXd   t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP);   typedef Block<t> b; }/// 4byte(4,294,967,295)[Win32], 8byte[x64], unsigned long (int) std::vector.size(), sizeof(), ... not unsigned int
namespace  rvxd { typedef RowVectorXd t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }/// 4byte(4,294,967,295)[Win32], 8byte[x64], unsigned long (int) std::vector.size(), sizeof(), ... not unsigned int


namespace  m2d  { typedef  Matrix2d   t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }
namespace  v2d  { typedef  Vector2d   t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }
namespace  rv2d { typedef RowVector2d t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }


namespace  m3d  { typedef  Matrix3d   t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }
namespace  v3d  { typedef  Vector3d   t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }
namespace  rv3d { typedef RowVector3d t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }


namespace  m4d  { typedef  Matrix4d   t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }
namespace  v4d  { typedef  Vector4d   t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }
namespace  rv4d { typedef RowVector4d t; typedef const t T; typedef t(&r); typedef T(&R); typedef t(*p); typedef T(*const P); typedef T(*Tp); typedef t(*const tP); typedef Block<t> b; }


template <typename T>
inline void co(T _text, string _s = "output"){   cout << _s << " = " << endl << _text << endl;  }

//convert radian angle to degree angle
inline double deg_rad(d::R _rad){   return _rad * 57.2958;   }

//theta degree between two vectors
inline double d_deg(vxd::t _v1, vxd::t _v2){  return deg_rad(acos(_v1.dot(_v2) / (_v1.norm() * _v2.norm())));  }

//get unit vector which is parallel to given vector
inline vxd::t v_uintV(vxd::t _v){  return _v / _v.norm();  }

//orthogonal projection of _vFrom on _vTo
inline vxd::t v_proj(vxd::t _vFrom, vxd::t _vTo){  return ( _vFrom.dot(_vTo) / ( _vTo.norm() * _vTo.norm() ) ) * _vTo;  }

//get sqaure number of _number
inline double square(double _number){  return _number * _number;  }

//least distance between a 3d point to 3d point
inline double d_poToPo(v3d::t _point1, v3d::t _point2){  return sqrt(square( _point1[0] - _point2[0] ) + square( _point1[1] - _point2[1] ) + square( _point1[2] - _point2[2] ) );  }

//least distance between a point to line
inline double d_poToLi(v2d::t _point,v3d::t _line){  return abs( ( _line[0] * _point[0] + _line[1] * _point[1] + _line[2] ) / sqrt( square(_line[0]) + square(_line[1]) ));  }

//least distance between a point to plain
inline double d_poToPl(v3d::t _point , v4d::t _plain){  return abs( ( _plain[0] * _point[0] + _plain[1] * _point[1] + _plain[2] * _point[2] + _plain[3] ) / sqrt( square(_plain[0]) + square(_plain[1]) + square(_plain[2]) ));  }

//linear combination with a recursive method
inline vxd::t v_comb(mxd::t _v, vxd::t coefficient, int _dimention, int acc = 0){

    if(acc == _dimention){ return VectorXd(_v.cols()).setZero(); }
    return  v_comb(_v, coefficient, _dimention, ++acc) + (_v.col(acc) * coefficient[acc]);

}

//linear combination with a for-statement
inline vxd::t v_comb(mxd::t _v, vxd::t coefficients){

    vxd::t v_(_v.cols());  v_.setZero();

    for(z::t i(0) ; i < _v.cols() ; ++i){  v_ = v_ + _v.col(i) * coefficients[i];  }

    return  v_;
}

//inner product( dot product)
inline double d_dot(vxd::t _v1, vxd::t _v2 , int _dim , int acc = 0){

    if( acc == _dim ) return 0;

    return _v1[acc] * _v2[acc] + d_dot(_v1, _v2 , _dim, ++acc);
}

//blocking a vector(partitioning) this method call _v by it's value not reference
inline vxd::t v_seg(vxd::r _v, int endIdx, int startIdx = 0 ){  return (_v.segment(startIdx,endIdx)); }

//slicing and redicing axpy algorithm
inline vxd::t v_axpy(double alpha, vxd::t _x, vxd::t _y){

    vxd::t v_(_x.rows()); v_.setZero();

    vxd::t xTop = _x.segment(0,0);
    vxd::t xBot = _x.segment(0,_x.rows());

    vxd::t yTop = _y.segment(0,0);
    vxd::t yBot = _y.segment(0,_y.rows());

    int    idx    = 0;

    while(xTop.rows() < _x.rows()){
        v_[idx++] = alpha * _x[idx] + _y[idx];


        xTop = _x.segment(  0,             idx );
        xBot = _x.segment(idx, _x.rows() - idx );
        yTop = _y.segment(  0,             idx );
        yBot = _y.segment(idx, _y.rows() - idx );

        co(xTop, "xTop");
        co(xBot, "xBot");

        co(yTop, "yTop");
        co(yBot, "yBot");


    }
    return v_;
}

inline mxd::t m_zero(mxd::t _m){
    mxd::t m_ = _m.block(0,0,_m.rows(),0);
    m_.setZero();

    int idx = 0;

    while(m_.cols() < _m.cols()){

        m_ =  _m.block(0,0,_m.rows(),++idx);  //partitioning
        m_.setZero();                         //setting zero

        co(m_,"M left :");

    }

    return m_;
}

inline mxd::t m_identity(mxd::t _m){

    for(z::t i(0) ; i < _m.cols() ; ++i){

        mxd::b tl   = _m.block(i,      i     ,  1                   ,  1                            );
        mxd::b tr   = _m.block(i     , i + 1 ,  1                   ,  _m.cols() - (i + 1)          );
        mxd::b bl   = _m.block(i + 1 , i     ,  _m.rows() - (i + 1) ,  1                            );
        mxd::b br   = _m.block(i + 1 , i + 1 ,  _m.rows() - (i + 1) ,  _m.cols() - (i + 1)          );

        co(tl,"tl");
        co(tr,"tr");
        co(bl,"bl");
        co(br,"br");

        tl << 1;
        tr.setZero();
        bl.setZero();
    }
    
    return _m;

}

inline mxd::t m_diagonal(mxd::t _m,vxd::t _entries){

    for(z::t i(0) ; i < _m.cols() ; ++i){

        mxd::b tl   = _m.block(i,      i     ,  1                   ,  1                            );
        mxd::b tr   = _m.block(i     , i + 1 ,  1                   ,  _m.cols() - (i + 1)          );
        mxd::b bl   = _m.block(i + 1 , i     ,  _m.rows() - (i + 1) ,  1                            );
        mxd::b br   = _m.block(i + 1 , i + 1 ,  _m.rows() - (i + 1) ,  _m.cols() - (i + 1)          );

        co(tl,"tl");
        co(tr,"tr");
        co(bl,"bl");
        co(br,"br");

        tl << _entries(i);
        tr.setZero();
        bl.setZero();
    }
    return _m;
}

inline mxd::t m_lower(mxd::t _m){

    for(z::t i(0) ; i < _m.cols() ; ++i){

        mxd::b tl   = _m.block(i,      i     ,  1                   ,  1                            );
        mxd::b tr   = _m.block(i     , i + 1 ,  1                   ,  _m.cols() - (i + 1)          );
        mxd::b bl   = _m.block(i + 1 , i     ,  _m.rows() - (i + 1) ,  1                            );
        mxd::b br   = _m.block(i + 1 , i + 1 ,  _m.rows() - (i + 1) ,  _m.cols() - (i + 1)          );

        co(tl,"tl");
        co(tr,"tr");
        co(bl,"bl");
        co(br,"br");

        tl << 1;
        tr.setZero();
    }
    return _m;
}

inline mxd::t m_upper(mxd::t _m){

    for(z::t i(0) ; i < _m.cols() ; ++i){

        mxd::b tl   = _m.block(i,      i     ,  1                   ,  1                            );
        mxd::b tr   = _m.block(i     , i + 1 ,  1                   ,  _m.cols() - (i + 1)          );
        mxd::b bl   = _m.block(i + 1 , i     ,  _m.rows() - (i + 1) ,  1                            );
        mxd::b br   = _m.block(i + 1 , i + 1 ,  _m.rows() - (i + 1) ,  _m.cols() - (i + 1)          );

        co(tl,"tl");
        co(tr,"tr");
        co(bl,"bl");
        co(br,"br");

        tl << 1;
        bl.setZero();
    }
    return _m;
}


inline mxd::t m_transpose(mxd::t _m){

    mxd::t m_(_m.cols(), _m.rows());
    m_.setZero();

    for(z::t i(0) ; i < _m.cols() ; ++i){

        mxd::b col   = _m.block(0, i ,_m.rows(),1 );

        mxd::b rows  = m_.block(i, 0, 1, _m.cols());

        for(z::t x(0) ; x < _m.cols() ; ++x){
            rows.col(x) = col.row(x);
        }

        co(col, "col");
        co(rows, "rows");



    }
    return m_;
}


inline mxd::t m_symmetric(mxd::t _m){


    for(z::t i(0) ; i < _m.cols() ; ++i){

        mxd::b tl   = _m.block(i,      i     ,  1                   ,  1                            );
        mxd::b tr   = _m.block(i     , i + 1 ,  1                   ,  _m.cols() - (i + 1)          );
        mxd::b bl   = _m.block(i + 1 , i     ,  _m.rows() - (i + 1) ,  1                            );
        mxd::b br   = _m.block(i + 1 , i + 1 ,  _m.rows() - (i + 1) ,  _m.cols() - (i + 1)          );


        bl = tr.transpose();



    }
    return _m;


}




























































////////////the code below can be compiled only over C++14 version ///////
//template<typename Func,typename... Args>
//inline auto curry(Func func, Args... args){

//    return [=]( auto... secondParam){   return func(args..., secondParam...);  };

//}

//template<typename Func,typename... Args>
//inline auto curryBackword(Func func, Args... args){

//    return [=]( auto... secondParam){   return func( secondParam..., args... );  };

//}


//template <typename T, typename enable_if_t<is_pointer<T>::value>* = nullptr>
//inline auto con_Iarr(T _arr, int size = INT_MAX , int acc = 0){

//    if(acc == size || *(_arr + acc) == INT_MAX){ return;}
//    cout << *(_arr + acc) << endl;

//    return con_Iarr(_arr, size, acc + 1);

//}


//template <typename T, typename enable_if_t<is_same<T, int(*)[6]>::value>* = nullptr>
//inline auto uPai6_pai6( T _pai6, int row = 3 ){


//    std::unique_ptr<int*, std::function<void(int**)>> uPtr(new int*[row](),
//        [](int** x) {std::for_each(x, x + 3, std::default_delete<int[]>());
//                     delete[] x;});

//    for(size_t i = 0 ; i < row ; i++){
//        uPtr.get()[i] = new int[6];
//    }

//    for(int i = 0 ; i < row ; ++i){
//        for(int j = 0 ; j < 6 ; ++j){
//            uPtr.get()[i][j] = _pai6[i][j];
//        }
//    }


//    delete[] _pai6;
//    return uPtr;

//}

//template < typename T , typename Fuc, typename enable_if_t<is_class<T>::value>* = nullptr>
//inline auto map_uPai6( T& _uPai6, Fuc _fuc, int row = 3){

//    for(int i = 0 ; i < row ; ++i){
//        for(int j = 0 ; j < 6 ; ++j){
//           _fuc(_uPai6.get()[i][j]);
//        }
//    }
//}
//template < typename T , typename Fuc, typename enable_if_t<is_pointer<T>::value>* = nullptr>
//inline auto map_Pai6( T& _uPai6, Fuc _fuc, int row = 3){

//    for(int i = 0 ; i < row ; ++i){
//        for(int j = 0 ; j < 6 ; ++j){
//           _fuc(_uPai6[i][j]);
//        }
//    }
//}














































#endif // KH_01_HPP
