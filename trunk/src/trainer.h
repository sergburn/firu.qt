#ifndef TRAINER_H
#define TRAINER_H

#include <QObject>
#include <QPointer>

#include "trainerdialog.h"
#include "model/model.h"
#include "model/dictionary.h"
#include "model/exercise.h"

class Trainer : public QObject
{
    Q_OBJECT
public:
    SHARED_POINTER( Trainer );

    Trainer( QObject* parent = NULL );

    void start( Dictionary::Ptr dictionary );

signals:
    void onExerciseBuildProgress( int progress );

    /** Reports step > count when exersice is finished */
    void onExerciseProgress( int step, int count );

private:
    Trainer();
    Q_DISABLE_COPY( Trainer );

    void showNextTest();

private slots:
    void handleTestDone();
    void handleTestCancel();

private:
    Exercise::Ptr m_exe;

    QPointer<TrainerDialog> m_dialog;
};

#endif // TRAINER_H
