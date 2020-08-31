#include "State.h"
#include "DxLib.h"
#include <utility>
#include "common/Input/KeyInput.h"
#include "common/Input/PadInput.h"
#include "common/_debug/_DeBugConOut.h"
#include "./Scene/SceneMng.h"
#include <functional>
#include <type_traits>
#include "EffectMng.h"
#include "common/ImageMng.h"

int State::_stateCount = 0;
State::State(Vector2&& _offset, Vector2&& _size):blockSize_(48),gridMax(8,14)
{
	_id = _stateCount;
	_stateCount++;
	
	tagetID = 0;
	dropSpeed_ = 1;

	winFlag_ = true;

	// move:
	//		�����ŗ^����ꂽ�I�u�W�F�N�g�̎����\�[�X�����ӂփI�u�W�F�N�g�Ɉړ�����@�\��񋟂��܂��B
	//		���L�����ړ����܂��Bstd::move�́AC++11�Œǉ�����܂����Bstd::move�́A�L���X�g�̂P�ł��B
	State::_offset =std::move(_offset);
	State::_size = std::move(_size);
	
	// �v���C��ID�ɂ���āAnext�Ղ�̈ʒu��ݒ�
	if (_id == 0)
	{
		auto pos = Vector2(_offset);
		nextPuyo_ = std::make_unique<NextPuyo>(pos, 3, 2,_id);
		name_ = "�v���C1";
	}
	else
	{
		auto pos = Vector2(_offset.x- blockSize_*9, _offset.y);
		nextPuyo_ = std::make_unique<NextPuyo>(pos, 3, 2,_id);
		name_ = "�v���C2";

	}

	// ������
	Init();
}

State::~State()
{
	_stateCount--;

}

int State::GetScreenId(void)
{
	return screenID;
}

void State::Draw(void)
{

	SetDrawScreen(screenID);
	
	ClsDrawScreen();
		
	ObjDraw();

	// �X�R�A�\������
	if (!_timeCount.GetFlag("�A��"))
	{
		// �A�����[�h����Ȃ����A�X�R�A��\������
		number_.Draw({ _offset.x+ blockSize_*7,_offset.y+ blockSize_*13+10 }, { 1.0f,1.0f }, scoreOld_);

	}
	else
	{
		// �A�����[�h�̎��A100���A������\������
		number_.Draw({ _offset.x + blockSize_ * 7 + 10,_offset.y + blockSize_ * 13 + 5 }, { 1.0f,1.0f }, rennsaCnt_);
		DrawRotaGraph3(_offset.x + blockSize_ * 5 + 10, _offset.y + blockSize_ * 13 + 5,0,0,1.0f,0.9f,0, IMAGE_ID("X")[0], true);
		number_.Draw({ _offset.x + blockSize_ * 3 + 10,_offset.y + blockSize_ * 13 + 5 }, { 1.0f,1.0f }, 100);
	}

}

// �Q�[�����
void State::ObjDraw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	DrawBox(_offset.x, _offset.y, _offset.x + (gridMax.x) * blockSize_, _offset.y + (gridMax.y) * blockSize_, _color2, true);
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	DrawBoxAA(_offset.x, _offset.y, _offset.x + (gridMax.x) * blockSize_, _offset.y + (gridMax.y) * blockSize_, 0xFFFFFF, false, 3.0f);

	DrawBox(_offset.x + blockSize_, _offset.y + blockSize_, _offset.x + (gridMax.x - 1) * blockSize_, _offset.y + (gridMax.y - 1) * blockSize_, _color, true);

	DrawBoxAA(_offset.x + blockSize_, _offset.y + blockSize_, _offset.x + (gridMax.x - 1) * blockSize_, _offset.y + (gridMax.y - 1) * blockSize_, 0xFFFFFF, false, 3.0f);

	SetFontSize(30);

	DrawString(_offset.x+10, _offset.y+5, name_, 0xFFFFFF);
	for (auto&& puyo : _puyo)
	{
		puyo->Draw(_offset);
		DrawString(_offset.x + _puyo[0]->Pos().x, _offset.y + _puyo[0]->Pos().y, "0", 0xFFFFFF, true);
		DrawString(_offset.x + _puyo[1]->Pos().x, _offset.y + _puyo[1]->Pos().y, "1", 0xFFFFFF, true);
	}
	//for (int x = 1; x < gridMax.x - 1; x++)
	//{
	//	for (int y = 1; y < gridMax.y - 1; y++)
	//	{
	//		if (_data[y][x] == PuyoID::��)
	//		{
	//			DrawBox(_offset.x + x * blockSize_, _offset.y + y * blockSize_,
	//				_offset.x + (1 + x) * blockSize_, _offset.y + (1 + y) * blockSize_, 0xFFFFFF, true);
	//		}
	//		if (_Eraserdata[y][x]!= PuyoID::NON)
	//		{
	//			DrawBox(_offset.x +x * blockSize_, _offset.y + y * blockSize_,
	//				_offset.x + (1 + x) * blockSize_, _offset.y + (1 + y) * blockSize_, 0xFF00FF, true);
	//		}
	//		if (_data[y][x] != PuyoID::NON)
	//		{
	//			DrawFormatString(_offset.x + x * blockSize_, _offset.y + (y)*blockSize_, 0xFFFFF, "%d\n", static_cast<int>(_data[y][x]));
	//		}
	//		if (_Puyodata[y][x] != PuyoID::NON)
	//		{
	//			DrawFormatString(_offset.x + x * blockSize_, _offset.y + (y)*blockSize_, 0xFFFFF, "%d\n", static_cast<int>(_Puyodata[y][x]));
	//		}
	//		DrawBox(_offset.x + x * blockSize_, _offset.y + y * blockSize_,
	//			_offset.x + (1 + x) * blockSize_, _offset.y + (1 + y) * blockSize_, 0xFFFFFF, false);
	//	}
	//}

	// next�Ղ�\������
	nextPuyo_->Draw( _color2);

	// ���ז��Ղ�\������
	for (auto&& ojyama : _ojyama)
	{
		ojyama->Draw(_offset);
	}

	SetFontSize(50);

	if (rennsaFlag_)
	{
		// �A���̎��A�A�����\������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _timeCount.GetCnt("�A��"));
		DrawFormatString(_offset.x + blockSize_ * 3, _offset.y + blockSize_ * 6, 0xFFFFFF, "%d�A��", rennsaCnt_);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}
}



void State::Run(void)
{
	// �Q�[���I����ĂȂ��Ƃ�
	if (!overFlag_)
	{		
		// �Ղ�̏������[�h�𗬂�
		puyoMode_[puyomode_]();
		// �A������1�ȏ�A�A�����[�h�i�\���p�j����Ȃ����A�A�������O�ɂ���
		if (rennsaCnt_ >1&&!_timeCount.GetFlag("�A��"))
		{
			rennsaCnt_ = 0;
		}
		// �A�����[�h�̎��A�A���v�Z�i�\���p�j���O�ȉ��̎��A�A�����[�h��false�ɂ���

		if (rennsaFlag_)
		{
			if (_timeCount.GetCnt("�A��") <= 0)
			{
				rennsaFlag_ = false;
			}
		}
		// �A�����[�h�i�\���p�j�̎�1�t���[���O�̃X�R�A���X�R�A
		if (_timeCount.GetFlag("�A��"))
		{
			scoreOld_ = score_;
		}
		else
		{
			// 1�t���[���O�̃X�R�A�ƃX�R�A��������Ȃ��ꍇ�A1�t���[���O�̃X�R�A���P����
			if (score_ != scoreOld_)
			{
				{
					scoreOld_++;

				}
			}
		}
	}
}

const int State::GetID(void)
{
	return _id;
}

void State::playerCtl(void)
{
	_pData._bit = { 1,1,1,1 };
	// �Ղ�̕����ϊ��֐�
	auto Rotemove = [&](sharePuyo& puyo0, sharePuyo& puyo1, InputID id) {
		auto pos = _puyo[tagetID]->GetGrid(blockSize_);
		int CntPos;
		id == InputID::Btn1 ? CntPos = blockSize_ : CntPos = -blockSize_;
		if (id == InputID::Btn1)
		{
			if (puyo0->Pos().y > puyo1->Pos().y)
			{
				if (_data[pos.y][pos.x -1] != PuyoID::NON)
				{
					return false;
				}
				//���ړ�
				puyo1->Pos({ puyo0->Pos().x- CntPos, puyo0->Pos().y  });
				return true;	
			}
			if (puyo0->Pos().x < puyo1->Pos().x)
			{
				//��ړ�
				puyo1->Pos({ puyo0->Pos().x, puyo0->Pos().y - CntPos });

				std::swap(puyo0, puyo1);
				tagetID = tagetID ^ 1;
				return true;
			}
			if (puyo0->Pos().x > puyo1->Pos().x)
			{
				//���ړ�

				puyo1->Pos({ puyo0->Pos().x, puyo0->Pos().y + CntPos });
				std::swap(puyo0, puyo1);
				tagetID = tagetID ^ 1;

			
				return true;
			}	
			if (puyo0->Pos().y < puyo1->Pos().y)
			{
				//�E�ړ�
				if (_data[pos.y][pos.x + 1] != PuyoID::NON)
				{
					return false;
				}
				puyo1->Pos({ puyo0->Pos().x + CntPos, puyo0->Pos().y });
				return true;
			}
		}
		else
		{
			if (puyo0->Pos().y > puyo1->Pos().y)
			{

				if (_data[pos.y][pos.x + 1] != PuyoID::NON)
				{
					return false;
				}
				//�E�ړ�
				puyo1->Pos({ puyo0->Pos().x - CntPos, puyo0->Pos().y });
				return true;
			}
			if (puyo0->Pos().x < puyo1->Pos().x)
			{
				//���ړ�
				puyo1->Pos({ puyo0->Pos().x, puyo0->Pos().y - CntPos });

				std::swap(puyo0, puyo1);
				tagetID = tagetID ^ 1;
				return true;
			}
			if (puyo0->Pos().x > puyo1->Pos().x)
			{
				//��ړ�
				puyo1->Pos({ puyo0->Pos().x, puyo0->Pos().y + CntPos });
				std::swap(puyo0, puyo1);
				tagetID = tagetID ^ 1;


				return true;
			}
			if (puyo0->Pos().y < puyo1->Pos().y)
			{
				//���ړ�
				if (_data[pos.y][pos.x - 1] != PuyoID::NON)
				{
					return false;
				}
				puyo1->Pos({ puyo0->Pos().x + CntPos, puyo0->Pos().y });
				return true;
			}
		}

	};
	
	// �L�[�{�[�h�ł���ꍇ
	(*controller[conType::Key])();
	for (auto data : controller[conType::Key]->GetCntData())
	{
		switch (data.first)
		{
		case InputID::Down:
			if (data.second[static_cast<int>(Trg::Now)])
			{
				for (int i = 0; i < 2; i++)
				{
					auto pos = _puyo[i]->GetGrid(blockSize_);

					if (_data[(_int64)pos.y + 1][pos.x] != PuyoID::NON)
					{
						_pData._bit.DOWN = 0;
						break;
					}
					_puyo[i]->SetPData(_pData._bit);
					_puyo[i]->Move(data.first);
				}
			}
			break;
		case InputID::�e�X�g�p:
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				InstanceOjyamapuyo();
			}
			break;
		default:
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				if ((data.first != InputID::Btn1) && (data.first != InputID::Btn2))
				{
					auto RLcheck = [&](int tagetID) {
						auto pos = _puyo[tagetID]->GetGrid(blockSize_);
						if (_data[pos.y][pos.x + 1] != PuyoID::NON)
						{
							_pData._bit.RIGHT = 0;
						}
						if (_data[pos.y][pos.x - 1] != PuyoID::NON)
						{
							_pData._bit.LEFT = 0;
						}
					};
					RLcheck(tagetID);
					RLcheck(tagetID ^ 1);
					for (int i = 0; i < 2; i++)
					{
						_puyo[i]->SetPData(_pData._bit);
						_puyo[i]->Move(data.first);
					}
				}
				else
				{

					Rotemove(_puyo[tagetID], _puyo[tagetID ^ 1], data.first);
				}
			}
			break;
		}

	}

	// �R���g���[���ł���ꍇ

	(*controller[conType::Pad])();
	for (auto data : controller[conType::Pad]->GetCntData(_id))
	{
		switch (data.first)
		{
		case InputID::Down:

			if (data.second[static_cast<int>(Trg::Now)])
			{
				for (int i = 0; i < 2; i++)
				{
					auto pos = _puyo[i]->GetGrid(blockSize_);

					if (_data[(_int64)pos.y + 1][pos.x] != PuyoID::NON)
					{
						_pData._bit.DOWN = 0;
						break;
					}
					_puyo[i]->SetPData(_pData._bit);
					_puyo[i]->Move(data.first);
				}
			}
			break;
		case InputID::Btn3:
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				puyomode_ = PuyoMode::���ڗ���;
				dropSpeed_ = 16;

			}
			break;
		default:
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				if ((data.first != InputID::Btn1) && (data.first != InputID::Btn2))
				{
					auto RLcheck = [&](int tagetID) {
						auto pos = _puyo[tagetID]->GetGrid(blockSize_);
						if (_data[pos.y][pos.x + 1] != PuyoID::NON)
						{
							_pData._bit.RIGHT = 0;
						}
						if (_data[pos.y][pos.x - 1] != PuyoID::NON)
						{
							_pData._bit.LEFT = 0;
						}
					};
					RLcheck(tagetID);
					RLcheck(tagetID ^ 1);
					for (int i = 0; i < 2; i++)
					{
						_puyo[i]->SetPData(_pData._bit);
						_puyo[i]->Move(data.first);
					}
				}
				else
				{
					Rotemove(_puyo[tagetID], _puyo[tagetID ^ 1], data.first);
				}			
			}
			break;
		}
	}
}

// �Ղ�̉��̃}�X�`�F�b�N
bool State::downCheck(sharePuyo& puyo)
{
	bool flag = true;

	// �Ղ�̃}�X�ʒu���Ƃ�
	auto pos = puyo->GetGrid(blockSize_);
	//�Ղ�̃}�X�ʒu�̉���NON����Ȃ��ꍇ
	if (_data[(__int64)pos.y + 1][pos.x] != PuyoID::NON)
	{
		//�Ղ�̃}�X�ʒu���ǂ���Ȃ��ꍇ�A�ʒu�C���A���ړ��t���O�A�R�}ID�ݒ�
		if(_data[(__int64)pos.y][pos.x] != PuyoID::��)
		{		
			puyo->Pos(pos * blockSize_);
			_pData._bit.DOWN = 0;
			_data[pos.y][pos.x] = puyo->ID();
		}
	}
	else
	{
		flag = false;
	}

	return flag;
	
}

// �C���X�^���X�Ղ�
bool State::InstancePuyo(void)
{
	auto pickupPuyo = nextPuyo_->pickUp();
	pickupPuyo.first->Pos(Vector2{ gridMax.x / 2 * blockSize_,blockSize_ });
	pickupPuyo.second->Pos(Vector2{ gridMax.x / 2 * blockSize_,blockSize_*1 });
	

	Vector2 pos_ = { gridMax.x / 2 * blockSize_,blockSize_ * 1 };
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), pickupPuyo.first->ID()));
	pos_ = { gridMax.x / 2 * blockSize_,blockSize_*2 };
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), pickupPuyo.second->ID()));


	return true;
}

// �C���X�^���X���ז��Ղ�
bool State::InstanceOjyamapuyo(void)
{
	Vector2 pos_ = { gridMax.x / 2 * blockSize_,blockSize_ };
	_ojyama.emplace(_ojyama.begin(), std::make_shared<Ojyama>(std::move(pos_), PuyoID::���ז��Ղ�));
	return true;

}

// �����Ղ�`�F�b�N
bool State::SetEraser(PuyoID id,Vector2 pos)
{
	memset(_EraserdataBase.data(), 0, _EraserdataBase.size() * sizeof(PuyoID));
	int count = 0;
	//�@�ċA�֐�
	std::function<void(PuyoID id, Vector2 vec)> DirCheck = [&](PuyoID id, Vector2 vec) {
		// �}�XID�Ǝw��Ղ�ID�Ɠ����ꍇ
		if (_data[vec.y][vec.x] == id)
		{
			// �����}�X��NON�̏ꍇ�A�}�X�J���h�{�P�A�l�����ċA
			if (_Eraserdata[vec.y][vec.x] == PuyoID::NON)
			{
				count++;
				if (_data[vec.y][vec.x + 1] == PuyoID::���ז��Ղ�)
				{
					_Eraserdata[vec.y][vec.x + 1] = _data[vec.y][vec.x + 1];

				}
				if (_data[vec.y][vec.x - 1] == PuyoID::���ז��Ղ�)
				{
					_Eraserdata[vec.y][vec.x - 1] = _data[vec.y][vec.x - 1];

				}
				if (_data[vec.y + 1][vec.x] == PuyoID::���ז��Ղ�)
				{
					_Eraserdata[vec.y + 1][vec.x] = _data[vec.y + 1][vec.x];

				}
				if (_data[vec.y - 1][vec.x] == PuyoID::���ז��Ղ�)
				{
					_Eraserdata[vec.y - 1][vec.x] = _data[vec.y - 1][vec.x];

				}

				_Eraserdata[vec.y][vec.x] = _data[vec.y][vec.x];

				DirCheck(id, { vec.x - 1,vec.y });
				DirCheck(id, { vec.x + 1,vec.y });
				DirCheck(id, { vec.x,vec.y - 1 });
				DirCheck(id, { vec.x,vec.y + 1 });
			}
		}

	};

	DirCheck(id, pos);

	if (count < 4)
	{
		// �}�X�J���h��4��菬�����ꍇ�A�����}�X�����Z�b�g
		memset(_EraserdataBase.data(), 0, _EraserdataBase.size() * sizeof(PuyoID));
		return false;

	}
	else
	{
		// �}�X�J���h��4�ȏ�ꍇ
		for (auto&& puyo : _puyo)
		{
			auto pos = puyo->GetGrid(blockSize_);
			// �}�X�f�[�^�Ə����}�X�f�[�^�Ɠ����ꍇ�A�Ղ�����A�}�X�ݒ�A�X�R�A�ݒ�
			if (_data[pos.y][pos.x] == _Eraserdata[pos.y][pos.x])
			{
				IpEffect.Play("puyo1", puyo->Pos(), _offset);
				_data[pos.y][pos.x] = PuyoID::NON;
				puyo->Alive(false);
				score_ = score_ + 10+(rennsaCnt_)*100;
			}
		}

		for (auto&& ojyama : _ojyama)
		{
			auto pos = ojyama->GetGrid(blockSize_);
			if (_data[pos.y][pos.x] == _Eraserdata[pos.y][pos.x])
			{
				_data[pos.y][pos.x] = PuyoID::NON;
				ojyama->Alive(false);
			}
		}
		return true;
	}
}

// �Ղ�񂷂邩�ǂ����`�F�b�N
bool State::PuyoCheck(Vector2 pos)
{
	int count = 0;
	// �ċA�֐�
	std::function<void(Vector2 pos)> PunyonCheck = [&](Vector2 pos) {
		// �w�肵���}�X���ǂ���Ȃ��ꍇ
		if (_data[pos.y][pos.x] != PuyoID::��)
		{
			// �w�肵���}�X��NON����Ȃ��ꍇ
			if (_data[pos.y][pos.x] != PuyoID::NON)
			{
				// �w�肵���Ղ悩��v�Z�n�߂܂��A���R�ڂ܂ŁA�Ղ��ݒ肷��
				if (count <= 3)
				{
					count++;
					_Puyodata[pos.y][pos.x] = _data[pos.y][pos.x];
					PunyonCheck({ pos.x,pos.y + 1 });
				}
			}
		}
	};

	PunyonCheck(pos);
	// �S����
	for (auto&& puyo : _puyo)
	{
		auto pos = puyo->GetGrid(blockSize_);
		// �Ղ��f�[�^��NON����Ȃ��ꍇ�A�Ղ��ݒ肷��
		if (_Puyodata[pos.y][pos.x] != PuyoID::NON)
		{

			puyo->puyoMode_ = PuyonState::puyo;
			puyo->puyoFlag_ = true;
		}
		else
		{
			// �~�܂�
			puyo->puyoMode_ = PuyonState::stay;
			puyo->puyoFlag_ = false;
		}
	}

	return true;

}

// �Ղ����
void State::delPuyo(void)
{
	// remove_if�F�����𖞂����v�f��������B
	// erase�F�w�肵���v�f���폜����B
	_puyo.erase(std::remove_if(
		_puyo.begin(),
		_puyo.end(),
		[](sharePuyo& obj) {return !(obj)->Alive(); }), _puyo.end());
	
	_ojyama.erase(std::remove_if(
		_ojyama.begin(),
		_ojyama.end(),
		[](sharedOjyama& obj) {return !(obj)->Alive(); }), _ojyama.end());

}

void State::Init(void)
{
	_color = 0x000033<<(16*_id);
	_color2 = 0x0000AA<<(16*_id);
	
	number_.Init();

	Vector2 pos_ = { gridMax.x / 2 * blockSize_,blockSize_ };
	int id = (rand() % (static_cast<int>(PuyoID::Max) - 3) + 1);
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));

	pos_ = { gridMax.x / 2 * blockSize_,blockSize_ * 2 };
	id = (rand() % (static_cast<int>(PuyoID::Max) - 3) + 1);
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));

	overFlag_ = false;

	controller.try_emplace(conType::Key, std::make_unique<KeyInput>());
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());

	controller[conType::Key]->SetUp(_id);
	controller[conType::Pad]->SetUp(_id);

	score_ = 0;
	rennsaCnt_ = 0;
	sceneFlag_ = false;
	rennsaFlag_ = false;

	_dataBase.resize((__int64)gridMax.x * gridMax.y);
	for (size_t no = 0; no < gridMax.y; no++)
	{
		_data.emplace_back(&_dataBase[no * gridMax.x]);
	}


	_EraserdataBase.resize((__int64)gridMax.x * gridMax.y);

	for (size_t no = 0; no < gridMax.y; no++)
	{
		_Eraserdata.emplace_back(&_EraserdataBase[no * gridMax.x]);
	}

	_PuyodataBase.resize((__int64)gridMax.x * gridMax.y);

	for (size_t no = 0; no < gridMax.y; no++)
	{
		_Puyodata.emplace_back(&_PuyodataBase[no * gridMax.x]);
	}

	for (int x = 0; x < gridMax.x; x++)
	{
		for (int y = 0; y < gridMax.y; y++)
		{
			_data[y][x] = PuyoID::NON;
			_data[0][x] = PuyoID::��;
			_data[(__int64)gridMax.y - 1][x] = PuyoID::��;
			_data[y][0] = PuyoID::��;
			_data[y][gridMax.x - 1] = PuyoID::��;
		}
	}


	screenID = MakeScreen(_size.x, _size.y, true);

	puyoMode_.try_emplace(PuyoMode::����, [&]() {
		// ����
		playerCtl();
		TRACE("����\n");

		// �Ղ�S����
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			// ������
			puyo->Run(dropSpeed_);
			{
				// �����邩�ǂ����`�F�b�N
				if (downCheck(puyo))
				{
					// �Ղ�̉��ɂ͂Ղ悠�邢�͕ǂ�����ꍇ�A�Ղ��̐ݒ�
					auto pos = puyo->GetGrid(blockSize_);
					_pData._bit = { 0,0,0,0 };
					puyo->SetPData(_pData._bit);
					// �������Ă����Ղ�
					if (!puyo->_pData._bit.DOWN && puyo->_pDataOld._bit.DOWN)
					{
						dropSpeed_ = 16;
						PuyoCheck(pos);
						puyomode_ = PuyoMode::�Ղ�;
					}
				}
			}
		});
	});

	puyoMode_.try_emplace(PuyoMode::���ڗ���, [&]() {
		TRACE("���ڗ���\n");

		// �Ղ�S����
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			// ������
			puyo->Run(dropSpeed_);
			{
				// �����邩�ǂ����`�F�b�N
				if (downCheck(puyo))
				{
					// �Ղ�̉��ɂ͂Ղ悠�邢�͕ǂ�����ꍇ�A�Ղ��̐ݒ�
					auto pos = puyo->GetGrid(blockSize_);
					_pData._bit = { 0,0,0,0 };
					puyo->SetPData(_pData._bit);
					// �������Ă����Ղ�
					if (!puyo->_pData._bit.DOWN && puyo->_pDataOld._bit.DOWN)
					{
						dropSpeed_ = 16;
						PuyoCheck(pos);
						puyomode_ = PuyoMode::�Ղ�;
					}
				}
			}
		});

		// �������Ă����Ղ悪�Ȃ��ꍇ�A�S���̂Ղ��������x�m�F�A���v�ł��B�Ȃ�Ղ�񃂁[�h�ɍs��
		bool Flag = true;
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {

			return Flag &= downCheck(puyo);
		});

		if (Flag)
		{
			puyomode_ = PuyoMode::�Ղ�;
		}
	});

	puyoMode_.try_emplace(PuyoMode::�A��, [&]() {
		TRACE("�A��\n");
		// ���ז��Ղ�̉��`�F�b�N�̃����_���֐�
		auto Check = [&](sharedOjyama& ojyama) {
			auto pos = ojyama->GetGrid(blockSize_);
			if (_data[(__int64)pos.y + 1][pos.x] != PuyoID::NON)
			{
				ojyama->Pos(pos * blockSize_);
				_pData._bit.DOWN = 0;
				_data[pos.y][pos.x] = PuyoID::���ז��Ղ�;
			}
			else
			{
				return false;
			}
		};
		// �A���̎��A������Ƒ҂�
		if (_timeCount.GetFlag("�A������"))
		{
			return;
		}

		bool rennsaFlag = true;

		//std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

		//	return rennsaFlag &= Check(ojyama);
		//});

		//std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

		//	if (!ojyama->Run(dropSpeed_))
		//	{
		//		rennsaFlag = false;
		//	}
		//});

		// �S���̂Ղ扺�`�F�b�N��true�Ȃ�A�������[�h�ɍs���A���false�Ȃ�A�܂����ڗ������[�h�ɍs��
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			return rennsaFlag &= downCheck(puyo);

		});

		if (rennsaFlag)
		{
			puyomode_ = PuyoMode::����;
		}
		else
		{
			puyomode_ = PuyoMode::���ڗ���;
		}
	
	});


	puyoMode_.try_emplace(PuyoMode::�Ղ�, [&]() {
		TRACE("�Ղ�\n");

		// �Ղ�񂪏I��邩�ǂ����̊m�F
		std::function<bool (sharePuyo& puyo)> PuyonState = [&](sharePuyo& puyo) {
			auto pos = puyo->GetGrid(blockSize_);
			// �Ղ�̉����ǂ���Ȃ��ꍇ
			if (_data[(__int64)pos.y + 1][pos.x] != PuyoID::��)
			{
				// �Ղ�̏�Ԃ��Ղ��Ȃ�i��~��Ԃ���Ȃ��ꍇ�j
				if (puyo->puyoFlag_)
				{
					// �Ղ��J�n
					if (puyo->puyo())
					{
						// �Ղ��I�����
						return true;
					}
				}
				else
				{
					// �Ղ�͒�~��Ԃ̏ꍇ�j
					return true;
				}
			}
			else
			{
				// �Ղ�̉����ǂ̏ꍇ
				return true;
			}
			// �������Ȃ�
			return  false;
		};

		bool Flag = true;
		bool drop = false;
		// �Ղ�S����
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			// ��ڂ̂Ղ悪�܂��������Ă���ꍇ
			if (!downCheck(puyo))
			{
				puyo->Run(dropSpeed_);
				drop = true;
			}
			else
			{
				// �Ղ��`�F�b�N
				auto pos = puyo->GetGrid(blockSize_);
				_pData._bit = { 0,0,0,0 };
				puyo->SetPData(_pData._bit);
				if (!puyo->_pData._bit.DOWN && puyo->_pDataOld._bit.DOWN)
				{
					PuyoCheck(pos);
				}
			}

			return Flag &= PuyonState(puyo);
		});

		// �Ղ�񂪏I���ꍇ
		if (Flag)
		{
			// �Ղ��p�`�F�b�N�ۑ������Z�b�g����
			memset(_PuyodataBase.data(), 0, _PuyodataBase.size() * sizeof(PuyoID));
			// �����S���������āA�Ղ������������A�ނɂ�񃂁[�h�ɍs��
			if(!drop)
			{
				puyomode_ = PuyoMode::�ނɂ��;
			}
		}
	});

	puyoMode_.try_emplace(PuyoMode::�ނɂ��, [&]() {
		TRACE("�ނɂ��\n");

		// �w������̃}�X��ID���`�F�b�N
		auto dirCheck = [&](PuyoID id, Vector2 vec) {

			return (id == _data[vec.y][vec.x]);
		};
		// �w��Ղ�̎l�����`�F�b�N�Atrue�̏ꍇ�ނɂ���\������
		auto check = [&](sharePuyo& puyo) {
			_drawData._bit = { 0,0,0,0 };
			auto Pos = puyo->GetGrid(blockSize_);
			_drawData._bit.RIGHT = dirCheck(puyo->ID(), { Pos.x + 1,Pos.y });
			_drawData._bit.LEFT = dirCheck(puyo->ID(), { Pos.x - 1,Pos.y });
			_drawData._bit.UP = dirCheck(puyo->ID(), { Pos.x ,Pos.y - 1 });
			_drawData._bit.DOWN = dirCheck(puyo->ID(), { Pos.x ,Pos.y + 1 });
			puyo->SetDrawData(_drawData._bit);
			return true;
		};
		bool flag = true;
		// �S���̂Ղ���񂵂āA�Ղ�ʒu���C�����āA�l�������`�F�b�N
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			puyo->Cnt(0);
			auto Pos = puyo->GetGrid(blockSize_);
			puyo->Pos(Pos* blockSize_);
			return flag &= check(puyo);

		});
		// �����S���`�F�b�N�I�������A�A�����[�h�ɖ߂�
		if (flag)
		{
			puyomode_ = PuyoMode::�A��;
		}
	});
	puyoMode_.try_emplace(PuyoMode::����, [&]() {
		TRACE("����\n");

		bool Flag = true;
		// �Ղ�S����
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			auto pos = puyo->GetGrid(blockSize_);
			auto id = puyo->ID();
			// �����Ղ�m�F
			if (SetEraser(id, pos))
			{
				// true�̏ꍇ�Ղ悪����}�X��NON�ɂ���A�����āA�R���g���[����U������
				std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {

					auto pos = puyo->GetGrid(blockSize_);

					_data[pos.y][pos.x] = PuyoID::NON;

					StartJoypadVibration(DX_INPUT_PAD1, 1000, 200);


				});

				std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

					auto pos = ojyama->GetGrid(blockSize_);
					_data[pos.y][pos.x] = PuyoID::NON;
					StartJoypadVibration(DX_INPUT_PAD1, 1000, 200);
				});
			}
		});

		// ���񂾂Ղ��list�������
		delPuyo();

		// ��������ɁA������x�m�F
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {

			return Flag &= downCheck(puyo);

		});
		
		// �Ղ�S�����~�܂�ꍇ
		if (Flag)
		{
			puyomode_ = PuyoMode::���ז��Ղ旎��;
		}
		else
		{
			// �܂�������ꍇ�A�A�����{�P�A�A���J�n���āA���ڗ������[�h�ɖ߂�
			rennsaCnt_++;
			if ((rennsaCnt_ > 1))
			{
				_timeCount.Set("�A��", true, 5);
				rennsaFlag_ = true;

			}
			_timeCount.Set("�A������", true, 2);
			puyomode_ = PuyoMode::���ڗ���;
		}

	
	
	});
	puyoMode_.try_emplace(PuyoMode::���ז��Ղ旎��, [&]() {


		auto Check = [&](sharedOjyama& ojyama) {
				
			auto pos = ojyama->GetGrid(blockSize_);
			if (_data[(__int64)pos.y + 1][pos.x] != PuyoID::NON)
			{			
				ojyama->Pos(pos* blockSize_);

				_pData._bit.DOWN = 0;

				_data[pos.y][pos.x] = PuyoID::���ז��Ղ�;

			}
			else
			{
				return false;
			}
		};


		bool rennsaFlag = true;

		std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

			return rennsaFlag &= Check(ojyama);
		});

		std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

			if (!ojyama->Run(8))
			{
				rennsaFlag = false;
			}




		});

		std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

			return rennsaFlag &= Check(ojyama);
		});


		if (rennsaFlag)
		{

			puyomode_ = PuyoMode::�I�[�o�[�`�F�b�N;

		}





	});

	puyoMode_.try_emplace(PuyoMode::�I�[�o�[�`�F�b�N, [&]() {
		TRACE("�I�[�o�[�`�F�b�N\n");

		bool Flag = false;

		// �S���Ղ���m�F�i�ォ��j�A������s�ڂɂՂ悪����ꍇ���[�v���~�܂��āAFlag��false�ɂ���
		std::for_each(_puyo.begin(), _puyo.end(), [&](sharePuyo& puyo)
		{
			//puyo->puyoMode_ = PuyonState::stay;
			auto pos = puyo->GetGrid(blockSize_);
			if (pos.y <= 1)
			{
				return Flag = true;
			}
		});

		// ����flase�̏ꍇ�A�V�����Ղ������A�A�����O�ɂ��āA�������[�h�ɖ߂��āA�Q�[��������
		if (!Flag)
		{
			InstancePuyo();
			if (rennsaCnt_ > 0 && !_timeCount.GetFlag("�A��"))
			{
				rennsaCnt_ = 0;
			}
			dropSpeed_ = 1;

			puyomode_ = PuyoMode::����;

		}
		else
		{
			//�Q�[���I�[�o�[
			winFlag_ = false;
			overFlag_ = true;
		}
	});
}
