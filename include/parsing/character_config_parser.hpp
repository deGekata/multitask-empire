#ifndef CHARACTER_CONFIG_PARSER_H
#define CHARACTER_CONFIG_PARSER_H

#include <parsing/abstract_parser.hpp>

class CharacterConfigParser : public AbstractParser {
public:
    bool Parse(const std::string& config_path, ecs::EntityManager* entities);

private:
    RET_CODE GetArg(const std::string& str, std::string& storage);
};

#endif // CHARACTER_CONFIG_PARSER_H
