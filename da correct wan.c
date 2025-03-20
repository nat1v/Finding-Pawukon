#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Definisi nama-nama dalam kalender Bali
const char *sapta_wara[] = {"Redite", "Soma", "Anggara", "Buda", "Wraspati", "Sukra", "Saniscara"};
const char *panca_wara[] = {"Paing", "Pon", "Wage", "Kliwon", "Umanis"};
const char *wuku[] = {"Sinta", "Landep", "Ukir", "Kulantir", "Taulu", "Gumbreg", "Wariga", 
                      "Warigadian", "Julungwangi", "Sungsang", "Dungulan", "Kuningan", 
                      "Langkir", "Medangsia", "Pujut", "Pahang", "Krulut", "Merakih", 
                      "Tambir", "Medangkungan", "Matal", "Uye", "Menail", "Prangbakat", 
                      "Bala", "Ugu", "Wayang", "Klau", "Dukut", "Watugunung"};

// Constants for the fixed reference date: 11 February 1979 (Redite Paing Sinta)
const int REFERENCE_YEAR = 1979;
const int REFERENCE_MONTH = 2;
const int REFERENCE_DAY = 11;
const int REFERENCE_SAPTA = 0;  // Redite is index 0 in sapta_wara
const int REFERENCE_PANCA = 0;  // Paing is index 0 in panca_wara
const int REFERENCE_WUKU = 0;   // Sinta is index 0 in wuku

// Fungsi untuk menghitung jumlah hari antara dua tanggal
int hitungHari(int tahun1, int bulan1, int tanggal1, int tahun2, int bulan2, int tanggal2) {
    // Mengkonversi tanggal ke jumlah hari sejak 1 Januari tahun 0
    // Modified to use January as start (month 1) instead of March
    
    // Calculate days for first date
    int hari1 = tahun1 * 365 + tahun1 / 4 - tahun1 / 100 + tahun1 / 400;
    
    // Add days for months that have passed
    for (int m = 1; m < bulan1; m++) {
        if (m == 2) {
            // February
            if ((tahun1 % 4 == 0 && tahun1 % 100 != 0) || (tahun1 % 400 == 0)) {
                hari1 += 29;  // Leap year
            } else {
                hari1 += 28;  // Not leap year
            }
        } else if (m == 4 || m == 6 || m == 9 || m == 11) {
            hari1 += 30;
        } else {
            hari1 += 31;
        }
    }
    
    // Add days of current month
    hari1 += tanggal1;
    
    // Calculate days for second date
    int hari2 = tahun2 * 365 + tahun2 / 4 - tahun2 / 100 + tahun2 / 400;
    
    // Add days for months that have passed
    for (int m = 1; m < bulan2; m++) {
        if (m == 2) {
            // February
            if ((tahun2 % 4 == 0 && tahun2 % 100 != 0) || (tahun2 % 400 == 0)) {
                hari2 += 29;  // Leap year
            } else {
                hari2 += 28;  // Not leap year
            }
        } else if (m == 4 || m == 6 || m == 9 || m == 11) {
            hari2 += 30;
        } else {
            hari2 += 31;
        }
    }
    
    // Add days of current month
    hari2 += tanggal2;
    
    return hari2 - hari1;
}

// Fungsi untuk mencari kombinasi kalender Bali pada tahun yang diinginkan
void cariKombinasi(int tahunTarget, const char *saptaWara, const char *pancaWara, const char *wukuInput) {
    // Compare string dengan semua list Pancawara / Saptawara / Wuku (CASE SENSITIVE)
    int targetSapta = -1, targetPanca = -1, targetWuku = -1;
    
    for (int i = 0; i < 7; i++) {
        if (strcmp(saptaWara, sapta_wara[i]) == 0) {
            targetSapta = i;
            break;
        }
    }
    
    for (int i = 0; i < 5; i++) {
        if (strcmp(pancaWara, panca_wara[i]) == 0) {
            targetPanca = i;
            break;
        }
    }
    
    for (int i = 0; i < 30; i++) {
        if (strcmp(wukuInput, wuku[i]) == 0) {
            targetWuku = i;
            break;
        }
    }
    
    if (targetSapta == -1 || targetPanca == -1 || targetWuku == -1) {
        printf("Error: Nama sapta wara, panca wara, atau wuku tidak valid.\n");
        return;
    }
    
    // Cari semua tanggal pada tahun target
    bool ditemukan = false;
    
    for (int bulan = 1; bulan <= 12; bulan++) {
        int akhirBulan;
        
        // Menentukan jumlah hari dalam bulan
        if (bulan == 2) {
            // Februari
            if ((tahunTarget % 4 == 0 && tahunTarget % 100 != 0) || (tahunTarget % 400 == 0)) {
                akhirBulan = 29;  // Tahun kabisat
            } else {
                akhirBulan = 28;  // Bukan tahun kabisat
            }
        } else if (bulan == 4 || bulan == 6 || bulan == 9 || bulan == 11) {
            akhirBulan = 30;
        } else {
            akhirBulan = 31;
        }
        
        for (int tanggal = 1; tanggal <= akhirBulan; tanggal++) {
            // Hitung jumlah hari sejak referensi
            int selisihHari = hitungHari(REFERENCE_YEAR, REFERENCE_MONTH, REFERENCE_DAY, 
                                        tahunTarget, bulan, tanggal);
            if (selisihHari < 0){
                selisihHari = -selisihHari;
            }
            
            // Hitung sapta wara, panca wara, dan wuku untuk tanggal ini
            // Modified to handle negative day differences correctly
            /*
            int sapta = (REFERENCE_SAPTA + selisihHari % 7 + 7) % 7;
            int panca = (REFERENCE_PANCA + selisihHari % 5 + 5) % 5;
            
            // For wuku, we need to handle negative day differences specially
            int wukuDiff = selisihHari / 7;
            int wukuIndeks;
            if (selisihHari < 0) {
                // For negative differences, we need to adjust the calculation
                wukuDiff = (selisihHari - 6) / 7; // Integer division rounds toward zero
                wukuIndeks = (REFERENCE_WUKU + wukuDiff % 30 + 30) % 30;
            } else {
                wukuIndeks = (REFERENCE_WUKU + wukuDiff % 30) % 30;
            }*/
            
            int sapta = (REFERENCE_SAPTA + selisihHari % 7);// % 7;
            int panca = (REFERENCE_PANCA + selisihHari % 5);// % 5;
            
            // For wuku, we need to handle negative day differences specially
            int wukuDiff = selisihHari / 7;
            int wukuIndeks;
                wukuIndeks = (REFERENCE_WUKU + wukuDiff % 30);// % 30;
            
            
            
            // Jika kombinasi sesuai dengan yang dicari
            if (sapta == targetSapta && panca == targetPanca && wukuIndeks == targetWuku) {
                printf("%d-%02d-%02d: %s %s %s\n", 
                       tahunTarget, bulan, tanggal, 
                       sapta_wara[sapta], panca_wara[panca], wuku[wukuIndeks]);
                ditemukan = true;
            }
        }
    }
    
    if (!ditemukan) {
        printf("Tidak ditemukan kombinasi %s %s %s pada tahun %d.\n", 
               saptaWara, pancaWara, wukuInput, tahunTarget);
    }
}

int main() {
    char saptaWara[20], pancaWara[20], wukuInput[20];
    int tahun;
    
    // Display reference information
    printf("Referensi: 11 Februari 1979 adalah Redite Paing Sinta\n\n");
    
    // Input informasi yang ingin dicari
    printf("Masukkan Pawukon Yang Ingin Dicari:\n");
    printf("Masukkan Sapta Wara yang dicari: ");
    scanf("%s", saptaWara);
    
    printf("Masukkan Panca Wara yang dicari: ");
    scanf("%s", pancaWara);
    
    printf("Masukkan Wuku yang dicari: ");
    scanf("%s", wukuInput);
    
    printf("Masukkan Tahun yang ingin dicari: ");
    scanf("%d", &tahun);
    
    printf("\nHasil Pencarian:\n");
    cariKombinasi(tahun, saptaWara, pancaWara, wukuInput);
    
    return 0;
}
