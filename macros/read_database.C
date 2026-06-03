#include <sqlite3.h>

#include <iostream>
#include <string>
#include <vector>

struct EtauCal {
    int iov;
    int itpc;
    double etau;
};

void read_database() {


    std::string sbnd_data_v = "v01_41_00";
    std::string SBND_etau_cal_f = "/cvmfs/sbnd.opensciencegrid.org/products/sbnd/sbnd_data/" + sbnd_data_v + "/CalibrationDatabase/tpc_elifetime.db";

    std::string db_file = SBND_etau_cal_f;   // your database file
    std::string table   = "tpc_elifetime_data";  // your table name

    sqlite3* db = nullptr;

    int rc = sqlite3_open(db_file.c_str(), &db);
    if (rc) {
        std::cerr << "Cannot open database: "
                  << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string query = "SELECT * FROM " + table;

    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(
        db,
        query.c_str(),
        -1,
        &stmt,
        nullptr
    );

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare query: "
                  << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    std::vector<EtauCal> records;

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {

        EtauCal row;

        row.iov   = sqlite3_column_int(stmt, 0);
        row.itpc  = sqlite3_column_int(stmt, 1);
        row.etau  = sqlite3_column_double(stmt, 2);

        records.push_back(row);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);


    for (const auto& row : records) {
        std::cout
            << "iov="  << row.iov
            << " itpc=" << row.itpc
            << " etau=" << row.etau
            << '\n';
    }

}
