#ifndef HARGA_TINDAKAN_H
#define HARGA_TINDAKAN_H

#include <string>

using namespace std;

class HargaTindakan {
public:
    static int getHarga(const string& tindakan) {
        if (tindakan == "Pemeriksaan") return 50000;
        if (tindakan == "Vaksinasi")   return 75000;
        if (tindakan == "Operasi")     return 300000;
        if (tindakan == "RawatInap")   return 100000;
        if (tindakan == "Perawatan")   return 60000;
        return 50000;
    }
};

#endif
