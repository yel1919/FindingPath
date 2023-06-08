#include "error.h"

find_path::Exception::Exception(const QString& str) : error(str) {}

void find_path::Exception::raise() const { throw *this; }
find_path::Exception* find_path::Exception::clone() const { return new Exception(*this); }

QString find_path::Exception::ToString() {
    return this->error + "\r\n" + (QObject::tr(what()) == "std::exception" ? "" : QObject::tr(what()));
}

void find_path::ErrorBox::Message(const QString& title, const QString& errorText) {
    QMessageBox error;
    error.setFixedSize(800,300);
    error.critical(0, title, errorText);
}
