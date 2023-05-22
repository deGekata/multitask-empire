#ifndef MULTITASK_EMPIRE_AUDIO_MUSIC_HPP
#define MULTITASK_EMPIRE_AUDIO_MUSIC_HPP

#include <SFML/Audio.hpp>

namespace audio {

class Music {
public:
	explicit Music(const std::string& path);

	// Playing
	void Play();
	void Pause();
	void Stop();

	// Volume
	void  SetVolume(float volume);
	float GetVolume();

private:
	sf::Music music_;
};

}  // namespace audio

#endif  // MULTITASK_EMPIRE_AUDIO_MUSIC_HPP
