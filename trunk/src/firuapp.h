#ifndef FIRUAPP_H
#define FIRUAPP_H

#include <QApplication>
#include <QSettings>

#include "model/model.h"
#include "model/database.h"

class Settings;

class FiruApp : public QApplication
{
public:
    FiruApp( int argc, char *argv[] );
    static FiruApp* get();
    static Settings& settings();
    static Database& database();

    bool openDatabase();

private:
    FiruApp();
    Settings* m_settings;
    Database* m_database;
};

class Settings
{
public:
    Settings();

    Lang getSourceLanguage() const;
    Lang getTargetLanguage() const;

    void setSourceLanguage( Lang src );
    void setTargetLanguage( Lang trg );

private:
//    QSettings m_settings;
    Lang m_src;
    Lang m_trg;
};

#endif // FIRUAPP_H
