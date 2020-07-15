#include "TicketMachine.h"
#include "Myself.h"
#include "_DebugConOut.h"
#include <Dxlib.h>
#include <algorithm>
#include "TimeCount.h"

//静的シングルトン
//TicketMachine::s_Instance;

void TicketMachine::Run(void)
{
	Vector2 pos = mouse->GetPos();

	//ラムダ式
	// ボタンの当たり判定
	auto CheckBtn = [&](Vector2 btnPos) {
		if (((pos.x >= btnPos.x) && (pos.x < btnPos.x + pay_btn_size.x)) &&
			((pos.y >= btnPos.y) && (pos.y < btnPos.y + pay_btn_size.y)))
		{
			return true;
		}
		return false;
	};

	// ボタンの表示変換
	auto BtnKey = [&](Vector2 btnPos,std::string& btnKey) {
		if (mouse->GetClickTrg())
		{
			if (mouse->GetClicking() &&(CheckBtn(btnPos)))
			{
				btnKey = "btn_push";
			}
			else
			{
				btnKey = "btn";
			}
		}
	};

	BtnKey(btnPos["ボタン1"], btnKey["ボタン1"]);
	BtnKey(btnPos["ボタン2"], btnKey["ボタン2"]);

	if (mouse->GetClickTrg() && CheckBtn(btnPos["ボタン2"]))
	{
		// 支払いはまだ成功してない場合、キャンセルする
		if (!paySuccess)
		{
			Cancel();
		}
		else
		{
			PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);
			_timeCount.Set("エラー", true, 2);
			_bug = "おつりとってません\n";

			TRACE("おつりとってません\n");
		}

	}

	if (mouse->GetClickTrg() && CheckBtn(btnPos["ボタン1"]))
	{
		// 支払いは成功した場合、キャンセルする
		if (paySuccess)
		{
			// 決済完了
			if(_myself.MergeCash(cashData[payType].second))
			{
				PlaySoundMem(_sound.soundGet("ボタン"), DX_PLAYTYPE_BACK);
				Clear();
			}
		}
		else
		{
			// 支払い方法見つけたら、支払い関数へ行く
			if (pay.count(payType))
			{
				pay[payType]();
			}
			else
			{
				TRACE("未知な支払い方\n");
				_timeCount.Set("エラー", true, 2);
				_bug = "未知な支払い方\n";
				PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);

			}
		}
	}
}

bool TicketMachine::Cancel(void)
{
	// 支払い方法見つけたら、キャンセル関数へ行く
	if (cancel.count(payType))
	{
		cancel[payType]();
		return true;
	}
	return false;
}

void TicketMachine::Draw(void)
{		

	// 切符の値段
	DrawString(screen_size.x / 2 - font_size, money_size.y / 2, "切符の価額　現金：130円　電子マネー:124円", GetColor(255, 255, 255));

	// 支払描画	
	// 方法１：
	if (draw.count(payType))
	{
		draw[payType]();
	}
	// 方法２：
	//if (draw.find(payType) != draw.end())
	//{
	//	draw[payType]();
	//}

	// ボタン描画
	DrawBtn();
	// バッグ描画
	BugDraw();

}

void TicketMachine::BugDraw(void)
{
	// エラー計算フラグがtrueの時、表示する
	if (_timeCount.GetFlag("エラー"))
	{
		DrawBox(200, 0, 800, 50*(static_cast<int>(moneyType.size())+2), GetColor(0, 0, 0), true);
		DrawBox(200, 0, 800, 50*(static_cast<int>(moneyType.size()) +2), GetColor(255, 255, 255), false);
		DrawString(200, 0, "エラーです", 0xFFFFFF, true);
		SetFontSize(20);
		DrawString(250, 100, _bug, 0xFFFFFF, true);
		DrawGraph(600, 200, image["エラー"], true);
	}
}

VecInt& TicketMachine::GetMoneyType(void)
{
	return moneyType;
}

bool TicketMachine::InitDraw(void)
{
	int moneyLine = 0;
	int totalMoney = 0;

	SetFontSize(font_size);

	// MAX
	draw.try_emplace(PayType::MAX, [&]() {
		int moneyLine = 0;
		int totalMoney = 0;

		SetFontSize(font_size);

		DrawGraph(0, 0, image["money"], true);

		DrawString(0,
			comment_offsetY + GetFontSize() / 2,
			"左の枠内の現金かICカードを選択してクリックして入金してください。\n入金が完了したら決済ボタンを押してください。",
			0xFFFFFF);
		
		//TRACE("FunctionのDraw::MAX\n");
	});
		
	//カード
	draw.try_emplace(PayType::CARD, [&]() {
		int moneyLine = 0;
		int totalMoney = 0;

		SetFontSize(font_size);

		DrawGraph(0, 0, image["act_cash"], true);

		if (paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2,
				"決済完了\nICカードを出す際は受け取りボタンを押してください。", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "電子マネー", 0xFFFFFF);
			DrawFormatString(draw_offset.x + GetFontSize(), draw_offset.y + GetFontSize(),
				0xFFFFFF, "  残高　%d円", cardData.first);
			DrawFormatString(draw_offset.x + GetFontSize(), draw_offset.y + GetFontSize() * 2,
				0xFFFFFF, "引く額　%d円", cardData.second);
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2,
				"左の枠内のICカードを選択してクリック入金してください。\n入金完了したら決済ボタンを押してください。", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "電子マネー", 0xFFFFFF);

			DrawFormatString(draw_offset.x + GetFontSize(), draw_offset.y + GetFontSize(),
				0xFFFFFF, "  残高　%d円", cardData.first);
			if (cardData.first < price_card)
			{
				DrawString(draw_offset.x, draw_offset.y + GetFontSize() * 3, "残高が足りません", 0xFF0000, true);
			}

		}
		//TRACE("FunctionのDraw::Card\n");
	});
	
	//キャシュー
	draw.try_emplace(PayType::CASH, [&]() {
		int moneyLine = 0;
		int totalMoney = 0;

		SetFontSize(font_size);

		DrawGraph(0, 0, image["act_cash"], true);

		if (paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2,
				"決済完了\nおつりを受け取る際は受け取りボタンを押してください。", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "投入金額", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "              枚数", 0xFFFFFF);
			int moneyLine = 0;
			int totalMoney = 0;
			for (auto cashData : cashData[payType].first)
			{
				DrawFormatString(draw_offset.x + GetFontSize(), (draw_offset.y + GetFontSize()) + moneyLine * GetFontSize(), 0xFFFFFF, "%d円", cashData.first);
				DrawFormatString(draw_offset.x + GetFontSize(), (draw_offset.y + GetFontSize()) + moneyLine * GetFontSize(), 0xFFFFFF, "　　　　 %5d枚", cashData.second);
				moneyLine++;
				totalMoney += (cashData.first * cashData.second);
			}
			DrawFormatString(draw_offset.x, (draw_offset.y + GetFontSize() * 2) + moneyLine * GetFontSize(), 0xFFFFFF, "合計投入額　%d円", totalMoney);


			//
			DrawString(draw_offset.x * 2, draw_offset.y, "   釣銭", 0xFFFFFF);
			DrawString(draw_offset.x * 2, draw_offset.y, "             枚数", 0xFFFFFF);
			int changeLine = 0;

			for (auto cashData : cashData[payType].second)
			{
				DrawFormatString(draw_offset.x * 2, (draw_offset.y + GetFontSize()) + changeLine * GetFontSize(), 0xFFFFFF, "%5d円", cashData.first);
				DrawFormatString(draw_offset.x * 2, (draw_offset.y + GetFontSize()) + changeLine * GetFontSize(), 0xFFFFFF, "%15d枚", cashData.second);
				changeLine++;

			}
		}
		else
		{
			DrawString(0,
				comment_offsetY + GetFontSize() / 2,
				"左の枠内の現金を選択してクリックして入金してください。\n入金が完了したら決済ボタンを押してください。",
				0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "投入金額", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "           枚数", 0xFFFFFF);


			for (auto moneyData : cashData[payType].first)
			{
				DrawFormatString(draw_offset.x + GetFontSize(), (draw_offset.y + GetFontSize()) + moneyLine * GetFontSize(),
					0xFFFFFF, "%d円", moneyData.first);

				DrawFormatString(draw_offset.x + GetFontSize(), (draw_offset.y + GetFontSize()) + moneyLine * GetFontSize(),
					0xFFFFFF, "　　　　　%d枚", moneyData.second);
				moneyLine++;
				totalMoney += moneyData.first * moneyData.second;

			}

			DrawFormatString(draw_offset.x, (draw_offset.y + GetFontSize() * 2) + moneyLine * GetFontSize(),
				0xFFFFFF, "合計投入　%3d円", totalMoney);


			if (totalMoney < price_card)
			{
				DrawString(draw_offset.x, (draw_offset.y + GetFontSize() * 3) + moneyLine * GetFontSize(),
					"金額が足りません", 0xFF0000);
			}
		}
		//TRACE("FunctionのDraw::Cash\n");
	});
	return true;
}

bool TicketMachine::InitPay(void)
{
	// キャシュー
	pay.try_emplace(PayType::CASH, [&]() {
		if (PayCash())
		{
			//決済成功
			TRACE("決済成功\n");
		}
	});
	
	// カード
	pay.try_emplace(PayType::CARD, [&]() {
		if (PayCard())
		{
			//決済成功
			TRACE("決済成功\n");
		}
	});

	// MAX
	pay.try_emplace(PayType::MAX, [&]() {
		PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);
		_timeCount.Set("エラー", true, 2);
		_bug = "入金してない状況で、決済ボタン\n";
		TRACE("入金してない状況で、決済ボタン")
	});

	return true;
}

bool TicketMachine::InitCancel(void)
{
	// キャシュー
	cancel.try_emplace(PayType::CASH, [&]() {
		if (_myself.MergeCash(cashData[payType].first))
		{
			PlaySoundMem(_sound.soundGet("ボタン"), DX_PLAYTYPE_BACK);
			TRACE("お金を返す\n");
			Clear();
		}
	});

	// カード
	cancel.try_emplace(PayType::CARD, [&]() {
		PlaySoundMem(_sound.soundGet("ボタン"), DX_PLAYTYPE_BACK);
		Clear();
		TRACE("カードキャンセル\n");
	});	
	
	// MAX
	cancel.try_emplace(PayType::MAX, [&]() {
		if (cashData[payType].first.size() == 0)
		{
			PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);
			_timeCount.Set("エラー", true, 2);
			_bug = "入金してない状況で、キャンセルする\n";
			TRACE("入金してない状況で、キャンセルする\n");
		}
	});

	return true;
}

bool TicketMachine::InitInsert(void)
{
	//方法2：
	insert.try_emplace(PayType::CASH, [&](int type) {
		// カードで払うが、現金を押す場合
		if (payType == PayType::CARD)
		{
			PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);
			_timeCount.Set("エラー", true, 2);
			LpTicketMachine._bug = "現金で払いたい場合は、キャンセルしてください。\n";
			return false;
		}

		if (payType == PayType::MAX)
		{
			payType = PayType::CASH;
		}

		cashData[payType].first.try_emplace(type, 0);
		cashData[payType].first[type]++;
		return true;
	});
	
	insert.try_emplace(PayType::CARD, [&](int) {
		// 現金で払うが、カードを押す場合
		if (payType == PayType::CASH)
		{
			PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);
			_timeCount.Set("エラー", true, 2);
			LpTicketMachine._bug = "カードで払いたい場合は、キャンセルしてください。\n";
			return false;
		}

		if (payType == PayType::MAX)
		{
			payType = PayType::CARD;
		}
		cardData = LpCardServer.GetCardState();
		return true;

	});

	return true;
}

bool TicketMachine::PayCash(void)
{
	int totalCash = 0;
	auto tmp = cashData[payType].first;
	//投入金額合計
	// 方法1
	for (auto Data : tmp)
	{
		totalCash += (Data.first * Data.second);
	}

	// 方法2
	//int totalCash = 0;
	//auto tmpCashData = cashData;
	//for (int j = 0; j < moneyType.size(); j++)
	//{
	//	try
	//	{
	//		totalCash += tmpCashData[moneyType[j]] * moneyType[j];
	//	}
	//	catch (...)
	//	{
	//		//何もない
	//	}
	//}

	// 方法3
	//int totalCash = 0;
	//auto tmpCashData = cashData;
	//for (int j = 0; j < moneyType.size(); j++)
	//{
	//	if (tmpCashData.count(moneyType[j]))
	//	{
	//		totalCash += tmpCashData[moneyType[j]] * moneyType[j];
	//	}
	//}

	if (totalCash < price_cash)
	{
		//投入金額不足
		PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);
		_timeCount.Set("エラー", true, 2);
		_bug = "投入金額不足\n";
		return false;
	}
	
	// 方法1
	int credit = price_cash;
	for (auto& Data : tmp)
	{
		while (Data.second)
		{
			credit -= Data.first;
			Data.second--;
			TRACE("使ったお金:%d\n", Data.first);
			if (credit <= 0)
			{
				break;
			}
		}
		
		if (credit <= 0)
		{
			//支払いが終わったので、おつりの処理
			cashData[payType].second = tmp;
			int change = -credit;
			//　方法1：
			//for (int j = moneyType.size() - 1; j >= 0; j--)
			//{
			//	while (change>=moneyType[j])
			//	{
			//		data[payType].second.try_emplace(moneyType[j], 0);
			//		data[payType].second[moneyType[j]]++;
			//		change -= moneyType[j];
			//	}
			//	if (change < 0)
			//	{
			//		TRACE("おつり異常\n");
			//		return false;
			//	}
			//	if (change == 0)
			//	{
			//		paySuccess = true;
			//		return true;
			//	}
			//}
			
			//方法2：
			//  crbegin:
			//				コンテナ内の最後の要素を指す逆イテレータを返す
			std::for_each(moneyType.crbegin(), moneyType.crend(), [&](int type) {
				while (change >= type)
				{
					cashData[payType].second.try_emplace(type, 0);
					cashData[payType].second[type]++;
					change -= type;
				}
			});
			if (change < 0)
			{
				TRACE("おつり異常\n");
				PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);
				_timeCount.Set("エラー", true, 2);
				_bug = "おつり異常\n";
				return false;
			}
			if (change == 0)
			{
				TRACE("おつり設定完了\n")
				PlaySoundMem(_sound.soundGet("ボタン"), DX_PLAYTYPE_BACK);

				paySuccess = true;
				return true;
			}
			break;
		}
	}
	// 方法2
	//int credit = price_cash;
	//for (int j = 0; j < moneyType.size(); j++)
	//{
	//	if (cashData.count(moneyType[j]))
	//	{
	//		while (cashData[moneyType[j]] > 0)
	//		{
	//			credit -= moneyType[j];
	//			cashData[moneyType[j]]--;
	//			
	//			TRACE("使ったお金:%d\n", moneyType[j]);
	//			if (credit <= 0)
	//			{
	//				break;
	//			}
	//		}
	//	}
	//}
	paySuccess = false;

	return false;
}

bool TicketMachine::PayCard(void)
{
	if (LpCardServer.Payment(price_card))
	{
		PlaySoundMem(_sound.soundGet("ボタン"), DX_PLAYTYPE_BACK);
		paySuccess = true;
		return true;
	}
	PlaySoundMem(_sound.soundGet("エラー"), DX_PLAYTYPE_BACK);
	_timeCount.Set("エラー", true, 2);
	_bug = "カード失敗\n";
	return false;
}

void TicketMachine::Clear(void)
{
	btnKey["ボタン1"] = "btn";
	btnKey["ボタン2"] = "btn";

	paySuccess = false;

	payType = PayType::MAX;
	cashData.clear();
}

void TicketMachine::DrawBtn(void)
{
	SetFontSize(font_size * 2);

	std::string btnName = (paySuccess == false ? "  決　済" : " 受け取り");


	DrawGraph(btnPos["ボタン1"].x, btnPos["ボタン1"].y, image[btnKey["ボタン1"]], true);
	DrawString(btnPos["ボタン1"].x+ font_size / 2, btnPos["ボタン1"].y + font_size / 2, btnName.c_str(), 0x000000);

	DrawGraph(btnPos["ボタン2"].x, btnPos["ボタン2"].y, image[btnKey["ボタン2"]], true);
	DrawString(btnPos["ボタン2"].x + font_size / 2, btnPos["ボタン2"].y + font_size / 2, "キャンセル", 0x000000);
}

bool TicketMachine::Init(sharedMouse mouse)
{
	TicketMachine::mouse = mouse;
	//UI配置されているお金を追加
	if (moneyType.size()==0)
	{
		moneyType.emplace_back(10);
		moneyType.emplace_back(50);
		moneyType.emplace_back(100);
		moneyType.emplace_back(500);
		moneyType.emplace_back(1000);
		moneyType.emplace_back(5000);
		moneyType.emplace_back(10000);
	}

	//moneyType.resize(7);
	//moneyType[0] = 10;
	//moneyType[1] = 50;
	//moneyType[2] = 100;
	//moneyType[3] = 500;
	//moneyType[4] = 1000;
	//moneyType[5] = 5000;
	//moneyType[6] = 10000;
	
	//画像の読み込み
	image.try_emplace("money", LoadGraph("image/money.png"));
	image.try_emplace("act_cash", LoadGraph("image/active_cash.png"));
	image.try_emplace("act_card", LoadGraph("image/active_card.png"));

	image.try_emplace("btn", LoadGraph("image/btn.png"));
	image.try_emplace("btn_push", LoadGraph("image/btn_push.png"));
	
	image.try_emplace("エラー", LoadGraph("image/bug.png"));

	btnPos.try_emplace("ボタン1", (screen_size.x - money_size.x * 3-20) - pay_btn_size.x, money_size.y * static_cast<int>(moneyType.size()) );
	btnPos.try_emplace("ボタン2", (screen_size.x - money_size.x +20) - pay_btn_size.x, money_size.y * static_cast<int>(moneyType.size()) );
	
	btnKey.try_emplace("ボタン1", "btn");
	btnKey.try_emplace("ボタン2", "btn");


	InitDraw();
	InitPay();
	InitCancel();
	InitInsert();
	return true;
}

//方法2：
//bool TicketMachine::InsertCash(int cash)
//{
//	if (payType == PayType::MAX)
//	{
//		payType = PayType::CASH;
//	}
//
//
//	if (payType != PayType::CASH)
//	{
//		return false;
//	}
//
//	//try
//	//{
//	//	(cashData.at(cash))++;
//	//}
//	//catch (...)
//	//{
//	//	cashData.emplace(cash, 1);
//	//}
//
//	data[payType].first.try_emplace(cash, 0);
//	data[payType].first[cash]++;
//
//	return true;
//}
//
//bool TicketMachine::InsertCard(void)
//{
//	if (payType == PayType::MAX)
//	{
//		payType = PayType::CARD;
//	}
//	else
//	{
//		//カードおよび現金が未投入の場合のみ受け付けるので、それ以外は処理しない
//		return false;
//	}
//
//	cardData = LpCardServer.GetCardState();
//	return true;
//}

const bool TicketMachine::paySuccessGet(void) const
{
	return paySuccess;
}

PayType& TicketMachine::GetPayType(void)
{
	return payType;
}

DataInt& TicketMachine::GetData(void)
{
	return cashData;
}

SetPay& TicketMachine::GetInsert(void)
{
	return insert;
}


TicketMachine::TicketMachine():
	screen_size{ 800,600 },
	money_size{ 100,50 },
	font_size(18),
	pay_btn_size{200,50},
	comment_offsetY(450),
	draw_offset{200,70},
	price_cash(130),
	price_card(124)
{
	Clear();
	TRACE("TicketMachineを生成\n");
}

TicketMachine::~TicketMachine()
{
	TRACE("TicketMachineを廃棄\n");
}
