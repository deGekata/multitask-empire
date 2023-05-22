#include <audio/manage/audio_manager.hpp>

#include <filesystem>

namespace audio::manage {

AudioManager::AudioManager(const std::string& sound_folder, const std::string& music_folder)
	: sound_folder_(sound_folder)
	, music_folder_(music_folder)
	, current_song_(0)
	, volume_(0.5) {

	for (auto& sound : std::filesystem::directory_iterator(sound_folder_)) {
		sounds_.insert({sound.path(), new Sound(sound.path())});
	}

	for (auto& song : std::filesystem::directory_iterator(music_folder_)) {
		music_.push_back(new Music(song.path()));
	}
}

AudioManager::~AudioManager() {
	for (auto& sound : sounds_) {
		delete sound.second;
	}

	for (auto& song : music_) {
		delete song;
	}
}

void AudioManager::MusicPlay() {
	if (music_.empty()) {
		return;
	}

	music_[current_song_]->Play();
}

void AudioManager::MusicNextSong() {
	if (music_.empty()) {
		return;
	}

	music_[current_song_]->Stop();
	current_song_ = (current_song_ + 1) % music_.size();
	music_[current_song_]->Play();
}

void AudioManager::MusicPrevSong() {
	if (music_.empty()) {
		return;
	}

	music_[current_song_]->Stop();
	current_song_ = (music_.size() + (current_song_ - 1) % music_.size()) % music_.size();
	music_[current_song_]->Play();
}

void AudioManager::MusicPause() {
	if (music_.empty()) {
		return;
	}

	music_[current_song_]->Pause();
}

void AudioManager::MusicStop() {
	if (music_.empty()) {
		return;
	}

	music_[current_song_]->Stop();
}

void AudioManager::SoundPlay(const std::string& sound_name) {
	auto sound = sounds_.find(sound_name);
	if (sound != sounds_.end()) {
		cur_sound_ = sound_name;
		sound->second->Play();
	}
}

void AudioManager::SoundPause() {
	sounds_.find(cur_sound_)->second->Pause();
}

void AudioManager::SoundStop() {
	sounds_.find(cur_sound_)->second->Stop();
}

void AudioManager::SetVolume(float volume) {
	volume_ = volume;

	for (auto& sound : sounds_) {
		sound.second->SetVolume(volume);
	}

	for (auto& song : music_) {
		song->SetVolume(volume);
	}
}

float AudioManager::GetVolume() {
	return volume_;
}

}  // namespace audio::manage
