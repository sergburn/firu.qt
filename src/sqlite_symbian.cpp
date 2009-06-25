/*
** 2009 June
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This file contains code that is specific to Symbian.
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <e32std.h>
#include <f32file.h>
#include <charconv.h>
#include <bautils.h>
#include <unistd.h>
#include <utf.h>

#include <sqlite3.h>
#include <QtGlobal>

#include "sqlite_symbian.h"

/*
** The symbianFile structure is a subclass of sqlite3_file* specific to the symbian
** portability layer.
*/

#define MAX_FILE_NAME KMaxFileName
#define ZSTR2PTR(_zstr) TPtrC8( (const TUint8*) _zstr )
#define RFS( _vfs ) (*((RFs*)(_vfs->pAppData)))

/** From sqlite3.c */
#define NO_LOCK         0
#define SHARED_LOCK     1
#define RESERVED_LOCK   2
#define PENDING_LOCK    3
#define EXCLUSIVE_LOCK  4

// ----------------------------------------------------------------------------

struct symFile 
{
    const struct sqlite3_io_methods *pMethods;  /* Methods for an open file */
	int isOpen;
	unsigned char locktype; /* Type of lock currently held on this file */
	short sharedLockByte;   /* Randomly chosen byte used as a shared lock */
//	char fileName[KMaxFileName];
	RFile file;
};


// ----------------------------------------------------------------------------

int symClose( sqlite3_file *id )
{
    int rc, cnt = 0;
    symFile *pFile = (symFile*) id;
    pFile->file.Close();
    return SQLITE_OK;
}

// ----------------------------------------------------------------------------
/*
 ** Read data from a file into a buffer.  Return SQLITE_OK if all
 ** bytes were read successfully and SQLITE_IOERR if anything goes
 ** wrong.
 */
int symRead( 
    sqlite3_file *id,       /* File to read from */
    void *pBuf,             /* Write content into this buffer */
    int amt,                /* Number of bytes to read */
    sqlite3_int64 offset    /* Begin reading at this offset */
    )
{
    symFile *pFile = (symFile*) id;
    TInt tOffset = (TInt) offset;
    if ( pFile->file.Seek( ESeekStart, tOffset ) != KErrNone )
    {
        return SQLITE_FULL;
    }

    TPtr8 buf( (TUint8*) pBuf, amt );
    if ( pFile->file.Read( buf, amt ) != KErrNone )
    {
        return SQLITE_IOERR_READ;
    }

    int got = buf.Length();
    if ( got == 0 )
    {
        TInt size = 0;
        if ( pFile->file.Size( size ) != KErrNone )
        {
            return SQLITE_IOERR_READ;
        }
        if ( size == 0 )
        {
            return SQLITE_IOERR_SHORT_READ;
        }
        return SQLITE_IOERR_READ;
    }
    if ( got == amt )
    {
        return SQLITE_OK;
    }
    else
    {
        return SQLITE_IOERR_SHORT_READ;
    }
}

// ----------------------------------------------------------------------------
/*
** Write data from a buffer into a file.  Return SQLITE_OK on success
** or some other error code on failure.
*/
int symWrite( 
    sqlite3_file *id,       /* File to write into */
    const void *pBuf,       /* The bytes to be written */
    int amt,                /* Number of bytes to write */
    sqlite3_int64 offset    /* Offset into the file to begin writing at */
    )
{
    symFile *pFile = (symFile*) id;

    TInt tOffset = (TInt) offset;
    if ( pFile->file.Seek( ESeekStart, tOffset ) != KErrNone )
    {
        return SQLITE_FULL;
    }

    TPtrC8 ptr( (TUint8*) pBuf, amt );
    if ( pFile->file.Write( ptr, amt ) != KErrNone )
    {
        return SQLITE_FULL;
    }

    return SQLITE_OK;
}

// ----------------------------------------------------------------------------

int symTruncate( sqlite3_file *id, sqlite3_int64 nByte )
{
    symFile *pFile = (symFile*)id;
    if (pFile->file.SetSize(nByte) != KErrNone)
    {
        return SQLITE_IOERR; 
    }
    return SQLITE_OK;
}

// ----------------------------------------------------------------------------

int symSync( sqlite3_file *id, int flags )
{
    symFile *pFile = (symFile*) id;
    if ( pFile->file.Flush() != KErrNone )
    {
        return SQLITE_IOERR;
    }
    return SQLITE_OK;
}

// ----------------------------------------------------------------------------

int symFileSize( sqlite3_file *id, sqlite3_int64 *pSize )
{
    symFile *pFile = (symFile*) id;

    TInt size = 0;
    if ( pFile->file.Size( size ) != KErrNone )
    {
        return SQLITE_IOERR;
    }

    *pSize = size;
    return SQLITE_OK;
}

// ----------------------------------------------------------------------------

int symLock( sqlite3_file *id, int locktype )
{
    int rc = SQLITE_OK; /* Return code from subroutines */
    int res = 1; /* Result of a windows lock call */
    int newLocktype; /* Set pFile->locktype to this value before exiting */
    int gotPendingLock = 0;/* True if we acquired a PENDING lock this time */
    symFile *pFile = (symFile*) id;

    qDebug( "LOCK %d %d was %d(%d)\n", pFile->file.SubSessionHandle(), locktype, pFile->locktype,
        pFile->sharedLockByte );

    TInt size = 0;
    if ( pFile->file.Size( size ) == KErrNone )
        return SQLITE_OK;

    return SQLITE_BUSY;
}

// ----------------------------------------------------------------------------

int symUnlock( sqlite3_file *id, int locktype )
{
//    symFile *pFile = (symFile*) id;
    return SQLITE_OK;
}

// ----------------------------------------------------------------------------
/*
** This routine checks if there is a RESERVED lock held on the specified
** file by this or any other process. If such a lock is held, return
** non-zero, otherwise zero.
*/
int symCheckLock( sqlite3_file *id, int *pResOut )
{
    symFile *pFile = (symFile*) id;
    if ( pFile->locktype >= RESERVED_LOCK )
    {
        *pResOut = 1;
        qDebug( "TEST WR-LOCK %d %d (local)\n", pFile->file.SubSessionHandle(), *pResOut );
    }
    else
    {
        TInt size = 0;
        if ( pFile->file.Size( size ) == KErrNone )
            *pResOut = 1;
        qDebug( "TEST WR-LOCK %d %d (remote)\n", pFile->file.SubSessionHandle(), *pResOut );
    }
    return SQLITE_OK;
}

// ----------------------------------------------------------------------------

int symFileControl( sqlite3_file *id, int op, void *pArg )
{
    switch ( op )
    {
        case SQLITE_FCNTL_LOCKSTATE:
        {
            *(int*) pArg = ((symFile*) id)->locktype;
            return SQLITE_OK;
        }
    }
    return SQLITE_ERROR;
}

// ----------------------------------------------------------------------------

int symSectorSize( sqlite3_file* /*id*/ )
{
    return KDefaultVolumeBlockSize;
}

// ----------------------------------------------------------------------------

int symDevice( sqlite3_file* /*id*/ )
{
    return 0;
}

// ----------------------------------------------------------------------------

struct sqlite3_io_methods symbian_io = 
{
    1,                              // int iVersion;
    symClose,                       // int (*xClose)(sqlite3_file*);
    symRead,                        // int (*xRead)(sqlite3_file*, void*, int iAmt, sqlite3_int64 iOfst);
    symWrite,                       // int (*xWrite)(sqlite3_file*, const void*, int iAmt, sqlite3_int64 iOfst);
    symTruncate,                    // int (*xTruncate)(sqlite3_file*, sqlite3_int64 size);
    symSync,                        // int (*xSync)(sqlite3_file*, int flags);
    symFileSize,                    // int (*xFileSize)(sqlite3_file*, sqlite3_int64 *pSize);
    symLock,                        // int (*xLock)(sqlite3_file*, int);
    symUnlock,                      // int (*xUnlock)(sqlite3_file*, int);
    symCheckLock,                   // int (*xCheckReservedLock)(sqlite3_file*, int *pResOut);
    symFileControl,                 // int (*xFileControl)(sqlite3_file*, int op, void *pArg);
    symSectorSize,                  // int (*xSectorSize)(sqlite3_file*);
    symDevice,                      // int (*xDeviceCharacteristics)(sqlite3_file*);
};

// ----------------------------------------------------------------------------

int symOpen( 
    sqlite3_vfs *pVfs,
    const char *zName,  /* Name of the file (UTF-8) */
    sqlite3_file *id,   /* Write the SQLite file handle here */
    int flags,          /* Open mode flags */
    int* /*pOutFlags*/      /* Status return flags */
    )
{
    symFile *pFile = (symFile*) id;

    pFile->isOpen = 0;
    pFile->locktype = NO_LOCK;
    pFile->pMethods = &symbian_io;
    pFile->sharedLockByte = 0;

    TFileName filename;
    CnvUtfConverter::ConvertToUnicodeFromUtf8( filename, ZSTR2PTR( zName ) );

    int ret = 0;
    if ( flags & SQLITE_OPEN_CREATE )
    {
        if ( BaflUtils::FileExists( RFS( pVfs ), filename ) == 1 )
        {
            ret = pFile->file.Open( RFS( pVfs ), filename, EFileStream | EFileWrite | EFileShareAny );
        }
        else
        {
            ret = pFile->file.Create( RFS( pVfs ), filename, EFileStream | EFileWrite | EFileShareAny );
        }
    }
    else if ( flags & SQLITE_OPEN_READWRITE )
    {
        ret = pFile->file.Open( RFS( pVfs ), filename, EFileStream | EFileWrite | EFileShareAny );
    }
    else
    {
        ret = pFile->file.Open( RFS( pVfs ), filename, EFileStream | EFileRead | EFileShareAny );
    }

    if ( ret != KErrNone )
    {
        return SQLITE_IOERR;
    }

    pFile->isOpen = 1;
    return SQLITE_OK;
}

// ----------------------------------------------------------------------------

int symDelete( 
    sqlite3_vfs *pVfs,
    const char *zFilename,  /* Name of file to delete */
    int syncDir             /* Not used on win32 */
    )
{
    TFileName filename;
    CnvUtfConverter::ConvertToUnicodeFromUtf8( filename, ZSTR2PTR( zFilename ) );
    
    if ( BaflUtils::DeleteFile( RFS( pVfs ), filename ) == KErrNone )
        return SQLITE_OK;
    else
        return SQLITE_IOERR_DELETE;
}

// ----------------------------------------------------------------------------

int symAccess( 
    sqlite3_vfs *pVfs, 
    const char *zFilename,  /* Name of file to check */
    int flags,              /* Type of test to make on this file */
    int *pResOut            /* Write result boolean here */
    )
{
    TFileName filename;
    CnvUtfConverter::ConvertToUnicodeFromUtf8( filename, ZSTR2PTR( zFilename ) );

    switch ( flags )
    {
        case SQLITE_ACCESS_READ:
        case SQLITE_ACCESS_EXISTS:
        {
            *pResOut = BaflUtils::FileExists( RFS( pVfs ), filename );
            break;
        }   
        case SQLITE_ACCESS_READWRITE:
        {
            TUint att( 0 );
            RFS( pVfs ).Att( filename, att );
            *pResOut = !( att & KEntryAttReadOnly );
            break;
        }
        default:
            break;
    }
    return SQLITE_OK;
}

// ----------------------------------------------------------------------------

int symFullName(
    sqlite3_vfs *pVfs,      /* Pointer to vfs object */
    const char *zRelative,  /* Possibly relative input path */
    int nFull,              /* Size of output buffer in bytes */
    char *zFull             /* Output buffer */
)
{
    TFileName relative;
    CnvUtfConverter::ConvertToUnicodeFromUtf8( relative, ZSTR2PTR( zRelative ) );
    
    TParse parse;
    RFS( pVfs ).Parse( relative, parse );
    
    TPtr8 full( (TUint8*) zFull, MAX_FILE_NAME );
    CnvUtfConverter::ConvertFromUnicodeToUtf8( full, parse.FullName() );
    full.ZeroTerminate();
    
    return SQLITE_OK;
}

// ----------------------------------------------------------------------------

int symRandom( 
    sqlite3_vfs* /*pVfs*/, 
    int nBuf, 
    char *zBuf )
{
    int i;
    for (i=0; i<nBuf; ++i)
    {
        zBuf[i] = rand() % 255;
    }
    return nBuf;
}


// ----------------------------------------------------------------------------

int symSleep(
    sqlite3_vfs* /*pVfs*/, 
    int microsec)
{
    return sleep( microsec / 1000 );
}

// ----------------------------------------------------------------------------

/*
** Find the current time (in Universal Coordinated Time).  Write the
** current time and date as a Julian Day number into *prNow and
** return 0.  Return 1 if the time and date cannot be found.
*/
int symTime(
    sqlite3_vfs* /*pVfs*/, 
    double* prNow )
{
    TTime now;
    now.UniversalTime();
    
    *(TInt64*)prNow = now.Int64();
    return 0;
}

// ----------------------------------------------------------------------------

struct sqlite3_vfs symbian_vfs = 
{
    1,                              // int iVersion;
    sizeof( symFile ),              // int szOsFile;
    MAX_FILE_NAME,                  // int mxPathname; in UTF8
    NULL,                           // sqlite3_vfs *pNext;
    "symbian",                      // const char *zName;
    NULL,                           // void *pAppData;
    symOpen,                        // int (*xOpen)(sqlite3_vfs*, const char *zName, sqlite3_file*, int flags, int *pOutFlags);
    symDelete,                      // int (*xDelete)(sqlite3_vfs*, const char *zName, int syncDir);
    symAccess,                      // int (*xAccess)(sqlite3_vfs*, const char *zName, int flags, int *pResOut);
    symFullName,                    // int (*xFullPathname)(sqlite3_vfs*, const char *zName, int nOut, char *zOut);
    NULL,                           // void *(*xDlOpen)(sqlite3_vfs*, const char *zFilename);
    NULL,                           // void (*xDlError)(sqlite3_vfs*, int nByte, char *zErrMsg);
    NULL,                           // void (*(*xDlSym)(sqlite3_vfs*,void*, const char *zSymbol))(void);
    NULL,                           // void (*xDlClose)(sqlite3_vfs*, void*);
    symRandom,                      // int (*xRandomness)(sqlite3_vfs*, int nByte, char *zOut);
    symSleep,                       // int (*xSleep)(sqlite3_vfs*, int microseconds);
    symTime,                        // int (*xCurrentTime)(sqlite3_vfs*, double*);
    NULL,                           // int (*xGetLastError)(sqlite3_vfs*, int, char *);
};

// ----------------------------------------------------------------------------

int register_symbian_vfs( RFs& fs )
{
    symbian_vfs.pAppData = &fs;
    return sqlite3_vfs_register( &symbian_vfs, 1 );
}
