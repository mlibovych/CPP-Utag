#pragma once

#include <QMainWindow>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QListView>
#include <QTableView>
#include <QFileDialog>

#include "tags.h"

namespace Ui {
    class MyModel;
}

class MyModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    int m_cols = 8;
    QVector<QString> m_headers = {"Name", "Artist", "Title", "Album", "Genre", "Year", "Track Number", "Audio file path"};
    QVector<Tags> m_data;
    bool editable = false;
public:
    void setEditable(bool flag) {
        editable = flag;
    }

    MyModel(const QVector<Tags> data, QObject* pobj = 0);

    QVariant data(const QModelIndex& index, int nRole) const;

    QVariant getPath(const QModelIndex& index) const;

    bool setData(const QModelIndex& index,
                const QVariant& value,
                int nRole);

    int rowCount(const QModelIndex& parent) const;

    int rowCount() const;

    int columnCount(const QModelIndex& parent) const;

    int columnCount() const;

    QVariant headerData(int nSection,
                        Qt::Orientation orientation,
                        int nRole
                        ) const;

    Qt::ItemFlags flags(const QModelIndex& index) const;

    void setNewData(QVector<Tags>&& data);

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
};