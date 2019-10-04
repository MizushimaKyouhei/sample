#include "..\\Input\Input.h"

//クラス内の静的データメンバの初期化
int CInput::Key[256] = { 0 };
int CInput::Pad[PAD_MAX] = { 0 };
int CInput::Click[128] = { 0 };
int CInput::DoubleClick[128] = { 0 };
int CInput::MousePos[MAX_2D] = { 0 };
int CInput::SavePos[MAX_2D] = { DEFAULT_POSITION, DEFAULT_POSITION };
int CInput::TimeCount = DEFAULT_TIME;

//キーが押されている間
bool CInput::GetKey(int DX_KeyCode) const{ return (Key[DX_KeyCode] == PUSH); }

//キーが押された瞬間
bool CInput::GetKeyDown(int DX_KeyCode) const{ return (Key[DX_KeyCode] == PUSH_DOWN); }

//パッドが入力されている間
bool CInput::GetPad(int DX_PadCode)const{ return (Pad[DX_PadCode] == PUSH); }

//パッドが押された瞬間
bool CInput::GetPadDown(int DX_PadCode)const{ return (Pad[DX_PadCode] == PUSH_DOWN); }

//マウスカーソルのX座標を取得
int CInput::GetMouseX() const{ return MousePos[X]; }

//マウスカーソルのY座標を取得
int CInput::GetMouseY() const{ return MousePos[Y]; }

//マウスが入力されている間
bool CInput::GetMouseButton(int DX_MouseCode) const { return (Click[DX_MouseCode] == PUSH); }

//マウスが入力された瞬間
bool CInput::GetMouseButtonDown(int DX_MouseCode) const { return (Click[DX_MouseCode] == PUSH_DOWN); }

//マウスが離された瞬間
bool CInput::GetMouseButtonRelease(int DX_MouseCode) const { return (Click[DX_MouseCode] == RELEASE); }

//マウスのダブルクリック入力された瞬間
bool CInput::GetMouseDoubleButtonDown(int DX_MouseCode) const { return (DoubleClick[DX_MouseCode] == PUSH_DOWN); }

//入力クラスの更新
void CInput::Update()
{
	InputKey();
	InputPad();
	InputMouse();
}

//入力情報の処理
void CInput::InputKey()
{
	static char buf[256];
	
	//すべてのキーの状態を格納
	GetHitKeyStateAll(buf);

	for (int i = 0; i < 256; ++i){
		//いずれかのキーを押されたら
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

//入力情報の処理
void CInput::InputPad()
{
	static int PadState;
	//すべてのキーの状態を格納

	PadState = GetJoypadInputState(DX_INPUT_PAD1);

	if(PadState != 0) {									   //入力情報があれば
		for (int i = 0; i < PAD_MAX; ++i){
			if (Pad[i] == 0 &&							   //入力された瞬間の処理
				!CInput::loopBit(PadState, i)){
				Pad[i] = PUSH_DOWN;
			}
			else if (Pad[i] == PUSH_DOWN &&					//入力されている間の処理
				!CInput::loopBit(PadState, i)){
				Pad[i] = PUSH;
			}
			else if (Pad[i] == PUSH &&						//入力情報があるがそのものの入力情報ではないとき
				CInput::loopBit(PadState, i)){
				Pad[i] = 0;
			}
		}
	}
	else{													//入力情報がなければすべてのパッド情報をリセット（0）にする
		for (int i = 0; i < PAD_MAX; ++i){
			if (Pad[i] == 0){ continue; }
			Pad[i] = 0;
		}
	}
}

//パッド情報のバイナリ情報を分析処理
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

//マウスの状態を監視する関数
void CInput::InputMouse()
{
	GetMousePoint(&MousePos[X], &MousePos[Y]);			 //マウスカーソルの座標を格納

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
	//TimeCountが記録されていないとき初期化処理
	if (TimeCount == DEFAULT_TIME){
		if (SavePos[X] == DEFAULT_POSITION &&
			SavePos[Y] == DEFAULT_POSITION){
			TimeCount = GetNowCount();
			SavePos[X] = MousePos[X];
			SavePos[Y] = MousePos[Y];
		}
	}
	else {	//TimeCountが記録されているとき
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

//マウスダブルクリック処理
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