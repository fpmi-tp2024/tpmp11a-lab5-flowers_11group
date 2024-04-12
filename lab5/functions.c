#include "functions.h"

char* getDates()
{
    char* result = (char*)calloc(9, sizeof(char));
    char* year = (char*)malloc(5 * sizeof(char));
    char* month = (char*)malloc(3 * sizeof(char));
    char* day = (char*)malloc(3 * sizeof(char));
    printf("Enter the day, month and year\n");
    scanf("%s %s %s", day, month, year);
    strcat(result, year);
    strcat(result, month);
    strcat(result, day);

    return result;
}
int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");

    return 0;
}

void getInfoOnDate(sqlite3* db, sqlite3_stmt* res, char* err_msg, char* date)
{
    char* sql = "SELECT * FROM \"order\" WHERE \"accept_date\" = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {

        printf("Failed to select data\n");
        printf("SQL error: %s\n", err_msg);
        return;
    }

    sqlite3_bind_text(res, 1, date, -1, NULL);

    for (int j = 0; j < 4; j++)
        printf("|%s|", sqlite3_column_name(res, j));
    printf("\n");

    while (sqlite3_step(res) == SQLITE_ROW) {
        for (int j = 0; j < 4; j++)
            printf("|%s|", sqlite3_column_text(res, j));
        printf("\n");
    }
}

int increaseFlowerCost(sqlite3* db, sqlite3_stmt* res, char* err_msg, int flowerId, double costIncrease)
{
    sqlite3_stmt* new_res;
    char* sql[2] = { "SELECT composition_id, count FROM flowerInComposition WHERE flower_id = ?;", "SELECT cost FROM composition WHERE id = ?;" };
    int rc = sqlite3_prepare_v2(db, sql[0], -1, &res, 0);
    if (rc != SQLITE_OK) {

        printf("Failed to select data\n");
        printf("SQL error: %s\n", err_msg);
        return -1;
    }

    sqlite3_bind_int(res, 1, flowerId);
    
    while (sqlite3_step(res) == SQLITE_ROW)
    {
        int compositionId;
        int count;
        double costDifference = 0.0;
        for (int i = 0; i < 2; ++i)
        {
            
            if (i == 0)
                sscanf(sqlite3_column_text(res, i), "%d", &compositionId);
            else {
                sscanf(sqlite3_column_text(res, i), "%d", &count);
                costDifference = count * costIncrease;
            }
        }
            int rc = sqlite3_prepare_v2(db, sql[1], -1, &new_res, 0);
            if (rc != SQLITE_OK) {

                printf("Failed to select data\n");
                printf("SQL error: %s\n", err_msg);
                return -1;
            }

            sqlite3_bind_int(new_res, 1, compositionId);
            sqlite3_step(new_res);
            double cost;
            sscanf(sqlite3_column_text(new_res, 0), "%lf", &cost);

            if (costDifference > cost * 0.1)
            {
                return 0;
            }
        }
    return 1;
    }

void createOrder(sqlite3* db, sqlite3_stmt* res, char* err_msg, int client_id, char* date, int compositions_size, int* compositions, int* counts, int urgency)
{
    char* sql = "INSERT INTO \"order\"(client_id, accept_date, urgency_id) values(?, ?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {

        printf("Failed to select data\n");
        printf("SQL error: %s\n", err_msg);
        return;
    }
    sqlite3_bind_int(res, 1, client_id);
    sqlite3_bind_text(res, 2, date, -1, NULL);
    sqlite3_bind_int(res, 3, urgency);
    while (sqlite3_step(res) == SQLITE_ROW);

    sql = "SELECT MAX(id) FROM \"order\";";
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_step(res);
    int order_id;
    sscanf(sqlite3_column_text(res, 0), "%d", &order_id);
    while (sqlite3_step(res) == SQLITE_ROW);

    sql = "SELECT coef FROM orderUrgency WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_int(res, 1, urgency);
    sqlite3_step(res);
    double coef;
    sscanf(sqlite3_column_text(res, 0), "%lf", &coef);
    while (sqlite3_step(res) == SQLITE_ROW);

    double compos_cost;
    double orderCost = 0;
    sql = "SELECT cost FROM composition WHERE id = ?;";
    for (int i = 0; i < compositions_size; ++i)
    {
        sqlite3_prepare_v2(db, sql, -1, &res, 0);
        sqlite3_bind_int(res, 1, compositions[i]);
        sqlite3_step(res);
        sscanf(sqlite3_column_text(res, 0), "%lf", &compos_cost);
        orderCost += compos_cost * counts[i] * coef;
    }
    while (sqlite3_step(res) == SQLITE_ROW);
    sql = "INSERT INTO compositionInOrder(order_id, composition_id, count) values(?, ?, ?);";
    for (int i = 0; i < compositions_size; ++i)
    {
        sqlite3_prepare_v2(db, sql, -1, &res, 0);
        sqlite3_bind_int(res, 1, order_id);
        sqlite3_bind_int(res, 2, compositions[i]);
        sqlite3_bind_int(res, 3, counts[i]);
        while (sqlite3_step(res) == SQLITE_ROW);
    }
    
    sql = "CREATE TABLE IF NOT EXISTS cost( \
        order_id Integer NOT NULL, \
        cost Double NOT NULL, \
        CONSTRAINT unique_order_id UNIQUE(order_id) );";
    rc = sqlite3_exec(db, sql, 0, 0, err_msg);
    if (rc != SQLITE_OK) {

        printf("Failed to select data\n");
        printf("SQL error: %s\n", err_msg);
    }

    sql = "INSERT INTO cost(order_id, cost) values(?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
    {
        return;
    }
    sqlite3_bind_int(res, 1, order_id);
    sqlite3_bind_double(res, 2, orderCost);
    while (sqlite3_step(res) == SQLITE_ROW);

    sql = "SELECT * FROM cost WHERE order_id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &res, 0);
    sqlite3_bind_int(res, 1, order_id);
    sqlite3_step(res);
    printf("Result of the order creation:\n");
    for (int i = 0; i < 2; ++i)
    {
        printf("|%s|", sqlite3_column_text(res, i));
    }
    
    
}
