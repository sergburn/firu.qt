/*
 ========================================================================
 Name        : FiruTrainerListBoxView.cpp
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
#include <aknquerydialog.h>
#include <firu.rsg>
// ]]] end generated region [Generated System Includes]

#include "FiruDocument.h"
#include "FiruData.h"
#include "FiruAppUi.h"

// [[[ begin generated region: do not modify [Generated User Includes]

#include "firu.hrh"
#include "FiruTrainerListBoxView.h"
#include "FiruListBox.hrh"
#include "FiruDictListBox.hrh"
#include "FiruTrainerListBox.hrh"
#include "FiruTrainerListBox.h"
// ]]] end generated region [Generated User Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

const TInt KNumTotalTests = 20;
const TInt KNumNewTests = 7;
const TInt KNumVariants = 6;

/**
 * First phase of Symbian two-phase construction. Should not contain any
 * code that could leave.
 */
CFiruTrainerListBoxView::CFiruTrainerListBoxView()
{
    // [[[ begin generated region: do not modify [Generated Contents]
    iFiruTrainerListBox = NULL;
    // ]]] end generated region [Generated Contents]

}

/**
 * The view's destructor removes the container from the control
 * stack and destroys it.
 */
CFiruTrainerListBoxView::~CFiruTrainerListBoxView()
{
    // [[[ begin generated region: do not modify [Generated Contents]
    delete iFiruTrainerListBox;
    iFiruTrainerListBox = NULL;
    // ]]] end generated region [Generated Contents]

}

/**
 * Symbian two-phase constructor.
 * This creates an instance then calls the second-phase constructor
 * without leaving the instance on the cleanup stack.
 * @return new instance of CFiruTrainerListBoxView
 */
CFiruTrainerListBoxView* CFiruTrainerListBoxView::NewL()
{
    CFiruTrainerListBoxView* self = CFiruTrainerListBoxView::NewLC();
    CleanupStack::Pop( self );
    return self;
}

/**
 * Symbian two-phase constructor.
 * This creates an instance, pushes it on the cleanup stack,
 * then calls the second-phase constructor.
 * @return new instance of CFiruTrainerListBoxView
 */
CFiruTrainerListBoxView* CFiruTrainerListBoxView::NewLC()
{
    CFiruTrainerListBoxView* self = new ( ELeave ) CFiruTrainerListBoxView();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
}

/**
 * Second-phase constructor for view.
 * Initialize contents from resource.
 */
void CFiruTrainerListBoxView::ConstructL()
{
    // [[[ begin generated region: do not modify [Generated Code]
    BaseConstructL( R_FIRU_TRAINER_LIST_BOX_FIRU_TRAINER_LIST_BOX_VIEW );

    // ]]] end generated region [Generated Code]

    // add your own initialization code here

}

/**
 * @return The UID for this view
 */
TUid CFiruTrainerListBoxView::Id() const
{
    return TUid::Uid( EFiruTrainerListBoxViewId );
}

/**
 * Handle a command for this view (override)
 * @param aCommand command id to be handled
 */
void CFiruTrainerListBoxView::HandleCommandL( TInt aCommand )
{
    // [[[ begin generated region: do not modify [Generated Code]
    TBool commandHandled = EFalse;
    switch ( aCommand )
    { // code to dispatch to the AknView's menu and CBA commands is generated here

        case EAknSoftkeyNext:
            commandHandled = HandleControlPaneRightSoftKeyPressedL( aCommand );
            break;
        case EFiruTrainerListBoxViewSelectMenuItemCommand:
            commandHandled = HandleSelectMenuItemSelectedL( aCommand );
            break;
        case EFiruTrainerListBoxViewShow_detailsMenuItemCommand:
            commandHandled = HandleShow_detailsMenuItemSelectedL( aCommand );
            break;
        case EFiruTrainerListBoxViewReset_statsMenuItemCommand:
            commandHandled = HandleReset_statsMenuItemSelectedL( aCommand );
            break;
        case EFiruTrainerListBoxViewCancelMenuItemCommand:
            commandHandled = HandleCancelMenuItemSelectedL( aCommand );
            break;
        default:
            break;
    }

    if ( !commandHandled )
    {

    }
    // ]]] end generated region [Generated Code]

    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]
}

/**
 *	Handles user actions during activation of the view,
 *	such as initializing the content.
 */
void CFiruTrainerListBoxView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
    TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/)
{
    // [[[ begin generated region: do not modify [Generated Contents]
    SetupStatusPaneL();

    if ( iFiruTrainerListBox == NULL )
    {
        iFiruTrainerListBox = CreateContainerL();
        iFiruTrainerListBox->SetMopParent( this );
        AppUi()->AddToStackL( *this, iFiruTrainerListBox );
    }
    // ]]] end generated region [Generated Contents]

    // [[[ begin generated region: do not modify [Generated Contents]
    // ]]] end generated region [Generated Contents]

    NextTestL();
}

// ----------------------------------------------------------

void CFiruTrainerListBoxView::NextTestL()
{
    if ( !iExercise )
    {
        iExercise = Data().CreateExerciseL( KNumTotalTests, KNumNewTests, KNumVariants );
    }
    iCurrentTest = iExercise->NextTest();
    if ( !iCurrentTest )
    {
        delete iExercise;
        iExercise = NULL;
        iCurrentTest = NULL;

        iExercise = Data().CreateExerciseL( KNumTotalTests, KNumNewTests, KNumVariants );

        iCurrentTest = iExercise->NextTest();
        if ( !iCurrentTest )
        {
            _LIT( KNoExercises, "Can't find data to start exercises" );
            CFiruAppUi::RunErrorNoteL( &KNoExercises() );
            HandleCancelMenuItemSelectedL( KErrCancel );
            return;
        }
    }

    ASSERT( iCurrentTest );
    iFiruTrainerListBox->ShowTestL( *iCurrentTest );
}

/**
 */
void CFiruTrainerListBoxView::DoDeactivate()
{
    // [[[ begin generated region: do not modify [Generated Contents]
    CleanupStatusPane();

    if ( iFiruTrainerListBox != NULL )
    {
        AppUi()->RemoveFromViewStack( *this, iFiruTrainerListBox );
        delete iFiruTrainerListBox;
        iFiruTrainerListBox = NULL;
    }
    // ]]] end generated region [Generated Contents]

    delete iExercise;
    iExercise = NULL;
}

/**
 * Handle status pane size change for this view (override)
 */
void CFiruTrainerListBoxView::HandleStatusPaneSizeChange()
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
void CFiruTrainerListBoxView::SetupStatusPaneL()
{
    // reset the context pane
    TUid contextPaneUid = TUid::Uid( EEikStatusPaneUidContext );
    CEikStatusPaneBase::TPaneCapabilities subPaneContext = StatusPane()->PaneCapabilities(
        contextPaneUid );
    if ( subPaneContext.IsPresent() && subPaneContext.IsAppOwned() )
    {
        CAknContextPane* context = static_cast<CAknContextPane*> ( StatusPane()->ControlL(
            contextPaneUid ) );
        context->SetPictureToDefaultL();
    }

    // setup the title pane
    TUid titlePaneUid = TUid::Uid( EEikStatusPaneUidTitle );
    CEikStatusPaneBase::TPaneCapabilities subPaneTitle = StatusPane()->PaneCapabilities(
        titlePaneUid );
    if ( subPaneTitle.IsPresent() && subPaneTitle.IsAppOwned() )
    {
        CAknTitlePane* title =
            static_cast<CAknTitlePane*> ( StatusPane()->ControlL( titlePaneUid ) );
        TResourceReader reader;
        iEikonEnv->CreateResourceReaderLC( reader, R_FIRU_TRAINER_LIST_BOX_TITLE_RESOURCE );
        title->SetFromResourceL( reader );
        CleanupStack::PopAndDestroy(); // reader internal state
    }

}

// ]]] end generated function

// [[[ begin generated function: do not modify
void CFiruTrainerListBoxView::CleanupStatusPane()
{
}

// ]]] end generated function

/**
 *	Creates the top-level container for the view.  You may modify this method's
 *	contents and the CFiruTrainerListBox::NewL() signature as needed to initialize the
 *	container, but the signature for this method is fixed.
 *	@return new initialized instance of CFiruTrainerListBox
 */
CFiruTrainerListBox* CFiruTrainerListBoxView::CreateContainerL()
{
    return CFiruTrainerListBox::NewL( ClientRect(), NULL, this );
}

/**
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CFiruTrainerListBoxView::HandleReset_statsMenuItemSelectedL( TInt aCommand )
{
    // TODO: implement selected event handler
    CFiruDocument* doc = (CFiruDocument*) ( AppUi()->Document() );
    doc->Data().ResetStatsL();
    return ETrue;
}

// [[[ begin generated function: do not modify
/**
 * Show the popup dialog for qrySure
 * @param aOverrideText optional override text
 * @return EAknSoftkeyYes (left soft key id) or 0
 */
TInt CFiruTrainerListBoxView::RunQrySureL( const TDesC* aOverrideText )
{

    CAknQueryDialog* queryDialog = CAknQueryDialog::NewL();

    if ( aOverrideText != NULL )
    {
        queryDialog->SetPromptL( *aOverrideText );
    }
    return queryDialog->ExecuteLD( R_FIRU_TRAINER_LIST_BOX_QRY_SURE );
}
// ]]] end generated function

/**
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CFiruTrainerListBoxView::HandleCancelMenuItemSelectedL( TInt aCommand )
{
    AppUi()->ActivateLocalViewL( TUid::Uid( EFiruListBoxViewId ) );
    return ETrue;
}

/**
 * Handle the rightSoftKeyPressed event.
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CFiruTrainerListBoxView::HandleControlPaneRightSoftKeyPressedL( TInt aCommand )
{
    HandleSelectMenuItemSelectedL( aCommand );
    return EFalse;
}

/**
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CFiruTrainerListBoxView::HandleSelectMenuItemSelectedL( TInt aCommand )
{
    TInt selection = iFiruTrainerListBox->ListBox()->CurrentItemIndex();

    if ( iCurrentTest )
    {
        if ( iCurrentTest->TryVariant( selection ) )
        {
            Data().SaveTestResultL( *iCurrentTest );
            NextTestL();
        }
        else
        {
            _LIT( KWrong, "Wrong!" );
            CFiruAppUi::RunErrorNoteL( &KWrong() );
        }
    }

    return ETrue;
}

/**
 * Handle the selected event.
 * @param aCommand the command id invoked
 * @return ETrue if the command was handled, EFalse if not
 */
TBool CFiruTrainerListBoxView::HandleShow_detailsMenuItemSelectedL( TInt aCommand )
{
    // TODO: implement selected event handler
    return ETrue;
}

// ----------------------------------------------------------

CFiruData& CFiruTrainerListBoxView::Data()
{
    CFiruDocument* doc = (CFiruDocument*) AppUi()->Document();
    return doc->Data();
}

