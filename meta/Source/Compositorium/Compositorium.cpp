#include "pch.h"
#include "Compositorium.h"

#include <fstream>
#include <commdlg.h>

Compositorium *Compositorium::sm_pInstance = nullptr;

Compositorium::Compositorium(std::string sRootPath) :
	m_sROOT_PATH(HyIO::CleanPath(sRootPath, "/")),
	m_sConsolePaths{ "NES/", "SNES/", "N64/", "GameCube/", "Wii/", "SMS/", "Genesis/", "SegaCD/", "Saturn/", "Dreamcast/", "PS1/", "PS2/", "Xbox/" },
	m_sMediaPaths{ "media/boxarts/", "media/logos/", "media/music/", "media/snaps/", "media/titles/" }
{
	HyAssert(sm_pInstance == nullptr, "Compositorium::Compositorium() - Only one instance of Compositorium allowed");

	for(uint32_t i = 0; i < NUM_CONSOLES; ++i)
	{
		if(!HyIO::DirectoryExists(m_sROOT_PATH + m_sConsolePaths[i]))
			HyError("Compositorium::Compositorium() - Console path does not exist: " << m_sROOT_PATH + m_sConsolePaths[i]);

		std::string sMetaFilePath = m_sROOT_PATH + m_sConsolePaths[i] + "meta.json";
		std::vector<char> sMetaFileContents;
		HyIO::ReadTextFile(sMetaFilePath.c_str(), sMetaFileContents);
		if(m_MetaDocs[i].Parse(sMetaFileContents.data()).HasParseError())
			HyError("Compositorium::Compositorium - JSON parsing error: " << rapidjson::GetParseErrorFunc(m_MetaDocs[i].GetParseError()));
		HyAssert(m_MetaDocs[i].IsObject(), "Compositorium - " << m_sConsolePaths[i] << " Meta json file wasn't an object");

		std::string sStatFilePath = m_sROOT_PATH + m_sConsolePaths[i] + "stats.json";
		if(HyIO::FileExists(sStatFilePath) == false)
		{
			std::ofstream file(sStatFilePath);
			file << "{}";
			file.close();
		}

		std::vector<char> sStatFileContents;
		HyIO::ReadTextFile(sStatFilePath.c_str(), sStatFileContents);
		if(m_StatDocs[i].Parse(sStatFileContents.data()).HasParseError())
			HyError("Compositorium::Compositorium - JSON parsing error: " << rapidjson::GetParseErrorFunc(m_StatDocs[i].GetParseError()));
		HyAssert(m_StatDocs[i].IsObject(), "Compositorium - " << m_sConsolePaths[i] << " Stats json file wasn't an object");
	}

	sm_pInstance = this;
}

/*virtual*/ Compositorium::~Compositorium()
{
	sm_pInstance = nullptr;
}

std::string Compositorium::GetRootPath() const
{
	return m_sROOT_PATH;
}

std::vector<GameConsole> Compositorium::GetConsoleList()
{
	std::vector<GameConsole> consoleList;
	for(uint32_t i = 0; i < NUM_CONSOLES; ++i)
		consoleList.push_back(static_cast<GameConsole>(1 << i));

	return consoleList;
}

std::vector<MusicTrack> Compositorium::GetMusicPlayList(uint32_t uiConsoleFlags)
{
	std::vector<MusicTrack> sMusicList;
	std::vector<std::string> tempDirFileList;
	for(uint32_t i = 0; i < NUM_CONSOLES; ++i)
	{
		if(uiConsoleFlags & (1 << i))
		{
			for(auto iter = m_MetaDocs[i].MemberBegin(); iter != m_MetaDocs[i].MemberEnd(); ++iter)
			{
				std::string test = iter->name.GetString();
				HyJsonObj gameObj = iter->value.GetObject();
				if(gameObj.HasMember("music") == false)
					HyLogError("Compositorium::GetMusicList() - " << iter->name.GetString() << " has no music list");
				else
				{
					HyJsonArray musicArray = gameObj["music"].GetArray();
					for(uint32_t j = 0; j < musicArray.Size(); ++j)
						sMusicList.push_back(MusicTrack(m_sROOT_PATH + m_sConsolePaths[i] + m_sMediaPaths[MEDIATYPE_Music] + musicArray[j].GetString(), iter->name.GetString()));
				}
			}
		}
	}

	HyRand::Shuffle(sMusicList);
	return sMusicList;
}

void Compositorium::GetMusicInfo(MusicTrack musicTrack, GameConsole &eConsoleOut, std::string &sSongOut, std::string &sComposerOut)
{
	eConsoleOut = GetConsoleFromPath(musicTrack.m_sFilePath);

	std::string sMusicTrack = HyIO::GetFileNameWithoutExtension(HyIO::GetFileNameFromPath(musicTrack.m_sFilePath));
	size_t uiDelimIndex = sMusicTrack.rfind('[', std::string::npos);
	if(uiDelimIndex == std::string::npos)
	{
		HyError("Compositorium::GetMusicInfo() - Invalid music file name: " << musicTrack.m_sFilePath);
		return; // This shouldn't happen. There should always be a [...] at the end of the file name
	}

	sSongOut = sMusicTrack.substr(uiDelimIndex + 1, sMusicTrack.size() - uiDelimIndex - 2);
	HyIO::TrimWhitespace(sSongOut);
	
	//// Load the .ogg file using TagLib's FileRef
	//TagLib::FileRef f(sMusicFile.c_str());

	//// Check if the file was loaded successfully
	//if(!f.isNull() && f.tag())
	//{
	//	TagLib::Tag *tag = f.tag();
	//	m_ComposerText.SetText(tag->artist());
	//}
	//else
	sComposerOut = "";
}

GameInfo Compositorium::GetGame(GameConsole eConsole, std::string sGameId)
{
	HyJsonObj consoleObj = m_MetaDocs[ToIndex(eConsole)].GetObject();
	if(consoleObj.HasMember(sGameId.c_str()) == false)
	{
		HyLogError("Compositorium::GetGame() - " << sGameId << " not found in " << m_sConsolePaths[ToIndex(eConsole)] << " meta.json");
		return GameInfo();
	}
	GameInfo returnObj(eConsole, sGameId, consoleObj[sGameId.c_str()].GetObject());
	return returnObj;
}

GameInfo Compositorium::GetAlphaJumpGame(GameConsole eConsole, std::string sAlphaJumpChar)
{
	const HyJsonObj &consoleObj = m_MetaDocs[ToIndex(eConsole)].GetObject();
	for(auto iter = consoleObj.MemberBegin(); iter != consoleObj.MemberEnd(); ++iter)
	{
		if(iter->value["name"].GetString()[0] == sAlphaJumpChar[0])
			return GameInfo(eConsole, iter->name.GetString(), iter->value.GetObject());
	}
	return GameInfo();
}

std::vector<GameInfo> Compositorium::GetNextGames(GameInfo hGame, int iNumGames)
{
	std::vector<GameInfo> sGameList;
	if(hGame.IsValid() == false || iNumGames <= 0)
		return sGameList;

	const HyJsonObj &consoleObj = m_MetaDocs[ToIndex(hGame.m_eConsole)].GetObject();
	bool bFound = false;
	for(auto iter = consoleObj.MemberBegin(); ; ++iter)
	{
		if(iter == consoleObj.MemberEnd())
			iter = consoleObj.MemberBegin();

		if(bFound)
		{
			sGameList.push_back(GameInfo(hGame.m_eConsole, iter->name.GetString(), iter->value.GetObject()));
			if(sGameList.size() >= iNumGames)
				break;
		}
		else if(iter->name.GetString() == hGame.GetGameId())
			bFound = true;
	}
	return sGameList;
}

std::vector<GameInfo> Compositorium::GetPrevGames(GameInfo hGame, int iNumGames)
{
	std::vector<GameInfo> sGameList;
	if(hGame.IsValid() == false || iNumGames <= 0)
		return sGameList;

	const HyJsonObj &consoleObj = m_MetaDocs[ToIndex(hGame.m_eConsole)].GetObject();
	auto iter = consoleObj.MemberBegin();
	for(; iter != consoleObj.MemberEnd(); ++iter)
	{
		if(iter->name.GetString() == hGame.GetGameId())
			break;
	}
	for(; ; --iter)
	{
		sGameList.push_back(GameInfo(hGame.m_eConsole, iter->name.GetString(), iter->value.GetObject()));
		if(sGameList.size() >= iNumGames)
			break;

		if(iter == consoleObj.MemberBegin())
			iter = consoleObj.MemberEnd();
	}

	std::reverse(sGameList.begin(), sGameList.end());
	return sGameList;
}

GameStats Compositorium::GetGameStats(GameInfo gameObj)
{
	HyJsonObj consoleObj = m_StatDocs[ToIndex(gameObj.m_eConsole)].GetObject();
	if(consoleObj.HasMember(gameObj.GetGameId().c_str()) == false)
		return GameStats(gameObj.GetConsole(), gameObj.GetGameId());

	return GameStats(gameObj.GetConsole(), gameObj.GetGameId(), consoleObj[gameObj.GetGameId().c_str()].GetObject());
}

//std::string Compositorium::GetGameName(GameInfo gameObj)
//{
//	if(gameObj.IsValid() == false)
//		return "";
//	return gameObj.m_JsonObj["name"].GetString();
//}
//std::string Compositorium::GetGameName(GameConsole eConsole, std::string sGameId)
//{
//	return GetGameName(GetGame(eConsole, sGameId));
//}
//
//std::string Compositorium::GetGameDescription(GameInfo gameObj)
//{
//	if(gameObj.IsValid() == false)
//		return "";
//	return gameObj.m_JsonObj["description"].GetString();
//}
//std::string Compositorium::GetGameDescription(GameConsole eConsole, std::string sGameId)
//{
//	return GetGameDescription(GetGame(eConsole, sGameId));
//}
//
//std::string Compositorium::GetGameRelease(GameInfo gameObj)
//{
//	if(gameObj.IsValid() == false)
//		return "";
//	return gameObj.m_JsonObj["release"].GetString();
//}
//
//std::string Compositorium::GetGameDeveloper(GameInfo gameObj)
//{
//	if(gameObj.IsValid() == false)
//		return "";
//	return gameObj.m_JsonObj["developer"].GetString();
//}
//
//std::string Compositorium::GetGamePublisher(GameInfo gameObj)
//{
//	if(gameObj.IsValid() == false)
//		return "";
//	return gameObj.m_JsonObj["publisher"].GetString();
//}
//
//std::string Compositorium::GetMediaName(MediaType eMediaType)
//{
//	return m_sMediaPaths[eMediaType];
//}
//
//
//std::vector<std::string> Compositorium::GetMediaList(GameInfo gameObj, MediaType eMediaType)
//{
//
//
//	std::vector<std::string> sMediaList;
//	if(gameObj.m_JsonObj.HasMember(sMediaName.c_str()))
//	{
//		HyJsonArray mediaArray = gameObj.m_JsonObj[sMediaName.c_str()].GetArray();
//		for(uint32_t i = 0; i < mediaArray.Size(); ++i)
//		{
//			std::string sFile = m_sROOT_PATH + m_sConsolePaths[ToIndex(gameObj.m_eConsole)] + m_sMediaPaths[eMediaType] + mediaArray[i].GetString();
//			sMediaList.push_back(sFile);
//		}
//	}
//	return sMediaList;
//}
//std::vector<std::string> Compositorium::GetMediaList(GameConsole eConsole, std::string sGameId, MediaType eMediaType)
//{
//	return GetMediaList(GetGame(eConsole, sGameId), eMediaType);
//}

std::string Compositorium::GetMediaPath(GameConsole eConsole, MediaType eMediaType)
{
	return m_sROOT_PATH + m_sConsolePaths[ToIndex(eConsole)] + m_sMediaPaths[eMediaType];
}

std::string Compositorium::GetBestMedia(std::vector<std::string> mediaList)
{
	if(mediaList.empty())
		return "";
	int pts = 10; // Lower the points the better
	std::string sBestMedia;
	for(auto &sMedia : mediaList)
	{
		// In order, prioritize: "(US)", "(World)", "(EU)", "(JP)"
		if(sMedia.find("(US)") != std::string::npos)
			return sMedia;
		else if(sMedia.find("(World)") != std::string::npos && pts > 1)
		{
			sBestMedia = sMedia;
			pts = 1;
		}
		else if(sMedia.find("(EU)") != std::string::npos && pts > 2)
		{
			sBestMedia = sMedia;
			pts = 2;
		}
		else if(sMedia.find("(JP)") != std::string::npos && pts > 3)
		{
			sBestMedia = sMedia;
			pts = 3;
		}
	}
	if(sBestMedia.empty() == false)
		return mediaList[0];
	return sBestMedia;
}
std::string Compositorium::GetBestMedia(GameInfo gameObj, MediaType eMediaType)
{
	return GetBestMedia(gameObj.GetMediaList(eMediaType));
}

GameConsole Compositorium::GetConsoleFromPath(std::string sPath)
{
	sPath = HyIO::CleanPath(sPath, "/");
	for(uint32_t i = 0; i < NUM_CONSOLES; ++i)
	{
		std::string s = "/" + m_sConsolePaths[i];
		if(sPath.find(s) != std::string::npos)
			return ToEnum(i);
	}

	return CONSOLE_None;
}

std::string Compositorium::GetConsoleName(GameConsole eConsole)
{
	std::string sConsoleName = m_sConsolePaths[ToIndex(eConsole)];
	if(sConsoleName.back() == '/') // Remove the trailing slash
		sConsoleName.pop_back();
	return sConsoleName;
}

GameConsole Compositorium::GetConsoleFromName(std::string sConsoleName)
{
	for(uint32_t i = 0; i < NUM_CONSOLES; ++i)
	{
		GameConsole eConsole = ToEnum(i);
		if(GetConsoleName(eConsole) == sConsoleName)
			return eConsole;
	}
	return CONSOLE_None;
}

std::string Compositorium::GetConsoleMediaPath(GameConsole eConsole, bool bLogo)
{
	return m_sROOT_PATH + m_sConsolePaths[ToIndex(eConsole)] + "media/" + (bLogo ? "console_logo.png" : "console.png");
}

std::string Compositorium::OpenHtmlFileDlg()
{
	// Browse for a file
	OPENFILENAME ofn;       // Common dialog box structure
	char szFile[260];       // Buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetConsoleWindow();  // Use the console window as parent
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "HTML files\0*.HTML;*.HTM\0All files\0*.*\0";  // Filter for .html and .htm files
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.lpstrTitle = "Select an HTML file";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open File dialog box
	if(GetOpenFileName(&ofn) == TRUE)
	{
		return ofn.lpstrFile;
	}

	return "";
}

Compositorium::GameConsoleIndex Compositorium::ToIndex(GameConsole eConsole)
{
	//int index = std::log2(eConsole & -eConsole);
	switch(eConsole)
	{
	case CONSOLE_NES:		return CONSOLEINDEX_NES;
	case CONSOLE_SNES:		return CONSOLEINDEX_SNES;
	case CONSOLE_N64:		return CONSOLEINDEX_N64;
	case CONSOLE_GameCube:	return CONSOLEINDEX_GameCube;
	case CONSOLE_Wii:		return CONSOLEINDEX_Wii;
	case CONSOLE_SMS:		return CONSOLEINDEX_SMS;
	case CONSOLE_Genesis:	return CONSOLEINDEX_Genesis;
	case CONSOLE_SegaCD:	return CONSOLEINDEX_SegaCD;
	case CONSOLE_Saturn:	return CONSOLEINDEX_Saturn;
	case CONSOLE_Dreamcast:	return CONSOLEINDEX_Dreamcast;
	case CONSOLE_PS1:		return CONSOLEINDEX_PS1;
	case CONSOLE_PS2:		return CONSOLEINDEX_PS2;
	case CONSOLE_Xbox:		return CONSOLEINDEX_Xbox;
	default:
		HyError("Compositorium::ToIndex() - Invalid GameConsole");
		break;
	}

	return CONSOLEINDEX_Invalid;
}

GameConsole Compositorium::ToEnum(int iIndex)
{
	return ToEnum(static_cast<GameConsoleIndex>(iIndex));
}
GameConsole Compositorium::ToEnum(GameConsoleIndex eIndex)
{
	switch(eIndex)
	{
	case CONSOLEINDEX_NES:			return CONSOLE_NES;
	case CONSOLEINDEX_SNES:			return CONSOLE_SNES;
	case CONSOLEINDEX_N64:			return CONSOLE_N64;
	case CONSOLEINDEX_GameCube:		return CONSOLE_GameCube;
	case CONSOLEINDEX_Wii:			return CONSOLE_Wii;
	case CONSOLEINDEX_SMS:			return CONSOLE_SMS;
	case CONSOLEINDEX_Genesis:		return CONSOLE_Genesis;
	case CONSOLEINDEX_SegaCD:		return CONSOLE_SegaCD;
	case CONSOLEINDEX_Saturn:		return CONSOLE_Saturn;
	case CONSOLEINDEX_Dreamcast:	return CONSOLE_Dreamcast;
	case CONSOLEINDEX_PS1:			return CONSOLE_PS1;
	case CONSOLEINDEX_PS2:			return CONSOLE_PS2;
	case CONSOLEINDEX_Xbox:			return CONSOLE_Xbox;
	default:
		HyError("Compositorium::ToEnum() - Invalid GameConsoleIndex");
		break;
	}
	return CONSOLE_None;
}

//std::string Compositorium::GetBaseMediaName(std::string sMediaName)
//{
//	size_t uiDelim = sMediaName.find('(');
//	if(uiDelim != std::string::npos)
//		sMediaName = sMediaName.substr(0, uiDelim);
//
//	HyIO::TrimWhitespace(sMediaName);
//	return sMediaName;
//}
