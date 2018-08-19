#ifndef SPECFORMATTER_SPECFORMATTER_HPP
#define SPECFORMATTER_SPECFORMATTER_HPP

#include "Spec.hpp"
#include <QXmlStreamReader>
#include <QFile>
#include <QCommandLineParser>

namespace SpecFormatter {
    int exec(int argc, char *argv[]);
    void parseSpec(QXmlStreamReader &xml, Spec *spec);
    void parseNoteList(QXmlStreamReader &xml, Note *note);
    void parseSection(QXmlStreamReader &xml, Section *section);
    void writeHtml(QFile &file, QVector<Spec*> specs, QCommandLineParser &cmd);
}

#endif

