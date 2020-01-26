#include <stdio.h>
#include <string.h>
#include <io.h>
#include "cntt_profile.h"

#define LMEM_FIXED 1

void * LocalAlloc(int type,int size)
{
  return malloc(size);   
}
void LocalFree(void *p)
{
   free(p);   
}

int ReadFile(FILE *fp,void *pointer,int size)
{
    return fread(pointer,1,size,fp);
}
int WriteFile(FILE *fp,void *pointer,int size)
{
    return fwrite(pointer,1,size,fp);
}
int SeekFile(FILE *fp,long offset,int pos)
{  
    return fseek(fp,offset,pos);
}
int GetFileSize(FILE * fp )
{
  int curPos = ftell(fp);
  int size = 0;
  fseek(fp,0,SEEK_END);
  size = ftell(fp);
  fseek(fp,curPos,SEEK_SET);
  return size;  
}
int TruncateFile(FILE *fp, int size )
{
   return _chsize(fileno(fp),size);   
}
int CloseFile(FILE *fp)
{
   return fclose(fp);  
}

long FS_GetAllSectionNames( char *lpReturn, 
												 unsigned long nSize, 
												 char *lpFileName );

long FS_GetAllKeyNamesInSection(						
						char *lpAppName,  /* points to section name */
						char *lpString,		/* pointer to string to add */
						int  nSize,
						char *lpFileName  /* points to initialization filename */
);


/* Delete string function from file system */
static char *CopyString( char *dest, char *src )
{
	char *ret = dest;
	while( *src ) *dest++ = *src++;
	*dest = 0;
	return ret;
}

static char *CopyStringN( char *dest, char *src, int n )
{
	char *ret = dest;
	while( *src && n-- ) *dest++ = *src++;
	*dest = 0;
	return ret;
}

static char *CatString( char *dest, char *src )
{
	char *ret = dest;
	while( *dest != 0 ) dest ++;
	CopyString( dest, src );
	return ret;
}

static int StringLength( char * str )
{
	int nLen = 0;
	while( *str ) nLen++, str++;
	return nLen;
}

static int CompareString( char *str1, char *str2 )
{
	while( *str1 || *str2 )
	{
		if( *str1 > *str2 ) 
			return 1;
		if( *str1 < *str2 )
			return -1;
		str1++;
		str2++;
	}
	return 0;
}

static char ToLower( char c )
{
	if( c >= 'A' && c <= 'Z' )
		return (char)(c + 'a' - 'A');
	else 
		return c;
}

static char ToUpper( char c )
{
	if( c >= 'a' && c <= 'z' )
		return (char)(c + 'A' - 'a');
	else
		return c;
}

static int CompareStringN( char *str1, char *str2, int n )
{
	while( (*str1 || *str2) && n-- )
	{
		if( *str1 > *str2 ) 
			return 1;
		if( *str1 < *str2 )
			return -1;
		str1++;
		str2++;
	}
	return 0;
}

static int CompareStringNoCase( char *str1, char *str2 )
{
	char c1, c2;
	while( (*str1 || *str2) )
	{
		c1 = ToUpper( *str1++ );
		c2 = ToUpper( *str2++ );
		if( c1 > c2 )
			return 1;
		if( c1 < c2 )
			return -1;
	}
	return 0;
}

static int CompareStringNoCaseN( char *str1, char *str2, int n )
{
	char c1, c2;
	while( (*str1 || *str2) && n-- )
	{
		c1 = ToUpper( *str1++ );
		c2 = ToUpper( *str2++ );
		if( c1 > c2 )
			return 1;
		if( c1 < c2 )
			return -1;
	}
	return 0;
}

static char *FindString( char *string, char * substr )
{
	char * ptr;
	int sublen;
	sublen = StringLength( substr );
	ptr = string;
	while( *ptr )
	{
		if( CompareStringN( ptr, substr, sublen ) == 0 )
			return ptr;
		ptr ++;
	}
	return NULL;
}

static char *FindStringNoCase( char *string, char * substr )
{
	char * ptr;
	int sublen;
	sublen = StringLength( substr );
	ptr = string;
	while( *ptr )
	{
		if( CompareStringNoCaseN( ptr, substr, sublen ) == 0 )
			return ptr;
		ptr ++;
	}
	return NULL;
}

static char *FindChar( char *string, char c )
{
	while( *string )
	{
		if( *string == c )
			return string;
		string ++;
	}
	return NULL;
}

static int StringToInt( char *str )
{
	int ret = 0;
	char *temp = str;
	if( *str == '-' || *str == '+')
		str++;
	while( *str >='0' && *str <= '9' )
	{
		ret = ret * 10;
		ret += *str - '0';
		str++;
	}
	if( *temp == '-' )
		ret = -ret;
	return ret;
}

static void IntToString( int nValue, char *buf )
{
	char stack[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int i = 0;
	int temp = nValue;
	if( nValue < 0 )
		temp = -nValue;
	while( temp != 0 )
	{
		stack[i++] = (char)(temp-(temp/10)*10);
		temp = temp / 10;
	}
	temp = 0;
	if( nValue < 0 )
		buf[temp++] = '-';
	i --; 
	while( i >= 0 )
	{
		buf[temp++] = (char)(stack[i--] + '0');
	}
	buf[temp] = 0;
	if(nValue == 0)
	{
       buf[0]='0';
       buf[1]='\0';        
    }
}

/*
 * Profile functions are moved from fileapi.c by xswang.
 */

static int ReadLine( FILE * handle, char *buf, int maxsize )
{
	int i = 0;
	int numRead = 0;
	char c;
	int ret = 1;
	
	do
	{
		ret = ReadFile( handle, &c, 1 );
		if( ret != 1 ) 
			return -1; 
		numRead ++ ;
		if( c > 0x20 ) 
			break;
	} while( ret == 1 );
	buf[i++] = c;
	do
	{
		ret = ReadFile( handle, &c, 1 );
		if( ret != 1 ) break;
		numRead ++;
		if( c == 0x0d  ) 
		{
			/*在一行的结尾遇到回车和换行符时，读字符的个数也要累加，但是并不放到buf中*/
			buf[i++] = 0;

			ret = ReadFile( handle, &c, 1 );
			if( ret )
			{
				if( c != 0x0A )
				{
					SeekFile( handle, -1, SEEK_CUR );
					break;
				}
				numRead ++;
			}
			break;
		}
		buf[i++] = c;
	} while( i < maxsize-1 );

    return numRead;
}

static int FInsert(FILE * handle, int offset, void * data, int length)
{
	int fsize;
	void * tem = NULL;
	fsize = GetFileSize( handle );

	if (length <= 0)
		return 0;

	if( offset < fsize )
	{
		tem = LocalAlloc(LMEM_FIXED, fsize - offset );		
		if (tem==NULL)
		{
			return -1;
		}
		SeekFile( handle, offset, SEEK_SET );		
		if( ReadFile( handle, tem, fsize - offset ) == -1 )
		{
			LocalFree( tem );			
			return -1;
		}
	}
	SeekFile( handle, offset, SEEK_SET );
	WriteFile( handle, data, length );
	if( offset < fsize )
	{
		SeekFile(handle, offset+length, SEEK_SET );
		WriteFile( handle, tem, fsize - offset );
		LocalFree( tem );
	}
	return 0;
}
/**
  delete block from offset and size is length,
*/
static int FDelete(FILE * handle, int offset, int length)
{
	char *buf;
	int len;
	len = GetFileSize( handle ) - offset - length;
	if(len == 0)
	{
		TruncateFile( handle, offset);
	}
	else
	{
		buf = LocalAlloc(LMEM_FIXED, len );
		if (buf == NULL)
			return -1;
		SeekFile( handle, offset+length, SEEK_SET );
		len = ReadFile( handle, buf, len );
		if( len >= 0 )
		{
			SeekFile( handle, offset, SEEK_SET );
			WriteFile( handle, buf, len );
			if( offset + len == 0 )
			{
				SeekFile( handle, 0, SEEK_SET );
				WriteFile( handle, "\r\n", 2 );
				TruncateFile( handle, 2 );
			}
			else
				TruncateFile( handle, offset + len );
		}
		LocalFree( buf );
		
	}
	return len;
}

#define NOFILE -1
#define NOSECTION -2
#define NOKEY -3

static long __FS_GetPrivateProfileString( 
						char *lpAppName,  /* points to section name */
						char *lpKeyName,  /* points to key name */
						char *lpDefault,  /* points to default string */
						char *lpReturnedString,  /* points to destination buffer */
						long nSize,				/* size of destination buffer */
						char *lpFileName  /* points to initialization filename */
)
{
	long ret;
	FILE * handle;
	char buf[512];
	char Section[256]="[";
	char *ptr = NULL;

/*连接Section,使其成为 [string]的形式*/
	CatString( Section, lpAppName );
	CatString( Section, "]" );

	/*打开ProFile文件*/
	handle = fopen( lpFileName, "rb");
	if( handle == NULL )
	{
		//CloseFile( handle );
		/*返回缺省字符*/
		CopyString( lpReturnedString, lpDefault );
		return NOFILE;
	}
/* find section;*/
	while( (ret=ReadLine( handle, buf, sizeof(buf) )) > 0 )
	{
		ptr = buf;
		/*跳过空格及不可见字符*/
		while( *ptr && *ptr<=0x20 ) 
			ptr++;
		/*比较两个字符串的长度*/
		if( StringLength(ptr) < StringLength(Section) ) 
			continue;
		/*比较两个字符串是否相等*/
		if( CompareStringNoCase( ptr, Section ) == 0 )
			break;
	}
/* No this section;*/
	if( ret <= 0 )
	{
		CloseFile( handle );
		CopyString( lpReturnedString, lpDefault );
		return NOSECTION;
	}
/*find key*/
	while( (ret=ReadLine( handle, buf, sizeof(buf) )) >0 )
	{
		char *end;
		ptr=buf;
		while( *ptr && *ptr<=0x20 ) 
			ptr++;
		/*如果找到了下一个Section，证明没有要找的key*/
		if( *ptr == '[' )
		{
			ret = -1;
			break;
		}
		end = strstr( ptr, "=" );
		if (end)
		{
			*end = 0;
			if( CompareStringNoCase( ptr, lpKeyName ) == 0 )
			{	
				*end = '=';
				break;
			}
		}
	}
/* Not Found;*/
	if( ret <= 0 )
	{ 
		CloseFile( handle );
		CopyString( lpReturnedString, lpDefault );
		return NOKEY;
	}
/* Found;*/
	ptr = FindString( ptr, "=" );
	if(!ptr) 
	{
		CloseFile( handle );
		CopyString( lpReturnedString, lpDefault );
		return -1;
	}
	ptr ++;
	
	if( StringLength( ptr ) < nSize )
		CopyString( lpReturnedString, ptr );
	else 
		CopyStringN( lpReturnedString, ptr, nSize );
	
	CloseFile( handle );
	return 0;
}

long FS_GetPrivateProfileInt
				( char *lpAppName,  /* address of section name */
				char *lpKeyName,  /* address of key name */
 				unsigned long nDefault,   		/* return value if key name is not found */
 				char *lpFileName  /* address of initialization filename */
)
{
	char buf[64];

	if( __FS_GetPrivateProfileString(
									lpAppName, 
									lpKeyName, 
									"", 
									buf, 
									sizeof(buf), 
									lpFileName)  < 0 )
		return nDefault;
	else
		return StringToInt( buf );
}

long FS_GetPrivateProfileString( 
						char *lpAppName,  /* points to section name */
						char *lpKeyName,  /* points to key name */
						char *lpDefault,  /* points to default string */
						char *lpReturnedString,  /* points to destination buffer */
						unsigned long nSize,				/* size of destination buffer */
						char *lpFileName  /* points to initialization filename */
)
{
	if( lpAppName == NULL )
		return FS_GetAllSectionNames( lpReturnedString, 
																	nSize, 
																	lpFileName );
	if( lpKeyName == NULL )
		return FS_GetAllKeyNamesInSection(						
										lpAppName, 
										lpReturnedString,
										nSize,
										lpFileName );


	__FS_GetPrivateProfileString(
				lpAppName, lpKeyName, lpDefault,
				lpReturnedString, nSize, lpFileName );
	return StringLength( lpReturnedString );			
}

static long NoFileWriteString( 
						char *lpAppName,  /* points to section name */
						char *lpKeyName,  /* points to key name */
						char *lpString,		/* pointer to string to add */
						char *lpFileName  /* points to initialization filename */
)
{
	FILE * handle;
	char buf[512] = "[";

	CatString( buf, lpAppName );
	CatString( buf, "]\r\n" );
	CatString( buf, lpKeyName );
	CatString( buf, "=" );
	CatString( buf, lpString );
	CatString( buf, "\r\n" );
	
	handle = fopen( lpFileName, "wb");
	if( handle == NULL ) 
		return -1;
	if(	WriteFile( handle, buf, StringLength( buf ) ) == -1 )
	{
		CloseFile( handle );
		return -1;
	}
	CloseFile( handle );
	return 0;
}

static long NoSectionWriteString( 
						char *lpAppName,  /* points to section name */
						char *lpKeyName,  /* points to key name */
						char *lpString,		/* pointer to string to add */
						char *lpFileName  /* points to initialization filename */
)
{
	FILE * handle;
	char buf[512] = "[";

	CatString( buf, lpAppName );
	CatString( buf, "]\r\n" );
	CatString( buf, lpKeyName );
	CatString( buf, "=" );
	CatString( buf, lpString );
	CatString( buf, "\r\n" );

	handle = fopen( lpFileName, "rb+");
	if( handle == NULL ) return -1;
	SeekFile( handle, 0, SEEK_END );
	if(	WriteFile( handle, buf, StringLength( buf ) ) == -1 )
	{
		CloseFile( handle );
		return -1;
	}
	CloseFile( handle );
	return 0;
}

static long NoKeyWriteString( 
						char *lpAppName,  /* points to section name */
						char *lpKeyName,  /* points to key name */
						char *lpString,		/* pointer to string to add */
						char *lpFileName  /* points to initialization filename */
)
{
	char buf[512];
	char readbuf[512];
	FILE * handle;
	int offset = 0;
	int ret; 
	char Section[256]="[";
	char *ptr;

	CatString( Section, lpAppName );
	CatString( Section, "]" );

/*	CopyString( buf, lpKeyName );
	CatString( buf, "=" );
	CatString( buf, lpString );
	CatString( buf, "\r\n" );
*/
//	CopyString( buf, "\r\n" );
	CopyString( buf, lpKeyName );
	CatString( buf, "=" );
	CatString( buf, lpString );
	CatString( buf, "\r\n" );

	handle = fopen( lpFileName, "rb+");
	if( handle == NULL )
		return -1;
	memset( readbuf, 0, sizeof(readbuf) );
	while( (ret=ReadLine( handle, readbuf, sizeof(readbuf) )) > 0 )
	{
		offset += ret;
		ptr = readbuf;
		while( *ptr && *ptr<=0x20 ) 
			ptr++;
		if( StringLength(ptr) < StringLength(Section) ) 
			continue;
		if( CompareStringNoCase( ptr, Section ) == 0 )
			break;
		memset( readbuf, 0, sizeof(readbuf) );
	}
	if( ret == -1 ) 
	{
		CloseFile( handle );
		return -1;
	}
	
	ret = FInsert( handle, offset, buf, StringLength(buf) );
	CloseFile( handle );
	return ret;
}

static long ChangeProfileString( 
						char *lpAppName,  /* points to section name */
						char *lpKeyName,  /* points to key name */
						char *lpString,		/* pointer to string to add */
						char *lpFileName  /* points to initialization filename */
)
{
	char buf[512];
	FILE * handle;
	int offset = 0;
	int ret;
	char Section[256]="[";
	char *ptr;
	//liuyuan添加
	int strl = 0;
	int Found = 0;
	int sectionbegin;

    CatString( Section, lpAppName );
	CatString( Section, "]" );

	handle = fopen( lpFileName, "rb+");

	if( handle == NULL ) 
		return -1;
	
	while( (ret=ReadLine( handle, buf, sizeof(buf) )) > 0 )
	{
		offset += ret;
		ptr = buf;
		while( *ptr && *ptr<=0x20 ) 
			ptr++;
		if( StringLength(ptr) < StringLength(Section) ) 
			continue;
		if( CompareStringNoCase( ptr, Section ) == 0 )
			break;
	}
	if( ret == -1 ) 
	{
		CloseFile( handle );
		return -1;
	}
	sectionbegin = offset - ret;
	while( (ret=ReadLine( handle, buf, sizeof(buf) )) >0 )
	{
		char *end;
		offset += ret;
		ptr=buf;
		while( *ptr && *ptr<=0x20 )
			ptr++;

		if( *ptr == '[' )
			break;
		if( lpKeyName )
		{
			end = strstr( ptr, "=" );
			if( end )
			{
				*end = 0;
				if( CompareStringNoCase( ptr, lpKeyName ) == 0 )
				{
					//liuyuan添加
					strl = StringLength(ptr) ; 
					//liuyuan添加
					*end = '=';
					Found = 1;
					break;
				}
			}
		}
	}

	if( lpKeyName == NULL )
	{
		if( ret < 0 )
			ret = 0;
		ret = FDelete( handle, sectionbegin, offset-sectionbegin-ret );
		CloseFile( handle );
		return ret;
	}
	if( Found == 0 )
	{
		CloseFile( handle );
		return -1;
	}


//	offset = offset - ret + 1;
	offset = offset - ret;
	SeekFile( handle, offset, SEEK_SET );

// Clear key value Kent 19991123
	if( FDelete( handle, offset, ret ) == -1 )
	{
		CloseFile( handle );
		return -1;
	}
	if( !lpString )
	{
		CloseFile (handle);
		return 0;
	}
// Clear key value Kent 19991123
	CopyString( buf, lpKeyName );
	CatString( buf, "=" );
	CatString( buf, lpString );
	CatString( buf, "\r\n" );

	FInsert ( handle, offset, buf, strlen(buf));
//	CatString( buf, "\r\n" );


//	if( ret > StringLength(buf) )
#ifdef OLD_CODE	
	if( strl > StringLength(buf) )
	{
		WriteFile( handle, buf, StringLength(buf) );
		
		//for( ;ret > StringLength(buf); ret -- )
		for( ;strl > StringLength(buf); strl -- )
			WriteFile( handle, " ", 1 );
	}
	else if ( strl < StringLength(buf) )
	{
		//WriteFile( handle, buf, ret );
		//FInsert( handle, offset+ret, buf+ret, StringLength(buf)-ret );
		WriteFile( handle, buf, strl );
		FInsert( handle, offset+strl, buf+strl, StringLength(buf)-strl );
	}
	else /*如果长度相等*/
		WriteFile( handle, buf, strl );
#endif	
	CloseFile( handle );
	return 0;
}

long FS_WritePrivateProfileString( 
						char *lpAppName,  /* points to section name */
						char *lpKeyName,  /* points to key name */
						char *lpString,	  /* pointer to string to add */
						char *lpFileName  /* points to initialization filename */
)
{
	int ret;
	char buf[512];
	char Section[128];
//	char Key[128];

	CopyString( Section, "[" );
	CatString( Section, lpAppName );
	CatString( Section, "]" );
//	if( lpKeyName )
// 		CopyString( Key, lpKeyName );

	if( lpAppName && lpKeyName && lpString )
		ret =  __FS_GetPrivateProfileString(
									lpAppName, 
									lpKeyName, 
									"", 
									buf, 
									sizeof(buf), 
									lpFileName);
	else
		ret = 0;

	switch ( ret )
	{
		case NOFILE:
			return NoFileWriteString( lpAppName, lpKeyName, lpString, lpFileName );
		case NOSECTION:
			return NoSectionWriteString( lpAppName, lpKeyName, lpString, lpFileName );
		case NOKEY:
			return NoKeyWriteString( lpAppName, lpKeyName, lpString, lpFileName );
		default:
			return ChangeProfileString( lpAppName, lpKeyName, lpString, lpFileName );
	}
}

long FS_WritePrivateProfileInt( 
						char *lpAppName,  /* points to section name */
						char *lpKeyName,  /* points to key name */
						long	 nValue,			/* pointer to string to add */
						char *lpFileName  /* points to initialization filename */
)
{
	char buf[64];
	IntToString( nValue, buf );
	
	return FS_WritePrivateProfileString
						( lpAppName, 
							lpKeyName, 
							buf, 
							lpFileName );
	
}

long FS_GetAllSectionNames( char *lpReturn, 
												 unsigned long nSize, 
												 char *lpFileName )
{
	FILE * hFile;
	char buf[512];
	int ret, retVal = 0;
	char *beg, *end;

	ret = 0;
	hFile = fopen( lpFileName, "rb");
	if( hFile == NULL )
	{
		*lpReturn = 0;
		return ret;
	}
	while( (ret=ReadLine( hFile, buf, sizeof(buf) )) >0 )
	{
		if( buf[0] == '[' )
		{
			beg = buf;
			while( *beg <= 0x20 && *beg )
				beg ++;
			end = strstr( beg, "]" );
			if( end )
			{
				*end = 0;
				if( nSize-retVal > (unsigned long)(end-beg + 1) )
				{
					memcpy( lpReturn+retVal, beg+1, end-beg );
					retVal += end-beg;
				}
				else
					break;
			}
		}
	}
	lpReturn[retVal] = 0;
	CloseFile( hFile );
	return retVal;
}

long FS_GetAllKeyNamesInSection(						
						char *lpAppName,  /* points to section name */
						char *lpString,		/* pointer to string to add */
						int  nSize,
						char *lpFileName  /* points to initialization filename */
)
{
	char readbuf[512];
	FILE * handle;
	int ret; 
	char Section[256]="[";
	char *ptr, *end;
	int retVal = 0;

	CatString( Section, lpAppName );
	CatString( Section, "]" );

	handle = fopen( lpFileName, "rb+");
	if( handle == NULL )
		return -1;
	memset( readbuf, 0, sizeof(readbuf) );
	while( (ret=ReadLine( handle, readbuf, sizeof(readbuf) )) > 0 )
	{
		ptr = readbuf;
		while( *ptr && *ptr<=0x20 ) 
			ptr++;
		if( StringLength(ptr) < StringLength(Section) ) 
			continue;
		if( CompareStringNoCase( ptr, Section ) == 0 )
			break;
		memset( readbuf, 0, sizeof(readbuf) );
	}
	if( ret == -1 ) 
	{
		CloseFile( handle );
		return -1;
	}
	while( (ret=ReadLine( handle, readbuf, sizeof(readbuf) )) > 0 )
	{
		ptr = readbuf;
		while( *ptr && *ptr<=0x20 ) 
			ptr++;
		if( *ptr == '[' )
			break;
		end = strstr( ptr, "=" );
		if( end )
		{
			*end = 0;
			if( nSize-retVal > end-ptr + 1 )
			{
				memcpy( lpString+retVal, ptr, end-ptr+1 );
				retVal += end-ptr+1;
			}
			else
				break;
		}
	}
	lpString[retVal] = 0;
	CloseFile( handle );
	return retVal;
}

long FS_ClearKey(void)
{
	return 0;
}
/* END OF PROGRAM FILE */

cntt_uint32 cntt_GetProfileString(CNTT_PCSTR secName, CNTT_PCSTR keyName, CNTT_PCSTR defaultString, CNTT_PSTR keyString, cntt_uint32 bufferSize, CNTT_PCSTR fileName)
{
    return  FS_GetPrivateProfileString((char*)secName, (char*)keyName, (char*)defaultString, keyString, bufferSize, (char*)fileName);           
}
CNTT_BOOL cntt_WriteProfileString(CNTT_PCSTR secName, CNTT_PCSTR keyName, CNTT_PCSTR keyString, CNTT_PCSTR fileName)
{
    return  FS_WritePrivateProfileString((char*)secName, (char*)keyName, (char*)keyString, (char*)fileName);                  
}
cntt_int32 cntt_GetProfileInt(CNTT_PCSTR secName, CNTT_PCSTR keyName, cntt_int32  defKeyValue, CNTT_PCSTR fileName)
{    
    return FS_GetPrivateProfileInt((char*)secName, (char*)keyName, defKeyValue, (char*)fileName);    
}
CNTT_BOOL cntt_WriteProfileInt(CNTT_PCSTR secName, CNTT_PCSTR keyName, cntt_int32 keyValue, CNTT_PCSTR fileName)
{
    return FS_WritePrivateProfileInt((char*)secName, (char*)keyName, keyValue, (char*)fileName);
}


