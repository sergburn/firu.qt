/*
========================================================================
 Name        : FiruTrainerListBoxView.h
 Author      :
 Copyright   : Your copyright notice
 Description :
========================================================================
*/
#ifndef FIRUTRAINERLISTBOXVIEW_H
#define FIRUTRAINERLISTBOXVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]

#include "FiruData.h"

// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CFiruTrainerListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for FiruTrainerListBoxView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CFiruTrainerListBoxView FiruTrainerListBoxView.h
 */

class CFiruTrainerListBoxView : public CAknView
	{


	// [[[ begin [Public Section]
public:
	// constructors and destructor
	CFiruTrainerListBoxView();
	static CFiruTrainerListBoxView* NewL();
	static CFiruTrainerListBoxView* NewLC();
	void ConstructL();
	virtual ~CFiruTrainerListBoxView();

	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );

	// [[[ begin generated region: do not modify [Generated Methods]
	CFiruTrainerListBox* CreateContainerL();
	static TInt RunQrySureL( const TDesC* aOverrideText = NULL );
	// ]]] end generated region [Generated Methods]

	// ]]] end [Public Section]

	CFiruData& Data();

	// [[[ begin [Protected Section]
protected:
	// from base class CAknView
	void DoActivateL(
		const TVwsViewId& aPrevViewId,
		TUid aCustomMessageId,
		const TDesC8& aCustomMessage );
	void DoDeactivate();
	void HandleStatusPaneSizeChange();

	// [[[ begin generated region: do not modify [Overridden Methods]
	// ]]] end generated region [Overridden Methods]


	// [[[ begin [User Handlers]
	TBool HandleReset_statsMenuItemSelectedL( TInt aCommand );
	TBool HandleCancelMenuItemSelectedL( TInt aCommand );
	TBool HandleControlPaneRightSoftKeyPressedL( TInt aCommand );
	TBool HandleSelectMenuItemSelectedL( TInt aCommand );
    TBool HandleShow_detailsMenuItemSelectedL( TInt aCommand );
	// ]]] end [User Handlers]

	// ]]] end [Protected Section]

    void NextTestL();
    void ShowStatsL( const CFiruExercise& aExercise );

	// [[[ begin [Private Section]
private:
	void SetupStatusPaneL();
	void CleanupStatusPane();

	// [[[ begin generated region: do not modify [Generated Instance Variables]
	CFiruTrainerListBox* iFiruTrainerListBox;
	// ]]] end generated region [Generated Instance Variables]

	// [[[ begin generated region: do not modify [Generated Methods]
	// ]]] end generated region [Generated Methods]

	// ]]] end [Private Section]

	CFiruExercise* iExercise;
	CFiruTest* iCurrentTest;
	};

#endif // FIRUTRAINERLISTBOXVIEW_H
