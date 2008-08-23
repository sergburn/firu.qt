/*
========================================================================
 Name        : FiruContainer.cpp
 Author      : 
 Copyright   : Your copyright notice
 Description : 
========================================================================
*/
// [[[ begin generated region: do not modify [Generated System Includes]
#include <barsread.h>
#include <stringloader.h>
#include <eiklabel.h>
#include <eikenv.h>
#include <gdi.h>
#include <eikedwin.h>
#include <aknviewappui.h>
#include <eikappui.h>
#include <firu.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "FiruContainer.h"
#include "FiruContainerView.h"
#include "firu.hrh"
#include "FiruListBox.hrh"
#include "FiruDictListBox.hrh"
#include "FiruTrainerListBox.hrh"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not 
 * contain any code that could leave.
 */
CFiruContainer::CFiruContainer()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	iLabel1 = NULL;
	iEdit1 = NULL;
    // ]]] end generated region [Generated Contents]
    
    }
/** 
 * Destroy child controls.
 */
CFiruContainer::~CFiruContainer()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	delete iLabel1;
	iLabel1 = NULL;
	delete iEdit1;
	iEdit1 = NULL;
    // ]]] end generated region [Generated Contents]
    
    }
                
/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CFiruContainer
 */
CFiruContainer* CFiruContainer::NewL( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver )
    {
    CFiruContainer* self = CFiruContainer::NewLC( 
            aRect, 
            aParent, 
            aCommandObserver );
    CleanupStack::Pop( self );
    return self;
    }

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is left on cleanup stack.
 * @param aRect The rectangle for this window
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return new instance of CFiruContainer
 */
CFiruContainer* CFiruContainer::NewLC( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver )
    {
    CFiruContainer* self = new ( ELeave ) CFiruContainer();
    CleanupStack::PushL( self );
    self->ConstructL( aRect, aParent, aCommandObserver );
    return self;
    }
            
/**
 * Construct the control (second phase).
 *  Creates a window to contain the controls and activates it.
 * @param aRect bounding rectangle
 * @param aCommandObserver command observer
 * @param aParent owning parent, or NULL
 */ 
void CFiruContainer::ConstructL( 
        const TRect& aRect, 
        const CCoeControl* aParent, 
        MEikCommandObserver* aCommandObserver )
    {
    if ( aParent == NULL )
        {
        CreateWindowL();
        }
    else
        {
        SetContainerWindowL( *aParent );
        }
    iFocusControl = NULL;
    iCommandObserver = aCommandObserver;
    InitializeControlsL();
    SetRect( aRect );
    ActivateL();
    // [[[ begin generated region: do not modify [Post-ActivateL initializations]
    // ]]] end generated region [Post-ActivateL initializations]
    
    }
            
/**
* Return the number of controls in the container (override)
* @return count
*/
TInt CFiruContainer::CountComponentControls() const
    {
    return ( int ) ELastControl;
    }
                
/**
* Get the control with the given index (override)
* @param aIndex Control index [0...n) (limited by #CountComponentControls)
* @return Pointer to control
*/
CCoeControl* CFiruContainer::ComponentControl( TInt aIndex ) const
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	switch ( aIndex )
		{
		case ELabel1:
			return iLabel1;
		case EEdit1:
			return iEdit1;
		}
    // ]]] end generated region [Generated Contents]
    
    // handle any user controls here...
    
    return NULL;
    }
                
/**
 *    Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */                
void CFiruContainer::SizeChanged()
    {
    CCoeControl::SizeChanged();
    LayoutControls();
    // [[[ begin generated region: do not modify [Generated Contents]
			
    // ]]] end generated region [Generated Contents]
    
    }
                
// [[[ begin generated function: do not modify
/**
 * Layout components as specified in the UI Designer
 */
void CFiruContainer::LayoutControls()
	{
	iLabel1->SetExtent( TPoint( 24, 15 ), TSize( 173, 102 ) );
	iEdit1->SetExtent( TPoint( 106, 140 ), TSize( 46, 17 ) );
	}
// ]]] end generated function

/**
 *    Handle key events.
 */                
TKeyResponse CFiruContainer::OfferKeyEventL( 
        const TKeyEvent& aKeyEvent, 
        TEventCode aType )
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	
    // ]]] end generated region [Generated Contents]
    
    if ( iFocusControl != NULL
        && iFocusControl->OfferKeyEventL( aKeyEvent, aType ) == EKeyWasConsumed )
        {
        return EKeyWasConsumed;
        }
    return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
    }
                
// [[[ begin generated function: do not modify
/**
 *	Initialize each control upon creation.
 */				
void CFiruContainer::InitializeControlsL()
	{
	iLabel1 = new ( ELeave ) CEikLabel;
	iLabel1->SetContainerWindowL( *this );
		{
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_CONTAINER_LABEL1 );
		iLabel1->ConstructFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
	iLabel1->SetPixelGapBetweenLines( 1 );
	iEdit1 = new ( ELeave ) CEikEdwin;
	iEdit1->SetContainerWindowL( *this );
		{
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_CONTAINER_EDIT1 );
		iEdit1->ConstructFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
		{
		HBufC* text = StringLoader::LoadLC( R_FIRU_CONTAINER_EDIT1_2 );
		iEdit1->SetTextL( text );
		CleanupStack::PopAndDestroy( text );
		}
	
	iEdit1->SetFocus( ETrue );
	iFocusControl = iEdit1;
	
	}
// ]]] end generated function

/** 
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CFiruContainer::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    SetRect( iAvkonViewAppUi->View( TUid::Uid( EFiruContainerViewId ) )->ClientRect() );
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
    
    }
                
/**
 *    Draw container contents.
 */                
void CFiruContainer::Draw( const TRect& aRect ) const
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	
    // ]]] end generated region [Generated Contents]
    
    }
                
