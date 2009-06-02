#ifndef MODEL_H
#define MODEL_H

#include <QLocale>

typedef QLocale::Language Lang;

enum TextMatch
{
    FullMatch,
    StartsWith,
    Contains,
    EndsWith
};

#endif // MODEL_H
