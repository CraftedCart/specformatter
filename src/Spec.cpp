#include "Spec.hpp"

namespace SpecFormatter {

    Spec::~Spec() {
        qDeleteAll(notes);
        qDeleteAll(flags);
        qDeleteAll(sections);
    }

    void Spec::setId(QString id) {
        this->id = id;
    }

    QString Spec::getId() {
        return id;
    }

    void Spec::setName(QString name) {
        this->name = name;
    }

    QString Spec::getName() {
        return name;
    }

    void Spec::setLinkImage(QString linkImage) {
        this->linkImage = linkImage;
    }

    QString Spec::getLinkImage() {
        return linkImage;
    }

    void Spec::addNote(Note *note) {
        notes.append(note);
    }

    QVector<Note*>& Spec::getNotes() {
        return notes;
    }

    void Spec::addFlag(Flag *flag) {
        flags.append(flag);
    }

    QVector<Flag*>& Spec::getFlags() {
        return flags;
    }

    void Spec::addSection(Section *section) {
        sections.append(section);
    }

    QVector<Section*>& Spec::getSections() {
        return sections;
    }

}

