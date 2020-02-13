#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#define FDE "FDE.AI-v8.0"
#define VER "/dev/fdeai/800"

static void runnability(void);
static char *timestamp(void);
static void frm(char ffrm[]);
static void fsetval(char fsetputval[], char ffsetput[]);
static void fecho(char fputval[], char ffput[]);
static void fecholong(long livsa, char lfdfput[]);
static void flog(char ftxt[], char itxt[], short int timed);
static void flogval(char ftxtt[], char ytxt[], long ival, short int timedd);
struct tm *localtime_r(const time_t *timep, struct tm *result);
char *asctime_r(const struct tm *tm, char *buf);
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);

static void runnability() { system("touch /dev/fdeai/800"); }

static char *timestamp() {
  time_t ltime = time(NULL);
  static struct tm result;
  localtime_r(&ltime, &result);
  static char stime[sizeof(result)];
  (void)asctime_r(&result, stime);
  (void)strftime(stime, sizeof(stime), "[%H:%M]", &result);
  return stime;
}

static short int file_exist(char *filename) {
  static struct stat buffer;
  return (stat(filename, &buffer) == 0);
}
static void frm(char ffrm[]) {
  if (file_exist(ffrm)) {
    remove(ffrm);
  };
}
static char *fcat(char ffcat[]) {
  if (file_exist(ffcat)) {
    FILE *fread = fopen(ffcat, "r");
    if (fread != NULL) {
      if (fgets(ffcat, 1024, fread) != NULL) {
        ffcat[strcspn(ffcat, "\r\n")] = 0;
      } else {
        strcpy(ffcat, "0");
      };
      fclose(fread);
    };
    return ffcat;
  } else {
    return NULL;
  };
}
static void fsetval(char fsetputval[], char ffsetput[]) {
  if (file_exist(ffsetput)) {
    chmod(ffsetput, 0664);
    FILE *fwrite = fopen(ffsetput, "w");
    if (fwrite != NULL) {
      fputs(fsetputval, fwrite);
      fclose(fwrite);
    };
    chmod(ffsetput, 0444);
  };
}
static void fecho(char fputval[], char ffput[]) {
  if (file_exist(ffput)) {
    FILE *ftwrite = fopen(ffput, "w");
    if (ftwrite == NULL) {
      fsetval(fputval, ffput);
    } else {
      fputs(fputval, ftwrite);
      fclose(ftwrite);
    };
  };
}
static void fecholong(long livsa, char lfdfput[]) {
  if (file_exist(lfdfput)) {
    FILE *fwrite = fopen(lfdfput, "w");
    fprintf(fwrite, "%ld", livsa);
    fclose(fwrite);
  };
}
static void flog(char ftxt[], char itxt[], short int timed) {
  FILE *aflog;
  if (file_exist("/data/data/com.feravolt.fdeai")) {
    aflog = fopen("/dev/fdeai/fde.txt", "a+");
  } else if (file_exist("/sdcard/Android")) {
    aflog = fopen("/sdcard/Android/fde.txt", "a");
  } else if (file_exist("/storage/emulated/0/Android")) {
    aflog = fopen("/storage/emulated/0/Android/fde.txt", "a");
  } else {
    aflog = fopen("/dev/fde.txt", "a+");
  };
  if (aflog != NULL) {
    if (itxt != NULL) {
      if (timed == 1) {
        fprintf(aflog, "%s %s%s\n", timestamp(), ftxt, itxt);
      } else {
        fprintf(aflog, "> %s%s\n", ftxt, itxt);
      };
    } else if (ftxt != NULL) {
      fprintf(aflog, "%s %s\n", timestamp(), ftxt);
    };
    fclose(aflog);
  };
}
static void flogval(char ftxtt[], char ytxt[], long ival, short int timedd) {
  FILE *fdelog;
  if (file_exist("/data/data/com.feravolt.fdeai")) {
    fdelog = fopen("/dev/fdeai/fde.txt", "a+");
  } else if (file_exist("/sdcard/Android")) {
    fdelog = fopen("/sdcard/Android/fde.txt", "a");
  } else if (file_exist("/storage/emulated/0/Android")) {
    fdelog = fopen("/storage/emulated/0/Android/fde.txt", "a");
  } else {
    fdelog = fopen("/dev/fde.txt", "a+");
  };
  if (fdelog != NULL) {
    if (ftxtt != NULL) {
      if (timedd == 1) {
        fprintf(fdelog, "%s %s%s%ld\n", timestamp(), ftxtt, ytxt, ival);
      } else {
        fprintf(fdelog, "> %s%ld\n", ftxtt, ival);
      };
    };
    fclose(fdelog);
  };
}
static long freeram() {
  long RAMFREE = 1;
  FILE *meminfoo = fopen("/proc/meminfo", "r");
  if (meminfoo != NULL) {
    char freeram[128];
    long real = 1;
    long cache = 1;
    while (fgets(freeram, 128, meminfoo) != NULL) {
      if (strstr(freeram, "emFree") != 0) {
        char *rim = strpbrk(freeram, "0123456789");
        char *ramfreekb = strtok(rim, " ");
        long rfreel = strtol(ramfreekb, NULL, 10);
        real = rfreel / 1024;
      };
      if (strstr(freeram, "Cached") != 0) {
        char *riim = strpbrk(freeram, "0123456789");
        char *ramifreekb = strtok(riim, " ");
        long rfreeil = strtol(ramifreekb, NULL, 10);
        cache = rfreeil / 1024;
        RAMFREE = real + cache;
        break;
      };
    };
    fclose(meminfoo);
    memset(freeram, 0, sizeof(freeram));
  };
  return RAMFREE;
}
static long GetCPULoad() {
  double a[5], b[5], loadavg;
  long cpuload = 0;
  FILE *cfp;
  cfp = fopen("/proc/stat", "r");
  fscanf(cfp, "%*s %lf %lf %lf %lf", &a[0], &a[1], &a[2], &a[3]);
  fclose(cfp);
  sleep(2);
  cfp = fopen("/proc/stat", "r");
  fscanf(cfp, "%*s %lf %lf %lf %lf", &b[0], &b[1], &b[2], &b[3]);
  fclose(cfp);
  loadavg = ((b[0] + b[1] + b[2]) - (a[0] + a[1] + a[2])) / ((b[0] + b[1] + b[2] + b[3]) - (a[0] + a[1] + a[2] + a[3]));
  cpuload = loadavg * 100;
  if (cpuload >= 100) {
    cpuload = 100;
  } else if (cpuload <= 1) {
    cpuload = 1;
  };
  memset(a, 0, sizeof(a));
  memset(b, 0, sizeof(b));
  return cpuload;
}
static long btmp() {
  static long BTEMP = 1;
  FILE *battt = popen("dumpsys battery", "r");
  if (battt != NULL) {
    char bettemp[128];
    while (fgets(bettemp, 128, battt) != NULL) {
      if (strstr(bettemp, "perature") != 0) {
        char *bim = strpbrk(bettemp, "0123456789");
        char *bkb = strtok(bim, ":");
        long bel = strtol(bkb, NULL, 10);
        BTEMP = bel / 10;
        break;
      };
    };
    fclose(battt);
    memset(bettemp, 0, sizeof(bettemp));
  };
  return BTEMP;
}

int main() {
  printf("%s by FeraVolt. 2020.\n", FDE);
  runnability();
  fsetval("1", VER);
  if (file_exist("/sys/power/wake_lock")) {
    system("/dev/fdeai/busybox echo 'FDE' > /sys/power/wake_lock");
  };
  if (file_exist("/data/data/com.feravolt.fdeai")) {
    frm("/sdcard/Android/fde.txt");
    frm("/dev/fdeai/fde.txt");
    system("touch /dev/fdeai/fde.txt;chown 0:0 /dev/fdeai/fde.txt;chcon u:object_r:system_data_file:s0 /dev/fdeai/fde.txt;");
    chmod("/dev/fdeai/fde.txt", 0666);
  } else if (file_exist("/sdcard/Android")) {
    frm("/sdcard/Android/fde.txt");
    system("touch /sdcard/Android/fde.txt");
    chmod("/sdcard/Android/fde.txt", 0666);
  } else if (file_exist("/storage/emulated/0/Android")) {
    frm("/storage/emulated/0/Android/fde.txt");
    system("touch /storage/emulated/0/Android/fde.txt");
    chmod("/storage/emulated/0/Android/fde.txt", 0666);
  } else {
    frm("/dev/fde.txt");
    system("touch /dev/fde.txt;chown 0:0 /dev/fde.txt;chcon u:object_r:system_data_file:s0 /dev/fde.txt;");
    chmod("/dev/fde.txt", 0666);
  };
  FILE *getroot = popen("su -v", "r");
  char ROOT[128];
  if (getroot != NULL) {
    if (fgets(ROOT, (int)sizeof(ROOT), getroot) != NULL) {
      ROOT[strcspn(ROOT, "\r\n")] = 0;
    };
    pclose(getroot);
  };
  FILE *meminfo = fopen("/proc/meminfo", "r");
  char ramm[2048];
  (void)fgets(ramm, (int)sizeof(ramm), meminfo);
  fclose(meminfo);
  char *rams = strpbrk(ramm, "0123456789");
  char *ramkb = strtok(rams, " ");
  long ramc = strtol(ramkb, NULL, 10);
  long RAM = ramc / 1024;
  FILE *possib = fopen("/sys/devices/system/cpu/possible", "r");
  static char coress[64];
  if (possib != NULL) {
    if (fgets(coress, (int)sizeof(coress), possib) != NULL) {
      coress[strcspn(coress, "\r\n")] = 0;
    };
    fclose(possib);
  };
  char *core = strstr(coress, "-");
  char *ccore = strpbrk(core, "0123456789");
  long cors = strtol(ccore, NULL, 10);
  long CORES = cors + 1;
  FILE *getabi = popen("getprop ro.product.cpu.abi", "r");
  char ARCH[64];
  if (getabi != NULL) {
    if (fgets(ARCH, (int)sizeof(ARCH), getabi) != NULL) {
      ARCH[strcspn(ARCH, "\r\n")] = 0;
    };
    pclose(getabi);
  };
  FILE *getlang = popen("getprop persist.sys.locale", "r");
  static char LANG[32];
  if (getlang != NULL) {
    if (fgets(LANG, (int)sizeof(LANG), getlang) != NULL) {
      LANG[strcspn(LANG, "\r\n")] = 0;
    };
    pclose(getlang);
  };
  static short int rus;
  if (strstr(LANG, "ru-") != 0) {
    rus = 0;
  } else {
    rus = 1;
  };
  if (strlen(LANG) <= 2) {
    strcpy(LANG, "unknown");
  };
  FILE *getrom = popen("getprop ro.build.display.id", "r");
  char RRM[256];
  if (getrom != NULL) {
    if (fgets(RRM, (int)sizeof(RRM), getrom) != NULL) {
      RRM[strcspn(RRM, "\r\n")] = 0;
    };
    pclose(getrom);
  };
  FILE *getkern = popen("uname -r", "r");
  static char KERN[256];
  if (getkern != NULL) {
    if (fgets(KERN, (int)sizeof(KERN), getkern) != NULL) {
      KERN[strcspn(KERN, "\r\n")] = 0;
    };
    pclose(getkern);
  };
  FILE *getsdk = popen("getprop ro.build.version.sdk", "r");
  static char ssdk[32];
  if (getsdk != NULL) {
    if (fgets(ssdk, (int)sizeof(ssdk), getsdk) != NULL) {
      ssdk[strcspn(ssdk, "\r\n")] = 0;
    };
    pclose(getsdk);
  };
  long SDK = strtol(ssdk, NULL, 10); // SDK
  char OSVER[8];
  if (SDK >= 30) {
    strcpy(OSVER, "11");
  } else if (SDK == 29) {
    strcpy(OSVER, "10");
  } else if (SDK == 28) {
    strcpy(OSVER, "9");
  } else if (SDK == 27) {
    strcpy(OSVER, "8.1");
  } else if (SDK == 26) {
    strcpy(OSVER, "8.0");
  } else if (SDK == 25) {
    strcpy(OSVER, "7.1");
  } else if (SDK == 24) {
    strcpy(OSVER, "7.0");
  } else if (SDK == 23) {
    strcpy(OSVER, "6.0");
  } else if (SDK == 22) {
    strcpy(OSVER, "5.1");
  } else if (SDK == 21) {
    strcpy(OSVER, "5.0");
  } else if (SDK == 20) {
    strcpy(OSVER, "4.4W");
  } else if (SDK == 19) {
    strcpy(OSVER, "4.4");
  } else if (SDK == 18) {
    strcpy(OSVER, "4.3");
  } else if (SDK == 17) {
    strcpy(OSVER, "4.2");
  } else if (SDK == 16) {
    strcpy(OSVER, "4.1");
  } else {
    strcpy(OSVER, "unknown");
  };
  fecho("9999", "/proc/sys/vm/vfs_cache_pressure");
  FILE *getgpu = popen("dumpsys SurfaceFlinger", "r");
  char GGPU[512];
  if (getgpu != NULL) {
    char GPU[3072];
    while (fgets(GPU, (int)sizeof(GPU), getgpu) != NULL) {
      if (strstr(GPU, "GLES:") != 0) {
        char *hmm = strchr(GPU, ':');
        if (strlen(GPU) >= 4) {
          strcpy(GGPU, hmm);
        } else {
          strcpy(GGPU, "unknown");
        };
      };
    };
    pclose(getgpu);
    GGPU[strcspn(GGPU, "\r\n")] = 0;
    memset(GPU, 0, sizeof(GPU));
  } else {
    strcpy(GGPU, "unknown");
  };
  FILE *getdevice = popen("A=$(getprop ro.product.brand);B=$(getprop ro.product.model);/dev/fdeai/busybox echo $A $B;", "r");
  char fulldevname[256];
  if (getdevice != NULL) {
    if (fgets(fulldevname, (int)sizeof(fulldevname), getdevice) != NULL) {
      fulldevname[strcspn(fulldevname, "\r\n")] = 0;
    };
    pclose(getdevice);
  };
  FILE *fdelogg;
  if (file_exist("/data/data/com.feravolt.fdeai")) {
    fdelogg = fopen("/dev/fdeai/fde.txt", "a+");
  } else if (file_exist("/sdcard/Android")) {
    fdelogg = fopen("/sdcard/Android/fde.txt", "a");
  } else if (file_exist("/storage/emulated/0/Android")) {
    fdelogg = fopen("/storage/emulated/0/Android/fde.txt", "a");
  } else {
    fdelogg = fopen("/dev/fde.txt", "a+");
  };
  if (fdelogg != NULL) {
    fprintf(fdelogg, "%s by FeraVolt. 2020.\n", FDE);
    fprintf(fdelogg, "%s\n", " ");
    fprintf(fdelogg, "%s\n", "...");
    fclose(fdelogg);
  };
  char CPUS[256];
  if (file_exist("/sys/devices/soc0/machine")) {
    char doots[] = ": ";
    char machs[] = "/sys/devices/soc0/machine";
    strcat(doots, fcat(machs));
    strcpy(CPUS, doots);
    memset(machs, 0, sizeof(machs));
    memset(doots, 0, sizeof(doots));
  } else {
    char cpuinfo[3072];
    FILE *cpuinfoo = fopen("/proc/cpuinfo", "r");
    if (cpuinfoo != NULL) {
      while (fgets(cpuinfo, 3072, cpuinfoo) != NULL) {
        if (strstr(cpuinfo, "ardware") != 0) {
          char *CPUU = strstr(cpuinfo, ":");
          if (strlen(CPUU) >= 4) {
            CPUU[strcspn(CPUU, "\r\n")] = 0;
            strcpy(CPUS, CPUU);
          } else {
            CPUU[strcspn(CPUU, "\r\n")] = 0;
            strcpy(CPUS, ": unknown");
          };
        }
      };
      fclose(cpuinfoo);
    };
    memset(cpuinfo, 0, sizeof(cpuinfo));
  };
  char cpufre[32];
  char cpugovern[32];
  static long MAXfreq = 0;
  if (file_exist("/sys/devices/system/cpu/cpu11/cpufreq/cpuinfo_max_freq")) {
    char CPpath[] = "/sys/devices/system/cpu/cpu11/cpufreq/cpuinfo_max_freq";
    strcpy(cpufre, fcat(CPpath));
  } else if (file_exist("/sys/devices/system/cpu/cpu9/cpufreq/cpuinfo_max_freq")) {
    char CPpath[] = "/sys/devices/system/cpu/cpu9/cpufreq/cpuinfo_max_freq";
    strcpy(cpufre, fcat(CPpath));
  } else if (file_exist("/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_max_freq")) {
    char CPpath[] = "/sys/devices/system/cpu/cpu7/cpufreq/cpuinfo_max_freq";
    strcpy(cpufre, fcat(CPpath));
  } else if (file_exist("/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_max_freq")) {
    char CPpath[] = "/sys/devices/system/cpu/cpu3/cpufreq/cpuinfo_max_freq";
    strcpy(cpufre, fcat(CPpath));
  } else if (file_exist("/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_max_freq")) {
    char CPpath[] = "/sys/devices/system/cpu/cpu1/cpufreq/cpuinfo_max_freq";
    strcpy(cpufre, fcat(CPpath));
  } else if (file_exist("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq")) {
    char CPpath[] = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq";
    strcpy(cpufre, fcat(CPpath));
  } else {
    char CPpath[] = "/sys/devices/system/cpu/cpufreq/cpuinfo_max_freq";
    strcpy(cpufre, fcat(CPpath));
  };
  if (file_exist("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor")) {
    char CPgov[] = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";
    strcpy(cpugovern, fcat(CPgov));
  } else {
    char CPgov[] = "/sys/devices/system/cpu/cpufreq/scaling_governor";
    strcpy(cpugovern, fcat(CPgov));
  };
  long MAXFR = strtol(cpufre, NULL, 10);
  MAXfreq = MAXFR / 1000;
  FILE *getselinx = popen("getenforce", "r");
  char selinx[64];
  if (getselinx != NULL) {
    if (fgets(selinx, (int)sizeof(selinx), getselinx) != NULL) {
      selinx[strcspn(selinx, "\r\n")] = 0;
    };
    pclose(getselinx);
  };
  static char AIN[64];
  if (file_exist("/data/data/com.feravolt.fdeai/ainame")) {
    FILE *getnume = popen("/dev/fdeai/busybox cat /data/data/com.feravolt.fdeai/ainame", "r");
    if (getnume != NULL) {
      if (fgets(AIN, 64, getnume) != NULL) {
        AIN[strcspn(AIN, "\r\n")] = 0;
      };
      pclose(getnume);
    };
  } else {
    if (file_exist("/data/data/com.feravolt.fdeai")) {
      system("touch /data/data/com.feravolt.fdeai/ainame");
      fecho("\n", "/data/data/com.feravolt.fdeai/ainame");
    };
  };
  if (strlen(AIN) <= 2) {
    if (rus == 0) {
      strcpy(AIN, "Элизабет");
      fecho(AIN, "/data/data/com.feravolt.fdeai/ainame");
    } else {
      strcpy(AIN, "Elizabeth");
      fecho(AIN, "/data/data/com.feravolt.fdeai/ainame");
    }
  };
  long BCAP = 0;
  char batcapa[256];
  FILE *batcap = popen("dumpsys batterystats", "r");
  if (batcap != NULL) {
    while (fgets(batcapa, 256, batcap) != NULL) {
      if (strstr(batcapa, "pacity") != 0) {
        char *bim = strpbrk(batcapa, "0123456789");
        char *bkb = strtok(bim, ":");
        long bel = strtol(bkb, NULL, 10);
        BCAP = bel + 0;
      }
    };
    fclose(batcap);
    memset(batcapa, 0, sizeof(batcapa));
    if (BCAP == 0) {
      BCAP = 1000;
    };
  } else {
    BCAP = 1000;
  };
  FILE *ddens = popen("wm density | /dev/fdeai/busybox tail -n 1", "r");
  static char dodens[128];
  if (ddens != NULL) {
    if (fgets(dodens, (int)sizeof(dodens), ddens) != NULL) {
      dodens[strcspn(dodens, "\r\n")] = 0;
    };
    fclose(ddens);
  };
  char *densit = strpbrk(dodens, "0123456789");
  FILE *dsiz = popen("wm size | /dev/fdeai/busybox cut -d 'x' -f 1 | /dev/fdeai/busybox cut -d ':' -f 2", "r");
  static char dwidth[sizeof(dsiz)];
  if (dsiz != NULL) {
    if (fgets(dwidth, (int)sizeof(dwidth), dsiz) != NULL) {
      dwidth[strcspn(dwidth, "\r\n")] = 0;
    };
    fclose(dsiz);
  };
  FILE *dhe = popen("wm size | /dev/fdeai/busybox cut -d 'x' -f 2", "r");
  static char dheight[sizeof(dhe)];
  if (dhe != NULL) {
    if (fgets(dheight, (int)sizeof(dheight), dhe) != NULL) {
      dheight[strcspn(dheight, "\r\n")] = 0;
    };
    fclose(dhe);
  };
  long disph = strtol(dheight, NULL, 10);
  long dispw = strtol(dwidth, NULL, 10);
  char dsize[128];
  strcat(dwidth, "x");
  strcat(dwidth, dheight);
  strcpy(dsize, dwidth);
  memset(dwidth, 0, sizeof(dwidth));
  memset(dheight, 0, sizeof(dheight));
  FILE *getvendr = popen("getprop ro.hardware", "r");
  char vendr[sizeof(getvendr)];
  if (getvendr != NULL) {
    if (fgets(vendr, (int)sizeof(vendr), getvendr) != NULL) {
      vendr[strcspn(vendr, "\r\n")] = 0;
    };
    pclose(getvendr);
  };
  if (rus == 0) {
    flog("👋 Привет! 🙂 Я ️", AIN, 0);
    flog("Персональный ассистент по оптимизации вашего ", fulldevname, 0);
    flog("Будет здорово если вы купите приложение FDE.AI в маркете, это поможет разработчику сделать меня лучше..", " 🤗", 0);
    flog("Итак..🤔 прошивка: ", RRM, 0);
    flog("Версия ОС Android: ", OSVER, 0);
    flog("Системный язык: ", LANG, 0);
    flog("Метод Root: ", ROOT, 0);
    flog("Режим SElinux: ", selinx, 0);
    flog("Версия ядра: ", KERN, 0);
    flog("Железо: ", vendr, 0);
    flog("Модель ЦП", CPUS, 0);
    flog("Архитектура ЦП: ", ARCH, 0);
    flog("Регулятор: ", cpugovern, 0);
    flogval("..и количество ядер: ", NULL, CORES, 0);
    FILE *fdeloggg;
    if (file_exist("/data/data/com.feravolt.fdeai")) {
      fdeloggg = fopen("/dev/fdeai/fde.txt", "a+");
    } else if (file_exist("/sdcard/Android")) {
      fdeloggg = fopen("/sdcard/Android/fde.txt", "a");
    } else if (file_exist("/storage/emulated/0/Android")) {
      fdeloggg = fopen("/storage/emulated/0/Android/fde.txt", "a");
    } else {
      fdeloggg = fopen("/dev/fde.txt", "a+");
    };
    if (fdeloggg != NULL) {
      fprintf(fdeloggg, "> %s%ld%s\n", "Максимальная частота ЦП: ", MAXfreq, " МГц");
      fprintf(fdeloggg, "> %s%ld%s\n", "Текущая нагрузка ЦП: ", GetCPULoad(), "%");
      fprintf(fdeloggg, "> %s%ld%s\n", "Всего ОЗУ: ", RAM, " Мб");
      fclose(fdeloggg);
    };
    if (freeram() < RAM / 9) {
      flogval("😦 Из них сейчас доступно только ", NULL, freeram(), 0);
    } else if (freeram() > RAM / 3) {
      flogval("👍 Из них сейчас доступно аж ", NULL, freeram(), 0);
    } else {
      flogval("🤔 Из них сейчас доступно примерно ", NULL, freeram(), 0);
    };
    flog("Графика", GGPU, 0);
  } else {
    flog("👋 Hello! 🙂 I'm ", AIN, 0);
    flog("Personal assistant for optimizing your ", fulldevname, 0);
    flog("Please consider buying FDE.AI app, it will really help developer to make me even better..", " 🤗", 0);
    flog("So..🤔 system ROM: ", RRM, 0);
    flog("Android OS version: ", OSVER, 0);
    flog("System language: ", LANG, 0);
    flog("Root method: ", ROOT, 0);
    flog("SElinux mode: ", selinx, 0);
    flog("Kernel version: ", KERN, 0);
    flog("Hardware: ", vendr, 0);
    flog("CPU model", CPUS, 0);
    flog("CPU architecture: ", ARCH, 0);
    flog("Governor: ", cpugovern, 0);
    flogval("..and number of cores: ", NULL, CORES, 0);
    FILE *fdelogggg;
    if (file_exist("/data/data/com.feravolt.fdeai")) {
      fdelogggg = fopen("/dev/fdeai/fde.txt", "a+");
    } else if (file_exist("/sdcard/Android")) {
      fdelogggg = fopen("/sdcard/Android/fde.txt", "a");
    } else if (file_exist("/storage/emulated/0/Android")) {
      fdelogggg = fopen("/storage/emulated/0/Android/fde.txt", "a");
    } else {
      fdelogggg = fopen("/dev/fde.txt", "a+");
    };
    if (fdelogggg != NULL) {
      fprintf(fdelogggg, "> %s%ld%s\n", "Maximum CPU frequency: ", MAXfreq, " MHz");
      fprintf(fdelogggg, "> %s%ld%s\n", "Current CPU load: ", GetCPULoad(), "%");
      fprintf(fdelogggg, "> %s%ld%s\n", "Total RAM: ", RAM, " MB");
      fclose(fdelogggg);
    };
    if (freeram() < RAM / 5) {
      flogval("😦 RAM available now is only ", NULL, freeram(), 0);
    } else if (freeram() > RAM / 2) {
      flogval("👍 RAM available now is ", NULL, freeram(), 0);
    } else {
      flogval("🤔 RAM available now is about ", NULL, freeram(), 0);
    };
    flog("GPU", GGPU, 0);
  };
  FILE *fdeloggggg;
  if (file_exist("/data/data/com.feravolt.fdeai")) {
    fdeloggggg = fopen("/dev/fdeai/fde.txt", "a+");
  } else if (file_exist("/sdcard/Android")) {
    fdeloggggg = fopen("/sdcard/Android/fde.txt", "a");
  } else if (file_exist("/storage/emulated/0/Android")) {
    fdeloggggg = fopen("/storage/emulated/0/Android/fde.txt", "a");
  } else {
    fdeloggggg = fopen("/dev/fde.txt", "a+");
  };
  char FPS[64];
  if (SDK >= 21) {
    long latms = 0;
    FILE *getfps = popen("dumpsys SurfaceFlinger --dispsync", "r");
    if (getfps != NULL) {
      char FPC[512];
      while (fgets(FPC, (int)sizeof(FPC), getfps) != NULL) {
        if (strstr(FPC, "mPeriod:") != 0) {
          char *hemm = strchr(FPC, ':');
          char *hemmf = strstr(FPC, "(");
          hemm[strcspn(hemm, "ns")] = 0;
          hemmf[strcspn(hemmf, "fp")] = 0;
          char *glatd = strpbrk(hemm, "0123456789");
          char *gfps = strpbrk(hemmf, "0123456789.");
          strcpy(FPS, gfps);
          FPS[strcspn(FPS, "\r\n")] = 0;
          long latlong = strtol(glatd, NULL, 10);
          latms = latlong / 1000000;
        };
      };
      pclose(getfps);
      memset(FPC, 0, sizeof(FPC));
    } else {
      strcpy(FPS, "unknown");
    };
    if (fdeloggggg != NULL) {
      if (rus == 0) {
        fprintf(fdeloggggg, "> %s%s%s%ld%s\n", "Частота кадров: ", FPS, "FPS, задержка: ", latms, " мс ⏱");
      } else {
        fprintf(fdeloggggg, "> %s%s%s%ld%s\n", "Frame rate: ", FPS, "FPS, latency: ", latms, " ms ⏱");
      };
    };
  };
  if (fdeloggggg != NULL) {
    if (rus == 0) {
      fprintf(fdeloggggg, "> %s%s%s%s%s\n", "Разрешение экрана и плотность пикселей:", dsize, ", ", densit, "ppi 📱");
      if (BCAP != 1000) {
        fprintf(fdeloggggg, "> %s%ld%s\n", "Ёмкость батареи: ", BCAP, " мАч 🔋");
      };
      fprintf(fdeloggggg, "> %s%ld%s\n", "Температура устройства: ", btmp(), " C🌡");
    } else {
      fprintf(fdeloggggg, "> %s%s%s%s%s\n", "Display size & density:", dsize, ", ", densit, "ppi 📱");
      if (BCAP != 1000) {
        fprintf(fdeloggggg, "> %s%ld%s\n", "Battery capacity: ", BCAP, " mAh 🔋");
      }
      fprintf(fdeloggggg, "> %s%ld%s\n", "Device temperature: ", btmp(), " C🌡");
    };
    fprintf(fdeloggggg, "...\n");
    fclose(fdeloggggg);
  };
  system("touch /dev/fdeai/worked");
  fsetval("1", "/dev/fdeai/worked");
  memset(ROOT, 0, sizeof(ROOT));
  memset(ramm, 0, sizeof(ramm));
  memset(coress, 0, sizeof(coress));
  memset(RRM, 0, sizeof(RRM));
  memset(KERN, 0, sizeof(KERN));
  memset(OSVER, 0, sizeof(OSVER));
  memset(GGPU, 0, sizeof(GGPU));
  memset(CPUS, 0, sizeof(CPUS));
  memset(fulldevname, 0, sizeof(fulldevname));
  memset(selinx, 0, sizeof(selinx));
  memset(batcapa, 0, sizeof(batcapa));
  memset(dodens, 0, sizeof(dodens));
  memset(dsize, 0, sizeof(dsize));
  memset(dwidth, 0, sizeof(dwidth));
  memset(FPS, 0, sizeof(FPS));
  memset(ARCH, 0, sizeof(ARCH));
  if (file_exist("/sbin/.magisk/modules/MAGNETAR")) {
    if (rus == 0) {
      flog("Обнаружен MAGNETAR. ", "Пожалуйста, удалите его прежде чем использовать FDE.AI...А пока я всё. 😕", 0);
    } else {
      flog("MAGNETAR detected. ", "Please uninstall it before running FDE.AI...Until that I'm done. 😕", 0);
    };
    frm(VER);
    sleep(1);
    exit(0);
  };
  if (file_exist("/sbin/.magisk/modules/glitchify")) {
    if (rus == 0) {
      flog("Обнаружен Glitchify. ", "Пожалуйста, удалите его прежде чем использовать FDE.AI...А пока я всё. 😕", 0);
    } else {
      flog("Glitchify detected. ", "Please uninstall it before running FDE.AI...Until that I'm done. 😕", 0);
    };
    frm(VER);
    sleep(1);
    exit(0);
  };
  if (file_exist("/dev/fmounted") == 0) {
    if (file_exist("/sbin/.magisk/modules/legendary_kernel_tweaks")) {
      if (rus == 0) {
        flog("Обнаружен LKT️. ", "Активирую в нём профиль 'battery' и перезаписываю свои настройки... 🙃", 0);
      } else {
        flog("LKT detected. ", "Setting it's profile to 'battery' & overriding my settings... 🙃", 0);
      };
      sleep(18);
      system("/dev/fdeai/busybox killall -q lkt >/dev/null 2>&1&");
      system("sh /sbin/.magisk/modules/legendary_kernel_tweaks/service.sh '0' '0.1' >/dev/null 2>&1");
      system("/dev/fdeai/busybox killall -q lkt >/dev/null 2>&1&");
    };
    if (file_exist("/sbin/.magisk/modules/zram_config")) {
      if (rus == 0) {
        flog("Обнаружен Memory/zRAM/SWAP optimizer. ", "Ждём минуту чтобы переписать свои настройки... 🙃", 0);
      } else {
        flog("Memory/zRAM/SWAP optimizer detected. ", "Waiting one minute to override my settings... 🙃", 0);
      };
      sleep(60);
    };
    if (file_exist("/sbin/.magisk/modules/PIXELARITY")) {
      if (rus == 0) {
        flog("Обнаружен PIXELARITY. ", "Ждём 2 минуты чтобы переписать свои настройки... 🙃", 0);
      } else {
        flog("PIXELARITY detected. ", "Waiting 2 minutes to override my settings... 🙃", 0);
      };
      sleep(120);
    };
    if (file_exist("/sbin/.magisk/modules/nfsinjector")) {
      if (rus == 0) {
        flog("Обнаружен NFS️ injector. ", "Ждём немного чтобы переписать свои настройки... 🙃", 0);
      } else {
        flog("NFS️ injector detected. ", "Waiting some time to override my settings... 🙃", 0);
      };
      sleep(120);
    };
    if (file_exist("/sbin/.magisk/modules/lspeed") || file_exist("/data/data/com.paget96.lspeedandroid") || file_exist("/data/data/com.paget96.lspeed")) {
      if (rus == 0) {
        flog("Обнаружен LSPEED. ", "Ждём немного чтобы переписать свои настройки... 🙃", 0);
      } else {
        flog("LSPEED detected. ", "Waiting some time to override my settings... 🙃", 0);
      };
      sleep(96);
      system("am force-stop com.paget96.lspeed >/dev/null 2>&1;am force-stop com.paget96.lspeedandroid >/dev/null 2>&1&");
    };
  };
  system("/dev/fdeai/busybox killall -q service.sh >/dev/null 2>&1&");
  FILE *getboard = popen("getprop ro.product.name", "r");
  static char boards[64];
  if (getboard != NULL) {
    if (fgets(boards, (int)sizeof(boards), getboard) != NULL) {
      boards[strcspn(boards, "\r\n")] = 0;
    };
    pclose(getboard);
  };
  int kpro = strcmp(boards, "raphael");
  int mi9t = strcmp(boards, "davinci");
  if (rus == 0) {
    flog("⚙️...начинаю оптимизацию системы...️", "⚙️", 1);
  } else {
    flog("⚙️...starting to optimize system...", "⚙️", 1);
  };
  if (file_exist("/dev/fmounted") == 0) {
    if (strstr(vendr, "qcom") != 0) {
      chmod("/data/data/com.feravolt.fdeai/qcom.bin", 0775);
    };
    if (kpro != 0 && mi9t != 0) {
      if (rus == 0) {
        flog("Оптимзирую параметры файловой систем️ы", "...️", 1);
      } else {
        flog("Optimizing file system options", "...", 1);
      };
      system("for x in $(/dev/fdeai/busybox cat /proc/mounts|/dev/fdeai/busybox grep ext4|/dev/fdeai/busybox cut -d ' ' -f2);do /dev/fdeai/busybox mount -o remount,defaults,noatime,delalloc,noauto_da_alloc,discard,init_itable=90,max_batch_time=33000,errors=remount-ro,commit=45 ${x};done >/dev/null 2>&1 & for gf in $(/dev/fdeai/busybox cat /proc/mounts|/dev/fdeai/busybox grep f2fs|/dev/fdeai/busybox cut -d ' ' -f2);do /dev/fdeai/busybox mount -o remount,defaults,noatime,discard,nobarrier ${gf};done >/dev/null 2>&1 &");
      fecho("0", "/sys/module/ext4/parameters/mballoc_debug");
      if (file_exist("/sys/kernel/debug/ufshcd0")) {
        fecho("1", "/sys/kernel/debug/ufshcd0/reset_controller");
      };
    };
    if (file_exist("/sbin/magisk")) {
      system("(resetprop ro.HOME_APP_ADJ 1;resetprop persist.radio.ramdump 0;resetprop pm.sleep_mode 1;resetprop persist.wpa_supplicant.debug false;resetprop dalvik.vm.checkjni false;resetprop dalvik.vm.check-dex-sum false;resetprop dalvik.vm.debug.alloc 0;resetprop dalvik.vm.deadlock-predict off;resetprop dalvik.vm.verify-bytecode false;resetprop libc.debug.malloc 0;resetprop debug.atrace.tags.enableflags 0;resetprop vidc.debug.level 0;resetprop debug.mdpcomp.logs 0;resetprop logd.logpersistd.enable false;resetprop ro.kernel.android.checkjni 0;resetprop touch.pressure.scale 0.5;resetprop ro.config.nocheckin 1;resetprop ro.kernel.checkjni 0;resetprop profiler.launch false;resetprop profiler.force_disable_err_rpt 1;resetprop profiler.force_disable_ulog 1;resetprop profiler.debugmonitor false;resetprop profiler.hung.dumpdobugreport false;resetprop debugtool.anrhistory 0;resetprop video.accelerate.hw 1;resetprop trustkernel.log.state disable;resetprop hwui.render_dirty_regions false;resetprop debug.hwui.render_dirty_regions false;resetprop persist.sys.ssr.enable_ramdumps 0;resetprop persist.vendor.sys.ssr.enable_ramdumps 0;resetprop persist.traced.enable 0;resetprop ro.lmk.log_stats 0;resetprop debug.egl.hw 1;) >/dev/null 2>&1 &");
    } else {
      system("(setprop ro.HOME_APP_ADJ 1;setprop persist.radio.ramdump 0;setprop pm.sleep_mode 1;setprop persist.wpa_supplicant.debug false;setprop dalvik.vm.checkjni false;setprop dalvik.vm.check-dex-sum false;setprop dalvik.vm.debug.alloc 0;setprop dalvik.vm.deadlock-predict off;setprop dalvik.vm.verify-bytecode false;setprop libc.debug.malloc 0;setprop debug.atrace.tags.enableflags 0;setprop vidc.debug.level 0;setprop debug.mdpcomp.logs 0;setprop logd.logpersistd.enable false;setprop ro.kernel.android.checkjni 0;setprop touch.pressure.scale 0.5;setprop ro.config.nocheckin 1;setprop ro.kernel.checkjni 0;setprop profiler.launch false;setprop profiler.force_disable_err_rpt 1;setprop profiler.force_disable_ulog 1;setprop profiler.debugmonitor false;setprop profiler.hung.dumpdobugreport false;setprop debugtool.anrhistory 0;setprop video.accelerate.hw 1;setprop trustkernel.log.state disable;setprop hwui.render_dirty_regions false;setprop debug.hwui.render_dirty_regions false;setprop persist.sys.ssr.enable_ramdumps 0;setprop persist.vendor.sys.ssr.enable_ramdumps 0;setprop persist.traced.enable 0;setprop ro.lmk.log_stats 0;setprop debug.egl.hw 1;) >/dev/null 2>&1 &");
    };
    if (SDK <= 20) {
      if (file_exist("/sbin/magisk")) {
        system("(resetprop media.stagefright.enable-http false;resetprop media.stagefright.enable-qcp false;resetprop media.stagefright.enable-fma2dp false;resetprop ro.ril.disable.power.collapse 0;resetprop ro.telephony.call_ring.delay 0;resetprop ro.ril.fast.dormancy.rule 0;resetprop ro.ril.gprsclass 12;resetprop ro.ril.enable.a53 1;resetprop ro.ril.hep 1;resetprop MIN_HIDDEN_APPS false;resetprop MIN_RECENT_TASKS false;resetprop APP_SWITCH_DELAY_TIME false;resetprop ro.max.fling_velocity 18000;resetprop ro.min.fling_velocity 900;resetprop dalvik.vm.dexopt-flags m=y,v=n,o=v;) >/dev/null 2>&1 &");
        if (MAXfreq >= 2400) {
          system("resetprop windowsmgr.max_events_per_sec 240;");
        } else {
          system("resetprop windowsmgr.max_events_per_sec 120;");
        };
      } else {
        system("(setprop media.stagefright.enable-http false;setprop media.stagefright.enable-qcp false;setprop media.stagefright.enable-fma2dp false;setprop ro.ril.disable.power.collapse 0;setprop ro.telephony.call_ring.delay 0;setprop ro.ril.fast.dormancy.rule 0;setprop ro.ril.gprsclass 12;setprop ro.ril.enable.a53 1;setprop ro.ril.hep 1;setprop MIN_HIDDEN_APPS false;setprop MIN_RECENT_TASKS false;setprop APP_SWITCH_DELAY_TIME false;setprop ro.max.fling_velocity 18000;setprop ro.min.fling_velocity 900;setprop dalvik.vm.dexopt-flags m=y,v=n,o=v;) >/dev/null 2>&1 &");
        if (MAXfreq >= 2400) {
          system("setprop windowsmgr.max_events_per_sec 240;");
        } else {
          system("setprop windowsmgr.max_events_per_sec 120;");
        };
      };
    };
    if (SDK <= 22) {
      if (file_exist("/sbin/magisk")) {
        system("(resetprop persist.sys.ui.hw 1;resetprop persist.sys.use_dithering 0;resetprop wifi.supplicant_scan_interval 180;resetprop dev.pm.dyn_samplingrate 1;resetprop debug.sf.hw 1;resetprop debug.gr.swapinterval 1;resetprop debug.gr.numframebuffers 3;resetprop ro.config.disable.hw_accel false;resetprop ro.floatingtouch.available 1;resetprop ro.audio.flinger_standbytime_ms 300;resetprop ro.media.enc.jpeg.quality 100;) >/dev/null 2>&1 &");
      } else {
        system("(setprop persist.sys.ui.hw 1;setprop persist.sys.use_dithering 0;setprop wifi.supplicant_scan_interval 180;setprop dev.pm.dyn_samplingrate 1;setprop debug.sf.hw 1;setprop debug.gr.swapinterval 1;setprop debug.gr.numframebuffers 3;setprop ro.config.disable.hw_accel false;setprop ro.floatingtouch.available 1;setprop ro.audio.flinger_standbytime_ms 300;setprop ro.media.enc.jpeg.quality 100;) >/dev/null 2>&1 &");
      };
    };
    if (SDK >= 21) {
      if (file_exist("/sbin/magisk")) {
        system("(resetprop dalvik.vm.dex2oat-filter speed;resetprop dalvik.vm.dex2oat-minidebuginfo false;resetprop sys.sysctl.tcp_def_init_rwnd 60;resetprop sys.display-size 3840x2160;resetprop dalvik.vm.dex2oat-flags --no-watch-dog;resetprop dalvik.vm.image-dex2oat-filter --no-watch-dog;) >/dev/null 2>&1 &");
      } else {
        system("(setprop dalvik.vm.dex2oat-filter speed;setprop dalvik.vm.dex2oat-minidebuginfo false;setprop sys.sysctl.tcp_def_init_rwnd 60;setprop sys.display-size 3840x2160;setprop dalvik.vm.dex2oat-flags --no-watch-dog;setprop dalvik.vm.image-dex2oat-filter --no-watch-dog;) >/dev/null 2>&1 &");
      };
    };
    if (SDK >= 24) {
      if (file_exist("/sbin/magisk")) {
        system("(resetprop pm.dexopt.bg-dexopt speed;resetprop pm.dexopt.shared speed;resetprop dalvik.vm.usejit true;resetprop vidc.debug.perf.mode 2;resetprop mm.enable.smoothstreaming true;resetprop ro.boot.warranty_bit 0;resetprop ro.warranty_bit 0;resetprop ro.config.hw_quickpoweron true;resetprop doze.pulse.notifications true;resetprop debug.egl.buffcount 4;resetprop debug.sf.disable_backpressure 1;resetprop ro.sf.disable_triple_buffer 0;resetprop debug.sf.latch_unsignaled 1;resetprop debug.hwui.use_buffer_age false;) >/dev/null 2>&1 &");
      } else {
        system("(setprop pm.dexopt.bg-dexopt speed;setprop pm.dexopt.shared speed;setprop dalvik.vm.usejit true;setprop vidc.debug.perf.mode 2;setprop mm.enable.smoothstreaming true;setprop ro.boot.warranty_bit 0;setprop ro.warranty_bit 0;setprop ro.config.hw_quickpoweron true;setprop doze.pulse.notifications true;setprop debug.egl.buffcount 4;setprop debug.sf.disable_backpressure 1;setprop ro.sf.disable_triple_buffer 0;setprop debug.sf.latch_unsignaled 1;setprop debug.hwui.use_buffer_age false;) >/dev/null 2>&1 &");
      };
    };
  };
  memset(vendr, 0, sizeof(vendr));
  static char ON[] = "1";
  static char OFF[] = "0";
  static char YES[] = "Y";
  static char NO[] = "N";
  if (file_exist("/sys/module/msm_performance/parameters/touchboost")) {
    fsetval(OFF, "/sys/module/msm_performance/parameters/touchboost");
    if (rus == 0) {
      flog("Отключено ускорение ЦП при касании️", ".️", 1);
    } else {
      flog("Touch boost deactivated", ".", 1);
    };
  } else if (file_exist("/sys/power/pnpmgr/touch_boost")) {
    fsetval(OFF, "/sys/power/pnpmgr/touch_boost");
    if (rus == 0) {
      flog("Отключено ускорение ЦП при касании️", ".️", 1);
    } else {
      flog("Touch boost deactivated", ".", 1);
    };
  };
  if (file_exist("/sys/module/cpu_input_boost/parameters/wake_boost_duration")) {
    fsetval("250", "/sys/module/cpu_input_boost/parameters/wake_boost_duration");
    if (rus == 0) {
      flog("Активировано ускорение при включении экрана️", ".️", 1);
    } else {
      flog("Device wakeup boost activated", ".", 1);
    };
  } else if (file_exist("/sys/module/cpu_boost/parameters/wakeup_boost")) {
    fsetval(ON, "/sys/module/cpu_boost/parameters/wakeup_boost");
    if (rus == 0) {
      flog("Активировано ускорение при включении экрана️", ".️", 1);
    } else {
      flog("Device wakeup boost activated", ".", 1);
    };
  };
  if (file_exist("/sys/module/cpu_boost/parameters/powerkey_input_boost_ms")) {
    fsetval("250", "/sys/module/cpu_boost/parameters/powerkey_input_boost_ms");
    if (rus == 0) {
      flog("Активировано ускорение при нажатии кнопки питания️", ".️", 1);
    } else {
      flog("Power-key input boost activated", ".", 1);
    };
  };
  if (file_exist("/sys/module/cpu_boost/parameters/sched_boost_on_input")) {
    fsetval(NO, "/sys/module/cpu_boost/parameters/sched_boost_on_input");
    if (rus == 0) {
      flog("Отключено ускорение планировщика при вводе️", ".️", 1);
    } else {
      flog("Scheduler input boost deactivated", ".", 1);
    };
  };
  if (file_exist("/sys/module/cpu_boost/parameters/sched_boost_on_powerkey_input")) {
    fsetval(YES, "/sys/module/cpu_boost/parameters/sched_boost_on_powerkey_input");
    if (rus == 0) {
      flog("Активировано ускорение планировщика при нажатии кнопки питания️", ".️", 1);
    } else {
      flog("Power-key sheduler input boost activated", ".", 1);
    };
  };
  fecho(OFF, "/sys/module/cpu_boost/parameters/boost_ms");
  fsetval(OFF, "/sys/module/cpu_boost/parameters/cpu_boost");
  fsetval(NO, "/sys/module/cpu_boost/parameters/cpuboost_enable");
  fsetval(OFF, "/sys/module/cpu_boost/parameters/input_boost_ms_s2");
  fecho("150", "/sys/module/cpu_boost/parameters/min_input_interval");
  long ibs = BCAP / 66;
  if (file_exist("/sys/module/cpu_boost/parameters/input_boost_ms")) {
    fecho(ON, "/sys/module/cpu_boost/parameters/input_boost_enabled");
    chmod("/sys/module/cpu_boost/parameters/input_boost_ms", 0644);
    fecholong(ibs, "/sys/module/cpu_boost/parameters/input_boost_ms");
    chmod("/sys/module/cpu_boost/parameters/input_boost_ms", 0444);
    if (rus == 0) {
      flog("Оптимизировано ускорение ввода️", ".️", 1);
    } else {
      flog("Input boost optimized", ".", 1);
    };
  } else if (file_exist("/sys/module/cpu_input_boost/parameters/input_boost_duration")) {
    fecho(ON, "/sys/kernel/cpu_input_boost/enabled");
    chmod("/sys/module/cpu_input_boost/parameters/input_boost_duration", 0644);
    fecholong(ibs, "/sys/module/cpu_input_boost/parameters/input_boost_duration");
    chmod("/sys/module/cpu_input_boost/parameters/input_boost_duration", 0444);
    if (rus == 0) {
      flog("Оптимизировано ускорение ввода️", ".️", 1);
    } else {
      flog("Input boost optimized", ".", 1);
    };
  };
  if (file_exist("/sys/kernel/fp_boost/enabled")) {
    fsetval(ON, "/sys/kernel/fp_boost/enabled");
    if (rus == 0) {
      flog("Ускорение сканера отпечатков️", ".️", 1);
    } else {
      flog("Fingerprint boost activated", ".", 1);
    };
  };
  if (file_exist("/sys/module/cpu_boost/parameters/hotplug_boost")) {
    fsetval(NO, "/sys/module/cpu_boost/parameters/hotplug_boost");
    if (rus == 0) {
      flog("Отключено ускорение ЦП при подключении ядра️", ".️", 1);
    } else {
      flog("CPU core hotplug boost deactivated", ".", 1);
    };
  };
  if (file_exist("/sys/module/cpu_boost/parameters/dynamic_stune_boost")) {
    if (BCAP > 4000) {
      fecho("6", "/sys/module/cpu_boost/parameters/dynamic_stune_boost");
    } else {
      fecho("3", "/sys/module/cpu_boost/parameters/dynamic_stune_boost");
    }
    if (rus == 0) {
      flog("Оптимизировано динамическое ускорение EAS️", ".️", 1);
    } else {
      flog("EAS dynamic boost optimized", ".", 1);
    };
  };
  if (file_exist("/sys/class/lcd/panel/power_reduce")) {
    fsetval(ON, "/sys/class/lcd/panel/power_reduce");
    if (rus == 0) {
      flog("Уменьшение энергопотребление дисплея️", ".️", 1);
    } else {
      flog("Reducing LCD power consumption", ".", 1);
    };
  };
  if (file_exist("/sys/devices/system/cpu/sched_mc_power_savings")) {
    fsetval(OFF, "/sys/devices/system/cpu/sched_mc_power_savings");
    if (rus == 0) {
      flog("Оптимизация многоядерного ЦП️", ".️", 1);
    } else {
      flog("Multi-core CPU optimization", ".", 1);
    };
  };
  if (rus == 0) {
    flog("Оптимизация вейклоков ядра️", "..️.", 1);
  } else {
    flog("Kernel wakelocks optimization", "...", 1);
  };
  fecho(NO, "/sys/module/wakeup/parameters/enable_msm_hsic_ws");
  fecho(NO, "/sys/module/wakeup/parameters/enable_si_ws");
  fecho(NO, "/sys/module/wakeup/parameters/enable_si_wsk");
  fecho(NO, "/sys/module/wakeup/parameters/enable_ipa_ws");
  fecho(NO, "/sys/module/wakeup/parameters/enable_qcom_rx_wakelock_ws");
  fecho(NO, "/sys/module/wakeup/parameters/enable_wlan_rx_wake_ws");
  fecho(NO, "/sys/module/wakeup/parameters/enable_wlan_ipa_ws");
  fecho(NO, "/sys/module/wakeup/parameters/enable_wlan_pno_wl_ws");
  fecho(NO, "/sys/module/wakeup/parameters/enable_wcnss_filter_lock_ws");
  fecho(NO, "/sys/module/wakeup/parameters/enable_smb135x_wake_ws");
  fecho("qcom_rx_wakelock;", "/sys/class/misc/boeffla_wakelock_blocker/wakelock_blocker");
  if (file_exist("/sys/devices/virtual/sec/sec_touchscreen/tsp_threshold")) {
    fecho("50", "/sys/devices/virtual/sec/sec_touchscreen/tsp_threshold");
    if (rus == 0) {
      flog("Оптимизирована чувствительность сенсора экрана️", ".️", 1);
    } else {
      flog("Touch-screen sensivity optimized", ".", 1);
    };
  } else if (file_exist("/sys/class/touch/switch/set_touchscreen")) {
    fecho("14005", "/sys/class/touch/switch/set_touchscreen");
    if (rus == 0) {
      flog("Оптимизирована чувствительность сенсора экрана️", ".️", 1);
    } else {
      flog("Touch-screen sensivity optimized", ".", 1);
    };
  };
  if (file_exist("/sys/devices/platform/galcore/gpu/gpu0/gpufreq/scaling_governor")) {
    fecho("conservative", "/sys/devices/platform/galcore/gpu/gpu0/gpufreq/scaling_governor");
    fsetval("5", "/sys/devices/platform/galcore/gpu/gpu0/gpufreq/conservative/freq_step");
    fsetval("20", "/sys/devices/platform/galcore/gpu/gpu0/gpufreq/conservative/down_threshold");
    fsetval("45", "/sys/devices/platform/galcore/gpu/gpu0/gpufreq/conservative/up_threshold");
    fsetval("9", "/sys/devices/platform/galcore/gpu/gpu0/gpufreq/conservative/sampling_down_factor");
    char viva[] = "/sys/class/devfreq/devfreq-vpu/max_freq";
    fsetval(fcat(viva), "/sys/class/devfreq/devfreq-vpu/userspace/set_freq");
    memset(viva, 0, sizeof(viva));
    if (rus == 0) {
      flog("Оптимизация частот видеоускорителя Vivante️", ".️", 1);
    } else {
      flog("Vivante GPU frequencies optimization", ".", 1);
    };
  };
  if (file_exist("/sys/devices/platform/kgsl/msm_kgsl/kgsl-3d0/io_fraction")) {
    fecho("50", "/sys/devices/platform/kgsl/msm_kgsl/kgsl-3d0/io_fraction");
    if (rus == 0) {
      flog("Оптимизация I/O ускорителя Adreno", ".️", 1);
    } else {
      flog("Adreno GPU I/O optimization", ".", 1);
    };
  };
  if (file_exist("/sys/class/kgsl/kgsl-3d0/max_pwrlevel")) {
    if (rus == 0) {
      flog("Оптимизация частот видеоускорителя Adreno", ".️..", 1);
    } else {
      flog("Adreno GPU frequencies optimization", "...", 1);
    };
    fecho(YES, "/sys/class/kgsl/kgsl-3d0/popp");
    fsetval(OFF, "/sys/class/kgsl/kgsl-3d0/max_pwrlevel");
    fsetval(ON, "/sys/class/kgsl/kgsl-3d0/bus_split");
    fsetval(OFF, "/sys/class/kgsl/kgsl-3d0/force_bus_on");
    fsetval(OFF, "/sys/class/kgsl/kgsl-3d0/force_rail_on");
    fsetval(OFF, "/sys/class/kgsl/kgsl-3d0/force_clk_on");
    fecho("64", "/sys/class/kgsl/kgsl-3d0/idle_timer");
    fecho(OFF, "/sys/class/kgsl/kgsl-3d0/throttling");
    if (file_exist("/sys/class/kgsl/kgsl-3d0/default_pwrlevel")) {
      char minadfre[] = "/sys/class/kgsl/kgsl-3d0/devfreq/min_freq";
      long minadfreq = strtol(fcat(minadfre), NULL, 10);
      char minpwlvl[] = "/sys/class/kgsl/kgsl-3d0/min_pwrlevel";
      if (minadfreq >= 160000000) {
        fsetval(fcat(minpwlvl), "/sys/class/kgsl/kgsl-3d0/default_pwrlevel");
      };
      memset(minpwlvl, 0, sizeof(minpwlvl));
      memset(minadfre, 0, sizeof(minadfre));
    };
  };
  if (file_exist("/sys/module/adreno_idler/parameters/adreno_idler_active")) {
    fecho(YES, "/sys/module/adreno_idler/parameters/adreno_idler_active");
    fecho("5000", "/sys/module/adreno_idler/parameters/adreno_idler_idleworkload");
    fecho("16", "/sys/module/adreno_idler/parameters/adreno_idler_downdifferential");
    fecho("27", "/sys/module/adreno_idler/parameters/adreno_idler_idlewait");
    if (rus == 0) {
      flog("Активирован и оптимизирован Adreno idler", ".️", 1);
    } else {
      flog("Adreno Idler activated & optimized", ".", 1);
    };
  };
  if (file_exist("/sys/module/mali/parameters/mali_debug_level")) {
    system("chmod 664 /sys/module/mali/parameters/*");
    fecho(OFF, "/sys/module/mali/parameters/mali_debug_level");
    if (rus == 0) {
      flog("Отключена отладка видеоускорителя Mali", ".️", 1);
    } else {
      flog("Mali GPU debugging dectivated", ".", 1);
    };
  };
  if (file_exist("/sys/module/mali/parameters/mali_gpu_utilization_timeout")) {
    fecho("250", "/sys/module/mali/parameters/mali_gpu_utilization_timeout");
    if (rus == 0) {
      flog("Оптимизация видеоускорителя Mali", "...️", 1);
    } else {
      flog("Mali GPU optimization", "...", 1);
    };
  };
  if (file_exist("/proc/mali/dvfs_enable")) {
    system("chmod 664 /proc/mali/*");
    fecho(ON, "/proc/mali/dvfs_enable");
    if (rus == 0) {
      flog("Активация DVFS для видеоускорителя Mali", ".️", 1);
    } else {
      flog("Mali GPU DVFS activated", ".", 1);
    };
  };
  if (file_exist("/sys/module/mali/parameters/mali_touch_boost_level")) {
    if (BCAP >= 3500) {
      fsetval(ON, "/sys/module/mali/parameters/mali_touch_boost_level");
    } else {
      fsetval(OFF, "/sys/module/mali/parameters/mali_touch_boost_level");
    }
    if (rus == 0) {
      flog("Оптимизировано ускорение ввода для видеоускорителя Mali", ".️", 1);
    } else {
      flog("Mali GPU touch boost activated", ".", 1);
    };
  } else if (file_exist("/proc/gpufreq/gpufreq_input_boost")) {
    if (BCAP >= 3500) {
      fsetval(ON, "/proc/gpufreq/gpufreq_input_boost");
    } else {
      fsetval(OFF, "/proc/gpufreq/gpufreq_input_boost");
    }
    if (rus == 0) {
      flog("Оптимизировано ускорение ввода для видеоускорителя Mali", ".️", 1);
    } else {
      flog("Mali GPU touch boost activated", ".", 1);
    };
  };
  if (file_exist("/d/mdss_panel_fb0/intf0/min_refresh_rate")) {
    if (rus == 0) {
      flog("Оптимизация частоты кадров", "...️", 1);
    } else {
      flog("FPS optimization", "...", 1);
    };
    fecho("56", "/d/mdss_panel_fb0/intf0/min_refresh_rate");
  } else if (file_exist("/sys/kernel/debug/msm_fb/0/vsync_enable")) {
    if (rus == 0) {
      flog("Оптимизация частоты кадров", "...️", 1);
    } else {
      flog("FPS optimization", "...", 1);
    };
    fsetval(OFF, "/sys/kernel/debug/msm_fb/0/vsync_enable");
    fsetval("16", "/sys/kernel/debug/msm_fb/mdp/mdp_usec_diff_treshold");
    fsetval("40", "/sys/kernel/debug/msm_fb/mdp/vs_rdcnt_slow");
  };
  if (file_exist("/sys/module/cryptomgr/parameters/notests")) {
    fsetval(YES, "/sys/module/cryptomgr/parameters/notests");
    if (rus == 0) {
      flog("Отключены принудительные тесты криптографии", ".️", 1);
    } else {
      flog("Forced cryptography tests deactivated", ".", 1);
    };
  };
  fecho(ON, "/proc/sys/net/ipv4/tcp_sack");
  fecho(ON, "/proc/sys/net/ipv4/tcp_fack");
  fecho(ON, "/proc/sys/net/ipv4/tcp_dsack");
  fecho(ON, "/proc/sys/net/ipv4/tcp_tw_reuse");
  fecho(OFF, "/proc/sys/net/ipv4/tcp_delayed_ack");
  fecho(OFF, "/proc/sys/net/ipv4/tcp_tw_recycle");
  fecho(ON, "/proc/sys/net/ipv4/tcp_timestamps");
  fecho(ON, "/proc/sys/net/ipv4/tcp_window_scaling");
  fecho("3", "/proc/sys/net/ipv4/tcp_synack_retries");
  fecho("240", "/proc/sys/net/ipv4/tcp_keepalive_intvl");
  fecho("5", "/proc/sys/net/ipv4/tcp_keepalive_probes");
  fecho("45", "/proc/sys/net/ipv4/tcp_fin_timeout");
  fecho("9999", "/proc/sys/net/ipv4/tcp_challenge_ack_limit");
  fecho(ON, "/proc/sys/net/ipv4/tcp_no_metrics_save");
  fecho(OFF, "/proc/sys/net/ipv4/tcp_slow_start_after_idle");
  fecho("3", "/proc/sys/net/ipv4/tcp_fastopen");
  fecho("60", "/proc/sys/net/ipv4/tcp_default_init_rwnd");
  fecho("3600", "/proc/sys/net/core/xfrm_acq_expires");
  fecho("cubic", "/proc/sys/net/ipv4/tcp_congestion_control");
  fecho("htcp", "/proc/sys/net/ipv4/tcp_congestion_control");
  fecho("westwood", "/proc/sys/net/ipv4/tcp_congestion_control");
  fecho("bbr", "/proc/sys/net/ipv4/tcp_congestion_control");
  fecho("2", "/sys/module/tcp_cubic/parameters/hystart_detect");
  if (rus == 0) {
    flog("Оптимизирован сетевой стек ядра", ".️", 1);
  } else {
    flog("Optimized kernel network stack", ".", 1);
  };
  if (SDK >= 23) {
    system("(pm disable --user 0 com.google.android.gms/.phenotype.service.sync.PhenotypeConfigurator;settings put secure location_providers_allowed ' ';) >/dev/null 2>&1 &");
    system("dumpsys deviceidle enable all");
    if (SDK >= 24) {
      system("(settings put global device_idle_constants inactive_to=60000,sensing_to=0,locating_to=0,location_accuracy=2000,motion_inactive_to=0,idle_after_inactive_to=0,idle_pending_to=60000,max_idle_pending_to=120000,idle_pending_factor=2.0,idle_to=900000,max_idle_to=21600000,idle_factor=2.0,max_temp_app_whitelist_duration=60000,mms_temp_app_whitelist_duration=30000,sms_temp_app_whitelist_duration=20000,light_after_inactive_to=10000,light_pre_idle_to=60000,light_idle_to=180000,light_idle_factor=2.0,light_max_idle_to=900000,light_idle_maintenance_min_budget=30000,light_idle_maintenance_max_budget=60000;) >/dev/null 2>&1 &");
    } else {
      system("(settings put global device_idle_constants inactive_to=60000,sensing_to=0,locating_to=0,location_accuracy=2000,motion_inactive_to=0,idle_after_inactive_to=0,idle_pending_to=60000,max_idle_pending_to=120000,idle_pending_factor=2.0,idle_to=900000,max_idle_to=21600000,idle_factor=2.0,max_temp_app_whitelist_duration=60000,mms_temp_app_whitelist_duration=30000,sms_temp_app_whitelist_duration=20000;) >/dev/null 2>&1 &");
    };
    if (rus == 0) {
      flog("Оптимизированы параметры Doze", ".️", 1);
    } else {
      flog("Doze parameters optimized", ".", 1);
    };
  };
  if (rus == 0) {
    flog("Провожу чистку разного системного мусора", "...️", 1);
  } else {
    flog("Cleaning various system trash files", "...", 1);
  };
  system("(rm -f /data/*.log;rm -f /data/*.txt;rm -f /data/anr/*;rm -f /data/backup/pending/*.tmp;rm -f /data/cache/*.*;rm -f /data/data/*.log;rm -f /data/data/*.txt;rm -f /data/log/*.log;rm -f /data/log/*.txt;rm -f /data/local/*.apk;rm -f /data/local/*.log;rm -f /data/local/*.txt;rm -f /data/last_alog/*.log;rm -f /data/last_alog/*.txt;rm -f /data/last_kmsg/*.log;rm -f /data/last_kmsg/*.txt;rm -f /data/mlog/*;rm -f /data/system/*.log;rm -f /data/system/*.txt;rm -f /data/system/dropbox/* >/dev/null 2>&1 & rm -Rf /data/system/usagestats/*;rm -f /data/system/shared_prefs/*;rm -f /data/tombstones/* >/dev/null 2>&1 & rm -Rf /sdcard/LOST.DIR >/dev/null 2>&1 & rm -Rf /sdcard/found000 >/dev/null 2>&1 & rm -Rf /sdcard/LazyList >/dev/null 2>&1 & rm -Rf /sdcard/albumthumbs >/dev/null 2>&1 & rm -Rf /sdcard/kunlun >/dev/null 2>&1 & rm -Rf /sdcard/.CacheOfEUI >/dev/null 2>&1 & rm -Rf /sdcard/.bstats >/dev/null 2>&1 & rm -Rf /sdcard/.taobao >/dev/null 2>&1 & rm -Rf /sdcard/Backucup >/dev/null 2>&1 & rm -Rf /sdcard/MIUI/debug_log >/dev/null 2>&1 & rm -Rf /sdcard/wlan_logs >/dev/null 2>&1 & rm -Rf /sdcard/ramdump >/dev/null 2>&1 & rm -Rf /sdcard/UnityAdsVideoCache;rm -f /sdcard/*.log;rm -f /sdcard/*.CHK;) >/dev/null 2>&1 &");
  if (kpro != 0 && mi9t != 0) {
    if (file_exist("/sys/module/mmc_core/parameters/crc")) {
      fecho(OFF, "/sys/module/mmc_core/parameters/crc");
      fecho(OFF, "/sys/module/mmc_core/parameters/removable");
      if (rus == 0) {
        flog("Оптимизация MMC памяти", "...️", 1);
      } else {
        flog("MMC NAND optimization", "...", 1);
      };
    } else if (file_exist("/sys/module/mmc_core/parameters/use_spi_crc")) {
      fecho(OFF, "/sys/module/mmc_core/parameters/use_spi_crc");
      fecho(OFF, "/sys/module/mmc_core/parameters/removable");
      if (rus == 0) {
        flog("Оптимизация MMC памяти", "...️", 1);
      } else {
        flog("MMC NAND optimization", "...", 1);
      };
    };
  };
  if (kpro != 0 && mi9t != 0) {
    if (rus == 0) {
      flog("Оптимизация операций I/O для всех разделов", "...️", 1);
    } else {
      flog("I/O optimization for all partitions", "...", 1);
    };
    if (RAM >= 3600) {
      system("for h in /sys/fs/ext4/*;do /dev/fdeai/busybox echo '64'>${h}/inode_readahead_blks;done >/dev/null 2>&1 &");
      system("for ma in /sys/devices/virtual/bdi/179*;do /dev/fdeai/busybox echo '1024'>${ma}/read_ahead_kb;done >/dev/null 2>&1 &");
      system("if [ -d /sys/block/sda ];then for sd in a b c d e f g;do /dev/fdeai/busybox echo '1024'>/sys/block/sd${sd}/queue/read_ahead_kb;done >/dev/null 2>&1 fi;if [ -d /sys/block/mmcblk0 ];then /dev/fdeai/busybox echo '1024'>/sys/block/mmcblk0/queue/read_ahead_kb;fi;if [ -d /sys/block/mtdblock0 ];then /dev/fdeai/busybox echo '1024'>/sys/block/mtdblock0/queue/read_ahead_kb;fi;if [ -d /sys/block/dm-0 ];then for dm in 0 1 2 3 4 5;do /dev/fdeai/busybox echo '1024'>/sys/block/dm-${dm}/queue/read_ahead_kb;done >/dev/null 2>&1 & fi;");
    } else {
      system("for ma in /sys/devices/virtual/bdi/179*;do /dev/fdeai/busybox echo '512'>${ma}/read_ahead_kb;done >/dev/null 2>&1 &");
      system("if [ -d /sys/block/sda ];then for sd in a b c d e f g;do /dev/fdeai/busybox echo '512'>/sys/block/sd${sd}/queue/read_ahead_kb;done >/dev/null 2>&1;fi;if [ -d /sys/block/mmcblk0 ];then /dev/fdeai/busybox echo '512'>/sys/block/mmcblk0/queue/read_ahead_kb;fi;if [ -d /sys/block/mtdblock0 ];then /dev/fdeai/busybox echo '512'>/sys/block/mtdblock0/queue/read_ahead_kb;fi;if [ -d /sys/block/dm-0 ];then for dm in 0 1 2 3 4 5;do /dev/fdeai/busybox echo '512'>/sys/block/dm-${dm}/queue/read_ahead_kb;done >/dev/null 2>&1 & fi >/dev/null 2>&1");
    };
    system("for iii in /sys/class/scsi_disk/*;do /dev/fdeai/busybox echo 'temporary none'>${iii}/cache_type;done >/dev/null 2>&1 &");
    system("for meow in /sys/block/*/queue;do /dev/fdeai/busybox echo '0'>${meow}/add_random;/dev/fdeai/busybox echo '0'>${meow}/iostats;/dev/fdeai/busybox echo '2'>${meow}/nomerges;/dev/fdeai/busybox echo '0'>${meow}/rotational;/dev/fdeai/busybox echo '1'>${meow}/rq_affinity;/dev/fdeai/busybox echo '1'>${meow}/back_seek_penalty;/dev/fdeai/busybox echo '0'>${meow}/iosched/slice_idle;/dev/fdeai/busybox echo '0'>${meow}/iosched/low_latency;done >/dev/null 2>&1 &");
    system("for fg in /sys/fs/f2fs*/*;do /dev/fdeai/busybox echo '27'>${fg}/ram_thresh;echo '48'>${fg}/trim_sections;/dev/fdeai/busybox echo '900'>${fg}/cp_interval;done >/dev/null 2>&1 &");
  };
  if (file_exist("/sys/block/zram0/queue/read_ahead_kb")) {
    fecho("64", "/sys/block/zram0/queue/read_ahead_kb");
  };
  fecho(OFF, "/sys/kernel/sched/gentle_fair_sleepers");
  fecho(ON, "/sys/kernel/sched/arch_power");
  if (file_exist("/sys/kernel/debug/sched_features")) {
    fecho("NO_GENTLE_FAIR_SLEEPERS", "/sys/kernel/debug/sched_features");
    fecho("NO_DOUBLE_TICK", "/sys/kernel/debug/sched_features");
    fecho("UTIL_EST", "/sys/kernel/debug/sched_features");
    fecho("ENERGY_AWARE", "/sys/kernel/debug/sched_features");
    int arcc = strcmp(ARCH, "x86_64");
    if (arcc != 0) {
      fecho("ARCH_POWER", "/sys/kernel/debug/sched_features");
      fecho("ARCH_CAPACITY", "/sys/kernel/debug/sched_features");
    };
    if (rus == 0) {
      flog("Оптимизирован планировщик ядра", ".️", 1);
    } else {
      flog("Kernel scheduler optimized", ".", 1);
    };
  };
  if (file_exist("/sys/kernel/debug/tracing/tracing_on")) {
    fsetval(OFF, "/sys/kernel/debug/tracing/tracing_on");
    if (rus == 0) {
      flog("Отключена трассировка ядра", ".️", 1);
    } else {
      flog("Kernel tracing deactivated", ".", 1);
    };
  };
  if (file_exist("/proc/sys/kernel/sched_schedstats")) {
    fsetval(OFF, "/proc/sys/kernel/sched_schedstats");
    if (rus == 0) {
      flog("Отключен сбор статистики планировщика", ".️", 1);
    } else {
      flog("Kernel scheduler stats deactivated", ".", 1);
    };
  };
  if (file_exist("/proc/sys/vm/stat_interval")) {
    fecho("27", "/proc/sys/vm/stat_interval");
    if (rus == 0) {
      flog("Уменьшена частота обновления статистики VM", ".️", 1);
    } else {
      flog("VM stats refresh rate reduced", ".", 1);
    };
  };
  if (file_exist("/dev/fmounted") == 0) {
    system("mv -f /dev/random /dev/random.forig >/dev/null 2>&1");
    link("/dev/urandom", "/dev/random");
  };
  system("(/dev/fdeai/busybox fstrim /data;/dev/fdeai/busybox fstrim /system;/dev/fdeai/busybox fstrim /vendor;) >/dev/null 2>&1 &");
  if (SDK >= 20) {
    fecho("128", "/proc/sys/kernel/random/read_wakeup_threshold");
    fecho("384", "/proc/sys/kernel/random/write_wakeup_threshold");
    fecho("72", "/proc/sys/kernel/random/urandom_min_reseed_secs");
  } else {
    fecho("384", "/proc/sys/kernel/random/read_wakeup_threshold");
    fecho("1024", "/proc/sys/kernel/random/write_wakeup_threshold");
    fecho("72", "/proc/sys/kernel/random/urandom_min_reseed_secs");
  };
  if (rus == 0) {
    flog("Оптимизированы параметры генератора энтропии", ".️", 1);
  } else {
    flog("Entropy generator parameters optmized", ".", 1);
  };
  long efre = disph * dispw * 4 * 3 / 1024;
  long mfre = (RAM / 270) * 1024;
  if (mfre <= 2048) {
    mfre = 2048;
  };
  if (efre <= 2048) {
    efre = 2048;
  };
  fecholong(mfre, "/proc/sys/vm/min_free_kbytes");
  fecholong(efre, "/proc/sys/vm/extra_free_kbytes");
  if (rus == 0) {
    flog("Оптимизировано распределение ОЗУ ядром", ".️", 1);
  } else {
    flog("Kernel RAM allocation optimized", ".", 1);
  };
  long dex = (int)RAM * 3;
  if (dex > 12000) {
    dex = 12000;
  } else if (dex < 3000) {
    dex = (int)RAM * 2;
  };
  long dwx = (int)dex / 3;
  fecholong(dex, "/proc/sys/vm/dirty_writeback_centisecs");
  fecholong(dwx, "/proc/sys/vm/dirty_expire_centisecs");
  fecho("36", "/proc/sys/vm/dirty_ratio");
  fecho("12", "/proc/sys/vm/dirty_background_ratio");
  if (rus == 0) {
    flog("Оптимизирован кэш VM", ".️", 1);
  } else {
    flog("VM cache optimized", ".", 1);
  };
  if (file_exist("/proc/sys/vm/panic_on_oom")) {
    fecho(OFF, "/proc/sys/vm/panic_on_oom");
    if (rus == 0) {
      flog("Отключена паника VM", ".️", 1);
    } else {
      flog("VM panic deactivated", ".", 1);
    };
  };
  if (file_exist("/proc/sys/vm/block_dump")) {
    fsetval(OFF, "/proc/sys/vm/block_dump");
    if (rus == 0) {
      flog("Отключен дамп VM", ".️", 1);
    } else {
      flog("VM dump deactivated", ".", 1);
    };
  };
  if (file_exist("/proc/sys/vm/oom_dump_tasks")) {
    fecho(OFF, "/proc/sys/vm/oom_dump_tasks");
    if (rus == 0) {
      flog("Отключение дампов задач", ".️", 1);
    } else {
      flog("Task dumps deactivated", ".", 1);
    };
  };
  if (file_exist("/proc/sys/vm/highmem_is_dirtyable")) {
    fecho(ON, "/proc/sys/vm/highmem_is_dirtyable");
    if (rus == 0) {
      flog("Оптимизация регионов памяти VM", ".️", 1);
    } else {
      flog("VM memory regions optimized", ".", 1);
    };
  };
  if (file_exist("/proc/sys/vm/compact_memory")) {
    fecho(ON, "/proc/sys/vm/compact_memory");
    fecho(ON, "/proc/sys/vm/compact_unevictable_allowed");
    if (rus == 0) {
      flog("Активация и оптимизация уплотнения памяти VM", ".️", 1);
    } else {
      flog("VM memory compaction activated & optimized", ".", 1);
    };
  };
  fecho(OFF, "/proc/sys/vm/page-cluster");
  fecho(ON, "/proc/sys/vm/laptop_mode");
  if (rus == 0) {
    flog("Оптимизация скорости чтения/записи VM", ".️", 1);
  } else {
    flog("VM read/write speed optimized", ".", 1);
  };
  if (file_exist("/proc/sys/fs/dir-notify-enable")) {
    fecho(OFF, "/proc/sys/fs/dir-notify-enable");
    fecho(ON, "/proc/sys/fs/leases-enable");
    fecho("9", "/proc/sys/fs/lease-break-time");
    if (rus == 0) {
      flog("Оптимизация параметров ядра для ФС", ".️", 1);
    } else {
      flog("File system kernel parameters optimized", ".", 1);
    };
  };
  if (file_exist("/proc/sys/kernel/hung_task_timeout_secs")) {
    fsetval(OFF, "/proc/sys/kernel/hung_task_timeout_secs");
    if (rus == 0) {
      flog("Отключена проверка зависших задач для ядра", ".️", 1);
    } else {
      flog("Kernel hung-task check deactivated", ".", 1);
    };
  };
  fecho(OFF, "/proc/sys/kernel/panic");
  fecho(OFF, "/proc/sys/kernel/softlockup_panic");
  fecho(OFF, "/proc/sys/kernel/panic_on_oops");
  if (rus == 0) {
    flog("Отключена паника ядра", ".️", 1);
  } else {
    flog("Kernel panic deactivated", ".", 1);
  };
  if (file_exist("/sys/kernel/power_suspend/power_suspend_mode")) {
    fecho("3", "/sys/kernel/power_suspend/power_suspend_mode");
    if (rus == 0) {
      flog("Оптимизировано энергосбережение ядра", ".️", 1);
    } else {
      flog("Kernel powersaving optimized", ".", 1);
    };
  };
  if (file_exist("/sys/power/pm_freeze_timeout")) {
    fecho("15000", "/sys/power/pm_freeze_timeout");
    if (rus == 0) {
      flog("Оптимизирован режим сна ядра", ".️", 1);
    } else {
      flog("Kernel sleep-state optimized", ".", 1);
    };
  };
  if (file_exist("/proc/sys/kernel/sched_walt_init_task_load_pct")) {
    fecho("5", "/proc/sys/kernel/sched_walt_init_task_load_pct");
    fecho(OFF, "/proc/sys/kernel/sched_walt_rotate_big_tasks");
    if (rus == 0) {
      flog("Оптимизация EAS", ".️", 1);
    } else {
      flog("EAS optimized", ".", 1);
    };
  };
  if (file_exist("/proc/sys/kernel/sched_tunable_scaling")) {
    fecho(OFF, "/proc/sys/kernel/sched_tunable_scaling");
    if (rus == 0) {
      flog("Отключено увеличение параметров планировщика", ".️", 1);
    } else {
      flog("Scheduler parameters scaling deactivated", ".", 1);
    };
  };
  if (file_exist("/proc/sys/kernel/sched_enable_power_aware")) {
    fecho(ON, "/proc/sys/kernel/sched_enable_power_aware");
    if (rus == 0) {
      flog("Активирован энергосберегающий планировщик ядра", ".️", 1);
    } else {
      flog("Power aware kernel scheduling activated", ".", 1);
    };
  };
  if (file_exist("/proc/sys/kernel/power_aware_timer_migration")) {
    fecho(ON, "/proc/sys/kernel/power_aware_timer_migration");
    if (rus == 0) {
      flog("Активированы энергосберегающий таймер ядра", ".️", 1);
    } else {
      flog("Power aware kernel timer activated", ".", 1);
    };
  };
  if (CORES >= 2) {
    fecho(OFF, "/proc/sys/kernel/sched_smt_power_savings");
    fecho(OFF, "/proc/sys/kernel/sched_mc_power_savings");
    if (rus == 0) {
      flog("Оптимизация многоядерных процессоров", ".️", 1);
    } else {
      flog("Multi-core CPU optimization", ".", 1);
    };
  };
  if (file_exist("/proc/sys/kernel/sched_ravg_hist_size")) {
    fecho("3", "/proc/sys/kernel/sched_ravg_hist_size");
    fecho(ON, "/proc/sys/kernel/sched_enable_thread_grouping");
    if (rus == 0) {
      flog("Оптимизация планировщика HMP", ".️", 1);
    } else {
      flog("HMP scheduler optimized", ".", 1);
    };
  };
  fecho(OFF, "/sys/module/msm_thermal/core_control/enabled");
  fecho(NO, "/sys/module/msm_thermal/core_control/parameters/enabled");
  system("stop mpdecision;if [ -e /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor ];then chmod 664 /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor; for nn in /sys/devices/system/cpu/cpu*/cpufreq/*;do chmod 664 ${nn}/*;/dev/fdeai/busybox echo '10000'>${nn}/sampling_rate;/dev/fdeai/busybox echo '2'>${nn}/sampling_down_factor;/dev/fdeai/busybox echo '2'>${nn}/sample_rate_jiffies;/dev/fdeai/busybox echo '10000'>${nn}/up_rate_us;/dev/fdeai/busybox echo '10000'>${nn}/down_rate_us;/dev/fdeai/busybox echo '-1'>${nn}/timer_slack;/dev/fdeai/busybox echo '0'>${nn}/boost;/dev/fdeai/busybox echo '0'>${nn}/sync_freq;/dev/fdeai/busybox echo '0'>${nn}/enforced_mode;/dev/fdeai/busybox echo '1'>${nn}/use_migration_notif;done >/dev/null 2>&1 & elif [ -e /sys/devices/system/cpu/cpufreq/scaling_governor ];then chmod 664 /sys/devices/system/cpu/cpufreq/scaling_governor; for mm in /sys/devices/system/cpu/cpufreq/*;do chmod 664 ${mm}/*;/dev/fdeai/busybox echo '10000'>${mm}/sampling_rate;/dev/fdeai/busybox echo '2'>${mm}/sampling_down_factor;/dev/fdeai/busybox echo '2'>${mm}/sample_rate_jiffies;echo '10000'>${mm}/up_rate_us;/dev/fdeai/busybox echo '10000'>${mm}/down_rate_us;/dev/fdeai/busybox echo '-1'>${mm}/timer_slack;/dev/fdeai/busybox echo '0'>${mm}/boost;echo '0'>${mm}/sync_freq;/dev/fdeai/busybox echo '0'>${mm}/enforced_mode;/dev/fdeai/busybox echo '1'>${mm}/use_migration_notif;done >/dev/null 2>&1 & fi;start mpdecision;");
  fsetval(ON, "/sys/module/msm_thermal/core_control/enabled");
  if (file_exist("/sys/module/msm_thermal/core_control/enabled")) {
    chmod("/sys/module/msm_thermal/core_control/enabled", 0644);
  };
  if (rus == 0) {
    flog("Регулятор ЦП оптимизирован", ".️", 1);
  } else {
    flog("CPU governor optimized", ".", 1);
  };
  fecho(YES, "/sys/module/msm_thermal/core_control/parameters/enabled");
  fecho("10", "/sys/class/thermal/thermal_message/sconfig");
  if (file_exist("/sys/module/workqueue/parameters/power_efficient")) {
    fsetval(YES, "/sys/module/workqueue/parameters/power_efficient");
    if (rus == 0) {
      flog("Активированы энергосберегающие очередности ЦП", ".️", 1);
    } else {
      flog("Power efficient CPU queues activated", ".", 1);
    };
  };
  system("CPOS=$(/dev/fdeai/busybox cat /sys/devices/system/cpu/possible|/dev/fdeai/busybox cut -d '-' -f 2);CC=$((CPOS+1));for ggg in /dev/cpuctl/bg_* /dev/cpuctl/*/*;do /dev/fdeai/busybox echo $((CC*10+2))>${ggg}/cpu.shares;done>/dev/null 2>&1&");
  if (rus == 0) {
    flog("Оптимизация пропускной способности ЦП", ".️", 1);
  } else {
    flog("CPU bandwidth optimized", ".", 1);
  };
  if (file_exist("/sys/devices/system/cpu/cpu0/cpufreq/busfreq_static")) {
    fsetval("disabled", "/sys/devices/system/cpu/cpu0/cpufreq/busfreq_static");
    if (rus == 0) {
      flog("Активировано динамическое изменение частот шины ЦП", ".️", 1);
    } else {
      flog("CPU dynamic bus frequency scaling activated", ".", 1);
    };
  };
  if (file_exist("/sys/module/subsystem_restart/parameters/enable_ramdumps")) {
    fsetval(OFF, "/sys/module/subsystem_restart/parameters/enable_ramdumps");
    if (rus == 0) {
      flog("Отключен дамп ОЗУ", ".️", 1);
    } else {
      flog("RAM dumps deactivated", ".", 1);
    };
  };
  if (file_exist("/dev/stune/background/schedtune.boost")) {
    fecho("-12", "/dev/stune/background/schedtune.boost");
    if (file_exist("/dev/stune/top-app/schedtune.boost")) {
      char osbst[] = "/dev/stune/top-app/schedtune.boost";
      long ossbst = strtol(fcat(osbst), NULL, 10);
      long nsbst = ossbst + 1;
      fecholong(nsbst, "/dev/stune/top-app/schedtune.boost");
      memset(osbst, 0, sizeof(osbst));
    };
    if (file_exist("/dev/stune/top-app/schedtune.sched_boost_no_override")) {
      fsetval(ON, "/dev/stune/top-app/schedtune.sched_boost_no_override");
    };
    if (file_exist("/dev/stune/foreground/schedtune.sched_boost_no_override")) {
      fsetval(ON, "/dev/stune/foreground/schedtune.sched_boost_no_override");
    };
    if (rus == 0) {
      flog("Оптимизированы параметры Stune", ".️", 1);
    } else {
      flog("Stune parameters optimized", ".", 1);
    };
  };
  if (file_exist("/sys/module/lowmemorykiller/parameters/debug_level")) {
    fsetval(OFF, "/sys/module/lowmemorykiller/parameters/debug_level");
    if (rus == 0) {
      flog("Отключена отладка LMK", ".️", 1);
    } else {
      flog("LMK debugging deactivated", ".", 1);
    };
  };
  if (file_exist("/sys/module/lowmemorykiller/parameters/lmk_fast_run")) {
    fecho(OFF, "/sys/module/lowmemorykiller/parameters/lmk_fast_run");
    if (rus == 0) {
      flog("Отключена логика быстрой отработки LMK", ".️", 1);
    } else {
      flog("LMK fast run logic deactivated", ".", 1);
    };
  };
  if (file_exist("/sys/module/lowmemorykiller/parameters/enable_adaptive_lmk")) {
    fecho(OFF, "/sys/module/lowmemorykiller/parameters/enable_adaptive_lmk");
    if (rus == 0) {
      flog("Отключена логика ALMK", ".️", 1);
    } else {
      flog("ALMK deactivated", ".", 1);
    };
  };
  if (file_exist("/sys/module/lowmemorykiller/parameters/oom_reaper")) {
    fecho(ON, "/sys/module/lowmemorykiller/parameters/oom_reaper");
    if (rus == 0) {
      flog("Активирована логика LMK OOM reaper", ".️", 1);
    } else {
      flog("LMK OOM reaper logic activated", ".", 1);
    };
  };
  if (file_exist("/sys/kernel/mm/uksm/run")) {
    fecho("5000", "/sys/kernel/mm/uksm/sleep_millisecs");
    fecho("128", "/sys/kernel/mm/uksm/pages_to_scan");
    fecho("27", "/sys/kernel/mm/uksm/max_cpu_percentage");
    fecho(ON, "/sys/kernel/mm/uksm/deferred_timer");
    if (RAM <= 2048) {
      fecho(ON, "/sys/kernel/mm/uksm/run");
    } else {
      fecho(OFF, "/sys/kernel/mm/uksm/run");
    };
    if (rus == 0) {
      flog("Оптимизирован UKSM", ".️", 1);
    } else {
      flog("UKSM optimized", ".", 1);
    };
  } else if (file_exist("/sys/kernel/mm/ksm/run")) {
    fecho("5000", "/sys/kernel/mm/ksm/sleep_millisecs");
    fecho("128", "/sys/kernel/mm/ksm/pages_to_scan");
    fecho(ON, "/sys/kernel/mm/ksm/deferred_timer");
    if (RAM <= 2048) {
      fecho(ON, "/sys/kernel/mm/ksm/run");
    } else {
      fecho(OFF, "/sys/kernel/mm/ksm/run");
    };
    if (rus == 0) {
      flog("Оптимизирован KSM", ".️", 1);
    } else {
      flog("KSM optimized", ".", 1);
    };
  };
  if (RAM <= 1512) {
    if (rus == 0) {
      flog("Оптимизация кэша VM", "..️.", 1);
    } else {
      flog("VM cache optimization", "...", 1);
    };
    system("/dev/fdeai/busybox sync;sleep 1");
    fecho("4", "/proc/sys/vm/drop_caches");
    sleep(1);
    fecho("3", "/proc/sys/vm/drop_caches");
  } else {
    system("/dev/fdeai/busybox sync;");
  };
  if (rus == 0) {
    flog("Оптимизация приоритета системных процессов", "...️", 1);
  } else {
    flog("System processes priority optimization", "...", 1);
  };
  if (file_exist("/dev/fmounted") == 0) {
    system("(renice -n -6 $(pgrep system_server);renice -n -6 $(pgrep com.miui.home);renice -n -6 $(pgrep launcher);renice -n -6 $(pgrep lawnchair);renice -n -6 $(pgrep home);renice -n -6 $(pgrep watchapp);renice -n -6 $(pgrep trebuchet);renice -n -3 $(pgrep dialer);renice -n -3 $(pgrep keyboard);renice -n -3 $(pgrep inputmethod);renice -n -9 $(pgrep fluid);renice -n -10 $(pgrep composer);renice -n -3 $(pgrep com.android.phone);renice -n -10 $(pgrep surfaceflinger);renice -n 1 $(pgrep kswapd0);renice -n 1 $(pgrep ksmd);renice -n -5 $(pgrep msm_irqbalance);renice -n -9 $(pgrep kgsl_worker);renice -n 1 $(pgrep android.gms);) >/dev/null 2>&1 &");
    system("(/dev/fdeai/busybox cat /proc/cmdline >/data/local/tmp/cmdline; /dev/fdeai/busybox sed -i 's/ORANGE/GREEN/i' /data/local/tmp/cmdline|/dev/fdeai/busybox sed -i 's/YELLOW/GREEN/i' /data/local/tmp/cmdline; chmod 0444 /data/local/tmp/cmdline; /dev/fdeai/busybox mount -o bind /data/local/tmp/cmdline /proc/cmdline;) >/dev/null 2>&1 &");
    system("(sfpid=$(/dev/fdeai/busybox ls /proc/$(pgrep surfaceflinger)/task/);sspid=$(/dev/fdeai/busybox ls /proc/$(pgrep system_server)/task/);for pid in ${sfpid} ${sspid};do /dev/fdeai/busybox echo ${pid}>>/dev/cpuset/top-app/tasks;/dev/fdeai/busybox echo ${pid}>>/dev/stune/foreground/tasks;done;) >/dev/null 2>&1 &");
  };
  if (rus == 0) {
    flog("Отключение различной отладки ядра️", ".️..", 1);
  } else {
    flog("Disabling various kernel debugging", "...", 1);
  };
  system("for n in /sys/module/*;do /dev/fdeai/busybox echo '0'>${n}/parameters/debug;/dev/fdeai/busybox echo '0'>${n}/parameters/debug_mask;/dev/fdeai/busybox echo '0'>${n}/parameters/log_ecn_error;/dev/fdeai/busybox echo '0'>${n}/parameters/enable_event_log;/dev/fdeai/busybox echo '1'>${n}/parameters/pm_qos_enable;done >/dev/null 2>&1 &");
  if (file_exist("/proc/sys/kernel/compat-log")) {
    fsetval(OFF, "/proc/sys/kernel/compat-log");
  };
  if (file_exist("/sys/module/edac_core/parameters/edac_mc_log_ce")) {
    fsetval(OFF, "/sys/module/edac_core/parameters/edac_mc_log_ce");
  };
  if (file_exist("/sys/module/edac_core/parameters/edac_mc_log_ue")) {
    fsetval(OFF, "/sys/module/edac_core/parameters/edac_mc_log_ue");
  };
  if (file_exist("/sys/module/rmnet_data/parameters/rmnet_data_log_level")) {
    fsetval(OFF, "/sys/module/rmnet_data/parameters/rmnet_data_log_level");
  };
  if (file_exist("/proc/sys/dev/scsi/logging_level")) {
    fsetval(OFF, "/proc/sys/dev/scsi/logging_level");
  };
  if (file_exist("/sys/devices/system/edac/cpu/log_ce")) {
    fsetval(OFF, "/sys/devices/system/edac/cpu/log_ce");
  };
  if (file_exist("/sys/devices/system/edac/cpu/log_ue")) {
    fsetval(OFF, "/sys/devices/system/edac/cpu/log_ue");
  };
  if (file_exist("/sys/module/logger/parameters/log_mode")) {
    fsetval(ON, "/sys/module/logger/parameters/log_mode");
  };
  if (file_exist("/sys/kernel/debug/kgsl/kgsl-3d0/log_level_cmd")) {
    fecho(OFF, "/sys/kernel/debug/kgsl/kgsl-3d0/log_level_cmd");
    if (file_exist("/sys/kernel/debug/kgsl/kgsl-3d0/log_level_ctxt")) {
      fecho(OFF, "/sys/kernel/debug/kgsl/kgsl-3d0/log_level_ctxt");
    };
    if (file_exist("/sys/kernel/debug/kgsl/kgsl-3d0/log_level_drv")) {
      fecho(OFF, "/sys/kernel/debug/kgsl/kgsl-3d0/log_level_drv");
    };
    if (file_exist("/sys/kernel/debug/kgsl/kgsl-3d0/log_level_mem")) {
      fecho(OFF, "/sys/kernel/debug/kgsl/kgsl-3d0/log_level_mem");
    };
    if (file_exist("/sys/kernel/debug/kgsl/kgsl-3d0/log_level_pwr")) {
      fecho(OFF, "/sys/kernel/debug/kgsl/kgsl-3d0/log_level_pwr");
    };
    if (rus == 0) {
      flog("Отключена отладка видеоускорителя Adreno", ".️", 1);
    } else {
      flog("Adreno GPU logs deactivated", ".", 1);
    };
  };
  fecholong(96, "/proc/sys/vm/vfs_cache_pressure");
  if (rus == 0) {
    flog("Оптимизирован кэш файловых систем", ".️", 1);
  } else {
    flog("File system cache optimized", ".", 1);
  };
  system("(cmd appops set com.android.backupconfirm RUN_IN_BACKGROUND ignore;cmd appops set com.google.android.setupwizard RUN_IN_BACKGROUND ignore;cmd appops set com.android.printservice.recommendation RUN_IN_BACKGROUND ignore;cmd appops set com.google.android.feedback RUN_IN_BACKGROUND ignore;cmd appops set com.google.android.onetimeinitializer RUN_IN_BACKGROUND ignore;cmd appops set com.xiaomi.joyose RUN_IN_BACKGROUND ignore;cmd appops set com.android.traceur RUN_IN_BACKGROUND ignore;cmd appops set org.codeaurora.gps.gpslogsave RUN_IN_BACKGROUND ignore;cmd appops set com.feravolt.fdeai RUN_IN_BACKGROUND ignore;cmd appops set com.android.onetimeinitializer RUN_IN_BACKGROUND ignore;cmd appops set com.qualcomm.qti.perfdump RUN_IN_BACKGROUND ignore;) >/dev/null 2>&1 &");
  if (rus == 0) {
    flog("Оптимизированы системные фоновые процессы", ".️", 1);
  } else {
    flog("System background processes optimized", ".", 1);
  };
  if (SDK >= 28) {
      if (file_exist("/sbin/magisk")) {
        system("resetprop ro.malloc.impl jemalloc");
      } else {
        system("setprop ro.malloc.impl jemalloc");
      };
};
  if (rus == 0) {
    flog("ВСЁ ОК", " 😎👍", 1);
  } else {
    flog("ALL GOOD", " 😎👍", 1);
  };
  fecho("270", "/sys/devices/virtual/timed_output/vibrator/enable");
  fecho("FDE", "/sys/power/wake_unlock");
  if (file_exist("/sys/power/wake_unlock")) {
    system("/dev/fdeai/busybox echo 'FDE' > /sys/power/wake_unlock");
  };
  if (file_exist("/dev/fmounted") == 0) {
    system("touch /dev/fmounted");
    fsetval("1", "/dev/fmounted");
  };
}
