#include <stdio.h>
#include <iostream>
#include "sqlite3.h"

/**@main */

/**
 * Ejercicio: Hallar el número total de los alumnos presentes adjuntando un listado que contenga:
 * - Nombre
 * - Apellido
 * - Edad
 * - Estado Civil
 * - Sexo
 * - Cantidad de materias aprobadas
 * Calcular el promedio de edad e intregrar un Menu
 */

/***************************************
 * Incorporar Matrices (Actividad anterior)
 * Incorporar estructuras (Actividad anterior)
 * Incorporar funciones (Actividad anterior)
 * Incorporar conexion a bases de datos (Actividad actual)
 ***************************************/

int callback(void *param, int argc, char **argv, char **col)
{
	std::cout << "------------------------------" << std::endl;
	for (int i = 0; i < argc; i++)
	{
		std::cout << col[i] << " : " << argv[i] << std::endl;
	}
	std::cout << "------------------------------" << std::endl;
	return 0;
}

bool CIN_FAILSAFE()
{
	std::cin.ignore(256, '\n');
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(256, '\n');
		std::cout << "Mal valor\n";
		return true;
	}
	return false;
}

#define WAIT_VALID_INPUT(message, min, max, var) \
	do                                           \
	{                                            \
		std::cout << message << std::endl;       \
		std::cin >> var;                         \
		CIN_FAILSAFE();                          \
	} while (var < min || var > max)

#define MAX_CHARS 25

/**
 * @brief Funcion para guiar al usuario en el proceso de la insercion de un alumno nuevo
 * 
 * @param db La base de datos al cual se le haran las modificaciones
 * @return int Devuelve 0 si no hay problemas, devuelve -1 si hubo un error
 */
int add_alumno(sqlite3 *db)
{
	struct
	{
		char nombre[MAX_CHARS];
		char apellido[MAX_CHARS];
		int edad;
		int materias_aprovadas;
		char estado_civil;
		char sexo;
	} alumno;

	do
	{
		std::cout << "Ingresar nombre" << std::endl;

		std::cin >> alumno.nombre;
	} while (CIN_FAILSAFE());
	do
	{
		std::cout << "Ingresar apellido" << std::endl;

		std::cin >> alumno.apellido;
	} while (CIN_FAILSAFE());
	do
	{
		std::cout << "Ingresar edad" << std::endl;

		std::cin >> alumno.edad;
	} while (CIN_FAILSAFE());
	int des;
	WAIT_VALID_INPUT("Ingrese estado civil\n-1 para soltero\n-2 para casado\n-3 para viudo", 1, 3, des);
	switch (des)
	{
	case 1:
		alumno.estado_civil = 'S';
		break;
	case 2:
		alumno.estado_civil = 'C';
		break;
	case 3:
		alumno.estado_civil = 'V';
		break;
	default:
		break;
	}
	des = 0;
	WAIT_VALID_INPUT("Ingrese sexo\n-1 para masculino\n-2 para femenino", 1, 2, des);
	alumno.sexo = des == 1 ? 'M' : 'F';

	do
	{
		std::cout << "Ingresar cantidad materias aprobadas" << std::endl;

		std::cin >> alumno.materias_aprovadas;
	} while (CIN_FAILSAFE());
	sqlite3_stmt *statement;
	sqlite3_prepare_v2(db, "INSERT INTO alumnos(id_al,nombre,apellido,edad,materias_aprovadas,estado_civil,sexo) SELECT " //! Se utiliza SELECT en vez de VALUES por que SELECT permite hacer consultas dentro de los parametros
	"(SELECT MAX(id) + 1 FROM (SELECT MAX(id_al) AS id FROM alumnos UNION ALL SELECT MAX(id_reg) as id FROM alumnos_reg))" //! Selecciona la mayor ID posible del mayor ID de alumnos o de la tabla de eliminados
	",?,?,?,?,?,?;", -1, &statement, NULL);
	sqlite3_bind_text(statement, 1, alumno.nombre, 25, SQLITE_STATIC);
	sqlite3_bind_text(statement, 2, alumno.apellido, 25, SQLITE_STATIC);
	sqlite3_bind_int(statement, 3, alumno.edad);
	sqlite3_bind_int(statement, 4, alumno.materias_aprovadas);
	sqlite3_bind_text(statement, 5, &alumno.estado_civil, 1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 6, &alumno.sexo, 1, SQLITE_STATIC);

	if (sqlite3_step(statement) != SQLITE_DONE)
	{
		std::cerr << "Error inserting alumnos: " << std::endl
				  << " " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}
	return 0;
}

/**
 * @brief Funcion para guiar al usuario a eliminar un alumno
 * 
 * @param db La base de datos al cual se le haran las modificaciones
 * @return int Devuelve 0 si no hay problemas, devuelve -1 si hubo un error
 */
int remove_alumno(sqlite3 *db)
{
	int id;
	char *errmsg;
	if (sqlite3_exec(db, "SELECT * FROM alumnos;", callback, NULL, &errmsg) != 0)
	{
		std::cerr << "Error selecting from alumnos: " << std::endl
				  << " " << errmsg << std::endl;
		return -1;
	}
	do
	{
		std::cout << "Ingresar la ID del alumno a eliminar: ";
		std::cin >> id;
	} while (CIN_FAILSAFE());

	sqlite3_stmt *statement;
	// Migracion, se inserta en la tabla de registros de alumnos retirados utilizando todos los datos del registro de la tabla alumnos que tenga la ID especificada
	sqlite3_prepare_v2(db, "INSERT INTO alumnos_reg SELECT id_al,nombre,apellido,edad,materias_aprovadas,estado_civil,sexo  FROM alumnos WHERE id_al = ?;", -1, &statement, NULL);
	sqlite3_bind_int(statement, 1, id);
	sqlite3_bind_int(statement, 2, id);
	if (sqlite3_step(statement) != SQLITE_DONE)
	{
		std::cerr << "Error inserting alumnos_reg: " << std::endl
				  << " " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}
	if (sqlite3_reset(statement) != SQLITE_OK)
	{
		std::cerr << "Error reseting the statement: " << std::endl
				  << " " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	sqlite3_prepare_v2(db, "DELETE FROM alumnos WHERE id_al = ?;", -1, &statement, NULL);
	sqlite3_bind_int(statement, 1, id);

	if (sqlite3_step(statement) != SQLITE_DONE)
	{
		std::cerr << "Error deleting alumno: " << std::endl
				  << " " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}
	if (sqlite3_finalize(statement) != SQLITE_OK)
	{
		std::cerr << "Error finalizing the statement: " << std::endl
				  << " " << sqlite3_errmsg(db) << std::endl;
		return -1;
	}

	return 0;
}

int main()
{
	sqlite3 *db;

	if (sqlite3_open("amongas.db", &db))
	{
		std::cerr << "Error opening the database" << std::endl;
		return -1;
	}

	char *errmsg;

	if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS alumnos (id_al INTEGER PRIMARY KEY,nombre TEXT, apellido TEXT, edad INTEGER, materias_aprovadas INTEGER, estado_civil varchar(1),sexo varchar(1));", NULL, NULL, &errmsg) != 0)
	{
		std::cerr << "Error creating a table alumnos: " << std::endl
				  << " " << errmsg << std::endl;
		return -1;
	}

	// En esta situacion añadiria una fecha de eliminacion, pero no me quiero adentrar mucho en el tema de los NTP
	if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS alumnos_reg (id_reg INTEGER PRIMARY KEY,nombre TEXT, apellido TEXT, edad INTEGER, materias_aprovadas INTEGER, estado_civil varchar(1),sexo varchar(1));", NULL, NULL, &errmsg) != 0)
	{
		std::cerr << "Error creating a table alumnos_reg: " << std::endl
				  << " " << errmsg << std::endl;
		return -1;
	}

	int cont = 0;
	while (true)
	{
		int menu;
		WAIT_VALID_INPUT("----------\
		\n-Ingresa 1 para añadir un alumno \
		\n-Ingresa 2 para ver el listado de alumnos\n-Ingresa 3 para ver el listado de nombres y apellidos\
		\n-Ingresa 4 para obtener el promedio de edad\
		\n-Ingresa 5 para eliminar un alumno\
		\n-Ingresa 6 para ver el listado de alumnos eliminados\
		\n-Ingresa 7 para salir\
		\n----------", 1, 7, menu);
		system("clear");
		switch (menu)
		{
		case 1:
		{
			add_alumno(db);
		}
		break;
		case 2:
			if (sqlite3_exec(db, "SELECT * FROM alumnos;", callback, NULL, &errmsg) != 0)
			{
				std::cerr << "Error selecting from alumnos: " << std::endl
						  << " " << errmsg << std::endl;
				return -1;
			}
			break;

		case 3:
			if (sqlite3_exec(db, "SELECT nombre, apellido FROM alumnos;", callback, NULL, &errmsg) != 0)
			{
				std::cerr << "Error selecting from alumnos: " << std::endl
						  << " " << errmsg << std::endl;
				return -1;
			}
			break;
		case 4:
			if (sqlite3_exec(db, "SELECT avg(edad) FROM alumnos;", callback, NULL, &errmsg) != 0)
			{
				std::cerr << "Error selecting from alumnos: " << std::endl
						  << " " << errmsg << std::endl;
				return -1;
			}
			break;
		case 5:
			remove_alumno(db);
			break;
		case 6:
			if (sqlite3_exec(db, "SELECT * FROM alumnos_reg;", callback, NULL, &errmsg) != 0)
			{
				std::cerr << "Error selecting from alumnos_reg: " << std::endl
						  << " " << errmsg << std::endl;
				return -1;
			}
			break;
		case 7:
			return 0;
			break;
		}
	}
	return 0;
}
