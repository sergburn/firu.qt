#include "query.h"

// ----------------------------------------------------------------------------

Query::Query()
{
}

// ----------------------------------------------------------------------------

Status Query::execute()
{
}

// ----------------------------------------------------------------------------

int Query::errorCode() const
{
}

// ----------------------------------------------------------------------------

int Query::readNextRecord()
{
}

// ----------------------------------------------------------------------------

int Query::readNextRecord()
{
    int err = sqlite3_step( m_stmt );
    switch ( err )
    {
        case SQLITE_ROW:
            readRecord();
            break;
        case SQLITE_DONE:
            err = SQLITE_OK;
            break;
        default: // error
            LogSqliteError( "readNextRecord" );
            break;
    }
    return err;
}

