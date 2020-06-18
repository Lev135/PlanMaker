#ifndef COMMANDER_H
#define COMMANDER_H

#include <QObject>

#include "mlib.h"

class Data;
class PointType;
class Point;

class Commander : public QObject
{
    Q_OBJECT
public:
    static const int modeCount = 5;
    enum CommanderMode {
        M_null = 0,
        M_print = 1 << 4,
        M_create = 2 << 4,
        M_change = 3 << 4,
        M_remove = 4 << 4
    };
    static CommanderMode modeAt(int index);
    static int index(CommanderMode mode);
    static QString toStr(CommanderMode mode);
    enum CommanderState {
        S_null = 0,
        S_print = M_print,
        S_create = M_create,
        S_createPoint1 = S_create | 1,
        S_createPoint2 = S_create | 2,
        S_change = M_change,
        S_remove = M_remove
    };

private:
    Q_PROPERTY(Data *data READ data)
    Q_PROPERTY(CommanderState state READ state NOTIFY stateChanged)
    Q_PROPERTY(CommanderMode mode READ mode WRITE setMode NOTIFY modeChanged)

    Data *m_data;
    CommanderState m_state;

    struct TempData {
        Point *curP, *p1, *p2;
        qreal dist1, dist2;
    };
    TempData m_temp;

public:
    explicit Commander(Data *data, QObject *parent = nullptr);

    Data * data() const;

    struct CommanderExit {
        enum ExitT {
            ok = 0,
            emptyCmd,
            unrealised,
            nullState,
            parseError,
            typeError
        };
        ExitT type;
        QString comment;
        CommanderExit();
        CommanderExit(ExitT type, const QString &comment = "");
        CommanderExit(const QString &comment);

        bool isOk() const;
    };

    void run(const QString &command);
    CommanderState state() const;
    CommanderMode mode() const;

signals:

    void stateChanged(Commander::CommanderState state);
    void modeChanged(Commander::CommanderMode mode);

public slots:
    void setMode(CommanderMode mode);
private slots:
    void setState(CommanderState state);
private:
    void cmdCreate(const QString &cmd);
    void listCreateType(const QStringList &list);
    void listCreatePoint0(const QStringList &list);
    void cmdCreatePoint12(const QString &cmd);

    static QPointF getCoords(const QString &xStr, const QString &yStr);
};

#endif // COMMANDER_H
