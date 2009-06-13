/*
 ========================================================================
 Name        : FiruDocument.cpp
 Author      :
 Copyright   : Your copyright notice
 Description :
 ========================================================================
 */
#include <s32file.h>
#include <utf.h>

#include "FiruData.h"

// [[[ begin generated region: do not modify [Generated User Includes]
#include "FiruDocument.h"
#include "FiruAppUi.h"
// ]]] end generated region [Generated User Includes]

_LIT( KNameInput, "dict.txt" );

/**
 * @brief Constructs the document class for the application.
 * @param anApplication the application instance
 */
CFiruDocument::CFiruDocument( CEikApplication& anApplication ) :
    CAknDocument( anApplication )
{
}

// ----------------------------------------------------------
// ----------------------------------------------------------
//
CFiruDocument::~CFiruDocument()
{
    delete iData;
    iFsSession.Close();
}

/**
 * @brief Completes the second phase of Symbian object construction.
 * Put initialization code that could leave here.
 */
void CFiruDocument::ConstructL()
{
    User::LeaveIfError( iFsSession.Connect() );
    InitL();
}

/**
 * Symbian OS two-phase constructor.
 *
 * Creates an instance of CFiruDocument, constructs it, and
 * returns it.
 *
 * @param aApp the application instance
 * @return the new CFiruDocument
 */
CFiruDocument* CFiruDocument::NewL( CEikApplication& aApp )
{
    CFiruDocument* self = new ( ELeave ) CFiruDocument( aApp );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
}

/**
 * @brief Creates the application UI object for this document.
 * @return the new instance
 */
CEikAppUi* CFiruDocument::CreateAppUiL()
{
    return new ( ELeave ) CFiruAppUi;
}

// ----------------------------------------------------------
// ----------------------------------------------------------
//
void CFiruDocument::InitL()
{
    iData = CFiruData::NewL( iFsSession );
    iData->CompactL();
    iData->SelectDictionaryL( ELangFinnish, ELangRussian );
    //iData->ClearDictionaryL();
    if ( iData->NumEntriesL() == 0 )
    {
        ImportDictionaryL();
    }
}

// ----------------------------------------------------------
// ----------------------------------------------------------
//
CFiruData& CFiruDocument::Data()
{
    return *iData;
}

// ----------------------------------------------------------
// ----------------------------------------------------------
//
void CFiruDocument::ImportDictionaryL()
{
    iFsSession.SetSessionToPrivate( RFs::GetSystemDrive() );

    RFileReadStream input;
    User::LeaveIfError( input.Open( iFsSession, KNameInput(), EFileShareReadersOnly | EFileStream ) );
    CleanupClosePushL( input );

    RBuf buffer;
    buffer.CreateL( 1024 );

    TInt batch = 0;
    TBool stop = EFalse;

    TUint32 entryId = 0, transId = 0, transNum = 0;

    _LIT( KTrnStart, "[trn]");
    _LIT( KTrnEnd, "[/trn]");

    while ( !stop )
    {
        HBufC* line16 = ReadUnicodeLineLC( input, buffer );
        TPtrC line = line16->Des();

        if ( line.Length() < 1 )
            break;

        TChar firstChar = line[0];
        if ( firstChar == '#' || firstChar == 0xFEFF )
        {
            continue;
        }
        else if ( !firstChar.IsSpace() )
        {
            RDebug::Print( _L("Entry: %S"), &line );
            entryId = iData->AddEntryL( line );
            batch++;
        }
        else
        {
            // translation?
            TInt trn = line.Find( KTrnStart );
            if ( trn > 0 )
            {
                TInt trnStart = trn + KTrnStart().Length();
                TInt trnEnd = line.Find( KTrnEnd() );
                if ( trnEnd > trnStart )
                {
                    TPtrC trans = line.Mid( trnStart, trnEnd - trnStart );
//                    RDebug::Print( _L("Translation: %S"), &trans );
                    transId = iData->AddTranslationL( entryId, trans );
                }
            }

            // example?
        }

        CleanupStack::PopAndDestroy( line16 );

        if ( batch >= 500 )
        {
            iData->CompactL();
            batch = 0;
        }
    }

    CleanupStack::PopAndDestroy( &input );

    iData->CompactL();
}

// ----------------------------------------------------------
// ----------------------------------------------------------
//
HBufC* CFiruDocument::ReadLineLC( RFile& aFile, RBuf8& aBuffer )
{
    TChar KLf( 10 );
    TInt pos = aBuffer.Locate( KLf );
    while ( pos < 0 )
    {
        TBuf8<64> buf;
        aFile.Read( buf );
        if ( buf.Length() == 0 )
        {
            break;
        }
        if ( aBuffer.Length() + buf.Length() > aBuffer.MaxLength() )
        {
            aBuffer.ReAllocL( aBuffer.Length() + buf.Length() );
        }
        aBuffer.Append( buf );
        pos = aBuffer.Locate( KLf );
    }

    HBufC* line = NULL;
    if ( pos >= 0 )
    {
        RBuf str;
        str.CreateL( pos * 2 );
        CleanupClosePushL( str );
        TInt err = CnvUtfConverter::ConvertToUnicodeFromUtf8( str, aBuffer.Left( pos ) );
        if ( err )
        {
            RDebug::Print( _L("Unicode error %d at word %S\n"), err, &str );
        }
        aBuffer.Delete( 0, pos + 1 );
        line = str.AllocL();
        CleanupStack::PopAndDestroy( &str );
    }
    else
    {
        line = HBufC::NewL( 0 );
    }
    CleanupStack::PushL( line );
    return line;
}

// ----------------------------------------------------------
//
HBufC* CFiruDocument::ReadUnicodeLineLC( RFileReadStream& aFile, RBuf& aBuffer )
{
    TChar KLf( 10 );
    TInt pos = aBuffer.Locate( KLf );
    while ( pos < 0 )
    {
        TBuf<64> buf;
        aFile.ReadL( buf );
        if ( buf.Length() == 0 )
        {
            break;
        }
        if ( aBuffer.Length() + buf.Length() > aBuffer.MaxLength() )
        {
            aBuffer.ReAllocL( aBuffer.Length() + buf.Length() );
        }
        aBuffer.Append( buf );
        pos = aBuffer.Locate( KLf );
    }

    HBufC* line = aBuffer.Left( pos ).AllocLC();
    aBuffer.Delete( 0, pos + 1 );
    return line;
}