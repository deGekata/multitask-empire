#include <sound/music.hpp>

namespace sound {

Music::Music(const std::string& path) {
	music_.openFromFile(path);
}

void Music::Play() {
	music_.play();
}

void Music::Pause() {
	music_.pause();
}

void Music::Stop() {
	music_.stop();
}

void Music::SetVolume(float volume) {
	music_.setVolume(volume);
}

float Music::GetVolume() {
	return music_.getVolume();
}

}  // namespace sound
