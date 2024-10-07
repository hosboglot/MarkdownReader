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
    struct ListEntry {
        QString name;
        bool is_dir;
        unsigned nested_depth;
        QUrl path;
        bool is_open = true;
        bool is_hidden = false;
    };
    enum RepositoryRoles {
        NameRole = Qt::UserRole + 1,
        NestedDepthRole,
        IsDirRole,
        PathRole,
        IsOpenRole,
        IsHiddenRole
    };

    explicit RepositoryListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Fetch data dynamically:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void setOpened(const QModelIndex &index, bool value);
    void setLocalSource(QUrl &path);
private:
    std::vector<ListEntry>::iterator _update_visibility(std::vector<ListEntry>::iterator parent);

    std::unique_ptr<AbstractRepositoryLoader> m_loader;
    std::vector<ListEntry> m_container;
};

#endif // REPOSITORYLISTMODEL_H
