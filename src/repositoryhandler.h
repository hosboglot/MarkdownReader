#ifndef REPOSITORYHANDLER_H
#define REPOSITORYHANDLER_H

#include "repositorylistmodel.h"
#include "documentmodel.h"
#include "searchdocumentproxymodel.h"

#include <QObject>
#include <QVariant>
#include <QFileInfo>
#include <QUrl>

#include <memory>

class RepositoryHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl localSource READ localSource WRITE setLocalSource NOTIFY localSourceChanged)
    Q_PROPERTY(QUrl openDocument READ openDocument WRITE setOpenDocument NOTIFY openDocumentChanged)
public:
    explicit RepositoryHandler(QObject *parent = nullptr);

    QUrl localSource() const;
    void setLocalSource(QUrl &newSource);

    QUrl openDocument() const;
    void setOpenDocument(QUrl &newDocument);

    Q_INVOKABLE QVariant repositoryModel() const;
    Q_INVOKABLE QVariant documentModel() const;
    Q_INVOKABLE QVariant searchDocumentModel();
signals:
    void localSourceChanged(QUrl);
    void openDocumentChanged(QUrl);
private:
    QUrl m_local_source;
    QUrl m_open_document;
    std::unique_ptr<RepositoryListModel> m_repository_model;
    std::unique_ptr<DocumentModel> m_document_model;
    std::unique_ptr<SearchDocumentProxyModel> m_search_document_model;
};

#endif // REPOSITORYHANDLER_H
