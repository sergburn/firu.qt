/*
========================================================================
 Name        : FiruDictListBoxView.h
 Author      : 
 Copyright   : Your copyright notice
 Description : 
========================================================================
*/
#ifndef FIRUDICTLISTBOXVIEW_H
#define FIRUDICTLISTBOXVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CFiruDictListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for FiruDictListBoxView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CFiruDictListBoxView FiruDictListBoxView.h
 */						
			
class CFiruDictListBoxView : public CAknView
	{
	
	
	// [[[ begin [Public Section]
public:
	// constructors and destructor
	CFiruDictListBoxView();
	static CFiruDictListBoxView* NewL();
	static CFiruDictListBoxView* NewLC();        
	void ConstructL();
	virtual ~CFiruDictListBoxView();
						
	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );
	
	// [[[ begin generated region: do not modify [Generated Methods]
	CFiruDictListBox* CreateContainerL();
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
	TBool HandleControlPaneRightSoftKeyPressedL( TInt aCommand );
	TBool HandleReverseMenuItemSelectedL( TInt aCommand );
	TBool HandleOpenEntryL( TInt aCommand );
	TBool HandleCompactMenuItemSelectedL( TInt aCommand );
	// ]]] end [User Handlers]
	
	// ]]] end [Protected Section]
	
	
	// [[[ begin [Private Section]
private:
	void SetupStatusPaneL();
	void CleanupStatusPane();
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
	CFiruDictListBox* iFiruDictListBox;
	// ]]] end generated region [Generated Instance Variables]
	
	// [[[ begin generated region: do not modify [Generated Methods]
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Private Section]
	
	};

#endif // FIRUDICTLISTBOXVIEW_H
