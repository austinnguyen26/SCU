# ifndef LABEL_H
# define LABEL_H

#include <ostream>
// Lecture 21 Slide 6

class Label{
    static unsigned _counter;
    unsigned _number;

public:
    Label();
    unsigned number() const;
};

std::ostream &operator <<(std::ostream &ostr, const Label &label);

# endif