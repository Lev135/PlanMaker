#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "MyException.h"

#include "Data.h"
#include "Commander.h"
#include "PointType.h"
#include "Point.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_data = new Data(this);
    m_commander = new Commander(m_data, this);
    m_commander->setMode(Commander::M_create);
    ui->w_logListWidget->setWordWrap(true);
    initStateComboBox();
    connect(m_data, SIGNAL(typeAdded(PointType *)), this, SLOT(on_typeAdded(PointType *)));
    connect(m_data, SIGNAL(pointAdded(Point *)), this, SLOT(onPointAdded(Point *)));
    connect(m_commander, SIGNAL(modeChanged(Commander::CommanderMode)), this, SLOT(onModeChanged(Commander::CommanderMode)));
    m_commander->setMode(Commander::M_create);

    m_commander->run("type t1 green");
    m_commander->run("type t2 red");
    m_commander->run("type t3 blue");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_typeAdded(PointType *type)
{
    Q_CHECK_PTR(type);
    Q_ASSERT(type->data() == m_data);
    QTableWidget *table = ui->w_typesTable;
    int row = table->rowCount();
    table->insertRow(row);
    QTableWidgetItem *typeNameItem = new QTableWidgetItem(type->name());
    table->setItem(row, 0, typeNameItem);
    QTableWidgetItem *colorItem = new QTableWidgetItem();
    colorItem->setBackgroundColor(type->color());
    table->setItem(row, 1, colorItem);
}

void MainWindow::onPointAdded(Point *point)
{
    Q_CHECK_PTR(point);
    Q_ASSERT(point->data() == m_data);

    QTableWidget *table = ui->w_pointsTable;
    int row = table->rowCount();
    table->insertRow(row);

    QTableWidgetItem *pointNameItem = new QTableWidgetItem(point->name());
    int col = 0;
    table->setItem(row, col++, pointNameItem);

    QTableWidgetItem *typeItem = new QTableWidgetItem(point->type()->name());
    typeItem->setTextColor(point->type()->color());
    table->setItem(row, col++, typeItem);

    QTableWidgetItem *xItem = new QTableWidgetItem(), *yItem = new QTableWidgetItem();
    if (point->defenite()) {
        xItem->setText(QString::number(point->coords().x()));
        yItem->setText(QString::number(point->coords().y()));
    }
    else {
        xItem->setText("indefenite");
        yItem->setText("indefenite");
    }
    table->setItem(row, col++, xItem);
    table->setItem(row, col++, yItem);
}

void MainWindow::onModeChanged(Commander::CommanderMode newMode)
{
    QString str = Commander::toStr(newMode);
    ui->w_stateComboBox->setCurrentIndex(Commander::index(newMode) - 1);
    ui->w_logListWidget->addItem(QString("Commander mode changed: new mode \'%1\'").arg(str));
}

void MainWindow::on_w_enterButton_clicked()
{
    QString command = ui->w_commandLineEdit->text();

    ui->w_logListWidget->addItem(command);
    try {
        m_commander->run(command);
    } catch (UserException &ex) {
        ui->w_logListWidget->addItem(QString("Error: %1").arg(ex.what()));
        ui->w_logListWidget->scrollToBottom();
        return;
    } catch (MyException &ex) {
        ui->w_logListWidget->addItem(QString("EXCEPTION: %1").arg(ex.what()));
        ui->w_logListWidget->scrollToBottom();
        return;
    }
//    ui->w_logListWidget->addItem("Ok");
    ui->w_commandLineEdit->clear();
    ui->w_logListWidget->scrollToBottom();
}


void MainWindow::initStateComboBox()
{
    QStringList list;
    // list << "SelectCommanderMode";
    for (int i = 1; i < Commander::modeCount; ++i) {
        list << Commander::toStr(Commander::modeAt(i));
    }
    ui->w_stateComboBox->addItems(list);
}

void MainWindow::on_w_stateComboBox_currentIndexChanged(int index)
{
    ++index;
    m_commander->setMode(Commander::modeAt(index));
}

void MainWindow::on_w_commandLineEdit_returnPressed()
{
    on_w_enterButton_clicked();
}
