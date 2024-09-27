#ifndef LOCALREPOSITORYLOADER_H
#define LOCALREPOSITORYLOADER_H

#include "abstractrepositoryloader.h"

#include <QUrl>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>

#include <stack>

class LocalRepositoryLoader : public AbstractRepositoryLoader
{
public:
    explicit LocalRepositoryLoader(QUrl path);

    bool canFetchMore() const override;
    std::vector<Entry> fetchMore() override;
    void setFetchNumber(const int n) override;
private:
    QFileInfoList _get_file_list(const QString &path) const;
    bool _is_markdown_document(const QString &path) const;

    std::stack<QString> m_loading_stack;
    std::stack<int> m_depth_stack;
    int m_items_to_fetch{10};
};

#endif // LOCALREPOSITORYLOADER_H
