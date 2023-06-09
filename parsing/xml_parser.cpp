#include <parsing/xml_parser.hpp>

#include <cstring>

using RetCode = typename AbstractParser::RET_CODE;

// to avoid copypaste
#define CHECK(call)                       \
    if ((res = call) != RetCode::PARSED) \
        return res;

RetCode XmlParser::GetArg(const std::string& name, std::string& storage) {

    RetCode res = RetCode::PARSED;

    CHECK(Skip(name))
    CHECK(Skip("="))
    CHECK(Skip("\""))

    std::string::iterator tmp_iter = buf_iter_;
    CHECK(SkipUntilSymb('\"'))

    storage = std::string(tmp_iter, buf_iter_);

    CHECK(Skip("\""))

    return RetCode::PARSED;
}

RetCode XmlParser::SkipArg(const std::string& name) {

    RetCode res = RetCode::PARSED;

    CHECK(Skip(name))
    CHECK(Skip("="))
    CHECK(Skip("\""))
    CHECK(SkipUntilSymb('\"'))
    CHECK(Skip("\""))

    return RetCode::PARSED;
}

RetCode XmlParser::ParseTitle() {
    RetCode res = RetCode::PARSED;
    CHECK(Skip("<?xml"))

    std::string ver_str;
    CHECK(GetArg("version", ver_str))
    if (ver_str != "1.0"){
        return RetCode::WRONG_PATTERN;    
    }

    if (*buf_iter_ == '?') {
        CHECK(Skip("?>"))
        return RetCode::PARSED;
    }

    // std::string encoding_str;
    CHECK(SkipArg("encoding"))
    // if(encoding_str != "UTF-8") return RetCode::WRONG_PATTERN;

    CHECK(Skip("/>"))
    return RetCode::PARSED;
}

static std::string GetStateName(const std::string& state_name_with_num) {

    std::string::const_iterator iter;
    for (iter = state_name_with_num.begin(); iter < state_name_with_num.end() && !isdigit(*iter); iter++) {
    }

    if (iter == state_name_with_num.end())
        return state_name_with_num;

    return {state_name_with_num.begin(), iter};
}

static std::string GetPathStr(const std::string& file_path_str) {

    std::string::const_iterator iter;
    std::string::const_iterator symb_pos = file_path_str.end();

    for(iter = file_path_str.cbegin(); iter < file_path_str.cend(); iter++) {
        if(*iter == '/'){
            symb_pos = iter;
        }
    }

    if(symb_pos == file_path_str.end()) {
        return "./";
    }
    return {file_path_str.begin(), symb_pos + 1};

    // std::string::const_reverse_iterator iter_end;
    // for (iter_end = file_path_str.rbegin(); iter_end < file_path_str.rend() && *iter_end != '/'; iter_end++) {
    // }

    // if(iter_end == file_path_str.rend()) {
    //     return "./";
    // }
}

RetCode XmlParser::ParseSubTexture(SpriteSheet* component) {
    RetCode res = RetCode::PARSED;

    CHECK(Skip("<"))

    if (*buf_iter_ == '/') {
        CHECK(Skip("/TextureAtlas"))
        CHECK(Skip(">"))
        return RetCode::PARSE_END;
    }

    CHECK(Skip("SubTexture"))

    std::string state_name_with_num;
    CHECK(GetArg("name", state_name_with_num))

    std::string state_name = GetStateName(state_name_with_num);

    // найти StateData с нейм state_name и добавить координаты или создать новый
    auto is_state_present = [&state_name](typename SpriteSheet::StateData& data) { return data.name_ == state_name; };

    auto iter = std::find_if(component->states_.begin(), component->states_.end(), is_state_present);

    std::vector<SpriteSheet::StateFrame>* p_poses = nullptr;
    if (iter != component->states_.end()) {
        p_poses = &iter->positions_;
    } else {
        typename SpriteSheet::StateData state_data;
        state_data.name_ = state_name;
        component->states_.push_back(state_data);
        p_poses = &component->states_[component->states_.size() - 1].positions_;
    }

    std::string x_str, y_str, width_str, height_str;

    CHECK(GetArg("x", x_str))
    CHECK(GetArg("y", y_str))
    CHECK(GetArg("width", width_str))
    CHECK(GetArg("height", height_str))

    SpriteSheet::StateFrame frame;
    frame.x_ = static_cast<uint>(std::stoi(x_str));
    frame.y_ = static_cast<uint>(std::stoi(y_str));
    frame.w_ = static_cast<uint>(std::stoi(width_str));
    frame.h_ = static_cast<uint>(std::stoi(height_str));

    if (*buf_iter_ == '/') {
        p_poses->push_back(frame);
        CHECK(Skip("/>"))
        return RetCode::PARSED;
    }

    std::string frameX_str, frameY_str, frameW_str, frameH_str;

    CHECK(GetArg("frameX", frameX_str))
    CHECK(GetArg("frameY", frameY_str))
    CHECK(GetArg("frameWidth", frameW_str))
    CHECK(GetArg("frameHeight", frameH_str))

    frame.x_offset_ = (std::stoi(frameX_str));
    frame.y_offset_ = (std::stoi(frameY_str));
    frame.frame_w_  = static_cast<uint>(std::stoi(frameW_str));
    frame.frame_h_  = static_cast<uint>(std::stoi(frameH_str));

    p_poses->push_back(frame);
    CHECK(Skip("/>"))

    return RetCode::PARSED;
}

RetCode XmlParser::ParseTexture(const std::string& xml_location, SpriteSheet* component) {
    RetCode res = RetCode::PARSED;

    CHECK(Skip("<"))

    CHECK(Skip("TextureAtlas"))

    std::string image_path_str;

    CHECK(GetArg("imagePath", image_path_str))
    component->img_path_ = xml_location + image_path_str;

    CHECK(Skip(">"))

    while ((res = ParseSubTexture(component)) == RetCode::PARSED) {
    }
    return res;
}

RetCode XmlParser::ParseBody(const std::string& xml_path, ecs::EntityManager* entities) {

    RetCode res = ParseTitle();

    if (res != RetCode::PARSED) {
        return res;
    }

    SpriteSheet component;

    // todo: refactor(error handling)
    do {
        res = ParseTexture(GetPathStr(xml_path), &component);

        if(res != RET_CODE::PARSED && res != RET_CODE::PARSE_END) {
            ShowFailedParseWarning(res);
            return res;       
        }

        component.texture_ = igraphicslib::Texture(component.img_path_.c_str());
        component.sprite_.SetTexture(igraphicslib::Texture(component.img_path_.c_str()));
        // if(entity.HasComponent<SpriteSheet>()) {
        //     auto storage = entities->GetEntitiesWithComponents<ObjectAnimationData>();
        //     SpriteSheet* p_spritesheet = entity.GetComponent<SpriteSheet>().Get();
            
        //     for(auto entity_iter = storage.begin(); entity_iter != storage.end(); entity_iter.operator++()) {
        //         if((*entity_iter).GetComponent<ObjectAnimationData>().Get()->sprite_sheet_ == p_spritesheet) {

        //             (*entity_iter).Remove<ObjectAnimationData>();
        //         }
        //     }
        //     entity.Remove<SpriteSheet>();
        // }
        
        ecs::Entity spritesheet_storage = entities->Create();
        spritesheet_storage.Assign<SpriteSheetStorageTag>();

        spritesheet_storage.AssignFromCopy(std::move(component));

    } while(res != RetCode::PARSE_END);

    return RET_CODE::PARSED;
}
