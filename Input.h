#pragma once

#include <DxLib.h>

namespace
{
	enum InputState
	{
		PUSH_DOWN = 1,	   //�����ꂽ�u��
		PUSH,			   //������Ă���Ƃ�
		RELEASE
	};

	enum Vector2D
	{
		X,
		Y,
		MAX_2D
	};
	//�R���g���[���[�̃r�b�g�������Ԃɗ񋓐錾���Ă���
	enum InputPad
	{
		PAD_DOWN,		  //StickDirection			   Bit1
		PAD_LEFT,		  //StickDirection			   Bit2
		PAD_RIGHT,		  //StickDirection			   Bit4
		PAD_UP,			  //StickDirection			   Bit8
		PAD_BUTTON_1,	  //ButtonLeft				   Bit16
		PAD_BUTTON_2,	  //ButtonUp				   Bit32
		PAD_BUTTON_3,	  //ButtonDown				   Bit64
		PAD_BUTTON_4,	  //ButtonRight				   Bit128
		PAD_BUTTON_5,	  //ButtonLeftR1			   Bit256
		PAD_BUTTON_6,	  //ButtonRightR1			   Bit512
		PAD_BUTTON_7,	  //ButtonLeftR2			   Bit1024
		PAD_BUTTON_8,	  //ButtonRightR2			   Bit2048
		PAD_BUTTON_9,	  //ButtonSelect			   Bit4096
		PAD_BUTTON_10,	  //ButtonStart				   Bit8192
		PAD_BUTTON_11,	  //StickButtonLeft			   Bit16384
		PAD_BUTTON_12,	  //StickButtonRight		   Bit32768
		PAD_MAX
	};
	const int DEFAULT_POSITION = -1;		//�������W�@��ʃT�C�Y��菬�����l����
	const int DEFAULT_TIME = -1;			//�������ԁ@�J�E���g�l��菬�����l����
}

//���̓N���X
class CInput
{
public:
	static void Update();
	static CInput& CreateInstance()
	{
		//�C���X�^���X�𐶐�
		static CInput Instance;
		return Instance;
	}
	//�L�[�{�[�h���͂�PUSH����
	bool GetKey(int DX_KeyCode)const;
	bool GetKeyDown(int DX_KeyCode)const;
	//�p�b�h���͂�PUSH����
	bool GetPad(int DX_PadCode)const;
	bool GetPadDown(int DX_PadCode)const;
	//�}�E�X���͂�PUSH
	int GetMouseX() const;
	int GetMouseY() const;
	bool GetMouseButton(int DX_MouseCode) const;
	bool GetMouseButtonDown(int DX_MouseCode) const;
	bool GetMouseDoubleButtonDown(int DX_MouseCode) const;
	bool GetMouseButtonRelease(int DX_MouseCode) const;

	~CInput() = default;
private:
	//���������\�b�h�֐��錾������
	//�L�[�{�[�h���͂̏��X�V
	static void InputKey();
	//�p�b�h���͂̏��X�V
	static void InputPad();		
	//�p�b�h�̃r�b�g���̎擾
	static bool loopBit(int PadState,const int Array);		
	//�}�E�X���͂̏��X�V
	static void InputMouse();
	//�}�E�X�_�u���N���b�N����
	static void CheckDoubleClick(int DX_MouseCode);
	//�}�E�X�̍��W�ƌ��݂̎��Ԃ��Z�b�g
	static void SetDoubleClickInfor(int DX_MouseCode);

	//�����������o�ϐ��錾������
	static int Key[256];									//�L�[�{�[�h�̓��͏����Ǘ�
	static int Pad[PAD_MAX];								//�p�b�h�̓��͏����Ǘ�
	static int Click[128];									//�N���b�N�̏�Ԃ��i�[����z��
	static int DoubleClick[128];							//�_�u���N���b�N�̏�Ԃ��i�[����z��
	static int MousePos[MAX_2D];							//�}�E�X�̈ʒu���i�[����z��
	static int SavePos[MAX_2D];								//�N���b�N���ꎞ�I�ɍ��W��ۑ�����
	static int TimeCount;									//�_�u���N���b�N�Ԋu�̃^�C�}�[
};

namespace
{
	CInput &Input = CInput::CreateInstance();				//�w�b�_�[�ǂݍ��ݎ��C���X�^���X
}