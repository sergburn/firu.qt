/*
========================================================================
 Name        : FiruDocument.h
 Author      : 
 Copyright   : Your copyright notice
 Description : 
========================================================================
*/
#ifndef FIRUDOCUMENT_H
#define FIRUDOCUMENT_H

#include <e32des16.h>
#include <akndoc.h>
		
class CEikAppUi;
class CFiruData;

/**
* @class	CFiruDocument FiruDocument.h
* @brief	A CAknDocument-derived class is required by the S60 application 
*           framework. It is responsible for creating the AppUi object. 
*/
class CFiruDocument : public CAknDocument
	{
public: 
	// constructor
	static CFiruDocument* NewL( CEikApplication& aApp );
	~CFiruDocument();
	
	void InitL();
	CFiruData& Data();
	
private: 
	// constructors
	CFiruDocument( CEikApplication& aApp );
	void ConstructL();
	
	void ImportDictionaryL();
	HBufC* ReadLineLC( RFile& aFile, RBuf8& aBuffer );
	
public: 
	// from base class CEikDocument
	CEikAppUi* CreateAppUiL();
	
private:
    RFs iFsSession;
    CFiruData* iData;
	};
#endif // FIRUDOCUMENT_H
