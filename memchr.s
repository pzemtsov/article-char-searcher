Dump of assembler code for function memchr:
   0x00007ffff7aa7790 <+0>:     movq   xmm1,rsi
   0x00007ffff7aa7795 <+5>:     mov    rcx,rdi
   0x00007ffff7aa7798 <+8>:     punpcklbw xmm1,xmm1
   0x00007ffff7aa779c <+12>:    test   rdx,rdx
   0x00007ffff7aa779f <+15>:    je     0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa77a5 <+21>:    punpcklbw xmm1,xmm1
   0x00007ffff7aa77a9 <+25>:    and    rcx,0x3f
   0x00007ffff7aa77ad <+29>:    pshufd xmm1,xmm1,0x0
   0x00007ffff7aa77b2 <+34>:    cmp    rcx,0x30
   0x00007ffff7aa77b6 <+38>:    ja     0x7ffff7aa7800 <memchr+112>
   0x00007ffff7aa77b8 <+40>:    movdqu xmm0,XMMWORD PTR [rdi]
   0x00007ffff7aa77bc <+44>:    pcmpeqb xmm0,xmm1
   0x00007ffff7aa77c0 <+48>:    pmovmskb eax,xmm0
   0x00007ffff7aa77c4 <+52>:    test   eax,eax
   0x00007ffff7aa77c6 <+54>:    jne    0x7ffff7aa7a90 <memchr+768>
   0x00007ffff7aa77cc <+60>:    sub    rdx,0x10
   0x00007ffff7aa77d0 <+64>:    jbe    0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa77d6 <+70>:    add    rdi,0x10
   0x00007ffff7aa77da <+74>:    and    rcx,0xf
   0x00007ffff7aa77de <+78>:    and    rdi,0xfffffffffffffff0
   0x00007ffff7aa77e2 <+82>:    add    rdx,rcx
   0x00007ffff7aa77e5 <+85>:    sub    rdx,0x40
   0x00007ffff7aa77e9 <+89>:    jbe    0x7ffff7aa79b0 <memchr+544>
   0x00007ffff7aa77ef <+95>:    jmp    0x7ffff7aa7850 <memchr+192>
   0x00007ffff7aa77f1 <+97>:    data32 data32 data32 data32 data32 nop WORD PTR cs:[rax+rax*1+0x0]
   0x00007ffff7aa7800 <+112>:   and    rcx,0xf
   0x00007ffff7aa7804 <+116>:   and    rdi,0xfffffffffffffff0
   0x00007ffff7aa7808 <+120>:   movdqa xmm0,XMMWORD PTR [rdi]
   0x00007ffff7aa780c <+124>:   pcmpeqb xmm0,xmm1
   0x00007ffff7aa7810 <+128>:   pmovmskb eax,xmm0
   0x00007ffff7aa7814 <+132>:   sar    eax,cl
   0x00007ffff7aa7816 <+134>:   test   eax,eax
   0x00007ffff7aa7818 <+136>:   je     0x7ffff7aa7830 <memchr+160>
   0x00007ffff7aa781a <+138>:   bsf    eax,eax
   0x00007ffff7aa781d <+141>:   sub    rdx,rax
   0x00007ffff7aa7820 <+144>:   jbe    0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa7826 <+150>:   add    rax,rdi
   0x00007ffff7aa7829 <+153>:   add    rax,rcx
   0x00007ffff7aa782c <+156>:   ret    
   0x00007ffff7aa782d <+157>:   nop    DWORD PTR [rax]
   0x00007ffff7aa7830 <+160>:   add    rdx,rcx
   0x00007ffff7aa7833 <+163>:   sub    rdx,0x10
   0x00007ffff7aa7837 <+167>:   jbe    0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa783d <+173>:   add    rdi,0x10
   0x00007ffff7aa7841 <+177>:   sub    rdx,0x40
   0x00007ffff7aa7845 <+181>:   jbe    0x7ffff7aa79b0 <memchr+544>
   0x00007ffff7aa784b <+187>:   nop    DWORD PTR [rax+rax*1+0x0]
   0x00007ffff7aa7850 <+192>:   movdqa xmm0,XMMWORD PTR [rdi]
   0x00007ffff7aa7854 <+196>:   pcmpeqb xmm0,xmm1
   0x00007ffff7aa7858 <+200>:   pmovmskb eax,xmm0
   0x00007ffff7aa785c <+204>:   test   eax,eax
   0x00007ffff7aa785e <+206>:   jne    0x7ffff7aa7a60 <memchr+720>
   0x00007ffff7aa7864 <+212>:   movdqa xmm2,XMMWORD PTR [rdi+0x10]
   0x00007ffff7aa7869 <+217>:   pcmpeqb xmm2,xmm1
   0x00007ffff7aa786d <+221>:   pmovmskb eax,xmm2
   0x00007ffff7aa7871 <+225>:   test   eax,eax
   0x00007ffff7aa7873 <+227>:   jne    0x7ffff7aa7a70 <memchr+736>
   0x00007ffff7aa7879 <+233>:   movdqa xmm3,XMMWORD PTR [rdi+0x20]
   0x00007ffff7aa787e <+238>:   pcmpeqb xmm3,xmm1
   0x00007ffff7aa7882 <+242>:   pmovmskb eax,xmm3
   0x00007ffff7aa7886 <+246>:   test   eax,eax
   0x00007ffff7aa7888 <+248>:   jne    0x7ffff7aa7a80 <memchr+752>
   0x00007ffff7aa788e <+254>:   movdqa xmm4,XMMWORD PTR [rdi+0x30]
   0x00007ffff7aa7893 <+259>:   pcmpeqb xmm4,xmm1
   0x00007ffff7aa7897 <+263>:   add    rdi,0x40
   0x00007ffff7aa789b <+267>:   pmovmskb eax,xmm4
   0x00007ffff7aa789f <+271>:   test   eax,eax
   0x00007ffff7aa78a1 <+273>:   jne    0x7ffff7aa7a50 <memchr+704>
   0x00007ffff7aa78a7 <+279>:   test   rdi,0x3f
   0x00007ffff7aa78ae <+286>:   je     0x7ffff7aa7920 <memchr+400>
   0x00007ffff7aa78b0 <+288>:   sub    rdx,0x40
   0x00007ffff7aa78b4 <+292>:   jbe    0x7ffff7aa79b0 <memchr+544>
   0x00007ffff7aa78ba <+298>:   movdqa xmm0,XMMWORD PTR [rdi]
   0x00007ffff7aa78be <+302>:   pcmpeqb xmm0,xmm1
   0x00007ffff7aa78c2 <+306>:   pmovmskb eax,xmm0
   0x00007ffff7aa78c6 <+310>:   test   eax,eax
   0x00007ffff7aa78c8 <+312>:   jne    0x7ffff7aa7a60 <memchr+720>
   0x00007ffff7aa78ce <+318>:   movdqa xmm2,XMMWORD PTR [rdi+0x10]
   0x00007ffff7aa78d3 <+323>:   pcmpeqb xmm2,xmm1
   0x00007ffff7aa78d7 <+327>:   pmovmskb eax,xmm2
   0x00007ffff7aa78db <+331>:   test   eax,eax
   0x00007ffff7aa78dd <+333>:   jne    0x7ffff7aa7a70 <memchr+736>
   0x00007ffff7aa78e3 <+339>:   movdqa xmm3,XMMWORD PTR [rdi+0x20]
   0x00007ffff7aa78e8 <+344>:   pcmpeqb xmm3,xmm1
   0x00007ffff7aa78ec <+348>:   pmovmskb eax,xmm3
   0x00007ffff7aa78f0 <+352>:   test   eax,eax
   0x00007ffff7aa78f2 <+354>:   jne    0x7ffff7aa7a80 <memchr+752>
   0x00007ffff7aa78f8 <+360>:   movdqa xmm3,XMMWORD PTR [rdi+0x30]
   0x00007ffff7aa78fd <+365>:   pcmpeqb xmm3,xmm1
   0x00007ffff7aa7901 <+369>:   pmovmskb eax,xmm3
   0x00007ffff7aa7905 <+373>:   add    rdi,0x40
   0x00007ffff7aa7909 <+377>:   test   eax,eax
   0x00007ffff7aa790b <+379>:   jne    0x7ffff7aa7a50 <memchr+704>
   0x00007ffff7aa7911 <+385>:   mov    rcx,rdi
   0x00007ffff7aa7914 <+388>:   and    rdi,0xffffffffffffffc0
   0x00007ffff7aa7918 <+392>:   and    rcx,0x3f
   0x00007ffff7aa791c <+396>:   add    rdx,rcx
   0x00007ffff7aa791f <+399>:   nop
   0x00007ffff7aa7920 <+400>:   sub    rdx,0x40
   0x00007ffff7aa7924 <+404>:   jbe    0x7ffff7aa79b0 <memchr+544>
   0x00007ffff7aa792a <+410>:   movdqa xmm0,XMMWORD PTR [rdi]
   0x00007ffff7aa792e <+414>:   movdqa xmm2,XMMWORD PTR [rdi+0x10]
   0x00007ffff7aa7933 <+419>:   movdqa xmm3,XMMWORD PTR [rdi+0x20]
   0x00007ffff7aa7938 <+424>:   movdqa xmm4,XMMWORD PTR [rdi+0x30]
   0x00007ffff7aa793d <+429>:   pcmpeqb xmm0,xmm1
   0x00007ffff7aa7941 <+433>:   pcmpeqb xmm2,xmm1
   0x00007ffff7aa7945 <+437>:   pcmpeqb xmm3,xmm1
   0x00007ffff7aa7949 <+441>:   pcmpeqb xmm4,xmm1
   0x00007ffff7aa794d <+445>:   pmaxub xmm3,xmm0
   0x00007ffff7aa7951 <+449>:   pmaxub xmm4,xmm2
   0x00007ffff7aa7955 <+453>:   pmaxub xmm4,xmm3
   0x00007ffff7aa7959 <+457>:   pmovmskb eax,xmm4
   0x00007ffff7aa795d <+461>:   add    rdi,0x40
   0x00007ffff7aa7961 <+465>:   test   eax,eax
   0x00007ffff7aa7963 <+467>:   je     0x7ffff7aa7920 <memchr+400>
   0x00007ffff7aa7965 <+469>:   sub    rdi,0x40
   0x00007ffff7aa7969 <+473>:   pmovmskb eax,xmm0
   0x00007ffff7aa796d <+477>:   test   eax,eax
   0x00007ffff7aa796f <+479>:   jne    0x7ffff7aa7a60 <memchr+720>
   0x00007ffff7aa7975 <+485>:   pmovmskb eax,xmm2
   0x00007ffff7aa7979 <+489>:   test   eax,eax
   0x00007ffff7aa797b <+491>:   jne    0x7ffff7aa7a70 <memchr+736>
   0x00007ffff7aa7981 <+497>:   movdqa xmm3,XMMWORD PTR [rdi+0x20]
   0x00007ffff7aa7986 <+502>:   pcmpeqb xmm3,xmm1
   0x00007ffff7aa798a <+506>:   pcmpeqb xmm1,XMMWORD PTR [rdi+0x30]
   0x00007ffff7aa798f <+511>:   pmovmskb eax,xmm3
   0x00007ffff7aa7993 <+515>:   test   eax,eax
   0x00007ffff7aa7995 <+517>:   jne    0x7ffff7aa7a80 <memchr+752>
   0x00007ffff7aa799b <+523>:   pmovmskb eax,xmm1
   0x00007ffff7aa799f <+527>:   bsf    eax,eax
   0x00007ffff7aa79a2 <+530>:   lea    rax,[rdi+rax*1+0x30]
   0x00007ffff7aa79a7 <+535>:   ret    
   0x00007ffff7aa79a8 <+536>:   nop    DWORD PTR [rax+rax*1+0x0]
   0x00007ffff7aa79b0 <+544>:   add    rdx,0x20
   0x00007ffff7aa79b4 <+548>:   jle    0x7ffff7aa7a20 <memchr+656>
   0x00007ffff7aa79b6 <+550>:   movdqa xmm0,XMMWORD PTR [rdi]
   0x00007ffff7aa79ba <+554>:   pcmpeqb xmm0,xmm1
   0x00007ffff7aa79be <+558>:   pmovmskb eax,xmm0
   0x00007ffff7aa79c2 <+562>:   test   eax,eax
   0x00007ffff7aa79c4 <+564>:   jne    0x7ffff7aa7a60 <memchr+720>
   0x00007ffff7aa79ca <+570>:   movdqa xmm2,XMMWORD PTR [rdi+0x10]
   0x00007ffff7aa79cf <+575>:   pcmpeqb xmm2,xmm1
   0x00007ffff7aa79d3 <+579>:   pmovmskb eax,xmm2
   0x00007ffff7aa79d7 <+583>:   test   eax,eax
   0x00007ffff7aa79d9 <+585>:   jne    0x7ffff7aa7a70 <memchr+736>
   0x00007ffff7aa79df <+591>:   movdqa xmm3,XMMWORD PTR [rdi+0x20]
   0x00007ffff7aa79e4 <+596>:   pcmpeqb xmm3,xmm1
   0x00007ffff7aa79e8 <+600>:   pmovmskb eax,xmm3
   0x00007ffff7aa79ec <+604>:   test   eax,eax
   0x00007ffff7aa79ee <+606>:   jne    0x7ffff7aa7ab0 <memchr+800>
   0x00007ffff7aa79f4 <+612>:   sub    rdx,0x10
   0x00007ffff7aa79f8 <+616>:   jle    0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa79fe <+622>:   pcmpeqb xmm1,XMMWORD PTR [rdi+0x30]
   0x00007ffff7aa7a03 <+627>:   pmovmskb eax,xmm1
   0x00007ffff7aa7a07 <+631>:   test   eax,eax
   0x00007ffff7aa7a09 <+633>:   jne    0x7ffff7aa7ac0 <memchr+816>
   0x00007ffff7aa7a0f <+639>:   xor    rax,rax
   0x00007ffff7aa7a12 <+642>:   ret    
   0x00007ffff7aa7a13 <+643>:   data32 data32 data32 nop WORD PTR cs:[rax+rax*1+0x0]
   0x00007ffff7aa7a20 <+656>:   add    rdx,0x20
   0x00007ffff7aa7a24 <+660>:   movdqa xmm0,XMMWORD PTR [rdi]
   0x00007ffff7aa7a28 <+664>:   pcmpeqb xmm0,xmm1
   0x00007ffff7aa7a2c <+668>:   pmovmskb eax,xmm0
   0x00007ffff7aa7a30 <+672>:   test   eax,eax
   0x00007ffff7aa7a32 <+674>:   jne    0x7ffff7aa7a90 <memchr+768>
   0x00007ffff7aa7a34 <+676>:   sub    rdx,0x10
   0x00007ffff7aa7a38 <+680>:   jbe    0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa7a3e <+686>:   pcmpeqb xmm1,XMMWORD PTR [rdi+0x10]
   0x00007ffff7aa7a43 <+691>:   pmovmskb eax,xmm1
   0x00007ffff7aa7a47 <+695>:   test   eax,eax
   0x00007ffff7aa7a49 <+697>:   jne    0x7ffff7aa7aa0 <memchr+784>
   0x00007ffff7aa7a4b <+699>:   xor    rax,rax
   0x00007ffff7aa7a4e <+702>:   ret    
   0x00007ffff7aa7a4f <+703>:   nop
   0x00007ffff7aa7a50 <+704>:   bsf    eax,eax
   0x00007ffff7aa7a53 <+707>:   lea    rax,[rax+rdi*1-0x10]
   0x00007ffff7aa7a58 <+712>:   ret    
   0x00007ffff7aa7a59 <+713>:   nop    DWORD PTR [rax+0x0]
   0x00007ffff7aa7a60 <+720>:   bsf    eax,eax
   0x00007ffff7aa7a63 <+723>:   add    rax,rdi
   0x00007ffff7aa7a66 <+726>:   ret    
   0x00007ffff7aa7a67 <+727>:   nop    WORD PTR [rax+rax*1+0x0]
   0x00007ffff7aa7a70 <+736>:   bsf    eax,eax
   0x00007ffff7aa7a73 <+739>:   lea    rax,[rax+rdi*1+0x10]
   0x00007ffff7aa7a78 <+744>:   ret    
   0x00007ffff7aa7a79 <+745>:   nop    DWORD PTR [rax+0x0]
   0x00007ffff7aa7a80 <+752>:   bsf    eax,eax
   0x00007ffff7aa7a83 <+755>:   lea    rax,[rax+rdi*1+0x20]
   0x00007ffff7aa7a88 <+760>:   ret    
   0x00007ffff7aa7a89 <+761>:   nop    DWORD PTR [rax+0x0]
   0x00007ffff7aa7a90 <+768>:   bsf    eax,eax
   0x00007ffff7aa7a93 <+771>:   sub    rdx,rax
   0x00007ffff7aa7a96 <+774>:   jbe    0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa7a98 <+776>:   add    rax,rdi
   0x00007ffff7aa7a9b <+779>:   ret    
   0x00007ffff7aa7a9c <+780>:   nop    DWORD PTR [rax+0x0]
   0x00007ffff7aa7aa0 <+784>:   bsf    eax,eax
   0x00007ffff7aa7aa3 <+787>:   sub    rdx,rax
   0x00007ffff7aa7aa6 <+790>:   jbe    0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa7aa8 <+792>:   lea    rax,[rdi+rax*1+0x10]
   0x00007ffff7aa7aad <+797>:   ret    
   0x00007ffff7aa7aae <+798>:   xchg   ax,ax
   0x00007ffff7aa7ab0 <+800>:   bsf    eax,eax
   0x00007ffff7aa7ab3 <+803>:   sub    rdx,rax
   0x00007ffff7aa7ab6 <+806>:   jbe    0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa7ab8 <+808>:   lea    rax,[rdi+rax*1+0x20]
   0x00007ffff7aa7abd <+813>:   ret    
   0x00007ffff7aa7abe <+814>:   xchg   ax,ax
   0x00007ffff7aa7ac0 <+816>:   bsf    eax,eax
   0x00007ffff7aa7ac3 <+819>:   sub    rdx,rax
   0x00007ffff7aa7ac6 <+822>:   jbe    0x7ffff7aa7ad0 <memchr+832>
   0x00007ffff7aa7ac8 <+824>:   lea    rax,[rdi+rax*1+0x30]
   0x00007ffff7aa7acd <+829>:   ret    
   0x00007ffff7aa7ace <+830>:   xchg   ax,ax
   0x00007ffff7aa7ad0 <+832>:   xor    rax,rax
   0x00007ffff7aa7ad3 <+835>:   ret    
