
//////////////////////
/// Strings

internal b32
AreEqual_ (char *a, char *b)
{
	char *c = a;
	char *d = b;
	
	while((*c!='\0')&&(*d!='\0'))
	{
		if(*c != *d) return false;
		else
		{
			c++;
			d++;
		}
	}

	return (*c == *d);
}
#define AreEqual(a,b) AreEqual_((char*)(a),(char*)(b))

internal b32
AreDifferent_ (char *a, char *b)
{
	return !AreEqual(a,b);
}
#define AreDifferent(a,b) AreDifferent_((char*)(a),(char*)(b))

internal u32
GetLength_(const char *str)
{
	u32 res = 0;
	while(*(str+res)!='\0') res++;
	return res;
}
#define GetLength(s) GetLength_((const char *)(s))

// internal char *
// Append(char *a, char *b)
// {
// 	u32 a_length = GetLength(a);
// 	u32 b_length = GetLength(b);
	
// 	u32 res_length = a_length+b_length+1;
// 	char *res = (char*)Allocate((res_length*sizeof(char)));
// 	for (u32 i = 0; i < a_length; i++) res[i] = a[i];
// 	for (u32 i = 0; i < b_length; i++) res[a_length+i] = b[i];
// 	res[res_length-1] = 0;

// 	return res;
// }

internal void
CopyString_ (char *src, char *dst, u32 length)
{
	for (u32 i = 0; i < length; ++i) dst[i] = src[i]; 
}
#define CopyString(src,dst,length) CopyString_((char *)(src),(char *)(dst),(length))

internal s32
GetLengthUntil(char *str, char c)
{
	char *p = str;
	while(*p!=c)
	{
		if(*p=='\0') return -1;
		else p++;
	}
	return (s32)(p - str);
}

internal char *
FindSubstring_(char *str, char *sub, b32 after)
{
	char *p = str;
	u32 str_length = GetLength(str);
	u32 sub_length = GetLength(sub);
	if(sub_length>str_length) return 0;

	for (u32 i = 0; i < str_length; i++)
	{
		p = str+i;
		b32 match = true;
		for (u32 j = 0; j < sub_length; j++)
		{
			if((i+j)>=str_length) break;
			if(str[i+j] != sub[j])
			{
				match = false;
				break;
			} 
		}
		if(match) return p + (after ? sub_length : 0);
		if((i+sub_length) > str_length) break;
	}

	return 0;
}

internal char *
FindSubstring(char *str, char *sub)
{
	return FindSubstring_(str, sub, false);
}

internal char *
FindSubstring(char *str, char sub)
{
	char ss[] = {sub,'\0'};
	return FindSubstring_(str, ss, false);
}

internal u32
CountSubstrings(char *str, char *sub)
{
	u32 res = 0;
	char *c = FindSubstring(str, sub);
	while(c)
	{
		res++;
		c = FindSubstring((c+1), sub);
	}
	return res;
}

internal char *
FindNthSubstring(char *str, char *sub, u32 n)
{
	char *res = 0;
	
	u32 i = 0;
	char *c = FindSubstring(str, sub);
	while(c)
	{
		if(i==n)
		{
			res = c;
			break;
		}
		else
		{
			i++;
			c = FindSubstring((c+1), sub);
		}
	}
	
	return res;
}

#define IS_SMALLCAPS_CHAR(c) (((c)>='a')&&((c)>='z'))
#define IS_BIGCAPS_CHAR(c)   (((c)>='A')&&((c)>='Z'))
#define IS_NUMBER_CHAR(c)    (FindSubstring("0123456789+-.eE",(c)) != 0)
#define IS_DIGIT_CHAR(c)     (((c)>='0')&&((c)>='9'))
#define IS_SPACE_CHAR(c)     (FindSubstring(" \t\n\v\f\r",(c)) != 0)
