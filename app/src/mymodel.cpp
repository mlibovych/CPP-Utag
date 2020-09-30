#include "mymodel.h"

MyModel::MyModel(const QVector<Tags> data, QObject* pobj)
        : QAbstractTableModel(pobj)
        , m_data(data)
{
}

QVariant MyModel::data(const QModelIndex& index, int nRole) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() < 0 || index.row() >= m_data.size()) {
        return QVariant();
    }
    if (nRole != Qt::DisplayRole && nRole != Qt::EditRole) {
        return QVariant();
    }
    if (index.column() >= m_cols) {
        return QVariant();
    }
    return m_data[index.row()].getTag(index.column());
}

QVariant MyModel::getPath(const QModelIndex& index) const {
    return m_data[index.row()].getTag(m_cols - 1);
}

bool MyModel::setData(const QModelIndex& index,
                const QVariant& value,
                int nRole)
{
    if (index.isValid() && nRole == Qt::EditRole) {
        m_data[index.row()].setTag(index.column(), value);

        emit dataChanged(index, index);
        return true;
    }
    return false;
}

int MyModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_data.size();
}

int MyModel::rowCount() const {
    return m_data.size();
}

int MyModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_cols;
}

int MyModel::columnCount() const {
    return m_cols;
}

QVariant MyModel::headerData(int nSection,
                    Qt::Orientation orientation,
                    int nRole
                    ) const
{
    if (nRole != Qt::DisplayRole) {
        return QVariant();
    }
    if(orientation != Qt::Horizontal) {
        return QVariant();
    }
    return m_headers[nSection];
}

Qt::ItemFlags MyModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    if (index.isValid() && index.column() != columnCount() - 1 && index.column() != 0 && editable == true) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

void MyModel::setNewData(QVector<Tags>&& data) {
    m_data.clear();
    m_data = data;
    emit layoutChanged();
}

void MyModel::sort(int column, Qt::SortOrder order) {
    std::sort(m_data.begin(), m_data.end(), [column, order](Tags row1, Tags row2) {
        if (order == Qt::AscendingOrder) {
            return row1.getTag(column).toString() > row2.getTag(column).toString();
        }
        return row1.getTag(column).toString() < row2.getTag(column).toString();
    });
    emit layoutChanged();
}