#pragma once
#include "BaseScene.h"
class CtlScene :
    public BaseScene
{
public:
	CtlScene();
	CtlScene(int no);
	~CtlScene();
	unique_Base Update(unique_Base own) override;					// �X�V
	void BaseDraw(void) override;					// �X�V


private:
	bool Ctl(conType input);
	int cnt_;
	void Init(void);

};

