void consumir();
void producir();
void printProds(char*);

char * prods[5];
int indexCons;
int indexProd;
int cont;

int prodcons_main(){
  	int i;
  	for(i=0; i<5; i++)
  		prods[i] = "X";
  	indexCons = 0;
  	indexProd = 0;
  	cont = 1;

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

	deleteSemaphore("cons");
	deleteSemaphore("prod");

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
	while(cont){
		waitSemaphore("cons");
		sleep(36);//esperar un poco antes de producir
		if(!cont) return;
		prods[indexProd%5] = "X";
		indexProd++;
		printProds("  -> Productor");
		signalSemaphore("prod");
		
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