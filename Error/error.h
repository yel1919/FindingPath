#ifndef ERROR_H
#define ERROR_H

#include <QString>
#include <QException>
#include <QMessageBox>

namespace find_path {
    class Exception : public QException {
    private:
        QString error;
    protected:
    public:
        explicit Exception(const QString& str);
        virtual ~Exception() = default;

        void raise() const override;
        Exception* clone() const override;
        QString ToString();
    };

    class ErrorBox {
    private:
    protected:
    public:
        static void Message(const QString& title, const QString& errorText);
    };
}

#endif // ERROR_H
