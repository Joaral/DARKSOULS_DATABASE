-- Active: 1742327066487@@localhost@5432@dark_souls_db
CREATE TABLE bosses (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    location VARCHAR(100),
    health INT,
    souls INT,
    important_items TEXT,
    boss_status STATUS
);