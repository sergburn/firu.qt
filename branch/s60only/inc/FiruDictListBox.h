/*
 ========================================================================
 Name        : FiruDictListBox.h
 Author      : 
 Copyright   : Your copyright notice
 Description : 
 ========================================================================
 */
#ifndef FIRUDICTLISTBOX_H
#define FIRUDICTLISTBOX_H

// [[[ begin generated region: do not modify [Generated Includes]
#include <coecntrl.h>		
#include <coecobs.h>
// ]]] end generated region [Generated Includes]


// [[[ begin [Event Handler Includes]
#include <eiklbo.h>
// ]]] end [Event Handler Includes]

// [[[ begin generated region: do not modify [Generated Forward Declarations]
class MEikCommandObserver;		
class CAknSingleStyleListBox;
class CEikTextListBox;
// ]]] end generated region [Generated Forward Declarations]
class CFiruEntry;
class CFiruDictListBoxModel;

/**
 * Container class for FiruDictListBox
 * 
 * @class	CFiruDictListBox FiruDictListBox.h
 */
class CFiruDictListBox : public CCoeControl, MEikListBoxObserver
    ,MCoeControlObserver{
public:
    // constructors and destructor
    CFiruDictListBox( );
    static CFiruDictListBox* NewL(
        const TRect& aRect,
        const CCoeControl* aParent,
        MEikCommandObserver* aCommandObserver,
        CFiruData& aData );
    static CFiruDictListBox* NewLC(
        const TRect& aRect,
        const CCoeControl* aParent,
        MEikCommandObserver* aCommandObserver,
        CFiruData& aData );
    void ConstructL(
        const TRect& aRect,
        const CCoeControl* aParent,
        MEikCommandObserver* aCommandObserver );
    virtual ~CFiruDictListBox( );

public:
    // from base class CCoeControl
    TInt CountComponentControls( ) const;
    CCoeControl* ComponentControl( TInt aIndex ) const;
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
    void HandleResourceChange( TInt aType );

protected:
    // from base class CCoeControl
    void SizeChanged( );

private:
    // from base class CCoeControl
    void Draw( const TRect& aRect ) const;

private:
    void InitializeControlsL( );
    void InitializeControls2L();
    void LayoutControls( );
    void LayoutControls2( );
    CCoeControl* iFocusControl;
    MEikCommandObserver* iCommandObserver;
    // [[[ begin generated region: do not modify [Generated Methods]
public: 
	static void AddListBoxItemL( 
			CEikTextListBox* aListBox,
			const TDesC& aString );
	static RArray< TInt >* GetSelectedListBoxItemsLC( CEikTextListBox* aListBox );
	static void DeleteSelectedListBoxItemsL( CEikTextListBox* aListBox );
	CAknSingleStyleListBox* ListBox();
	static void CreateListBoxItemL( TDes& aBuffer, 
			const TDesC& aMainText );
	void AddListBoxResourceArrayItemL( TInt aResourceId );
	void SetupListBoxIconsL();
	TBool HandleMarkableListCommandL( TInt aCommand );
    // ]]] end generated region [Generated Methods]

    void RefreshListBoxL( );
    void ResetFilterL();
    CFiruEntry* CurrentItem( );
    
    CFiruDictListBoxModel& Model();

    // [[[ begin generated region: do not modify [Generated Type Declarations]
public: 
    // ]]] end generated region [Generated Type Declarations]

    // [[[ begin generated region: do not modify [Generated Instance Variables]
private: 
	CAknSingleStyleListBox* iListBox;
    // ]]] end generated region [Generated Instance Variables]
    CEikEdwin* iSearchField;

    // [[[ begin [Overridden Methods]
protected:
    void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent anEventType );
    void HandleControlEventL( 
            CCoeControl* aControl, 
            TCoeEvent anEventType );
    // ]]] end [Overridden Methods]


    // [[[ begin [User Handlers]
protected:
    void HandleListBoxItemClickedL( CEikListBox* aListBox, TListBoxEvent anEventType );
    void HandleOpenItemL( CEikListBox* aListBox, TListBoxEvent anEventType );
    void HandleListBoxEditingStoppedL( CEikListBox* aListBox, TListBoxEvent anEventType );
    void HandleListBoxStateChangedL( 
            CCoeControl* aControl, 
            TCoeEvent anEvent );
    // ]]] end [User Handlers]
    
    void HandleSearchFieldStateChangedL( CCoeControl*, TCoeEvent );
public:
    enum TControls
    {
        // [[[ begin generated region: do not modify [Generated Contents]
		EListBox,
		
        // ]]] end generated region [Generated Contents]

        // add any user-defined entries here...
        ESearchField,

        ELastControl
    };
    enum TListBoxImages
    {
        // [[[ begin generated region: do not modify [Generated Enums]
		EListBoxFirstUserImageIndex
		
        // ]]] end generated region [Generated Enums]

    };

    // [[[ begin [MEikListBoxObserver support]
private:
    typedef void ( CFiruDictListBox::*ListBoxEventHandler )(
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

    RArray< TListBoxEventDispatch> iListBoxEventDispatch;
    // ]]] end [MEikListBoxObserver support]
    
    CFiruDictListBoxModel* iModel;
    
    // [[[ begin [MCoeControlObserver support]
private: 
    typedef void ( CFiruDictListBox::*ControlEventHandler )( 
            CCoeControl* aControl, TCoeEvent anEvent );
    
    void AddControlEventHandlerL( 
            CCoeControl* aControl, 
            TCoeEvent anEvent, 
            ControlEventHandler aHandler );
    
    class TControlEventDispatch 
        {
    public: 
        CCoeControl* src; 
        TCoeEvent event; 
        ControlEventHandler handler;
        };
        
    RArray< TControlEventDispatch > iControlEventDispatch;
    // ]]] end [MCoeControlObserver support]
    
};

#endif // FIRUDICTLISTBOX_H
