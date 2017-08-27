#ifndef SPECFORMATTER_SPEC_HPP
#define SPECFORMATTER_SPEC_HPP

#include "Section.hpp"
#include <QString>
#include <QVector>

namespace SpecFormatter {
    class Spec {
        protected:
            QString id;
            QString name;
            QString linkImage;
            QVector<QString> notes;
            QVector<Flag*> flags;
            QVector<Section*> sections;

        public:
            ~Spec();

            void setId(QString id);
            QString getId();
            void setName(QString name);
            QString getName();
            void setLinkImage(QString linkImage);
            QString getLinkImage();
            void addNote(QString note);
            QVector<QString>& getNotes();
            void addFlag(Flag *flag);
            QVector<Flag*>& getFlags();
            void addSection(Section *section);
            QVector<Section*>& getSections();
    };
}

#endif

