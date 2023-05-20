#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>
#include <ecs/entity.hpp>
#include <components/graphic_components.hpp>

#include <parsing/abstract_parser.hpp>

class XmlParser : public AbstractParser {
public:

    bool Parse(const std::string& xml_path, ecs::EntityManager* entities) override;

private:
    
    RET_CODE ParseTitle();
    RET_CODE ParseTexture(const std::string& xml_location, SpriteSheet* component);
    RET_CODE ParseSubTexture(SpriteSheet* component);

private:
    // parses such types: version="1.0"
    RET_CODE GetArg(const std::string& str, std::string& storage);
    RET_CODE SkipArg(const std::string& str);
};

#endif // XML_PARSER_H
