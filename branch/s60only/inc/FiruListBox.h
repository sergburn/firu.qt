/*
========================================================================
 Name        : FiruListBox.h
 Author      : 
 Copyright   : Your copyright notice
 Description : 
========================================================================
*/
#ifndef FIRULISTBOX_H
#define FIRULISTBOX_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <coecntrl.h>        
#include <akniconutils.h>
#include <gulicon.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
#include <eiklbo.h>
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;        
class CAknSingleLargeStyleListBox;
class CEikTextListBox;
// ]]] end generated region [Generated Forward Declarations]

/**
 * Container class for FiruListBox
 * 
 * @class	CFiruListBox FiruListBox.h
 */
class CFiruListBox : public CCoeControl
	,MEikListBoxObserver	{
public:
	// constructors and destructor
	CFiruListBox();
	static CFiruListBox* NewL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	static CFiruListBox* NewLC( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	void ConstructL( 
		const TRect& aRect, 
		const CCoeControl* aParent, 
		MEikCommandObserver* aCommandObserver );
	virtual ~CFiruListBox();

public:
	// from base class CCoeControl
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl( TInt aIndex ) const;
	TKeyResponse OfferKeyEventL( 
			const TKeyEvent& aKeyEvent, 
			TEventCode aType );
	void HandleResourceChange( TInt aType );
	
protected:
	// from base class CCoeControl
	void SizeChanged();

private:
	// from base class CCoeControl
	void Draw( const TRect& aRect ) const;

private:
	void InitializeControlsL();
	void LayoutControls();
	CCoeControl* iFocusControl;
	MEikCommandObserver* iCommandObserver;
	// [[[ begin generated region: do not modify [Generated Methods]
public: 
    static void AddListBoxItemL( 
            CEikTextListBox* aListBox,
            const TDesC& aString );
    static RArray< TInt >* GetSelectedListBoxItemsLC( CEikTextListBox* aListBox );
    static void DeleteSelectedListBoxItemsL( CEikTextListBox* aListBox );
    CAknSingleLargeStyleListBox* ListBox();
    static void CreateListBoxItemL( TDes& aBuffer, 
            TInt aIconIndex,
            const TDesC& aMainText );
    void AddListBoxResourceArrayItemL( TInt aResourceId, TInt aIconIndex );
    void SetupListBoxIconsL();
    static CGulIcon* LoadAndScaleIconL(
            const TDesC& aFileName,
            TInt aBitmapId,
            TInt aMaskId,
            TSize* aSize,
            TScaleMode aScaleMode );
    TBool HandleMarkableListCommandL( TInt aCommand );
	// ]]] end generated region [Generated Methods]
	
	// [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
	// ]]] end generated region [Generated Type Declarations]
	
	// [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
    CAknSingleLargeStyleListBox* iListBox;
	// ]]] end generated region [Generated Instance Variables]
	
	
	// [[[ begin [Overridden Methods]
protected: 
	void HandleListBoxEventL( 
			CEikListBox* aListBox,
			TListBoxEvent anEventType );
	// ]]] end [Overridden Methods]
	
	
	// [[[ begin [User Handlers]
protected: 
	// ]]] end [User Handlers]
	
public: 
	enum TControls
		{
		// [[[ begin generated region: do not modify [Generated Contents]
        EListBox,
        
		// ]]] end generated region [Generated Contents]
		
		// add any user-defined entries here...
		
		ELastControl
		};
	enum TListBoxImages
		{
		// [[[ begin generated region: do not modify [Generated Enums]
        EListBoxFiruList_iconIndex = 0,
        EListBoxFirstUserImageIndex
        
		// ]]] end generated region [Generated Enums]
		
		};
	
	// [[[ begin [MEikListBoxObserver support]
private: 
	typedef void ( CFiruListBox::*ListBoxEventHandler )( 
			CEikListBox* aListBox, 
			TListBoxEvent anEvent );
	
	void AddListBoxEventHandlerL( 
			CEikListBox* aListBox, 
			TListBoxEvent anEvent, 
			ListBoxEventHandler aHandler );
	
	struct TListBoxEventDispatch 
		{ 
		CEikListBox* src; 
		TListBoxEvent event; 
		ListBoxEventHandler handler;
		};
		
	RArray< TListBoxEventDispatch > iListBoxEventDispatch;
	// ]]] end [MEikListBoxObserver support]
	
	};
				
#endif // FIRULISTBOX_H
