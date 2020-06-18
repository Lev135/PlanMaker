#ifndef POINTTYPE_H
#define POINTTYPE_H

#include <QObject>
#include <QColor>


class Data;

class PointType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(Data *data READ data NOTIFY dataChanged)

    QString m_name;
    QColor m_color;
    int m_lastId;
    Data *m_data;

public:
    explicit PointType(const QColor &color, QObject *parent = nullptr);

    QString name() const;
    QColor color() const;

    int nextId();

    Data *data() const;

signals:
    void nameChanged(QString name);
    void colorChanged(QColor color);
    void dataChanged(Data *data);

public slots:
    void setColor(QColor color);
private slots:
    friend class Data;
    void setName(QString name);
    void setData(Data *data);
};

#endif // POINTTYPE_H
