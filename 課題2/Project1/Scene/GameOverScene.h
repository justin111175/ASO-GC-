#pragma once
#include "BaseScene.h"
#include <memory>
#include "../State.h"
class GameOverScene :
    public BaseScene
{
public:

    GameOverScene(unique_Base own, std::vector <std::unique_ptr<State>>& playerState);
    GameOverScene(unique_Base own, std::vector <std::unique_ptr<State>>& playerState,int no);
    GameOverScene(unique_Base own);
    ~GameOverScene();
    
    unique_Base Update(unique_Base own) override;					// �X�V
    void BaseDraw(void) override;					                // �X�V
private:
    std::vector <std::unique_ptr<State>> playerState_;
    void Init(void);
    int cnt_;
    int playerSize_;
    unique_Base childScene_;
    bool OverCtl(conType input);
};

