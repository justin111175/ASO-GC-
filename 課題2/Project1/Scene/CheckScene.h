#pragma once
#include "BaseScene.h"


enum class CheckMean
{
    はい,
    いいえ
};
class CheckScene :
    public BaseScene
{

public:

    CheckScene(unique_Base own);
    ~CheckScene();

    unique_Base Update(unique_Base own) override;					// 更新
    void BaseDraw(void) override;					                // 更新
private:
    bool CheckCtl(conType input);
    unique_Base childScene_;
    int cnt_;
    CheckMean checkMeanID_;
    int key_;
};

