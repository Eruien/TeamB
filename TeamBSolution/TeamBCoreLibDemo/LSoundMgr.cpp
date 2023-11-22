#include "LSoundMgr.h"

void LSound::Set(FMOD::System* system)
{
	m_pSystem = system;
}

void LSound::SetName(std::wstring key)
{
	m_csName = key;
}

bool LSound::Load(std::wstring filePath)
{
	std::string path = wtm(filePath);
	FMOD_RESULT hr = m_pSystem->createSound(path.c_str(), FMOD_DEFAULT, NULL, &m_pSound);

	if (hr)
	{
		MessageBoxA(NULL, "Sound Load Error", "Error Box", MB_OK);
		return false;
	}

	return true;
}

void LSound::Play(bool bLoop)
{
	if (m_pChannel == nullptr)
	{
		if (bLoop)
		{
			m_pSound->setMode(FMOD_LOOP_NORMAL);
		}
		else
		{
			m_pSound->setMode(FMOD_LOOP_OFF);
		}
		m_pSystem->playSound(m_pSound, NULL, false, &m_pChannel);
	}
	else
	{
		bool isPlay;
		m_pChannel->isPlaying(&isPlay);
		if (isPlay == false)
		{
			m_pSystem->playSound(m_pSound, NULL, false, &m_pChannel);
		}
	}
}

void LSound::PlayEffect()
{
	m_pSystem->playSound(m_pSound, NULL, false, &m_pChannel);
}

void LSound::Stop()
{
	m_pChannel->stop();
}

void LSound::Pause()
{
	bool isPaused;
	m_pChannel->getPaused(&isPaused);
	m_pChannel->setPaused(!isPaused);
}

void LSound::VolumeUp()
{
	float volume;
	m_pChannel->getVolume(&volume);
	volume += 0.33f;
	volume = fmin(volume, 1.0f);
	m_pChannel->setVolume(volume);
}

void LSound::VolumeDown()
{
	float volume;
	m_pChannel->getVolume(&volume);
	volume -= 0.33f;
	volume = fmax(volume, 0.0f);
	m_pChannel->setVolume(volume);
}


bool LSound::Init()
{
	return true;
}

bool LSound::Frame()
{
	return true;
}

bool LSound::Release()
{
	m_pChannel->stop();
	m_pSound->release();
	m_pChannel = nullptr;
	m_pSound = nullptr;
	return true;
}

LSound* LSoundMgr::Load(std::wstring filePath)
{
	std::size_t found = filePath.find_last_of(L"/");
	std::wstring path = filePath.substr(0, found + 1);
	std::wstring key = filePath.substr(found + 1);
	LSound* data = GetPtr(key);

	if (data != nullptr)
	{
		return data;
	}

	std::unique_ptr<LSound> newData = std::make_unique <LSound> ();

	newData->Set(m_pSystem);
	newData->SetName(key);
	
	if (newData->Load(filePath))
	{
		m_map.insert(std::make_pair(key, std::move(newData)));
		return GetPtr(key);
	}

	return nullptr;
}

LSound* LSoundMgr::GetPtr(std::wstring key)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return nullptr;
	}

	return iter->second.get();
}

bool LSoundMgr::Get(std::wstring key, LSound& ret)
{
	auto iter = m_map.find(key);

	if (m_map.end() == iter)
	{
		return false;
	}

	ret = *(iter->second);

	return true;
}

bool LSoundMgr::Frame()
{
	m_pSystem->update();

	for (auto& data : m_map)
	{
		data.second->Frame();
	}

	return true;
}

bool LSoundMgr::Release()
{
	for (auto& data : m_map)
	{
		data.second->Release();
	}

	m_map.clear();

	m_pSystem->close();
	m_pSystem->release();

	return true;
}

LSoundMgr::LSoundMgr()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
}

LSoundMgr::~LSoundMgr() {}


