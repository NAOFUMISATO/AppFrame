
/*****************************************************************//**
 * \file   Utility.cpp
 * \brief  有用算術
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <random>
#include <algorithm>
#include <cmath>
#include "vector4.h"
#include "utility.h"
namespace {
   std::random_device _seedGen;
   std::mt19937_64 random(_seedGen());
}
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame {
   /**
    * \brief 算術関係
    */
   namespace Math {

      int Utility::GetRandom(const int min, const int max) {
         std::uniform_int_distribution<> distr(min, max);
         return distr(random);
      }
      double Utility::GetRandom(const double min, const double max) {
         std::uniform_real_distribution<> distr(min, max);
         return distr(random);
      }
      float Utility::GetRandom(const float min, const float max) {
         std::uniform_real_distribution<> distr(min, max);
         return static_cast<float>(distr(random));
      }

      // 矩形と線分の当たり判定
      collision Utility::CollisionPolygonLine(const Vector4& polygon_point0, const Vector4& polygon_point1,
         const Vector4& polygon_point2, const Vector4& polygon_point3,
         const Vector4& line_start, const Vector4& line_end) {
         // 平面の情報をセット
         const auto normal = GetNormal(polygon_point0, polygon_point1, polygon_point2);
         const auto plane = std::make_tuple(polygon_point0, normal);
         auto result = std::make_tuple(false, Vector4());

         // 平面と線分が交わるか
         if (!CollisionPlaneLine(plane, line_start, line_end, result)) {
            return result;
         }

         // 平面との交点
         const Vector4 point = std::get<1>(result);

         // 矩形を三角形に分けて判定
         if (InsideTrianglePoint(polygon_point0, polygon_point1, polygon_point2, point)) {
            std::get<0>(result) = true;
            std::get<1>(result) = point;

            return result;
         }

         if (InsideTrianglePoint(polygon_point1, polygon_point3, polygon_point2, point)) {
            std::get<0>(result) = true;
            std::get<1>(result) = point;

            return result;
         }

         std::get<0>(result) = false;

         return result;
      }

      // 面の法線を求める
      Vector4 Utility::GetNormal(const Vector4& point0, const Vector4& point1, const Vector4& point2) {
         // 面の法線を求める
         const auto v0 = point1 - point0;
         const auto v1 = point2 - point0;
         auto cross = v0.Cross(v1);

         return cross.Normalize();
      }

      // 三角形と点の内外判定(point は必ず三角形と同平面である事)
      bool Utility::InsideTrianglePoint(const Vector4& triangle_point0, const Vector4& triangle_point1, const Vector4& triangle_point2, const Vector4& point) {
         const auto v01 = triangle_point1 - triangle_point0;
         const auto v12 = triangle_point2 - triangle_point1;
         const auto v21 = triangle_point0 - triangle_point2;
         const auto v0p = point - triangle_point0;
         const auto v1p = point - triangle_point1;
         const auto v2p = point - triangle_point2;

         auto c0 = v01.Cross(v1p);
         auto c1 = v12.Cross(v2p);
         auto c2 = v21.Cross(v0p);

         c0.Normalized();
         c1.Normalized();
         c2.Normalized();

         const auto dot01 = c0.Dot(c1);
         const auto dot02 = c0.Dot(c2);

         return (dot01 > 0.0) && (dot02 > 0.0);
      }

      // 平面と線分の当たり判定
      bool Utility::CollisionPlaneLine(const plane& p, const Vector4& line_start, const Vector4& line_end, collision& result) {
         const Vector4 plane_point = std::get<0>(p);
         const Vector4 plane_normal = std::get<1>(p);
         auto point_to_start = line_start - plane_point;
         auto point_to_end = line_end - plane_point;

         point_to_start.Normalized();
         point_to_end.Normalized();

         const auto dot_p_s = plane_normal.Dot(point_to_start);
         const auto dot_p_e = plane_normal.Dot(point_to_end);
         const auto pattern_a = (dot_p_s >= 0.0) && (dot_p_e <= 0.0);
         const auto pattern_b = (dot_p_s <= 0.0) && (dot_p_e >= 0.0);
         const auto ret = pattern_a || pattern_b;

         if (ret) {
            // 平面と線分の交点
            const auto line = line_end - line_start;
            const auto ratio = std::abs(dot_p_s) / std::abs(dot_p_s) + std::abs(dot_p_e);
            const auto point = line_start + (line * ratio);

            std::get<1>(result) = point;
         }

         return ret;
      }
   }
}