#include "Point.h"

#include "PointType.h"

Point::Point(PointType *type, QObject *parent) : QObject(parent), m_type(type), m_defenite(false), m_data(nullptr)
{
    Q_CHECK_PTR(type);
}

PointType *Point::type() const
{
    return m_type;
}

QPointF Point::coords() const
{
    return m_coords;
}

bool Point::defenite() const
{
    return m_defenite;
}

QString Point::name() const
{
    return m_name;
}

Data *Point::data() const
{
    return m_data;
}

void Point::setType(PointType *type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
}

void Point::setCoords(QPointF coords)
{
    if (m_defenite && m_coords == coords)
        return;
    if (!m_defenite) {
        m_defenite = true;
        emit defeniteChanged(true);
    }
    m_coords = coords;
    emit coordsChanged(m_coords);
}

void Point::setIndefenite()
{
    if (m_defenite) {
        m_defenite = false;
        emit defeniteChanged(false);
    }
}

void Point::setName(const QString &name)
{
    if (name == m_name) {
        return;
    }
    m_name = name;
    emit nameChanged(name);
}

void Point::setData(Data *data)
{
    Q_CHECK_PTR(data);
    if (m_data == data)
        return;

    m_data = data;
    emit dataChanged(data);
}
