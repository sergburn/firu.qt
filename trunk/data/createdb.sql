CREATE TABLE entries_fi ( id INTEGER PRIMARY KEY AUTOINCREMENT, text text );
CREATE TABLE entries_ru ( id INTEGER PRIMARY KEY AUTOINCREMENT, text text );
CREATE INDEX index_entries_fi_text ON entries_fi ( text ASC );
CREATE INDEX index_entries_ru_text ON entries_ru ( text ASC );

CREATE TABLE trans_fi_ru (
        sid INTEGER NOT NULL REFERENCES entries_fi ( id ) ON DELETE CASCADE,
        tid INTEGER NOT NULL REFERENCES entries_ru ( id ) ON DELETE CASCADE,
        fmark INTEGER DEFAULT 0,
        rmark INTEGER DEFAULT 0,
        PRIMARY KEY ( sid, tid ) );
CREATE INDEX index_trans_fi_ru_tid ON trans_fi_ru( tid );
CREATE INDEX index_trans_fi_ru_fmark ON trans_fi_ru ( fmark ASC );
CREATE INDEX index_trans_fi_ru_rmark ON trans_fi_ru ( rmark ASC );

