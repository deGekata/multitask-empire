#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>
#include <ecs/entity.hpp>
#include <components/graphic_components.hpp>

class XmlParser {
public:

    bool Parse(const std::string& xml_path, ecs::EntityManager* entities);

    enum class RET_CODE {
        PARSED,
        PARSE_END,
        REACHED_BUFFER_END,
        WRONG_PATTERN
    };
private:
    
    RET_CODE ParseTitle();
    RET_CODE ParseTexture(const std::string& xml_location, SpriteSheet* component);
    RET_CODE ParseSubTexture(SpriteSheet* component);

private:
    RET_CODE Skip(const std::string& str);

    // parses such types: version="1.0"
    RET_CODE GetArg(const std::string& str, std::string& storage);
    RET_CODE SkipArg(const std::string& str);
    
    void SkipSpaces();
    RET_CODE SkipUntilSymb(char symb);

    void ShowFailedParseWarning(RET_CODE res);
private:
    // todo: to iterator
    uint col_, line_;
    
    std::string::iterator       buf_iter_;
    std::string::const_iterator buf_end_;

    // SpriteSheet* cur_component_;
};

#endif // XML_PARSER_H
