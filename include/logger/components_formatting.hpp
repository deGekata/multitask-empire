#ifndef COMPONENT_FORMATTING_H
#define COMPONENT_FORMATTING_H

#include <components/player_components.hpp>
#include <components/movement_components.hpp>
#include <components/graphic_components.hpp>

#include <fmt/color.h>
#include <fmt/format.h>

// parse is inherited from formatter<string_view>.

template <> struct fmt::formatter<PlayerTag> : formatter<string_view> {

template <typename FormatContext>
    auto format(const PlayerTag& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), fmt::runtime("{}"), fmt::ptr(&p));
    }
};

// template <> struct fmt::formatter<PendingPlayerCommand> : formatter<string_view> {

//     template <typename FormatContext>
//     auto format(const PendingPlayerCommand& p, FormatContext& ctx) const -> decltype(ctx.out()) {
//         return fmt::format_to(ctx.out(), fmt::runtime("{}"), p.command_);
//     }
// };

template <> struct fmt::formatter<Position> : formatter<string_view> {

    template <typename FormatContext>
    auto format(const Position& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), fmt::runtime("(x={}, y={})"), p.x_, p.y_);
    }
};

template <> struct fmt::formatter<Velocity> : formatter<string_view> {

    template <typename FormatContext>
    auto format(const Velocity& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), fmt::runtime("(vx={}, vy={})"), p.vx_, p.vy_);
    }
};

template <> struct fmt::formatter<Acceleration> : formatter<string_view> {

    template <typename FormatContext>
    auto format(const Acceleration& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), fmt::runtime("(ax={}, ay={})"), p.ax_, p.ay_);
    }
};

template <> struct fmt::formatter<SpriteSheet> : formatter<string_view> {

    template <typename FormatContext>
    auto format(const SpriteSheet& p, FormatContext& ctx) const -> decltype(ctx.out()) {
        std::string format_str = fmt::format(fmt::runtime("\n\tpath: {}\n\tstates[\n"), p.img_path_);

        for(size_t n_state = 0; n_state < p.states_.size(); n_state++) {
            format_str +=fmt::format(fmt::runtime("\t\t[{}] {}({})\n"), n_state, p.states_[n_state].name_, p.states_[n_state].positions_.size());
            for(auto pos : p.states_[n_state].positions_) {
                format_str += fmt::format(fmt::runtime("\t\t\t<x({0:4d}), y({0:4d}), w({0:4d}), h({0:4d}), fx({0:4d}), fy({0:4d}), fw({0:4d}), fh({0:4d})>\n"), 
                                                                pos.x_, pos.y_, pos.w_, pos.h_, pos.x_offset_, pos.y_offset_, pos.frame_w_, pos.frame_h_);
            }
            format_str += "\n";
        }
        format_str += "\t]\n";

        return fmt::format_to(ctx.out(), fmt::runtime(format_str));
    }
};

// template <> struct fmt::formatter<PendingMovementCommand>: formatter<string_view> {

//     template <typename FormatContext>
//     auto format(PendingMovementCommand cmd, FormatContext& ctx) const {
//     string_view name = "unknown";
//     switch (cmd){
//         case PendingMovementCommand::Left:   name = "Left"; break;
//         case PendingMovementCommand::Right:  name = "Right"; break;
//         case PendingMovementCommand::Jump:   name = "Jump"; break;
//     }

//         return formatter<string_view>::format(name, ctx);
//     }
// };

#endif // COMPONENT_FORMATTING_H
