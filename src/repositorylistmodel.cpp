#include "repositorylistmodel.h"

RepositoryListModel::RepositoryListModel(QObject *parent)
    : m_container{},
    QAbstractListModel(parent)
{}

int RepositoryListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    qDebug() << "container size" << m_container.size();
    return m_container.size();
}

QHash<int, QByteArray> RepositoryListModel::roleNames() const
{
    return { { NameRole, "name" }, { NestedDepthRole, "nestedDepth" } };
}

bool RepositoryListModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_loader)
        return false;

    qDebug() << "has next" << m_loader->canFetchMore();
    return m_loader->canFetchMore();
}

void RepositoryListModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid())
        return;

    std::vector<AbstractRepositoryLoader::Entry> fetched = m_loader->fetchMore();
    qDebug() << "fetched" << fetched.size();

    beginInsertRows(QModelIndex(), m_container.size(), m_container.size() + fetched.size() - 1);
    m_container.insert(m_container.end(), fetched.begin(), fetched.end());
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
        return QVariant::fromValue(QString::fromStdString(m_container[index.row()].name));
    case NestedDepthRole:
        return QVariant::fromValue(m_container[index.row()].nested_depth);
    }

    return QVariant();
}

void RepositoryListModel::setLocalSource(QUrl &path)
{
    if (!path.isValid() || path.isEmpty()) return;

    beginResetModel();
    m_loader = std::make_unique<LocalRepositoryLoader>(path);
    m_container.empty();
    endResetModel();
}
