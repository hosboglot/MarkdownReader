#ifndef ABSTRACTDOCUMENTLOADER_H
#define ABSTRACTDOCUMENTLOADER_H

#include <vector>
#include <string>
#include <memory>

class AbstractDocumentLoader
{
public:
    struct Block {
        std::string content{""};
        std::vector<std::string> tags{};
        std::string badge_text{""};
        std::string color{""};
        std::string type{"text"};
    };

    virtual ~AbstractDocumentLoader() {};

    virtual bool isValid() const = 0;
    virtual std::string version() const = 0;
    virtual std::vector<std::string> tags() const = 0;

    virtual bool canFetchMore() const = 0;
    virtual std::vector<std::unique_ptr<Block>> fetchMore() = 0;
    virtual void setFetchNumber(const int n) = 0;
};

#endif // ABSTRACTDOCUMENTLOADER_H
