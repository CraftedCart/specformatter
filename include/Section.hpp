#ifndef SPECFORMATTER_SECTION_HPP
#define SPECFORMATTER_SECTION_HPP

#include "Flag.hpp"
#include "Entry.hpp"
#include <QVector>

namespace SpecFormatter {
    class Section {
        protected:
            QString id;
            QString name;
            QVector<Entry*> entries;

        public:
            ~Section();

            void setId(QString id);
            QString getId();
            void setName(QString name);
            QString getName();
            void addEntry(Entry *entry);
            QVector<Entry*>& getEntries();
    };
}

#endif

