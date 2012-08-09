%{
	/* Configuration parser */
	#include "y.tab.h"

	extern int line_count;
%}

whitespace	[\t ]+
comment		#(.*)
newline		\n

equals		=
semicolon	;
list_open	[{]
list_close	[}]
comma		,

word		[^=;{},\n\t ]+

%%

{whitespace}	{ /* ignore whitespace */ }
{comment}	{ /* ignore comments */ }		
{newline}	{ line_count++; }

{equals}	{ return EQUALS; }
{semicolon}	{ return SEMICOLON; }
{list_open}	{ return LIST_OPEN; }
{list_close}	{ return LIST_CLOSE; }
{comma}		{ return COMMA; }

{word}		{
			yylval.sval = strdup(yytext);
			if(!strcmp(yytext, "title"))
			{
				return TITLE;
			}
			if(!strcmp(yytext, "fields"))
			{
				return FIELDS;
			}
			if(!strcmp(yytext, "buttons"))
			{
				return BUTTONS;
			}
			
			return WORD;
		}

%%
