#ifndef CFIRUDATA_H_
#define CFIRUDATA_H_

#include <e32base.h>
#include <d32dbms.h>
#include <badesca.h>

// ----------------------------------------------------------

class CFiruDataEntry : public CBase
{
public:
    static CFiruDataEntry* NewLC(
        TInt aId, const TDesC& aText, TInt aCounter, TInt aRate );

    virtual ~CFiruDataEntry();

    TInt Id() const;
    TPtrC Text() const;

    TInt Counter() const { return iCounter; };
    TInt Rate() const { return iRate; };

    TUint NumTranslations() const;
    TPtrC Translation( TUint aIndex ) const;

    const MDesC16Array& Translations() const;

    void AddTranslationL( const TDesC& aText );

protected:
    CFiruDataEntry();

private:
    TInt iId;
    HBufC* iText;
    TInt iCounter;
    TInt iRate;
    CDesC16ArrayFlat* iTranslations;
};

// ----------------------------------------------------------

class CFiruTest : public CBase
{
public:
    virtual ~CFiruTest();

    static CFiruTest* NewLC(
        CFiruDataEntry* aEntry,
        TBool aReversed );

    void AddWrongVariantL( const TDesC& entry );

    TPtrC Question() const;
    TPtrC Answer() const;

    TBool TryVariant( TUint aIndex );
    const RPointerArray<HBufC>& AllVariants() const;

    TBool IsPassed() const { return iPassed; };
    TUint NumMistakes() const { return iNumMistakes; };

    TBool IsReversed() const { return iReversed; };
    const CFiruDataEntry& Entry() const { return *iEntry; };

protected:
    CFiruTest();

private:
    CFiruDataEntry* iEntry;
    TBool iReversed;

    TInt iCorrectTranslationIndex;
    RPointerArray<HBufC> iVariants;

    TBool iPassed;
    TUint iNumMistakes;

    TInt64 iRandSeed;
};

// ----------------------------------------------------------

class CFiruExercise : public CBase
{
public:
    CFiruExercise();
    virtual ~CFiruExercise();

    void AddTestL( CFiruTest* );
    CFiruTest* NextTest();

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
    void AddPair( const TDesC& aEntry, const TDesC& aTranslation );
    void AddPairL( const TDesC& aEntry, const TDesC& aTranslation );
    void ReverseDictionaryL();
	void GetLanguagesL( TLanguage& aInputLanguage, TLanguage& aOutputLanguage ) const;
	TInt NumEntriesL() const;

//    void SetFilterL( const TDesC& aEntryPattern );
//    void ResetFilterL();

    void GetEntriesL( RPointerArray<CFiruDataEntry>& aEntries, const TDesC& aPattern, TInt aMax = 10 );
    void GetMoreEntriesL( RPointerArray<CFiruDataEntry>& aEntries, TInt aMax = 10 );

    CFiruDataEntry* TranslationLC( TInt aEntryID );

    CFiruExercise* CreateExerciseL(
        TInt numTotalTests,
        TInt numNewTests,
        TInt numVariants );

    void SaveTestResultL( const CFiruTest& aTest );

    void ResetStatsL();
    void ClearDictionaryL();

    void CompactL();

protected:
	CFiruData( RFs& aFs );
	void ConstructL();

	void OpenDatabaseL();
	void CreateDictionaryL( const TDesC& aDictName );
	CFiruDataEntry* CreateEntryLC( RDbRowSet& aView, TBool aReverse );
	void FindEntryRowL( RDbView& aView, TInt aEntryID );
	void AdjustMarkL( TInt aEntryID, TInt aCorrection, TBool aReversed, TBool aUpdateCounter );
	void EvaluateViewL( RDbView& aView, const TDesC& query );

    void ReadTopEntriesL(
        RDbRowSet& aView,
        RPointerArray<CFiruDataEntry>& entries,
        TInt aCount,
        TBool aReversed );

    void ReadRandomEntriesL(
        RDbRowSet& aView,
        RPointerArray<CFiruDataEntry>& entries,
        TInt aCount,
        TBool aReversed );

private:
    static HBufC* DictionaryTableNameLC( TLanguage aInputLanguage, TLanguage aOutputLanguage );
    static HBufC* ForwardIndexNameLC( const TDesC& aTableName );
    static HBufC* ReverseIndexNameLC( const TDesC& aTableName );

    void SetTableIndexL();

private:
    TLanguage iSourceLang;
    TLanguage iTargetLang;
    TBool iReversed;

    RFs& iFs;
    RDbNamedDatabase iDb;
    HBufC*  iTableName;
    RDbTable iTable;

    TInt64 iRandSeed;
};

#endif /*CFIRUDATA_H_*/
