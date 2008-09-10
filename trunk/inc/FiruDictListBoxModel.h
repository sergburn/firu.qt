/*
 ============================================================================
 Name		: CFiruDictListBoxModel.h
 Author	  :
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CFiruDictListBoxModel declaration
 ============================================================================
 */

#ifndef CFIRUDICTLISTBOXMODEL_H
#define CFIRUDICTLISTBOXMODEL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "FiruData.h"
#include <badesca.h>

/**
 *  CFiruDictListBoxModel
 *
 */
class CFiruDictListBoxModel : public CBase
{
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CFiruDictListBoxModel( );

    /**
     * Two-phased constructor.
     */
    static CFiruDictListBoxModel* NewL( CFiruData& aData );

    /**
     * Two-phased constructor.
     */
    static CFiruDictListBoxModel* NewLC( CFiruData& aData );

public:
    void SetFilterL( const TDesC& aText );
    void GetMoreL();

    CDesCArray* GetTextListLC( );
    CFiruEntry* Entry( TInt pos );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CFiruDictListBoxModel( CFiruData& aData );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL( );

private:

    CFiruData& iData; // not owned

    RPointerArray<CFiruEntry> iEntries;
};

#endif // CFIRUDICTLISTBOXMODEL_H
