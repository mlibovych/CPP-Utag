#pragma once

#include <string>

#include <QMainWindow>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QListView>
#include <QTableView>
#include <QMenu>
#include <QMessageBox>

#include "mymodel.h"
#include "tags.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirmodel = new QFileSystemModel(this);
    MyModel *m_model = new MyModel(QVector<Tags>());
    QLineEdit *m_ptxtMask = new QLineEdit("*.mp3");

    QString getStartDir(int argc, char **argv);

public:
    explicit MainWindow(int argc, char **argv, QWidget *parent = nullptr);
    ~MainWindow();

    void setDir(QString str);
    void displayDirContent(QString path);

    void setTableColsWidth();
    void hideDirModelCols();

private slots:
    void on_pushButton_clicked();

    void on_treeView_clicked(const QModelIndex &index);

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void slotEdit();
};

