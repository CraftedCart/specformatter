#ifndef SPECFORMATTER_ENTRY_HPP
#define SPECFORMATTER_ENTRY_HPP

#include <QString>

namespace SpecFormatter {
    class Entry {
        protected:
            unsigned int length;
            QString flags;
            QString info;
            QString link;

        public:
            void setLength(unsigned int length);
            unsigned int getLength();
            void setFlags(QString flags);
            QString getFlags();
            void setInfo(QString info);
            QString getInfo();
            void setLink(QString link);
            QString getLink();
    };
}

#endif

