#ifndef REPOSITORYHANDLER_H
#define REPOSITORYHANDLER_H

#include "repositorylistmodel.h"

#include <QObject>
#include <QVariant>
#include <QUrl>

#include <memory>

class RepositoryHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl localSource READ localSource WRITE setLocalSource NOTIFY localSourceChanged)
public:
    explicit RepositoryHandler(QObject *parent = nullptr);

    QUrl localSource() const;
    void setLocalSource(QUrl &newSource);
    Q_INVOKABLE QVariant repositoryModel() const;
signals:
    void localSourceChanged(QUrl);
private:
    QUrl m_local_source;
    std::unique_ptr<RepositoryListModel> m_repository_model;
};

#endif // REPOSITORYHANDLER_H
