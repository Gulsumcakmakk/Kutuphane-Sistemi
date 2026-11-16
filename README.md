# Kütüphane Yönetim Sistemi

C diliyle yazılmış, konsol tabanlı bir kütüphane yönetim sistemidir.

## Özellikler

- Kullanıcı kaydı ve girişi
- Kullanıcı ve yönetici (admin) panelleri
- Kitap ekleme (admin) ve listeleme
- Kitap adına, yazara ve kategoriye göre arama
- Kitap ödünç alma ve iade etme (kullanıcı başına en fazla 3 kitap)
- Kullanıcıların ve tüm işlemlerin listelenmesi (admin)
- Veriler ikili dosyalarda saklanır

## Derleme ve Çalıştırma

```
gcc -Wall -o prog_proje prog_proje.c
.\prog_proje.exe
```

Türkçe karakterler bozuk görünürse konsolda `chcp 65001` komutunu çalıştırın.

## Varsayılan Yönetici Hesabı

Hiç kullanıcı yoksa program otomatik olarak bir yönetici hesabı oluşturur:

- Kullanıcı adı: `admin`
- Şifre: `admin123`

## Veri Dosyaları

İlk çalıştırmada program klasöründe şu dosyalar oluşur:

- `books.dat` – kitaplar
- `users.dat` – kullanıcılar
- `transactions.dat` – ödünç/iade işlemleri

## Notlar

- Tarihler `YYYY.AA.GG` formatında girilir (örn. `2026.11.16`).
- Şifreler düz metin olarak saklanır; bu proje eğitim amaçlıdır.
