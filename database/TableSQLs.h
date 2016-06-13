//
// Created by paulina on 09.06.16.
//

#ifndef TIN_SERVER_TABLESQLS_H
#define TIN_SERVER_TABLESQLS_H

#include <string>

static const std::string DROP_TABLES = "drop table if exists tokens; drop table if exists notifications; "
        "drop table if exists devices; drop table if exists users; ";
static const std::string INIT_DATABASE_QUERY = "create table users (id serial primary key not null,login character varying(128), "
        "hashpassword character varying(128) ); create table notifications (id serial not null primary key, "
        "text character varying(1024), created date, userid integer references users, readid integer ); "
        "create table devices ( id serial not null primary key, lastlogindate date, userid integer references users,"
        "lastqueryid integer, lastreadid integer, lastquerytmp integer, lastreadtmp integer); CREATE TABLE tokens "
        "( id bigint NOT NULL, expiry_date timestamp without time zone, token character varying(255), user_id bigint NOT NULL, CONSTRAINT tokens_pkey PRIMARY KEY (id), "
        "CONSTRAINT fk_lgokc3vw1rct83pdwryntacb9 FOREIGN KEY (user_id) REFERENCES users (id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE NO ACTION )";

#endif //TIN_SERVER_TABLESQLS_H
