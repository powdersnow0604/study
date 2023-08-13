#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char str[] = "{120,8,200,6},{12,93,96,16},{74,271,16,110},{126,336,96,290},\
{142, 337, 181, 335}, { 192,335,232,280 }, { 244,259,288,108 }, { 285,92,212,13 },\
{ 96,135,136,118 }, { 194,119,223,125 }, { 105,145,124,134 }, { 110,146,138,151 },\
{ 131,133,139,146 }, { 188,146,198,134 }, { 189,153,218,146 }, { 204,133,221,140 },\
{ 91,268,122,202 }, { 149,206,159,209 }, { 170,209,181,204 }, { 235,265,208,199 },\
{ 121,280,205,284 }, { 112,286,160,301 }, { 166,301,214,287 }";

char* openP;
char* closeP;
char* secondC;

char* newStr;
int pos = 0;

int main_change(void)
{
	openP = str;
	newStr = (char*)malloc(1);

	while ((openP = strchr(openP, '{')) != NULL) {
		closeP = strchr(openP, '}');
		secondC = strchr(strchr(openP, ',') + 1, ',');
		
		newStr = (char*)realloc(newStr, _msize(newStr) + (closeP-openP+1+3) * sizeof(char));

		newStr[pos++] = '(';
		for (char* i = openP+1; i < secondC; i++) {
			newStr[pos++] = *i;
		}
		newStr[pos++] = ')';
		newStr[pos++] = ',';

		newStr[pos++] = '(';
		for (char* i = secondC + 1; i < closeP; i++) {
			newStr[pos++] = *i;
		}
		newStr[pos++] = ')';
		newStr[pos++] = ',';

		openP = closeP + 1;
	}
	newStr[pos++] = '\0';

	printf("%s", newStr);

	free(newStr);
	return 0;
}