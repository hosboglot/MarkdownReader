#ifndef REPOSITORYLISTMODEL_H
#define REPOSITORYLISTMODEL_H

#include "repositoryloaders/localrepositoryloader.h"

#include <QAbstractListModel>
#include <QUrl>
#include <QDir>
#include <QQmlFile>
#include <QDebug>

#include <memory>
#include <vector>

class RepositoryListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum RepositoryRoles {
        NameRole = Qt::UserRole + 1,
        NestedDepthRole,
    };

    explicit RepositoryListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Fetch data dynamically:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setLocalSource(QUrl &path);
private:
    std::unique_ptr<AbstractRepositoryLoader> m_loader;
    std::vector<AbstractRepositoryLoader::Entry> m_container;
};

#endif // REPOSITORYLISTMODEL_H
