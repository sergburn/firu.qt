#ifndef FIRUAPP_H
#define FIRUAPP_H

#include <QApplication>
#include <QSettings>

#include "trainerdialog.h"

#include "trainer.h"

#include "model/model.h"
#include "model/database.h"
#include "model/dictionary.h"

class Settings;

class FiruApp : public QApplication
{
public:
    FiruApp( int argc, char *argv[] );
    ~FiruApp();

    static FiruApp* get();
    static Settings& settings();
    static Database& database();

    static QStringList getKeypadGroups( Lang lang );

    bool openDatabase();

    static void startTrainer( Dictionary::Ptr dictionary );
    static TrainerDialog* getTrainerDialog();

private:
    FiruApp();
    Settings* m_settings;
    Database* m_database;
    TrainerDialog* m_dlgTrainer;

#ifdef __SYMBIAN32__    
    RFs iFs;
#endif
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
