#include "functions.h"
#include "info.h"
int main()
{
    sqlite3* db;
    char* err_msg = 0;
    sqlite3_stmt* res = 0;

    setlocale(LC_ALL, "Rus");
    int rc = sqlite3_open("flowershop.db", &db);

    if (rc != SQLITE_OK) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    else {
        printf("Opened database successfully\n");
    }

    int* compositions[2] = { 3, 4 };
    int* counts[2] = { 1,2 };
    while (isAllRight) {
        printf("Choose action:\n\t0) Show all data\n\t1) SELECT\n\t2) INSERT, UPDATE and DELETE\n\t3) Function 1\n\t4) Function 2\n\t5) Function 3\n\t6) EXIT\n");
        int action_id;
        scanf("%d", &action_id);
        if (action_id < 0 || action_id > 7) {
            printf("Wrong action_id\n");
            continue;
        }

        if (action_id == 6) {
            printf("End of programm\n");
            break;
        }

        switch (action_id) {
        case 0: {
            printf("Choose a table to show all data\n");
            for (int i = 0; i < 7; ++i)
            {
                printf("%d) %s\t", i, table_names[i]);
            }
            printf("\n");
            int table;
            scanf("%d", &table);
            printf("Table %s\n", table_names[table]);
            rc = sqlite3_exec(db, sql[table], callback, 0, &err_msg);

            if (rc != SQLITE_OK) {

                printf("Failed to select data\n");
                printf("SQL error: %s\n", err_msg);

                isAllRight = 0;
            }
            break;
        }
        case 1: {
            printf("Enter range of accept date:\n");
            char* accept_date_start = getDates();
            char* accept_date_finish = getDates();
            printf("Enter urgency id (from 1 to 3)\n");
            int urgency;
            scanf("%d", &urgency);
            if (urgency < 1 || urgency > 3)
            {
                break;
            }

            for (int i = 0; i < 5; ++i)
            {
                printf("Select statement: %s\n", select[i]);
                rc = sqlite3_prepare_v2(db, select[i], -1, &res, 0);
                if (rc != SQLITE_OK) {
                    printf("Failed to execute statement: %s\n", sqlite3_errmsg(db));
                    isAllRight = 0;
                    break;
                }
                if (i == 0 || i == 3)
                {
                    sqlite3_bind_text(res, 1, accept_date_start, -1, NULL);
                    sqlite3_bind_text(res, 2, accept_date_finish, -1, NULL);
                }
                if (i == 2)
                {
                    sqlite3_bind_int(res, 1, urgency);
                }

                for (int j = 0; j < select_col_number[i]; j++)
                    printf("|%s|", sqlite3_column_name(res, j));
                printf("\n");

                while (sqlite3_step(res) == SQLITE_ROW) {
                    for (int j = 0; j < select_col_number[i]; j++)
                        printf("|%s|", sqlite3_column_text(res, j));
                    printf("\n");
                    }
                printf("\n");
            }
            break;
        }
        case 2: {
            for (int i = 0; i < 6; ++i)
            {
                printf("Table before insert: \n");
                rc = sqlite3_exec(db, sql[i], callback, 0, &err_msg);

                if (rc != SQLITE_OK) {

                    printf("Failed to select data\n");
                    printf("SQL error: %s\n", err_msg);

                    isAllRight = 0;
                    break;
                }

                rc = sqlite3_exec(db, insert[i], 0, 0, &err_msg);
                if (rc != SQLITE_OK) {
                    printf("Failed to execute statement: %s\n", sqlite3_errmsg(db));
                    isAllRight = 0;
                    break;
                }

                printf("Table after insert: \n");
                rc = sqlite3_exec(db, sql[i], callback, 0, &err_msg);

                if (rc != SQLITE_OK) {

                    printf("Failed to select data\n");
                    printf("SQL error: %s\n", err_msg);

                    isAllRight = 0;
                    break;
                }

                rc = sqlite3_exec(db, update[i], 0, 0, &err_msg);
                printf("Table after update: \n");
                rc = sqlite3_exec(db, sql[i], callback, 0, &err_msg);

                if (rc != SQLITE_OK) {

                    printf("Failed to select data\n");
                    printf("SQL error: %s\n", err_msg);

                    isAllRight = 0;
                }

                rc = sqlite3_exec(db, delete_query[i], 0, 0, &err_msg);
                printf("Table after delete: \n");
                rc = sqlite3_exec(db, sql[i], callback, 0, &err_msg);

                if (rc != SQLITE_OK) {

                    printf("Failed to select data\n");
                    printf("SQL error: %s\n", err_msg);

                    isAllRight = 0;
                }
            }
            break;
        }
        case 3:
        {
            printf("Function that returns true if flower cost can be increased and false if not\n");
            printf("Select flower id\n");
            sqlite3_exec(db, "SELECT * FROM flower;", callback, 0, &err_msg);
            int flowerId;
            scanf("%d", &flowerId);
            printf("Enter how much you would like to increase flower cost\n");
            double costIncrease;
            scanf("%lf", &costIncrease);
            printf(increaseFlowerCost(db, res, err_msg, flowerId, costIncrease) ? "TRUE\n" : "FALSE\n");
            break;
        }
        case 4:
        {
            printf("Function that adds new order and calculates its cost\n");
            printf("Enter client id from the existant\n");
            sqlite3_exec(db, "SELECT * FROM client;", callback, 0, &err_msg);
            int clientId;
            scanf("%d", &clientId);
            char* date = getDates();
            printf("Enter how many diffent types of compositions do you want in your order\n");
            int count;
            scanf("%d", &count);
            int* compositions = calloc(count, sizeof(int));
            int* counts = calloc(count, sizeof(int));
            for (int i = 0; i < count; ++i)
            {
                printf("Enter composition id and count of this composition\n");
                sqlite3_exec(db, "SELECT * FROM composition;", callback, 0, &err_msg);
                scanf("%d %d", &compositions[i], &counts[i]);
            }

            printf("Enter urgency id\n");
            sqlite3_exec(db, "SELECT * FROM orderUrgency;", callback, 0, &err_msg);
            int urgencyId;
            scanf("%d", &urgencyId);
            createOrder(db, res, err_msg, clientId, date, count, compositions, counts, urgencyId);
            break;
        }
        case 5:
        {
            printf("Enter date:\n");
            char* date = getDates();
            getInfoOnDate(db, res, err_msg, date);
            break;
        }
        default:
            printf("Something is wrong\n");
            isAllRight = 0;
            break;
        }
    }
    sqlite3_finalize(res);
    sqlite3_close(db);

    return 0;
}


