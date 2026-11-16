
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_KITAPLAR 500
#define MAX_KULLANICI 100
#define MAX_ISIM 50
#define MAX_BASLIK 100
#define MAX_KULLANICI_AD 30
#define MAX_SIFRE 30
#define MAX_KATEGORI 30
#define KITAPLAR_DOSYA "books.dat"
#define KULLANICILAR_DOSYA "users.dat"
#define ISLEMLER_DOSYA "transactions.dat"

typedef struct {
    int id;
    char baslik[MAX_BASLIK];
    char yazar[MAX_ISIM];
    char kategori[MAX_KATEGORI];
    bool odunctemi;
    int oduncalan; // user id who borrowed the book
    char odunctarihi[11]; 
} Kitap;

typedef struct {
    int id;
    char kullaniciadi[MAX_KULLANICI_AD];
    char sifre[MAX_SIFRE];
    char ad[MAX_ISIM];
    char soyad[MAX_ISIM];
    bool adminmi;
    int oduncte_kitap[3]; 
    int oduncsayaci;
} Kullanici;

typedef struct {
    int kitap_id;
    int kullanici_id;
    char odunctarihi[11];
    char iadetarihi[11]; 
} Islem;

void verileriyukle();
void kitaplarikaydet();
void kullanicilarikaydet();
void islemlerikaydet();

void anasayfa();
void kullanici_menu();
void admin_menu();

void kayit();
bool giris();
void cikis();

void kitapekle();
void kitaplarigoruntule();
void kitapara();
void basliktanara();
void yazardanara();
void kategoridenara();
void kitapoduncal();
void kitapiade();
void odunclerim();

void kullanicigoruntule();
void islemlerigoruntule();

int id_kitapbul(int id);
int id_kullanicibul(int id);
int kullaniciadi_kullanicibul(char kullaniciadi[]);
void girdiyitemizle();
void kitapyazdir(Kitap kitap);
void kullaniciyazdir(Kullanici kullanici);
void islemleriyazdir(Islem islem);
bool tarihkontrol(char tarih[]);
void kucukharfe(char *str);

Kitap kitaplar[MAX_KITAPLAR];
Kullanici kullanicilar[MAX_KULLANICI];
Islem islemler[MAX_KITAPLAR * 2]; // Assuming each book can be borrowed twice

int kitapsayaci = 0;
int kullanicisayaci = 0;
int islemsayaci = 0;
Kullanici aktifkullanici;

int main() {
    verileriyukle();
    anasayfa();
    return 0;
}

void verileriyukle() {
    FILE *file;
    
    file = fopen(KITAPLAR_DOSYA, "rb");
    if (file != NULL) {
        kitapsayaci = fread(kitaplar, sizeof(Kitap), MAX_KITAPLAR, file);
        fclose(file);
    }
    
    file = fopen(KULLANICILAR_DOSYA, "rb");
    if (file != NULL) {
        kullanicisayaci = fread(kullanicilar, sizeof(Kullanici), MAX_KULLANICI, file);
        fclose(file);
    }
    
    file = fopen(ISLEMLER_DOSYA, "rb");
    if (file != NULL) {
        islemsayaci = fread(islemler, sizeof(Islem), MAX_KITAPLAR * 2, file);
        fclose(file);
    }

    // Varsayilan admin hesabi (kullanici yoksa olusturulur)
    if (kullanicisayaci == 0) {
        Kullanici admin;
        memset(&admin, 0, sizeof(admin));
        admin.id = 1;
        strcpy(admin.kullaniciadi, "admin");
        strcpy(admin.sifre, "admin123");
        strcpy(admin.ad, "Sistem");
        strcpy(admin.soyad, "Yonetici");
        admin.adminmi = true;
        for (int i = 0; i < 3; i++) admin.oduncte_kitap[i] = -1;
        kullanicilar[kullanicisayaci++] = admin;
        kullanicilarikaydet();
    }
}

void kitaplarikaydet() {
    FILE *file = fopen(KITAPLAR_DOSYA, "wb");
    if (file != NULL) {
        fwrite(kitaplar, sizeof(Kitap), kitapsayaci, file);
        fclose(file);
    }
}

void kullanicilarikaydet() {
    FILE *file = fopen(KULLANICILAR_DOSYA, "wb");
    if (file != NULL) {
        fwrite(kullanicilar, sizeof(Kullanici), kullanicisayaci, file);
        fclose(file);
    }
}

void islemlerikaydet() {
    FILE *file = fopen(ISLEMLER_DOSYA, "wb");
    if (file != NULL) {
        fwrite(islemler, sizeof(Islem), islemsayaci, file);
        fclose(file);
    }
}
    
void kucukharfe(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int id_kitapbul(int id) {
    for (int i = 0; i < kitapsayaci; i++) {
        if (kitaplar[i].id == id) {
            return i;
        }
    }
    return -1;
}

int id_kullanicibul(int id) {
    for (int i = 0; i < kullanicisayaci; i++) {
        if (kullanicilar[i].id == id) {
            return i;
        }
    }
    return -1;
}

int kullaniciadi_kullanicibul(char kullaniciadi[]) {
    for (int i = 0; i < kullanicisayaci; i++) {
        if (strcmp(kullanicilar[i].kullaniciadi, kullaniciadi) == 0) {
            return i;
        }
    }
    return -1;
}

void girdiyitemizle() {
    while (getchar() != '\n');
}

void kitapyazdir(Kitap kitap) {
    printf("\nKitap ID: %d\n", kitap.id);
    printf("Baslik (Kitap ismi): %s\n", kitap.baslik);
    printf("Yazar: %s\n", kitap.yazar);
    printf("Kategori: %s\n", kitap.kategori);
    printf("Durum: %s\n", kitap.odunctemi ? "Odunc Alindi" : "Rafta");
    if (kitap.odunctemi) {
        int kullanici_index = id_kullanicibul(kitap.oduncalan);
        if (kullanici_index != -1) {
            printf("Odunc Alan: %s %s\n", kullanicilar[kullanici_index].ad, kullanicilar[kullanici_index].soyad);
        }
        printf("Odunc Alma Tarihi: %s\n", kitap.odunctarihi);
    }
}

void kullaniciyazdir(Kullanici kullanici) {
    printf("\nKullanici ID: %d\n", kullanici.id);
    printf("Kullanici Adi: %s\n", kullanici.kullaniciadi);
    printf("Ad: %s\n", kullanici.ad);
    printf("Soyad: %s\n", kullanici.soyad);
    printf("Rol: %s\n", kullanici.adminmi ? "Yonetici" : "Kullanici");
    printf("Odunc Alinan Kitap Sayisi: %d\n", kullanici.oduncsayaci);
    if (kullanici.oduncsayaci > 0) {
        printf("Odunc Alinan Kitaplar:\n");
        for (int i = 0; i < kullanici.oduncsayaci; i++) {
            int kitap_index = id_kitapbul(kullanici.oduncte_kitap[i]);
            if (kitap_index != -1) {
                printf("- %s (ID: %d)\n", kitaplar[kitap_index].baslik, kullanici.oduncte_kitap[i]);
            }
        }
    }
}

void islemleriyazdir(Islem islem) {
    printf("\nİslem Bilgileri:\n");
    int kitap_index = id_kitapbul(islem.kitap_id);
    int kullanici_index = id_kullanicibul(islem.kullanici_id);
    
    if (kitap_index != -1) {
        printf("Kitap: %s (ID: %d)\n", kitaplar[kitap_index].baslik, islem.kitap_id);
    } else {
        printf("Kitap ID: %d (silinmis)\n", islem.kitap_id);
    }
    
    if (kullanici_index != -1) {
        printf("Kullanici: %s %s (ID: %d)\n", kullanicilar[kullanici_index].ad, kullanicilar[kullanici_index].soyad, islem.kullanici_id);
    } else {
        printf("Kullanici ID: %d (silinmis)\n", islem.kullanici_id);
    }
    
    printf("odunc Alma Tarihi: %s\n", islem.odunctarihi);
    if (strlen(islem.iadetarihi) > 0) {
        printf("İade Tarihi: %s\n", islem.iadetarihi);
    } else {
        printf("Durum: Henuz iade edilmedi\n");
    }
}

bool tarihkontrol(char tarih[]) {
    if (strlen(tarih) != 10) return false;
    if (tarih[4] != '.' || tarih[7] != '.') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(tarih[i])) return false;
    }
    
    int yil, ay, gun;
    sscanf(tarih, "%d.%d.%d", &yil, &ay, &gun);
    
    if (yil < 1900 || yil > 2100) return false;
    if (ay < 1 || ay > 12) return false;
    if (gun < 1 || gun > 31) return false;
    
    return true;
}

void kayit() {
    if (kullanicisayaci >= MAX_KULLANICI) {
        printf("Maksimum kullanici sayisina ulasildi. Yeni kullanici eklenemiyor.\n");
        return;
    }
    
    Kullanici yenikullanici;
    char sifreonay[MAX_SIFRE];
    
    printf("\n\t Yeni Kullanici Kaydi \n");
    printf("Kullanici Adi: ");
    scanf("%29s", yenikullanici.kullaniciadi);
    girdiyitemizle();
    
    if (kullaniciadi_kullanicibul(yenikullanici.kullaniciadi) != -1) {
        printf("Bu kullanici adi zaten alinmis.\n");
        return;
    }
    
    printf("sifre: ");
    scanf("%29s", yenikullanici.sifre);
    girdiyitemizle();
    
    printf("sifre (Tekrar): ");
    scanf("%29s", sifreonay);
    girdiyitemizle();
    
    if (strcmp(yenikullanici.sifre, sifreonay) != 0) {
        printf("sifreler uyusmuyor.\n");
        return;
    }
    
    printf("Ad: ");
    scanf("%49s", yenikullanici.ad);
    girdiyitemizle();
    
    printf("Soyad: ");
    scanf("%49s", yenikullanici.soyad);
    girdiyitemizle();
    
    yenikullanici.id = kullanicisayaci > 0 ? kullanicilar[kullanicisayaci-1].id + 1 : 1;
    yenikullanici.adminmi = false;
    yenikullanici.oduncsayaci = 0;
    for (int i = 0; i < 3; i++) {
        yenikullanici.oduncte_kitap[i] = -1;
    }
    
    kullanicilar[kullanicisayaci++] = yenikullanici;
    kullanicilarikaydet();
    
    printf("\nKayit basarili. Kullanici ID'niz: %d\n", yenikullanici.id);
}

bool giris() {
    char kullaniciadi[MAX_KULLANICI_AD];
    char sifre[MAX_SIFRE];
    
    printf("\n\t Giris Yap \t\n");
    printf("Kullanici Adi: ");
    scanf("%29s", kullaniciadi);
    girdiyitemizle();
    
    printf("Sifre: ");
    scanf("%29s", sifre);
    girdiyitemizle();
    
    int kullanici_index = kullaniciadi_kullanicibul(kullaniciadi);
    if (kullanici_index == -1) {
        printf("Kullanici adi bulunamadi.\n");
        return false;
    }
    
    if (strcmp(kullanicilar[kullanici_index].sifre, sifre) != 0) {
        printf("Hatali sifre.\n");
        return false;
    }
    
    aktifkullanici = kullanicilar[kullanici_index];
    printf("\n Giris Basarili, %s %s!\n", aktifkullanici.ad, aktifkullanici.soyad);
    return true;
}

void cikis() {
    printf("\n Cikisiniz Yapiliyor.\n");
    aktifkullanici.id = -1;
}

void kitapekle() {
    if (kitapsayaci >= MAX_KITAPLAR) {
        printf("Maksimum kitap sayisina ulasildi. Yeni kitap eklenemiyor.\n");
        return;
    }
    
    Kitap yenikitap;
    
    printf("\n\t Yeni Kitap Ekle \n");
    
    yenikitap.id = kitapsayaci > 0 ? kitaplar[kitapsayaci-1].id + 1 : 1;
    
    printf("Baslik: ");
    scanf(" %99[^\n]", yenikitap.baslik);
    girdiyitemizle();
    
    printf("Yazar: ");
    scanf(" %49[^\n]", yenikitap.yazar);
    girdiyitemizle();
    
    printf("Kategori: ");
    scanf(" %29[^\n]", yenikitap.kategori);
    girdiyitemizle();
    
    yenikitap.odunctemi = false;
    yenikitap.oduncalan = -1;
    strcpy(yenikitap.odunctarihi, "");
    
    kitaplar[kitapsayaci++] = yenikitap;
    kitaplarikaydet();
    
    printf("\nKitap basariyla eklendi. Kitap ID: %d\n", yenikitap.id);
}

void kitaplarigoruntule() {
    printf("\n\t Tüm Kitaplar \n");
    printf("Toplam Kitap Sayisi: %d\n", kitapsayaci);
    
    for (int i = 0; i < kitapsayaci; i++) {
       kitapyazdir(kitaplar[i]);
    }
}

void kitapara() {
    int secim;
    
    printf("\n\t Kitap Arama Seçenekleri \n");
    printf("1. Kitap Adina Gore Ara\n");
    printf("2. Yazar Adina Gore Ara\n");
    printf("3. Kategoriye Gore Ara\n");
    printf("Secim Yapiniz: ");
    scanf("%d", &secim);
    girdiyitemizle();
    
    switch (secim) {
        case 1:
            basliktanara();
            break;
        case 2:
            yazardanara();
            break;
        case 3:
            kategoridenara();
            break;
        default:
            printf("Gecersiz secim.\n");
    }
}

void basliktanara() {
    char aramasekli[MAX_BASLIK];
    int bulundu = 0;
    
    printf("\n\t Kitap Adina Gore Ara \n");
    printf("Aranacak kitap adi: ");
    scanf(" %99[^\n]", aramasekli);
    girdiyitemizle();
    
    char kucugeaarama[MAX_BASLIK];
    strcpy(kucugeaarama, aramasekli);
    kucukharfe(kucugeaarama);
    
    printf("\nArama Sonuclari (Kitap Adi: %s):\n", aramasekli);
    
    for (int i = 0; i < kitapsayaci; i++) {
        char kucugebaslik[MAX_BASLIK];
        strcpy(kucugebaslik, kitaplar[i].baslik);
        kucukharfe(kucugebaslik);
        
        if (strstr(kucugebaslik, kucugeaarama) != NULL) {
            kitapyazdir(kitaplar[i]);
            bulundu++;
        }
    }
    
    if (bulundu == 0) {
        printf("Bu ada sahip kitap bulunamadi.\n");
    } else {
        printf("Toplam %d kitap bulundu.\n", bulundu);
    }
}

void yazardanara() {
    char aramasekli[MAX_ISIM];
    int bulundu = 0;
    
    printf("\n--- Yazar Adina Gore Ara ---\n");
    printf("Aranacak yazar adi: ");
    scanf(" %49[^\n]", aramasekli);
    girdiyitemizle();
    
    char kucugeaarama[MAX_ISIM];
    strcpy(kucugeaarama, aramasekli);
    kucukharfe(kucugeaarama);
    
    printf("\nArama Sonuclari (Yazar: %s):\n", aramasekli);
    
    for (int i = 0; i < kitapsayaci; i++) {
        char kucugeyazar[MAX_ISIM];
        strcpy(kucugeyazar, kitaplar[i].yazar);
        kucukharfe(kucugeyazar);
        
        if (strstr(kucugeyazar, kucugeaarama) != NULL) {
            kitapyazdir(kitaplar[i]);
            bulundu++;
        }
    }
    
    if (bulundu == 0) {
        printf("Bu yazara ait kitap bulunamadi.\n");
    } else {
        printf("Toplam %d kitap bulundu.\n", bulundu);
    }
}

void kategoridenara() {
    char aramasekli[MAX_KATEGORI];
    int bulundu = 0;
    
    printf("\n--- Kategoriye Gore Ara ---\n");
    printf("Aranacak kategori: ");
    scanf(" %29[^\n]", aramasekli);
    girdiyitemizle();
    
    char kucugeaarama[MAX_KATEGORI];
    strcpy(kucugeaarama, aramasekli);
    kucukharfe(kucugeaarama);
    
    printf("\nArama Sonuclari (Kategori: %s):\n", aramasekli);
    
    for (int i = 0; i < kitapsayaci; i++) {
        char kucugekategori[MAX_KATEGORI];
        strcpy(kucugekategori, kitaplar[i].kategori);
        kucukharfe(kucugekategori);
        
        if (strstr(kucugekategori, kucugeaarama) != NULL) {
            kitapyazdir(kitaplar[i]);
            bulundu++;
        }
    }

    if (bulundu == 0) {
        printf("Bu kategoride kitap bulunamadi.\n");
    } else {
        printf("Toplam %d kitap bulundu.\n", bulundu);
    }
}

void kitapoduncal() {
    if (aktifkullanici.oduncsayaci >= 3) {
        printf("Zaten maksimum sayida kitap odunc almissiniz (3 kitap).\n");
        return;
    }
    
    int kitap_id;
    char odunctarihi[11];
    
    printf("\n\t Kitap Odunc Al \n");
    printf("Odunc almak istediginiz kitabin ID'sini girin: ");
    scanf("%d", &kitap_id);
    girdiyitemizle();
    
    int kitap_index = id_kitapbul(kitap_id);
    if (kitap_index == -1) {
        printf("Bu ID'ye sahip bir kitap bulunamadi.\n");
        return;
    }
    
    if (kitaplar[kitap_index].odunctemi) {
        printf("Bu kitap zaten odunc alinmis.\n");
        return;
    }
    
    printf("Odunc alma tarihi (YYYY.AA.GG): ");
    scanf("%10s", odunctarihi);
    girdiyitemizle();
    
    if (!tarihkontrol(odunctarihi)) {
        printf("Gecersiz tarih formati. Lütfen YYYY.AA.GG formatinda girin.\n");
        return;
    }
    
    // Kitabı ödünç al
    kitaplar[kitap_index].odunctemi = true;
    kitaplar[kitap_index].oduncalan = aktifkullanici.id;
    strcpy(kitaplar[kitap_index].odunctarihi, odunctarihi);
    
    // Kullanıcının ödünç aldığı kitaplara ekle
    aktifkullanici.oduncte_kitap[aktifkullanici.oduncsayaci++] = kitap_id;
    
    // İşlem kaydı oluştur
    Islem yeniislem;
    yeniislem.kitap_id = kitap_id;
    yeniislem.kullanici_id = aktifkullanici.id;
    strcpy(yeniislem.odunctarihi , odunctarihi);
    strcpy(yeniislem.iadetarihi, "");
    islemler[islemsayaci++] = yeniislem;
    
    // Kullanıcı bilgisini güncelle
    int kullanici_index = id_kullanicibul(aktifkullanici.id);
    if (kullanici_index != -1) {
        kullanicilar[kullanici_index] = aktifkullanici;
    }
    
    kitaplarikaydet();
    kullanicilarikaydet();
    islemlerikaydet();
    
    printf("\nKitap basariyla odunc alindi.\n");
}

void kitapiade() {
    if (aktifkullanici.oduncsayaci == 0) {
        printf("Odunc aldiginiz kitap bulunmamaktadir.\n");
        return;
    }
    
    int kitap_id;
    char iadetarihi[11];
    
    printf("\n\t Kitap İade Et \n");
    printf("İade etmek istediğiniz kitabin ID'sini girin: ");
    scanf("%d", &kitap_id);
    girdiyitemizle();
    
    bool odunaalindimi = false;
    int oduncte_index = -1;
    for (int i = 0; i < aktifkullanici.oduncsayaci; i++) {
        if (aktifkullanici.oduncte_kitap[i] == kitap_id) {
            odunaalindimi = true;
            oduncte_index = i;
            break;
        }
    }
    
    if (!odunaalindimi) {
        printf("Bu kitabi odunc almamissiniz.\n");
        return;
    }
    
    int kitap_index = id_kitapbul(kitap_id);
    if (kitap_index == -1) {
        printf("Bu ID'ye sahip bir kitap bulunamadi.\n");
        return;
    }
    
    printf("İade tarihi (YYYY.AA.GG): ");
    scanf("%10s", iadetarihi);
    girdiyitemizle();
    
    if (!tarihkontrol(iadetarihi)) {
        printf("Gecersiz tarih formati. Lütfen YYYY.AA.GG formatinda girin.\n");
        return;
    }
    
    // Kitabı iade et
    kitaplar[kitap_index].odunctemi = false;
    kitaplar[kitap_index].oduncalan = -1;
    strcpy(kitaplar[kitap_index].odunctarihi, "");
    
    // Kullanıcının ödünç aldığı kitaplardan çıkar
    for (int i = oduncte_index; i < aktifkullanici.oduncsayaci - 1; i++) {
        aktifkullanici.oduncte_kitap[i] = aktifkullanici.oduncte_kitap[i+1];
    }
    aktifkullanici.oduncsayaci--;
    
    // İşlem kaydını güncelle
    for (int i = 0; i < islemsayaci; i++) {
        if (islemler[i].kitap_id == kitap_id && 
            islemler[i].kullanici_id == aktifkullanici.id && 
            strlen(islemler[i].iadetarihi) == 0) {
            strcpy(islemler[i].iadetarihi, iadetarihi);
            break;
        }
    }
    
    // Kullanıcı bilgisini güncelle
    int kullanici_index = id_kullanicibul(aktifkullanici.id);
    if (kullanici_index != -1) {
        kullanicilar[kullanici_index] = aktifkullanici;
    }
    
    kitaplarikaydet();
    kullanicilarikaydet();
    islemlerikaydet();
    
    printf("\nKitap basariyla iade edildi.\n");
}

void odunclerim() {
    printf("\n\t Odunc Alinan Kitaplar \n");
    
    if (aktifkullanici.oduncsayaci == 0) {
        printf("Su anda odunc aldiginiz kitap bulunmamaktadir.\n");
        return;
    }
    
    printf("Odunc aldiginiz kitaplar:\n");
    for (int i = 0; i < aktifkullanici.oduncsayaci; i++) {
        int kitap_index = id_kitapbul(aktifkullanici.oduncte_kitap[i]);
        if (kitap_index != -1) {
            kitapyazdir(kitaplar[kitap_index]);
        }
    }
}

void kullanicigoruntule() {
    printf("\n\t Tum Kullanicilar \n");
    printf("Toplam Kullanici Sayisi: %d\n", kullanicisayaci);
    
    for (int i = 0; i < kullanicisayaci; i++) {
        kullaniciyazdir(kullanicilar[i]);
    }
}


void islemlerigoruntule() {
    printf("\n\t Tum İslemler \n");
    printf("Toplam İslem Sayisi: %d\n", islemsayaci);
    
    for (int i = 0; i < islemsayaci; i++) {
        islemleriyazdir(islemler[i]);
    }
}


void anasayfa() {
    int secim;
    
    while (1) {
        printf("\n\t KUTUPHANE YONETIM SISTEMI \n");
        printf("1. Giris Yap\n");
        printf("2. Kayit Ol\n");
        printf("3. Cikis Yap\n");
        printf("Secim Yapiniz: ");
        scanf("%d", &secim);
        girdiyitemizle();
        
        switch (secim) {
            case 1:
                if (giris()) {
                    if (aktifkullanici.adminmi) {
                        admin_menu();
                    } else {
                        kullanici_menu();
                    }
                }
                break;
            case 2:
                kayit();
                break;
            case 3:
                printf("Cikisiniz Yapiliyor.\n");
                exit(0);
            default:
                printf("Gecersiz secim. Lütfen tekrar deneyin.\n");
        }
    }
}

void kullanici_menu() {
    int secim;
    
    while (aktifkullanici.id != -1) {
        printf("\n\t Kullanici Paneli (Ogrenci ve Personel icin.) \n");
        printf("Ad ve soyadinizi giriniz., %s %s\n", aktifkullanici.ad, aktifkullanici.soyad);
        printf("1. Kitaplari Listele\n");
        printf("2. Kitap Ara\n");
        printf("3. Kitap Odunc Al\n");
        printf("4. Kitap İade Et\n");
        printf("5. Odunc Aldigim Kitaplar\n");
        printf("6. Cikis Yap\n");
        printf("Secim Yapiniz: ");
        scanf("%d", &secim);
        girdiyitemizle();
        
        switch (secim) {
            case 1:
                kitaplarigoruntule();
                break;
            case 2:
                kitapara();
                break;
            case 3:
                kitapoduncal();
                break;
            case 4:
                kitapiade();
                break;
            case 5:
                odunclerim();
                break;
            case 6:
                cikis();
                break;
            default:
                printf("Gecersiz secim. Lütfen tekrar deneyin.\n");
        }
    }
}

void admin_menu() {
    int secim;
    
    while (aktifkullanici.id != -1) {
        printf("\n\t Yonetici Paneli (Admin icin.) \n");
        printf("Ad ve soyadinizi Giriniz., %s %s (Yonetici)\n", aktifkullanici.ad, aktifkullanici.soyad);
        printf("1. Kitaplari Listele\n");
        printf("2. Kitap Ara\n");
        printf("3. Yeni Kitap Ekle\n");
        printf("4. Kullanicilari Listele\n");
        printf("5. Cikis Yap\n");
        printf("6. Tum İslemleri Goruntule\n");
        printf("Secim Yapiniz: ");
        scanf("%d", &secim);
        girdiyitemizle();
        
        switch (secim) {
            case 1:
                kitaplarigoruntule();
                break;
            case 2:
                kitapara();
                break;
            case 3:
                kitapekle();
                break;
            case 4:
                kullanicigoruntule();
                break;
            case 5:
                cikis();
                break;
            case 6:
                islemlerigoruntule();
                break;
            default:
                printf("Gecersiz secim. Lütfen tekrar deneyin.\n");
        }
    }
}

