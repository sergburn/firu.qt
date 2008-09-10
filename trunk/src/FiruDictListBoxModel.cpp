/*
 ============================================================================
 Name		: CFiruDictListBoxModel.cpp
 Author	  :
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CFiruDictListBoxModel implementation
 ============================================================================
 */

#include "FiruData.h"
#include "FiruDictListBoxModel.h"

// ------------------------------------------------------

CFiruDictListBoxModel::CFiruDictListBoxModel( CFiruData& aData )
: iData( aData )
{
}

// ------------------------------------------------------

CFiruDictListBoxModel::~CFiruDictListBoxModel( )
{
    iEntries.ResetAndDestroy();
}

// ------------------------------------------------------

CFiruDictListBoxModel* CFiruDictListBoxModel::NewLC( CFiruData& aData )
{
    CFiruDictListBoxModel* self = new (ELeave)CFiruDictListBoxModel( aData );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
}

// ------------------------------------------------------

CFiruDictListBoxModel* CFiruDictListBoxModel::NewL( CFiruData& aData )
{
    CFiruDictListBoxModel* self=CFiruDictListBoxModel::NewLC( aData );
    CleanupStack::Pop( ); // self;
    return self;
}

// ------------------------------------------------------

void CFiruDictListBoxModel::ConstructL( )
{
    SetFilterL( _L("") );
}

// ------------------------------------------------------

void CFiruDictListBoxModel::SetFilterL( const TDesC& aText )
{
    iData.GetEntriesL( iEntries, aText );
}

// ------------------------------------------------------

void CFiruDictListBoxModel::GetMoreL()
{
    iData.GetMoreEntriesL( iEntries );
}

// ------------------------------------------------------

CDesCArray* CFiruDictListBoxModel::GetTextListLC( )
{
    CDesCArray* words = new CDesC16ArrayFlat( Max( 1, iEntries.Count() ) );
    CleanupStack::PushL( words );

    for ( int i = 0; i < iEntries.Count(); ++i )
    {
        words->AppendL( iEntries[i]->Text() );
    }

    return words;
}

// ------------------------------------------------------

CFiruEntry* CFiruDictListBoxModel::Entry( TInt aPos )
{
    if ( aPos >=0 && aPos < iEntries.Count() )
        return iEntries[aPos];
    else
        return NULL;
}
