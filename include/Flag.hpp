#ifndef SPECFORMATTER_FLAG_HPP
#define SPECFORMATTER_FLAG_HPP

#include <QString>

namespace SpecFormatter {
    class Flag {
        protected:
            QChar id;
            QString info;
            QString image;

        public:
            void setId(QChar id);
            QChar getId();
            void setInfo(QString info);
            QString getInfo();
            void setImage(QString image);
            QString getImage();
    };
}

#endif

