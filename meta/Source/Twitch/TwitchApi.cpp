#include "pch.h"
#include "TwitchApi.h"

TwitchApi::TwitchApi(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent)
{
	// Have local host tell us the query params
	//CURL *pCurl = curl_easy_init();
}

/*virtual*/ TwitchApi::~TwitchApi()
{
}

void TwitchApi::StartEventSub()
{
	
}

/*virtual*/ void TwitchApi::OnUpdate() /*override*/
{
}
