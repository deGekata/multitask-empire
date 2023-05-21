#include <parsing/character_config_parser.hpp>
#include <logger/components_formatting.hpp>
using RetCode = typename AbstractParser::RET_CODE;

/*
    getInt, getDouble, getStr, checkName,
*/
#define ADD_ATTR_MATCH(attr) \
    str_to_attr_[#attr] = CharacterConfigParser::CharacterAttribute::attr;

#define ADD_KB_MATCH(kb_key) \
    str_to_kb_key_[#kb_key] = igraphicslib::KeyboardKey::kb_key;

CharacterConfigParser::CharacterConfigParser() {
    ADD_ATTR_MATCH(Name)
    ADD_ATTR_MATCH(SpriteSheetPath)
    ADD_ATTR_MATCH(XmlPath)
    ADD_ATTR_MATCH(HpLvl)
    ADD_ATTR_MATCH(JmpRatio)
    ADD_ATTR_MATCH(SpeedRatio)
    ADD_ATTR_MATCH(BasicFrames)
    ADD_ATTR_MATCH(Replics)
    ADD_ATTR_MATCH(Attacks)

    ADD_KB_MATCH(H)
    ADD_KB_MATCH(J)
    ADD_KB_MATCH(K)
    ADD_KB_MATCH(L)
}

// to avoid copypaste
#define CHECK(call)                       \
    if ((res = call) != RetCode::PARSED) \
        return res;

RetCode CharacterConfigParser::GetInt(uint& val, char close_symb) {
    RetCode res = RetCode::PARSED;

    std::string::iterator tmp_iter = buf_iter_;
    CHECK(SkipUntilSymb(close_symb))

    val = static_cast<uint>(std::stoi(std::string(tmp_iter, buf_iter_)));

    return RetCode::PARSED;
}

RetCode CharacterConfigParser::GetDouble(double& val, char close_symb) {
    RetCode res = RetCode::PARSED;

    std::string::iterator tmp_iter = buf_iter_;
    CHECK(SkipUntilSymb(close_symb))

    val = static_cast<double>(std::stod(std::string(tmp_iter, buf_iter_)));

    return RetCode::PARSED;
}

RetCode CharacterConfigParser::GetStr(std::string& str) {
    RetCode res = RetCode::PARSED;
    CHECK(Skip("\""))

    std::string::iterator tmp_iter = buf_iter_;
    CHECK(SkipUntilSymb('\"'))

    str = std::string(tmp_iter, buf_iter_);

    CHECK(Skip("\""))

    return RetCode::PARSED;
}

RetCode CharacterConfigParser::GetAttrName(std::string& str) {
    RetCode res = RetCode::PARSED;

    std::string::iterator tmp_iter = buf_iter_;
    CHECK(SkipUntilSymb(':'))

    str = std::string(tmp_iter, buf_iter_);

    CHECK(Skip(":"))

    return RetCode::PARSED;
}


RetCode CharacterConfigParser::SkipAttr(const std::string& str) {
    RetCode res = RetCode::PARSED;

    CHECK(SkipUntilSymb(':'))
    CHECK(Skip(":"))

    return RetCode::PARSED;
} 

RetCode CharacterConfigParser::GetFrames(BattleAbleAttributes::basicFrames& basic_frames) {
    RetCode res = RetCode::PARSED;

    CHECK(Skip("{"))

    CHECK(SkipAttr("Run"))
    CHECK(GetStr(basic_frames.run_))
    CHECK(Skip(","))

    CHECK(SkipAttr("Idle"))
    CHECK(GetStr(basic_frames.idle_))
    CHECK(Skip(","))

    CHECK(SkipAttr("Death"))
    CHECK(GetStr(basic_frames.death_))
    CHECK(Skip(","))

    CHECK(SkipAttr("Jump"))
    CHECK(GetStr(basic_frames.jump_))

    CHECK(Skip("}"))

    return RetCode::PARSED;
}

RetCode CharacterConfigParser::GetReplics(std::vector<std::string>& replics) {
    RetCode res = RetCode::PARSED;

    CHECK(Skip("{"))
    if(*buf_iter_ == '}') {
        CHECK(Skip("}"))
        return RetCode::PARSED;
    }
            
    while(true) {
        if(*buf_iter_ == '}')
            break;

        std::string new_replic;
        CHECK(GetStr(new_replic))
        replics.push_back(new_replic);

        if(*buf_iter_ == '}')
            break;
        CHECK(Skip(","))
    }

    CHECK(Skip("}"))

    return RetCode::PARSED;
}

RetCode CharacterConfigParser::GetAttacks(std::vector<BattleAbleAttributes::attackData>& attacks) {
    RetCode res = RetCode::PARSED;

    CHECK(Skip("{"))
    if(*buf_iter_ == '}') {
        CHECK(Skip("}"))
        return RetCode::PARSED;
    }

    while(true) {
        CHECK(Skip("{"))
        BattleAbleAttributes::attackData data;

        CHECK(GetStr(data.xml_name_))
        CHECK(Skip(","))

        CHECK(GetStr(data.name_))
        CHECK(Skip(","))

        std::string key_str;
        CHECK(GetStr(key_str))
        
        if(str_to_kb_key_.count(key_str) == 0) {
            logger::Print(kError, "key {} is not reserved for attack moves\n", key_str);
        }

        data.binded_key_ = str_to_kb_key_[key_str];

        CHECK(Skip(","))

        CHECK(GetInt(data.damage_value_, '}'))

        attacks.push_back(data);

        CHECK(Skip("}"))
        if(*buf_iter_ == '}')
            break;
        CHECK(Skip(","))
    }

    CHECK(Skip("}"))

    return RetCode::PARSED;
}

#define RESET_MARKER_MATCH(attr) \
    parsed_attrs_marker_[CharacterConfigParser::CharacterAttribute::attr] = 0;

void CharacterConfigParser::ResetMarker() {
    RESET_MARKER_MATCH(Name)
    RESET_MARKER_MATCH(SpriteSheetPath)
    RESET_MARKER_MATCH(XmlPath)
    RESET_MARKER_MATCH(HpLvl)
    RESET_MARKER_MATCH(JmpRatio)
    RESET_MARKER_MATCH(SpeedRatio)
    RESET_MARKER_MATCH(BasicFrames)
    RESET_MARKER_MATCH(Replics)
    RESET_MARKER_MATCH(Attacks)
}

RetCode CharacterConfigParser::ParseBody(const std::string& config_path, ecs::EntityManager* entities) {
    
    uint n_parsed_attrs = 0;

    BattleAbleAttributes attrs;
    ResetMarker();

    RetCode res = RetCode::PARSED;

    while(buf_iter_ < buf_end_ && n_parsed_attrs < static_cast<uint>(CharacterAttribute::NAttrs)) {
        std::string attr_name;

        res = GetAttrName(attr_name);

        if(res != RetCode::PARSED) {
            ShowFailedParseWarning(res);
        }

        auto attr_id = str_to_attr_[attr_name];
        if(parsed_attrs_marker_[attr_id]) {
            logger::Print(kError, "attribute {} is duplicated\n", attr_name);
        }

        switch(attr_id) {
            case CharacterAttribute::Name:{
                CHECK(GetStr(attrs.name_))
                break;
            }
            case CharacterAttribute::SpriteSheetPath:{
                CHECK(GetStr(attrs.sprite_sheet_path_))
                break;
            }
            case CharacterAttribute::XmlPath:{
                CHECK(GetStr(attrs.xml_path_))
                break;
            }
            case CharacterAttribute::HpLvl:{
                CHECK(GetInt(attrs.hp_lvl_, ','))
                break;
            }
            case CharacterAttribute::JmpRatio:{
                CHECK(GetDouble(attrs.jmp_ratio_, ','))
                break;
            }
            case CharacterAttribute::SpeedRatio:{
                CHECK(GetDouble(attrs.speed_ratio_, ','))
                break;
            }
            case CharacterAttribute::BasicFrames:{
                CHECK(GetFrames(attrs.basic_frames_))
                break;
            }
            case CharacterAttribute::Replics:{
                CHECK(GetReplics(attrs.replics_))
                break;
            }
            case CharacterAttribute::Attacks:{
                CHECK(GetAttacks(attrs.attacks_))
                break;
            }
            default:{
                logger::Print(kError, "not implemented\n");
                break;
            }
        }

        if(buf_iter_ < buf_end_)
            CHECK(Skip(","))
        n_parsed_attrs++;
    }

    attrs.config_path_ = config_path;
    
    if(res == RetCode::PARSE_END || res == RetCode::PARSED){
        ecs::Entity storage = entities->Create();
        storage.Assign<BattleAbleAttributesStorageTag>();
        storage.AssignFromCopy(std::move(attrs));
    }

    return RetCode::PARSED;
}
