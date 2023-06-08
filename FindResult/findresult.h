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
    };
}

#endif // FINDRESULT_H
