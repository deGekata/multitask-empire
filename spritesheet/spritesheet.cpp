#include <spritesheet/spritesheet.hpp>
#include <string.h>

#include <graphics/aTexture.hpp>
// TODO: path make valid
// TODO: fix output of component in logger

// todo: to iterator
static struct xml_position {
    size_t col, line;
} xml_pos;

static void ParseXml(const std::string& xml_path, ecs::Entity entity);

void SpriteSheetSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    // LoadSpriteSheet("default_player_skin.png", "default_player_xml.png");
    events.Subscribe<SpriteSheetLoadRequest, SpriteSheetSystem>(*this);
    events.Subscribe<SkinChangeRequest, SpriteSheetSystem>(*this);
    entities_ = &entities;

    ecs::Entity spritesheet_storage = entities.Create();
    spritesheet_storage.Assign<SpriteSheetStorageTag>();

    LoadSpriteSheet("orc.xml");
}

void SpriteSheetSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void SpriteSheetSystem::LoadSpriteSheet(const std::string& xml_path) {

    entities_->Each<SpriteSheetStorageTag>([&xml_path](ecs::Entity entity, SpriteSheetStorageTag&){
        ParseXml(xml_path, entity);
    });
}

void SpriteSheetSystem::Recieve(const SpriteSheetLoadRequest& event) {
    LoadSpriteSheet(event.xml_path_);
}

void SpriteSheetSystem::Recieve(const SkinChangeRequest& event) {

    bool was_found = false;

    entities_->Each<SpriteSheetStorageTag, SpriteSheet>([this, &event, &was_found](ecs::Entity entity, SpriteSheetStorageTag&, SpriteSheet& sprite_sheet) {
        if(sprite_sheet.img_path_ == event.skin_name_) {
            ObjectAnimationData animation_data = {
                .sprite_sheet_ = entity.GetComponent<SpriteSheet>().Get(),
                .cur_frame_    = 0,
                .n_sprite_sheet_state_ = 0 // todo;
            };
            this->entities_->Assign<ObjectAnimationData>(event.entity_.GetId(), animation_data);
            was_found = true;
        }
    });

    if(!was_found) {
        std::cout << "Warning, unable to find requested sprite sheet: " << event.skin_name_ << "\n";
        return;
    }
}

static size_t GetFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    size_t file_size = static_cast<size_t>(ftell(file));
    if(file_size == 0) {
        // logger::print(ERROR, "xml file {} size is zero", xml_path);
        assert(0);
    }
    fseek(file, 0, SEEK_SET);

    return file_size;
}

#define SKIP_SPACES \
{\
    while(buf_iter < buf_end && isspace(*buf_iter)){\
        if(*buf_iter == '\n') {\
            xml_pos.line++;\
            xml_pos.col = 0;\
        }\
        else if(*buf_iter == '\t') {\
            xml_pos.col += 4;\
        }\
        else{\
            xml_pos.col++;\
        }\
        buf_iter++;\
    }\
}

enum class RET_CODE {
    PARSED,
    PARSE_END,
    REACHED_BUFFER_END,
    WRONG_PATTERN
};
// logger::print(WARNING, "xml file {} got invalid format");

#define SKIP_LIGHT(str)\
{\
    if(buf_iter >= buf_end) {\
        return RET_CODE::REACHED_BUFFER_END;\
    }\
 \
    size_t str_len = strlen(str);\
    if(buf_end - buf_iter < static_cast<int>(str_len)) {\
        return RET_CODE::REACHED_BUFFER_END;\
    }\
 \
    RET_CODE res = strncmp(&*buf_iter, str, str_len) == 0 ? RET_CODE::PARSED : RET_CODE::WRONG_PATTERN; \
    if(res == RET_CODE::WRONG_PATTERN) return res;\
    buf_iter += static_cast<int>(str_len); \
    xml_pos.col += (str_len);\
}

#define SKIP_UNTIL_SYMB(symb)\
{\
    while(buf_iter < buf_end && *buf_iter != symb){\
        xml_pos.col++;\
        buf_iter++;\
    }\
    if(*buf_iter != symb) return RET_CODE::WRONG_PATTERN;\
}

#define SKIP(str)  \
SKIP_LIGHT(str) \
SKIP_SPACES

#define GET_ARG(arg_name, storage_name) \
SKIP(#arg_name) \
SKIP("=")\
SKIP("\"")\
std::string storage_name;\
{\
    std::string::iterator tmp_iter = buf_iter;\
    SKIP_UNTIL_SYMB('\"')\
\
    storage_name = std::string(tmp_iter, buf_iter);\
}\
SKIP("\"")

#define SKIP_ARG(arg_name) \
{\
    SKIP(#arg_name) \
    SKIP("=")\
    SKIP("\"")\
    SKIP_UNTIL_SYMB('\"')\
    SKIP("\"")\
}

static RET_CODE ParseTitle(std::string::iterator& buf_iter, const std::string::iterator& buf_end) {
    SKIP("<?xml")

    GET_ARG(version, ver_str)
    if(ver_str != "1.0") return RET_CODE::WRONG_PATTERN;

    if(*buf_iter == '?'){
        SKIP("?>")
        return RET_CODE::PARSED;
    }

    SKIP_ARG(encoding)
    // if(encoding_str != "UTF-8") return RET_CODE::WRONG_PATTERN;
    
    SKIP("?>")
    return RET_CODE::PARSED;
}   

std::string GetStateName(const std::string& state_name_with_idx) {
    for(auto iter = state_name_with_idx.begin(); iter < state_name_with_idx.end(); iter++) {
        if(isdigit(*iter)) {
            return std::string(state_name_with_idx.begin(), iter);
        }
    }
    return state_name_with_idx;
}

static RET_CODE ParseSubTexture(std::string::iterator& buf_iter, const std::string::iterator& buf_end, SpriteSheet* component) {
    SKIP("<")
    if(*buf_iter == '/') {
        SKIP("/TextureAtlas")
        SKIP(">")
        return RET_CODE::PARSE_END;
    }

    SKIP("SubTexture")
    
    GET_ARG(name, state_name_with_idx)
    std::string state_name = GetStateName(state_name_with_idx);

    // найти StateData с нейм state_name и добавить координаты или создать новый
    auto is_state_present = [&state_name](typename SpriteSheet::StateData& data) { return data.name_ == state_name; };
    
    auto iter = std::find_if(component->states_.begin(), component->states_.end(), is_state_present);

    std::vector<SpriteSheet::StateFrame>* p_poses = nullptr;
    if(iter != component->states_.end()){
        p_poses = &iter->positions_;
    }
    else {
        typename SpriteSheet::StateData state_data;
        state_data.name_ = state_name;
        component->states_.push_back(state_data);
        p_poses = &component->states_[component->states_.size() - 1].positions_;
    }

    GET_ARG(x, x_str)
    GET_ARG(y, y_str)
    GET_ARG(width, width_str)
    GET_ARG(height, height_str)

    SpriteSheet::StateFrame frame = {};
    frame.x_ = std::stoi(x_str);
    frame.y_ = std::stoi(y_str);
    frame.w_ = std::stoi(width_str);
    frame.h_ = std::stoi(height_str);

    if(*buf_iter == '/'){
        p_poses->push_back(frame);
        SKIP("/>")
        return RET_CODE::PARSED;
    }

    GET_ARG(frameX, frameX_str)
    GET_ARG(frameY, frameY_str)
    GET_ARG(frameWidth, frameW_str)
    GET_ARG(frameHeight, frameH_str)
    
    frame.x_offset_ = std::stoi(frameX_str);
    frame.y_offset_ = std::stoi(frameY_str);
    frame.frame_w_  = std::stoi(frameW_str);
    frame.frame_h_  = std::stoi(frameH_str);
    
    p_poses->push_back(frame);

    SKIP("/>")
    return RET_CODE::PARSED;
}

static RET_CODE ParseTexture(std::string::iterator& buf_iter, const std::string::iterator& buf_end, SpriteSheet* component) {
    
    SKIP("<")

    SKIP("TextureAtlas")
    GET_ARG(imagePath, image_path_str)
    component->img_path_ = image_path_str;
    SKIP(">")
    
    RET_CODE res;
    while((res = ParseSubTexture(buf_iter, buf_end, component)) == RET_CODE::PARSED){
    }
    return res;
}

static void ParseXml(const std::string& xml_path, ecs::Entity entity) {
    // todo: to ostream

    FILE* xml_file = fopen(xml_path.c_str(), "r");
    if(xml_file == nullptr) {
        // logger::print(ERROR, "unable to open xml file {}", xml_path);
        assert(0);
    }

    std::string buffer(GetFileSize(xml_file), 0);

    fread(reinterpret_cast<void*>(const_cast<char*>(buffer.c_str())), sizeof(char), buffer.size(), xml_file);

    auto buffer_iter = buffer.begin();
    xml_pos.col = 0;
    xml_pos.line = 0;

    RET_CODE res = ParseTitle(buffer_iter, buffer.end());
    if(res != RET_CODE::PARSED) {
        fmt::print("WARNING, xml parsing failed on pos<{}, {}> because of the ", xml_pos.col, xml_pos.line);
        if(res == RET_CODE::WRONG_PATTERN){
            fmt::print("wrong pattern\n");
            std::cout << (int)*buffer_iter << "\n";
        }
        else if (res == RET_CODE::REACHED_BUFFER_END){
            fmt::print("end of the buffer has been reached\n");
        }
        else {
            assert(0);
        }
        return;
    }

    SpriteSheet component;

    while((res = ParseTexture(buffer_iter, buffer.end(), &component)) == RET_CODE::PARSED) {
        
        component.texture_ = igraphicslib::Texture(component.img_path_.c_str());
        // component.sprite_.SetTexture(igraphicslib::Texture(component.img_path_.c_str()));
        entity.AssignFromCopy(std::move(component));
    }

    if(res == RET_CODE::PARSE_END){
        std::cout << &component.sprite_ << "\n";
        component.texture_ = igraphicslib::Texture(component.img_path_.c_str());
        // component.sprite_.SetTexture(igraphicslib::Texture(component.img_path_.c_str()));
        entity.AssignFromCopy(std::move(component));
    }

    else{
        fmt::print("WARNING, xml parsing failed on pos<{}, {}> because of the ", xml_pos.col, xml_pos.line);
        if(res == RET_CODE::WRONG_PATTERN){

            fmt::print("wrong pattern\n");
            std::cout << (int)*buffer_iter << "\n";
        }
        else if (res == RET_CODE::REACHED_BUFFER_END){
            fmt::print("end of the buffer has been reached\n");
        }
    }
}
