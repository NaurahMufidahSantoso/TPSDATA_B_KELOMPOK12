#ifndef KTP_H
#define KTP_H

#include <string>
#include <iostream>

using namespace std;

struct KTP {
    string nik;
    string namaPemilik;
    string alamat;

    KTP() {
        nik = "";
        namaPemilik = "";
        alamat = "";
    }

    bool valid() const {
        return nik.length() == 16;
    }

    void tampil() const {
        cout << "NIK          : " << nik << endl;
        cout << "Nama Pemilik : " << namaPemilik << endl;
        cout << "Alamat       : " << alamat << endl;
    }
};

#endif
