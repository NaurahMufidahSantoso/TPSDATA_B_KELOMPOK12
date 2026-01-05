#ifndef KTP_MANAGER_H
#define KTP_MANAGER_H

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "ktp.h"

using namespace std;

class KTPManager {
public:
    static bool loginByKTP(const string& nik, KTP& hasil) {
        ifstream file("data_ktp.txt");
        string line;

        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string temp;
            KTP k;

            getline(ss, k.nik, '|');
            getline(ss, k.namaPemilik, '|');
            getline(ss, k.alamat);

            if (k.nik == nik) {
                hasil = k;
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }
};

#endif
