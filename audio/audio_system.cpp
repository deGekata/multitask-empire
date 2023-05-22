#include <audio/audio_system.hpp>
#include <events/audio_events.hpp>

AudioSystem::AudioSystem():
    manager_("sounds/", "music/"){}

void AudioSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlaySoundEvent>(*this);
    events.Subscribe<ActionCommandEvent>(*this);
}

void AudioSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
}

void AudioSystem::Receive(const PlaySoundEvent& event) {

    switch(event.sound_id_) {
        case Sound::Attack: {
            manager_.SoundPlay("poison.wav");
            break;
        }
        case Sound::Jump: {
            manager_.SoundPlay("poison.wav");
            break;
        }
        case Sound::Hit: {
            manager_.SoundPlay("poison.wav");
            break;
        }
        case Sound::Poison: {
            manager_.SoundPlay("poison.wav");
            break;
        }
    }
}

void AudioSystem::Receive(const ActionCommandEvent& event) {

    ecs::Entity action = event.action_;
    switch(action.GetComponent<ActionCommand>().Get()->type_) {
        
        case ActionCommandType::Special: {
            manager_.SoundPlay("sounds/poison.wav");
            break;
        }
        case ActionCommandType::Attack: {
            manager_.SoundPlay("sounds/attack.wav");
            break;
        }
        case ActionCommandType::Jump: {
            manager_.SoundPlay("sounds/jump.wav");
            break;
        }
    }
}
