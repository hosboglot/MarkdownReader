#include "documentmodel.h"

DocumentModel::DocumentModel(QObject *parent)
    : m_container{},
    QAbstractListModel(parent)
{}

int DocumentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_container.size();
}

QHash<int, QByteArray> DocumentModel::roleNames() const
{
    return { { ContentRole, "content" }, { TagsRole, "tags" },
            { BadgeRole, "badge" }, { ColorRole, "color" },
            { TypeRole, "type" }, { TagsVisibleRole, "tagsVisible"} };
}

bool DocumentModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid() || !m_loader)
        return false;

    return m_loader->canFetchMore();
}

void DocumentModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid())
        return;

    std::vector<std::unique_ptr<AbstractDocumentLoader::Block>> fetched = m_loader->fetchMore();

    beginInsertRows(QModelIndex(), m_container.size(),
                    m_container.size() + fetched.size() - 1);
    for (auto const &block : fetched) {
        m_container.push_back(
            Block {
                QString::fromStdString(block->content),
                {},
                QString::fromStdString(block->badge_text),
                QColor(QString::fromStdString(block->color)),
                QString::fromStdString(block->type)
            });
        for (auto const &tag : block->tags) {
            m_container.back().tags.push_back(QString::fromStdString(tag));
        }
    }
    endInsertRows();
}

QVariant DocumentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= m_container.size()) {
        return QVariant();
    }

    switch (role)
    {
    case ContentRole:
        return m_container.at(index.row()).content;
    case TagsRole:
        return m_container.at(index.row()).tags.join(", ");
    case BadgeRole:
        return m_container.at(index.row()).badge_text;
    case ColorRole:
        return m_container.at(index.row()).color;
    case TypeRole:
        return m_container.at(index.row()).type;
    case TagsVisibleRole:
        return m_container.at(index.row()).tags_visible;
    }

    return QVariant();
}

bool DocumentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    auto &block = m_container.at(index.row());
    switch (role)
    {
    case TagsVisibleRole:
        block.tags_visible = value.toBool();
        emit dataChanged(index, index, {TagsVisibleRole});
        return true;
    }

    return false;
}

Qt::ItemFlags DocumentModel::flags(const QModelIndex &index) const
{
    auto flags = QAbstractListModel::flags(index);
    flags.setFlag(Qt::ItemIsEditable);
    return flags;
}

void DocumentModel::setLocalSource(QUrl &path)
{
    if (!path.isValid() || path.isEmpty()) return;

    beginResetModel();
    m_loader = std::make_unique<LocalDocumentLoader>(path);
    m_container.clear();
    endResetModel();
}
