// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DISARMPEDS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DISARMPEDS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DISARMPEDS_EXPORTS
#define DISARMPEDS_API __declspec(dllexport)
#else
#define DISARMPEDS_API __declspec(dllimport)
#endif

// This class is exported from the dll
class DISARMPEDS_API CDisarmPeds {
public:
	CDisarmPeds(void);
	// TODO: add your methods here.
};

extern DISARMPEDS_API int nDisarmPeds;

DISARMPEDS_API int fnDisarmPeds(void);
