#include <stdio.h>
#include <iostream>
#include <limits>
#include"sqlite3.h"


int callback(void* param, int argc, char** argv, char** col){
	std::cout << "------------------------------" << std::endl;
	for (int i = 0; i < argc; i++){
		std::cout << col[i] << " : " << argv[i] << std::endl;
	}
	std::cout << "------------------------------" << std::endl;
	return 0;
}

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

struct alumno
{
	char nombre[MAX_CHARS];
	char apellido[MAX_CHARS];
	int edad;
	int materias_aprovadas;
	char estado_civil;
	char sexo;
};

float obtener_promedio(struct alumno al[MAX_STUDENTS], int cant)
{
	float sum = 0;
	for (int i = 0; i < cant; i++)
	{
		sum += al[i].edad;
	}
	return sum / cant;
}

void print_alumno(struct alumno al)
{
	std::cout << "----------------\n"
			  << "Nombre: " << al.nombre << "\nApellido: " << al.apellido << "\nEdad: " << al.edad << "\nMaterias aprobadas: " << al.materias_aprovadas << "\nEstado civil: " << al.estado_civil << "\nSexo: " << al.sexo << std::endl
			  << "------------\n";
	return;
}

int add_alumno(sqlite3* db)
{
	std::string nombre;
	std::string apellido;
	int edad;
	int materias_aprovadas;
	char estado_civil;
	char sexo;
	
	do
	{
		std::cout << "Ingresar nombre" << std::endl;

		std::cin >> nombre;
	} while (CIN_FAILSAFE());
	do
	{
		std::cout << "Ingresar apellido" << std::endl;

		std::cin >> apellido;
	} while (CIN_FAILSAFE());
	do
	{
		std::cout << "Ingresar edad" << std::endl;

		std::cin >> edad;
	} while (CIN_FAILSAFE());
	int des;
	WAIT_VALID_INPUT("Ingrese estado civil\n-1 para soltero\n-2 para casado\n-3 para viudo", 1, 3, des);
	switch (des)
	{
	case 1:
		estado_civil = 'S';
		break;
	case 2:
		estado_civil = 'C';
		break;
	case 3:
		estado_civil = 'V';
		break;
	default:
		break;
	}
	des = 0;
	WAIT_VALID_INPUT("Ingrese sexo\n-1 para masculino\n-2 para femenino", 1, 2, des);
	sexo = des == 1 ? 'M' : 'F';

	do
	{
		std::cout << "Ingresar cantidad materias aprobadas" << std::endl;

		std::cin >> materias_aprovadas;
	} while (CIN_FAILSAFE());
	std::string query = "INSERT INTO alumnos(nombre,apellido,edad,materias_aprovadas,estado_civil,sexo) VALUES('"+nombre+"','"+apellido+"',"+std::to_string(edad)+","+std::to_string(materias_aprovadas)+",'"+estado_civil+"','"+sexo+"');";
	char* errmsg;
	if (sqlite3_exec(db,query.c_str(),NULL,NULL,&errmsg) != 0){
		std::cerr << "Error inserting alumnos: " << std::endl <<" "<<errmsg << std::endl;
		return -1;
	}
	return 0;
}

int main()
{
	system("clear");
	/**
	 * DB Setup
	 */
sqlite3* db;


	if (sqlite3_open("amongas.db",&db)){
		std::cerr << "Error opening the database" << std::endl;
		return -1;
	}

	char* errmsg;

	if (sqlite3_exec(db,"CREATE TABLE IF NOT EXISTS alumnos (id INTEGER PRIMARY KEY,nombre TEXT, apellido TEXT, edad INTEGER, materias_aprovadas INTEGER, estado_civil varchar(1),sexo varchar(1));",NULL,NULL,&errmsg) != 0){
		std::cerr << "Error creating a table alumnos: " << std::endl <<" "<<errmsg << std::endl;
		return -1;
	}

	struct alumno alumnos[MAX_STUDENTS];
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
			add_alumno(db);
		}
		break;
		case 2:
			if (sqlite3_exec(db,"SELECT * FROM alumnos;",callback,NULL,&errmsg) != 0){
					std::cerr << "Error selecting from alumnos: " << std::endl <<" "<<errmsg << std::endl;
					return -1;
				}
			break;

		case 3:
			if (sqlite3_exec(db,"SELECT nombre, apellido FROM alumnos;",callback,NULL,&errmsg) != 0){
					std::cerr << "Error selecting from alumnos: " << std::endl <<" "<<errmsg << std::endl;
					return -1;
				}
			break;
		case 4:
			if (sqlite3_exec(db,"SELECT avg(edad) FROM alumnos;",callback,NULL,&errmsg) != 0){
					std::cerr << "Error selecting from alumnos: " << std::endl <<" "<<errmsg << std::endl;
					return -1;
				}
			break;
		case 5:
			return 0;
			break;
		}
	}
	return 0;
}
