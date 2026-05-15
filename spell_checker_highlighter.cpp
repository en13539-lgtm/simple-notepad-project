#include "spell_checker_highlighter.h"

#include <QRegularExpression>

#include <algorithm>
#include <cctype>

spell_checker_highlighter::spell_checker_highlighter(
        QTextDocument *parent, const spell_checker &checker) : QSyntaxHighlighter(parent), checker(checker) {
    misspelled_format.setUnderlineColor(Qt::red);
    misspelled_format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
}

void spell_checker_highlighter::highlightBlock(const QString &text) {
    static const QRegularExpression word_regex("[A-Za-z]+");

    auto match_iterator = word_regex.globalMatch(text);
    while (match_iterator.hasNext()) {
        const auto match = match_iterator.next();
        const QString word = match.captured();

        const std::string normalized = normalize_word(word);
        if (!normalized.empty() && !checker.is_correct(normalized)) {
            setFormat(match.capturedStart(), match.capturedLength(), misspelled_format);
        }
    }
}

std::string spell_checker_highlighter::normalize_word(const QString &word) {
    std::string result = word.toStdString();
    std::ranges::transform(result, result.begin(), [](const unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });

    return result;
}