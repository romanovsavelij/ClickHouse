-- Tags: distributed

set query_plan_remove_redundant_distinct = 1;
set optimize_duplicate_order_by_and_distinct = 0;
SET distributed_group_by_no_merge = 0;

SELECT DISTINCT number
FROM
(
    SELECT DISTINCT number
    FROM remote('127.0.0.{1,2}', system.numbers)
    LIMIT 1
    SETTINGS distributed_group_by_no_merge = 1
);

SET distributed_group_by_no_merge = 1;

SELECT DISTINCT number
FROM
(
    SELECT DISTINCT number
    FROM remote('127.0.0.{1,2}', system.numbers)
    LIMIT 1
);

set optimize_duplicate_order_by_and_distinct = 0;
SET distributed_group_by_no_merge = 0;

SELECT DISTINCT number
FROM
(
    SELECT DISTINCT number
    FROM remote('127.0.0.{1,2}', system.numbers)
    LIMIT 1
    SETTINGS distributed_group_by_no_merge = 1
);

SET distributed_group_by_no_merge = 1;
set optimize_duplicate_order_by_and_distinct = 0;
SELECT DISTINCT number
FROM
(
    SELECT DISTINCT number
    FROM remote('127.0.0.{1,2}', system.numbers)
    LIMIT 1
);
