#ifndef MULTITASK_EMPIRE_SOUND_SOUND_HPP
#define MULTITASK_EMPIRE_SOUND_SOUND_HPP

#include <SFML/Audio.hpp>

namespace sound {

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

}  // namespace sound

#endif  // MULTITASK_EMPIRE_SOUND_SOUND_HPP
