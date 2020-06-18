#include "PointType.h"

#include "Data.h"

PointType::PointType(const QColor &color, QObject *parent) : QObject(parent), m_color(color), m_lastId(0), m_data(nullptr)
{
    Q_ASSERT(color.isValid());
}

QString PointType::name() const
{
    return m_name;
}

QColor PointType::color() const
{
    return m_color;
}

int PointType::nextId()
{
    return ++m_lastId;
}

Data *PointType::data() const
{
    return m_data;
}

void PointType::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(m_color);
}

void PointType::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

void PointType::setData(Data *data)
{
    Q_CHECK_PTR(data);
    if (m_data == data)
        return;

    m_data = data;
    emit dataChanged(data);
}
