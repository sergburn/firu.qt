/*
 ========================================================================
 Name        : FiruAppUi.cpp
 Author      :
 Copyright   : Your copyright notice
 Description :
 ========================================================================
 */
// [[[ begin generated region: do not modify [Generated System Includes]
#include <eikmenub.h>
#include <akncontext.h>
#include <akntitle.h>
#include <aknnavide.h>
#include <aknnavi.h>
#include <akntabgrp.h>
#include <aknnotewrappers.h>
#include <barsread.h>
#include <stringloader.h>
#include <firu.rsg>
// ]]] end generated region [Generated System Includes]

// [[[ begin generated region: do not modify [Generated User Includes]
#include "FiruAppUi.h"
#include "Firu.hrh"
#include "FiruListBox.hrh"
#include "FiruDictListBox.hrh"
#include "FiruTrainerListBox.hrh"
#include "FiruApplication.h"
#include "FiruListBoxView.h"
#include "FiruDictListBoxView.h"
#include "FiruTrainerListBoxView.h"
#include "FiruContainerView.h"
#include "FiruTesterView.h"
#include "FiruEntryViewView.h"
// ]]] end generated region [Generated User Includes]

#include "FiruDocument.h"

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

/**
 * Construct the CFiruAppUi instance
 */
CFiruAppUi::CFiruAppUi( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
	iNaviDecorator_ = NULL;
    // ]]] end generated region [Generated Contents]

}

/**
 * The appui's destructor removes the container from the control
 * stack and destroys it.
 */
CFiruAppUi::~CFiruAppUi( )
{
    // [[[ begin generated region: do not modify [Generated Contents]
	if ( iNaviDecorator_ != NULL )
		{
		delete iNaviDecorator_;
		iNaviDecorator_ = NULL;
		}
    // ]]] end generated region [Generated Contents]

}

// [[[ begin generated function: do not modify
void CFiruAppUi::InitializeContainersL()
	{
	CAknNavigationControlContainer* naviPane = ( CAknNavigationControlContainer* )
		StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) );
	iNaviDecorator_ = naviPane->ResourceDecorator();
	if ( iNaviDecorator_ != NULL )
		{
		iNaviTabs1 = ( CAknTabGroup* ) iNaviDecorator_->DecoratedControl();

		}

	iFiruListBoxView = CFiruListBoxView::NewL();
	AddViewL( iFiruListBoxView );
	iFiruDictListBoxView = CFiruDictListBoxView::NewL();
	AddViewL( iFiruDictListBoxView );
	SetDefaultViewL( *iFiruDictListBoxView );
	SetActiveTabByViewId( iFiruDictListBoxView->Id() );
	iFiruTrainerListBoxView = CFiruTrainerListBoxView::NewL();
	AddViewL( iFiruTrainerListBoxView );
	iFiruContainerView = CFiruContainerView::NewL();
	AddViewL( iFiruContainerView );
	iFiruTesterView = CFiruTesterView::NewL();
	AddViewL( iFiruTesterView );
	iFiruEntryViewView = CFiruEntryViewView::NewL();
	AddViewL( iFiruEntryViewView );
	}
// ]]] end generated function

/**
 * Handle a command for this appui (override)
 * @param aCommand command id to be handled
 */
void CFiruAppUi::HandleCommandL( TInt aCommand )
{
    // [[[ begin generated region: do not modify [Generated Code]
	TBool commandHandled = EFalse;
	switch ( aCommand )
		{ // code to dispatch to the AppUi's menu and CBA commands is generated here
		default:
			break;
		}


	if ( !commandHandled )
		{
		if ( aCommand == EAknSoftkeyExit || aCommand == EEikCmdExit )
			{
			Exit();
			}
		}
    // ]]] end generated region [Generated Code]

}

/**
 * Override of the HandleResourceChangeL virtual function
 */
void CFiruAppUi::HandleResourceChangeL( TInt aType )
{
    CAknViewAppUi::HandleResourceChangeL( aType );
    // [[[ begin generated region: do not modify [Generated Code]
    // ]]] end generated region [Generated Code]

}

/**
 * Override of the HandleKeyEventL virtual function
 * @return EKeyWasConsumed if event was handled, EKeyWasNotConsumed if not
 * @param aKeyEvent
 * @param aType
 */
TKeyResponse CFiruAppUi::HandleKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode /*aType*/)
{
    // The inherited HandleKeyEventL is private and cannot be called
    // [[[ begin generated region: do not modify [Generated Contents]
	TVwsViewId activeViewId;
	if ( GetActiveViewId( activeViewId ) == KErrNone
		&& iNaviTabs1->TabIndexFromId( activeViewId.iViewUid.iUid )
			== KErrNotFound )
		{
		return EKeyWasNotConsumed;
		}

	TInt active = iNaviTabs1->ActiveTabIndex();
	TInt count = iNaviTabs1->TabCount();

	switch ( aKeyEvent.iCode )
		{
		case EKeyLeftArrow:
			if ( active > 0 )
				{
				active--;
				ActivateLocalViewL( TUid::Uid( iNaviTabs1->TabIdFromIndex( active ) ) );
				return EKeyWasConsumed;
				}
			break;
		case EKeyRightArrow:
			if ( active + 1 < count )
				{
				active++;
				ActivateLocalViewL( TUid::Uid( iNaviTabs1->TabIdFromIndex( active ) ) );
				return EKeyWasConsumed;
				}
			break;
		default:
			return EKeyWasNotConsumed;
		}


    // ]]] end generated region [Generated Contents]

    return EKeyWasNotConsumed;
}

/**
 * Override of the HandleViewDeactivation virtual function
 *
 * @param aViewIdToBeDeactivated
 * @param aNewlyActivatedViewId
 */
void CFiruAppUi::HandleViewDeactivation(
    const TVwsViewId& aViewIdToBeDeactivated,
    const TVwsViewId& aNewlyActivatedViewId )
{
    CAknViewAppUi::HandleViewDeactivation( aViewIdToBeDeactivated,
        aNewlyActivatedViewId );
    // [[[ begin generated region: do not modify [Generated Contents]
	if ( aNewlyActivatedViewId.iAppUid == KUidFiruApplication )
		{
		SetActiveTabByViewId( aNewlyActivatedViewId.iViewUid );
		}
    // ]]] end generated region [Generated Contents]

}

/**
 * @brief Completes the second phase of Symbian object construction.
 * Put initialization code that could leave here.
 */
void CFiruAppUi::ConstructL( )
{
    // [[[ begin generated region: do not modify [Generated Contents]

	BaseConstructL( EAknEnableSkin  |
					 EAknEnableMSK );
	InitializeContainersL();
    // ]]] end generated region [Generated Contents]

}

// ----------------------------------------------------------
// ----------------------------------------------------------
//
RFs& CFiruAppUi::FsSession( )
{
    return iCoeEnv->FsSession( );
}

// ----------------------------------------------------------
// ----------------------------------------------------------
//
CFiruData& CFiruAppUi::Data( )
{
    CFiruDocument* doc = (CFiruDocument*) Document( );
    return doc->Data( );
}
// [[[ begin generated function: do not modify
/**
 * Show the popup note for errorNote
 * @param aOverrideText optional override text
 */
void CFiruAppUi::RunErrorNoteL( const TDesC* aOverrideText )
	{
	CAknErrorNote* note = new ( ELeave ) CAknErrorNote();
	if ( aOverrideText == NULL )
		{
		HBufC* noteText = StringLoader::LoadLC( R_APPLICATION_ERROR_NOTE );
		note->ExecuteLD( *noteText );
		CleanupStack::PopAndDestroy( noteText );
		}
	else
		{
		note->ExecuteLD( *aOverrideText );
		}
	}
// ]]] end generated function

// [[[ begin generated function: do not modify
/**
 * Show the popup note for infoNote
 * @param aOverrideText optional override text
 */
void CFiruAppUi::RunInfoNoteL( const TDesC* aOverrideText )
	{
	CAknInformationNote* note = new ( ELeave ) CAknInformationNote( ETrue );
	if ( aOverrideText == NULL )
		{
		HBufC* noteText = StringLoader::LoadLC( R_APPLICATION_INFO_NOTE );
		note->ExecuteLD( *noteText );
		CleanupStack::PopAndDestroy( noteText );
		}
	else
		{
		note->ExecuteLD( *aOverrideText );
		}
	}
// ]]] end generated function

// [[[ begin generated function: do not modify
void CFiruAppUi::SetActiveTabByViewId( TUid aViewId )
	{
	if ( iNaviTabs1 != NULL
		&& iNaviTabs1->TabIndexFromId( aViewId.iUid ) != KErrNotFound )
		{
		iNaviTabs1->SetActiveTabById( aViewId.iUid );
		}
	}
// ]]] end generated function

