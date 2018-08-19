#include "SpecFormatter.hpp"
#include <cstdlib>
#include <QDebug>
#include <QXmlStreamWriter>

namespace SpecFormatter {

    int exec(int argc, char *argv[]) {
        //Qt and CLI parsing
        QCoreApplication app(argc, argv);
        QCoreApplication::setApplicationName("specformatter");

        QCommandLineParser cmd;
        cmd.setApplicationDescription("Parses XML file specs and processes them into a more readable form");
        cmd.addHelpOption();
        cmd.addPositionalArgument("spec", "XML file spec");
        cmd.addPositionalArgument("output", "Target file to output to");

        cmd.addOptions({
                {
                "table-class",
                "Set the class attribute for table tags",
                "value"
                }
                });

        cmd.process(app);

        const QStringList args = cmd.positionalArguments();
        if (args.length() != 2) cmd.showHelp(EXIT_FAILURE);

        //Start reading and parsing and doing magical stuff
        QFile file(args.at(0));
        QFile outFile(args.at(1));

        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open the file";
            return EXIT_FAILURE;
        }

        QXmlStreamReader xml(&file);
        QVector<Spec*> specs;

        while (!xml.atEnd()) {
            xml.readNext();

            if (!xml.isStartElement()) continue;

            if (xml.name() == "spec") {
                //Found spec
                Spec *spec = new Spec();
                specs.append(spec);

                foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                    if (attr.name().toString() == "id") spec->setId(attr.value().toString());
                    if (attr.name().toString() == "name") spec->setName(attr.value().toString());
                    if (attr.name().toString() == "linkImage") spec->setLinkImage(attr.value().toString());
                }

                qDebug() << "Found spec id:" << spec->getId() << "name:" << spec->getName() << "linkImage:" << spec->getLinkImage();
                parseSpec(xml, spec);
            }
        }

        if (xml.hasError()) {
            qCritical() << "Error while reading XML" << xml.errorString();
            return EXIT_FAILURE;
        }

        writeHtml(outFile, specs, cmd);

        //Cleanup
        qDeleteAll(specs);

        return EXIT_SUCCESS;
    }

    void parseSpec(QXmlStreamReader &xml, Spec *spec) {
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isEndElement() && xml.name() == "spec") return; //End of spec
            if (!xml.isStartElement()) continue;

            if (xml.name() == "flag") {
                //Found flag
                Flag *flag = new Flag();
                spec->addFlag(flag);

                foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                    if (attr.name().toString() == "id") flag->setId(attr.value().toString()[0]);
                    if (attr.name().toString() == "info") flag->setInfo(attr.value().toString());
                    if (attr.name().toString() == "image") flag->setImage(attr.value().toString());
                }

                qDebug() << "    Found flag id:" << flag->getId() << "info:" << flag->getInfo() + "image:" << flag->getImage();
            } else if (xml.name() == "note") {
                //Found note
                Note *note = new Note();
                note->setText(xml.readElementText());
                spec->addNote(note);
                qDebug() << "    Found note:" << note->getText();
            } else if (xml.name() == "note-list") {
                //Found note list
                Note *note = new Note();
                spec->addNote(note);
                qDebug() << "    Found note list";
                parseNoteList(xml, note);
            } else if (xml.name() == "section") {
                //Found section
                Section *section = new Section();
                spec->addSection(section);

                foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                    if (attr.name().toString() == "id") section->setId(attr.value().toString());
                    if (attr.name().toString() == "name") section->setName(attr.value().toString());
                }

                qDebug() << "    Found section id:" << section->getId() << "name:" << section->getName();
                parseSection(xml, section);
            }
        }
    }

    void parseNoteList(QXmlStreamReader &xml, Note *note) {
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isEndElement() && xml.name() == "note-list") return;
            if (!xml.isStartElement()) continue;

            if (xml.name() == "note") {
                Note *newNote = new Note();
                newNote->setText(xml.readElementText());
                note->addNote(newNote);
                qDebug() << "        Found note:" << newNote->getText();
            } else if (xml.name() == "note-list") {
                qWarning() << "        Nested note lists are not supported";
            }
        }
    }

    void parseSection(QXmlStreamReader &xml, Section *section) {
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isEndElement() && xml.name() == "section") return; //End of section
            if (!xml.isStartElement()) continue;

            if (xml.name() == "entry") {
                //Found entry
                Entry *entry = new Entry();
                section->addEntry(entry);

                foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                    if (attr.name().toString() == "length") entry->setLength(attr.value().toUInt());
                    if (attr.name().toString() == "flags") entry->setFlags(attr.value().toString());
                    if (attr.name().toString() == "info") entry->setInfo(attr.value().toString());
                    if (attr.name().toString() == "link") entry->setLink(attr.value().toString());
                }

                qDebug() << "        Found entry length:" << entry->getLength() << "flags:" << entry->getFlags() << "info:" << entry->getInfo() << "link:" << entry->getLink();
            }
        }
    }

    void writeHtml(QFile &file, QVector<Spec*> specs, QCommandLineParser &cmd) {
        file.open(QIODevice::WriteOnly);

        qDebug() << "Writing HTML File";

        QXmlStreamWriter xml(&file);
        //xml.setAutoFormatting(true); //Commented out as we don't need whitespace only text nodes everywhere
        //xml.writeStartDocument(); //Don't add the XML declaration to HTML

        //Add auto generated warning comment
        xml.writeComment("This is auto-generated. Do not edit.");

        foreach(Spec* spec, specs) {
            qDebug() << "    Writing spec to file";
            //Add spec title header
            xml.writeStartElement("h1"); //TODO: Make headers configurable
            xml.writeAttribute("id", "spec-" + spec->getId());
            xml.writeCharacters(spec->getName());
            xml.writeEndElement();

            //Add notes
            qDebug() << "        Writing notes";
            QVector<Note*> notes = spec->getNotes();
            if (notes.size() > 0) {
                xml.writeTextElement("h2", "Notes"); //TODO: Make headers configurable

                xml.writeStartElement("ul");

                foreach(Note* note, notes) {
                    if (note->getText() != "") {
                        xml.writeTextElement("li", note->getText());
                    } else {
                        xml.writeStartElement("ul");
                        foreach(Note* listNote, note->getNotes()) {
                            xml.writeTextElement("li", listNote->getText());
                        }
                        xml.writeEndElement();
                    }
                }

                xml.writeEndElement();
            }

            //Add table of contents
            qDebug() << "        Writing TOC";
            xml.writeTextElement("h2", "Sections"); //TODO: Make headers configurable
            xml.writeStartElement("ul");
            foreach(Section* section, spec->getSections()) {
                xml.writeStartElement("li");
                xml.writeStartElement("a");
                xml.writeAttribute("href", "#spec-" + spec->getId() + "-section-" + section->getId());
                xml.writeCharacters(section->getName());
                xml.writeEndElement();
                xml.writeEndElement();
            }
            xml.writeEndElement();


            //Add flags key
            qDebug() << "        Adding flags";
            xml.writeTextElement("h2", "Flags"); //TODO: Make headers configurable
            xml.writeStartElement("ul");
            foreach(Flag* flag, spec->getFlags()) {
                xml.writeStartElement("li");
                xml.writeStartElement("img");
                xml.writeAttribute("src", flag->getImage());
                xml.writeAttribute("title", flag->getInfo());
                xml.writeEndElement();
                xml.writeCharacters(" " + flag->getInfo()); //Add a bit of spacing between the image
                xml.writeEndElement();
            }
            xml.writeEndElement();

            //Add sections
            qDebug() << "        Adding sections";
            foreach(Section* section, spec->getSections()) {
                //Add section title header
                xml.writeStartElement("h2"); //TODO: Make headers configurable
                xml.writeAttribute("id", "spec-" + spec->getId() + "-section-" + section->getId());
                xml.writeCharacters(section->getName());

                xml.writeCharacters(" "); //Add spacing

                xml.writeStartElement("a");
                xml.writeAttribute("href", "#spec-" + spec->getId() + "-section-" + section->getId());
                xml.writeStartElement("img");
                xml.writeAttribute("src", spec->getLinkImage());
                xml.writeEndElement();
                xml.writeEndElement();

                xml.writeEndElement(); //h2

                //Section length
                unsigned int sectionLength = 0;
                foreach(Entry *entry, section->getEntries()) sectionLength += entry->getLength();
                xml.writeTextElement("h4", "Length: 0x" + QString::number(sectionLength, 16).toUpper() + "/" + QString::number(sectionLength));

                xml.writeStartElement("table");
                if (cmd.isSet("table-class")) {
                    xml.writeAttribute("class", cmd.value("table-class"));
                }

                //Add the table headers
                xml.writeStartElement("tr");
                xml.writeTextElement("th", "Relative Offset (Hex)");
                xml.writeTextElement("th", "Relative Offset (Dec)");
                xml.writeTextElement("th", "Length (Hex)");
                xml.writeTextElement("th", "Length (Dec)");
                xml.writeTextElement("th", "Flags");
                xml.writeTextElement("th", "Info");
                xml.writeTextElement("th", "Link");
                xml.writeEndElement(); //tr

                //Add all entries
                unsigned int relOffset = 0;
                foreach(Entry *entry, section->getEntries()) {
                    QString hexOffset;
                    hexOffset.setNum(relOffset, 16);
                    hexOffset = "0x" + hexOffset.toUpper(); //Upper case hex
                    QString hexLength;
                    hexLength.setNum(entry->getLength(), 16);
                    hexLength = "0x" + hexLength.toUpper(); //Upper case hex

                    xml.writeStartElement("tr");

                    //xml.writeTextElement("td", hexOffset);
                    xml.writeStartElement("td"); xml.writeTextElement("code", hexOffset); xml.writeEndElement();
                    xml.writeTextElement("td", QString::number(relOffset));
                    //xml.writeTextElement("td", hexLength);
                    xml.writeStartElement("td"); xml.writeTextElement("code", hexLength); xml.writeEndElement();
                    xml.writeTextElement("td", QString::number(entry->getLength()));

                    //Flags
                    xml.writeStartElement("td");
                    for (int i = 0; i < entry->getFlags().length(); i++) {
                        QChar flagId = entry->getFlags()[i];
                        Flag *flag = nullptr;

                        //Find the flag
                        foreach(Flag *checkFlag, spec->getFlags()) {
                            if (checkFlag->getId() == flagId) flag = checkFlag;
                        }

                        //Add the flag image
                        if (flag != nullptr) {
                            xml.writeStartElement("img");
                            xml.writeAttribute("src", flag->getImage());
                            xml.writeAttribute("title", flag->getInfo());
                            xml.writeEndElement();
                        } else {
                            qWarning() << "        Element" << i << "in section" << section->getName() << "references non-existent flag";
                        }
                    }
                    xml.writeEndElement();

                    xml.writeTextElement("td", entry->getInfo());

                    //Link
                    if (entry->getLink() != "") {
                        xml.writeStartElement("td");
                        xml.writeStartElement("a");
                        xml.writeAttribute("href", entry->getLink());
                        xml.writeStartElement("img");
                        xml.writeAttribute("src", spec->getLinkImage());
                        xml.writeEndElement();
                        xml.writeEndElement();
                        xml.writeEndElement();
                    } else {
                        xml.writeTextElement("td", "");
                    }

                    xml.writeEndElement(); //tr

                    relOffset += entry->getLength();
                }

                xml.writeEndElement(); //table
            }
        }

        xml.writeEndDocument();
    }

}

