#include <parsing/abstract_parser.hpp>
#include <logger/logger.hpp>

#include <cstring>

using RetCode = typename AbstractParser::RET_CODE;

AbstractParser::~AbstractParser() {
}
void AbstractParser::SkipSpaces() {
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

RetCode AbstractParser::Skip(const std::string& str) {

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

RetCode AbstractParser::SkipUntilSymb(char symb) {
    while (buf_iter_ < buf_end_ && *buf_iter_ != symb) {
        col_++;
        buf_iter_++;
    }
    if (*buf_iter_ != symb)
        return RetCode::WRONG_PATTERN;

    return RetCode::PARSED;
}

void AbstractParser::ShowFailedParseWarning(RetCode res) {
    logger::Print(kWarning, "xml parsing failed on pos<{}, {}> because of the ", col_, line_);
    if (res == RetCode::WRONG_PATTERN) {
        logger::Print("wrong pattern\n");
    } else if (res == RetCode::REACHED_BUFFER_END) {
        logger::Print("end of the buffer has been reached\n");
    }
}

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

void AbstractParser::InitParser(const std::string& path) {

    // todo: to ostream

    FILE* parsed_file = fopen(path.c_str(), "r");
    if (parsed_file == nullptr) {
        logger::Print(kError, "unable to open file {} on parsing", path);
        assert(0);
    }

    buffer_.resize(GetFileSize(parsed_file), 0);

    fread(reinterpret_cast<void*>(const_cast<char*>(buffer_.c_str())), sizeof(char), buffer.size(), xml_file);

    buf_iter_ = buffer_.begin();
    buf_end_ = buffer_.end();

    col_ = 0;
    line_ = 0;
}

void AbstractParser::CloseParser() {
    buffer_.resize(0);
}
