<p align="center">
  <img src="https://img.shields.io/badge/Elli-bash-red?style=for-the-badge&logoColor=white&labelColor=gray" alt="ElliBash Prompt"/>
</p>

## 🤖 Minishell  - Bir Unix Kabuğunun Yeniden Doğuşu | Ecole 42
Bu repository, 42 müfredatının zirve projelerinden biri olan minishell'in geliştirme sürecini ve sonuçlarını sunmaktadır. Bu proje, ekip arkadaşım **Oktay Çubukcu** (ocubukcu) ile birlikte yürüttüğümüz, bash gibi bir Unix kabuğunun C dilinde sıfırdan implemente edildiği teknik bir çalışmadır. Bu proje, sadece bir kodlama görevi değil, aynı zamanda iki kişilik bir ekibin karmaşık bir sistem üzerinde nasıl uyum içinde çalışabileceğinin, sorunları nasıl birlikte çözebileceğinin ve ortak bir mühendislik vizyonu oluşturabildiğinin bir kanıtıdır.

---

### Projenin Felsefesi ve Kısıtlamaları

Minishell, bizden sadece kod yazmamızı istemedi; bir işletim sisteminin ruhunu anlamamızı talep etti. Projenin en temel ve zorlayıcı kuralı, tüm bu karmaşık yapıyı yalnızca belirli bir grup harici fonksiyon kullanarak inşa etme zorunluluğuydu. Bu kısıtlama, bizi yüksek seviye soyutlamalardan uzaklaştırıp, sistemin en temel çağrılarıyla doğrudan çalışmaya itti.

Tüm proje boyunca kullanmamıza izin verilen fonksiyonlar şunlardı:

readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

Bu sınırlı araç setiyle çalışmak, bize sadece teknik yetkinlikler kazandırmakla kalmadı, aynı zamanda paha biçilmez ekip çalışması becerileri de kazandırdı.

**Teknik Hakimiyet:** Proses yönetimi, prosesler arası iletişim, sinyal yönetimi ve kaynak yönetimi gibi konularda derin bir anlayış kazandık.

**Ekip Çalışması ve İletişim:** İki kişilik bir ekip olarak, kod standartları belirlemek, görevleri adil bir şekilde bölmek, düzenli kod birleştirme seansları yapmak ve karşılaşılan teknik çıkmazlarda birbirine destek olmak gibi kritik süreçleri deneyimledik.

**Problem Çözme:** Minishell, ezberci çözümlerin işe yaramadığı, her detayın titizlikle düşünülmesi gereken bir projeydi. Karşılaştığımız sayısız "edge case" ve hata senaryosu, algoritmik düşünme ve "debugging" yeteneklerimizi en üst seviyeye taşıdı.

**Çekirdek Fonksiyonalite: Teknik Implementasyon**

Projenin geliştirilmesi, 42 tarafından belirlenen katı kurallar çerçevesinde, ekibimizin ortak mühendislik kararlarıyla şekillendi.

---

#### 1. Komut Ayrıştırma (Parsing) ve Veri Yapıları

**Gereksinim:** Kullanıcı girdisini, tırnak işaretleri, pipe'lar ve yönlendirmeler gibi özel karakterleri doğru yorumlayarak, yürütülebilir bir yapıya dönüştürmek.

**Bizim Çözümümüz:** Projemizin kalbinde, her bir komutu ve onun tüm bileşenlerini temsil eden bir t_list (linked list) yapısı bulunmaktadır. Bu listenin her düğümü, komutun kendisini, argümanlarını ve ona ait yönlendirmeleri barındıran bir t_cmd struct'ını tutar. Bu esnek yapı, Lexer (girdiyi token'lara ayıran) ve Parser (token'ları bu yapıya dönüştüren) modüllerimiz tarafından titizlikle inşa edilir.

---

#### 2. Yürütme Motoru ve Proses Yönetimi

**Gereksinim:** Ayrıştırılmış komutları çalıştırmak, PATH değişkenini kullanmak ve pipe'lar aracılığıyla komutlar arası veri akışını sağlamak.

**Bizim Çözümümüz:** Geliştirdiğimiz yürütme motoru, komut listemizi alır ve her bir komut için fork() ile bir alt proses (child process) oluşturur. Bir pipeline söz konusu olduğunda, pipe() ile yarattığımız dosya tanımlayıcılarını (FD), bir komutun çıktısını bir sonrakinin girdisine dup2 ile bağlayarak veri akışını yönettik.

---

#### 3. Sıfırdan Yazılan Yerleşik Komutlar (Built-in Commands)

**Gereksinim:** echo, cd, pwd, export, unset, env, exit gibi komutları, kabuğun kendi prosesini etkileyecek şekilde implemente etmek.

**Bizim Çözümümüz:** Bu komutların, ana prosesin durumunu değiştirmesi gerektiğinin farkındaydık. Bu nedenle, her birini sıfırdan, harici bir programa ihtiyaç duymadan çalışacak şekilde kodladık. export ve unset için, envp dizisini yönetimi daha kolay olan kendi t_list yapımıza aktararak, ortam değişkenleri üzerinde tam kontrol sağladık.

---

#### 4. Bellek ve Kaynak Yönetimi

**Gereksinim:** valgrind ile test edildiğinde sıfır bellek sızıntısı ve sıfır açık dosya tanımlayıcısı.

**Bizim Çözümümüz:** Bu, projedeki en tavizsiz kuralımızdı. Ekip olarak, programın her bir olası çıkış noktasında tüm kaynakların eksiksiz serbest bırakılmasını garanti altına alan merkezi temizlik (garbage collection) rutinleri yazdık. Her fork()'tan sonra, alt proseste execve() çağrılmadan hemen önce, o ana kadar tahsis edilmiş tüm bellek ve gereksiz FD'ler titizlikle temizlendi.

---

### ⚙️ Kullanım Talimatları

**Derleme**
```Bash

make
```
**Çalıştırma**
```Bash

./minishell
```
---

## Bellek Yönetimi ve Sızıntı Kontrolü (Valgrind)

Minishell projesinin en kritik gereksinimlerinden biri, kendi yazdığımız kodda hiçbir bellek sızıntısı (leak) veya açık dosya tanımlayıcısı (FD) bırakmamaktır. readline kütüphanesi, kendi doğası gereği bazı bellek blokları ayırır ve valgrind'de "gürültü" olarak adlandırabileceğimiz bazı hatalar rapor edebilir.

Bu "gürültüyü" genel komutlarla bastırmak yerine, projemizde çok daha rafine bir yaklaşım benimsenmiştir: Valgrind Suppression File. Bu yöntemle, sadece ve sadece readline kütüphanesinden kaynaklandığını bildiğimiz spesifik hata ve sızıntıları, ignore-readline.supp adında bir dosyada tanımlayarak valgrind'in bunları görmezden gelmesini sağlarız. **Proje dizininde bahsettiğimiz dosya mevcut onunla deneyebilir ve dosyayı inceleyebilirsiniz**.Bu, kendi kodumuzdaki en ufak bir hatayı bile gözden kaçırmamamızı garanti altına alır.

Projenin bellek ve FD yönetimini bu hassas yöntemle test etmek için aşağıdaki komut kullanılır:

```Bash

valgrind --leak-check=full --show-leak-kinds=all --suppressions=ignore-readline.supp --track-fds=yes ./minishell
```
**Komut Açıklaması:**

+ **--leak-check=full:** Mümkün olan en detaylı bellek sızıntısı kontrolünü yapar.

+ **--show-leak-kinds=all:** Sadece kesin sızıntıları değil, still reachable dahil tüm sızıntı türlerini gösterir. Bu, tam bir rapor almak için önemlidir.

+ **--suppressions=ignore-readline.supp:** Bu, en kritik bayraktır. valgrind'e, proje dizininde bulunan ignore-readline.supp dosyasını okumasını ve bu dosya içinde tanımlanmış olan tüm hata/sızıntı raporlarını görmezden gelmesini söyler. Böylece readline'ın bilinen "hataları" rapordan temizlenir.

+ **--track-fds=yes:** Program sonlandığında hala açık olan dosya tanımlayıcılarını (FD) raporlar. Bu, pipe ve yönlendirmelerdeki kaynak yönetimimizin doğruluğunu test etmek için hayati önem taşır.

**Bu komutla yapılan testlerde "All heap blocks were freed -- no leaks are possible" ve "FILE DESCRIPTORS: 3 open" (stdin, stdout, stderr) sonucunu almak, projenin kaynak yönetimindeki mutlak başarısını gösterir.**

```bash

ellibash~> exit
exit
==49119== 
==49119== FILE DESCRIPTORS: 3 open (3 std) at exit.
==49119== 
==49119== HEAP SUMMARY:
==49119==     in use at exit: 232,863 bytes in 234 blocks
==49119==   total heap usage: 522 allocs, 288 frees, 257,709 bytes allocated
==49119== 
==49119== LEAK SUMMARY:
==49119==    definitely lost: 0 bytes in 0 blocks
==49119==    indirectly lost: 0 bytes in 0 blocks
==49119==      possibly lost: 0 bytes in 0 blocks
==49119==    still reachable: 0 bytes in 0 blocks
==49119==         suppressed: 232,863 bytes in 234 blocks
==49119== 
==49119== For lists of detected and suppressed errors, rerun with: -s
==49119== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
---

### ⚠️ Sorumluluk Reddi

Bu repository'deki çözümler tamamen eğitim ve portföy amaçlıdır. 42'nin onur kuralları (honour code) gereği, Common Core sürecindeki öğrencilerin çözümleri kopyalaması kesinlikle yasaktır. Buradaki kodlar, repodaki projeyi tamamlamış biri olarak gelişimimi göstermek ve gelecekteki projelerim için bir referans noktası oluşturmak amacıyla paylaşılmıştır.

---