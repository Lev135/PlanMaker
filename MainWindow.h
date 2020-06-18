#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Commander.h"

namespace Ui {
class MainWindow;
}

class Data;
class Commander;
class Point;
class PointType;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Data *m_data;
    Commander *m_commander;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_typeAdded(PointType *type);
    void onPointAdded(Point *point);

    void onModeChanged(Commander::CommanderMode newMode);

    void on_w_enterButton_clicked();
    void on_w_stateComboBox_currentIndexChanged(int index);

    void on_w_commandLineEdit_returnPressed();

private:
    Ui::MainWindow *ui;

    void initStateComboBox();
};

#endif // MAINWINDOW_H
