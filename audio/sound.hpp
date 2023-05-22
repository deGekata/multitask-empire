#ifndef MULTITASK_EMPIRE_AUDIO_SOUND_HPP
#define MULTITASK_EMPIRE_AUDIO_SOUND_HPP

#include <SFML/Audio.hpp>

namespace audio {

class Sound {
public:
	explicit Sound(const std::string& path);

	// Playing
	void Play();
	void Pause();
	void Stop();

	// Volume
	void  SetVolume(float volume);
	float GetVolume();

private:
	sf::SoundBuffer buffer_;
	sf::Sound sound_;
};

}  // namespace audio

#endif  // MULTITASK_EMPIRE_AUDIO_SOUND_HPP
