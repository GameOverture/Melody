#ifndef TwitchApi_h__
#define TwitchApi_h__

#include "pch.h"

//#include <curl/curl.h>

class TwitchApi : public HyEntity2d
{
public:
	TwitchApi(HyEntity2d *pParent = nullptr);
	virtual ~TwitchApi();

protected:
	virtual void OnUpdate() override;
};

#endif // TwitchApi_h__
