#include <iostream>
#include <Windows.h>
#include <vector>
#include <sstream>
#include <stdlib.h> 
#include <ctime>
struct Snake
{
	float x = 0;
	float y = 0;
};

bool move(std::vector<Snake> *ssnake, wchar_t *screen, int *lastmove, float speed, double flaps);
bool render(std::vector<Snake> ssnake, wchar_t* screen, int ancho);
bool movercuerpo(std::vector<Snake>* ssnake, int size, int ancho, int alto);
bool spawncomida(wchar_t* screen, int ancho, int alto, int *x, int *y);
bool crecer(wchar_t* screen, int ancho, std::vector<Snake>* ssnake, int *score);
bool menu(wchar_t* screen, int ancho, int alto, HANDLE hConsole, DWORD charescritos);
bool menu1(wchar_t* screen, int ancho, int alto, HANDLE hConsole, DWORD charescritos);
int main() {
	
	double flaps = 0;
	int ancho = 120;
	int alto = 40;
	srand(time(NULL));
	int foodx = rand() % ancho;
	int foody = rand() % alto;
	float speed =70;
	int t1;
	int t2;
	bool vivo = 1;
	HANDLE wd = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(wd);
	wchar_t* screen = new wchar_t[ancho * alto];
	
	//primer spwan de la comida
	
	DWORD letrasescritas = 0;
	vivo = menu(screen, ancho, alto, wd, letrasescritas);
	//output de la funcion move el ultimomovimiento 
	int* lastmove = new int(0);
	bool game = vivo;
	std::vector<Snake> ssnake;
	Snake nueva;
	while (game)
	{
		ssnake.push_back(nueva);
		ssnake.push_back(nueva);
		ssnake.push_back(nueva);
		*lastmove = 0;
		int score = 0;
		while (vivo)
		{
			t1 = clock();
			for (int i = 0; i < ancho * alto; i++)
			{
				screen[i] = ' ';
			}

			std::stringstream ss;
			ss << "Score: " << score;
			for (int i = 0; i < ss.str().length(); i++) {
				screen[i + 4] = ss.str()[i];
			}
			screen[foody * ancho + foodx] = '*';
			move(&ssnake, screen, lastmove, speed, flaps);
			

			//ss << foody;

			vivo = movercuerpo(&ssnake, ssnake.size(), ancho, alto);
			crecer(screen, ancho, &ssnake, &score);
			render(ssnake, screen, ancho);
			spawncomida(screen, ancho, alto, &foodx, &foody);
			//screen[static_cast<int>(ssnake[1].y * ancho + ssnake[1].x)] = '#';
			WriteConsoleOutputCharacter(wd, screen, ancho * alto, { 0,0 }, &letrasescritas);
			t2 = clock();
			flaps = (double(t2 - t1) / CLOCKS_PER_SEC);
			if (!vivo) {
				ssnake.clear();
				vivo = false;
				game = menu1(screen, ancho, alto, wd, letrasescritas);
			}
			
		}
		vivo = game;
	}

	return 0;
}
//input y sistema de movimiento
bool move(std::vector<Snake> *ssnake, wchar_t *screen, int *lastmove, float speed, double flaps) {

	if ((GetAsyncKeyState(unsigned('W')) || *lastmove == 3) && *lastmove != 2) {
		//ssnake->operator[](1).x += 0.1 * speed;
		*lastmove = 3;
		ssnake->operator[](1).y += -0.1 * speed * flaps;
	}
	if ((GetAsyncKeyState(unsigned('S')) || *lastmove == 2) && *lastmove != 3) {
		//ssnake[1].x += 0.1 * speed;
		*lastmove = 2;
		ssnake->operator[](1).y += 0.1 * speed * flaps;
	}if ((GetAsyncKeyState(unsigned('A')) || *lastmove == 1) && *lastmove != 0) {
		*lastmove = 1;
		ssnake->operator[](1).x += -0.1 * speed * flaps;
		//ssnake[0].y += 0.1 * speed;
	}if ((GetAsyncKeyState(unsigned('D')) || *lastmove == 0) && *lastmove != 1) {
		*lastmove = 0;
		ssnake->operator[](1).x += 0.1 * speed * flaps;
		//ssnake[0].y += 0.1 * speed;
	}
	
	return true;
}
bool render(std::vector<Snake> ssnake, wchar_t* screen, int ancho) {
	for (int i = 1; i < ssnake.size(); i++) {
		screen[static_cast<int>(ssnake[i].y) * ancho + static_cast<int>(ssnake[i].x)] = '#';
	}
	return true;
}
bool movercuerpo(std::vector<Snake>* ssnake, int size , int ancho, int alto) {
	
	if (static_cast<int>(ssnake->operator[](1).x) != static_cast<int>(ssnake->operator[](2).x) || static_cast<int>(ssnake->operator[](1).y) != static_cast<int>(ssnake->operator[](2).y))
	{
		for (int i = size-1 ; i >1; i--) {
			
			
			ssnake->operator[](i).x = ssnake->operator[](i - 1).x;
			ssnake->operator[](i).y = ssnake->operator[](i - 1).y;

		}
		
	}
	//retorna true si la snake no se a comido a si misma
	//false si murio
	bool retornar = true;
	for (int i = 3; i < size; i++) {
		if (static_cast<int>(ssnake->operator[](1).x) == static_cast<int>(ssnake->operator[](i).x)  && static_cast<int>(ssnake->operator[](1).y) == static_cast<int>(ssnake->operator[](i).y)) {
			
			retornar = false;
		}
	}
	if (ssnake->operator[](1).x > ancho || ssnake->operator[](1).y > alto || ssnake->operator[](1).x <0 || ssnake->operator[](1).y <0 ) {
		retornar = false;
	}
	return retornar;
}

bool spawncomida(wchar_t* screen,int ancho, int alto, int *x, int *y)
{
	
	bool spawn = true;
	while (spawn)
	{
		
		if (screen[*y * ancho + *x] != '*')
		{
			srand(time(NULL));
			*x = rand() % ancho;
			*y = rand() % alto;
			screen[*y * ancho + *x] = '*';
		
			spawn = false;
		}if (screen[*y * ancho + *x] == '*') {
			spawn = false;
		}
		
	}
	
	return true;
}
bool crecer(wchar_t* screen, int ancho, std::vector<Snake>* ssnake, int *score) {
	
	Snake nuevo;

	if (screen[static_cast<int>(ssnake->operator[](1).y) * ancho + static_cast<int>(ssnake->operator[](1).x)] == '*') {
		ssnake->push_back(nuevo);
		*score = *score + 10;
	}

	return true;
}

bool menu(wchar_t* screen, int ancho, int alto, HANDLE hConsole, DWORD charescritos)
{
	bool menu = true;
	while (menu)
	{
		for (int i = 0; i < alto * ancho; i++) {

			screen[i] = ' ';

		}
		std::wstring	text = L"░██████╗███╗░░██╗░█████╗░██╗░░██╗███████╗";
		text += L"██╔════╝████╗░██║██╔══██╗██║░██╔╝██╔════╝";
		text += L"╚█████╗░██╔██╗██║███████║█████═╝░█████╗░░";
		text += L"░╚═══██╗██║╚████║██╔══██║██╔═██╗░██╔══╝░░";
		text += L"██████╔╝██║░╚███║██║░░██║██║░╚██╗███████╗";
		text += L"╚═════╝░╚═╝░░╚══╝╚═╝░░╚═╝╚═╝░░╚═╝╚══════╝";




		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < text.length() / 6; j++) {
				screen[(i + 10) * ancho + (j + 23)] = text[i * text.length() / 6 + j];
			}
		}

		std::wstring	text2 = L"░░█ █░█ █▀▀ ▄▀█ █▀█";
		text2 += L"█▄█ █▄█ █▄█ █▀█ █▀▄";


		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < text2.length() / 2; j++) {
				screen[(i + 20) * ancho + (j + 23)] = text2[i * text2.length() / 2 + j];
			}
		}
		text2 = L"█▀ ▄▀█ █░░ █ █▀█";
		text2 += L"▄█ █▀█ █▄▄ █ █▀▄";


		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < text2.length() / 2; j++) {
				screen[(i + 24) * ancho + (j + 23)] = text2[i * text2.length() / 2 + j];
			}
		}
		WriteConsoleOutputCharacter(hConsole, screen, alto * ancho, { 0,0 }, &charescritos);
		bool menu1 = 1;//menus para seleccionar que desea hacer el jugador el uno es para jugar otra vez
		bool menu2 = 0;//menu para salir del juego
		if (GetAsyncKeyState((unsigned)'W') || menu1) {
			menu1 = true;
			while (menu1) {
				std::wstring text4 = L"->->";

				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < text4.length() / 2; j++) {
						screen[(i + 24) * ancho + (j + 21)] = ' ';
					}
				}
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < text4.length() / 2; j++) {
						screen[(i + 20) * ancho + (j + 21)] = text4[i * text4.length() / 2 + j];
					}
				}
				if (GetAsyncKeyState(VK_RETURN)) {

					return true;
					menu1 = false;
				}
				if (GetAsyncKeyState(((unsigned)'S'))) {
					menu1 = false;
				}
				WriteConsoleOutputCharacter(hConsole, screen, alto * ancho, { 0,0 }, &charescritos);
			}
		}
		if (GetAsyncKeyState((unsigned)'S') || menu2) {
			menu2 = true;
			while (menu2) {
				std::wstring text4 = L"->->";

				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < text4.length() / 2; j++) {
						screen[(i + 20) * ancho + (j + 21)] = ' ';
					}
				}
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < text4.length() / 2; j++) {
						screen[(i + 24) * ancho + (j + 21)] = text4[i * text4.length() / 2 + j];
					}
				}
				if (GetAsyncKeyState(VK_RETURN))
				{


					return false;
					menu2 = false;

				}
				if (GetAsyncKeyState(((unsigned)'W'))) {
					menu2 = false;
				}
				WriteConsoleOutputCharacter(hConsole, screen, alto * ancho, { 0,0 }, &charescritos);
			}
		}
		WriteConsoleOutputCharacter(hConsole, screen, alto * ancho, { 0,0 }, &charescritos);

	}

	return true;
}

bool menu1(wchar_t* screen, int ancho, int alto, HANDLE hConsole, DWORD charescritos)
{
	bool menu = true;
	while (menu)
	{
		std::wstring	text = L"░██████╗░░█████╗░███╗░░░███╗███████╗  ░█████╗░██╗░░░██╗███████╗██████╗░";
		text += L"██╔════╝░██╔══██╗████╗░████║██╔════╝  ██╔══██╗██║░░░██║██╔════╝██╔══██╗";
		text += L"██║░░██╗░███████║██╔████╔██║█████╗░░  ██║░░██║╚██╗░██╔╝█████╗░░██████╔╝";
		text += L"██║░░╚██╗██╔══██║██║╚██╔╝██║██╔══╝░░  ██║░░██║░╚████╔╝░██╔══╝░░██╔══██╗";
		text += L"╚██████╔╝██║░░██║██║░╚═╝░██║███████╗  ╚█████╔╝░░╚██╔╝░░███████╗██║░░██║";
		text += L"░╚═════╝░╚═╝░░╚═╝╚═╝░░░░░╚═╝╚══════╝  ░╚════╝░░░░╚═╝░░░╚══════╝╚═╝░░╚═╝";




		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < text.length() / 6; j++) {
				screen[(i + 10) * ancho + (j + 23)] = text[i * text.length() / 6 + j];
			}
		}

		std::wstring	text2 = L"░░█ █░█ █▀▀ ▄▀█ █▀█   █▀█ ▀█▀ █▀█ ▄▀█   █░█ █▀▀ ▀█";
		text2 += L"█▄█ █▄█ █▄█ █▀█ █▀▄   █▄█ ░█░ █▀▄ █▀█   ▀▄▀ ██▄ █▄";


		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < text2.length() / 2; j++) {
				screen[(i + 20) * ancho + (j + 23)] = text2[i * text2.length() / 2 + j];
			}
		}
		text2 = L"█▀ ▄▀█ █░░ █ █▀█";
		text2 += L"▄█ █▀█ █▄▄ █ █▀▄";


		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < text2.length() / 2; j++) {
				screen[(i + 24) * ancho + (j + 23)] = text2[i * text2.length() / 2 + j];
			}
		}
		WriteConsoleOutputCharacter(hConsole, screen, alto * ancho, { 0,0 }, &charescritos);
		bool menu1 = 1;//menus para seleccionar que desea hacer el jugador el uno es para jugar otra vez
		bool menu2 = 0;//menu para salir del juego
		if (GetAsyncKeyState((unsigned)'W') || menu1) {
			menu1 = true;
			while (menu1) {
				std::wstring text4 = L"->->";

				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < text4.length() / 2; j++) {
						screen[(i + 24) * ancho + (j + 21)] = ' ';
					}
				}
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < text4.length() / 2; j++) {
						screen[(i + 20) * ancho + (j + 21)] = text4[i * text4.length() / 2 + j];
					}
				}
				if (GetAsyncKeyState(VK_RETURN)) {
					menu = false;
					return true;
					menu1 = false;
				}
				if (GetAsyncKeyState(((unsigned)'S'))) {
					menu1 = false;
				}
				WriteConsoleOutputCharacter(hConsole, screen, alto * ancho, { 0,0 }, &charescritos);
			}
		}
		if (GetAsyncKeyState((unsigned)'S') || menu2) {
			menu2 = true;
			while (menu2) {
				std::wstring text4 = L"->->";

				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < text4.length() / 2; j++) {
						screen[(i + 20) * ancho + (j + 21)] = ' ';
					}
				}
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < text4.length() / 2; j++) {
						screen[(i + 24) * ancho + (j + 21)] = text4[i * text4.length() / 2 + j];
					}
				}
				if (GetAsyncKeyState(VK_RETURN))
				{
					menu = false;
					menu2 = false;
					return false;//sale del programa
					
				}
				if (GetAsyncKeyState(((unsigned)'W'))) {
					menu2 = false;
				}
				WriteConsoleOutputCharacter(hConsole, screen, alto * ancho, { 0,0 }, &charescritos);
			}
		}
		WriteConsoleOutputCharacter(hConsole, screen, alto * ancho, { 0,0 }, &charescritos);
	}

	return true;
}


