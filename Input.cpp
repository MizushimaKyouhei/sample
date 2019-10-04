#include "..\\Input\Input.h"

//�N���X���̐ÓI�f�[�^�����o�̏�����
int CInput::Key[256] = { 0 };
int CInput::Pad[PAD_MAX] = { 0 };
int CInput::Click[128] = { 0 };
int CInput::DoubleClick[128] = { 0 };
int CInput::MousePos[MAX_2D] = { 0 };
int CInput::SavePos[MAX_2D] = { DEFAULT_POSITION, DEFAULT_POSITION };
int CInput::TimeCount = DEFAULT_TIME;

//�L�[��������Ă����
bool CInput::GetKey(int DX_KeyCode) const{ return (Key[DX_KeyCode] == PUSH); }

//�L�[�������ꂽ�u��
bool CInput::GetKeyDown(int DX_KeyCode) const{ return (Key[DX_KeyCode] == PUSH_DOWN); }

//�p�b�h�����͂���Ă����
bool CInput::GetPad(int DX_PadCode)const{ return (Pad[DX_PadCode] == PUSH); }

//�p�b�h�������ꂽ�u��
bool CInput::GetPadDown(int DX_PadCode)const{ return (Pad[DX_PadCode] == PUSH_DOWN); }

//�}�E�X�J�[�\����X���W���擾
int CInput::GetMouseX() const{ return MousePos[X]; }

//�}�E�X�J�[�\����Y���W���擾
int CInput::GetMouseY() const{ return MousePos[Y]; }

//�}�E�X�����͂���Ă����
bool CInput::GetMouseButton(int DX_MouseCode) const { return (Click[DX_MouseCode] == PUSH); }

//�}�E�X�����͂��ꂽ�u��
bool CInput::GetMouseButtonDown(int DX_MouseCode) const { return (Click[DX_MouseCode] == PUSH_DOWN); }

//�}�E�X�������ꂽ�u��
bool CInput::GetMouseButtonRelease(int DX_MouseCode) const { return (Click[DX_MouseCode] == RELEASE); }

//�}�E�X�̃_�u���N���b�N���͂��ꂽ�u��
bool CInput::GetMouseDoubleButtonDown(int DX_MouseCode) const { return (DoubleClick[DX_MouseCode] == PUSH_DOWN); }

//���̓N���X�̍X�V
void CInput::Update()
{
	InputKey();
	InputPad();
	InputMouse();
}

//���͏��̏���
void CInput::InputKey()
{
	static char buf[256];
	
	//���ׂẴL�[�̏�Ԃ��i�[
	GetHitKeyStateAll(buf);

	for (int i = 0; i < 256; ++i){
		//�����ꂩ�̃L�[�������ꂽ��
		if (buf[i]){
			if (Key[i] == 0){
				Key[i] = PUSH_DOWN;
			}
			else if (Key[i] == PUSH_DOWN){
				Key[i] = PUSH;
			}
		}
		else {
			Key[i] = 0;
		}
	}
}

//���͏��̏���
void CInput::InputPad()
{
	static int PadState;
	//���ׂẴL�[�̏�Ԃ��i�[

	PadState = GetJoypadInputState(DX_INPUT_PAD1);

	if(PadState != 0) {									   //���͏�񂪂����
		for (int i = 0; i < PAD_MAX; ++i){
			if (Pad[i] == 0 &&							   //���͂��ꂽ�u�Ԃ̏���
				!CInput::loopBit(PadState, i)){
				Pad[i] = PUSH_DOWN;
			}
			else if (Pad[i] == PUSH_DOWN &&					//���͂���Ă���Ԃ̏���
				!CInput::loopBit(PadState, i)){
				Pad[i] = PUSH;
			}
			else if (Pad[i] == PUSH &&						//���͏�񂪂��邪���̂��̂̓��͏��ł͂Ȃ��Ƃ�
				CInput::loopBit(PadState, i)){
				Pad[i] = 0;
			}
		}
	}
	else{													//���͏�񂪂Ȃ���΂��ׂẴp�b�h�������Z�b�g�i0�j�ɂ���
		for (int i = 0; i < PAD_MAX; ++i){
			if (Pad[i] == 0){ continue; }
			Pad[i] = 0;
		}
	}
}

//�p�b�h���̃o�C�i�����𕪐͏���
bool CInput::loopBit(int PadState,const int Array)
{
	static int PadStateH;
	static int Cnt;
	static int MinusNum;

	PadStateH = PadState;
	Cnt = 0;

	while (PadStateH % 2 == 0){
		PadStateH /= 2;
		Cnt++;
	}
	if (PadStateH != 1){
		MinusNum = 1;
		for (int i = 0; i < Cnt; ++i){
			MinusNum *= 2;
		}
		PadState -= MinusNum;

		if (Array == Cnt){
			return false;
		}
		else{
			if (!loopBit(PadState, Array)){
				return false;
			}
			else if(loopBit(PadState,Array)){
				return true;
			}
		}
	}
	else if (PadStateH == 1){
		if (Array == Cnt){
			return false;
		}
		else{
			return true;
		}
	}
	return NULL;
}

//�}�E�X�̏�Ԃ��Ď�����֐�
void CInput::InputMouse()
{
	GetMousePoint(&MousePos[X], &MousePos[Y]);			 //�}�E�X�J�[�\���̍��W���i�[

	for (int i = 0; i < 128; i++) {
		CheckDoubleClick(i);
		if (GetMouseInput() & i) {
			if (Click[i] == 0) {
				Click[i] = PUSH_DOWN;				
			}
			else if (Click[i] == PUSH_DOWN) {
				Click[i] = PUSH;
			}
		}
		else {
			if (Click[i] == PUSH_DOWN || Click[i] == PUSH){
				Click[i] = RELEASE;
				SetDoubleClickInfor(i);
			}
			else if (Click[i] == RELEASE){
				Click[i] = 0;
			}
		}
	}
}

void CInput::SetDoubleClickInfor(int DX_MouseCode)
{
	//TimeCount���L�^����Ă��Ȃ��Ƃ�����������
	if (TimeCount == DEFAULT_TIME){
		if (SavePos[X] == DEFAULT_POSITION &&
			SavePos[Y] == DEFAULT_POSITION){
			TimeCount = GetNowCount();
			SavePos[X] = MousePos[X];
			SavePos[Y] = MousePos[Y];
		}
	}
	else {	//TimeCount���L�^����Ă���Ƃ�
		if (DoubleClick[DX_MouseCode] == 0){
			if (GetNowCount() - TimeCount < 500){
				if (SavePos[X] != DEFAULT_POSITION &&
					SavePos[Y] != DEFAULT_POSITION){
					if (SavePos[X] - MousePos[X] < 3 &&
						SavePos[X] - MousePos[X] > -3){
						if (SavePos[Y] - MousePos[Y] < 3 &&
							SavePos[Y] - MousePos[Y] > -3){
							SavePos[X] = MousePos[X];
							SavePos[Y] = MousePos[Y];
							DoubleClick[DX_MouseCode] = PUSH_DOWN;
						}
					}
				}
			}
		}
	}
}

//�}�E�X�_�u���N���b�N����
void CInput::CheckDoubleClick(int DX_MouseCode)
{
	if (GetNowCount() - TimeCount >= 500 &&
		GetNowCount() - TimeCount < 550){
		if (TimeCount != DEFAULT_TIME){
			if (SavePos[X] != DEFAULT_POSITION &&
				SavePos[Y] != DEFAULT_POSITION){
				SavePos[X] = DEFAULT_POSITION;
				SavePos[Y] = DEFAULT_POSITION;
				TimeCount = DEFAULT_TIME;
			}
		}
	}
	if (DoubleClick[DX_MouseCode] == PUSH_DOWN){
		DoubleClick[DX_MouseCode] = 0;
	}
}