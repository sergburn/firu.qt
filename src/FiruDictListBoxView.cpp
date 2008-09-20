/*
 ========================================================================
 Name        : FiruDictListBoxView.cpp
 Author      :
 Copyright   : Your copyright notice
 Description :
 ========================================================================
 */
// [[[ begin generated region: do not modify [Generated System Includes]
#include <aknviewappui.h>
#include <eikmenub.h>
#include <avkon.hrh>
#include <barsread.h>
#include <stringloader.h>
#include <aknlists.h>
#include <eikenv.h>
#include <akniconarray.h>
#include <eikclbd.h>
#include <akncontext.h>
#include <akntitle.h>
#include <eikbtgpc.h>
#include <firu.rsg>
// ]]] end generated region [Generated System Includes]

#include "FiruData.h"
#include "FiruDocument.h"
#include "FiruAppUi.h"

// [[[ begin generated region: do not modify [Generated User Includes]

#include "firu.hrh"
#include "FiruDictListBoxView.h"
#include "FiruListBox.hrh"
#include "FiruDictListBox.hrh"
#include "FiruTrainerListBox.hrh"
#include "FiruDictListBox.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CFiruDictListBoxView::CFiruDictListBoxView( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
	iFiruDictListBox = NULL;
    // ]]] end generated region [Generated Contents]

}

/**
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CFiruDictListBoxView::~CFiruDictListBoxView( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
	delete iFiruDictListBox;
	iFiruDictListBox = NULL;
    // ]]] end generated region [Generated Contents]

}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CFiruDictListBoxView
 */
CFiruDictListBoxView* CFiruDictListBoxView::NewL( )
{
    CFiruDictListBoxView* self = CFiruDictListBoxView::NewLC( );
    CleanupStack::Pop( self );
    return self;
}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CFiruDictListBoxView
 */
CFiruDictListBoxView* CFiruDictListBoxView::NewLC( )
{
    CFiruDictListBoxView* self = new ( ELeave ) CFiruDictListBoxView();
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
}

/**
 * Second-phase constructor for view.
 * Initialize contents from resource.
 */
void CFiruDictListBoxView::ConstructL( )
{
    // [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_FIRU_DICT_LIST_BOX_FIRU_DICT_LIST_BOX_VIEW );

    // ]]] end generated region [Generated Code]

    // add your own initialization code here

}

/**
 * @return The UID for this view
 */
TUid CFiruDictListBoxView::Id( ) const
{
    return TUid::Uid( EFiruDictListBoxViewId );
}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CFiruDictListBoxView::HandleCommandL( TInt aCommand )
{
    // [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here

		case EAknSoftkeyBack:
			commandHandled = HandleControlPaneRightSoftKeyPressedL( aCommand );
			break;
		case EFiruDictListBoxViewOpenMenuItemCommand:
			commandHandled = HandleOpenEntryL( aCommand );
			break;
		case EFiruDictListBoxViewReverseMenuItemCommand:
			commandHandled = HandleReverseMenuItemSelectedL( aCommand );
			break;
		default:
			break;
		}


	if ( !commandHandled )
		{

		if ( aCommand == EAknSoftkeyBack )
			{
			AppUi()->HandleCommandL( EEikCmdExit );
			}

		}
    // ]]] end generated region [Generated Code]

}

/**
 *	Handles user actions during activation of the view,
 *	such as initializing the content.
 */
void CFiruDictListBoxView::DoActivateL(
    const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/,
    const TDesC8& /*aCustomMessage*/)
{
    // [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();




	if ( iFiruDictListBox == NULL )
		{
		iFiruDictListBox = CreateContainerL();
		iFiruDictListBox->SetMopParent( this );
		AppUi()->AddToStackL( *this, iFiruDictListBox );
		}
    // ]]] end generated region [Generated Contents]

}

/**
 */
void CFiruDictListBoxView::DoDeactivate( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();

	if ( iFiruDictListBox != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iFiruDictListBox );
		delete iFiruDictListBox;
		iFiruDictListBox = NULL;
		}
    // ]]] end generated region [Generated Contents]

}

/**
 * Handle status pane size change for this view (override)
 */
void CFiruDictListBoxView::HandleStatusPaneSizeChange( )
{
    CAknView::HandleStatusPaneSizeChange( );

    // this may fail, but we're not able to propagate exceptions here
    TVwsViewId view;
    AppUi()->GetActiveViewId( view );
    if ( view.iViewUid == Id( ) )
    {
        TInt result;
        TRAP( result, SetupStatusPaneL() );
    }

    // [[[ begin generated region: do not modify [Generated Code]
    // ]]] end generated region [Generated Code]

}

// [[[ begin generated function: do not modify
void CFiruDictListBoxView::SetupStatusPaneL()
	{
	// reset the context pane
	TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
	CEikStatusPaneBase::TPaneCapabilities subPaneContext =
		StatusPane()->PaneCapabilities( contextPaneUid );
	if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
		{
		CAknContextPane* context = static_cast< CAknContextPane* > (
			StatusPane()->ControlL( contextPaneUid ) );
		context->SetPictureToDefaultL();
		}

	// setup the title pane
	TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
	CEikStatusPaneBase::TPaneCapabilities subPaneTitle =
		StatusPane()->PaneCapabilities( titlePaneUid );
	if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
		{
		CAknTitlePane* title = static_cast< CAknTitlePane* >(
			StatusPane()->ControlL( titlePaneUid ) );
		TResourceReader reader;
		iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_DICT_LIST_BOX_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}

	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CFiruDictListBoxView::CleanupStatusPane()
	{
	}

// ]]] end generated function

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CFiruDictListBox::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CFiruDictListBox
 */
CFiruDictListBox* CFiruDictListBoxView::CreateContainerL( )
{
    return CFiruDictListBox::NewL( ClientRect( ), NULL, this, Data( ) );
}

/**
 * Handle the rightSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CFiruDictListBoxView::HandleControlPaneRightSoftKeyPressedL( TInt /*aCommand*/ )
{
    AppUi()->ActivateLocalViewL( TUid::Uid( EFiruListBoxViewId ) );
    return ETrue;
}

/**
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CFiruDictListBoxView::HandleReverseMenuItemSelectedL( TInt /*aCommand*/ )
{
    Data().ReverseDictionaryL( );
    iFiruDictListBox->RefreshListBoxL( );
    return ETrue;
}

/**
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CFiruDictListBoxView::HandleOpenEntryL( TInt /*aCommand*/ )
{
    CFiruEntry* entry = iFiruDictListBox->CurrentItem( );
    TPtrC src( entry->Text( ) );

    RBuf text;
    text.CreateL( src.Length() );
    text.Copy( src );
    CleanupClosePushL( text );

    Data().GetTranslationsL( *entry );
    for ( int i = 0; i < entry->NumTranslations(); ++i )
    {
        TPtrC trans = entry->TranslationText( i );
        text.ReAllocL( text.MaxLength() + trans.Length() + 5 );
        text.AppendFormat( _L("\n%d) %S"), i+1, &trans );
    }

    CFiruAppUi::RunInfoNoteL( &text );

    CleanupStack::PopAndDestroy( &text );
    return ETrue;
}

// ----------------------------------------------------------

CFiruData& CFiruDictListBoxView::Data( )
{
    CFiruDocument* doc = (CFiruDocument*) AppUi()->Document( );
    return doc->Data( );
}

