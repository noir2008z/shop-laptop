#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define MAX_LAPTOPS 200
#define MAX_CRITERIA 20
#define NAME_LEN 80
#define TEXT_LEN 160
#define FILE_LAPTOPS "laptops.txt"
#define FILE_CRITERIA "criteria.txt"
typedef struct  {
    int id;
    char name[NAME_LEN], brand[NAME_LEN], cpu[TEXT_LEN], gpu[TEXT_LEN];
    int ramGB, ssdGB;
    long long price;
    char screen[TEXT_LEN], battery[TEXT_LEN], weight[TEXT_LEN], url[TEXT_LEN];
    int active;
}
Laptop;
typedef struct  {
    char key[NAME_LEN];
    char value[TEXT_LEN];
    int mandatory;
    int weight;
}
Criterion;
Laptop laptops[MAX_LAPTOPS];
Criterion criteria[MAX_CRITERIA];
int laptopCount = 0, criterionCount = 0;
int nextId = 1;
void trimNewline(char *s)  {
    s[strcspn(s, "\r\n")] = '\0';
}
void readLine(const char *prompt, char *out, size_t size)  {
    printf("%s", prompt);
    if (!fgets(out, (int)size, stdin))  {
        out[0] = '\0';
        return;
    }
    trimNewline(out);
}
int readInt(const char *prompt, int min, int max)  {
    char buf[100], *end;
    long v;
    for (;;)  {
        readLine(prompt, buf, sizeof(buf));
        v = strtol(buf, &end, 10);
        while (isspace((unsigned char)*end)) end++;
        if (buf[0] && *end == '\0' && end != buf && v >= min && v <= max) return (int)v;
        printf("Nhap so nguyen trong [%d, %d].\n", min, max);
    }
}
long long readMoney(const char *prompt, long long min, long long max)  {
    char buf[100], *end;
    long long v;
    for (;;)  {
        readLine(prompt, buf, sizeof(buf));
        v = strtoll(buf, &end, 10);
        while (isspace((unsigned char)*end)) end++;
        if (buf[0] && *end == '\0' && end != buf && v >= min && v <= max) return v;
        printf("Nhap so tien hop le trong [%lld, %lld] VND.\n", min, max);
    }
}
void pauseEnter(void)  {
    char tmp[8];
    printf("\nNhan Enter de tiep tuc...");
    fgets(tmp, sizeof(tmp), stdin);
}
void lowerCopy(char *dst, size_t n, const char *src)  {
    size_t i;
    for (i = 0; i + 1 < n && src[i]; i++) dst[i] = (char)tolower((unsigned char)src[i]);
    dst[i] = '\0';
}
int containsIgnoreCase(const char *text, const char *needle)  {
    char a[TEXT_LEN * 2], b[TEXT_LEN * 2];
    lowerCopy(a, sizeof(a), text);
    lowerCopy(b, sizeof(b), needle);
    return strstr(a, b) != NULL;
}
Laptop *findLaptop(int id)  {
    int i;
    for (i = 0; i < laptopCount; i++) if (laptops[i].id == id && laptops[i].active) return &laptops[i];
    return NULL;
}
void addSample(const char *name, const char *brand, const char *cpu, const char *gpu,
int ram, int ssd, long long price, const char *screen,
const char *battery, const char *weight, const char *url)  {
    Laptop *l;
    if (laptopCount >= MAX_LAPTOPS) return;
    l = &laptops[laptopCount++];
    memset(l, 0, sizeof(*l));
    l->id = nextId++;
    snprintf(l->name, sizeof(l->name), "%s", name);
    snprintf(l->brand, sizeof(l->brand), "%s", brand);
    snprintf(l->cpu, sizeof(l->cpu), "%s", cpu);
    snprintf(l->gpu, sizeof(l->gpu), "%s", gpu);
    l->ramGB = ram;
    l->ssdGB = ssd;
    l->price = price;
    l->active = 1;
    snprintf(l->screen, sizeof(l->screen), "%s", screen);
    snprintf(l->battery, sizeof(l->battery), "%s", battery);
    snprintf(l->weight, sizeof(l->weight), "%s", weight);
    snprintf(l->url, sizeof(l->url), "%s", url);
}
void seedData(void)  {
    addSample("Aspire 5 A515", "Acer", "Core i5-1335U", "Intel Iris Xe", 16, 512, 14500000, "15.6 inch FHD IPS", "50Wh", "1.8kg", "https://example.com/acer-aspire");
    addSample("IdeaPad Slim 3", "Lenovo", "Ryzen 5 7530U", "Radeon Graphics", 16, 512, 13900000, "15.6 inch FHD", "47Wh", "1.6kg", "https://example.com/lenovo-ideapad");
    addSample("Vivobook 15", "ASUS", "Core i5-12500H", "Intel Iris Xe", 16, 512, 16900000, "15.6 inch FHD OLED", "50Wh", "1.7kg", "https://example.com/asus-vivobook");
    addSample("Nitro V 15", "Acer", "Core i5-13420H", "RTX 5050 8GB", 16, 512, 29990000, "15.6 inch FHD IPS 165Hz", "57Wh", "2.1kg", "https://example.com/acer-nitro");
    addSample("LOQ 15", "Lenovo", "Core i5-12450HX", "RTX 4050 6GB", 16, 512, 25990000, "15.6 inch FHD 144Hz", "60Wh", "2.4kg", "https://example.com/lenovo-loq");
    addSample("TUF Gaming A15", "ASUS", "Ryzen 7 7735HS", "RTX 4060 8GB", 16, 512, 31990000, "15.6 inch FHD 144Hz", "90Wh", "2.2kg", "https://example.com/asus-tuf");
    addSample("MacBook Air 13", "Apple", "Apple M2", "Integrated 10-core", 16, 512, 26990000, "13.6 inch Liquid Retina", "Up to 18h", "1.24kg", "https://example.com/macbook-air");
    addSample("CreatorBook 16", "ASUS", "Core i7-13620H", "RTX 4050 6GB", 32, 1000, 35990000, "16 inch 2.5K 120Hz", "90Wh", "2.0kg", "https://example.com/creatorbook");
    addSample("ThinkPad E14", "Lenovo", "Core i5-1335U", "Intel Iris Xe", 16, 512, 19990000, "14 inch WUXGA IPS", "57Wh", "1.5kg", "https://example.com/thinkpad");
    addSample("Modern 14", "MSI", "Ryzen 5 7530U", "Radeon Graphics", 8, 512, 11990000, "14 inch FHD IPS", "39Wh", "1.4kg", "https://example.com/msi-modern");
}
int saveLaptops(void)  {
    FILE *f = fopen(FILE_LAPTOPS, "w");
    int i;
    if (!f) return 0;
    for (i = 0; i < laptopCount; i++)  {
        Laptop *l = &laptops[i];
        fprintf(f, "%d|%s|%s|%s|%s|%d|%d|%lld|%s|%s|%s|%s|%d\n",
        l->id,l->name,l->brand,l->cpu,l->gpu,l->ramGB,l->ssdGB,l->price,
        l->screen,l->battery,l->weight,l->url,l->active);
    }
    fclose(f);
    return 1;
}
int loadLaptops(void)  {
    FILE *f = fopen(FILE_LAPTOPS, "r");
    char line[1200];
    int loaded = 0;
    if (!f) return 0;
    laptopCount = 0;
    nextId = 1;
    while (fgets(line, sizeof(line), f) && laptopCount < MAX_LAPTOPS)  {
        Laptop l;
        char *p;
        memset(&l, 0, sizeof(l));
        trimNewline(line);
        p = strtok(line, "|");
        if (!p) continue;
        l.id = atoi(p);
#define GETSTR(field) do { p = strtok(NULL, "|"); if (!p) goto bad_laptop; snprintf(l.field, sizeof(l.field), "%s", p); } while (0)
        GETSTR(name);
        GETSTR(brand);
        GETSTR(cpu);
        GETSTR(gpu);
        p = strtok(NULL, "|");
        if (!p) goto bad_laptop;
        l.ramGB = atoi(p);
        p = strtok(NULL, "|");
        if (!p) goto bad_laptop;
        l.ssdGB = atoi(p);
        p = strtok(NULL, "|");
        if (!p) goto bad_laptop;
        l.price = atoll(p);
        GETSTR(screen);
        GETSTR(battery);
        GETSTR(weight);
        GETSTR(url);
        p = strtok(NULL, "|");
        if (!p) goto bad_laptop;
        l.active = atoi(p);
        laptops[laptopCount++] = l;
        if (l.id >= nextId) nextId = l.id + 1;
        loaded++;
        continue;
        bad_laptop: ;
#undef GETSTR
    }
    fclose(f);
    return loaded;
}
int saveCriteria(void)  {
    FILE *f=fopen(FILE_CRITERIA,"w");
    int i;
    if(!f) return 0;
    for(i=0;i<criterionCount;i++) fprintf(f,"%s|%s|%d|%d\n",criteria[i].key,criteria[i].value,criteria[i].mandatory,criteria[i].weight);
    fclose(f);
    return 1;
}
int loadCriteria(void)  {
    FILE *f=fopen(FILE_CRITERIA,"r");
    char line[500];
    int n=0;
    if(!f) return 0;
    criterionCount=0;
    while(fgets(line,sizeof(line),f)&&criterionCount<MAX_CRITERIA)  {
        Criterion c;
        char *p;
        memset(&c,0,sizeof(c));
        trimNewline(line);
        p=strtok(line,"|");
        if(!p) continue;
        snprintf(c.key,sizeof(c.key),"%s",p);
        p=strtok(NULL,"|");
        if(!p) continue;
        snprintf(c.value,sizeof(c.value),"%s",p);
        p=strtok(NULL,"|");
        if(!p) continue;
        c.mandatory=atoi(p);
        p=strtok(NULL,"|");
        if(!p) continue;
        c.weight=atoi(p);
        criteria[criterionCount++]=c;
        n++;
    }
    fclose(f);
    return n;
}
void printLaptop(const Laptop *l)  {
    printf("\nMa san pham (ID - ma nhan dien rieng cua laptop): %d\nTen san pham: %s (%s)\n  CPU: %s | GPU: %s\n  RAM: %dGB | SSD: %dGB | Gia tham khao: %lld VND\n  Man hinh: %s | Pin: %s | Khoi luong: %s\n  Link tham khao: %s\n",
    l->id,l->name,l->brand,l->cpu,l->gpu,l->ramGB,l->ssdGB,l->price,l->screen,l->battery,l->weight,l->url);
}
void listLaptops(void)  {
    int i, count=0;
    puts("\n===== DANH SACH LAPTOP =====");
    puts("Luu y: Ma san pham (ID) la ma rieng de nhan dien va tim dung laptop.");
    for(i=0;i<laptopCount;i++) if(laptops[i].active) printLaptop(&laptops[i]),count++;
    if(!count) puts("Danh sach trong.");
    printf("\nTong: %d laptop\n",count);
}
void saveAll(void)  {
    saveLaptops();
    saveCriteria();
}
void loadAll(void)  {
    int n = loadLaptops();
    if(n == 0) {
        laptopCount = 0;
        nextId = 1;
        seedData();
        saveLaptops();
    }
    loadCriteria();
}
int matchesCriterion(const Laptop *l, const Criterion *c)  {
    if(!containsIgnoreCase(c->key,"brand") && !containsIgnoreCase(c->key,"hang"))  {
        if(containsIgnoreCase(c->key,"cpu")) return containsIgnoreCase(l->cpu,c->value);
        if(containsIgnoreCase(c->key,"gpu")) return containsIgnoreCase(l->gpu,c->value);
        if(containsIgnoreCase(c->key,"ram"))  {
            char b[20];
            snprintf(b,sizeof(b),"%d",l->ramGB);
            return strstr(b,c->value)!=NULL;
        }
        if(containsIgnoreCase(c->key,"ssd"))  {
            char b[20];
            snprintf(b,sizeof(b),"%d",l->ssdGB);
            return strstr(b,c->value)!=NULL;
        }
        if(containsIgnoreCase(c->key,"screen") || containsIgnoreCase(c->key,"man hinh")) return containsIgnoreCase(l->screen,c->value);
    }
    if(containsIgnoreCase(c->key,"brand") || containsIgnoreCase(c->key,"hang")) return containsIgnoreCase(l->brand,c->value);
    return 1;
}
int laptopPassesCriteria(const Laptop *l)  {
    int i;
    for(i=0;i<criterionCount;i++) if(criteria[i].mandatory && !matchesCriterion(l,&criteria[i])) return 0;
    return 1;
}
void manageCriteria(void)  {
    int ch,i;
    do  {
        puts("\n--- TIEU CHI CA NHAN ---\n1. Xem\n2. Them\n3. Xoa tat ca\n0. Quay lai");
        ch=readInt("Chon: ",0,3);
        if(ch==1)  {
            if(!criterionCount) puts("Chua co tieu chi.");
            for(i=0;i<criterionCount;i++) printf("%d. %s = %s | bat buoc=%s | trong so=%d\n",i+1,criteria[i].key,criteria[i].value,criteria[i].mandatory?"Co":"Khong",criteria[i].weight);
        } else if(ch==2 && criterionCount<MAX_CRITERIA)  {
            Criterion *c=&criteria[criterionCount++];
            readLine("Ten truong (brand/cpu/gpu/ram/ssd/screen): ",c->key,sizeof(c->key));
            readLine("Gia tri can co: ",c->value,sizeof(c->value));
            c->mandatory=readInt("Bat buoc? (1 co, 0 khong): ",0,1);
            c->weight=readInt("Trong so uu tien (1-10): ",1,10);
            saveCriteria();
            puts("Da them tieu chi.");
        } else if(ch==3)  {
            criterionCount=0;
            saveCriteria();
            puts("Da xoa tat ca tieu chi.");
        }
    } while(ch!=0);
}
int scoreLaptop(const Laptop *l, int field, int usage)  {
    int score=0;
    if(l->price <= 15000000) score += 2;
    if(l->ramGB >= 16) score += 2;
    if(l->ssdGB >= 512) score += 1;
    if(usage==1 && (containsIgnoreCase(l->cpu,"i5")||containsIgnoreCase(l->cpu,"Ryzen 5")||containsIgnoreCase(l->cpu,"i7")||containsIgnoreCase(l->cpu,"Ryzen 7")||containsIgnoreCase(l->cpu,"M2"))) score+=3;
    if(usage==2 && (containsIgnoreCase(l->gpu,"RTX")||containsIgnoreCase(l->gpu,"Radeon Pro"))) score+=5;
    if(usage==3 && (containsIgnoreCase(l->gpu,"RTX")||containsIgnoreCase(l->gpu,"Arc"))) score+=4;
    if(usage==4 && containsIgnoreCase(l->gpu,"RTX")) score+=5;
    if(field==1 && (containsIgnoreCase(l->cpu,"i5")||containsIgnoreCase(l->cpu,"Ryzen 5")||containsIgnoreCase(l->cpu,"M2"))) score+=1;
    if(field==2 && (containsIgnoreCase(l->gpu,"RTX")||containsIgnoreCase(l->screen,"OLED"))) score+=2;
    if(field==3 && l->weight[0] && (l->weight[0]=='1')) score+=1;
    return score;
}
void consult(void)  {
    long long budget;
    int field,usage,i,best=-1,bestScore=-1;
    const char *fields[]= {
        "","CNTT / Lap trinh","Thiet ke do hoa","Kinh te / Van phong","Kien truc / Ky thuat","Nganh khac"
    }
    ;
    const char *uses[]= {
        "","Hoc tap / van phong","Lap trinh","Do hoa / chinh sua anh","Dung video","Choi game"
    }
    ;
    budget=readMoney("Nhap ngan sach toi da (VND): ",1000000,1000000000LL);
    puts("Nganh hoc: 1.CNTT 2.Thiet ke do hoa 3.Kinh te/van phong 4.Kien truc/ky thuat 5.Khac");
    field=readInt("Chon nganh: ",1,5);
    puts("Nhu cau: 1.Hoc tap/van phong 2.Lap trinh 3.Do hoa/chinh sua anh 4.Dung video 5.Choi game");
    usage=readInt("Chon nhu cau: ",1,5);
    for(i=0;i<laptopCount;i++)  {
        int sc;
        if(!laptops[i].active || laptops[i].price>budget || !laptopPassesCriteria(&laptops[i])) continue;
        sc=scoreLaptop(&laptops[i],field,usage);
        if(sc>bestScore)  {
            bestScore=sc;
            best=i;
        }
    }
    if(best<0)  {
        puts("Khong tim thay laptop phu hop voi ngan sach/tieu chi. Hay tang ngan sach hoac sua tieu chi.");
        return;
    }
    puts("\n===== GOI Y LAPTOP PHU HOP =====");
    printLaptop(&laptops[best]);
    printf("Diem phu hop tham khao: %d\n",bestScore);
    printf("Ly do: gia nam trong ngan sach; RAM %dGB, SSD %dGB; GPU %s; phu hop voi nhu cau %s va nganh %s.\n",
    laptops[best].ramGB,laptops[best].ssdGB,laptops[best].gpu,uses[usage],fields[field]);
}
void searchLaptops(void)  {
    char q[TEXT_LEN];
    int i,found=0;
    readLine("Nhap tu khoa (ten/hang/CPU/GPU/man hinh): ",q,sizeof(q));
    for(i=0;i<laptopCount;i++)  {
        Laptop *l=&laptops[i];
        if(l->active && (containsIgnoreCase(l->name,q)||containsIgnoreCase(l->brand,q)||containsIgnoreCase(l->cpu,q)||containsIgnoreCase(l->gpu,q)||containsIgnoreCase(l->screen,q))) printLaptop(l),found++;
    }
    printf("Tim thay %d laptop.\n",found);
}
void filterSort(void)  {
    int i,j,sortBy,found=0,idx[MAX_LAPTOPS];
    long long minP,maxP;
    int minRam;
    minP=readMoney("Gia thap nhat (VND): ",0,1000000000LL);
    maxP=readMoney("Gia cao nhat (VND): ",minP,1000000000LL);
    minRam=readInt("RAM toi thieu (GB): ",0,256);
    for(i=0;i<laptopCount;i++) if(laptops[i].active&&laptops[i].price>=minP&&laptops[i].price<=maxP&&laptops[i].ramGB>=minRam&&laptopPassesCriteria(&laptops[i])) idx[found++]=i;
    puts("Sap xep: 1.Gia tang dan 2.Gia giam dan 3.RAM giam dan 4.SSD giam dan");
    sortBy=readInt("Chon: ",1,4);
    for(i=0;i<found;i++) for(j=i+1;j<found;j++)  {
        int a=idx[i],b=idx[j],swap=0;
        if(sortBy==1) swap=laptops[a].price>laptops[b].price;
        if(sortBy==2) swap=laptops[a].price<laptops[b].price;
        if(sortBy==3) swap=laptops[a].ramGB<laptops[b].ramGB;
        if(sortBy==4) swap=laptops[a].ssdGB<laptops[b].ssdGB;
        if(swap)  {
            int t=idx[i];
            idx[i]=idx[j];
            idx[j]=t;
        }
    }
    printf("\nTim thay %d laptop:\n",found);
    for(i=0;i<found;i++) printLaptop(&laptops[idx[i]]);
}
void inputLaptopFields(Laptop *l, int isNew)  {
    if(isNew)  {
        l->id=nextId++;
        l->active=1;
    }
    if(isNew) printf("Ma san pham (ID) duoc tao tu dong: %d\n",l->id);
    if(isNew) puts("Hay ghi nho ma nay de sua, xoa hoac so sanh dung san pham.");
    readLine("Ten laptop: ",l->name,sizeof(l->name));
    readLine("Hang: ",l->brand,sizeof(l->brand));
    readLine("CPU: ",l->cpu,sizeof(l->cpu));
    readLine("GPU: ",l->gpu,sizeof(l->gpu));
    l->ramGB=readInt("RAM (GB): ",1,256);
    l->ssdGB=readInt("SSD (GB): ",1,10000);
    l->price=readMoney("Gia (VND): ",0,1000000000LL);
    readLine("Man hinh: ",l->screen,sizeof(l->screen));
    readLine("Pin: ",l->battery,sizeof(l->battery));
    readLine("Khoi luong: ",l->weight,sizeof(l->weight));
    readLine("URL tham khao: ",l->url,sizeof(l->url));
}
void addLaptop(void)  {
    if(laptopCount>=MAX_LAPTOPS)  {
        puts("Danh sach da day.");
        return;
    }
    inputLaptopFields(&laptops[laptopCount++],1);
    saveLaptops();
    puts("Da them laptop.");
}
void editLaptop(void)  {
    int id=readInt("Nhap ma san pham (ID) can sua: ",1,1000000000);
    Laptop *l=findLaptop(id);
    if(!l)  {
        puts("Khong tim thay ID.");
        return;
    }
    printLaptop(l);
    puts("Nhap lai cac truong (ID duoc giu nguyen):");
     {
        int oldId=l->id;
        inputLaptopFields(l,0);
        l->id=oldId;
    }
    saveLaptops();
    puts("Da cap nhat.");
}
void deleteLaptop(void)  {
    int id=readInt("Nhap ma san pham (ID) can xoa: ",1,1000000000);
    Laptop *l=findLaptop(id);
    if(!l)  {
        puts("Khong tim thay ID.");
        return;
    }
    l->active=0;
    saveLaptops();
    puts("Da xoa (an khoi danh sach).");
}
void compareLaptops(void)  {
    int a=readInt("Nhap ma san pham (ID) thu nhat: ",1,1000000000);
    int b=readInt("Nhap ma san pham (ID) thu hai: ",1,1000000000);
    Laptop *x=findLaptop(a),*y=findLaptop(b);
    if(!x||!y)  {
        puts("ID khong hop le.");
        return;
    }
    puts("\n===== SO SANH =====");
    printLaptop(x);
    printLaptop(y);
    printf("\nChenh lech gia: %lld VND (duong nghia la laptop 1 dat hon).\n",x->price-y->price);
    printf("RAM: %s | SSD: %s | Gia re hon: %s\n",
    x->ramGB==y->ramGB?"Bang nhau":(x->ramGB>y->ramGB?"Laptop 1":"Laptop 2"),
    x->ssdGB==y->ssdGB?"Bang nhau":(x->ssdGB>y->ssdGB?"Laptop 1":"Laptop 2"),
    x->price==y->price?"Bang nhau":(x->price<y->price?"Laptop 1":"Laptop 2"));
}
void mainMenu(void)  {
    int ch;
    do  {
        puts("\n========== LAPTOP ADVISOR ==========");
        puts("1. Tu van laptop (4 buoc)");
        puts("2. Tim kiem laptop");
        puts("3. Loc va sap xep laptop");
        puts("4. Them laptop");
        puts("5. Sua laptop");
        puts("6. Xoa laptop");
        puts("7. Xem danh sach laptop");
        puts("8. So sanh 2 laptop");
        puts("9. Quan ly tieu chi rieng");
        puts("0. Thoat");
        ch=readInt("Chon chuc nang: ",0,9);
        switch(ch)  {
            case 1: consult();
            break;
            case 2: searchLaptops();
            break;
            case 3: filterSort();
            break;
            case 4: addLaptop();
            break;
            case 5: editLaptop();
            break;
            case 6: deleteLaptop();
            break;
            case 7: listLaptops();
            break;
            case 8: compareLaptops();
            break;
            case 9: manageCriteria();
            break;
        }
        if(ch!=0) pauseEnter();
    } while(ch!=0);
    saveAll();
    puts("Cam on ban da su dung Laptop Advisor!");
}
int main(void)  {
    loadAll();
    if(laptopCount==0)  {
        seedData();
        saveAll();
    }
    mainMenu();
    return 0;
}
