#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
struct data
{
	char *fname;
	char* lname;
	int age;
	char *height;
	struct phoneno *nos;
	struct data *link;
};
struct phoneno
{
	char *type;
	char *no;
	struct phoneno *link;
};
int lengthOfString(char *name)
{
	int len = 0, i = 0;
	while (name[i] != '\0')
	{
		++i;
		++len;
	}
	return len;
}
int string_compare(char *str1, char *str2,int len)
{
	int len1;
	len1 = lengthOfString(str1);
	if (len1 == len)
	{
		while (len1 > 0)
		{
			if (str1[len1 - 1] != str2[len1 - 1] && (str1[len1 - 1] != str2[len1 - 1]-32)&& (str1[len1 - 1]-32 != str2[len1 - 1]))
			{
				return 0;
			}
			len1--;
		}
		return 1;
	}
	return 0;
}
void strcopy(char *src, char *dest, int end, int start)
{
	int i = 0;
	for (i = start; i < end; i++)
		dest[i - start] = src[i];
	dest[i - start] = '\0';
}
struct data* insertIntoData(struct data *list,struct data* node,struct phoneno *phlist)
{
	struct data *temp1;
	node->nos = phlist;
	node->link = NULL;
	if (list == NULL)
	{
		list = node;
	}
	else
	{
		temp1 = list;
		while (temp1->link != NULL)
			temp1 = temp1->link;
		temp1->link = node;
	}
	return list;
}
struct phoneno* enterPhoneno(char *line,char *line1, struct phoneno *node,int *flag)
{
	int len,comma=0,index = 0, typestart= 0,typeend=0 ,valueflag = 0, valuestart = 0, valueend = 0;
	len = lengthOfString(line);
	struct phoneno *temp = (struct phoneno*)malloc(sizeof(struct phoneno)),*temp1=NULL;
	while (index < len)
	{
		if (line[index] == '\"'&&valueflag == 1)
		{
			typestart = index+1;
			valueflag++;
		}
		else if (line[index] == ':')
		{
			valueflag++;
		}
		else if (line[index] == '\"'&&valueflag == 2)
		{
			typeend = index;
			valueflag++;
		}
		index++;
	}
	index = 0; valueflag = 0; len = lengthOfString(line1);
	while(index<len)
	{
		if (line1[index] == '\"'&&valueflag == 1)
		{
			valuestart = index+1;
			valueflag++;
		}
		else if (line1[index] == ':')
		{
			valueflag++;
		}
		else if (line1[index] == '\"'&&valueflag == 2)
		{
			valueend = index; valueflag++;
		}
		else if (line1[index] == ',')
			comma = 1;
		index++;
	}
	temp->type = (char*)malloc((typeend + 1 - typestart)*sizeof(char));
	strcopy(line,temp->type, typeend ,typestart);
	temp->no = (char*)malloc((valueend + 1 - valuestart)*sizeof(char));
	strcopy(line1,temp->no, valueend ,valuestart);
	temp->link = NULL;
	if (comma == 0)
		*flag = 0;
	if (node == NULL)
	{
		node = temp;
	}
	else
	{
		temp1 = node;
		while (temp1 ->link!= NULL)
			temp1 = temp1->link;
		temp1->link = temp;
	}
	return node;
}
int convert_into_int(char *line,int end,int start)
{
	int value = 0;
	while (start < end)
	{
		value = (value * 10) + line[start] - '0';
		start++;
	}
	return value;
}
struct data* enterData(char *line, struct data *node, int *flag)
{
	int len,index=0,key=0,valueflag=0,valuestart=0,valueend=0;
	len = lengthOfString(line);
	while (index < len)
		{
			if (line[index] == '\"'&&valueflag==0)
			{
				key = index;
			}
			else if (line[index] == ':')
			{
				valueflag++;
			}
			else if ((line[index] == '\"'&&valueflag == 1))
			{
				valuestart = index + 1;
				valueflag++;
			}
			else if((line[index]-'0'>=0) &&valueflag == 1&& (line[index] - '0' <= 9))
			{
				valuestart = index;
				valueflag++;
			}
			else if ((line[index] == '\"'&&valueflag == 2 )|| (line[index] == ','&&valueflag == 2))
			{
				valueend = index; 
				break;
			}
			index++;
		}
		if (string_compare("firstName", line, key) == 1)
		{
			node->fname = (char*)malloc((valueend + 1 - valuestart)*sizeof(char));
			strcopy(line, node->fname, valueend, valuestart);
		}
		else if (string_compare("lastName", line, key) == 1)
		{
			node->lname = (char*)malloc((valueend + 1 - valuestart)*sizeof(char));
			strcopy(line, node->lname, valueend, valuestart);
		}
		else if (string_compare("age", line, key) == 1)
		{
			node->age = (char*)malloc((valueend + 1 - valuestart)*sizeof(char));
			node->age=convert_into_int(line, valueend, valuestart);
		}
		else if (string_compare("height", line, key ) == 1)
		{
			node->height = (char*)malloc((valueend + 1 - valuestart)*sizeof(char));
			strcopy(line, node->height, valueend, valuestart);
		}
		else if (string_compare("phoneNumbers", line, key) == 1)
			*flag = 1;
	return node;
}
void retreiveData(struct data *memlist,char *fname,char *lname)
{
	struct data *temp = memlist;
	struct phoneno *tempp = NULL;
	int len,len2;
	while (temp!=NULL)
	{
		len = lengthOfString(temp->fname);
		len2= lengthOfString(temp->lname);
		if(string_compare(fname,temp->fname,len)==1&&string_compare(lname,temp->lname,len2)==1)
		{
			tempp = temp->nos;
			printf("Phone Numbers are : \n");
			while (tempp!= NULL)
			{
				printf("%s - %s\n",tempp->type,tempp->no);
				tempp = tempp->link;
			}
		}
		temp = temp->link;
	}
}
struct data* readFromFile(struct data *list)
{
	FILE *fp;
	list = NULL;
	char line[50],ch,line1[50];
	int phflag = 0,temp=0,index=0;
	fp = fopen("jsontext.txt","r");
	struct data *node = NULL;
	struct phoneno *phno=NULL;
	if (fp != NULL)
	{
		while (!feof(fp))
		{
			phflag = 0; ch = fgetc(fp); temp = 0;
			node = (struct data*)malloc(sizeof(struct data));
			node->age = 0;
			node->height = "";
			node->fname = "";
			node->lname="";
			phno = (struct phoneno*)malloc(sizeof(struct phoneno));
			phno = NULL;
			while ((ch) !='}'&&!feof(fp))//for one struct 
			{
				if (ch == '\"')
				{
					temp = 1;
					index = 0;
					while (ch != 10)
					{
						ch = fgetc(fp);line[index++] = ch;
					}
					line[index]= '\0';
					if (phflag != 1)
						node = enterData(line, node, &phflag);
					while(phflag)
					{
						temp = 1; index = 0;
						ch = fgetc(fp);
						while (ch != 10&&!feof(fp))
						{
							line[index++] = ch;
							ch = fgetc(fp);
						}line[index] = '\0';
						index=0;
						ch = fgetc(fp);
						while (ch != 10&&!feof(fp))
						{
							line1[index++] = ch;
							ch = fgetc(fp);
						}line1[index] = '\0';
						phno = enterPhoneno(line,line1,phno,&phflag);
					}
				}
				ch = fgetc(fp);
			}
			if(temp==1)
			list=insertIntoData(list,node,phno);
		}
	}
	fclose(fp);
	return list;
}
void print_average_age(struct data *list)
{
	struct data *temp = list;
	int avg = 0,count=0;
	while (temp != NULL)
	{
		avg = avg+temp->age ;
		count++;
		temp = temp->link;
	}
	printf("Average age of given records are %f\n",(float)avg/count);
}
void main()
{
	struct data *members = NULL, *temp;
	struct phoneno *tempp;
	char fname[50],lname[50];
	members=readFromFile(members);
	temp = members;
	while (temp!=NULL)
	{
		printf("%s\n",temp->fname);
		printf("%s\n", temp->lname);
		printf("%d\n", temp->age);
		printf("%s\n", temp->height);
		tempp = temp->nos;
		printf("Phone Numbers\n");
		while (tempp!= NULL)
		{
			printf("%s - %s\n",tempp->type,tempp->no);
			tempp = tempp->link;
		}
		temp = temp->link;
	}
	printf("enter first name");
	scanf("%s",fname);
	printf("enter last name");
	scanf("%s",lname);
	retreiveData(members,fname,lname);
	print_average_age(members);
	_getch();
}