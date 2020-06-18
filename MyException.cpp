#include "MyException.h"

MyException::MyException(const QString &what) : QException(), m_what(what)
{
    if (what.isNull()) {
        m_what = "Unrealised";
    }
}

const char *MyException::what() const noexcept
{
    return m_what.toStdString().c_str();
}

UserException::UserException(const QString &what) : MyException(what)
{
}
