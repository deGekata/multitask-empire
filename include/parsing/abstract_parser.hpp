#ifndef ABSTRACT_PARSER_H
#define ABSTRACT_PARSER_H

#include <ecs/entity.hpp>

#include <string>

class AbstractParser {
public:

    virtual ~AbstractParser();
    virtual bool Parse(const std::string& file_path, ecs::EntityManager* entities) = 0;

    enum class RET_CODE {
        PARSED,
        PARSE_END,
        REACHED_BUFFER_END,
        WRONG_PATTERN
    };

protected:
    RET_CODE    Skip(const std::string& str);
    
    void        SkipSpaces();
    RET_CODE    SkipUntilSymb(char symb);

    void        ShowFailedParseWarning(RET_CODE res);

    void        InitParser(const std::string& path);
    void        CloseParser();
    
protected:
    // todo: to iterator
    uint col_, line_;
    
    std::string                 buffer_;

    std::string::iterator       buf_iter_;
    std::string::const_iterator buf_end_;
};

#endif // ABSTRACT_PARSER_H
