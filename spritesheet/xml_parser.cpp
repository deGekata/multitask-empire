#include <spritesheet/xml_parser.hpp>

#include <spritesheet/spritesheet.hpp>
#include <cstring>

using RetCode = typename XmlParser::RET_CODE;

static size_t GetFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    auto file_size = static_cast<size_t>(ftell(file));
    if (file_size == 0) {
        // logger::print(ERROR, "xml file {} size is zero", xml_path);
        assert(0);
    }
    fseek(file, 0, SEEK_SET);

    return file_size;
}

void XmlParser::SkipSpaces() {
    while (buf_iter_ < buf_end_ && isspace(*buf_iter_)) {
        if (*buf_iter_ == '\n') {
            line_++;
            col_ = 0;
        } else if (*buf_iter_ == '\t') {
            col_ += 4;
        } else {
            col_++;
        }
        buf_iter_++;
    }
}

// logger::print(WARNING, "xml file {} got invalid format");

RetCode XmlParser::Skip(const std::string& str) {

    if (buf_iter_ >= buf_end_) {
        return RetCode::REACHED_BUFFER_END;
    }

    size_t str_len = strlen(str.c_str());
    if (buf_end_ - buf_iter_ < static_cast<int>(str_len)) {
        return RetCode::REACHED_BUFFER_END;
    }

    RetCode res = strncmp(&*buf_iter_, str.c_str(), str_len) == 0 ? RetCode::PARSED : RetCode::WRONG_PATTERN;
    if (res == RetCode::WRONG_PATTERN)
        return res;
    buf_iter_ += static_cast<int>(str_len);
    col_ += (str_len);

    SkipSpaces();
    return RetCode::PARSED;
}

RetCode XmlParser::SkipUntilSymb(char symb) {
    while (buf_iter_ < buf_end_ && *buf_iter_ != symb) {
        col_++;
        buf_iter_++;
    }
    if (*buf_iter_ != symb)
        return RetCode::WRONG_PATTERN;

    return RetCode::PARSED;
}

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
    frame.x_ = std::stoi(x_str);
    frame.y_ = std::stoi(y_str);
    frame.w_ = std::stoi(width_str);
    frame.h_ = std::stoi(height_str);

    p_poses->push_back(frame);

    if (*buf_iter_ == '/') {
        CHECK(Skip("/>"))
        return RetCode::PARSED;
    }

    CHECK(SkipArg("frameX"))
    CHECK(SkipArg("frameY"))
    CHECK(SkipArg("frameWidth"))
    CHECK(SkipArg("frameHeight"))

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

void XmlParser::ShowFailedParseWarning(RetCode res) {
    logger::Print(kWarning, "xml parsing failed on pos<{}, {}> because of the ", col_, line_);
    if (res == RetCode::WRONG_PATTERN) {
        logger::Print("wrong pattern\n");
    } else if (res == RetCode::REACHED_BUFFER_END) {
        logger::Print("end of the buffer has been reached\n");
    }
}

bool XmlParser::Parse(const std::string& xml_path, ecs::Entity entity) {
    // todo: to ostream

    FILE* xml_file = fopen(xml_path.c_str(), "r");
    if (xml_file == nullptr) {
        logger::Print(kError, "unable to open xml file {}", xml_path);
        assert(0);
    }

    std::string buffer(GetFileSize(xml_file), 0);

    fread(reinterpret_cast<void*>(const_cast<char*>(buffer.c_str())), sizeof(char), buffer.size(), xml_file);

    buf_iter_ = buffer.begin();
    buf_end_ = buffer.end();

    col_ = 0;
    line_ = 0;

    RetCode res = ParseTitle();

    if (res != RetCode::PARSED) {
        ShowFailedParseWarning(res);
        return false;
    }

    SpriteSheet component;

    while ((res = ParseTexture(GetPathStr(xml_path), &component)) == RetCode::PARSED) {
        entity.AssignFromCopy(std::move(component));
    }

    if (res == RetCode::PARSE_END) {
        entity.AssignFromCopy(std::move(component));
    } else {
        ShowFailedParseWarning(res);
        return false;
    }
    return true;
}
