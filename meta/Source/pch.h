#ifndef pch_h__
#define pch_h__

#include "HyEngine.h"

#define DISPLAYORDER_DEBUG			9999999
#define DISPLAYORDER_GameBrowser	999999
#define DISPLAYORDER_MessageCycle	99999
#define DISPLAYORDER_AboveMonitor	9999
#define DISPLAYORDER_Monitor		9000
#define DISPLAYORDER_LiveSplitMask	999
#define DISPLAYORDER_Docket			899
#define DISPLAYORDER_NowPlaying		799
#define DISPLAYORDER_CrtVolume		99
#define DISPLAYORDER_Buttons		99
#define DISPLAYORDER_BallTop		85
#define DISPLAYORDER_GatePath		80
#define DISPLAYORDER_Gate			70

#define DISPLAYORDER_CrtStatic		60

enum CameraTag
{
	CAMTAG_Center = 0,
	CAMTAG_Divider,
	CAMTAG_Game
};

#define LIVESPLIT_WIDTH 544
#define LIVESPLIT_HEIGHT 768

#define GAMEBROWSER_WIDTH (1920 - LIVESPLIT_WIDTH)
#define GAMEBROWSER_HEIGHT 1080
#define GAMEBROWSER_MARGINS HyMargins<int16_t>(64, 150, 64, 42 + 25)
#define GAMEBROWSER_LAYOUT_WIDTH (GAMEBROWSER_WIDTH - GAMEBROWSER_MARGINS.left - GAMEBROWSER_MARGINS.right)
#define GAMEBROWSER_LAYOUT_HEIGHT (GAMEBROWSER_HEIGHT - GAMEBROWSER_MARGINS.top - GAMEBROWSER_MARGINS.bottom)

#define MISC_WIDTH 544
#define MISC_HEIGHT 312

#define DIVIDER_WIDTH 4

#define RETRO_SIDES 274
#define CRT_SCREEN_WIDTH 855
#define CRT_SCREEN_HEIGHT 601

#define LOWERTHIRD_HEIGHT 172

#define CAMERA_CENTER_POS 0.0f, 0.0f
#define CAMERA_CENTER_SCALE 1.0f, 1.0f
#define CAMERA_DIVIDER_POS -RETRO_SIDES, 0
#define CAMERA_DIVIDER_SCALE 1.0f, 1.0f
#define CAMERA_GAME_POS -293.0f, -13.0f
#define CAMERA_GAME_SCALE 0.67f, 0.67f

#define MESSAGECYCLE_POS_X 240.0f
#define MESSAGECYCLE_GAMEPOS_X 325.0f

enum GameConsole
{
	CONSOLE_None = 0,

	CONSOLE_NES				= 1 << 0,
	CONSOLE_SNES			= 1 << 1,
	CONSOLE_N64				= 1 << 2,
	CONSOLE_GameCube		= 1 << 3,
	CONSOLE_Wii				= 1 << 4,
	CONSOLE_SMS				= 1 << 5,
	CONSOLE_Genesis			= 1 << 6,
	CONSOLE_SegaCD			= 1 << 7,
	CONSOLE_Saturn			= 1 << 8,
	CONSOLE_Dreamcast		= 1 << 9,
	CONSOLE_PS1				= 1 << 10,
	CONSOLE_PS2				= 1 << 11,
	CONSOLE_Xbox			= 1 << 12
};

enum MediaType
{
	MEDIATYPE_Boxarts,
	MEDIATYPE_Logos,
	MEDIATYPE_Music,
	MEDIATYPE_Snaps,
	MEDIATYPE_Titles,

	NUM_MEDIATYPES
};

enum InputAction
{
	FIGHTSTICK_LK,
	FIGHTSTICK_MK,
	//FIGHTSTICK_HK, // Trigger Positive
	FIGHTSTICK_LP,
	FIGHTSTICK_MP,
	FIGHTSTICK_HP,
	FIGHTSTICK_Start,
	FIGHTSTICK_Select,
	FIGHTSTICK_Up,
	FIGHTSTICK_Down,
	FIGHTSTICK_Left,
	FIGHTSTICK_Right,
	FIGHTSTICK_DriveImpact,
	FIGHTSTICK_Parry
};

enum InputController
{
	INPUTCONTROLLER_StreetFighter = 0,
	INPUTCONTROLLER_NES,
	INPUTCONTROLLER_StarCitizen,

	NUM_INPUTCONTROLLERS
};

enum ComponentType
{
	COMPONENT_Brb = 0,
	COMPONENT_GameBrowser,
	COMPONENT_InputViewer,
	COMPONENT_Crt,
	COMPONENT_NowPlaying,
	COMPONENT_Docket,
	COMPONENT_LiveSplit,
	COMPONENT_HeartBeat,
	COMPONENT_Wheel,
	COMPONENT_MessageCycle,

	NUM_COMPONENTS
};

enum StatusFlag
{
	STATUS_Blind = 1 << 0,
	STATUS_Owned = 1 << 1,
	STATUS_Wishlist = 1 << 2,
	STATUS_Played = 1 << 3,
	STATUS_Interested = 1 << 4,
	STATUS_Evergreen = 1 << 5,
	STATUS_Beaten = 1 << 6,
	STATUS_100Percent = 1 << 7,
	STATUS_Speedrunning = 1 << 8,
};

class GameInfo
{
	friend class Compositorium;
	GameConsole					m_eConsole;
	std::string					m_sGameId;

	std::string					m_sName;
	std::string					m_sDescription;
	std::string					m_sRelease;
	std::string					m_sDeveloper;
	std::string					m_sPublisher;
	std::vector<std::string>	m_MediaLists[NUM_MEDIATYPES];

	GameInfo(GameConsole eConsole, const std::string &sGameId, const HyJsonObj &jsonObj) : m_eConsole(eConsole), m_sGameId(sGameId)
	{
		m_sName = jsonObj["name"].GetString();
		m_sDescription = jsonObj["description"].GetString();
		m_sRelease = jsonObj["release"].GetString();
		m_sDeveloper = jsonObj["developer"].GetString();
		m_sPublisher = jsonObj["publisher"].GetString();
		for(int iMediaType = 0; iMediaType < NUM_MEDIATYPES; ++iMediaType)
		{
			std::string sMediaName;
			switch(static_cast<MediaType>(iMediaType))
			{
			case MEDIATYPE_Boxarts:		sMediaName = "boxarts"; break;
			case MEDIATYPE_Logos:		sMediaName = "logos"; break;
			case MEDIATYPE_Music:		sMediaName = "music"; break;
			case MEDIATYPE_Snaps:		sMediaName = "snaps"; break;
			case MEDIATYPE_Titles:		sMediaName = "titles"; break;
			}
			if(jsonObj.HasMember(sMediaName.c_str()))
			{
				const HyJsonArray &mediaArray = jsonObj[sMediaName.c_str()].GetArray();
				for(uint32_t i = 0; i < mediaArray.Size(); ++i)
					m_MediaLists[iMediaType].push_back(std::string(mediaArray[i].GetString()));
			}
		}
	}
public:
	GameInfo() : m_eConsole(CONSOLE_None), m_sGameId("")
	{ }

	bool IsValid() const { return m_eConsole != CONSOLE_None && m_sGameId.empty() == false; }
	GameConsole GetConsole() const { return m_eConsole; }
	const std::string &GetGameId() const { return m_sGameId; }
	const std::string &GetName() const { return m_sName; }
	const std::string &GetDescription() const { return m_sDescription; }
	const std::string &GetRelease() const { return m_sRelease; }
	const std::string &GetDeveloper() const { return m_sDeveloper; }
	const std::string &GetPublisher() const { return m_sPublisher; }
	const std::vector<std::string> &GetMediaList(MediaType eType) const { return m_MediaLists[eType]; }
};

class GameStats
{
	friend class Compositorium;
	GameConsole		m_eConsole;
	std::string		m_sGameId;

	uint32			m_uiStatusFlags;
	std::string		m_sDateTime_FirstPlayedOnStream;
	double			m_dElapsedPlayTime;
	std::string		m_sDateTime_BeatenOnStream;
	std::string		m_sNotes;

	GameStats(GameConsole eConsole, const std::string &sGameId) : m_eConsole(eConsole), m_sGameId(sGameId)
	{
		m_uiStatusFlags = 0;
		m_uiStatusFlags |= STATUS_Blind;
		m_dElapsedPlayTime = 0.0;
	}

	GameStats(GameConsole eConsole, const std::string &sGameId, const HyJsonObj &jsonObj) : 
		m_eConsole(eConsole),
		m_sGameId(sGameId)
	{
		m_uiStatusFlags = jsonObj["StatusFlags"].GetUint();
		m_sDateTime_FirstPlayedOnStream = jsonObj["FirstPlayedOnStream"].GetString();
		m_dElapsedPlayTime = jsonObj["ElapsedPlayTime"].GetDouble();
		m_sDateTime_BeatenOnStream = jsonObj["BeatenOnStream"].GetString();
		m_sNotes = jsonObj["Notes"].GetString();
	}
public:
	GameStats() : m_eConsole(CONSOLE_None), m_sGameId(""), m_uiStatusFlags(0), m_dElapsedPlayTime(0.0)
	{ }
	GameStats &operator=(const GameStats &copy)
	{
		m_eConsole = copy.m_eConsole;
		m_sGameId = copy.m_sGameId;
		m_uiStatusFlags = copy.m_uiStatusFlags;
		m_sDateTime_FirstPlayedOnStream = copy.m_sDateTime_FirstPlayedOnStream;
		m_dElapsedPlayTime = copy.m_dElapsedPlayTime;
		m_sDateTime_BeatenOnStream = copy.m_sDateTime_BeatenOnStream;
		m_sNotes = copy.m_sNotes;
		return *this;
	}

	bool IsValid() const { return m_eConsole != CONSOLE_None && m_sGameId.empty() == false; }
	GameConsole GetConsole() const { return m_eConsole; }
	const std::string &GetGameId() const { return m_sGameId; }
	bool IsStatusFlagSet(StatusFlag eFlag) const { return (m_uiStatusFlags & eFlag) != 0; }
	const std::string &GetFirstPlayedOnStream() const { return m_sDateTime_FirstPlayedOnStream; }
	double GetElapsedPlayTime() const { return m_dElapsedPlayTime; }
	const std::string &GetBeatenOnStream() const { return m_sDateTime_BeatenOnStream; }
	const std::string &GetNotes() const { return m_sNotes; }

	void Serialize(HyJsonDoc &jsonDoc)
	{
		rapidjson::Value o(rapidjson::kObjectType);
		o.AddMember("StatusFlags", rapidjson::Value(m_uiStatusFlags), jsonDoc.GetAllocator());  // deep clone contacts (may be with lots of allocations)
		o.AddMember("FirstPlayedOnStream", rapidjson::Value(m_sDateTime_FirstPlayedOnStream.c_str(), jsonDoc.GetAllocator()), jsonDoc.GetAllocator());
		o.AddMember("ElapsedPlayTime", rapidjson::Value(m_dElapsedPlayTime), jsonDoc.GetAllocator());
		o.AddMember("BeatenOnStream", rapidjson::Value(m_sDateTime_BeatenOnStream.c_str(), jsonDoc.GetAllocator()), jsonDoc.GetAllocator());
		o.AddMember("Notes", rapidjson::Value(m_sNotes.c_str(), jsonDoc.GetAllocator()), jsonDoc.GetAllocator());
		
		rapidjson::Value key(m_sGameId.c_str(), jsonDoc.GetAllocator());
		jsonDoc.AddMember(key, o, jsonDoc.GetAllocator());
	}
};

struct MusicTrack
{
	std::string		m_sFilePath;
	std::string		m_sGameId;
	MusicTrack(const std::string &sFilePath, const std::string &sGameId) : m_sFilePath(sFilePath), m_sGameId(sGameId)
	{ }
};

#endif // pch_h__
