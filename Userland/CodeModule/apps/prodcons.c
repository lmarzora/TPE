void consumir();
void producir();

char * prods[5] = {"X", "X", "X","X", "X"};
int indexCons = 0;
int indexProd = 0;
int cont = 1;

int prodcons_main(){

	printLn("");
	printLn("Creando semaforos");
	newSemaphore("prod", 5);
	newSemaphore("cons", 0);
	
	printLn("Creando productor\n");
	newProcess("productor", &producir, 0, 0, 0);

	printLn("************* Producer-Consumer ************");
	printLn("Los espacios _ estan disponibles para producir, ");
	printLn("mientras que los que tienen X ya tienen un producto");
	printLn("-> El sistema va a producir automaticamente");
	printLn("-> Apreta Enter para consumir, o escribi exit para salir\n");

	printProds("");

	char * txt;
			
	txt = getLn();	
	while(cmpstr(txt, "exit") == 0){
		consumir();
		txt = getLn();
	}

	cont = 0;

	printLn("");
	print("$ ");

}

void consumir(){
	waitSemaphore("prod");
	prods[indexCons%5] = "_";
	indexCons++;
	printProds("  -> Consumidor");
	signalSemaphore("cons");
		
}

void producir(){
	int y = 0;
	while(cont){
		if(y>40000000){
			waitSemaphore("cons");
			prods[indexProd%5] = "X";
			indexProd++;
			printProds("  -> Productor");
			signalSemaphore("prod");
			y=0;
		}
		y++;
	}
	
}

void printProds(char * dueno){
	int i;
	print("| ");
	for(i=0; i<5; i++){
		print(prods[i]);
		print(" | ");
	}
	print(dueno);
	printLn("");
}