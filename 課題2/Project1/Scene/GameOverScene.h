#pragma once
#include "BaseScene.h"
#include <memory>
#include "../State.h"
class GameOverScene :
    public BaseScene
{
public:

    GameOverScene(unique_Base own, int no);
    ~GameOverScene();
    
    unique_Base Update(unique_Base own) override;					// �X�V
    void BaseDraw(void) override;					                // �X�V
private:
    int cnt_;
    int playerSize_;
    unique_Base childScene_;
};

