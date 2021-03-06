
/*****************************************************************//**
 * \file   Matrix44.cpp
 * \brief  4次元行列
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <cmath>
#include "Utility.h"
#include "Matrix44.h"
#include "Vector4.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief 算術関係
    */
   namespace Math {

      Matrix44::Matrix44() {
         Unit();
      }

      Matrix44::Matrix44(const MatrixArray values) {
         this->_rowColumn = values;
      }

      void Matrix44::Zero(MatrixArray& target) const {
         for (auto i = 0; i < _rowMax; ++i) {
            target[i].fill(0.0);
         }
      }

      void Matrix44::Unit() {
         Zero(_rowColumn);

         for (auto i = 0; i < _rowMax; ++i) {
            _rowColumn[i][i] = 1.0;
         }
      }

      // 本来はカメラに属する処理だが便宜上実装する
      void Matrix44::LookAt(const Vector4& position, const Vector4& target, const Vector4& up) {
         auto axis_z = target - position;
         auto axis_x = up.Cross(axis_z);
         auto axis_y = axis_z.Cross(axis_x);

         axis_x.Normalized();
         axis_y.Normalized();
         axis_z.Normalized();

         auto [xx, xy, xz] = axis_x.GetVec3();
         auto [yx, yy, yz] = axis_y.GetVec3();
         auto [zx, zy, zz] = axis_z.GetVec3();

         _rowColumn[0][0] = xx;
         _rowColumn[0][1] = yx;
         _rowColumn[0][2] = zx;
         _rowColumn[0][3] = 0.0;

         _rowColumn[1][0] = xy;
         _rowColumn[1][1] = yy;
         _rowColumn[1][2] = zy;
         _rowColumn[1][3] = 0.0;

         _rowColumn[2][0] = xz;
         _rowColumn[2][1] = yz;
         _rowColumn[2][2] = zz;
         _rowColumn[2][3] = 0.0;

         _rowColumn[3][0] = -axis_x.Dot(position);
         _rowColumn[3][1] = -axis_y.Dot(position);
         _rowColumn[3][2] = -axis_z.Dot(position);
         _rowColumn[3][3] = 1.0;
      }

      void Matrix44::Perspective(const double fov_y, const double aspect, const double near_z, const double far_z) {
         auto cot = 1.0 / tan(fov_y * 0.5);
         auto range = far_z - near_z;
         auto temp = far_z / range;

         Zero(_rowColumn);

         _rowColumn[0][0] = cot * aspect;
         _rowColumn[1][1] = cot;
         _rowColumn[2][2] = temp;
         _rowColumn[2][3] = 1.0;
         _rowColumn[3][2] = -near_z * temp;
      }

      void Matrix44::Viewport(const double width, const double height) {
         auto w = width * 0.5;
         auto h = height * 0.5;

         Unit();

         _rowColumn[0][0] = w;
         _rowColumn[1][1] = -h;
         _rowColumn[3][0] = w;
         _rowColumn[3][1] = h;
      }

      const Matrix44 Matrix44::operator *(const Matrix44 rhs) const {
         MatrixArray result{ {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}} };

         for (auto i = 0; i < _rowMax; ++i) {
            for (auto j = 0; j < _columnMax; ++j) {
               auto m = 0.0;

               for (auto k = 0; k < 4; ++k) {
                  m += _rowColumn[i][k] * rhs.GetValue(k, j);
               }

               result[i][j] = m;
            }
         }

         return Matrix44(result);
      }

      const Matrix44 Matrix44::operator +(const Vector4 rhs) const {
         MatrixArray result = _rowColumn;

         auto [x, y, z] = rhs.GetVec3();
         result[3][0] += x;
         result[3][1] += y;
         result[3][2] += z;

         return Matrix44(result);
      }

      void Matrix44::Transfer(const double x, const double y, const double z, bool make) {
         if (make) {
            Unit();
         }

         _rowColumn[3][0] += x;
         _rowColumn[3][1] += y;
         _rowColumn[3][2] += z;
      }

      void Matrix44::Scale(const double x, const double y, const double z, bool make) {
         if (make) {
            Unit();
         }

         _rowColumn[0][0] *= x;
         _rowColumn[1][1] *= y;
         _rowColumn[2][2] *= z;
      }

      void Matrix44::RotateX(const double degree, bool make) {
         auto [sin, cos] = GetSinCos(degree);

         if (make) {
            Unit();

            _rowColumn[1][1] = cos;
            _rowColumn[1][2] = sin;
            _rowColumn[2][1] = -sin;
            _rowColumn[2][2] = cos;
         }
         else {
            MatrixArray array{ {{1, 0, 0, 0}, {0, cos, sin, 0}, {0, -sin, cos, 0}, {0, 0, 0, 1}} };
            Matrix44 rotX(array);

            *this = *this * rotX;
         }
      }

      void Matrix44::RotateY(const double degree, bool make) {
         auto [sin, cos] = GetSinCos(degree);

         if (make) {
            Unit();

            _rowColumn[0][0] = cos;
            _rowColumn[0][2] = -sin;
            _rowColumn[2][0] = sin;
            _rowColumn[2][2] = cos;
         }
         else {
            MatrixArray array{ {{cos, 0, -sin, 0}, {0, 1, 0, 0}, {sin, 0, cos, 0}, {0, 0, 0, 1}} };
            Matrix44 rotY(array);

            *this = *this * rotY;
         }
      }

      void Matrix44::RotateZ(const double degree, bool make) {
         auto [sin, cos] = GetSinCos(degree);

         if (make) {
            Unit();

            _rowColumn[0][0] = cos;
            _rowColumn[0][1] = sin;
            _rowColumn[1][0] = -sin;
            _rowColumn[1][1] = cos;
         }
         else {
            MatrixArray array{ {{cos, sin, 0, 0}, {-sin, cos, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}} };
            Matrix44 rotZ(array);

            *this = *this * rotZ;
         }
      }

      void Matrix44::RotateAnyVec(const Vector4 vec, const double degree, bool make) {
          auto anyVec = vec.Normalize();
          auto [sin, cos] = GetSinCos(degree);
          auto [x, y, z] = anyVec.GetVec3();

          if (make) {
              Unit();

              _rowColumn[0][0] = x * x * (1 - cos) + cos;
              _rowColumn[0][1] = x * y * (1 - cos) - z * sin;
              _rowColumn[0][2] = x * z * (1 - cos) + y * sin;
              _rowColumn[1][0] = y * x * (1 - cos) + z * sin;
              _rowColumn[1][1] = y * y * (1 - cos) + cos;
              _rowColumn[1][2] = y * z * (1 - cos) - x * sin;
              _rowColumn[2][0] = z * x * (1 - cos) - y * sin;
              _rowColumn[2][1] = z * y * (1 - cos) + x * sin;
              _rowColumn[2][2] = z * z * (1 - cos) + cos;
          }
          else {
              MatrixArray array{ {{x * x * (1 - cos) + cos, x * y * (1 - cos) - z * sin, x * z * (1 - cos) + y * sin, 0}, {y * x * (1 - cos) + z * sin, y * y * (1 - cos) + cos, y * z * (1 - cos) - x * sin, 0}, {z * x * (1 - cos) - y * sin, z * y * (1 - cos) + x * sin, z * z * (1 - cos) + cos, 0}, {0, 0, 0, 1}} };
              Matrix44 rotAny(array);

              *this = *this * rotAny;
          }

      }

      void Matrix44::RotateAnyVecQuaternion(const Vector4 vec, const double degree, bool make) {
          // クォータニオン生成
          Quaternion qRot;
          // 任意軸の単位化
          auto anyVec = vec.Normalize();
          auto [sin, cos] = GetSinCos(degree / 2.0);
          auto [x, y, z] = anyVec.GetVec3();
          x = sin * x;
          y = sin * y;
          z = sin * z;
          auto w = cos;
          qRot = std::make_tuple(x, y, z, w);

          if (make) {
              Unit();

              _rowColumn[0][0] = 1.0 - 2.0 * (std::get<1>(qRot) * std::get<1>(qRot) + std::get<2>(qRot) * std::get<2>(qRot));
              _rowColumn[0][1] = 2.0 * (std::get<0>(qRot) * std::get<1>(qRot) - std::get<3>(qRot) * std::get<2>(qRot));
              _rowColumn[0][2] = 2.0 * (std::get<0>(qRot) * std::get<2>(qRot) + std::get<3>(qRot) * std::get<1>(qRot));
              _rowColumn[1][0] = 2.0 * (std::get<0>(qRot) * std::get<1>(qRot) + std::get<3>(qRot) * std::get<2>(qRot));
              _rowColumn[1][1] = 1.0 - 2.0 * (std::get<0>(qRot) * std::get<0>(qRot) + std::get<2>(qRot) * std::get<2>(qRot));
              _rowColumn[1][2] = 2.0 * (std::get<1>(qRot) * std::get<2>(qRot) - std::get<3>(qRot) * std::get<0>(qRot));
              _rowColumn[2][0] = 2.0 * (std::get<0>(qRot) * std::get<2>(qRot) - std::get<3>(qRot) * std::get<1>(qRot));
              _rowColumn[2][1] = 2.0 * (std::get<1>(qRot) * std::get<2>(qRot) + std::get<3>(qRot) * std::get<0>(qRot));
              _rowColumn[2][2] = 1.0 - 2.0 * (std::get<0>(qRot) * std::get<0>(qRot) + std::get<1>(qRot) * std::get<1>(qRot));
          }
          else {
              MatrixArray array{ {{1.0 - 2.0 * (std::get<1>(qRot) * std::get<1>(qRot) + std::get<2>(qRot) * std::get<2>(qRot)), 2.0 * (std::get<0>(qRot) * std::get<1>(qRot) - std::get<3>(qRot) * std::get<2>(qRot)), 2.0 * (std::get<0>(qRot) * std::get<2>(qRot) + std::get<3>(qRot) * std::get<1>(qRot)), 0},
                                 {2.0 * (std::get<0>(qRot) * std::get<1>(qRot) + std::get<3>(qRot) * std::get<2>(qRot)), 1.0 - 2.0 * (std::get<0>(qRot) * std::get<0>(qRot) + std::get<2>(qRot) * std::get<2>(qRot)), 2.0 * (std::get<1>(qRot) * std::get<2>(qRot) - std::get<3>(qRot) * std::get<0>(qRot)), 0},
                                 {2.0 * (std::get<0>(qRot) * std::get<2>(qRot) - std::get<3>(qRot) * std::get<1>(qRot)), 2.0 * (std::get<1>(qRot) * std::get<2>(qRot) + std::get<3>(qRot) * std::get<0>(qRot)), 1.0 - 2.0 * (std::get<0>(qRot) * std::get<0>(qRot) + std::get<1>(qRot) * std::get<1>(qRot)), 0},
                                 {0, 0, 0, 1}} };
              Matrix44 rotAny(array);

              *this = *this * rotAny;
          }
      }

      const Matrix44 Matrix44::GetRotate() const {
         MatrixArray noneTransfer = _rowColumn;

         noneTransfer[3][0] = 0.0;
         noneTransfer[3][1] = 0.0;
         noneTransfer[3][2] = 0.0;

         return Matrix44(noneTransfer);
      }

      std::tuple<double, double> Matrix44::GetSinCos(const double degree) const {
         auto radian = Utility::DegreeToRadian(degree);

         return std::make_tuple(std::sin(radian), std::cos(radian));
      }
   }
}