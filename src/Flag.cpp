#include "Flag.hpp"

namespace SpecFormatter {

    void Flag::setId(QChar id) {
        this->id = id;
    }

    QChar Flag::getId() {
        return id;
    }

    void Flag::setInfo(QString info) {
        this->info = info;
    }

    QString Flag::getInfo() {
        return info;
    }

    void Flag::setImage(QString image) {
        this->image = image;
    }

    QString Flag::getImage() {
        return image;
    }

}

