#ifndef ABSTRACTREPOSITORYLOADER_H
#define ABSTRACTREPOSITORYLOADER_H

#include <string>
#include <vector>

class AbstractRepositoryLoader
{
public:
    struct Entry {
        std::string name;
        bool is_dir;
        unsigned nested_depth;
        std::string path;
    };

    virtual ~AbstractRepositoryLoader() {};

    int getNumber() { return 2; }
    virtual bool canFetchMore() const = 0;
    virtual std::vector<Entry> fetchMore() = 0;
    virtual void setFetchNumber(int n) = 0;
protected:
    int m_items_to_fetch{5};
};

#endif // ABSTRACTREPOSITORYLOADER_H
