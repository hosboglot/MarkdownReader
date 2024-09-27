#include "localrepositoryloader.h"

LocalRepositoryLoader::LocalRepositoryLoader(QUrl path)
    : m_loading_stack{},
    m_depth_stack{}
{
    auto pathStr = path.toString();

    for (auto &it : _get_file_list(pathStr)) {
        m_loading_stack.push(it.absoluteFilePath());
        qDebug() << it.absoluteFilePath();
    }
    m_depth_stack.push(m_loading_stack.size());
}

bool LocalRepositoryLoader::canFetchMore() const
{
    return !m_loading_stack.empty();
}

std::vector<AbstractRepositoryLoader::Entry> LocalRepositoryLoader::fetchMore()
{
    std::vector<Entry> result;
    while (!m_loading_stack.empty() && result.size() < m_items_to_fetch) {
        const QString current_path = m_loading_stack.top();
        m_loading_stack.pop();
        m_depth_stack.top() -= 1;

        const QFileInfo info(current_path);
        if (info.isDir() || (info.isFile() && _is_markdown_document(current_path))) {
            result.push_back(
                Entry{/*file_name=*/info.fileName().toStdString(),
                      /*   is_dir=*/info.isDir(),
                      /*    depth=*/m_depth_stack.size(),
                      /*file_path=*/info.absoluteFilePath().toStdString() }
                );
        }

        if (m_depth_stack.top() == 0) {
            m_depth_stack.pop();
        }

        if (info.isDir()) {
            const auto subentries = _get_file_list(current_path);
            if (!subentries.isEmpty()) {
                m_depth_stack.push(subentries.size());
                for (const auto &entry : subentries) {
                    m_loading_stack.push(entry.absoluteFilePath());
                }
            }
        }
    }
    return result;
}

void LocalRepositoryLoader::setFetchNumber(const int n)
{
    m_items_to_fetch = n;
}

QFileInfoList LocalRepositoryLoader::_get_file_list(const QString &path) const
{
    auto dir = QDir(path, "*.md", QDir::DirsLast,
                    QDir::Files | QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    qDebug() << dir;

    const QFileInfoList entries = dir.entryInfoList();
    for (auto &entry : entries) {
        qDebug() << entry.exists() << entry.size() << entry.canonicalFilePath();
    }
    return entries;
}

bool LocalRepositoryLoader::_is_markdown_document(const QString &path) const
{
    return true;  // TODO
}
