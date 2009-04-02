#ifndef CFIRUDATA_H_
#define CFIRUDATA_H_

#include <e32base.h>
#include <d32dbms.h>
#include <badesca.h>

class CFiruTranslation;

// ----------------------------------------------------------

class CFiruEntry : public CBase
{
public:
    static CFiruEntry* NewLC( TInt aId, const TDesC& aText );

    virtual ~CFiruEntry();

    TInt Id() const { return iId; };
    TPtrC Text() const { return *iText; };

    void AddTranslationL( CFiruTranslation* aTarget );
    TUint NumTranslations() const;

    CFiruTranslation* Translation( TUint aIndex );
    TPtrC TranslationText( TUint aIndex ) const;

protected:
    CFiruEntry();

private:
    TInt iId;
    HBufC* iText;
    RPointerArray<CFiruTranslation> iTranslations;
};

// ----------------------------------------------------------

class CFiruTranslation : public CBase
{
public:
    CFiruTranslation( CFiruEntry* aEntry, TInt aCounter, TInt aRate )
    : iEntry( aEntry ), iCounter( aCounter ), iRate( aRate ) {};
    
    ~CFiruTranslation();

    TInt Counter() const { return iCounter; };
    TInt Rate() const { return iRate; };

    CFiruEntry& Entry() { return *iEntry; };

private:
    CFiruEntry* iEntry;
    TInt iCounter;
    TInt iRate;
};

// ----------------------------------------------------------

class CFiruTest : public CBase
{
public:
    virtual ~CFiruTest();

    static CFiruTest* NewLC(
        CFiruEntry* aQuestion,
        CFiruEntry* aAnswer,
        TBool aReversed );

    void AddWrongVariantL( const TDesC& text );

    TPtrC QuestionText() const;
    TPtrC AnswerText() const;

    TBool TryVariant( TUint aIndex );
    TBool TryAnswer( const TDesC& aText );
    const RPointerArray<HBufC>& AllVariants() const;

    TBool IsPassed() const { return iPassed; };
    TUint NumMistakes() const { return iNumMistakes; };

    TBool IsReversed() const { return iReversed; };
    const CFiruEntry& Question() const { return *iQuestion; };
    const CFiruEntry& Answer() const { return *iAnswer; };

protected:
    CFiruTest();

private:
    CFiruEntry* iQuestion, *iAnswer;
    TBool iReversed;

    TInt iCorrectTranslationIndex;
    RPointerArray<HBufC> iVariants;

    TBool iPassed;
    TUint iNumMistakes;
};

// ----------------------------------------------------------

class CFiruExercise : public CBase
{
public:
    CFiruExercise();
    virtual ~CFiruExercise();

    void AddTestL( CFiruTest* );
    CFiruTest* NextTest();

    class Stats
    {
    public:
        Stats( TInt passed, TInt asked ) : iPassed( passed ), iAsked( asked ) {};

        TInt Passed() const { return iPassed; };
        TInt Asked() const { return iAsked; };
    private:
        Stats();

        TInt iPassed;
        TInt iAsked;
    };

    Stats GetStats() const;

protected:
private:
    RPointerArray<CFiruTest> iTests;
    TInt iCounter;
};

// ----------------------------------------------------------

class CFiruData : public CBase
{
public:
	virtual ~CFiruData();
	static CFiruData* NewL( RFs& aFs );

	void SelectDictionaryL( TLanguage aInputLanguage, TLanguage aOutputLanguage );
    void ReverseDictionaryL();
	void GetLanguagesL( TLanguage& aInputLanguage, TLanguage& aOutputLanguage ) const;
	TInt NumEntriesL() const;

    TInt AddEntryL( const TDesC& aEntry );
    TInt AddTranslationL( TInt aEntryId, const TDesC& aTranslation );
    TInt AddExampleL( TInt aEntryId, TInt aTranslationId, const TDesC& aExample );

    void GetEntriesL( RPointerArray<CFiruEntry>& aEntries, const TDesC& aPattern, TInt aMax = 10 );
    void GetMoreEntriesL( RPointerArray<CFiruEntry>& aEntries, TInt aMax = 10 );

    void GetTranslationsL( CFiruEntry& aEntry );

    CFiruExercise* CreateExerciseL(
        TInt numTotalTests,
        TInt numNewTests,
        TInt numVariants );

    void SaveTestResultL( const CFiruTest& aTest );
    void AddEntryToLearningSetL( TInt aEntryId );

    struct Stats
    {
        TInt iTotalEntries;
        TInt iNotAsked;
        TInt iPositives[3]; // [0]- +0, [1]- +1, [2]- >=2
        TInt iNegatives[3]; // [0]- -1, [1]- -2, [2]- <-2
    };

    Stats GetStats();
    void ResetStatsL();
    void ClearDictionaryL();

    void CompactL();

protected:
	CFiruData( RFs& aFs );
	void ConstructL();

	void OpenDatabaseL();
	void CreateDictionaryL( const TDesC& aDictName );
	CFiruEntry* CreateEntryLC( RDbRowSet& aView );
	void EvaluateViewL( RDbView& aView, const TDesC& query );

    void ReadTopEntriesL(
        RDbRowSet& aView,
        RPointerArray<CFiruEntry>& entries,
        TInt aCount,
        TBool aReversed );

    void ReadRandomEntriesL(
        RDbRowSet& aView,
        RPointerArray<CFiruEntry>& entries,
        TInt aCount,
        TBool aReversed );

    TBool FindTranslationRowL( RDbView& aView, TInt aEntryId, TInt aTargetId );

    TInt AddEntryL( const TDesC& aEntry, TBool aReversed );

    void AdjustMarkL(
        TInt aEntryID, TInt aTargetID,
        TInt aMark, TBool aShiftMark,
        TBool aUpdateCounter );

private:

    void SetTableIndexL();
    void DumpL();

private:
    TLanguage iSourceLang;
    TLanguage iTargetLang;
    TBool iReversed;

    RFs& iFs;
    RDbNamedDatabase iDb;

    HBufC*  iTableNameSources;
    HBufC*  iTableNameTargets;
    HBufC*  iTableNameTranslations;

    RDbTable iTableSources;
    RDbTable iTableTargets;
    RDbTable iTableTranslations;
    RDbTable iTableSourcesById;
    RDbTable iTableTargetsById;
};

#endif /*CFIRUDATA_H_*/
