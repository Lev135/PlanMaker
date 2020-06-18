#ifndef POINT_H
#define POINT_H

#include <QObject>
#include <QPointF>

class PointType;
class Data;

class Point : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PointType *type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QPointF coords READ coords WRITE setCoords NOTIFY coordsChanged)
    Q_PROPERTY(bool defenite READ defenite NOTIFY defeniteChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(Data * data READ data NOTIFY dataChanged)


    PointType *m_type;
    QPointF m_coords;
    bool m_defenite;
    QString m_name;
    Data *m_data;

public:
    explicit Point(PointType *type, QObject *parent = nullptr);

    PointType *type() const;
    QPointF coords() const;
    bool defenite() const;
    QString name() const;
    Data *data() const;

signals:
    void typeChanged(PointType * type);
    void coordsChanged(QPointF coords);
    void defeniteChanged(bool defenite);
    void nameChanged(QString name);
    void dataChanged(Data * data);

public slots:
    void setType(PointType * type);
    void setCoords(QPointF coords);
    void setIndefenite();
private slots:
    friend class Data;
    void setName(const QString &name);
    void setData(Data *data);
};

#endif // POINT_H
