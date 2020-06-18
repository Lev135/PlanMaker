#include "Data.h"

#include "MyException.h"
#include "PointType.h"
#include "Point.h"

Data::Data(QObject *parent) : QObject(parent)
{

}

PointType *Data::type(const QString &name)
{
    auto it = m_types.find(name);
    if (it == m_types.end()) {
        throw UserException(QString("Unregistered type \'%1\'").arg(name));
    }
    return it.value();
}

Point *Data::point(const QString &name)
{
    auto it = m_points.find(name);
    if (it == m_points.end()) {
        throw UserException(QString("Unregistered point \'%1\'").arg(name));
    }
    return it.value();
}

void Data::addType(PointType *type, const QString &name)
{
    Q_CHECK_PTR(type);
    if (m_types.find(name) != m_types.end())
        throw UserException(QString("Type \'%1\' already exists").arg(name));
    if (name.isEmpty())
        throw UserException(QString("Invalid type name: \'%1\'").arg(name));
    if (isReservedWord(name))
        throw UserException(QString("The word \'%1\' is reserved").arg(name));
    if (type->data())
        throw MyException(QString("Type \'%1\' already is in data").arg(type->name()));
    m_types.insert(name, type);
    type->setName(name);
    type->setData(this);
    emit typeAdded(type);
}

void Data::removeType(PointType *type)
{
    Q_CHECK_PTR(type);
    if (type->data() != this)
        throw MyException(QString("Type \'%1\' isn't in data").arg(type->name()));
    QString name = type->name();
    auto it = m_types.find(type->name());
    Q_ASSERT(it != m_types.end());
    m_types.erase(it);
    emit typeRemoved(type);
}

void Data::addPoint(Point *point, const QString &name)
{
    Q_CHECK_PTR(point);
    if (m_points.find(name) != m_points.end())
        throw UserException(QString("Point \'%1\' already exists").arg(name));
    if (name.isEmpty())
        throw UserException(QString("Invalid point name: \'%1\'").arg(name));
    if (isReservedWord(name))
        throw UserException(QString("The word \'%1\' is reserved").arg(name));
    if (point->data())
        throw MyException(QString("Point \'%1\' already is in data").arg(point->name()));
    m_points.insert(name, point);
    point->setName(name);
    point->setData(this);
    emit pointAdded(point);
}

bool Data::isReservedWord(const QString &word)
{
    return isTypeWord(word);
}

bool Data::isTypeWord(QString word)
{
    word = word.toLower();
    return word == "t" || word == "type";
}


