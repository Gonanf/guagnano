#include <stdio.h>
#include <iostream>
#include <limits>

bool CIN_FAILSAFE()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Mal valor\n";
		return true;
	}
	return false;
}

/*3 estados civiles*/
#define WAIT_VALID_INPUT(message, min, max, var) \
	do                                           \
	{                                            \
		std::cout << message << std::endl;       \
		std::cin >> var;                         \
		CIN_FAILSAFE();                          \
	} while (var < min || var > max)

#define MAX_STUDENTS 40
#define MAX_CHARS 25

int main()
{
	system("clear");

	char nombre[MAX_STUDENTS][MAX_CHARS];
	char apellido[MAX_STUDENTS][MAX_CHARS];
	int edad_materias_aprovadas[2][MAX_STUDENTS];
	char estado_civil_sexo[2][MAX_STUDENTS];
	int cont = 0;
	while (true)
	{
		int menu;
		WAIT_VALID_INPUT("----------\n-Ingresa 1 para añadir un alumno\n-Ingresa 2 para ver el listado de alumnos\n-Ingresa 3 para ver el listado de nombres y apellidos\n-Ingresa 4 para obtener el promedio de edad\n-Ingresa 5 para salir\n----------", 1, 5, menu);
		system("clear");
		switch (menu)
		{
		case 1:
		{
			if (cont >= MAX_STUDENTS)
			{
				std::cerr << "Limite de alumnos alcanzado\n";
				return -1;
			}
			do
			{
				std::cout << "Ingresar nombre" << std::endl;

				std::cin >> nombre[cont];
			} while (CIN_FAILSAFE());
			do
			{
				std::cout << "Ingresar apellido" << std::endl;

				std::cin >> apellido[cont];
			} while (CIN_FAILSAFE());
			do
			{
				std::cout << "Ingresar edad" << std::endl;

				std::cin >> edad_materias_aprovadas[0][cont];
			} while (CIN_FAILSAFE());
			int des;
			WAIT_VALID_INPUT("Ingrese estado civil\n-1 para soltero\n-2 para casado\n-3 para viudo", 1, 3, des);
			switch (des)
			{
			case 1:
				estado_civil_sexo[0][cont] = 'S';
				break;
			case 2:
				estado_civil_sexo[0][cont] = 'C';
				break;
			case 3:
				estado_civil_sexo[0][cont] = 'V';
				break;
			default:
				break;
			}
			des = 0;
			WAIT_VALID_INPUT("Ingrese sexo\n-1 para masculino\n-2 para femenino", 1, 2, des);
			estado_civil_sexo[1][cont] = des == 1 ? 'M' : 'F';

			do
			{
				std::cout << "Ingresar cantidad materias aprobadas" << std::endl;

				std::cin >> edad_materias_aprovadas[1][cont];
			} while (CIN_FAILSAFE());
			cont++;
		}
		break;
		case 2:
			if (cont <= 0)
			{
				std::cout << "No hay registros\n";
				continue;
			}
			for (int i = 0; i < cont; i++)
			{
				std::cout << "----------------\n"
						  << "Nombre: " << nombre[i] << "\nApellido: " << apellido[i] << "\nEdad: " << edad_materias_aprovadas[0][i] << "\nMaterias aprobadas: " << edad_materias_aprovadas[1][i] << "\nEstado civil: " << estado_civil_sexo[0][i] << "\nSexo: " << estado_civil_sexo[1][i] << std::endl
						  << "------------\n";
			}
			break;

		case 3:
			if (cont <= 0)
			{
				std::cout << "No hay registros\n";
				continue;
			}
			for (int i = 0; i < cont; i++)
			{
				std::cout << "----------------\n"
						  << nombre[i] << " " << apellido[i] << "\n----------------\n";
			}
			break;
		case 4:
		{
			if (cont <= 0)
			{
				std::cout << "No hay registros\n";
				continue;
			}
			float sum = 0;
			for (int i = 0; i < cont; i++)
			{
				sum += edad_materias_aprovadas[0][i];
			}
			std::cout << "El promedio es: " << sum / cont << std::endl;
		}
		break;
		case 5:
			return 0;
			break;
		}
	}
	return 0;
}
