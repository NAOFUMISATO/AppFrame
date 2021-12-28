
/*****************************************************************//**
 * \file   LoadJson.cpp
 * \brief  素材情報をJsonファイルから読み込む
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "LoadJson.h"
#include <nlohmann/json.hpp>
#include <tuple>
#include <string_view>
#include <stdexcept>
#include <fstream>
#include <windows.h>
#include "GameBase.h"
#include "PathServer.h"
#include "ResourceServer.h"
#include "Texture.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief リソース関係
    */
   namespace Resource {

      LoadJson::LoadJson(Game::GameBase& gameBase) :_gameBase{ gameBase } {
      };

      void LoadJson::LoadTextures(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetPath("TextureJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "ファイルは見つかりませんでした。----------\n");
            }
         }
         catch (std::logic_error& e) {
            OutputDebugString(e.what());
         }
#endif
         nlohmann::json value;
         reading >> value;
         reading.close();
         auto textureArray = value[jsonName.generic_string()];
         auto textureDirectory = _gameBase.pathServer().GetPath("Texture") / jsonName;
         for (auto& textureData : textureArray) {
            auto keyName = textureData["keyname"];
            auto fileName = textureData["filename"];
            auto allNum = textureData["allnum"];
            auto xNum = textureData["xnum"];
            auto yNum = textureData["ynum"];
            auto xSize = textureData["xsize"];
            auto ySize = textureData["ysize"];
            Texture tex = Texture();
            auto texturePath = (textureDirectory / fileName).generic_string();
            tex.SetTextureName(texturePath);
            tex.SetDivParams(std::make_tuple(allNum, xNum, yNum, xSize, ySize));
            _gameBase.resServer().LoadTexture(keyName, tex);
         }
      }

      void LoadJson::LoadModels(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetPath("ModelJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "ファイルが開けませんでした。----------\n");
            }
         }
         catch (std::logic_error& e) {
            OutputDebugString(e.what());
         }
#endif
         nlohmann::json value;
         reading >> value;
         reading.close();
         auto modelArray = value[jsonName.generic_string()];
         auto modelDirectory = _gameBase.pathServer().GetPath("Model") / jsonName;
         for (auto& modelData : modelArray) {
            auto keyName = modelData["keyname"];
            auto fileName = modelData["filename"];
            auto modelPath = (modelDirectory / fileName).generic_string();
            _gameBase.resServer().LoadModel(keyName, modelPath);
         }
      }

      void LoadJson::LoadSounds(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetPath("SoundJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "ファイルが開けませんでした。----------\n");
            }
         }
         catch (std::logic_error& e) {
            OutputDebugString(e.what());
         }
#endif
         nlohmann::json value;
         reading >> value;
         reading.close();
         auto soundArray = value[jsonName.generic_string()];
         auto soundDirectory = _gameBase.pathServer().GetPath("Sound") / jsonName;
         for (auto& soundData : soundArray) {
            auto keyName = soundData["keyname"];
            auto fileName = soundData["filename"];
            auto isLoad = soundData["isload"];
            auto volume = soundData["volume"];
            auto soundPath = (soundDirectory / fileName).generic_string();
            _gameBase.resServer().LoadSound(keyName, std::make_tuple(soundPath, isLoad, volume));
         }
      }

      void LoadJson::LoadParams(const std::filesystem::path jsonName) {
         auto jsonDirectory = _gameBase.pathServer().GetPath("ParamJson");
         auto jsonPath = (jsonDirectory / jsonName).generic_string() + ".json";
         std::ifstream reading(jsonPath, std::ios::in);
#ifdef _DEBUG
         try {
            if (reading.fail()) {
               throw std::logic_error("----------" + jsonPath + "ファイルが開けませんでした。----------\n");
            }
         }
         catch (std::logic_error& e) {
            OutputDebugString(e.what());
         }
#endif
         nlohmann::json value;
         reading >> value;
         reading.close();
         auto paramArray = value[jsonName.generic_string()];
         for (auto& paramData : paramArray) {
            for (auto& param : paramData)
               _params[paramData].emplace(param);
         }
      }

      template<typename T>
      T LoadJson::GetParams(const std::filesystem::path jsonName, const std::filesystem::path paramName) {
         if (!_params.contains(jsonName.generic_string())) {
            return -1;
         }
         if (!_params[jsonName.generic_string()].contains(paramName.generic_string())) {
            return -1;
         }
         T param = _params[jsonName.generic_string()][paramName.generic_string()];
         return param;
      }
   }
}