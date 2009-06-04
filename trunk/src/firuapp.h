#ifndef FIRUAPP_H
#define FIRUAPP_H

#include <QApplication>
#include <QSettings>

#include "model/model.h"

class Settings;

class FiruApp : public QApplication
{
public:
    FiruApp( int argc, char *argv[] );
    static FiruApp* get();
    static Settings& settings();

private:
    FiruApp();
    Settings* m_settings;
};

class Settings
{
public:
    Settings();

    Lang getSourceLanguage() const;
    Lang getTargetLanguage() const;

    void setSourceLanguage( Lang src ) const;
    void setTargetLanguage( Lang trg ) const;

private:
//    QSettings m_settings;
    Lang m_src;
    Lang m_trg;
};

#endif // FIRUAPP_H
