#include <stdio.h>
 
const unsigned char *s1 = "G_hello world";
 
unsigned char a = 9;
 
unsigned char* fun(const unsigned char s,const unsigned char *str)
{
    unsigned char *p = (unsigned char*)&s;
    //s = 2;                             //（1）为什么编译错误
    // 因为被const关键字修饰变量，不能出现在赋值符号左边，所以编译出错

    *p = 1;                              //（2）p指针指向s，但是s是const修饰的，不能更改，但是这里为啥能够更改
    printf("s = %d *p = %d\n",s,*p);
    // 因为用const定义变量只是告诉编译器不能出现赋值符号左边，但是本质还是变量，这里就是局部变量，还是可以通过指针修改它的值
    p = (unsigned char*)str;
    
    //*p = '_';                          //（3）为什么编译通过，运行段错误
    // 编译肯定通过，因为p是指针当然可以指向任何地方，运行错误是因为p指针指向的是字符串字面量，
    // 而字符串字面量是存储在全局只读存储区，所以运行错误（具体为什么是全局只读区域，后面我在（11）提问里面会说）
    printf("str = %s\np = %s\n",str,p);
    
    return "ABCDEF GHIJK";               //（4）这种定义的字符串字面量和使用字符指针指向字符串字面量有啥区别，还是一样的？
    // 是一样的
}
 
int main()
{
    const unsigned char *s2 = "Hello world";//（5）s2指针指向的内容能不能更改？
    // 不能更改，因为定义的字符串指针是指向字符串字面量，而字符串字面量存储的区域是全局只读区
    unsigned char *s3  = "LMNOP ORST";      //（6）你们看到s3比s2少了一个const,那编译会不会出错？ s3指向内容能不能更改呢？那和s2有啥区别呢？
    // 和s2没有区别 存储在全局只读区域
    unsigned char *s4  = "LMNOP ORST";      //（7）s4和s3指向的内容都是一样的，那他们地址是不是一样的呢？
    // 通过终端打印我们发现地址居然一样，编译器居然为了节省空间(我猜想的)，只存储一个"LMNOP ORST"，
    // 当然他们都是存在只读内存空间，不能更改，比较安全，如果是可更改空间，那可就出大事了，修改其中一个内容值，另外一个变量内容也跟着更改了
    unsigned char s5[] = "LMNOP ORST";      //（8）s5和s3一个是数组，一个是指针，那他们有啥区别呢？而且他们内容也是相同的，那他们的地址是不是也是一样的呢
    // 首先s3是字符指针，指向内容是一个字符串字面量，而且s3指向内容的区域是全局只读区域，所以不能更改，而s5是数组是可以更改的，而且s5是局部的，
    // 也就是存储在栈中，临时分配的内存，
    // 函数执行完释放掉，同时通过终端打印我们也发现s3和s5内存地址也是完全不一样的，相差很多，因为一个是全局只读区域，另一个是局部内存区域
    const unsigned char s6[] = "LMNOP ORST";//（9）s6比s5多了一个const，多了这个导致有啥区别么？
    // s5可以直接更改，就是s[0] = 'A';,而s6是不能直接更改的，s6[0] = 'A'编译器在编译时就会报错，
    // 但是他们都是存储在局部内存区域(就是栈)，这个区域是可以通过指针进行更改的，所以s6还是可以更改的，通过终端打印发现他们地址也很接近

    const unsigned char i = 2;
    const static unsigned char j = 3;       //（10）j比i多了一个static，多了这个导致有啥区别么？
    // j和i的区别是，j存储在全局只读区域，不能更改，i是存储在栈中，是可以更改的，
    // 但是不能直接更改，必须通过指针进行更改，通过终端打印也发现，j的地址和s1、a的地址都很接近，所以j肯定是全局只读区域
    unsigned char *pc = fun(i,s2);          
    
    printf("&i = %p &a = %p\n",&i,&a);
    printf("&j = %p j = %d\n",&j,j);
    printf("s1 = %p s2 = %p\n",s1,s2);
    printf("s3 = %p s4 = %p\n",s3,s4); 
    printf("s5 = %p s5 = %s\n",s5,s5);          
    printf("s6 = %p s6 = %s\n",s6,s6); 
    printf("pc = %p\npc = %s\n",pc,pc);
    
    //（11）通过观测这么多变量，字符指针，数组你发现什么规律没(从地址去观察)
    
    //j = 4;                               //编译出错，我们通过终端打印发现j是存储在全局只读区域中，所以不能更改
    pc = &j;                               //编译出现警告，运行通过，因为指针可以指向任何地方
    //*pc = 5;                             //编译通过，运行段错误，因为pc指向的是全局只读区域，所以不能更改
    
    //*pc = '!';                           //（12）编译通过，为什么运行段错误
    // 因为pc指针指向fun函数返回的内容是存储在全局只读区域，不能更改，所以运行错误
    //*s2 = '_';                           //（13）为什么编译错误
    // 因为const定义变量是不能出现在赋值符号左边，而且s2指针，
    // 指向的内容是字符串字面量，是存储在全局只读区域内，是不能更改的
    pc = s2;    //编译出现警告，因为类型不一样
    
    //*pc = '$';                           //（13）编译通过，为什么运行段错误
    
    //*s3 = 'A';                           //（14）编译通过，为什么运行段错误
    // 因为s3指针，指向的内容是字符串字面量，是存储在全局只读区域内，是不能更改的
    pc = s3;
    //*pc = '_';                           //（15）编译通过，为什么运行段错误
    // 同上
    
    printf("更改前：s5 = %s\n",s5);
    s5[0] = 'A';
    pc = s5;
    *(pc + 1) = 'B';
    printf("更改后：pc = %s s5 = %s\n",pc,s5);
    
    printf("更改前：s6 = %s\n",s6);
    //s6[0] = 'A';                          //（16）为什么不能更改
    // 因为s6是const关键字定义的局部变量，是不能出现在赋值符号左边，
    // 但是可以更改，不能这样直接更改，需要用指针进行更改
    pc = &s6[0];
    *pc = 'A';                              //（17）为什么用一个指针却可以更改s6呢，再从地址观察s5和s6，有啥发现
    // 通过终端打印发现s5和s6地址很接近，因为他们都是局部变量，存储在栈中，但是因为s6是用const关键字定义变量，
    // 是不能出现在赋值符号左边的，但是又因为s6是存储在局部变量区域，所以可以通过指针进行更改
    printf("更改后：pc = %s s6 = %s\n",pc,s6);
    
    return 0;
}