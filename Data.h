#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QMap>

class Point;
class PointType;

class Data : public QObject
{
    Q_OBJECT
    QMap<QString, PointType *> m_types;
    QMap<QString, Point *> m_points;

public:
    explicit Data(QObject *parent = nullptr);

    PointType *type(const QString &name);
    Point *point(const QString &name);

    QMap<QString, PointType *> types() const;
    QMap<QString, Point *> points() const;

    void addType(PointType *type, const QString &name);
    void renameType(PointType *type, const QString newName);
    void removeType(PointType *type);

    void addPoint(Point *point, const QString &name);
    void renamePoint(Point *point, const QString &newName);
    void removePoint(Point *point);

    static bool isReservedWord(const QString &word);
    static bool isTypeWord(QString word);


signals:
    void typeAdded(PointType *type);
    void typeRemoved(PointType *type);

    void pointAdded(Point *point);
    void pointRemoved(Point *point);
public slots:
};

#endif // DATA_H
