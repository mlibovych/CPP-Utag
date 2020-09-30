#include "mainwindow.h"
#include "./ui_mainwindow.h"

QString MainWindow::getStartDir(int argc, char **argv) {
    QString startPath;

    if (argc == 2 && QDir (argv[1]).exists()) {
        startPath = argv[1];
    }
    else {
        QMessageBox::warning(this, "Alert", "Command line arguments are invalid.\nCurrent dir will be opened.");
        startPath = QDir::currentPath();
    }
    return startPath;
}


MainWindow::MainWindow(int argc, char **argv, QWidget *parent) :
                        QMainWindow(parent),
                        ui(new Ui::MainWindow)
{
    QString startPath = getStartDir(argc, argv);

    ui->setupUi(this);

    dirmodel->setFilter(QDir::NoDotDot | QDir::AllDirs);
    dirmodel->setRootPath(startPath);
    ui->treeView->setModel(dirmodel);
    ui->treeView->setHeaderHidden(true);
    setDir(startPath);
    hideDirModelCols();

    ui->tableView->setModel(m_model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(50);
    ui->tableView->setShowGrid(false);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    setTableColsWidth();
}

void MainWindow::hideDirModelCols() {
    for (int i = 1; i < dirmodel->columnCount(); ++i) {
        ui->treeView->hideColumn(i);
    }
}

void MainWindow::setTableColsWidth() {
    for (int col = 0; col < m_model->columnCount(); ++col) {
        int width = 100;

        if (!col) {
            width = 280;
        }
        ui->tableView->setColumnWidth(col, width);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dirmodel;
    delete m_model;
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this);

    if (!path.isNull()) {
        setDir(path);
    }
}

void MainWindow::setDir(QString str)
{
    ui->treeView->setRootIndex(dirmodel->index(str));
    displayDirContent(str);
}

void MainWindow::displayDirContent(QString path) {
    QDir dir(path);
    QStringList listFiles = dir.entryList(m_ptxtMask->text().split(" "), QDir::Files);
    QVector<Tags> data;

    foreach (QString file, listFiles) {
        std::string fullPath = path.toStdString() + "/" + file.toStdString();
        std::string name = file.toStdString();
        QFileInfo info(fullPath.data());
        Tags tags(name, fullPath, info.isReadable());

        data.push_back(tags);
    }
    m_model->setNewData(std::move(data));
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString path = dirmodel->fileInfo(index).absoluteFilePath();

    displayDirContent(path);
}

void MainWindow::slotEdit() {
    QModelIndex index = ui->tableView->currentIndex();

    if (index.isValid() && index.column() != m_model->columnCount() - 1 && index.column() != 0) {
        QFileInfo info(m_model->getPath(index).toString());

        if (info.isWritable() && info.isReadable()) {
            m_model->setEditable(true);
            ui->tableView->edit(index);
            m_model->setEditable(false);
        }
        else {
            QMessageBox::warning(this, "Alert", "You do not have permissions\nto change this file.");
        }
    }
}

void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tableView->currentIndex();

    if (index.isValid() && index.column() != m_model->columnCount() - 1 && index.column() != 0) {
        QMenu* menu = new QMenu(this);
        QAction* editDevice = new QAction(("Edit"), this);

        connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEdit()));
        menu->addAction(editDevice);
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}
