#include <physics/movement.hpp>

#include <iostream>

#include <components/movement_components.hpp>

void MovementSystem::Update(ecs::EntityManager& entities, ecs::TimeDelta dt) {
    entities.Each<Velocity, Acceleration>([dt](ecs::Entity entity, Velocity& vel, Acceleration& acc) {
        vel.vx_ += acc.ax_ * dt;
        vel.vy_ += acc.ay_ * dt;

        std::cout << "[*] " << entity.GetId().GetIndex() << " had acceleration " << acc.ax_ << " " << acc.ay_ << std::endl;
        std::cout << "[*] " << entity.GetId().GetIndex() << " now has velocity " << vel.vx_ << " " << vel.vy_ << std::endl; 
    });

    entities.Each<Position, Velocity>([dt](ecs::Entity entity, Position& pos, Velocity& vel) {
        pos.x_ += vel.vx_ * dt;

        // TODO emit landing event
        if ((pos.y_ + vel.vy_ * dt) >= 0) {
            pos.y_ += vel.vy_ * dt;
        }
        // TODO Move to recieve of landing event
        else {
            pos.y_ = 0;
            vel.vy_ = 0;
        }
        
        std::cout << "[*] " << entity.GetId().GetIndex() << " now has coordinates " << pos.x_ << " " << pos.y_ << std::endl; 
    });
}
