#ifndef H_AUDIO_EVENTS
#define H_AUDIO_EVENTS

#include <ecs/entity.hpp>

#include <components/player_components.hpp>

enum class Sound {
    Attack,
    Jump,
    Hit,
    Poison
};

struct PlaySoundEvent : public ecs::Event<PlaySoundEvent> {
    explicit PlaySoundEvent(Sound sound_id) : sound_id_(sound_id){
    }

    PlaySoundEvent(const PlaySoundEvent& other) = default;
    ~PlaySoundEvent() override = default;

    Sound sound_id_;
};

#endif // H_ACTION_EVENTS
