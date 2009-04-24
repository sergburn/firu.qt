
#ifndef FIRU_PAN_H
#define FIRU_PAN_H

/** Firu application panic codes */
enum TFiruPanics
	{
	EFiruUi = 1
	// add further panics here
	};

inline void Panic(TFiruPanics aReason)
	{
	_LIT(applicationName,"Firu");
	User::Panic(applicationName, aReason);
	}

#endif // FIRU_PAN_H
