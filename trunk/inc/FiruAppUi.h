/*
========================================================================
 Name        : FiruAppUi.h
 Author      : 
 Copyright   : Your copyright notice
 Description : 
========================================================================
*/
#ifndef FIRUAPPUI_H
#define FIRUAPPUI_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknviewappui.h>
// ]]] end generated region [Generated Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CFiruListBoxView;
class CFiruDictListBoxView;
class CFiruTrainerListBoxView;
class CFiruContainerView;
class CFiruTesterView;
// ]]] end generated region [Generated Forward Declarations]

class CFiruData;

/**
 * @class	CFiruAppUi FiruAppUi.h
 * @brief The AppUi class handles application-wide aspects of the user interface, including
 *        view management and the default menu, control pane, and status pane.
 */
class CFiruAppUi : public CAknViewAppUi
	{
public: 
	// constructor and destructor
	CFiruAppUi();
	virtual ~CFiruAppUi();
	void ConstructL();

	RFs& FsSession();
	CFiruData& Data();
	
public:
	// from CCoeAppUi
	TKeyResponse HandleKeyEventL(
				const TKeyEvent& aKeyEvent,
				TEventCode aType );

	// from CEikAppUi
	void HandleCommandL( TInt aCommand );
	void HandleResourceChangeL( TInt aType );

	// from CAknAppUi
	void HandleViewDeactivation( 
			const TVwsViewId& aViewIdToBeDeactivated, 
			const TVwsViewId& aNewlyActivatedViewId );

private:
	void InitializeContainersL();
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
	static void RunErrorNoteL( const TDesC* aOverrideText = NULL );
	static void RunInfoNoteL( const TDesC* aOverrideText = NULL );
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CFiruListBoxView* iFiruListBoxView;
	CFiruDictListBoxView* iFiruDictListBoxView;
	CFiruTrainerListBoxView* iFiruTrainerListBoxView;
	CFiruContainerView* iFiruContainerView;
	CFiruTesterView* iFiruTesterView;
	// ]]] end generated region [Generated Instance Variables]
	
	
	// [[[ begin [User Handlers]
protected: 
	// ]]] end [User Handlers]
	};

#endif // FIRUAPPUI_H			
