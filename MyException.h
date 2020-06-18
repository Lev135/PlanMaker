#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <QDebug>

#include <QException>
#include <QString>

class MyException : public QException
{
    QString m_what;
public:
    MyException(const QString &what = QString());

    const char *what() const noexcept override;
};

class UserException : public MyException
{
public:
    UserException(const QString &what);
};

#endif // MYEXCEPTION_H
