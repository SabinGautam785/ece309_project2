#include "core/sentinel_scanner.h"
SentinelScanner::SentinelScanner(std::string sentinel) : sentinel_(sentinel) {

}
SentinelScanner::Out SentinelScanner::feed(std::string_view chunk) {
    std::string combined = pending_;
    combined.append(chunk.data(), chunk.size());
    std::size_t pos = combined.find(sentinel_);
    if (pos != std::string::npos) {
     Out result;
     result.safe_text = combined.substr(0, pos);
     result.sentinel_found = true;
     pending_.clear();
     return result;
    }
    std::size_t keep = sentinel_.size() - 1;
    if (combined.size() > keep) {
        std::size_t safe_count = combined.size() - keep;
        Out result;
        result.safe_text = combined.substr(0, safe_count);
        pending_ = combined.substr(safe_count);
        result.sentinel_found = false;
        return result;
    }
    else {
        pending_ = combined;
        Out result;
        result.safe_text = "";
        result.sentinel_found = false;
        return result;
    }
}
SentinelScanner::Out SentinelScanner::flush() {
    Out result;
    result.safe_text = pending_;
    result.sentinel_found = false;
    pending_.clear();
    return result;
}