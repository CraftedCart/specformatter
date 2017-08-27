#include "Section.hpp"

namespace SpecFormatter {

    Section::~Section() {
        qDeleteAll(entries);
    }

    void Section::setId(QString id) {
        this->id = id;
    }

    QString Section::getId() {
        return id;
    }

    void Section::setName(QString name) {
        this->name = name;
    }

    QString Section::getName() {
        return name;
    }

    void Section::addEntry(Entry *entry) {
        entries.append(entry);
    }

    QVector<Entry*>& Section::getEntries() {
        return entries;
    }

}

