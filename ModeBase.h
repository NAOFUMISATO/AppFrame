#pragma once
/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  ���[�h���N���X
 *
 * \author NAOFUMISATO
 * \date   October 2021
 *********************************************************************/
#include <memory>
#include "ParamBase.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   // ��d�C���N���[�h�h�~
   namespace Resource {
      class ResourceServer;
      class LoadResourceJson;
   }
   namespace Input {
      class InputManager;
   }
   namespace Sound {
      class SoundComponent;
   }
   namespace Texture {
      class TextureComponent;
   }
   namespace Sprite {
      class SpriteServer;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      //��d�C���N���[�h�h�~
      class ModeServer;
      /**
       * \class �e���[�h�̊��N���X
       * \brief ModeServer�ɓo�^���郂�[�h�̊��N���X
       *        ���̃N���X���p�����ă��[�h���`����
       */
      class ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          */
         ModeBase() {};
         /**
          * \brief ����������
          */
         virtual void Init() = 0;
         /**
          * \brief ��������
          */
         virtual void Enter() {};
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         virtual void Input(Input::InputManager& input) {};
         /**
          * \brief �X�V����
          */
         virtual void Update() {};
         /**
          * \brief �X�V����
          */
         virtual void Render() {};
         /**
          * \brief �o������
          */
         virtual void Exit() {};

         /**
          * \brief �t�F�[�h���Ԃ̐ݒ�
          * \param fadeType �t�F�[�h���Ԃ��w�肷�镶��
          */
         inline void fadeType(char fadeType) { _fadeType = fadeType; }

      protected:
         std::unique_ptr<Param::ParamBase> _param; //!< �l�Ǘ��p�N���X�̃|�C���^
         char _fadeType{ 'M' };                    //!< �t�F�[�h���Ԃ��w�肷�镶��
      };
   }
}