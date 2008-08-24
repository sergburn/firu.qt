/*
========================================================================
 Name        : FiruContainerView.cpp
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
#include <eiklabel.h>
#include <eikenv.h>
#include <gdi.h>
#include <eikedwin.h>
#include <akncontext.h>
#include <akntitle.h>
#include <eikbtgpc.h>
#include <firu.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]

#include "firu.hrh"
#include "FiruContainerView.h"
#include "FiruListBox.hrh"
#include "FiruDictListBox.hrh"
#include "FiruTrainerListBox.hrh"
#include "FiruContainer.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CFiruContainerView::CFiruContainerView()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	iFiruContainer = NULL;
    // ]]] end generated region [Generated Contents]
    
    }

/** 
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CFiruContainerView::~CFiruContainerView()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	delete iFiruContainer;
	iFiruContainer = NULL;
    // ]]] end generated region [Generated Contents]
    
    }

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CFiruContainerView
 */
CFiruContainerView* CFiruContainerView::NewL()
    {
    CFiruContainerView* self = CFiruContainerView::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CFiruContainerView
 */
CFiruContainerView* CFiruContainerView::NewLC()
    {
    CFiruContainerView* self = new ( ELeave ) CFiruContainerView();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


/**
 * Second-phase constructor for view.  
 * Initialize contents from resource.
 */ 
void CFiruContainerView::ConstructL()
    {
    // [[[ begin generated region: do not modify [Generated Code]
	BaseConstructL( R_FIRU_CONTAINER_FIRU_CONTAINER_VIEW );
				
    // ]]] end generated region [Generated Code]
    
    // add your own initialization code here
    
    }

/**
 * @return The UID for this view
 */
TUid CFiruContainerView::Id() const
    {
    return TUid::Uid( EFiruContainerViewId );
    }

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CFiruContainerView::HandleCommandL( TInt aCommand )
    {
    // [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{	// code to dispatch to the AknView's menu and CBA commands is generated here
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
 *    Handles user actions during activation of the view, 
 *    such as initializing the content.
 */
void CFiruContainerView::DoActivateL( 
        const TVwsViewId& /*aPrevViewId*/,
        TUid /*aCustomMessageId*/,
        const TDesC8& /*aCustomMessage*/ )
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	SetupStatusPaneL();
	
				
				
	
	if ( iFiruContainer == NULL )
		{
		iFiruContainer = CreateContainerL();
		iFiruContainer->SetMopParent( this );
		AppUi()->AddToStackL( *this, iFiruContainer );
		} 
    // ]]] end generated region [Generated Contents]
    
    }

/**
 */
void CFiruContainerView::DoDeactivate()
    {
    // [[[ begin generated region: do not modify [Generated Contents]
	CleanupStatusPane();
	
	if ( iFiruContainer != NULL )
		{
		AppUi()->RemoveFromViewStack( *this, iFiruContainer );
		delete iFiruContainer;
		iFiruContainer = NULL;
		}
    // ]]] end generated region [Generated Contents]
    
    }

/** 
 * Handle status pane size change for this view (override)
 */
void CFiruContainerView::HandleStatusPaneSizeChange()
    {
    CAknView::HandleStatusPaneSizeChange();
    
    // this may fail, but we're not able to propagate exceptions here
    TVwsViewId view;
    AppUi()->GetActiveViewId( view );
    if ( view.iViewUid == Id() )
        {
        TInt result;
        TRAP( result, SetupStatusPaneL() );
        }
    
    // [[[ begin generated region: do not modify [Generated Code]
    // ]]] end generated region [Generated Code]
    
    }

// [[[ begin generated function: do not modify
void CFiruContainerView::SetupStatusPaneL()
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
		iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_CONTAINER_TITLE_RESOURCE );
		title->SetFromResourceL( reader );
		CleanupStack::PopAndDestroy(); // reader internal state
		}
				
	}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CFiruContainerView::CleanupStatusPane()
	{
	}

// ]]] end generated function

/**
 *    Creates the top-level container for the view.  You may modify this method's
 *    contents and the CFiruContainer::NewL() signature as needed to initialize the
 *    container, but the signature for this method is fixed.
 *    @return new initialized instance of CFiruContainer
 */
CFiruContainer* CFiruContainerView::CreateContainerL()
    {
    return CFiruContainer::NewL( ClientRect(), NULL, this );
    }

