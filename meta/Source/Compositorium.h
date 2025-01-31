#ifndef Compositorium_h__
#define Compositorium_h__

#include "pch.h"

class Compositorium
{
	static Compositorium *	sm_pInstance;

	const std::string		m_sROOT_PATH;

	enum GameConsoleIndex
	{
		CONSOLEINDEX_Invalid	= -1,

		CONSOLEINDEX_NES		= 0,
		CONSOLEINDEX_SNES		= 1,
		CONSOLEINDEX_N64		= 2,
		CONSOLEINDEX_GameCube	= 3,
		CONSOLEINDEX_Wii		= 4,
		CONSOLEINDEX_SMS		= 5,
		CONSOLEINDEX_Genesis	= 6,
		CONSOLEINDEX_Saturn		= 7,
		CONSOLEINDEX_Dreamcast	= 8,
		CONSOLEINDEX_PS1		= 9,
		CONSOLEINDEX_PS2		= 10,
		CONSOLEINDEX_Xbox		= 11,

		NUM_CONSOLES
	};
	static_assert(NUM_CONSOLES < 32, "Compositorium::GameConsoleIndex - Too many consoles, increase all bitmasks to 64bits");
	const std::string		m_sConsolePaths[NUM_CONSOLES];
	const std::string		m_sMediaPaths[NUM_MEDIATYPES];

	HyJsonDoc				m_MetaDocs[NUM_CONSOLES];
	HyJsonDoc				m_CollectionDocs[NUM_CONSOLES];

public:
	Compositorium(std::string sRootPath);
	virtual ~Compositorium();
	static Compositorium *Get() { return sm_pInstance; }

	std::string GetRootPath() const;

	std::vector<MusicTrack> GetMusicPlayList(uint32_t uiConsoleFlags);
	void GetMusicInfo(MusicTrack musicTrack, GameConsole &eConsoleOut, std::string &sGameOut, std::string &sSongOut, std::string &sComposerOut);

	GameInfo GetGame(GameConsole eConsole, std::string sGameId);

	std::string GetGameName(GameInfo gameObj);
	std::string GetGameName(GameConsole eConsole, std::string sGameId);
	std::string GetGameDescription(GameInfo gameObj);
	std::string GetGameDescription(GameConsole eConsole, std::string sGameId);
	std::string GetGameRelease(GameInfo gameObj);
	std::string GetGameDeveloper(GameInfo gameObj);
	std::string GetGamePublisher(GameInfo gameObj);

	std::vector<std::string> GetMediaList(GameInfo gameObj, MediaType eMediaType);
	std::vector<std::string> GetMediaList(GameConsole eConsole, std::string sGameId, MediaType eMediaType);
	std::string GetBestMedia(std::vector<std::string> mediaList);
	std::string GetBestMedia(GameInfo gameObj, MediaType eMediaType);
	std::string GetBestMedia(GameConsole eConsole, std::string sGameId, MediaType eMediaType);

	GameConsole GetConsoleFromPath(std::string sPath);
	std::string GetConsoleName(GameConsole eConsole);
	GameConsole GetConsoleFromName(std::string sConsoleName);

	std::string OpenHtmlFileDlg();

private:
	GameConsoleIndex ToIndex(GameConsole eConsole);
	GameConsole ToEnum(int iIndex);
	GameConsole ToEnum(GameConsoleIndex eIndex);

	//std::vector<std::string> GetEntireMediaList(MediaType eMediaType);

	//std::string GetBaseMediaName(std::string sMediaName);
};

#endif // Compositorium_h__
