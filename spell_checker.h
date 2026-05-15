#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <set>
#include <string>
#include <vector>

class spell_checker {
public:
    spell_checker();
    [[nodiscard]] std::vector<std::string>
    suggest(const std::string& word) const;
    [[nodiscard]] bool is_correct(const std::string &word) const;

private:
    std::set<std::string> dictionary;
};

#endif // SPELL_CHECKER_H