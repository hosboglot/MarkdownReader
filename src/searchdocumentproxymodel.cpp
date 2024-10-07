#include "searchdocumentproxymodel.h"

SearchDocumentProxyModel::SearchDocumentProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{}

int SearchDocumentProxyModel::getModelIndex(int row)
{
    QModelIndex sourceIndex = mapToSource(this->index(row, 0));
    return sourceIndex.row();
}

bool SearchDocumentProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const auto sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);

    const QStringList tags = sourceModel()->data(sourceIndex, DocumentModel::TagsRole).toStringList();
    for (const auto& tag : tags) {
        if (tag.contains(filterRegExp())) {
            return true;
        }
    }

    return sourceModel()->data(sourceIndex, DocumentModel::ContentRole)
        .toString().contains(filterRegExp());
}
