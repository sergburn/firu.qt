/*
 ========================================================================
 Name        : FiruListBoxView.cpp
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

// [[[ begin generated region: do not modify [Generated User Includes]

#include "firu.hrh"
#include "FiruListBoxView.h"
#include "FiruListBox.hrh"
#include "FiruDictListBox.hrh"
#include "FiruTrainerListBox.hrh"
#include "FiruListBox.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CFiruListBoxView::CFiruListBoxView( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
    iFiruListBox = NULL;
    // ]]] end generated region [Generated Contents]

}

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CFiruListBoxView::~CFiruListBoxView( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
    delete iFiruListBox;
    iFiruListBox = NULL;
    // ]]] end generated region [Generated Contents]

}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CFiruListBoxView
 */
CFiruListBoxView* CFiruListBoxView::NewL( )
{
    CFiruListBoxView* self = CFiruListBoxView::NewLC( );
    CleanupStack::Pop( self );
    return self;
}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CFiruListBoxView
 */
CFiruListBoxView* CFiruListBoxView::NewLC( )
{
    CFiruListBoxView* self = new ( ELeave ) CFiruListBoxView();
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
}

/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */
void CFiruListBoxView::ConstructL( )
{
    // [[[ begin generated region: do not modify [Generated Code]
    BaseConstructL( R_FIRU_LIST_BOX_FIRU_LIST_BOX_VIEW );
                
    // ]]] end generated region [Generated Code]

    // add your own initialization code here

}

/**
 * @return The UID for this view
 */
TUid CFiruListBoxView::Id( ) const
{
    return TUid::Uid( EFiruListBoxViewId );
}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CFiruListBoxView::HandleCommandL( TInt aCommand )
{
    // [[[ begin generated region: do not modify [Generated Code]
    TBool commandHandled = EFalse;
    switch ( aCommand )
        {    // code to dispatch to the AknView's menu and CBA commands is generated here
        case EFiruListBoxViewOpenMenuItemCommand:
            commandHandled = HandleOpenMenuItemSelectedL( aCommand );
            break;
        default:
            break;
        }
    
        
    if ( !commandHandled ) 
        {
    
        if ( aCommand == EAknSoftkeyExit )
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
void CFiruListBoxView::DoActivateL(
    const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/,
    const TDesC8& /*aCustomMessage*/)
{
    // [[[ begin generated region: do not modify [Generated Contents]
    SetupStatusPaneL();
    
                
                
    
    if ( iFiruListBox == NULL )
        {
        iFiruListBox = CreateContainerL();
        iFiruListBox->SetMopParent( this );
        AppUi()->AddToStackL( *this, iFiruListBox );
        } 
    // ]]] end generated region [Generated Contents]

}

/**
 */
void CFiruListBoxView::DoDeactivate( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
    CleanupStatusPane();
    
    if ( iFiruListBox != NULL )
        {
        AppUi()->RemoveFromViewStack( *this, iFiruListBox );
        delete iFiruListBox;
        iFiruListBox = NULL;
        }
    // ]]] end generated region [Generated Contents]

}

/** 
 * Handle status pane size change for this view (override)
 */
void CFiruListBoxView::HandleStatusPaneSizeChange( )
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
void CFiruListBoxView::SetupStatusPaneL()
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
        iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_LIST_BOX_TITLE_RESOURCE );
        title->SetFromResourceL( reader );
        CleanupStack::PopAndDestroy(); // reader internal state
        }
                
    }

// ]]] end generated function

// [[[ begin generated function: do not modify
void CFiruListBoxView::CleanupStatusPane()
    {
    }

// ]]] end generated function

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CFiruListBox::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CFiruListBox
 */
CFiruListBox* CFiruListBoxView::CreateContainerL( )
{
    return CFiruListBox::NewL( ClientRect( ), NULL, this );
}

/** 
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CFiruListBoxView::HandleOpenMenuItemSelectedL( TInt /*aCommand*/ )
{
    switch ( iFiruListBox->ListBox()->CurrentItemIndex( ) )
    {
        case 0:
            AppUi()->ActivateLocalViewL( TUid::Uid( EFiruDictListBoxViewId ) );
            return ETrue;
        case 1:
            AppUi()->ActivateLocalViewL( TUid::Uid( EFiruTrainerListBoxViewId ) );
            return ETrue;
        default:
            return false;
    }
}

