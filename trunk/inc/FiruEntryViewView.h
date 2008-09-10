/*
========================================================================
 Name        : FiruEntryViewView.h
 Author      : ${author}
 Copyright   : ${copyright}
 Description : 
========================================================================
*/
#ifndef FIRUENTRYVIEWVIEW_H
#define FIRUENTRYVIEWVIEW_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <aknview.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Constants]
// ]]] end generated region [Generated Constants]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class CFiruEntryView;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Avkon view class for FiruEntryViewView. It is register with the view server
 * by the AppUi. It owns the container control.
 * @class	CFiruEntryViewView FiruEntryViewView.h
 */						
			
class CFiruEntryViewView : public CAknView
	{
	
	
	// [[[ begin [Public Section]
public:
	// constructors and destructor
	CFiruEntryViewView();
	static CFiruEntryViewView* NewL();
	static CFiruEntryViewView* NewLC();        
	void ConstructL();
	virtual ~CFiruEntryViewView();
						
	// from base class CAknView
	TUid Id() const;
	void HandleCommandL( TInt aCommand );
	
	// [[[ begin generated region: do not modify [Generated Methods]
	CFiruEntryView* CreateContainerL();
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Public Section]
	
	
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
	// ]]] end [User Handlers]
	
	// ]]] end [Protected Section]
	
	
	// [[[ begin [Private Section]
private:
	void SetupStatusPaneL();
	void CleanupStatusPane();
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
	CFiruEntryView* iFiruEntryView;
	// ]]] end generated region [Generated Instance Variables]
	
	// [[[ begin generated region: do not modify [Generated Methods]
	// ]]] end generated region [Generated Methods]
	
	// ]]] end [Private Section]
	
	};

#endif // FIRUENTRYVIEWVIEW_H
