BEGIN TRANSACTION;
CREATE TABLE entries_fi ( id integer primary key autoincrement, text text);
DELETE FROM sqlite_sequence;
CREATE TABLE entries_ru ( id integer primary key autoincrement, text text);
CREATE TABLE trans_fi_ru ( id integer primary key autoincrement, sid integer not null references entries_fi (id) on delete cascade, tid integer not null references entries_ru (id) on delete cascade, fmark integer default 0, rmark integer default 0 );
CREATE INDEX index_text_fi on entries_fi ( text asc );
CREATE INDEX index_text_ru on entries_ru ( text asc );
CREATE INDEX index_fmark_fi_ru ON trans_fi_ru (fmark ASC);
CREATE INDEX index_rmark_fi_ru ON trans_fi_ru (rmark ASC);
CREATE INDEX IF NOT EXISTS index_sid_fi_ru ON trans_fi_ru (sid);
CREATE INDEX IF NOT EXISTS index_tid_fi_ru ON trans_fi_ru (tid);
CREATE TRIGGER genfkey1_insert_referencing BEFORE INSERT ON "trans_fi_ru" WHEN
    new."tid" IS NOT NULL AND NOT EXISTS (SELECT 1 FROM "entries_ru" WHERE new."tid" == "id")
BEGIN
  SELECT RAISE(ABORT, 'constraint failed');
END;
CREATE TRIGGER genfkey1_update_referencing BEFORE
    UPDATE OF tid ON "trans_fi_ru" WHEN
    new."tid" IS NOT NULL AND
    NOT EXISTS (SELECT 1 FROM "entries_ru" WHERE new."tid" == "id")
BEGIN
  SELECT RAISE(ABORT, 'constraint failed');
END;
CREATE TRIGGER genfkey1_delete_referenced BEFORE DELETE ON "entries_ru" WHEN
    EXISTS (SELECT 1 FROM "trans_fi_ru" WHERE old."id" == "tid")
BEGIN
  DELETE FROM "trans_fi_ru" WHERE "tid" = old."id";
END;
CREATE TRIGGER genfkey1_update_referenced AFTER
    UPDATE OF id ON "entries_ru" WHEN
    EXISTS (SELECT 1 FROM "trans_fi_ru" WHERE old."id" == "tid")
BEGIN
  SELECT RAISE(ABORT, 'constraint failed');
END;
CREATE TRIGGER genfkey2_insert_referencing BEFORE INSERT ON "trans_fi_ru" WHEN
    new."sid" IS NOT NULL AND NOT EXISTS (SELECT 1 FROM "entries_fi" WHERE new."sid" == "id")
BEGIN
  SELECT RAISE(ABORT, 'constraint failed');
END;
CREATE TRIGGER genfkey2_update_referencing BEFORE
    UPDATE OF sid ON "trans_fi_ru" WHEN
    new."sid" IS NOT NULL AND
    NOT EXISTS (SELECT 1 FROM "entries_fi" WHERE new."sid" == "id")
BEGIN
  SELECT RAISE(ABORT, 'constraint failed');
END;
CREATE TRIGGER genfkey2_delete_referenced BEFORE DELETE ON "entries_fi" WHEN
    EXISTS (SELECT 1 FROM "trans_fi_ru" WHERE old."id" == "sid")
BEGIN
  DELETE FROM "trans_fi_ru" WHERE "sid" = old."id";
END;
CREATE TRIGGER genfkey2_update_referenced AFTER
    UPDATE OF id ON "entries_fi" WHEN
    EXISTS (SELECT 1 FROM "trans_fi_ru" WHERE old."id" == "sid")
BEGIN
  SELECT RAISE(ABORT, 'constraint failed');
END;
COMMIT;
