#include "spell_checker.h"
#include <QFile>
#include <QTextStream>

spell_checker::spell_checker() {
    QFile file("data/words.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();

        if (!line.isEmpty()) {
            dictionary.insert(line.toStdString());
        }
    }
}

bool spell_checker::is_correct(const std::string &word) const {
    return dictionary.contains(word);
}