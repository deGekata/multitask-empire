#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include <ecs/entity.hpp>
#include <ecs/system.hpp>

#include "../../audio/manage/audio_manager.hpp"

#include <events/audio_events.hpp>
#include <events/action_events.hpp>

class AudioSystem : public ecs::System<AudioSystem>, public ecs::Receiver<AudioSystem> {
public:

    AudioSystem();

    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const PlaySoundEvent& event);
    void Receive(const ActionCommandEvent& event);

private:
    audio::manage::AudioManager manager_;
};

#endif // AUDIO_SYSTEM_H
