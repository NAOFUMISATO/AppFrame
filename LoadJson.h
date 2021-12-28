
/*****************************************************************//**
 * \file   LoadJson.h
 * \brief  �f�ޏ���Json�t�@�C������ǂݍ��ރN���X
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
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   //��d�C���N���[�h�h�~
   namespace Game {
      class GameBase;
   }
   /**
    * \brief ���\�[�X�֌W
    */
   namespace Resource {
      //��d�C���N���[�h�h�~
      class Texture;
      /**
       * \class �f�ޏ���Json�t�@�C������ǂݍ��ރN���X
       * \brief ���\�[�X�T�[�o�[�ɓo�^����f�ޏ���json�t�@�C������ǂݍ���
       */
      class LoadJson {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameBase �Q�[���x�[�X�̎Q��
          */
         LoadJson(Game::GameBase& gameBase);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~LoadJson() = default;
         /**
          * \brief json�t�@�C������摜����ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonFileName json�t�@�C����
          */
         void LoadTextures(const std::filesystem::path jsonFileName);
         /**
          * \brief json�t�@�C�����烂�f������ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonFileName json�t�@�C����
          */
         void LoadModels(const std::filesystem::path jsonFileName);
         /**
          * \brief json�t�@�C�����特������ǂݍ��݁A���\�[�X�T�[�o�[�ɓo�^����
          * \param jsonFileName json�t�@�C����
          */
         void LoadSounds(const std::filesystem::path jsonFileName);

      private:
         Game::GameBase& _gameBase;   //!< �Q�[���x�[�X�̎Q��
      };
   }
}
