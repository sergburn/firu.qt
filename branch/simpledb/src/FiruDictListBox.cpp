/*
 ========================================================================
 Name        : FiruDictListBox.cpp
 Author      :
 Copyright   : Your copyright notice
 Description :
 ========================================================================
 */
// [[[ begin generated region: do not modify [Generated System Includes]
#include <barsread.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>
#include <aknviewappui.h>
#include <eikappui.h>
#include <firu.rsg>
// ]]] end generated region [Generated System Includes]
#include <eikedwin.h>

#include "FiruDictListBoxModel.h"
#include "FiruData.h"
// [[[ begin generated region: do not modify [Generated User Includes]
#include "FiruDictListBox.h"
#include "FiruDictListBoxView.h"
#include "firu.hrh"
#include "FiruListBox.hrh"
#include "FiruDictListBox.hrh"
#include "FiruTrainerListBox.hrh"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]
const TInt KMaxListEntries = 50;

/**
 * First phase of Symbian two-phase construction. Should not
 * contain any code that could leave.
 */
CFiruDictListBox::CFiruDictListBox( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
	iListBox = NULL;
    // ]]] end generated region [Generated Contents]
}

/**
 * Destroy child controls.
 */
CFiruDictListBox::~CFiruDictListBox( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
	delete iListBox;
	iListBox = NULL;
	iControlEventDispatch.Close();		
    // ]]] end generated region [Generated Contents]
    delete iSearchField;

    delete iModel;
}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CFiruDictListBox
 */
CFiruDictListBox* CFiruDictListBox::NewL(
    const TRect& aRect,
    const CCoeControl* aParent,
    MEikCommandObserver* aCommandObserver,
    CFiruData& aData )
{
    CFiruDictListBox* self = CFiruDictListBox::NewLC( aRect, aParent, aCommandObserver, aData );
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
 * @return new instance of CFiruDictListBox
 */
CFiruDictListBox* CFiruDictListBox::NewLC(
    const TRect& aRect,
    const CCoeControl* aParent,
    MEikCommandObserver* aCommandObserver,
    CFiruData& aData )
{
    CFiruDictListBox* self = new ( ELeave ) CFiruDictListBox;
    CleanupStack::PushL( self );
    self->iModel = CFiruDictListBoxModel::NewL( aData );
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
void CFiruDictListBox::ConstructL(
    const TRect& aRect,
    const CCoeControl* aParent,
    MEikCommandObserver* aCommandObserver )
{
    if ( aParent == NULL )
    {
        CreateWindowL( );
    }
    else
    {
        SetContainerWindowL( *aParent );
    }
    iFocusControl = NULL;
    iCommandObserver = aCommandObserver;
    InitializeControlsL( );
    InitializeControls2L( );

    SetRect( aRect );
    ActivateL( );
    // [[[ begin generated region: do not modify [Post-ActivateL initializations]
    // ]]] end generated region [Post-ActivateL initializations]

    RefreshListBoxL( );
}

/**
 * Return the number of controls in the container (override)
 * @return count
 */
TInt CFiruDictListBox::CountComponentControls( ) const
{
    return ( int ) ELastControl;
}

/**
 * Get the control with the given index (override)
 * @param aIndex Control index [0...n) (limited by #CountComponentControls)
 * @return Pointer to control
 */
CCoeControl* CFiruDictListBox::ComponentControl( TInt aIndex ) const
{
    // [[[ begin generated region: do not modify [Generated Contents]
	switch ( aIndex )
		{
	case EListBox:
		return iListBox;
		}
    // ]]] end generated region [Generated Contents]

    // handle any user controls here...
    switch ( aIndex )
    {
        case ESearchField:
            return iSearchField;
    }

    return NULL;
}

/**
 *	Handle resizing of the container. This implementation will lay out
 *  full-sized controls like list boxes for any screen size, and will layout
 *  labels, editors, etc. to the size they were given in the UI designer.
 *  This code will need to be modified to adjust arbitrary controls to
 *  any screen size.
 */
void CFiruDictListBox::SizeChanged( )
{
    CCoeControl::SizeChanged( );
    LayoutControls2( );
    // [[[ begin generated region: do not modify [Generated Contents]
			
    // ]]] end generated region [Generated Contents]

}

// [[[ begin generated function: do not modify
/**
 * Layout components as specified in the UI Designer
 */
void CFiruDictListBox::LayoutControls()
	{
	iListBox->SetExtent( TPoint( 0, 0 ), iListBox->MinimumSize() );
	}
// ]]] end generated function

void CFiruDictListBox::LayoutControls2( )
{
    TSize searchFieldSize( Rect().Size().iWidth, iSearchField->Size().iHeight);
    iSearchField->SetExtent( TPoint( 0, 0 ), searchFieldSize );
    iListBox->SetExtent( TPoint( 0, iSearchField->Size().iHeight + 1 ), iListBox->MinimumSize( ) );
}

/**
 *	Handle key events.
 */
TKeyResponse CFiruDictListBox::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
{
    if ( aKeyEvent.iCode == EKeyUpArrow )
    {
        return iListBox->OfferKeyEventL( aKeyEvent, aType );
    }
    else if ( aKeyEvent.iCode == EKeyDownArrow )
    {
        if ( iListBox->CurrentItemIndex() == iListBox->Model()->NumberOfItems() - 1 )
        {
            iModel->GetMoreL();
            RefreshListBoxL();
        }
        return iListBox->OfferKeyEventL( aKeyEvent, aType );
    }
    else
    {
        return iSearchField->OfferKeyEventL( aKeyEvent, aType );
    }

    // [[[ begin generated region: do not modify [Generated Contents]
	if ( aKeyEvent.iCode == EKeyLeftArrow 
		|| aKeyEvent.iCode == EKeyRightArrow )
		{
		// Listbox takes all events even if it doesn't use them
		return EKeyWasNotConsumed;
		}
	
    // ]]] end generated region [Generated Contents]

    if ( iFocusControl != NULL && iFocusControl->OfferKeyEventL( aKeyEvent, aType )
        == EKeyWasConsumed )
    {
        return EKeyWasConsumed;
    }
    return CCoeControl::OfferKeyEventL( aKeyEvent, aType );
}

// [[[ begin generated function: do not modify
/**
 *	Initialize each control upon creation.
 */				
void CFiruDictListBox::InitializeControlsL()
	{
	iListBox = new ( ELeave ) CAknSingleStyleListBox;
	iListBox->SetContainerWindowL( *this );
		{
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_DICT_LIST_BOX_LIST_BOX );
		iListBox->ConstructFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
	// the listbox owns the items in the list and will free them
	iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
	
	// setup the icon array so graphics-style boxes work
	SetupListBoxIconsL();
	
	
	// add list items
	iListBox->SetObserver( this );
	AddControlEventHandlerL( 
			iListBox, 
			EEventStateChanged, 
			&CFiruDictListBox::HandleListBoxStateChangedL );
	
	iListBox->SetFocus( ETrue );
	iFocusControl = iListBox;
	
	}
// ]]] end generated function


void CFiruDictListBox::InitializeControls2L( )
{
    iSearchField = new ( ELeave ) CEikEdwin;
    iSearchField->SetContainerWindowL( *this );
    {
        TResourceReader reader;
        iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_DICT_EDIT1 );
        iSearchField->ConstructFromResourceL( reader );
        CleanupStack::PopAndDestroy(); // reader internal state
    }
    _LIT( KEmpty, "" );
    iSearchField->SetTextL( &KEmpty );
    iSearchField->SetObserver( this );

    AddControlEventHandlerL(
        iSearchField,
        EEventStateChanged,
        &CFiruDictListBox::HandleSearchFieldStateChangedL );

    iSearchField->SetFocus( ETrue );
    iFocusControl = iSearchField;
}

/**
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CFiruDictListBox::HandleResourceChange( TInt aType )
{
    CCoeControl::HandleResourceChange( aType );
    SetRect( iAvkonViewAppUi->View( TUid::Uid( EFiruDictListBoxViewId ) )->ClientRect() );
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]

}

/**
 *	Draw container contents.
 */
void CFiruDictListBox::Draw( const TRect& aRect ) const
{
    // [[[ begin generated region: do not modify [Generated Contents]
	CWindowGc& gc = SystemGc();
	gc.Clear( aRect );
	
    // ]]] end generated region [Generated Contents]

}

// [[[ begin generated function: do not modify
/**
 *	Add a list box item to a list.
 */
void CFiruDictListBox::AddListBoxItemL( 
		CEikTextListBox* aListBox,
		const TDesC& aString )
	{
	CTextListBoxModel* model = aListBox->Model();
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	itemArray->AppendL( aString );
	aListBox->HandleItemAdditionL();
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 * Get the array of selected item indices, with respect to the list model.
 * The array is sorted in ascending order.
 * The array should be destroyed with two calls to CleanupStack::PopAndDestroy(),
 * the first with no argument (referring to the internal resource) and the
 * second with the array pointer.
 * @return newly allocated array, which is left on the cleanup stack;
 *	or NULL for empty list. 
 */
RArray< TInt >* CFiruDictListBox::GetSelectedListBoxItemsLC( CEikTextListBox* aListBox )
	{
	CAknFilteredTextListBoxModel* model = 
		static_cast< CAknFilteredTextListBoxModel *> ( aListBox->Model() );
	if ( model->NumberOfItems() == 0 )
		return NULL;
		
	// get currently selected indices
	const CListBoxView::CSelectionIndexArray* selectionIndexes =
		aListBox->SelectionIndexes();
	TInt selectedIndexesCount = selectionIndexes->Count();
	if ( selectedIndexesCount == 0 )
		return NULL;
		
	// copy the indices and sort numerically
	RArray<TInt>* orderedSelectedIndices = 
		new (ELeave) RArray< TInt >( selectedIndexesCount );
	
	// push the allocated array
	CleanupStack::PushL( orderedSelectedIndices );
	
	// dispose the array resource
	CleanupClosePushL( *orderedSelectedIndices );
	
	// see if the search field is enabled
	CAknListBoxFilterItems* filter = model->Filter();
	if ( filter != NULL )
		{
		// when filtering enabled, translate indices back to underlying model
		for ( TInt idx = 0; idx < selectedIndexesCount; idx++ )
			{
			TInt filteredItem = ( *selectionIndexes ) [ idx ];
			TInt actualItem = filter->FilteredItemIndex ( filteredItem );
			orderedSelectedIndices->InsertInOrder( actualItem );
			}
		}
	else
		{
		// the selection indices refer directly to the model
		for ( TInt idx = 0; idx < selectedIndexesCount; idx++ )
			orderedSelectedIndices->InsertInOrder( ( *selectionIndexes ) [ idx ] );
		}	
		
	return orderedSelectedIndices;
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 * Delete the selected item or items from the list box.
 */
void CFiruDictListBox::DeleteSelectedListBoxItemsL( CEikTextListBox* aListBox )
	{
	CAknFilteredTextListBoxModel* model = 
		static_cast< CAknFilteredTextListBoxModel *> ( aListBox->Model() );
	if ( model->NumberOfItems() == 0 )
		return;
	
	RArray< TInt >* orderedSelectedIndices = GetSelectedListBoxItemsLC( aListBox );		
	if ( !orderedSelectedIndices )
		return;
		
	// Delete selected items from bottom up so indices don't change on us
	CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
	TInt currentItem = 0;
	
	for ( TInt idx = orderedSelectedIndices->Count(); idx-- > 0; ) 
		{
		currentItem = ( *orderedSelectedIndices )[ idx ];
		itemArray->Delete ( currentItem );
		}
	
	// dispose the array resources
	CleanupStack::PopAndDestroy();
	
	// dispose the array pointer
	CleanupStack::PopAndDestroy( orderedSelectedIndices );
	
	// refresh listbox's cursor now that items are deleted
	AknListBoxUtils::HandleItemRemovalAndPositionHighlightL(
		aListBox, currentItem, ETrue );
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Get the listbox.
 */
CAknSingleStyleListBox* CFiruDictListBox::ListBox()
	{
	return iListBox;
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Create a list box item with the given column values.
 */
void CFiruDictListBox::CreateListBoxItemL( TDes& aBuffer, 
		const TDesC& aMainText )
	{
	_LIT ( KStringHeader, "\t%S" );
	
	aBuffer.Format( KStringHeader(), &aMainText );
	} 
				
// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Add an item to the list by reading the text items from the array resource
 *	and setting a single image property (if available) from an index
 *	in the list box's icon array.
 *	@param aResourceId id of an ARRAY resource containing the textual
 *	items in the columns
 *	
 */
void CFiruDictListBox::AddListBoxResourceArrayItemL( TInt aResourceId )
	{
	CDesCArray* array = iCoeEnv->ReadDesCArrayResourceL( aResourceId );
	CleanupStack::PushL( array );
	// This is intended to be large enough, but if you get 
	// a USER 11 panic, consider reducing string sizes.
	TBuf<512> listString; 
	CreateListBoxItemL( listString, ( *array ) [ 0 ] );
	AddListBoxItemL( iListBox, listString );
	CleanupStack::PopAndDestroy( array );
	} 
				
// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Set up the list's icon array.
 */
void CFiruDictListBox::SetupListBoxIconsL()
	{
	CArrayPtr< CGulIcon >* icons = NULL;		
	
	if ( icons != NULL )
		{
		iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
		}
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/** 
 *	Handle commands relating to markable lists.
 */
TBool CFiruDictListBox::HandleMarkableListCommandL( TInt aCommand )
	{
	return EFalse;
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 * Override of the HandleListBoxEventL virtual function
 */
void CFiruDictListBox::HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent anEventType )
{
    for ( int i = 0; i < iListBoxEventDispatch.Count( ); i++ )
    {
        const TListBoxEventDispatch& currEntry = iListBoxEventDispatch[i];
        if ( currEntry.src == aListBox && currEntry.event == anEventType )
        {
            ( this->*currEntry.handler )( aListBox, anEventType );
            break;
        }
    }
}

// ]]] end generated function

/**
 * Helper function to register MEikListBoxObserver event handlers
 */
void CFiruDictListBox::AddListBoxEventHandlerL(
    CEikListBox* aListBox,
    TListBoxEvent anEvent,
    ListBoxEventHandler aHandler )
{
    TListBoxEventDispatch entry;
    entry.src = aListBox;
    entry.event = anEvent;
    entry.handler = aHandler;
    TInt err = iListBoxEventDispatch.Append( entry );
    User::LeaveIfError( err );
}

/**
 *	Handle the itemClicked event
 */
void CFiruDictListBox::HandleListBoxItemClickedL( CEikListBox* aListBox, TListBoxEvent anEventType )
{
    iCommandObserver->ProcessCommandL( EFiruDictListBoxViewOpenMenuItemCommand );
}

/**
 *	Handle the enterKeyPressed event
 */
void CFiruDictListBox::HandleOpenItemL( CEikListBox* aListBox, TListBoxEvent anEventType )
{
    iCommandObserver->ProcessCommandL( EFiruDictListBoxViewOpenMenuItemCommand );
}

// ------------------------------------------------------
// ------------------------------------------------------
//
CFiruDataEntry* CFiruDictListBox::CurrentItem( )
{
    TInt pos = iListBox->CurrentItemIndex( );
    return iModel->Entry( pos );
}

// ------------------------------------------------------
//
CFiruDictListBoxModel& CFiruDictListBox::Model( )
{
    return *iModel;
}

// ------------------------------------------------------
// ------------------------------------------------------
//
void CFiruDictListBox::RefreshListBoxL( )
{
    CDesCArray* entries = iModel->GetTextListLC();

    CAknFilteredTextListBoxModel* model =
        static_cast< CAknFilteredTextListBoxModel *> ( iListBox->Model() );

    if ( model->NumberOfItems() != 0 )
    {
        CDesCArray* itemArray = static_cast< CDesCArray* > ( model->ItemTextArray() );
        itemArray->Reset();

        // dispose the array resources
//        CleanupStack::PopAndDestroy();

        // refresh listbox's cursor now that items are deleted
        AknListBoxUtils::HandleItemRemovalAndPositionHighlightL(
            iListBox, 0, ETrue );
    }

    TBuf<512> listString;
    for ( int i = 0; i < entries->Count( ); i++ )
    {
        CreateListBoxItemL( listString, (*entries)[i] );
        AddListBoxItemL( iListBox, listString );
    }

    CleanupStack::PopAndDestroy( entries );
}

/**
 * Override of the HandleControlEventL virtual function
 */
void CFiruDictListBox::HandleControlEventL( CCoeControl* aControl, TCoeEvent anEventType )
{
    for ( int i = 0; i < iControlEventDispatch.Count( ); i++ )
    {
        const TControlEventDispatch& currEntry = iControlEventDispatch[i];
        if ( currEntry.src == aControl && currEntry.event == anEventType )
        {
            ( this->*currEntry.handler )( aControl, anEventType );
            break;
        }
    }
}
/**
 * Helper function to register MCoeControlObserver event handlers
 */
void CFiruDictListBox::AddControlEventHandlerL(
    CCoeControl* aControl,
    TCoeEvent anEvent,
    ControlEventHandler aHandler )
{
    TControlEventDispatch entry;
    entry.src = aControl;
    entry.event = anEvent;
    entry.handler = aHandler;
    TInt err = iControlEventDispatch.Append( entry );
    User::LeaveIfError( err );
}

/**
 * Handle the EEventStateChanged event.
 */
void CFiruDictListBox::HandleListBoxStateChangedL( CCoeControl* /* aControl */, TCoeEvent /* anEvent */)
{
    // TODO: implement stateChanged event handler
//    CFiruDataEntry* entry = CurrentItem();
//    iSearchField->SetTextL( &( entry->Text() ) );
}

/**
 * Handle the EEventStateChanged event.
 */
void CFiruDictListBox::HandleSearchFieldStateChangedL( CCoeControl* aControl, TCoeEvent /* anEvent */)
{
    if ( aControl == iSearchField )
    {
        HBufC* pattern = iSearchField->GetTextInHBufL();
        if ( !pattern )
        {
            pattern = HBufC::NewL( 1 );
        }
        CleanupStack::PushL( pattern );
        iModel->SetFilterL( *pattern );
        CleanupStack::PopAndDestroy( pattern );

        RefreshListBoxL( );
    }
}
