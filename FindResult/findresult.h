#ifndef FINDRESULT_H
#define FINDRESULT_H

#include "../Node/node.h"

namespace find_path {
    class FindResult {
    private:
    protected:
    public:
        Path<QPair<int, int>>* path;
        bool success;

        FindResult();
        FindResult(const FindResult& result);
        FindResult(FindResult&& result);
        virtual ~FindResult() {}

        FindResult& operator=(const FindResult& result);
        FindResult& operator=(FindResult&& result);
    };
}

#endif // FINDRESULT_H
