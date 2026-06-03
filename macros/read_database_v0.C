#include <sqlite3.h>

#include <iostream>
#include <string>
#include <vector>

struct EtauCal {
    int iov;
    double etau_E;
    double etau_W;
};

void read_database_v0() {
    std::string sbnd_data_v = "v01_41_00";

    std::string db_file =
        "/cvmfs/sbnd.opensciencegrid.org/products/sbnd/sbnd_data/"
        + sbnd_data_v
        + "/CalibrationDatabase/tpc_elifetime.db";

    std::string table = "tpc_elifetime_data";

    sqlite3* db = nullptr;

    if (sqlite3_open(db_file.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: "
                  << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string query = "SELECT * FROM " + table;

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare query: "
                  << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }


    int ncols = sqlite3_column_count(stmt);

    std::cout << "Columns:\n";
    for (int i = 0; i < ncols; ++i) {
        std::cout << i << ": "
              << sqlite3_column_name(stmt, i)
              << '\n';
    }

    std::cout << "\nRows:\n";



    std::vector<EtauCal> etau_data;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        EtauCal row;

        row.iov    = sqlite3_column_int(stmt, 0);
        row.etau_E = sqlite3_column_double(stmt, 5);
        row.etau_W = sqlite3_column_double(stmt, 8);

        etau_data.push_back(row);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    for (const auto& row : etau_data) {
        std::cout
            << "iov = "    << row.iov
            << ", etau_E = " << row.etau_E
            << ", etau_W = " << row.etau_W
            << '\n';
    }

}
