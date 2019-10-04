#pragma once

#include <DxLib.h>

namespace
{
	enum InputState
	{
		PUSH_DOWN = 1,	   //押された瞬間
		PUSH,			   //押されているとき
		RELEASE
	};

	enum Vector2D
	{
		X,
		Y,
		MAX_2D
	};
	//コントローラーのビット情報を順番に列挙宣言していく
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
	const int DEFAULT_POSITION = -1;		//初期座標　画面サイズより小さい値を代入
	const int DEFAULT_TIME = -1;			//初期時間　カウント値より小さい値を代入
}

//入力クラス
class CInput
{
public:
	static void Update();
	static CInput& CreateInstance()
	{
		//インスタンスを生成
		static CInput Instance;
		return Instance;
	}
	//キーボード入力のPUSH判定
	bool GetKey(int DX_KeyCode)const;
	bool GetKeyDown(int DX_KeyCode)const;
	//パッド入力のPUSH判定
	bool GetPad(int DX_PadCode)const;
	bool GetPadDown(int DX_PadCode)const;
	//マウス入力のPUSH
	int GetMouseX() const;
	int GetMouseY() const;
	bool GetMouseButton(int DX_MouseCode) const;
	bool GetMouseButtonDown(int DX_MouseCode) const;
	bool GetMouseDoubleButtonDown(int DX_MouseCode) const;
	bool GetMouseButtonRelease(int DX_MouseCode) const;

	~CInput() = default;
private:
	//＜＜＜メソッド関数宣言＞＞＞
	//キーボード入力の情報更新
	static void InputKey();
	//パッド入力の情報更新
	static void InputPad();		
	//パッドのビット情報の取得
	static bool loopBit(int PadState,const int Array);		
	//マウス入力の情報更新
	static void InputMouse();
	//マウスダブルクリック処理
	static void CheckDoubleClick(int DX_MouseCode);
	//マウスの座標と現在の時間をセット
	static void SetDoubleClickInfor(int DX_MouseCode);

	//＜＜＜メンバ変数宣言＞＞＞
	static int Key[256];									//キーボードの入力情報を管理
	static int Pad[PAD_MAX];								//パッドの入力情報を管理
	static int Click[128];									//クリックの状態を格納する配列
	static int DoubleClick[128];							//ダブルクリックの状態を格納する配列
	static int MousePos[MAX_2D];							//マウスの位置を格納する配列
	static int SavePos[MAX_2D];								//クリック時一時的に座標を保存する
	static int TimeCount;									//ダブルクリック間隔のタイマー
};

namespace
{
	CInput &Input = CInput::CreateInstance();				//ヘッダー読み込み時インスタンス
}