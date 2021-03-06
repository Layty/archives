#include "lock.h"

int ii_keysize=5;


//解析逻辑版本
int lock_getCommVersion(char* pdata,char* commVersion){
 int li_len=0,li_1;
 li_len=pdata[HEADLENGTH+495];
 for(li_1=0;li_1<li_len;li_1++){
   commVersion[li_1]=pdata[HEADLENGTH+496+li_1];
 }
 commVersion[li_len] = '\0';

 return 1;
}

//根据当前时间生成格式为yymmdd的时间，这六个数将作为通信随机因子
int lock_getCommRandom(char *commRandom){
	struct tm* tm_strcut;
	time_t timer;
	char *ptime;
	int year,random_length = 6;//时间格式为yymmdd
	
	time(&timer);
	tm_strcut = localtime(&timer);

	ptime = (char *)malloc((random_length+1)*sizeof(char));
	year = 1900 + tm_strcut->tm_year -2000;//从年份格式yyyy截取最后两位yy，临时解决方案
	sprintf(ptime,"%02d%02d%02d", year, 1 + tm_strcut->tm_mon, tm_strcut->tm_mday);
	ptime[random_length] = '\0';
	memcpy(commRandom,ptime,random_length);	
	free(ptime);
	return 1;
}

//从报文pdata中取出通信随机因子
int lock_getCommRandom_v2(char * pdata,char *commRandom){
	int li_1,li_len=0;
	char* comm;
		
	li_len=pdata[HEADLENGTH+466];
	comm = (char *)malloc((li_len+1)*sizeof(char));
	memset(comm, 0, (li_len+1)*sizeof(char));
	comm[li_len]='\0';
	
	for(li_1=0;li_1<li_len;li_1++){
		comm[li_1]=pdata[HEADLENGTH+467+li_1];
	}
	memcpy(commRandom,comm,strlen(comm));	
	free(comm);
	return 1;
}

int lock_random_range(int ai_1,int ai_2){
	struct timeval tpstart;
	int n;
	float i;
  if (ai_1>ai_2) return -1;
  	
  gettimeofday(&tpstart,NULL);
  srand(tpstart.tv_usec);
  i = ai_2 - ai_1;
  n = (1+(int)(i*rand()/(RAND_MAX+1.0)));
  return n;	
}

int lock_random(int ai_len,char* str){
	int li_1;
	int t;
	char *buf;
	char tmp[2]={'\0'};
 
	buf = (char*)malloc((ai_len+1)*sizeof(char));
	memset(buf,0,(ai_len+1));
	buf[ai_len]='\0';
  for(li_1=0;li_1<ai_len;li_1++){
  	t = lock_random_range(0,9);
  	sprintf(tmp,"%01x",t);
  	strcat(buf,tmp);
  }
  memcpy(str,buf,strlen(buf));
  free(buf);
  return 1;
}

//计算字符串的md5值
int	lock_md5(char* as_question,char* as_answer){
	struct MD5Context md5c;
	char *s;
	unsigned char ss[16];
	char buf[33]={'\0'};
	char tmp[3]={'\0'};
	int i;
	
	s = as_question;
	
	MD5Init( &md5c );
	MD5Update( &md5c, s, strlen(s) );
	MD5Final( ss, &md5c );
	
	for( i=0; i<16; i++ ){
	sprintf(tmp,"%02X", ss[i] );
	strcat(buf,tmp);
	}
	memcpy(as_answer,buf,strlen(buf));
	return 1;
}

//将报文每个字节与一个随机因子与或操作，作为第一道加密
int lock_xor(char* pdata,int ai_len,char* pkey){
		int li_1;
    for(li_1=0;li_1<ai_len;li_1++){
    	pdata[li_1]= (char)((byte)pdata[li_1] ^ pkey[li_1 % ii_keysize]);
    }
    return 1;
}

/*字符串增加
as_text:源字符串
ai_lx:操作类型1--字母数字同时加,2--字母加,3--数字加
ai_zl:增量
*/
byte lock_add(byte as_text,int ai_lx,int ai_zl){
	int li_1;
	if (ai_zl==0) return as_text; 
	if(as_text==(byte)'@') return as_text;
	if(as_text==(byte)'|') return as_text;
	if (ai_zl>1){
	   for(li_1=0;li_1<ai_zl;li_1++){
	       as_text=lock_add(as_text,ai_lx,1);
	   }
	   return as_text;
	}
	if (ai_zl<-1){
	   for(li_1=ai_zl;li_1<0;li_1++){
	       as_text=lock_add(as_text,ai_lx,-1);
	   }
	   return as_text;
	}
	byte lb_byte=as_text;
	switch (ai_lx) {
	case 1:
	       if (lb_byte>=48 && lb_byte<=57){
	           if(ai_zl==1){
	               if (lb_byte == 57) {
	                   lb_byte= 65;
	               } else {
	                   lb_byte=(byte) (lb_byte + 1);
	               }
	               break;
	           }else{
	               if (lb_byte == 48) {
	                   lb_byte= 90;
	               } else {
	                   lb_byte=(byte) (lb_byte - 1);
	               }
	               break;
	           }
	       }else if (lb_byte>=65 && lb_byte<=90){
	           if(ai_zl==1){
	               if (lb_byte == 90) {
	                   lb_byte= 48;
	               } else {
	                   lb_byte= (byte) (lb_byte + 1);
	               }
	               break;
	           }else{
	               if (lb_byte == 65) {
	                   lb_byte= 57;
	               } else {
	                   lb_byte= (byte) (lb_byte - 1);
	               }
	               break;
	           }
	       }else{
	        //lb_byte = (lb_byte ^ OR_ADD);
	        //    if ((lb_byte >= 48 && lb_byte <= 57) ||
	         //       (lb_byte >= 65 && lb_byte <= 90) ||
	          //      (lb_byte >= 97 && lb_byte <= 122)) {
	          //      lb_byte = (byte) (lb_byte ^ OR_ADD);
	          //  }
	       }
	       break;
	   case 2:
	      if (lb_byte>=65 && lb_byte<=90){
	          if(ai_zl==1){
	              if (lb_byte == 90) {
	                  lb_byte= 65;
	              } else {
	                  lb_byte= (byte) (lb_byte + 1);
	              }
	              break;
	          }else{
	              if (lb_byte == 65) {
	                  lb_byte= 90;
	              } else {
	                  lb_byte= (byte) (lb_byte - 1);
	              }
	              break;
	          }
	      }else if (lb_byte>=97 && lb_byte<=122){
	          if(ai_zl==1){
	              if (lb_byte == 122) {
	                  lb_byte= 97;
	              } else {
	                 lb_byte=(byte) (lb_byte + 1);
	              }
	              break;
	          }else{
	              if (lb_byte == 97) {
	                  lb_byte= 122;
	              } else {
	                  lb_byte= (byte) (lb_byte - 1);
	              }
	              break;
	          }
	
	      }else{
	        //lb_byte= (byte) (lb_byte ^ OR_ADD);
	         //   if ((lb_byte >= 48 && lb_byte <= 57) ||
	         //       (lb_byte >= 65 && lb_byte <= 90) ||
	         //       (lb_byte >= 97 && lb_byte <= 122)) {
	          //      lb_byte = (byte) (lb_byte ^ OR_ADD);
	          //  }
	       }
	       break;
	  case 3:
	      if (lb_byte>=48 && lb_byte<=57){
	          if(ai_zl==1){
	              if (lb_byte == 57) {
	                  lb_byte= 48;
	              } else {
	                  lb_byte= (byte) (lb_byte + 1);
	              }
	              break;
	          }else{
	              if (lb_byte == 48) {
	                  lb_byte= 57;
	              } else {
	                  lb_byte=  (byte) (lb_byte - 1);
	              }
	              break;
	          }
	      }
	      break;
	   }
	return lb_byte;
}

int lockcz(char* as_text,int ai_len,int n1,int m1,int n2,int m2,int li_add1,int li_add2){
		int li_1;
    int li_length=ai_len;
    byte lb_byte=0;
    int li_sz=0,li_zm=0;
    bool lb_sz=true;
    for (li_1=0;li_1<li_length;li_1++){
        lb_byte=(byte)as_text[li_1];
        if (lb_byte>=48 && lb_byte<=57){
            li_sz++;
            lb_sz=true;
        }else{
            li_zm++;
            lb_sz=false;
        }
        if (li_sz==n1){
            as_text[li_1]=lock_add(lb_byte,3,m1);
        }
        if (li_zm==n2){
          as_text[li_1]=lock_add(lb_byte,2,m2);
        }
        if (lb_sz){
              if(li_sz%2==0 && li_sz>0){
                      as_text[li_1]=lock_add(lb_byte,3,li_add1);
              }else{
                      as_text[li_1]=lock_add(lb_byte,3,li_add2);
              }
        }else{
              if(li_zm%2==0 && li_zm>0 ){
                      as_text[li_1]=lock_add(lb_byte,2,li_add1);
              }else{
                      as_text[li_1]=lock_add(lb_byte,2,li_add2);
              }
        }
    }
    return 1;
}

//设置通讯随机因子
int lock_setCommRandom(char* pdata,char* as_commrandom){
	int li_1;
	int li_len= strlen(as_commrandom);
	pdata[HEADLENGTH+466]=(char)li_len;
	for(li_1=0;li_1<li_len;li_1++){
	 pdata[HEADLENGTH+467+li_1]=as_commrandom[li_1];
	}
	return 1;
}

//设置逻辑版本
int lock_setCommVersion(char* pdata,char* as_commversion){
	int li_1;
	int li_len= strlen(as_commversion);

	pdata[HEADLENGTH+495]=(char) li_len;
	for(li_1=0;li_1<li_len;li_1++){
	 pdata[HEADLENGTH+496+li_1]=as_commversion[li_1];
	}
	
	return 1;
}

//得到逻辑问题,
int lock_getCommQuestion(char* as_commversion,char* as_question){
	char* str="zbn";
	if(strcmp(as_commversion, "1.0")== 0){
		memcpy(as_question,str,strlen(str));
		as_question[strlen(str)]='\0';
	}else if(strcmp(as_commversion, "2.0")== 0){
	 //随机数
		lock_random(9,as_question);
	}else{
		//return uf_getCommQuestion("2.0");
	}
  return 1;
}  

//设置逻辑问题
int lock_setCommQuestion(char* pdata,char* as_question){
	int li_1;
	int li_len= strlen(as_question);
	
	pdata[HEADLENGTH+475]=(char)li_len;
	for(li_1=0;li_1<li_len;li_1++){
	 pdata[HEADLENGTH+476+li_1]=as_question[li_1];
	}
	return 1;
}

//根据问题获得答案
int lock_getCommAnswer(char* as_commversion,char* as_question,char* as_answer){
		char* str="zbn";
		if(strcmp(as_commversion, "1.0")== 0){
			memcpy(as_answer,str,strlen(str));
			as_question[strlen(str)]='\0';
		}else if(strcmp(as_commversion, "2.0")== 0){
		 //随机数
			lock_md5(as_question,as_answer);
		}else{
			//return uf_getCommQuestion("2.0");
		}
	  return 1;
} 

//将答案写入报文
int lock_setCommAnswer(char* pdata,char* as_answer){
	int li_1;
	int li_len= strlen(as_answer);
	
  pdata[HEADLENGTH+433]=(char) li_len;
	for(li_1=0;li_1<li_len;li_1++){
	 pdata[HEADLENGTH+434+li_1]=as_answer[li_1];
	}
   return 1;
}

//获得逻辑口令,这个逻辑口令，目前固定值为zbn
int lock_getCommPWD(char* as_pwd){
 char* pwd="zbn";
 memcpy(as_pwd,pwd,strlen(pwd));
 return 1;
}  

//将逻辑口令写入报文
int lock_setCommPWD(char* pdata,char* as_pwd){
	int li_1;
	int li_len= strlen(as_pwd);
	
	pdata[HEADLENGTH+423]=(char) li_len;
	for(li_1=0;li_1<li_len;li_1++){
	 pdata[HEADLENGTH+424+li_1]=as_pwd[li_1];
	}
 return 1;
}

/*获得随机密钥*/
int lock_getRandomKey(char* pkey2) {
	int li_1;
	char lb_byte[6]={'\0'};
	for (li_1 = 0; li_1 < ii_keysize; li_1++) {
		lb_byte[li_1] = (char)lock_random_range(0,127);
	}
	memcpy(pkey2,lb_byte,strlen(lb_byte));
	return 1;
}

/*数据库加密
  第n1位数字加m1，第n2位字母加m2
  数字位加3，偶数位加1
  字母奇数位加3，偶数位加1
  更正：
  第n1位数字加m1，第n2位字母加m2
  奇数位数字加3，偶数位数字加2
  奇数位字母加3，偶数位字母加2
*/
int lock(char* pdata,int ai_len,char* pkey){
	int n1=1,m1=4,n2=2,m2=5;
	int li_add1=2,li_add2=3;

	lock_xor(pdata,ai_len,pkey);
  int i = 0;
	lockcz(pdata,ai_len,n1,m1,n2,m2,li_add1,li_add2);
  return 1;
}

int encrypt_data(char* pdata,char* as_commversion,char* as_commrandom,int datalen,char* pkey,int ai_len){
  int li_1,li_2;
  char ls_question[10]={'\0'};
  char ls_answer[33]={'\0'};
  char ls_pwd[4]={'\0'};
  char pkey2[6]={'\0'};
  //填充随机信息
  for(li_1=HEADLENGTH+399;li_1<datalen;li_1++){
    pdata[li_1]=(char)lock_random_range(0,127);
  }
  
  //设置通讯随机因子(467-475)
  lock_setCommRandom(pdata,as_commrandom);
 
  //设置逻辑版本
  lock_setCommVersion(pdata,as_commversion);
  
  //设置question,question为包含9个随机字符的字符串
  lock_getCommQuestion(as_commversion,ls_question);
  lock_setCommQuestion(pdata,ls_question);
  
  //设置answer
  lock_getCommAnswer(as_commversion,ls_question,ls_answer);
  lock_setCommAnswer(pdata,ls_answer);

 	//设置pwd
  lock_getCommPWD(ls_pwd);
  lock_setCommPWD(pdata,ls_pwd);
  
 	
  //用随机密钥加密
  lock_getRandomKey(pkey2);

#ifdef LOCK_DEBUG
	printf("[encrypt_data] as_commrandom len %d : %s\n",strlen(as_commrandom),as_commrandom);
	printf("[encrypt_data] as_commversion len %d : %s\n",strlen(as_commversion),as_commversion);
	printf("[encrypt_data] ls_answer len %d : %s\n",strlen(ls_answer),ls_answer);
	printf("[encrypt_data] ls_answer len %d : %s\n",strlen(ls_answer),ls_answer);
	printf("[encrypt_data]pkey2 unencrypt len %d %x-%x-%x-%x-%x\n",strlen(pkey2),pkey2[0],pkey2[1],pkey2[2],pkey2[3],pkey2[4]);
#endif 
  
#ifdef DEBUG
  int i = 0;
	printf("################### [encrypt_data] brfore encrypt data / datalen %d \n",datalen);
	while (i < datalen)
	{
		if ((0 != (i%8)) || (0 == i))
		{
			printf("%02x ",(unsigned char)pdata[i]);
			i++;
			continue;
		}
		printf("\n");
		printf("%02x ",(unsigned char)pdata[i]);
		i++;					 
	}
	printf("############################\n");
	printf("\n\n");
#endif
	
  lock(pdata,datalen,pkey2);
  //用原始密钥加密随机密钥(501-502,508-510)
  lock(pkey2,ii_keysize,pkey);
#ifdef LOCK_DEBUG
 	printf("[encrypt_data]pkey2 encrypt len %d %x-%x-%x-%x-%x\n",strlen(pkey2),pkey2[0],pkey2[1],pkey2[2],pkey2[3],pkey2[4]);
#endif
  for(li_1=0;li_1<2;li_1++){
    pdata[HEADLENGTH+500+li_1]=pkey2[li_1];
  }
  for(li_2=2;li_2<5;li_2++){
    pdata[HEADLENGTH+505+li_2]=pkey2[li_2];
  }
  return 1;
}


int unlock(char* as_text,int ai_len,char* OR_KEY3){
    int n1=1,m1=-4,n2=2,m2=-5;
    int li_add1=-2,li_add2=-3;
    lockcz(as_text,ai_len,n1,m1,n2,m2,li_add1,li_add2);
    lock_xor(as_text,ai_len,OR_KEY3);
    
    return 1;
}

int decrypt_data(char* pdata,int datalen,char* pkey,int ai_len){
	int li_1,li_2;
	char* pkey2;
  //用原始密钥解密获得当前密钥
  pkey2 = (char*)malloc(ii_keysize*sizeof(char));
  for(li_1=0;li_1<2;li_1++){
    pkey2[li_1]=pdata[HEADLENGTH+500+li_1];
  }
  for(li_2=2;li_2<5;li_2++){
    pkey2[li_2]=pdata[HEADLENGTH+505+li_2];
  }
 
#ifdef DEBUG
  int i = 0;
	printf("################### [decrypt_data] before decrypt data %d \n",datalen);
	while (i < datalen){
		if ((0 != (i%8)) || (0 == i)){
			printf("%02x ",(unsigned char)pdata[i]);
			i++;
			continue;
		}
		printf("\n");
		printf("%02x ",(unsigned char)pdata[i]);
		i++;					 
	}
	printf("############################\n");
	printf("\n\n");
#endif
	
	//使用公共密钥pkey解密加密数据密钥pkey2
  unlock(pkey2,ii_keysize,pkey);
  //用密钥pkey2解密数据
  unlock(pdata,datalen,pkey2);
  
#ifdef DEBUG
  i = 0;
	printf("################### [decrypt_data] after decrypt data %d \n",datalen);
	while (i < datalen){
		if ((0 != (i%8)) || (0 == i)){
			printf("%02x ",(unsigned char)pdata[i]);
			i++;
			continue;
		}
		printf("\n");
		printf("%02x ",(unsigned char)pdata[i]);
		i++;					 
	}
	printf("############################\n");
	printf("\n\n");
#endif
  return 1;
}