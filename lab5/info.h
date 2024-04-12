#pragma once
const char* select[5] = {"SELECT SUM(\"composition\".\"cost\" * \"compositionInOrder\".\"count\" * \"orderUrgency\".\"coef\") AS sum FROM  \"order\" \
INNER JOIN \"orderUrgency\"  ON \"order\".\"urgency_id\" = \"orderUrgency\".\"id\" \
INNER JOIN \"compositionInOrder\"  ON \"order\".\"id\" = \"compositionInOrder\".\"order_id\" \
INNER JOIN \"composition\"  ON \"composition\".\"id\" = \"compositionInOrder\".\"composition_id\" \
WHERE (\"order\".\"accept_date\" > ?) AND (\"order\".\"accept_date\" < ?);", 
"SELECT   \"composition\".\"id\", \
         \"composition\".\"title\", \
         \"composition\".\"cost\", \
         SUM(\"compositionInOrder\".\"count\") AS sum \
FROM     \"composition\" \
INNER JOIN \"compositionInOrder\"  ON \"composition\".\"id\" = \"compositionInOrder\".\"composition_id\" \
GROUP BY \"composition\".\"id\" \
ORDER BY \"compositionInOrder\".\"count\" DESC \
LIMIT 1",
"SELECT   COUNT(\"order\".\"id\") AS count \
FROM     \"order\" \
WHERE(\"order\".\"urgency_id\" = ?)",
"SELECT   \"flower\".\"name\", \
         \"flower\".\"sort\", \
         SUM(\"flowerInComposition\".\"count\") AS sum \
FROM     \"flower\" \
INNER JOIN \"flowerInComposition\"  ON \"flower\".\"id\" = \"flowerInComposition\".\"flower_id\" \
INNER JOIN \"compositionInOrder\"  ON \"flowerInComposition\".\"composition_id\" = \"compositionInOrder\".\"composition_id\" \
INNER JOIN \"order\"  ON \"order\".\"id\" = \"compositionInOrder\".\"order_id\" \
WHERE (\"order\".\"accept_date\" > ?) AND (\"order\".\"accept_date\" < ?) \
GROUP BY \"flower\".\"id\";",
"SELECT  \"composition\".\"title\", \
         \"compositionInOrder\".\"count\", \
         \"composition\".\"cost\"*\"compositionInOrder\".\"count\" AS total_cost \
FROM     \"composition\" \
INNER JOIN \"compositionInOrder\"  ON \"composition\".\"id\" = \"compositionInOrder\".\"composition_id\" \
INNER JOIN \"order\"  ON \"order\".\"id\" = \"compositionInOrder\".\"order_id\" \
GROUP BY \"composition\".\"id\" " };

    const char* insert[6] = {
        "insert into client(name, login, password) values(\"Galya Aleksanrovna\", \"galina\", \"1112\");",
        "insert into composition(title, cost) values(\"Сюрприз\", \"110\");",
        "insert into compositionInOrder(order_id, composition_id, count) values(\"4\", \"2\", \"3\");",
        "insert into flower(name, sort, cost) values(\"роза\", \"крсная\", \"7.0\");",
        "insert into flowerInComposition(flower_id, composition_id, count) values(\"1\", \"2\", \"3\");",
        "insert into \"order\"(client_id, accept_date, urgency_id) values(\"4\", \"20240212\", \"2\");"
    };

    const char* delete_query[6] =
        {"delete from client where name = \"Galya Aleksanrovna\";", "delete from composition where title like \"W%\";",
        "delete from compositionInOrder where order_id = \"4\";", "delete from flower where name like \"d%\";",
        "delete from flowerInComposition where flower_id = \"1\";", "delete from \"order\" where accept_date = \"20210212\";" };

    const char* update[6] =
    {
        "update client set login = \"Galina Aleksanrovna\" where name = \"Galya Aleksanrovna\";", "update composition set cost = \"120\" where title like \"W%\";",
        "update compositionInOrder set count = \"5\" where order_id = \"5\";", "update flower set sort = \"white\" where name like \"d%\";",
        "update flowerInComposition set count = \"5\" where flower_id = \"1\";", "update \"order\" set urgency_id = \"1\" where accept_date = \"20240212\";"
    };

    const char* col_names[7][4] = {
        {"id", "name", "login", "password"},
        {"id", "title", "cost"},
        {"id", "order_id", "composition_id", "count"},
        {"id", "name", "sort", "cost"},
        {"id", "flower_id", "composition_id", "count"},
        {"id", "client_id", "accept_date", "urgency_id"},
        {"id", "type", "coef", "days"}
    };

    const char* sql[7] = { "SELECT * FROM client;", "SELECT * FROM composition;", "SELECT * FROM compositionInOrder;", "SELECT * FROM flower;", "SELECT * FROM flowerInComposition;", "SELECT * FROM \"order\";", "SELECT * FROM orderUrgency;" };

    int col_number[7] = { 4, 3, 4, 4, 4, 4, 4 };
    int select_col_number[5] = { 1, 4, 1, 3, 3 };
    const char* table_names[7] = { "client", "composition", "compositionInOrder", "flower", "flowerInComposition", "order", "orderUrgency" };
    int isAllRight = 1;
