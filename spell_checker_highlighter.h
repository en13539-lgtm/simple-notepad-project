#ifndef SPELL_CHECKER_HIGHLIGHTER_H
#define SPELL_CHECKER_HIGHLIGHTER_H

#include "spell_checker.h"
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class spell_checker_highlighter : public QSyntaxHighlighter {
public:
    explicit spell_checker_highlighter(QTextDocument *parent, const spell_checker &checker);

protected:
    void highlightBlock(const QString &text) override;

private:
    const spell_checker &checker;
    QTextCharFormat misspelled_format;

    [[nodiscard]] static std::string normalize_word(const QString &word);
};

#endif // SPELL_CHECKER_HIGHLIGHTER_H