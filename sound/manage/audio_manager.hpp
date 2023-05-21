#ifndef MULTITASK_EMPIRE_SOUND_AUDIO_MUSIC_HPP
#define MULTITASK_EMPIRE_SOUND_AUDIO_MUSIC_HPP

// Multitask-Empire
#include <sound/sound.hpp>
#include <sound/music.hpp>

// Containers
#include <map>
#include <vector>

namespace sound::manage {

class AudioManager {
public:
	AudioManager(const std::string& sound_folder, const std::string& music_folder);

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
	std::map<std::string, Sound> sounds_;
	std::string cur_sound_;

	std::string music_folder_;
	std::vector<Music> music_;
	size_t current_song_;

	float volume_;
};

}  // namespace sound::manage

#endif  // MULTITASK_EMPIRE_SOUND_AUDIO_MUSIC_HPP
