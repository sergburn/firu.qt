/*
 ========================================================================
 Name        : FiruTrainerListBox.cpp
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

// [[[ begin generated region: do not modify [Generated User Includes]
#include "FiruTrainerListBox.h"
#include "FiruTrainerListBoxView.h"
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
CFiruTrainerListBox::CFiruTrainerListBox()
{
    // [[[ begin generated region: do not modify [Generated Contents]
    iListBox = NULL;
    // ]]] end generated region [Generated Contents]

}
/**
 * Destroy child controls.
 */
CFiruTrainerListBox::~CFiruTrainerListBox()
{
    // [[[ begin generated region: do not modify [Generated Contents]
    delete iListBox;
    iListBox = NULL;
    // ]]] end generated region [Generated Contents]
    delete iLabel;
    iLabel = NULL;
}

/**
 * Construct the control (first phase).
 *  Creates an instance and initializes it.
 *  Instance is not left on cleanup stack.
 * @param aRect bounding rectangle
 * @param aParent owning parent, or NULL
 * @param aCommandObserver command observer
 * @return initialized instance of CFiruTrainerListBox
 */
CFiruTrainerListBox* CFiruTrainerListBox::NewL( const TRect& aRect, const CCoeControl* aParent,
        MEikCommandObserver* aCommandObserver )
{
    CFiruTrainerListBox* self = CFiruTrainerListBox::NewLC( aRect, aParent, aCommandObserver );
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
 * @return new instance of CFiruTrainerListBox
 */
CFiruTrainerListBox* CFiruTrainerListBox::NewLC( const TRect& aRect, const CCoeControl* aParent,
        MEikCommandObserver* aCommandObserver )
{
    CFiruTrainerListBox* self = new ( ELeave ) CFiruTrainerListBox();
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
void CFiruTrainerListBox::ConstructL( const TRect& aRect, const CCoeControl* aParent,
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
    InitializeControls2L();

    SetRect( aRect );
    ActivateL();
    // [[[ begin generated region: do not modify [Post-ActivateL initializations]
    // ]]] end generated region [Post-ActivateL initializations]
}

/**
 * Return the number of controls in the container (override)
 * @return count
 */
TInt CFiruTrainerListBox::CountComponentControls() const
{
    return (int) ELastControl;
}

/**
 * Get the control with the given index (override)
 * @param aIndex Control index [0...n) (limited by #CountComponentControls)
 * @return Pointer to control
 */
CCoeControl* CFiruTrainerListBox::ComponentControl( TInt aIndex ) const
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
        case ELabel:
            return iLabel;
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
void CFiruTrainerListBox::SizeChanged()
{
    CCoeControl::SizeChanged();

    // [[[ begin generated region: do not modify [Generated Contents]

    // ]]] end generated region [Generated Contents]
    LayoutControls2();
}

// [[[ begin generated function: do not modify
/**
 * Layout components as specified in the UI Designer
 */
void CFiruTrainerListBox::LayoutControls()
{
    iListBox->SetExtent( TPoint( 0, 0 ), iListBox->MinimumSize() );
}
// ]]] end generated function

void CFiruTrainerListBox::LayoutControls2()
{
    TSize area = Rect().Size();
    TInt listHeight = iListBox->CalcHeightBasedOnNumOfItems( 6 );
    iLabel->SetExtent( TPoint( 0, 0 ), TSize( area.iWidth, area.iHeight - listHeight ) );
    iListBox->SetExtent( TPoint( 0, area.iHeight - listHeight ), TSize( area.iWidth, listHeight ) );
}

/**
 *	Handle key events.
 */
TKeyResponse CFiruTrainerListBox::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
{
    // [[[ begin generated region: do not modify [Generated Contents]
    if ( aKeyEvent.iCode == EKeyLeftArrow || aKeyEvent.iCode == EKeyRightArrow )
    {
        // Listbox takes all events even if it doesn't use them
        return EKeyWasNotConsumed;
    }

    // ]]] end generated region [Generated Contents]

    if ( aKeyEvent.iCode == EKeyEnter )
    {
        iCommandObserver->ProcessCommandL( EFiruTrainerListBoxViewSelectMenuItemCommand );
        return EKeyWasConsumed;
    }

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
void CFiruTrainerListBox::InitializeControlsL()
{
    iListBox = new ( ELeave ) CAknSingleStyleListBox;
    iListBox->SetContainerWindowL( *this );
    {
        TResourceReader reader;
        iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_TRAINER_LIST_BOX_LIST_BOX );
        iListBox->ConstructFromResourceL( reader );
        CleanupStack::PopAndDestroy(); // reader internal state
    }
    // the listbox owns the items in the list and will free them
    iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );

    // setup the icon array so graphics-style boxes work
    SetupListBoxIconsL();

    // add list items

    iListBox->SetFocus( ETrue );
    iFocusControl = iListBox;

}
// ]]] end generated function

void CFiruTrainerListBox::InitializeControls2L()
{
    iLabel = new ( ELeave ) CEikLabel;
    iLabel->SetContainerWindowL( *this );
    {
        TResourceReader reader;
        iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_CONTAINER_LABEL1 );
        iLabel->ConstructFromResourceL( reader );
        CleanupStack::PopAndDestroy(); // reader internal state
    }
    iLabel->SetPixelGapBetweenLines( 1 );
}

/**
 * Handle global resource changes, such as scalable UI or skin events (override)
 */
void CFiruTrainerListBox::HandleResourceChange( TInt aType )
{
    CCoeControl::HandleResourceChange( aType );
    SetRect( iAvkonViewAppUi->View( TUid::Uid( EFiruTrainerListBoxViewId ) )->ClientRect() );
    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]

}

/**
 *	Draw container contents.
 */
void CFiruTrainerListBox::Draw( const TRect& aRect ) const
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
void CFiruTrainerListBox::AddListBoxItemL( CEikTextListBox* aListBox, const TDesC& aString )
{
    CTextListBoxModel* model = aListBox->Model();
    CDesCArray* itemArray = static_cast<CDesCArray*> ( model->ItemTextArray() );
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
RArray<TInt>* CFiruTrainerListBox::GetSelectedListBoxItemsLC( CEikTextListBox* aListBox )
{
    CAknFilteredTextListBoxModel* model =
            static_cast<CAknFilteredTextListBoxModel *> ( aListBox->Model() );
    if ( model->NumberOfItems() == 0 )
        return NULL;

    // get currently selected indices
    const CListBoxView::CSelectionIndexArray* selectionIndexes = aListBox->SelectionIndexes();
    TInt selectedIndexesCount = selectionIndexes->Count();
    if ( selectedIndexesCount == 0 )
        return NULL;

    // copy the indices and sort numerically
    RArray<TInt>* orderedSelectedIndices = new ( ELeave ) RArray<TInt> ( selectedIndexesCount );

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
            TInt filteredItem = ( *selectionIndexes )[idx];
            TInt actualItem = filter->FilteredItemIndex( filteredItem );
            orderedSelectedIndices->InsertInOrder( actualItem );
        }
    }
    else
    {
        // the selection indices refer directly to the model
        for ( TInt idx = 0; idx < selectedIndexesCount; idx++ )
            orderedSelectedIndices->InsertInOrder( ( *selectionIndexes )[idx] );
    }

    return orderedSelectedIndices;
}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 * Delete the selected item or items from the list box.
 */
void CFiruTrainerListBox::DeleteSelectedListBoxItemsL( CEikTextListBox* aListBox )
{
    CAknFilteredTextListBoxModel* model =
            static_cast<CAknFilteredTextListBoxModel *> ( aListBox->Model() );
    if ( model->NumberOfItems() == 0 )
        return;

    RArray<TInt>* orderedSelectedIndices = GetSelectedListBoxItemsLC( aListBox );
    if ( !orderedSelectedIndices )
        return;

    // Delete selected items from bottom up so indices don't change on us
    CDesCArray* itemArray = static_cast<CDesCArray*> ( model->ItemTextArray() );
    TInt currentItem = 0;

    for ( TInt idx = orderedSelectedIndices->Count(); idx-- > 0; )
    {
        currentItem = ( *orderedSelectedIndices )[idx];
        itemArray->Delete( currentItem );
    }

    // dispose the array resources
    CleanupStack::PopAndDestroy();

    // dispose the array pointer
    CleanupStack::PopAndDestroy( orderedSelectedIndices );

    // refresh listbox's cursor now that items are deleted
    AknListBoxUtils::HandleItemRemovalAndPositionHighlightL( aListBox, currentItem, ETrue );
}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Get the listbox.
 */
CAknSingleStyleListBox* CFiruTrainerListBox::ListBox()
{
    return iListBox;
}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Create a list box item with the given column values.
 */
void CFiruTrainerListBox::CreateListBoxItemL( TDes& aBuffer, const TDesC& aMainText )
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
void CFiruTrainerListBox::AddListBoxResourceArrayItemL( TInt aResourceId )
{
    CDesCArray* array = iCoeEnv->ReadDesCArrayResourceL( aResourceId );
    CleanupStack::PushL( array );
    // This is intended to be large enough, but if you get
    // a USER 11 panic, consider reducing string sizes.
    TBuf<512> listString;
    CreateListBoxItemL( listString, ( *array )[0] );
    AddListBoxItemL( iListBox, listString );
    CleanupStack::PopAndDestroy( array );
}

// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 *	Set up the list's icon array.
 */
void CFiruTrainerListBox::SetupListBoxIconsL()
{
    CArrayPtr<CGulIcon>* icons = NULL;

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
TBool CFiruTrainerListBox::HandleMarkableListCommandL( TInt aCommand )
{
    return EFalse;
}

// ]]] end generated function

// ----------------------------------------------------------

void CFiruTrainerListBox::ShowTestL( CFiruTest& test )
{
    TBuf<128> text;
    _LIT( KLabelFormat, "%S\n(%d/%d)" );

    text.Format( KLabelFormat, &(test.Question()), test.Entry().Rate(), test.Entry().Counter() );
    iLabel->SetTextL( text );

    const RPointerArray<HBufC>& variants = test.AllVariants();

    CDesC16ArrayFlat* items =
        new (ELeave) CDesC16ArrayFlat( variants.Count() );
    CleanupStack::PushL( items );

    for ( int i = 0; i < variants.Count(); ++i )
    {
        CreateListBoxItemL( text, variants[i]->Left( 32 ) );
        items->AppendL( text );
    }

    iListBox->Model()->SetItemTextArray( items );
    iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
    iListBox->SetCurrentItemIndex( 0 );
    DrawDeferred();

    CleanupStack::Pop( items );
}
