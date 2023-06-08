#include "findresult.h"

find_path::FindResult::FindResult()
    :
      path(nullptr),
      success(false)
{}

find_path::FindResult::FindResult(const FindResult& result)
    :
      path(result.path),
      success(result.success)
{}

find_path::FindResult::FindResult(FindResult&& result)
    :
      path(result.path),
      success(result.success)
{
    result.path = nullptr;
    result.success = false;
}

find_path::FindResult& find_path::FindResult::operator=(const FindResult& result) {
    if(&result != this) {
        path = result.path;
        success = result.success;
    }
    return *this;
}

find_path::FindResult& find_path::FindResult::operator=(FindResult&& result) {
    if(&result != this) {
        path = result.path;
        success = result.success;

        result.path = nullptr;
        result.success = false;
    }
    return *this;
}
