CREATE TABLE entries_fi ( id INTEGER PRIMARY KEY AUTOINCREMENT, text TEXT );
CREATE INDEX index_entries_fi_text ON entries_fi ( text ASC );

CREATE TABLE trans_fi_ru (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        sid INTEGER NOT NULL REFERENCES entries_fi ( id ) ON DELETE CASCADE,
        text TEXT,
        fmark INTEGER DEFAULT 0,
        rmark INTEGER DEFAULT 0 );
CREATE INDEX index_trans_fi_ru_sid ON trans_fi_ru( sid );
CREATE INDEX index_trans_fi_ru_text ON trans_fi_ru ( text ASC );
CREATE INDEX index_trans_fi_ru_fmark ON trans_fi_ru ( fmark ASC );
CREATE INDEX index_trans_fi_ru_rmark ON trans_fi_ru ( rmark ASC );

