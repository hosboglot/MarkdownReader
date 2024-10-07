#include "repositorylistmodel.h"

RepositoryListModel::RepositoryListModel(QObject *parent)
    : m_container{},
    QAbstractListModel(parent)
{}

int RepositoryListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_container.size();
}

QHash<int, QByteArray> RepositoryListModel::roleNames() const
{
    return { { NameRole, "name" }, { NestedDepthRole, "nestedDepth" },
            { IsDirRole, "isDir" }, { PathRole, "path" },
            { IsOpenRole, "isOpen"}, { IsHiddenRole, "isHidden"} };
}

bool RepositoryListModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_loader)
        return false;

    return m_loader->canFetchMore();
}

void RepositoryListModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid())
        return;

    std::vector<AbstractRepositoryLoader::Entry> fetched = m_loader->fetchMore();

    beginInsertRows(QModelIndex(), m_container.size(),
                    m_container.size() + fetched.size() - 1);
    for (auto const &entry : fetched) {
        m_container.push_back(
            ListEntry {
                QString::fromStdString(entry.name),
                entry.is_dir,
                entry.nested_depth,
                QUrl::fromEncoded(QByteArray::fromStdString(entry.path))
            });
    }
    endInsertRows();
}

QVariant RepositoryListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() < 0 || index.row() >= m_container.size()) {
        return QVariant();
    }

    switch (role)
    {
    case NameRole:
        return m_container.at(index.row()).name;
    case NestedDepthRole:
        return m_container.at(index.row()).nested_depth;
    case IsDirRole:
        return m_container.at(index.row()).is_dir;
    case PathRole:
        return m_container.at(index.row()).path;
    case IsOpenRole:
        return m_container.at(index.row()).is_open;
    case IsHiddenRole:
        return m_container.at(index.row()).is_hidden;
    }

    return QVariant();
}

bool RepositoryListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    auto &entry = m_container.at(index.row());
    switch (role)
    {
    case IsOpenRole:
        if (entry.is_dir) {
            setOpened(index, value.toBool());
        }
        return true;
    }

    return false;
}

Qt::ItemFlags RepositoryListModel::flags(const QModelIndex &index) const
{
    auto flags = QAbstractListModel::flags(index);
    flags.setFlag(Qt::ItemIsEditable);
    return flags;
}

void RepositoryListModel::setOpened(const QModelIndex &index, bool value)
{
    auto &entry = m_container.at(index.row());
    entry.is_open = value;

    auto end_it = _update_visibility(m_container.begin() + index.row());

    emit dataChanged(index,
                     this->index(end_it - m_container.begin(), 0),
                     {IsOpenRole, IsHiddenRole});
}

std::vector<RepositoryListModel::ListEntry>::iterator RepositoryListModel::_update_visibility(std::vector<ListEntry>::iterator parent)
{
    auto it = parent + 1;
    for (;it->nested_depth >= parent->nested_depth + 1 && it < m_container.end(); ++it) {
        if (it->nested_depth == parent->nested_depth + 1) {
            it->is_hidden = !parent->is_open || parent->is_hidden;
        } else {
            it = _update_visibility(it - 1);
        }
    }
    return it - 1;
}

void RepositoryListModel::setLocalSource(QUrl &path)
{
    if (!path.isValid() || path.isEmpty()) return;

    beginResetModel();
    m_loader = std::make_unique<LocalRepositoryLoader>(path);
    m_container.clear();
    endResetModel();
}
