#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <set>
#include <string>

class spell_checker {
public:
    spell_checker();

    [[nodiscard]] bool is_correct(const std::string &word) const;

private:
    std::set<std::string> dictionary;
};

#endif // SPELL_CHECKER_H