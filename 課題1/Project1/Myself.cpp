#include "Myself.h"
#include <Dxlib.h>
#include "_DebugConOut.h"
#include "TicketMachine.h"
#include "MouseCtl.h"
#include "_DebugDispOut.h"
#include "_DebugConOut.h"
Myself* Myself::s_Instance = nullptr;

bool Myself::Run(void)
{
	
	if (!SysInit())
	{
		return false;
	}
	if (!MyInit())
	{
		return false;
	}
	if (!LpTicketMachine.Init(mouse))
	{
		return false;
	}
	Myself::GetInstance();
	TicketMachine::GetInstance();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		mouse->Update();

		if (!LpTicketMachine.paySuccessGet())
		{
			//クリックをした瞬間、エラーの計算フラグがファルスの場合
			if (mouse->GetClickTrg()&&!_timeCount.GetFlag("エラー"))
			{
				Vector2 pos = mouse->GetPos();
				//当たり判定の位置
				if (pos.x < money_size.x)
				{
					if (pos.y < static_cast<int>(moneyType.size()) * money_size.y)
					{
						int type = moneyType[pos.y / money_size.y];
						//クリックお金の枚数が0以上、そして入金終わってない場合
						if ((cash[type] > 0)&& !LpTicketMachine.paySuccessGet())
						{
							//指定したお金の枚数を減る
							if (LpTicketMachine.GetInsert()[PayType::CASH](type));
							{
								cash[type]--;
								PlaySoundMem(_sound.soundGet("ボタン"), DX_PLAYTYPE_BACK);
							}
						}
						else
						{
							PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);
							_timeCount.Set("エラー", true, 2);
							LpTicketMachine._bug = "お金が足りない、あるいは決済完了のに入金します\n";
							TRACE("お金が足りない、あるいは決済完了のに入金します");
						}
					}
					else
					{
						//現金の範囲+1の位置がちょうど電子マネー
						if (pos.y < static_cast<int>(moneyType.size() + 1) * money_size.y)
						{
							PlaySoundMem(_sound.soundGet("ボタン"), DX_PLAYTYPE_BACK);
							LpTicketMachine.GetInsert()[PayType::CARD](NULL);
						}
					}
				}

			}
		}

		//auto moneyLine = 0;
			//if (mouse->GetClickTrg())
			//{
			//	//for (auto cashData : cash)
			//	//{
			//	//	//座標位置が特定の現金の画像の位置
			//	//	if ((mouse->GetPos().x - money_size.x / 2) * (mouse->GetPos().x - money_size.x / 2) +
			//	//		(mouse->GetPos().y - (money_size.y * moneyLine +money_size.y / 2)) * (mouse->GetPos().y - (money_size.y * moneyLine +money_size.y / 2))
			//	//		<= (money_size.y / 2) * (money_size.y / 2))
			//	//	{
			//	//		//現金の枚数が0ではない場合
			//	//		if (cashData.second > 0)
			//	//		{
			//	//			//その金券の種類の枚数を減る
			//	//			cash[cashData.first]--;
			//	//		}
			//	//	}
			//	//	moneyLine++;
			//	//}
			//}
		//}

		LpTicketMachine.Run();
		
		_timeCount.Run();
		SetDrawScreen(DX_SCREEN_BACK);										// 絵画先をバックバッファに設定 裏の画面(表示されていない画面）
		ClsDrawScreen();
		Draw();
		ScreenFlip();
	}
	return true;
}

bool Myself::MergeCash(MapInt& changeCash)
{
	//方法1：
	//auto type = LpTicketMachine.GetMoneyType();
	//for (int i = 0; i < type.size(); i++)
	//{
	//	if (changeCash.count(type[i]))
	//	{
	//		cash[type[i]] += changeCash[type[i]];
	//	}
	//}

	//方法2：
	for (auto cashData : changeCash)
	{
		cash[cashData.first] += cashData.second;
	}

	return true;
}

void Myself::Draw(void)
{	
	auto moneyLine = 0;
	SetFontSize(font_size);

	//範囲FOR文
	for (auto cashData : cash)
	{
		//お金の枚数
		DrawFormatString(money_size.x+25, money_size.y* moneyLine +money_size.y/3, GetColor(255, 255, 255), "%2d 枚", cashData.second);
		moneyLine++;
	}

	//仕切り線
	DrawLine(0, money_size.y * (static_cast<int>(moneyType.size()) + 2), screen_size.x, money_size.y * (static_cast<int>(moneyType.size()) + 2), GetColor(255, 255, 255),true);
	DrawLine(money_size.x * 2, 0, money_size.x * 2, money_size.y * (static_cast<int>(moneyType.size()) + 2), GetColor(255, 255, 255), true);
	
	//チケット用描画
	LpTicketMachine.Draw();
}

bool Myself::SysInit(void)
{
	SetMainWindowText("1916229_課題1");
	SetGraphMode(screen_size.x, screen_size.y, 16);											// 画面モードの変更 800*600ドット、６５５３６色モードに設定
	ChangeWindowMode(true);		// ture:window false:フルスクリーン


	if (DxLib_Init() == -1)												// DXラリブラリの初期化
	{
		return false;
	}
	return true;
}

bool Myself::MyInit(void)
{

	//方法1：
	//	宣言の中身の作成を同時にする場合
	//	std::unique_ptr<MouseCtl> mouse(new MouseCtl());
	
	//方法2：
	//	.はポインターをアクセス
	//	－＞はメンバーをアクセス
	
	//	reset
	//　　	リソースの所有権を放棄し、新たなリソースの所有権を設定する。
	//mouse.reset();
	//mouse.reset(new MouseCtl());

	//方法3：
	//	make_unique
	//	unique_ptrオブジェクトを構築する。

	//	(1) : 非配列型Tのコンストラクタ引数を受け取り、unique_ptr<T>オブジェクトを構築する。
	//	(2) : 配列型Tの要素数を受け取り、unique_ptr<T>オブジェクトを構築する。
	//	(3) : (1)に配列型が指定された場合に、許可されていないオーバーロードとして宣言される。
	//mouse = std::make_unique<MouseCtl>();

	mouse = std::make_shared<MouseCtl>();

	if (!mouse)
	{
		return false;
	}

	// 保持する現金を追加-単位、データ
	cash.try_emplace(10, 15);
	cash.try_emplace(50, 3);
	cash.try_emplace(100, 2);
	cash.try_emplace(500, 1);
	cash.try_emplace(1000, 1);
	cash.try_emplace(5000, 1);
	cash.try_emplace(10000, 1);

	return true;
}

Myself::Myself() :
	screen_size{ 800,600 },
	money_size{100,50},
	font_size(18)
{
	//MyInit();
	mouse = nullptr;
	TRACE("Myselfを生成\n");

}

Myself::~Myself()
{
	//delete mouse;
	TRACE("Myselfを廃棄\n");

}
