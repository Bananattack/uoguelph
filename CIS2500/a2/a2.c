/*
	Assignment #2 - String Processing

	Andrew Crowell
	0545826
	
*/

/* External dependencies */
#include <stdio.h>

/*
	Statistics related to a particular document
	that has been read and analyzed.
*/
typedef struct
{
	int sentences;
	int syllables;
	int words;
} DocumentProperties;

/*
	A list of possible character scan modes
	during document analysis.
*/
typedef enum
{
	NONE,
	VOWEL,
	CONSONANT,
	WHITESPACE,
	SENTENCE
} ScanMode;

/*
	A function that returns the greatest of two integer inputs.
	(Because there isn't one in math.h.)

	Pass: two numbers a and b
	Return: greatest of a and b
*/
int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	return b;
}

/*
	A function that rounds a floating point value to the nearest integer.
	(Because there isn't one in math.h.)

	Pass: a
	Return: closest int to a
*/
int round (double a)
{
	if (a < 0)
	{
		return (int) (a - 0.5);
	}
	return (int) (a + 0.5);
}

/*
	Determines if a given character is a vowel.
	Pass: a character
	Return:	a boolean evaluation of whether or not the
		supplied character is considered a vowel.
*/
int IsVowel(char c)
{
	if (c == 'A' || c == 'a'
		|| c == 'E' || c == 'e'
		|| c == 'I' || c == 'i'
		|| c == 'O' || c == 'o'
		|| c == 'U' || c == 'u'
		|| c == 'Y' || c == 'y')
	{
		return 1;
	}
	return 0;
}

/*
	Determines if a given character is whitespace.
	Pass: a character
	Return:	a boolean evaluation of whether or not the
		supplied character is considered whitespace.
*/
int IsWhiteSpace(char c)
{
	if (c == '\n'
		|| c == '\t'
		|| c == '\r'
		|| c == ' ')
	{
		return 1;
	}
	return 0;
}

/*
	Determines if a given character is punctuation.
	Pass: a character
	Return:	a boolean evaluation of whether or not the
		supplied character is considered punctuation.
*/
int IsPunctuation(char c)
{
	if (c == '.'
		|| c == ':'
		|| c == ';'
		|| c == '?'
		|| c == '!')
	{
		return 1;
	}
	return 0;
}

/*
	Analyzes a document and returns various information about it.
	Pass: a file stream to read from.
	Return:	a boolean evaluation of whether or not the
		supplied character is considered punctuation.
*/
DocumentProperties AnalyzeDocument(FILE* f)
{
	char lastCharacter, currentCharacter = 0;
	int syllables;

	int i;

	ScanMode mode = NONE;
	DocumentProperties data;

	data.sentences = 0;
	data.syllables = 0;
	data.words = 0;

	while (!feof(f))
	{
		lastCharacter = currentCharacter;
		currentCharacter = fgetc(f);

		/* Met the end of a word, update syllable count. */
		if ((IsWhiteSpace(currentCharacter)
				|| IsPunctuation(currentCharacter))
				&& mode != WHITESPACE
				&& mode != SENTENCE)
		{
			/* If the last character was a letter... */
			if (mode == CONSONANT || mode == VOWEL)
			{
				/* The letter E at the end of word is not counted. */
				if (lastCharacter == 'e' || lastCharacter == 'E')
				{
					syllables--;
				}
				
				/* Ensure the word is *at least* one syllable */
				syllables = max (1, syllables);
				data.syllables += syllables;

				i = 0;
			}
			syllables = 0;
		}

		/* A vowel. */
		if (IsVowel(currentCharacter))
		{
			/* Non-consecutive vowels increase the syllable count. */
			if (mode != VOWEL)
			{
				mode = VOWEL;
				syllables++;
				i++;
			}
		}
		/* Some whitespace. */
		else if (IsWhiteSpace(currentCharacter))
		{
			/*
				Whitespace denotes the end of a word
				when the last character is a letter.
			*/
			if (mode == CONSONANT || mode == VOWEL)
			{
				data.words++;
			}
			mode = WHITESPACE;
		}
		/* Some punctuation. */
		else if (IsPunctuation(currentCharacter))
		{
			if (mode != SENTENCE)
			{
				if (mode == CONSONANT || mode == VOWEL)
				{
					data.words++;
				}
				data.sentences++;
			}
			mode = SENTENCE;
		}
		/* Eh, we don't care what the hell it is. Assume it's a consonant. */
		else
		{
			mode = CONSONANT;
			i++;
		}
	}

	/* Return our document data */
	return data;
}

/*
	Calculates a legibility index value from specific document information
	Pass: document properties
	Return:	The legibility index.
*/
double CalculateIndex(DocumentProperties data)
{
	return 206.835 - 84.6 * ((double) data.syllables / (double) data.words)
			- 1.015 * ((double) data.words / (double) data.sentences);
}

/*
	The embodiment of all awesomeness, created with epic win.
	Returns: 0 upon success, something else that's bad on failure.
*/
int main()
{
	DocumentProperties data;
	data = AnalyzeDocument(stdin);
	printf("Legibility Index  = %d \n", round(CalculateIndex(data)));
	printf("Syllable Count    = %d\n", data.syllables);
	printf("Word count        = %d\n", data.words);
	printf("Sentence count    = %d\n", data.sentences);
	return 0;
}






