#include "pch.h"
#include "Compositorium.h"

Compositorium *Compositorium::sm_pInstance = nullptr;

Compositorium::Compositorium(std::string sRootPath) :
	m_sROOT_PATH(HyIO::CleanPath(sRootPath, "/")),
	m_sConsolePaths{ "NES/", "SNES/", "N64/", "GameCube/", "Wii/", "SMS/", "Genesis/", "Saturn/", "Dreamcast/", "PS1/", "PS2/", "Xbox/" },
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
			HyError("HyAssets::ParseManifestFile - Manifest had JSON parsing error: " << rapidjson::GetParseErrorFunc(m_MetaDocs[i].GetParseError()));
		HyAssert(m_MetaDocs[i].IsObject(), "Compositorium - " << m_sConsolePaths[i] << " Meta json file wasn't an object");
	}

	sm_pInstance = this;
}

/*virtual*/ Compositorium::~Compositorium()
{
	sm_pInstance = nullptr;
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

void Compositorium::GetMusicInfo(MusicTrack musicTrack, GameConsole &eConsoleOut, std::string &sGameOut, std::string &sSongOut, std::string &sComposerOut)
{
	eConsoleOut = GetConsoleFromPath(musicTrack.m_sFilePath);

	std::string sMusicTrack = HyIO::GetFileNameWithoutExtension(HyIO::GetFileNameFromPath(musicTrack.m_sFilePath));
	size_t uiDelimIndex = sMusicTrack.rfind('[', std::string::npos);
	if(uiDelimIndex == std::string::npos)
	{
		HyError("Compositorium::GetMusicInfo() - Invalid music file name: " << musicTrack.m_sFilePath);
		return; // This shouldn't happen. There should always be a [...] at the end of the file name
	}

	sGameOut = GetGameName(eConsoleOut, musicTrack.m_sGameId);
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

GameObj Compositorium::GetGame(GameConsole eConsole, std::string sGameId)
{
	HyJsonObj consoleObj = m_MetaDocs[ToIndex(eConsole)].GetObject();
	if(consoleObj.HasMember(sGameId.c_str()) == false)
	{
		HyLogError("Compositorium::GetGame() - " << sGameId << " not found in " << m_sConsolePaths[ToIndex(eConsole)] << " meta.json");
		return GameObj();
	}
	GameObj returnObj(eConsole, sGameId, consoleObj[sGameId.c_str()].GetObject());
	return returnObj;
}

std::string Compositorium::GetGameName(GameObj gameObj)
{
	if(gameObj.IsValid() == false)
		return "";
	return gameObj.m_JsonObj["name"].GetString();
}
std::string Compositorium::GetGameName(GameConsole eConsole, std::string sGameId)
{
	return GetGameName(GetGame(eConsole, sGameId));
}

std::string Compositorium::GetGameDescription(GameObj gameObj)
{
	if(gameObj.IsValid() == false)
		return "";
	return gameObj.m_JsonObj["description"].GetString();
}
std::string Compositorium::GetGameDescription(GameConsole eConsole, std::string sGameId)
{
	return GetGameDescription(GetGame(eConsole, sGameId));
}

std::vector<std::string> Compositorium::GetMediaList(GameObj gameObj, MediaType eMediaType)
{
	std::string sMediaName;
	switch(eMediaType)
	{
	case MEDIATYPE_Boxarts:		sMediaName = "boxarts"; break;
	case MEDIATYPE_Logos:		sMediaName = "logos"; break;
	case MEDIATYPE_Music:		sMediaName = "music"; break;
	case MEDIATYPE_Snaps:		sMediaName = "snaps"; break;
	case MEDIATYPE_Titles:		sMediaName = "titles"; break;
	}

	std::vector<std::string> sMediaList;
	if(gameObj.m_JsonObj.HasMember(sMediaName.c_str()))
	{
		HyJsonArray mediaArray = gameObj.m_JsonObj[sMediaName.c_str()].GetArray();
		for(uint32_t i = 0; i < mediaArray.Size(); ++i)
		{
			std::string sFile = m_sROOT_PATH + m_sConsolePaths[ToIndex(gameObj.m_eConsole)] + m_sMediaPaths[eMediaType] + mediaArray[i].GetString();
			sMediaList.push_back(sFile);
		}
	}
	return sMediaList;
}
std::vector<std::string> Compositorium::GetMediaList(GameConsole eConsole, std::string sGameId, MediaType eMediaType)
{
	return GetMediaList(GetGame(eConsole, sGameId), eMediaType);
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
std::string Compositorium::GetBestMedia(GameObj gameObj, MediaType eMediaType)
{
	return GetBestMedia(GetMediaList(gameObj, eMediaType));
}
std::string Compositorium::GetBestMedia(GameConsole eConsole, std::string sGameId, MediaType eMediaType)
{
	return GetBestMedia(GetMediaList(eConsole, sGameId, eMediaType));
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
	case CONSOLEINDEX_NES:		return CONSOLE_NES;
	case CONSOLEINDEX_SNES:		return CONSOLE_SNES;
	case CONSOLEINDEX_N64:		return CONSOLE_N64;
	case CONSOLEINDEX_GameCube:	return CONSOLE_GameCube;
	case CONSOLEINDEX_Wii:		return CONSOLE_Wii;
	case CONSOLEINDEX_SMS:		return CONSOLE_SMS;
	case CONSOLEINDEX_Genesis:	return CONSOLE_Genesis;
	case CONSOLEINDEX_Saturn:	return CONSOLE_Saturn;
	case CONSOLEINDEX_Dreamcast:	return CONSOLE_Dreamcast;
	case CONSOLEINDEX_PS1:		return CONSOLE_PS1;
	case CONSOLEINDEX_PS2:		return CONSOLE_PS2;
	case CONSOLEINDEX_Xbox:		return CONSOLE_Xbox;
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
