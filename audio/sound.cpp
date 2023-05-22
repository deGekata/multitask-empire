#include <audio/sound.hpp>

namespace audio {

Sound::Sound(const std::string& path) {
	buffer_.loadFromFile(path);
	sound_.setBuffer(buffer_);
}

void Sound::Play() {
	sound_.play();
}

void Sound::Pause() {
	sound_.pause();
}

void Sound::Stop() {
	sound_.stop();
}

void Sound::SetVolume(float volume) {
	sound_.setVolume(volume);
}

float Sound::GetVolume() {
	return sound_.getVolume();
}

}  // namespace audio
