#ifndef CHARACTER_CONFIG_PARSER_H
#define CHARACTER_CONFIG_PARSER_H

#include <parsing/abstract_parser.hpp>
#include <components/player_components.hpp>

#include <map>

class CharacterConfigParser : public AbstractParser {
public:
    CharacterConfigParser();

    enum class CharacterAttribute {
        Name,
        SpriteSheetPath,
        XmlPath,
        HpLvl,
        JmpRatio,
        SpeedRatio,
        BasicFrames,
        Replics,
        Attacks,

        NAttrs
        // todo:
        // combos
    };

private:

    RET_CODE ParseBody(const std::string& config_path, ecs::EntityManager* entities) override;

    RET_CODE GetInt(uint& val, char close_symb);
    RET_CODE GetDouble(double& val, char close_symb);
    RET_CODE GetStr(std::string& str);

    RET_CODE GetAttrName(std::string& str);
    RET_CODE SkipAttr(const std::string& str);

    RET_CODE GetFrames(BattleAbleAttributes::basicFrames& basic_frames);
    RET_CODE GetReplics(std::vector<std::string>& replics);
    RET_CODE GetAttacks(std::vector<BattleAbleAttributes::attackData>& attacks);

    void ResetMarker();

private:
    // todo: to static
    std::map<std::string, CharacterAttribute> str_to_attr_;
    std::map<CharacterAttribute, bool> parsed_attrs_marker_;
    std::map<std::string, igraphicslib::KeyboardKey> str_to_kb_key_;
};

#endif // CHARACTER_CONFIG_PARSER_H
