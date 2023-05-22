#ifndef MULTITASK_EMPIRE_AUDIO_AUDIO_MANAGER_HPP
#define MULTITASK_EMPIRE_AUDIO_AUDIO_MANAGER_HPP

// Multitask-Empire
#include <audio/sound.hpp>
#include <audio/music.hpp>

// Containers
#include <map>
#include <vector>

namespace audio::manage {

class AudioManager {
public:
	AudioManager(const std::string& sound_folder, const std::string& music_folder);

	~AudioManager();

	// Music
	void MusicPlay();
	void MusicNextSong();
	void MusicPrevSong();
	void MusicPause();
	void MusicStop();

	// Sound
	void SoundPlay(const std::string& sound_name);
	void SoundPause();
	void SoundStop();

	// Volume
	void SetVolume(float volume);
	float GetVolume();

private:
	std::string sound_folder_;
	std::map<std::string, Sound*> sounds_;
	std::string cur_sound_;

	std::string music_folder_;
	std::vector<Music*> music_;
	size_t current_song_;

	float volume_;
};

}  // namespace audio::manage

#endif  // MULTITASK_EMPIRE_AUDIO_AUDIO_MANAGER_HPP
