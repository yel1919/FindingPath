#ifndef PATH_H
#define PATH_H

#include <QVector>
#include "../Error/error.h"

namespace find_path {
    template<typename T>
    class Path {
    private:
        QVector<T> path;
    protected:
    public:
        Path() : path({}) {}
        explicit Path(const int size) : path(size) {}
        explicit Path(const QVector<T>& path) : path(path) {}
        explicit Path(QVector<T>&& path) { path = std::move(path); }
        Path(const Path& path) : path(path) {}
        Path(Path&& path) { this->path = std::move(path.path); }
        virtual ~Path() = default;

        inline Path& operator=(const Path& path) {
            if(this != &path) {
                this->path = path.path;
            }
            return *this;
        }

        inline Path& operator=(Path&& path) {
            if(this != &path) {
                this->path = std::move(path.path);
            }
            return *this;
        }

        inline const T& operator[](int index) const {
            return path[index];
        }

        inline T& operator[](int index) {
            return path[index];
        }

        inline const T& at(int index) const {
            if(index < 0 || index > path.size())
                throw Exception("Path object error: Array index out of bounds");
            return path[index];
        }

        inline T& at(int index) {
            if(index < 0 || index > path.size())
                throw Exception("Path object error: Array index out of bounds");
            return path[index];
        }

        inline const T* const begin() const {
            return reinterpret_cast<const T* const>(&*path.begin());
        }

        inline T* const begin() {
            return reinterpret_cast<T* const>(&*path.begin());
        }

        inline const T* const end() const {
            return reinterpret_cast<const T* const>(&*path.end());
        }

        inline T* const end() {
            return reinterpret_cast<T* const>(&*path.end());
        }

        inline void push_back(const T& vertex) {
            path.push_back(vertex);
        }

        inline void push_front(const T& vertex) {
            path.push_front(vertex);
        }

        inline const int weight() const {
            return path.size();
        }

        inline const QVector<T>& ToVector() const {
            return path;
        }

        inline QVector<T>& ToVector() {
            return path;
        }
    };
}

#endif // PATH_H
