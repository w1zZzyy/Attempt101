CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

DROP SCHEMA IF EXISTS chess CASCADE;

CREATE SCHEMA IF NOT EXISTS chess;

CREATE TABLE IF NOT EXISTS chess.users (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL,
    status TEXT NOT NULL -- online, offline, searching, playing
);

CREATE TABLE IF NOT EXISTS chess.match (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    status TEXT NOT NULL, -- searching, in_progress, finished, canceled
    result TEXT,
    white_id BIGINT,
    black_id BIGINT,
    foreign key (white_id) references chess.users(id),
    foreign key (black_id) references chess.users(id)
);

CREATE TABLE IF NOT EXISTS chess.match_token (
    id TEXT PRIMARY KEY DEFAULT uuid_generate_v4(), --- token
    match_id BIGINT UNIQUE NOT NULL,
    foreign key(match_id) REFERENCES chess.match(id)
);
