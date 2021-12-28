
/*****************************************************************//**
 * \file   GameBase.cpp
 * \brief  ゲーム本体の基底
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include "GameBase.h"
#include <DxLib.h>
#include "ModeServer.h"
#include "InputManager.h"
#include "ResourceServer.h"
#include "SoundServer.h"
#include "PathServer.h"
#include "ResourceLoadJson.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief ゲームベース
    */
   namespace Game {
      GameBase* GameBase::_gameInstance = nullptr;

      GameBase::GameBase() {
         _gameInstance = this;
      };

      GameBase::~GameBase() {
      };
      bool GameBase::Initialize(HINSTANCE hinstance) {
         //デバッグモードならウインドウモードに設定及び、デバッグログの出力を行う
#ifdef _DEBUG
         AppWindowMode(true);
         OutputDebugLog(true);
#else
         AppWindowMode(false);
         OutputDebugLog(false);
#endif
         //画面サイズ設定
         auto [width, height, depth] = GraphSize();
         SetGraphMode(width, height, depth);
         //エラーが起きたら直ちに終了
         if (DxLib_Init() == -1) {
            return false;
         }
         SetBackgroundColor(0, 0, 255);
         //描画先を裏画面にセット
         SetDrawScreen(DX_SCREEN_BACK);
         // Ｚバッファを有効にする
         SetUseZBuffer3D(TRUE);
         // Ｚバッファへの書き込みを有効にする
         SetWriteZBuffer3D(TRUE);

         _inputManager = std::make_unique<Input::InputManager>();

         _resServer = std::make_unique<Resource::ResourceServer>(*this);

         _soundServer = std::make_unique<Sound::SoundServer>(*this);

         _pathServer = std::make_unique<Path::PathServer>();

         _resJson = std::make_unique<Resource::LoadJson>(*this);

         return true;
      }

      void GameBase::Run() {
         //メインループ
         while (_gState != GAME_STATE::QUIT) {
            Input();  // 入力
            Update(); // 更新
            Render(); // 描画
         }
      }

      void GameBase::ShutDown() {
         DxLib_End();
      }


      void GameBase::Input() {
         if (ProcessMessage() == -1) {
            _gState = GAME_STATE::QUIT;  // -1 が返ってきたのでゲームを終了する
         }
         // 入力状態の更新
         _inputManager->Update();
         if (_inputManager->GetKeyboard().EscClick()) {
            _gState = GAME_STATE::QUIT;  // ESC押されたのでゲームを終了する
         }
         _modeServer->Input(*_inputManager);    // モードサーバーの入力処理を実行
      }

      void GameBase::Update() {
         //モードの更新処理
         _modeServer->Update();
      }

      void GameBase::Render() {
         //モードの描画処理
         ClearDrawScreen();
         _modeServer->Render();
         ScreenFlip();
      }
   }
}