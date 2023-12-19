#include "pch.h"
#include "VgMusic.h"

VgMusic::VgMusic(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_BoxArt(this),
	m_Title(this),
	m_Snapshot(this),
	m_AudioTrackText("", "MainText", this),
	m_iCurrTrackIndex(-1)
{
	std::vector<std::string> tempDirFileList;

	// Nintendo Entertainment System
	tempDirFileList = HyIO::GetFileList("\\\\IronMountain/Documents/Video Games Meta-Scrape/NES/media/Named_Ost", ".ogg", false);
	m_MusicFileList.insert(std::end(m_MusicFileList), std::begin(tempDirFileList), std::end(tempDirFileList));

	// Super Nintendo
	tempDirFileList = HyIO::GetFileList("\\\\IronMountain/Documents/Video Games Meta-Scrape/SNES/media/Named_Ost", ".ogg", false);
	m_MusicFileList.insert(std::end(m_MusicFileList), std::begin(tempDirFileList), std::end(tempDirFileList));

	// Genesis
	tempDirFileList = HyIO::GetFileList("\\\\IronMountain/Documents/Video Games Meta-Scrape/Genesis/media/Named_Ost", ".ogg", false);
	m_MusicFileList.insert(std::end(m_MusicFileList), std::begin(tempDirFileList), std::end(tempDirFileList));

	HyRand::Shuffle(m_MusicFileList);
	NextTrack();
}

/*virtual*/ VgMusic::~VgMusic()
{
}

/*virtual*/ void VgMusic::OnUpdate() /*override*/
{
}

void VgMusic::NextTrack()
{
	m_iCurrTrackIndex = (m_iCurrTrackIndex + 1) % m_MusicFileList.size();

	const std::string sMusicFile = m_MusicFileList[m_iCurrTrackIndex];

	
	std::string sMediaDirectory = HyIO::GetDirectoryFromPath(sMusicFile, false); // First strip the file name
	sMediaDirectory = HyIO::GetDirectoryFromPath(sMediaDirectory, false); // Then go back one directory

	std::string sMusicTrack = HyIO::GetFileNameWithoutExtension(HyIO::GetFileNameFromPath(sMusicFile, false), false);

	size_t uiDelimIndex = sMusicTrack.rfind('[', std::string::npos);
	if(uiDelimIndex == std::string::npos)
		return; // This shouldn't happen. There should always be a [...] at the end of the file name

	std::string sBaseFileName = sMusicTrack.substr(0, uiDelimIndex);
	sMusicTrack = sMusicTrack.substr(uiDelimIndex + 1, sMusicTrack.size() - uiDelimIndex - 2);
	
	std::string sBoxartFile = sMediaDirectory + "/Named_Boxarts/" + sBaseFileName + ".png";
	std::string sTitleFile = sMediaDirectory + "/Named_Titles/" + sBaseFileName + ".png";
	std::string sSnapshotFile = sMediaDirectory + "/Named_Snaps/" + sBaseFileName + ".png";

	m_BoxArt.Init(sBoxartFile, HyTextureInfo(), this);
	m_Title.Init(sTitleFile, HyTextureInfo(), this);
	m_Snapshot.Init(sSnapshotFile, HyTextureInfo(), this);

	Load();
}
