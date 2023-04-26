// /**
//  * This is an example of using EntityX.
//  *
//  * It is an SFML2 application that spawns 100 random circles on a 2D plane
//  * moving in random directions. If two circles collide they will explode and
//  * emit particles.
//  *
//  * This illustrates a bunch of EC/EntityX concepts:
//  *
//  * - Separation of data via components.
//  * - Separation of logic via systems.
//  * - Use of events (colliding bodies trigger a CollisionEvent).
//  *
//  * Compile with:
//  *
//  *    c++ -I.. -O3 -std=c++11 -Wall -lsfml-system -lsfml-window -lsfml-graphics -lentityx example.cc -o example
//  */
// #include <cmath>
// #include <unordered_set>
// #include <sstream>
// #include <cstdlib>
// #include <memory>
// #include <string>
// #include <vector>
// #include <iostream>

// #include <ecs/quick.hpp>

// #include <SFML/Window.hpp>
// #include <SFML/Graphics.hpp>

// using std::cerr;
// using std::cout;
// using std::endl;

// float r(int a, float b = 0) {
//     return static_cast<float>(static_cast<float>(std::rand() % (a * 1000)) + b * 1000) / static_cast<float>(1000.0);
// }

// struct Body {
//     Body(const sf::Vector2f &position, const sf::Vector2f &direction, float rotationd = 0.0)
//         : position_(position), direction_(direction), rotationd_(rotationd), alpha_(0.0) {
//     }

//     sf::Vector2f position_;
//     sf::Vector2f direction_;
//     float rotation_ = 0.0, rotationd_, alpha_;
// };

// using Renderable = std::shared_ptr<sf::Shape>;

// struct Particle {
//     explicit Particle(sf::Color colour, float radius, float duration)
//         : colour_(colour), radius_(radius), alpha_(colour.a), d_(colour.a / duration) {
//     }

//     sf::Color colour_;
//     float radius_, alpha_, d_;
// };

// struct Collideable {
//     explicit Collideable(float radius) : radius_(radius) {
//     }

//     float radius_;
// };

// // Emitted when two entities collide.
// struct CollisionEvent {
//     CollisionEvent(ecs::Entity left, ecs::Entity right) : left_(left), right_(right) {
//     }

//     ecs::Entity left_, right_;
// };

// class SpawnSystem : public ecs::System<SpawnSystem> {
// public:
//     explicit SpawnSystem(sf::RenderTarget &target, int count) : size_(target.getSize()), count_(count) {
//     }

//     void Update(ecs::EntityManager &es, ecs::EventManager &, ecs::TimeDelta) override {
//         int c = 0;
//         ecs::ComponentHandle<Collideable> collideable;
//         es.Each<Collideable>([&](ecs::Entity, Collideable &) { ++c; });

//         for (int i = 0; i < count_ - c; i++) {
//             ecs::Entity entity = es.Create();

//             // Mark as collideable (explosion particles will not be collideable).
//             collideable = entity.Assign<Collideable>(r(10, 5));

//             // "Physical" attributes.
//             entity.Assign<Body>(sf::Vector2f(r(static_cast<int>(size_.x)), r(static_cast<int>(size_.y))),
//                                 sf::Vector2f(r(100, -50), r(100, -50)));

//             // Shape to apply to entity.
//             Renderable shape(new sf::CircleShape(collideable->radius_));
//             shape->setFillColor(sf::Color(static_cast<unsigned char>(r(128, 127)),
//                                           static_cast<unsigned char>(r(128, 127)),
//                                           static_cast<unsigned char>(r(128, 127)), 0));
//             shape->setOrigin(collideable->radius_, collideable->radius_);
//             entity.Assign<Renderable>(shape);
//         }
//     }

// private:
//     sf::Vector2u size_;
//     int count_;
// };

// // Updates a body's position and rotation.
// struct BodySystem : public ecs::System<BodySystem> {
//     void Update(ecs::EntityManager &es, ecs::EventManager &, ecs::TimeDelta dt) override {
//         const float fdt = static_cast<float>(dt);
//         es.Each<Body>([fdt](ecs::Entity, Body &body) {
//             body.position_ += body.direction_ * fdt;
//             body.rotation_ += body.rotation_ * fdt;
//             body.alpha_ = std::min(1.0f, body.alpha_ + fdt);
//         });
//     }
// };

// // Bounce bodies off the edge of the screen.
// class BounceSystem : public ecs::System<BounceSystem> {
// public:
//     explicit BounceSystem(sf::RenderTarget &target) : size(target.getSize()) {
//     }

//     void Update(ecs::EntityManager &es, ecs::EventManager &, ecs::TimeDelta) override {
//         es.Each<Body>([this](ecs::Entity, Body &body) {
//             if (body.position_.x + body.direction_.x < 0 ||
//                 body.position_.x + body.direction_.x >= static_cast<float>(size.x))
//                 body.direction_.x = -body.direction_.x;
//             if (body.position_.y + body.direction_.y < 0 ||
//                 body.position_.y + body.direction_.y >= static_cast<float>(size.y))
//                 body.direction_.y = -body.direction_.y;
//         });
//     }

// private:
//     sf::Vector2u size;
// };

// // Determines if two Collideable bodies have collided. If they have it emits a
// // CollisionEvent. This is used by ExplosionSystem to create explosion
// // particles, but it could be used by a SoundSystem to play an explosion
// // sound, etc..
// //
// // Uses a fairly rudimentary 2D partition system, but performs reasonably well.
// class CollisionSystem : public ecs::System<CollisionSystem> {
//     static const int PARTITIONS = 200;

//     struct Candidate {
//         sf::Vector2f position;
//         float radius;
//         ecs::Entity entity;
//     };

// public:
//     explicit CollisionSystem(sf::RenderTarget &target) : size(target.getSize()) {
//         size.x = size.x / PARTITIONS + 1;
//         size.y = size.y / PARTITIONS + 1;
//     }

//     void Update(ecs::EntityManager &es, ecs::EventManager &events, ecs::TimeDelta) override {
//         reset();
//         collect(es);
//         collide(events);
//     }

// private:
//     std::vector<std::vector<Candidate>> grid;
//     sf::Vector2u size;

//     void reset() {
//         grid.clear();
//         grid.resize(size.x * size.y);
//     }

//     void collect(ecs::EntityManager &entities) {
//         entities.Each<Body, Collideable>([this](ecs::Entity entity, Body &body, Collideable &collideable) {
//             unsigned int left = static_cast<unsigned int>(static_cast<int>(body.position_.x - collideable.radius_) /
//                                                           PARTITIONS),
//                          top = static_cast<unsigned int>(static_cast<int>(body.position_.y - collideable.radius_) /
//                                                          PARTITIONS),
//                          right = static_cast<unsigned int>(static_cast<int>(body.position_.x + collideable.radius_) /
//                                                            PARTITIONS),
//                          bottom = static_cast<unsigned int>(static_cast<int>(body.position_.y + collideable.radius_) /
//                                                             PARTITIONS);
//             Candidate candidate{body.position_, collideable.radius_, entity};
//             unsigned int slots[4] = {
//                 left + top * size.x,
//                 right + top * size.x,
//                 left + bottom * size.x,
//                 right + bottom * size.x,
//             };
//             grid[slots[0]].push_back(candidate);
//             if (slots[0] != slots[1])
//                 grid[slots[1]].push_back(candidate);
//             if (slots[1] != slots[2])
//                 grid[slots[2]].push_back(candidate);
//             if (slots[2] != slots[3])
//                 grid[slots[3]].push_back(candidate);
//         });
//     }

//     void collide(ecs::EventManager &events) {
//         for (const std::vector<Candidate> &candidates : grid) {
//             for (const Candidate &left : candidates) {
//                 for (const Candidate &right : candidates) {
//                     if (left.entity == right.entity)
//                         continue;
//                     if (collided(left, right))
//                         events.Emit<CollisionEvent>(left.entity, right.entity);
//                 }
//             }
//         }
//     }

//     float length(const sf::Vector2f &v) {
//         return std::sqrt(v.x * v.x + v.y * v.y);
//     }

//     bool collided(const Candidate &left, const Candidate &right) {
//         return length(left.position - right.position) < left.radius + right.radius;
//     }
// };

// // Fade out and then remove particles.
// class ParticleSystem : public ecs::System<ParticleSystem> {
// public:
//     void Update(ecs::EntityManager &es, ecs::EventManager &, ecs::TimeDelta dt) override {
//         es.Each<Particle>([dt](ecs::Entity entity, Particle &particle) {
//             particle.alpha_ -= particle.d_ * dt;
//             if (particle.alpha_ <= 0) {
//                 entity.Destroy();
//             } else {
//                 particle.colour_.a = static_cast<unsigned char>(particle.alpha_);
//             }
//         });
//     }
// };

// // Renders all explosion particles efficiently as a quad vertex array.
// class ParticleRenderSystem : public ecs::System<ParticleRenderSystem> {
// public:
//     explicit ParticleRenderSystem(sf::RenderTarget &target) : target_(target) {
//     }

//     void Update(ecs::EntityManager &es, ecs::EventManager &, ecs::TimeDelta) override {
//         sf::VertexArray vertices(sf::Quads);
//         es.Each<Particle, Body>([&vertices](ecs::Entity, Particle &particle, Body &body) {
//             const float r = particle.radius_;
//             // Spin the particles.
//             sf::Transform transform;
//             transform.rotate(body.rotation_);
//             vertices.append(
//                 sf::Vertex(body.position_ + transform.transformPoint(sf::Vector2f(-r, -r)), particle.colour_));
//             vertices.append(
//                 sf::Vertex(body.position_ + transform.transformPoint(sf::Vector2f(r, -r)), particle.colour_));
//             vertices.append(
//                 sf::Vertex(body.position_ + transform.transformPoint(sf::Vector2f(r, r)), particle.colour_));
//             vertices.append(
//                 sf::Vertex(body.position_ + transform.transformPoint(sf::Vector2f(-r, r)), particle.colour_));
//         });
//         target_.draw(vertices);
//     }

// private:
//     sf::RenderTarget &target_;
// };

// // For any two colliding bodies, destroys the bodies and emits a bunch of bodgy explosion particles.
// class ExplosionSystem : public ecs::System<ExplosionSystem>, public ecs::Reciever<ExplosionSystem> {
// public:
//     void Configure(ecs::EntityManager &, ecs::EventManager &events) override {
//         events.Subscribe<CollisionEvent>(*this);
//     }

//     void Update(ecs::EntityManager &es, ecs::EventManager &, ecs::TimeDelta) override {
//         for (ecs::Entity entity : collided) {
//             emit_particles(es, entity);
//             entity.Destroy();
//         }
//         collided.clear();
//     }

//     void emit_particles(ecs::EntityManager &es, ecs::Entity entity) {
//         ecs::ComponentHandle<Body> body = entity.GetComponent<Body>();
//         ecs::ComponentHandle<Renderable> renderable = entity.GetComponent<Renderable>();
//         ecs::ComponentHandle<Collideable> collideable = entity.GetComponent<Collideable>();
//         sf::Color colour = (*renderable)->getFillColor();
//         colour.a = 200;

//         float area = (static_cast<float>(M_PI) * collideable->radius_ * collideable->radius_) / static_cast<float>(3.0);
//         for (int i = 0; i < static_cast<int>(area); i++) {
//             ecs::Entity particle = es.Create();

//             float rotationd = r(720, 180);
//             if (std::rand() % 2 == 0)
//                 rotationd = -rotationd;

//             float offset = r(static_cast<int>(collideable->radius_), 1);
//             float angle = r(360) * static_cast<float>(M_PI) / static_cast<float>(180.0);
//             particle.Assign<Body>(
//                 body->position_ + sf::Vector2f(offset * static_cast<float>(cos(static_cast<double>(angle))),
//                                                offset * static_cast<float>(sin(static_cast<double>(angle)))),
//                 body->direction_ + sf::Vector2f(offset * 2 * static_cast<float>(cos(static_cast<double>(angle))),
//                                                 offset * 2 * static_cast<float>(sin(static_cast<double>(angle)))),
//                 rotationd);

//             float radius = r(3, 1);
//             particle.Assign<Particle>(colour, radius, radius / 2);
//         }
//     }

//     void Recieve(const CollisionEvent &collision) {
//         // Events are immutable, so we can't destroy the entities here. We defer
//         // the work until the update loop.
//         collided.insert(collision.left_);
//         collided.insert(collision.right_);
//     }

// private:
//     std::unordered_set<ecs::Entity> collided;
// };

// // Render all Renderable entities and draw some informational text.
// class RenderSystem : public ecs::System<RenderSystem> {
// public:
//     explicit RenderSystem(sf::RenderTarget &target, sf::Font &font) : target_(target) {
//         text.setFont(font);
//         text.setPosition(sf::Vector2f(2, 2));
//         text.setCharacterSize(18);
//         text.setColor(sf::Color::White);
//     }

//     void Update(ecs::EntityManager &es, ecs::EventManager &, ecs::TimeDelta dt) override {
//         es.Each<Body, Renderable>([this](ecs::Entity, Body &body, Renderable &renderable) {
//             sf::Color fillColor = renderable->getFillColor();
//             fillColor.a = sf::Uint8(body.alpha_ * 255);
//             renderable->setFillColor(fillColor);
//             renderable->setPosition(body.position_);
//             renderable->setRotation(body.rotation_);
//             target_.draw(*renderable.get());
//         });
//         last_update += static_cast<double>(dt);
//         frame_count++;
//         if (last_update >= 0.5) {
//             std::ostringstream out;
//             const double fps = frame_count / last_update;
//             out << es.GetCapacity() << " entities (" << static_cast<int>(fps) << " fps)";
//             text.setString(out.str());
//             last_update = 0.0;
//             frame_count = 0.0;
//         }
//         target_.draw(text);
//     }

// private:
//     double last_update = 0.0;
//     double frame_count = 0.0;
//     sf::RenderTarget &target_;
//     sf::Text text;
// };

// class Application : public ecs::ECS {
// public:
//     explicit Application(sf::RenderTarget &target, sf::Font &font) {
//         systems_.Add<SpawnSystem>(target, 500);
//         systems_.Add<BodySystem>();
//         systems_.Add<BounceSystem>(target);
//         systems_.Add<CollisionSystem>(target);
//         systems_.Add<ExplosionSystem>();
//         systems_.Add<ParticleSystem>();
//         systems_.Add<RenderSystem>(target, font);
//         systems_.Add<ParticleRenderSystem>(target);
//         systems_.Configure();
//     }

//     void update(ecs::TimeDelta dt) {
//         systems_.UpdateAll(dt);
//     }
// };

// int main() {
//     std::srand(static_cast<unsigned int>(std::time(nullptr)));

//     sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "EntityX Example", sf::Style::Fullscreen);
//     sf::Font font;
//     if (!font.loadFromFile("LiberationSans-Regular.ttf")) {
//         cerr << "error: failed to load LiberationSans-Regular.ttf" << endl;
//         return 1;
//     }

//     Application app(window, font);

//     sf::Clock clock;
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             switch (event.type) {
//                 case sf::Event::Closed:
//                 case sf::Event::KeyPressed:
//                     window.close();
//                     break;

//                 default:
//                     break;
//             }
//         }

//         window.clear();
//         sf::Time elapsed = clock.restart();
//         app.update(elapsed.asSeconds());
//         window.display();
//     }
// }
