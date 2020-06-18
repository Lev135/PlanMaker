#include "Commander.h"

#include "MyException.h"

#include "Data.h"
#include "Point.h"
#include "PointType.h"

Commander::CommanderMode Commander::modeAt(int index)
{
    Q_ASSERT(index < modeCount);
    return CommanderMode(index << 4);
}

int Commander::index(Commander::CommanderMode mode)
{
    return mode >> 4;
}

QString Commander::toStr(Commander::CommanderMode mode)
{
    switch (mode) {
    case M_null:
        return "null";
    case M_print:
        return "print";
    case M_create:
        return "create";
    case M_change:
        return "change";
    case M_remove:
        return "remove";
    }
    return "default in Commander::toStr";
}

Commander::Commander(Data *data, QObject *parent) : QObject(parent), m_data(data), m_state(S_null)
{

}

Data *Commander::data() const
{
    return m_data;
}

void Commander::run(const QString &command)
{
    if (command.isEmpty()) {
        throw UserException("Empty cmd");
    }
    switch (m_state) {
    case S_null:
        throw MyException("Null state");
    case S_create:
        cmdCreate(command);
        break;
    case S_createPoint1:
    case S_createPoint2:
        cmdCreatePoint12(command);
        break;
    default:
        throw MyException();
    }
}

Commander::CommanderState Commander::state() const
{
    return m_state;
}

Commander::CommanderMode Commander::mode() const
{
    return CommanderMode(~M_null << 4 & m_state);
}

void Commander::setMode(Commander::CommanderMode mode)
{
    if (mode == this->mode())
        return;
    m_state = CommanderState(mode);
    emit modeChanged(mode);
    emit stateChanged(m_state);
}

void Commander::setState(Commander::CommanderState state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(m_state);
}

void Commander::cmdCreate(const QString &cmd)
{
    QStringList list = cmd.split(" ", QString::SkipEmptyParts);
    if (list.size() >= 1 && Data::isTypeWord(list[0])) {
        if (list.size() == 3) { // creating type by "type" & typeName & color
            QString typeName = list[1], colorStr = list[2];
            QColor color(colorStr);
            if (!color.isValid()) {
                throw UserException(QString("Expected color name, found: \'%1\'").arg(colorStr));
            }
            PointType *type = new PointType(color, this);
            m_data->addType(type, typeName);
            //return CommanderExit(CommanderExit::ok, QString("type \'%1\' succesfully added").arg(typeName));
        }
        else {
            throw UserException(QString("Expected word \'type\' and 2 args, found \'type\' and %1 args").arg(list.size() - 1));
        }
    }
    else if (1 <= list.size() && list.size() <= 4) { // creating point (by type) or (by type & name) or
                                                     //    (by type & x & y) or (by type & name & x & y)
        QString typeName = list[0], name;
        PointType *type = m_data->type(typeName);
        Point *point = new Point(type, this);
        if (list.size() >= 3) {
            QPointF coords = getCoords(list[list.size() - 2], list[list.size() - 1]);
            point->setCoords(coords);
        }
        if (list.size() == 2 || list.size() == 4) {
            name = list[1];
        }
        else {
            name = QString("%1_%2").arg(type->name()).arg(type->nextId());
        }
        m_data->addPoint(point, name);
        if (list.size() <= 2) {
            this->setState(S_createPoint1);
        }
        else {
            this->setState(S_create);
        }
    }
    else {
        throw UserException(QString("(1 - 2 args) or (\'type\' and 2 args) were expected, %1 found").arg(list.size()));
    }
}

void Commander::cmdCreatePoint12(const QString &cmd)
{
    QStringList list = cmd.split(" ", QString::SkipEmptyParts);
    if (list.size() == 2) {
        QString pointName = list[0], distStr = list[1];
        Point *point = m_data->point(pointName);
        bool f;
        qreal dist = distStr.toDouble(&f);
        if (!f) {
            throw UserException(QString("Real number (distance) exspected \'%1\' found").arg(distStr));
        }
        if (m_state == S_createPoint1) {
            m_temp.p1 = point;
            m_temp.dist1 = dist;
            this->setState(S_createPoint2);
        }
        else {
            m_temp.p2 = point;
            m_temp.dist2 = dist;
            this->setState(S_create);
        }
    }
    else {
        throw UserException(QString("2 args were expected, %1 found").arg(list.size()));
    }
}

QPointF Commander::getCoords(const QString &xStr, const QString &yStr)
{
    bool fX, fY;
    qreal x = xStr.toDouble(&fX), y = yStr.toDouble(&fY);
    if (!fX || !fY)
        throw UserException(QString("Expected two real values (coords). Found \'%1\' and \'%2\'").arg(xStr).arg(yStr));
    return QPointF(x, y);
}

/*
Commander::CommanderExit Commander::checkNewTypeName(const QString &typeName)
{
    if (Data::isReservedWord(typeName)) {
        return CommanderExit(CommanderExit::typeError, QString("the name \'%1\' is reserved. Please, choose other name").arg(typeName));
    }
    if (m_data->type(typeName)) {
        return CommanderExit(CommanderExit::typeError, QString("type \'%1\' already exist").arg(typeName));
    }
    return CommanderExit::ok;
}

Commander::CommanderExit Commander::checkNewTypeColor(const QString &colorStr)
{
    QColor color(colorStr);
    if (!color.isValid()) {
        return CommanderExit(CommanderExit::parseError, QString("expected name of color, found \'%1\'").arg(colorStr));
    }
    if (color == Qt::white) {
        return CommanderExit(CommanderExit::typeError, QString("white color can\'t be choosen. Please choose other color"));
    }
    return CommanderExit::ok;
}

Commander::CommanderExit Commander::checkNewPointName(const QString &name)
{
    if (Data::isReservedWord(name)) {
        return CommanderExit(CommanderExit::typeError, QString("the name \'%1\' is reserved. Please, choose other name").arg(name));
    }
    if (m_data->point(name)) {
        return CommanderExit(CommanderExit::typeError, QString("point \'%1\' already exist").arg(name));
    }
    return CommanderExit::ok;
}
*/

///////////// Commander::CommanderExit //////////////////////////////

Commander::CommanderExit::CommanderExit() : type(unrealised), comment("Unrealised")
{
}

Commander::CommanderExit::CommanderExit(Commander::CommanderExit::ExitT _type, const QString &_comment) : type(_type), comment(_comment) {
    if (comment.isEmpty()) {
        switch (_type) {
        case ok:
            break;
        case unrealised:
            comment = "Unrealised";
            break;
        case emptyCmd:
            comment = "Empty command";
            break;
        case nullState:
            comment = "Commander is in null state";
            break;
        default:
            comment = "Default in constructor Commander::CommanderExit";
            break;
        }
    }
}

Commander::CommanderExit::CommanderExit(const QString &_comment) : type(parseError), comment(_comment)
{
}

bool Commander::CommanderExit::isOk() const
{
    return type == ok;
}
