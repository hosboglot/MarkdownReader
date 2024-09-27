#include "repositoryhandler.h"

RepositoryHandler::RepositoryHandler(QObject *parent)
    : m_local_source{},
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

QVariant RepositoryHandler::repositoryModel() const
{
    return QVariant::fromValue(m_repository_model.get());
}
