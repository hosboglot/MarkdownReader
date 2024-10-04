#ifndef SEARCHDOCUMENTPROXYMODEL_H
#define SEARCHDOCUMENTPROXYMODEL_H

#include "documentmodel.h"

#include <QSortFilterProxyModel>

class SearchDocumentProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchDocumentProxyModel(QObject *parent = nullptr);
public slots:
    int getModelIndex(int);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // SEARCHDOCUMENTPROXYMODEL_H
