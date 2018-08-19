#include "Note.hpp"

namespace SpecFormatter {

    void Note::addNote(Note *note) {
        notes.append(note);
    }
    
    QVector<Note*> Note::getNotes() {
        return notes;
    }
    
    void Note::setText(QString text) {
        this->text = text;
    }
    
    QString Note::getText() {
        return text;
    }

}