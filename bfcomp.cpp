#include <stdio.h>
#include <math.h>
#include <string.h>
//Engin Mercan in muhtesem BF compiler i

char buff[1024*16];

//compare operasyonlari
enum CMP { EQ, NE, GT, GE, LT, LE };  

char *cur=buff, *cur2;


//c karakter arrayini print eden makro
#define print(x) \
  do{ \
    cur2=x; \
    while(*cur2){\
      *(cur) = *(cur2);\
      cur++;\
      cur2++;\
    }\
  }while(0)
  
//x karakter arrayini n kez print eden makro
#define printn(x, n) for(int temp=0; temp<(n); temp++) print(x)

int stack[100];
int st=0;

int stackSize[100];
int s=0;

struct{
  int k;
  int i;
  int j;
  CMP type;
}wstack[100];
int wst;

int current = 0;
int size = 0;

int push(int);
void pop();
void move(int);
void clear(int);
void inc(int, int);
int copy(int);
int addPush(int, int);
int subPush(int, int);


//imleci bir cellin üstüne goturme, 
void move(int i){
  if(i<current) printn("<", current-i);
  else if(i>current) printn(">", i-current);
  current = i;
}


//bir cell i bir constant a atama
//cell[i]=value
void set(int i, int value){
  clear(i);
  if(value) inc(i, value);
}

//cell i sifirlama
void clear(int i){
  move(i);
  print("[-]");
}

//4 islem (aslinda modla 5)
//bunlarin herbiri 2 cell id si aliyor, islem sonucunu push edip id sini return ediyor

//push cell[i]+cell[j]
int addPush(int i, int j){
   copy(i);
   int z = copy(j);
   move(z);
   print("[-<+>]");
   pop();
   return z-1;
}


//push cell[i]-cell[j]
int subPush(int i, int j){
   copy(i);
   int z = copy(j);
   move(z);
   print("[-<->]");
   pop();
   return z-1;
}


//push cell[i]*cell[j]
int mulPush(int i, int j){
   push(0);
   push(0);
   copy(i);
   int p = copy(j);
   move(p);
   print("[-< [-<+<+>>] < [->+<]>>]");
   pop();
   pop();
   pop();
   return p-3;
}


//push cell[i]/cell[j]
int divPush(int i, int j){
   int p =copy(i);
   copy(j);
   move(p);
   // >0 d-n%d n%d n/d
   //div mod algoritmasi, yapanin gozlerinden opuyorum.
   print("[->-[>+>>]>[+[-<+>]>+>>]<<<<<]  >[-]>[-]>[-<<<+>>>]<<<");
   pop();
   return p;
}


//push cell[i]%cell[j]
int modPush(int i, int j){
   int p =copy(i);
   copy(j);
   move(p);
   // >0 d-n%d n%d n/d
   print("[->-[>+>>]>[+[-<+>]>+>>]<<<<<]  >[-]>[-<<+>>]>[-]<<<");
   pop();
   return p;
}



//bu iki islemde bir cell i digerine direk eklemek icin. 

//cell[i]+=cell[j]
void addTo(int i, int j){
  int z = copy(i);
  move(z);
  print("[-");
  move(j);
  print("+");
  move(z);
  print("]");
  pop();
}

//cell[i]-=cell[j]
void subTo(int i, int j){
  int z = copy(i);
  move(z);
  print("[-");
  move(j);
  print("-");
  move(z);
  print("]");
  pop();
}

//bu iki fonksiyon bir cell e bir constant eklemek icin
//cell[i]+=value
void inc(int i, int value){
  move(i);
  if(value<=10){
    printn("+", value);
    return;
  }
  int sq = (int)sqrt(value);
  int dv = value/sq;

  move(size);
  printn("+", sq);
  
  print("[-");
  move(i);
  printn("+", dv);
  move(size);
  print("]");
  
  move(i);
  printn("+", value-sq*dv);
}

void inc(int i){
  inc(i, 1);
}
//cell[i]-=value
void dec(int i, int value){
  move(i);
  if(value<=10){
    printn("-", value);
    return;
  }
  int sq = (int)sqrt(value);
  int dv = value/sq;

  move(size);
  printn("+", sq);
  
  print("[-");
  move(i);
  printn("-", dv);
  move(size);
  print("]");
  
  move(i);
  printn("-", value-sq*dv);
}

void dec(int i){
  dec(i, 1);
}

//stack a bir integer push etmek icin
//push edip id sini donduruyor
int push(int a){
  int k = size;
  size++;
  set(k, a);
  stackSize[s]++;
  return k;
}

//stack sonundan bir cell pop ediyor. push edilen her sey scope disina cikarken 
//otomatik pop edilir, ama isterseniz manuel de yapabilirsiniz
//boylece stack cok buyumez
//pop çok problem cikarmisti, stackte kullanilmayan seylerin sifirlanmasi sart

void popAndClear(){
  size--;  
  stackSize[s]--;
  clear(size);
}
void pop(){
  return popAndClear();
  
}

//bir celli kopyalayip push ediyor. kopyanin id sini donduruyor
int copy(int i){
  move(i);
  print("[-");
  move(size);
  print("+>+<");
  move(i);
  print("]");
  move(size);
  print(">[-<");
  move(i);
  print("+");
  move(size);
  print(">]<");
  
  size++;
  stackSize[s]++;
  return size-1;
}

//tek rakamdan olusan sayilari print etmek icin
void printInt(int i){
  int k = copy(i);
  move(k);
  print(">++++++++[-<++++++>]<.[-]");
  pop();
}

//iki rakama kadar cikabilecek celleri print etmek icin
void printInt2(int i){
  int k = copy(i);
  move(k);
  print("> +++++ +++++ <");
  
 // >n d
 print("[->-[>+>>]>[+[-<+>]>+>>]<<<<<]");
// >0 d-n%d n%d n/d
  
  print(">[-]++++ ++++ ++++[->++++>++++<<]>>.[-]<.[-]<<");
  pop();
}

//uc rakama kadar cikabilecek celleri print etmek icin
void printInt3(int i){
  int k = copy(i);
  move(k);
  print("> +++++ +++++ <");
  print("[->-[>+>>]>[+[-<+>]>+>>]<<<<<]");
  print(">[-] > > > +++++ +++++ <");
  print("[->-[>+>>]>[+[-<+>]>+>>]<<<<<]");
  print(">[-] ++++ ++++ ++++ [->++++>++++<< <<++++>>] >>.[-]<.[-] <<< .[-]  <<. "); 
  pop();
}

//compare fonksiyonu, diger fonksiyonlar tarafindan kullaniliyor ama direkte kullanilabilir
//compare dogru ise 1 yanlis ise 0 push ediyor ve onu return ediyor
int compare(int i, int j, CMP type){
  if(type == EQ){
    int k = copy(i);
    copy(j);
    move(k);
    print("[->-<]+>[[-]<->]<");
    pop();
    return k;
  }else if(type == NE){
    int k = copy(i);
    copy(j);
    move(k);
    print("[->-<]>[[-]<+>]<");
    pop();
    return k;
  } else if(type == GE){
    int k = copy(i);
    copy(j);
    move(k);
    //ij01r
    print("+>+<[->- >>+<<"); 
    
    print("[->+>[-]<<] > [-<+>] > [-<<<[-]>>>>+<] <<<] >[-]>>>[-<<<<+>>>>]<<<< " );
    pop();
    return k;
  }  else if(type == GT){
    int k = copy(i);
    copy(j);
    move(k);
    //ij01r
    print("+>++<[->- >>+<<"); 
    
    print("[->+>[-]<<] > [-<+>] > [-<<<[-]>>>>+<] <<<] >[-]>>>[-<<<<+>>>>]<<<< " );
    pop();
    return k;
  } else if(type == LT){
    return compare(j, i, GT);
  } else if(type == LE){
    return compare(j, i, GE);
  }
  return -1;
}


//char input almak icin
//, ile rakam aliyor. stack a pushluyor ve id sini return ediyor
int inputChar(){
  int k = push(0);
  move(k);
  print(",");
  return k;
}


//tek rakam input almak icin
//bir char aliyor, '0' cikariyor pushluyor ve return ediyor
int inputInt(){
  int k = push(0);
  move(k);
  print(",>++++ ++++[-<------>]<");
  return k;
}


//2 rakamli integer input almak icin. eger tek rakam girmek isterse 05 gibi girmeli
//3 rakamida yazabilirdik aslinda, 255 e kadar yolu var
int inputInt2(){
  int k = push(0);
  move(k);
  print(">,<,>>++++ ++++ ++++[-<----<---->>]<[-<+++++ +++++>]<");
  
  return k;
}


//if sarti. else lazim ise alttakini kullanin

// ifBegin(int i, int j, CMP type);
//   ... if dogru ise calisacak kodlar
// ifEnd();

void ifBegin(int i, int j, CMP type){
  s++;
  int k = compare(i, j, type);
  stack[st++] = k;
  move(k);
  print("[-");
}

void ifEnd(){
  move(stack[--st]);
  print("]");
  pop();
  while(stackSize[s])pop();
  s--;
}


//ifelse yapmak icin bu if i kullanin
//kullanim sekli

// ifElse_if(int i, int j, CMP type);
//   ... if dogru ise calisacak kodlar
// ifElse_else();
//   ... if yanlis ise calisacak kodlar
// ifElse_end();
void ifElse_if(int i, int j, CMP type){
  s++;
  int k = compare(i, j, type);
  copy(k);
  stack[st++] = k;
  move(k);
  print(">-<[-");
}
void ifElse_else(){
  move(stack[st-1]);
  print("]>[+<");
  while(stackSize[s]>2)pop();
}
void ifElse_end(){
  move(stack[--st]);
  print(">]<");
  pop();
  pop();
  printf("%d %d\n", s, stackSize[s]);
  
  while(stackSize[s])pop();
  s--;
}


//gerek duydugum bir operatordu cell[i]%2 yi push ediyor
//direk modunu almaktan daha temiz
int pushModByTwo(int i){
  int k = copy(i);
  move(k);
  //ic1
  print("[->>+<<[-[->+<]>>-<<]>[-<+>]<]>[-]>[-<<+>>]<<");
  return k;
}
//yukardakinin zitti. 1-cell[i]%2 pushluyor
int pushNotModByTwo(int i){
  int k = copy(i);
  move(k);
  //ic1
  print(">>+<<[->>-<<[-[->+<]>>+<<]>[-<+>]<]>[-]>[-<<+>>]<<");
  return k;
}




//do while dongusu, while dan daha optimize ama whileNotZero kadar degil
void doWhileBegin(int i, int j, CMP type){
  
  s++;
  int k = push(1);
  
  wstack[wst].k = k;
  wstack[wst].i = i;
  wstack[wst].j = j;
  wstack[wst].type = type;
  wst++;
  move(k);
  print("[-");
  
}
void doWhileEnd(){
  printf("%d", stackSize[s]);
  pop();
  wst--;
  
  while(stackSize[s])pop();
  s--;
  int k = compare(wstack[wst].i, wstack[wst].j, wstack[wst].type);
  if(k != wstack[wst].k) printf("\n\n%d %d ERRRR\n\n", k, wstack[wst].k);
  move(k);
  
  print("]");
  pop();
}

//while not zero, bir cell 0 olmadigi sürece calisiyor
//en optimize olan dongu bu, direk [] e acilabiliyor
void whileNotZeroBegin(int i){
  s++;
  move(i);
  print("[");
  wstack[wst++].i = i;
}

void whileNotZeroEnd(){
  while(stackSize[s])pop();
  move(wstack[--wst].i);
  s--;
  print("]");
  
}

//while döngüsü
//iki cell i karsilastiriyor, CMP operator ü belirliyor
//zorunlu kalmadikca kullanmayin, test kodunu iki kere bastirmak zorunda kaliyor
//kodu sisirir
void whileBegin(int i, int j, CMP type){
  s++;
  int k = compare(i,j,type);
  
  wstack[wst].k = k;
  wstack[wst].i = i;
  wstack[wst].j = j;
  wstack[wst].type = type;
  wst++;
  move(k);
  print("[-");
}

void whileEnd(){
  return doWhileEnd();  
}

//ekrana string bastirmak icin. optimize edilebilir ama simdilik kalsin gibi
void printStr(char *x){
  unsigned char *c = (unsigned char*)x;
  int i;
  int k = push(c[0]);
  move(k);
  print(".");
  i=1;
  while(c[i]){
    if(c[i] < c[i-1]) dec(k, c[i-1]-c[i]);           
    else if(c[i] > c[i-1]) inc(k, c[i]-c[i-1]);  
    print(".");      
    i++;
  }
  popAndClear();
}


//bir cell i karakter olarak bastirmak icin
//direk o cell e move edip . koyuyor
void printCh(int i){
  move(i);
  print(".");
}


//son kod üzerinde biraz optimzasyon yapiyor. problem cikarmamasi lazim
int optimize(){
  char *c, *c2;
  c=buff;
  
  int p=0;
  int cl[100];
  for(int i=0; i<100; i++){
    cl[i] = 0;
  }
  
  //burada zaten clear edilmis cell leri tekrar clear etmemeye calisiyoruz
  //cl array i 1 ise o cell kesin 0dir demek
  while(c<cur-2){
    
    
    //eger su anki cell zaten 0 ise [-] i cikarabilirsin
    if(strncmp(c, "[-]", 3) == 0 && cl[p] == 0){
      *c=*(c+1)=*(c+2)=' ';  
      c+=2;
    }
    //diger komutlar cell i kaydiriyor
    else if(*c == '>') p++;
    else if(*c == '<') p--;
    //bu komutlardan biri cell in sifirdan farkli bir degere gecegini gosteriyor
    else if(*c == '+' || *c == '-' || *c == ',') cl[p] = 1;
    //bu komutlarda cellin sifirlandigini garantiliyor. loop bittikten sonra o cell kesin sifirlanmistir
    else if(*c == ']') cl[p] = 0;
    
    
    c++;
  }
  
  //burada ise eger yanyana <> veya >< ciftleri varsa bunlar kaldiriliyor
  //+- -+ icinde test edebilirdik ama geregi yok, kodun öyle bir sey cikarmamasi lazim
  c = buff;
  while(c-cur){
    if(*c == '<'){
      c2 = c+1;
      while(c2-cur && *c2==' ') c2++;
      if(*c2 == '>'){
        *c=*c2=' ';
        while(c-buff && *c == ' ') c--;
        c--;
      }
    }else if(*c == '>'){
      c2 = c+1;
      while(c2-cur && *c2==' ') c2++;
      if(*c2 == '<'){
        *c=*c2=' ';
        while(c-buff && *c == ' ') c--;
        c--;
      }
    }
    c++;
  }
}

//sonucu düzgün bir kare seklinde bastirmak icin kullanabilirsiniz
void printToFile(char *fname){
  FILE *f = fopen(fname, "w");
  
  char *c = buff;
  
  int a =0;
  while(c-cur){
    if(*c != ' ') a++;
    c++;
  }
  //a toplam karakter sayisi
  //fprintf(f, "%d\n", a);
  a = (int)sqrt((a)/2)*2+1;
  //simdi de her satirin boyutu 
  //fprintf(f, "%d\n", a);
  int b=0;
  c = buff;
  
  
  while(c-cur){
    if(*c != ' '){ putc(*c, f); b++; }
    c++;
    
    if(b==a){
      b=0;
      putc('\n', f);
    }
  }
  
  fclose(f);
}

int main(){
  
  int size = inputInt2();
  int i = push(1);
  whileBegin(i, size, LT);
    int total = copy(i);
    whileNotZeroBegin(total);
      printStr("*");
      dec(total);
    whileNotZeroEnd();
    printStr("\n");
    inc(i);
  whileEnd();
  
  printToFile("out.txt");
  return 0;
}
