#include <battle/curses.hpp>

#include <components/battle_components.hpp>

void CursesSystem::Configure(ecs::EntityManager& , ecs::EventManager& events) {
    events.Subscribe<PlayerInitiatedEvent>(*this);

    events.Subscribe<ActiveCurseEvent>(*this);
    events.Subscribe<PassiveCurseEvent>(*this);
}

void CursesSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta dt) {
    entities.Each<ActiveCursesStorage>([dt](ecs::Entity entity, ActiveCursesStorage& storage) {
        std::vector<std::list<ActiveCurseInfo>::iterator> delete_candidates;

        for (auto it = storage.storage_.begin(); it != storage.storage_.end(); it++) {
            it->periodic_action_(entity);

            it->time_left_ -= dt;
            if (it->time_left_ <= 0) {
                delete_candidates.push_back(it);
            }
        }

        for (auto& it : delete_candidates) {
            storage.storage_.erase(it);
        }
    });

    entities.Each<PassiveCursesStorage>([dt](ecs::Entity entity, PassiveCursesStorage& storage) {
        std::vector<std::list<PassiveCurseInfo>::iterator> delete_candidates;

        for (auto it = storage.storage_.begin(); it != storage.storage_.end(); it++) {
            it->time_left_ -= dt;

            if (it->time_left_ <= 0) {
                it->remove_action_(entity);
                delete_candidates.push_back(it);
            }
        }

        for (auto& it : delete_candidates) {
            storage.storage_.erase(it);
        }
    });
}

void CursesSystem::Receive(const PlayerInitiatedEvent& event) {
    ecs::Entity player = event.entity_;

    player.Assign<PassiveCursesStorage>(PassiveCursesStorage{});
    player.Assign<ActiveCursesStorage>(ActiveCursesStorage{});
}

void CursesSystem::Receive(const ActiveCurseEvent& event) {
    if (event.entity_.HasComponent<ActiveCursesStorage>()) {
        ecs::Entity cursed = event.entity_;
        auto curse_storage = cursed.GetComponent<ActiveCursesStorage>();

        curse_storage->storage_.emplace_back(std::move(event.curse_));
    }
}

void CursesSystem::Receive(const PassiveCurseEvent& event) {
    if (event.entity_.HasComponent<PassiveCursesStorage>()) {
        ecs::Entity cursed = event.entity_;
        auto curse_storage = cursed.GetComponent<PassiveCursesStorage>();

        curse_storage->storage_.emplace_back(std::move(event.curse_));
    }
}
