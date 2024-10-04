#ifndef LOCALDOCUMENTLOADER_H
#define LOCALDOCUMENTLOADER_H

#include "abstractdocumentloader.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QUrl>
#include <QString>

#include <string>
#include <vector>
#include <stack>
#include <memory>

class LocalDocumentLoader : public AbstractDocumentLoader
{
public:
    explicit LocalDocumentLoader(const QUrl &path);

    bool isValid() const override;
    std::string version() const override;
    std::vector<std::string> tags() const override;

    bool canFetchMore() const override;
    std::vector<std::unique_ptr<Block>> fetchMore() override;
    void setFetchNumber(const int n) override;
private:
    static QJsonValue _extract_json_from_comment(const QString meta_name, const QString &comment);
    static void _extract_meta(Block &block, const QVariantMap &meta_map);

    void _read_block(Block &block);
    void _read_text(Block &block);
    void _read_video(Block &block);
    void _read_html(Block &block);
    void _read_code(Block &block);

    QUrl m_path;
    QFile m_doc;
    QTextStream m_stream;
    QVariantMap m_metainfo;
    bool _is_valid;

    int m_items_to_fetch{10};
};

#endif // LOCALDOCUMENTLOADER_H
