
/*****************************************************************//**
 * \file   LoadJson.h
 * \brief  素材情報をJsonファイルから読み込むクラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#pragma once
#include <DxLib.h>
#include <unordered_map>
#include <string_view>
#include <filesystem>
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   //二重インクルード防止
   namespace Game {
      class GameBase;
   }
   /**
    * \brief リソース関係
    */
   namespace Resource {
      //二重インクルード防止
      class Texture;
      /**
       * \class 素材情報をJsonファイルから読み込むクラス
       * \brief リソースサーバーに登録する素材情報及び各データをjsonファイルから読み込む
       */
      class LoadJson {
      public:
         /**
          * \brief コンストラクタ
          * \param gameBase ゲームベースの参照
          */
         LoadJson(Game::GameBase& gameBase);
         /**
          * \brief デフォルトデストラクタ
          */
         ~LoadJson() = default;
         /**
          * \brief jsonファイルから画像情報を読み込み、リソースサーバーに登録する
          * \param jsonFileName jsonファイル名
          */
         void LoadTextures(const std::filesystem::path jsonName);
         /**
          * \brief jsonファイルからモデル情報を読み込み、リソースサーバーに登録する
          * \param jsonFileName jsonファイル名
          */
         void LoadModels(const std::filesystem::path jsonName);
         /**
          * \brief jsonファイルから音源情報を読み込み、リソースサーバーに登録する
          * \param jsonFileName jsonファイル名
          */
         void LoadSounds(const std::filesystem::path jsonName);
         /**
          * \brief jsonファイルから各データを連想配列に登録する
          * \param jsonFileName jsonファイル名
          */
         void LoadParams(const std::filesystem::path jsonName);
         /**
          * \brief 連想配列に登録したデータを取得する
          * \param jsonFileName jsonファイル名
          * \param paramName データ名
          * \return データ
          */
         template<typename T>
         T GetParams(const std::filesystem::path jsonName,const std::filesystem::path paramName);

      private:
         Game::GameBase& _gameBase;   //!< ゲームベースの参照
         std::unordered_map<std::string, std::unordered_map<std::string, double>> _params;   //!< 各データを登録する連想配列
      };
   }
}
