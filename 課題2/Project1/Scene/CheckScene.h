#pragma once
#include "BaseScene.h"


enum class CheckMean
{
    �͂�,
    ������
};
class CheckScene :
    public BaseScene
{

public:

    CheckScene(unique_Base own);
    ~CheckScene();

    unique_Base Update(unique_Base own) override;					// �X�V
    void BaseDraw(void) override;					                // �X�V
private:
    bool CheckCtl(conType input);
    unique_Base childScene_;
    int cnt_;
    CheckMean checkMeanID_;
    int key_;
};

