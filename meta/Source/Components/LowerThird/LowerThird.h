#ifndef LowerThird_h__
#define LowerThird_h__

#include "pch.h"

class LowerThird : public HyEntity2d
{
public:
	LowerThird(HyEntity2d *pParent = nullptr);
	virtual ~LowerThird();

protected:
	virtual void OnUpdate() override;
};

#endif // LowerThird_h__
