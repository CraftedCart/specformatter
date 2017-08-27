#include "Entry.hpp"

namespace SpecFormatter {

    void Entry::setLength(unsigned int length) {
        this->length = length;
    }

    unsigned int Entry::getLength() {
        return length;
    }

    void Entry::setFlags(QString flags) {
        this->flags = flags;
    }

    QString Entry::getFlags() {
        return flags;
    }

    void Entry::setInfo(QString info) {
        this->info = info;
    }

    QString Entry::getInfo() {
        return info;
    }

    void Entry::setLink(QString link) {
        this->link = link;
    }

    QString Entry::getLink() {
        return link;
    }

}

