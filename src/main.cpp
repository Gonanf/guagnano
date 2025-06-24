#include<stdio.h>
#include<iostream>
#include<limits>

bool CIN_FAILSAFE(){
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
	if(std::cin.fail()){std::cin.clear();std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');std::cout<<"Mal valor\n";return true;} return false;}

/*3 estados civiles*/
#define WAIT_VALID_INPUT(message, min, max, var) do{std::cout<<message<<std::endl;std::cin >>var; CIN_FAILSAFE();}while(var < min || var > max)

#define MAX_STUDENTS 40
#define MAX_CHARS 25

	struct alumno{
	char nombre[MAX_CHARS];
	char apellido[MAX_CHARS];
	int edad;
	int materias_aprovadas;
	char estado_civil;
	char sexo;
	};	


float obtener_promedio(struct alumno al[MAX_STUDENTS], int cant){
	float sum = 0;
	for (int i = 0; i < cant; i++){
	 sum+=al[i].edad;
	}
	return sum / cant;
}

void print_alumno(struct alumno al){
	std::cout << "----------------\n" << "Nombre: " << al.nombre << "\nApellido: " << al.apellido << "\nEdad: " << al.edad << "\nMaterias aprobadas: " << al.materias_aprovadas << "\nEstado civil: " << al.estado_civil << "\nSexo: " << al.sexo << std::endl << "------------\n";
return;
}


int main(){
	system("clear");	
	struct alumno alumnos[MAX_STUDENTS];
	int cont = 0;
	while (true){
		int menu;
		WAIT_VALID_INPUT("----------\n-Ingresa 1 para añadir un alumno\n-Ingresa 2 para ver el listado de alumnos\n-Ingresa 3 para ver el listado de nombres y apellidos\n-Ingresa 4 para obtener el promedio de edad\n-Ingresa 5 para salir\n----------",1,5,menu);
		system("clear");
			switch(menu){
				case 1:
					{	
		if (cont >= MAX_STUDENTS){
			goto error;
		}
		std::cout << "Ingresar nombre" << std::endl;
		do{std::cin >> alumnos[cont].nombre;}while(CIN_FAILSAFE());
		std::cout << "Ingresar apellido" << std::endl;
		do{std::cin >> alumnos[cont].apellido;}while(CIN_FAILSAFE());
		std::cout<< "Ingresar edad" << std::endl;
		do{std::cin >> alumnos[cont].edad;}while(CIN_FAILSAFE());
int des;
WAIT_VALID_INPUT("Ingrese estado civil\n-1 para soltero\n-2 para casado\n-3 para viudo",1,3,des);
		switch(des){
			case 1:
				alumnos[cont].estado_civil = 'S';
				break;
			case 2:
				
				alumnos[cont].estado_civil = 'C';
				break;
			case 3:
				alumnos[cont].estado_civil= 'V';
				break;
			default:
				break;

		}
		des = 0;
		WAIT_VALID_INPUT("Ingrese sexo\n-1 para masculino\n-2 para femenino",1,2,des);
			switch(des){
				case 1:
					alumnos[cont].sexo = 'M';
				break;
				case 2:

					alumnos[cont].sexo = 'F';
					break;
				default:
					break;
			}
		
		std::cout << "Ingresar cantidad materias aprobadas" << std::endl;
		do{std::cin >> alumnos[cont].materias_aprovadas;}while(CIN_FAILSAFE());
	cont++;
}
					break;
				case 2:
					for (int i = 0; i < cont; i++){
						print_alumno(alumnos[i]);
					}
					break;

					case 3:
					if (cont <= 0){
						std::cout << "No hay registros\n";
					continue;}
					for (int i = 0; i < cont; i++){
						std::cout << "----------------\n" << alumnos[i].nombre << " " << alumnos[i].apellido << "\n----------------\n";
					}
					break;
				case 4:
					if (cont <= 0){
					std::cout << "No hay registros\n";
					continue;
					}
					std::cout << "El promedio es: " << obtener_promedio(alumnos,cont) << std::endl;
					break;
				case 5:
				return 0;
				break;

			}
		}
return 0;

error:
	std::cout << "Error: Max students reached" << std::endl;
	return -1;
}

