#ifndef pch_h__
#define pch_h__

#include "HyEngine.h"

#define DISPLAYORDER_DEBUG			99999
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

	CONSOLE_NES = 1 << 0,
	CONSOLE_SNES = 1 << 1,
	CONSOLE_N64 = 1 << 2,
	CONSOLE_GameCube = 1 << 3,
	CONSOLE_Wii = 1 << 4,
	CONSOLE_SMS = 1 << 5,
	CONSOLE_Genesis = 1 << 6,
	CONSOLE_Saturn = 1 << 7,
	CONSOLE_Dreamcast = 1 << 8,
	CONSOLE_PS1 = 1 << 9,
	CONSOLE_PS2 = 1 << 10,
	CONSOLE_Xbox = 1 << 11
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

enum StatusFlags
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
	GameConsole		m_eConsole;
	std::string		m_sGameId;
	HyJsonObj		m_JsonObj;
	GameInfo(GameConsole eConsole, const std::string &sGameId, const HyJsonObj &jsonObj) : m_eConsole(eConsole), m_sGameId(sGameId), m_JsonObj(jsonObj)
	{ }
public:
	GameInfo() : m_eConsole(CONSOLE_None), m_sGameId(""), m_JsonObj(HyJsonDoc(rapidjson::kObjectType).GetObject())
	{ }
	GameInfo &operator=(const GameInfo &copy)
	{
		m_eConsole = copy.m_eConsole;
		m_sGameId = copy.m_sGameId;
		m_JsonObj = copy.m_JsonObj;
		return *this;
	}
	bool IsValid() const { return m_eConsole != CONSOLE_None && m_sGameId.empty() == false; }
	GameConsole GetConsole() const { return m_eConsole; }
	const std::string &GetGameId() const { return m_sGameId; }
};

struct GameStats
{
	GameConsole		m_eConsole;
	std::string		m_sGameId;

	uint32			m_uiStatusFlags;
	std::string		m_sDateTime_FirstPlayedOnStream;
	double			m_dElapsedPlayTime;
	std::string		m_sDateTime_BeatenOnStream;
	std::string		m_sNotes;

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
