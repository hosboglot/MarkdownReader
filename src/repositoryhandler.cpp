#include "repositoryhandler.h"

RepositoryHandler::RepositoryHandler(QObject *parent)
    : m_local_source{},
    m_open_document{},
    m_repository_model{},
    QObject{parent}
{}

QUrl RepositoryHandler::localSource() const
{
    return m_local_source;
}

void RepositoryHandler::setLocalSource(QUrl &newSource)
{
    if (m_local_source != newSource) {
        m_local_source = newSource;
        m_repository_model = std::make_unique<RepositoryListModel>();
        m_repository_model->setLocalSource(newSource);
        emit localSourceChanged(newSource);
    }
}

QUrl RepositoryHandler::openDocument() const
{
    return m_open_document;
}

void RepositoryHandler::setOpenDocument(QUrl &newDocument)
{
    if (m_open_document != newDocument) {
        m_open_document = newDocument;
        m_document_model = std::make_unique<DocumentModel>();
        m_document_model->setLocalSource(newDocument);
        emit openDocumentChanged(newDocument);
    }
}

QVariant RepositoryHandler::repositoryModel() const
{
    return QVariant::fromValue(m_repository_model.get());
}

QVariant RepositoryHandler::documentModel() const
{
    return QVariant::fromValue(m_document_model.get());
}

QVariant RepositoryHandler::searchDocumentModel()
{
    m_search_document_model = std::make_unique<SearchDocumentProxyModel>();
    m_search_document_model->setSourceModel(m_document_model.get());
    return QVariant::fromValue(m_search_document_model.get());
}


