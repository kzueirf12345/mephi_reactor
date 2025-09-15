#include <string>

#include "common/ErrorHandle.hpp"

#define CASE_ENUM_TO_STRING_(Error) case Error: return #Error
std::string Common::ErrorToString(const enum Common::Error Error) {
    switch (Error) {
        CASE_ENUM_TO_STRING_(Common::Error::SUCCESS);         
        CASE_ENUM_TO_STRING_(Common::Error::STANDART_ERRNO); 
    
        default:
            return "UNKNOWN_ERROR";
    }

    return "UNKNOWN_ERROR";
}
#undef CASE_ENUM_TO_STRING_