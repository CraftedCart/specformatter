#ifndef SPECFORMATTER_NOTE_HPP
#define SPECFORMATTER_NOTE_HPP

#include <QString>
#include <QVector>

namespace SpecFormatter {
    class Note {
        protected:
            QString text;
            QVector<Note*> notes;

        public:
            void setText(QString text);
            QString getText();
            void addNote(Note *note);
            QVector<Note*> getNotes();
    };
}

#endif

