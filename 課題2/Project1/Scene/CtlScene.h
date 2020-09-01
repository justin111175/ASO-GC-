#pragma once
#include "BaseScene.h"
class CtlScene :
    public BaseScene
{
public:
	CtlScene();
	CtlScene(int no);
	~CtlScene();
	unique_Base Update(unique_Base own) override;					// 更新
	void BaseDraw(void) override;					// 更新


private:
	bool Ctl(conType input);
	int cnt_;
	void Init(void);

};

